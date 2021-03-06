#!/usr/local/bin/bash
# Setup operating system dependent variables:

#
# C compiler:

export  LinuxCC="gcc -g -I/usr/include/tcl"
export  OSF1CC="gcc -g"
export  HPUXCC="gcc -g"
export  DarwinCC="cc -g -Dunix"
export  CYGWINCC="gcc -g"

# C++ compiler

export LinuxCPP="g++ -g -I/usr/include/tcl"
export OSF1CPP="g++ -g"
export HPUXCPP="g++ -g"
export DarwinCPP="c++ -g -Dunix"
export CYGWINCPP="g++ -g -Dunix"

# Who does the LD?

export LinuxLD="g++ -L/usr/X11/lib -L/usr/X11R6/lib"
export OSF1LD="g++ -L/usr/X11/lib"
export HPUXLD="g++ -L/usr/X11/lib"
export DarwinLD="c++ -undefined warning -L/usr/X11R6/lib"
export CYGWINLD="g++ -L/usr/X11R6/lib -L/usr/local/tcl/lib"

# Where are the Motif includes?

export LinuxMOTIFINCLUDES="-I/usr/X11/include -I/usr/X11R6/include"
export OSF1MOTIFINCLUDES="-I/usr/X11/include"
export HPUXMOTIFINCLUDES="-I/usr/X11/include"
export DarwinMOTIFINCLUDES="-I/usr/X11R6/include"
export CYGWINMOTIFINCLUDES="-I/usr/X11R6/include"


#  Fortran compiler for Xamine Fortran Bindings:
#  This is supported for now on:
#   Linux: Intel fortran compiler which universities
#          can get for free from intel.
#   Tru64: Digital Fortran which in general needs to be
#          bought Makefile will check the existence of these
#          and on the basis of that decide whether or not
#          to build the fortran bindings.

export LinuxF77="ifc -w90 -w95"
export OSF1F77="/usr/bin/f77 -check bounds"
