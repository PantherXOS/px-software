(define-module (px-software-update-check)
  #:use-module (guix store)
  #:use-module (guix packages)
  #:use-module (guix scripts)
  #:use-module (ice-9 match)
  #:use-module (ice-9 vlist)
  #:use-module (srfi srfi-1)
  #:use-module (srfi srfi-11)
  #:use-module (gnu packages)
  #:use-module (guix packages)
  #:use-module (guix ui)
  #:use-module (guix utils)
  #:use-module (guix discovery)
  #:use-module (guix memoization)
  #:use-module (guix profiles)
  #:use-module (guix describe)
  #:export (check-profile))

(define (cache-is-authoritative?)
  "Return true if the pre-computed package cache is authoritative.  It is not
authoritative when entries have been added via GUIX_PACKAGE_PATH or '-L'
flags."
  (equal? (%package-module-path)
          (append %default-package-module-path
                  (package-path-entries))))

(define* (fold-packages proc init
                        #:optional
                        (modules (all-modules (%package-module-path)
                                              #:warn
                                              warn-about-load-error))
                        #:key (select? (negate hidden-package?)))
  "Call (PROC PACKAGE RESULT) for each available package defined in one of
MODULES that matches SELECT?, using INIT as the initial value of RESULT.  It
is guaranteed to never traverse the same package twice."
  (fold-module-public-variables (lambda (object result)
                                  (if (and (package? object) (select? object))
                                      (proc object result)
                                      result))
                                init
                                modules))

(define %package-cache-file
  ;; Location of the package cache.
  "/lib/guix/package.cache")

(define load-package-cache
  (mlambda (profile)
    "Attempt to load the package cache.  On success return a vhash keyed by
package names.  Return #f on failure."
    (match profile
      (#f #f)
      (profile
       (catch 'system-error
         (lambda ()
           (define lst
             (load-compiled (string-append profile %package-cache-file)))
           (fold (lambda (item vhash)
                   (match item
                     (#(name version module symbol outputs
                             supported? deprecated?
                             file line column)
                      (vhash-cons name item vhash))))
                 vlist-null
                 lst))
         (lambda args
           (if (= ENOENT (system-error-errno args))
               #f
               (apply throw args))))))))

(define find-packages-by-name/direct              ;bypass the cache
  (let ((packages (delay
                    (fold-packages (lambda (p r1)
                                     (vhash-cons (package-name p) p r1))
                                   vlist-null)))
        (version>? (lambda (p1 p2)
                     (version>? (package-version p1) (package-version p2)))))
    (lambda* (name #:optional version)
      "Return the list of packages with the given NAME.  If VERSION is not #f,
then only return packages whose version is prefixed by VERSION, sorted in
decreasing version order."
      (let ((matching (sort (vhash-fold* cons '() name (force packages))
                            version>?)))
        (if version
            (filter (lambda (package)
                      (version-prefix? version (package-version package)))
                    matching)
            matching)))))

(define (cache-lookup cache name)
  "Lookup package NAME in CACHE.  Return a list sorted in increasing version
order."
  (define (package-version<? v1 v2)
    (version>? (vector-ref v2 1) (vector-ref v1 1)))

  (sort (vhash-fold* cons '() name cache)
        package-version<?))


(define current-channel-entries
  (mlambda ()
    "Return manifest entries corresponding to extra channels--i.e., not the
'guix' channel."
    (remove (lambda (entry)
              (string=? (manifest-entry-name entry) "guix"))
            (manifest-entries (profile-manifest (string-append (getenv "HOME") "/.config/guix/current"))))))

(define* (find-packages-by-name name #:optional version)
  "Return the list of packages with the given NAME.  If VERSION is not #f,
then only return packages whose version is prefixed by VERSION, sorted in
decreasing version order."
  (define cache
    (load-package-cache (current-profile)))
  (if (and (cache-is-authoritative?) cache)
      (match (cache-lookup cache name)
        (#f #f)
       ((#(_ versions modules symbols _ _ _ _ _ _) ...)
         (fold (lambda (version* module symbol result)
                 (if (or (not version)
                         (version-prefix? version version*))
                     (cons (module-ref (resolve-interface module)
                                       symbol)
                           result)
                     result))
               '()
               versions modules symbols)))
      (find-packages-by-name/direct name version)))


(define (find-best-packages-by-name name version)
  "If version is #f, return the list of packages named NAME with the highest
version numbers; otherwise, return the list of packages named NAME and at
VERSION."
  (if version
      (find-packages-by-name name version)
      (match (find-packages-by-name name)
        (()
         '())
        ((matches ...)
         ;; Return the subset of MATCHES with the higher version number.
         (let ((highest (package-version (first matches))))
           (take-while (lambda (p)
                         (string=? (package-version p) highest))
                       matches))))))

;; Prevent Guile 3 from inlining this procedure so we can mock it in tests.
(set! find-best-packages-by-name find-best-packages-by-name)

(define (print-update-info pkg-update-entry)
    (if (car pkg-update-entry)
      (display (string-append (cadr pkg-update-entry) ":" 
                              (caddr pkg-update-entry) ">"
                              (cadddr pkg-update-entry) "\n"))
      'nope))

(define (check-entry entry manifest)
    (define result '(#f))
    (define pkg-entry-update #f)
    (define pkg-entry-name "")
    (define pkg-entry-version "")
    (define pkg-entry-candidate-version "")

    (match entry
      (($ <manifest-entry> name version output (? string? path))
       (set! pkg-entry-name name)
       (set! pkg-entry-version version)
       (set! pkg-entry-candidate-version version)

       (match (find-best-packages-by-name name #f)
         ((pkg . rest)
          (let ((candidate-version (package-version pkg)))
            (match (package-superseded pkg)
              (#f
               (case (version-compare candidate-version version)
                 ((>)
                  (set! pkg-entry-candidate-version candidate-version)
                  (set! pkg-entry-update #t)))))))
         (()
          (warning (G_ "package '~a' no longer exists~%") name)))))

    (fold (lambda (child-entry result-flag)
                (define result-child (check-entry child-entry manifest))
                (if (car result-child)
                   (set! pkg-entry-update (car result-child))
                   'nope))
      #f (manifest-entry-dependencies entry))
    (set! result (cons pkg-entry-candidate-version '()))
    (set! result (cons pkg-entry-version result))
    (set! result (cons pkg-entry-name result))
    (set! result (cons pkg-entry-update result))
    result)

(define (check-manifest manifest)
  "Check if any update available for a manifest"
  (fold (lambda (entry previous)
          (print-update-info (check-entry entry manifest)))
    #f (manifest-entries manifest)))

(define (get-stores-from-manifest manifest)
  (fold (lambda (entry previous)
          (or previous (get-store entry manifest)))
    #f (manifest-entries manifest)))

(define (package-path-entries)
  "Return two values: the list of package path entries to be added to the
package search path, and the list to be added to %LOAD-COMPILED-PATH.  These
entries are taken from the 'guix pull' profile the calling process lives in,
when applicable."
  ;; Filter out Guix itself.
  (unzip2 (map (lambda (entry)
                 (list (string-append (manifest-entry-item entry)
                                      "/share/guile/site/"
                                      (effective-version))
                       (string-append (manifest-entry-item entry)
                                      "/lib/guile/" (effective-version)
                                      "/site-ccache")))
               (current-channel-entries))))

(define %package-module-path
  ;; Search path for package modules.  Each item must be either a directory
  ;; name or a pair whose car is a directory and whose cdr is a sub-directory
  ;; to narrow the search.
  (let*-values (((not-colon)
                 (char-set-complement (char-set #\:)))
                ((environment)
                 (string-tokenize (or (getenv "GUIX_PACKAGE_PATH") "")
                                  not-colon))
                ((channels-scm channels-go)
                 (package-path-entries)))
    ;; Automatically add channels and items from $GUIX_PACKAGE_PATH to Guile's
    ;; search path.  For historical reasons, $GUIX_PACKAGE_PATH goes to the
    ;; front; channels go to the back so that they don't override Guix' own
    ;; modules.
    (set! %load-path
      (append environment %load-path channels-scm))
    (set! %load-compiled-path
      (append environment %load-compiled-path channels-go))

    (make-parameter
     (append environment
             %default-package-module-path
             channels-scm))))

(define* (check-profile #:optional (profile %current-profile))
  "Check if any update is available for a profile"
;  (display (string-append "profile:" profile "\n"))
  (let ((manifest (profile-manifest profile)))
    (check-manifest manifest)))
;      (display "\n"))
