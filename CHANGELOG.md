# Change Log

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)

## [0.3.2]
### Fixed

- Update Installing/Removing/Updating buttons on hover to Status.
- Gathering the debug messages of System Update button.


## [0.3.1]
### Fixed

- Fixing an issue in inserting an empty item at top of the package list after clicking on package in bottom of the lists.


## [0.3.0]
### Changed

- Added `UPDATE ALL` button to User/System upgradable package list view.


## [0.2.9]
### Fixed

- Fixed stacking issue in Software with multiple screenshots.
- Added Count label to In-Progress item.
- SearchBar Improvment.
- DownloadManager added for downloading the images and icons.


## [0.2.8]
### Fixed

- Fixed an issue in parsing the result of guix package search.
- Minor modification in embedded terminal.
- Set timeout fo DB Update Check task.


## [0.2.7]
### Fixed

- Fixed issue in package searching.
- Fixed issee in jumping to the end of list when click on `Install`/`Remove`/`Update`.
- Some minor GUI updates.
- Closing opened `rec` file after reading.


## [0.2.6]
### Fixed

- Fixed issue in browsing while an application is in-progress status.


## [0.2.5]
### Updated

- Checking for update approach updated.
- Updated the Settings button url.


## [0.2.3]
### Updated

- Removed deprecated methods based on new update of `px-gui-library`.


## [0.2.2]
### Fixed

- Fixed an issue in loading the Software DB.
- Creating a temporary `desktop` file in `XDG_DATA_DIR` for refreshing the desktop file after each install/remove.
- Added All System Update.


## [0.2.1]
### Changed

- Updated help button url.


## [0.2.0]
### Changed

- Integrated Help button with `wiki`.


## [0.1.9]
### Fixed and Changed
 
 - DB path relocation.
 - Updated Other Applications layout.
 - Updated the `Categories` view layout.
 - Search in guix profile.
 - Added search icon at the end of the address bar.
 - Fixed issue in showing the remaining space storage.
 

## [0.1.8]
### Fixed and Changed

 - Link `Settings` button with Software Settings GUI.
 - Fixed issue in `Update`/`Remove` other applications.
 - Using QDir for Creating the directories.
 - Image slide show improvement.
 

## [0.1.7]
### Changed

 - Retrieve all installed/user-upgradable/system-upgradable packages in backend and showing in GUI.


## [0.1.6]
### Changed

 - Fixing warning issue in using `endl` from QTextStresm.
 - Registered `px-software` as url scheme handler for openning specific list or specific application page. 


## [0.1.5]
### Changed

 - Remove QProgressIndicator and using PXProgressIndicator of GUI library.


## [0.1.4]
### Changed

 - Update Software icon.
 - Disable Free Disk Space Item in buttom bar for Clicking.


## [0.1.3]
### Changed

 - Integrate with GUI library.


## [0.1.1]
### Changed

 - Initial guix pull (db update) on fresh system.
 - Fix alignment in Category list GUI.
 - Making the category items and package list items hoverable.
 - Hiding `In-Progress` item in left panel while there is no happening.
 - Update GUI (buttons) in `installing`/`removing`/`updating`.
 - Fixing issue in showing the free disk space value in bottom bar.
 - Update `Name` in desktop file to `Software`.
 - Set minimum size for main window.
 - Show error message in case of invalid database path on fresh install.
 

 
