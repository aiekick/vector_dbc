# Introduction

This is a library to access CAN Databases (aka CANdb, aka DBC files) from Vector Informatik.

# Build on Linux (e.g. Debian Testing)

Building under Linux works as usual:

    mkdir build
    cd build
    cmake ..
    make
    make install DESTDIR=..
    make package

# Build on Windows (e.g. Windows 7 64-Bit)

Building under Windows contains the following steps:

* Use cmake-gui
* Set "Where is the source code:" to the root directory.
* Set "Where to build the binaries:" to folder build below the root directory. Eventually create it.
* Configure and Generate
* Open the Visual Studio Solution (.sln) file in the build folder.
* Compile it in Release Configuration.

# Test

Static tests are

* Cppcheck (if OPTION_RUN_CPPCHECK is set)
* CCCC (if OPTION_RUN_CCCC is set)

Dynamic tests are

* Unit tests (if OPTION_RUN_TESTS is set)

The test execution can be triggered using

    make test

# Package

The package generation can be triggered using

    make package

# Repository Structure

The following files are part of the source code distribution:

* src/_project_/
* src/_project_/tests/

The following files are working directories for building and testing:

* build/_project_/

The following files are products of installation and building:

* bin/
* lib/
* share/doc/_project_/
* share/man/
* include/_project_/
