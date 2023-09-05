# nglibc: binary compatible with glibc, with simpler leaner code and better static linking

nglibc aims to extensively overhaul & rewrite the glibc code base whilst retaining backwards binary compatibility.

To leverage work done on other libc's like musl, where implementation is better than glibc, code from other libraries will merge into or replace glibc code.

Criteria for determining implementation choices:
1. glibc binary compatibility to be retained to maximum extent, but obsolete or rarely used older versioned ABIs will be dropped if these are not needed for major modern software packages like Chromium/Electron
2. Simple, easy to understand code is preferred over complex code.  More complex code will only be chosen only if it is critical to overall performance of programs, not just for sake of micro-benchmarks.
3. Modular/standalone code is preferred over highly interdependent code.  Major glibc sub-systems (eg. stdio+stdlib, ctype, string, intl, and iconv) are to compile and run standalone to greatest extent possible, with cross-system interdependencies to make use of public libc APIs rather than private APIs.
4. Locale sensitive APIs are to be minimised, and UTF-8 is likely to be the only supported encoding for stdio, ctype and libintl.
5. nglibc to support both static linking & shared linking (whereas glibc is nearly useless for static linking due to it's bloated code size and excess use of dynamic loading of character encoding converters - much of which is obsolete in today's pervasive adoption of UTF-8)

# Build and installation instructions

To build and install to /usr/, execute the build script as follows:
```
./build /
```
The above script will run configure, make and then make install.

To make and run tests, execute the following:
```
cd glibc-build/
make tests
```

# glibc initial source files

Initial files are forked from glibc-v2.37 (GKISS Linux package manager build), with the following additional modifications
1. Test cases are moved into separate 'tst' sub directories.
2. iconvdata and non supported sysdeps were removed
3. 'io' & 'libio' renamed to 'file' and 'stdio' respectively

The GPL licence of glibc is currently retained for any glibc derived source code.
Over time, it is expected that much of this glibc source code will be replaced with non-GNU code and other licensing options may be possible.

# sh alternatives

This repo contains scripts written for GKISS Linux to remove bash dependencies. Scripts are hosted in this repo to avoid taking up space in grepo.

Though these scripts were developed for GKISS, they should be functional on other linux distributions as well.

Subfolders show which package the scripts are used for.

All files in this repo are licensed under The MIT License, unless otherwise specified so in the file. (Some files are modified GPL Licensed files).
