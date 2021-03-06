#! /usr/bin/sh

# Setup the operating system dependent commands:

# C compiler:

setenv  LinuxCC "gcc -g -I/usr/include/tcl"
setenv  OSF1CC  "gcc -g"
setenv  HPUXCC  "gcc -g"
setenv  DarwinCC "cc -g -Dunix"
setenv  CYGWINCC "gcc -g"

# C++ compiler

setenv LinuxCPP "g++ -g -I/usr/include/tcl"
setenv OSF1CPP  "g++   -g"
setenv HPUXCPP   "g++ -g"
setenv DarwinCPP "c++ -g -Dunix"
setenv CYGWINCPP "g++ -g -Dunix"

# Who does the LD?

setenv LinuxLD  "g++ -L/usr/X11/lib -L/usr/X11R6/lib"
setenv OSF1LD   "g++ -L/usr/X11/lib"
setenv HPUXLD   "g++ -L/usr/X11/lib"
setenv DarwinLD "c++ -undefined warning -L/usr/X11R6/lib"
setenv CYGWINLD "g++ -L/usr/X11/lib -L/usr/local/tcl/lib"

# Where are the Motif includes?

setenv LinuxMOTIFINCLUDES "-I/usr/X11/include -I/usr/X11R6/include"
setenv OSF1MOTIFINCLUDES "-I/usr/X11/include"
setenv HPUXMOTIFINCLUDES "-I/usr/X11/include"
setenv DarwinMOTIFINCLUDES "-I/usr/X11R6/include"
setenv CYGWINMOTIFINCLUDES "-I/usr/X11/include"


#  Fortran compiler for Xamine Fortran Bindings:
#  This is supported for now on:
#   Linux: Intel fortran compiler which universities
#          can get for free from intel.
#   Tru64: Digital Fortran which in general needs to be
#          bought Makefile will check the existence of these
#          and on the basis of that decide whether or not
#          to build the fortran bindings.

setenv LinuxF77 "ifc -w90 -w95"
setenv OSF1F77   "f77 -check bounds"
