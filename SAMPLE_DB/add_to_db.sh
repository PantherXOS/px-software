#!/bin/sh
if [ "$#" -ne 3 ]; then
   echo "Usage: $0 <package> <category> <dbPath>"
   exit 1
fi

package_name=$1
category=$2
dbPath=$3
guix package -s $package_name | recsel -e "name = '$package_name'" > _package.rec

if [ -s _package.rec ]; then
   name=$(recsel -n 1 -P name _package.rec)
   title=$name
   version=$(recsel -n 1 -P version _package.rec)
   description=$(recsel -n 1 -P description _package.rec)
   homepage=$(recsel -n 1 -P homepage _package.rec)
   license=$(recsel -n 1 -P name _package.rec)
   #echo $name $title $version $description $homepage $license $category
   rm _package.rec
   file_name=$package_name".rec"
   > $file_name
   recins -f name -v $name -f title -v $title -f version -v $version -f description -v "$description" -f homepage -v $homepage -f license -v $license -f category -v $category $file_name
   sed -i '1i # -*- mode: rec -*-' $file_name
   mv $file_name $dbPath
   exit 0
fi
rm _package.rec
echo "$package_name not found"
exit 1

