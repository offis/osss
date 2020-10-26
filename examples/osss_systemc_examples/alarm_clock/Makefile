# Copyright (c) 2006 OFFIS e.V., Oldenburg, Germany
# Some rights reserved.
# -------------------------------------------------------------------
# Example Makefile for OSSS simulation library
# -------------------------------------------------------------------
#
# This file is part of the OSSS simulation library.
# Copyright (c) 2006 OFFIS e.V. Institute for Information Technology
#
# See the files COPYING and LICENSE included with the OSSS library
# for copyright and license details. If you have not received these
# files, please contact OFFIS e.V. for further information.
#
# OFFIS e.V.
# Institute for Information Technology
#   Escherweg 2
#   D-26121 Oldenburg
#   Germany
# www  : http://offis.de
# phone: +49 (441) 9722-0
# fax  : +49 (441) 9722-102
# -------------------------------------------------------------------
#
# Author(s): Philipp A. Hartmann
# Version  : $Revision: 1466 $
# Date     : $Date: 2007-08-31 13:32:31 +0200 (Fri, 31 Aug 2007) $
#
# -------------------------------------------------------------------

#
# This is an example Makefile for an OSSS application.
# It uses the generic OSSS Makefile, named Makefile.osss
# in the top-level example directory.
#

#
# Configuration section of this example/benchmark application
# (if a variable is omitted here, a default value is used)
#

# Name of the application (required!)
MODULE:= alarm_clock

# Default parameters to pass to application, if invoked by
# "simulate" target (optional).
#ARGS:=42

# Target architecture
#TARGET_ARCH := linux
#TARGET_ARCH := linux64
#TARGET_ARCH := gccsparcOS5

# OSSS installation path (required)
#
# The variable OSSS_HOME needs to point to the top-level
# directory of the OSSS library installation.
OSSS_HOME ?= ../../..

# SystemC installation (required)
#
# The variable SYSTEMC_HOME is required to determine the
# location of the SystemC installation. If set in the
# environment, this setting is used. If there is a configuration
# file config.mk in the OSSS library directory, the setting
# is inherited from there.
#SYSTEMC_HOME := /path/to/systemc/directory

# The variable SYSTEMC_LIB is required to locate the static
# library file libsystemc.a, matching the current commpiler
# and SystemC headers.
#SYSTEMC_LIB := $(SYSTEMC_HOME)/lib-$(TARGET_ARCH)

#
# Source files of the current application
#

# source file extension
#
# Set the extension of C++ source files.
# Defaults to "<name>.cpp"
SRCEXT := cc

# The actual source code files can be specified via a list of
# source directories (in the variable SRCDIRS) OR as a list of
# .cpp-files (in the variable SRCS). If set, the variable SRCS takes
# precedence, otherwise, all .cpp-files in SRCDIRS are taken as
# input files (see also: $(SRCEXT) above).
#
# Source-Directories
#SRCDIRS := .
# Source files
#SRCS := example1.cpp example2.cpp

#
# Various settings
#

# Verbose compilation output? (yes,no)
#VERBOSE=no

# Include debug symbols? (yes,no)
DEBUG=yes

# Include profiling information (yes,no)
#PROFILE=no

# Use different OSSS library flavour to link against
# Only required, if you use a specific flavoured
# version of the OSSS library.
#OSSS_LIBNAME := osss

# set additional defines
#
# This variable can hold additional command-line defines, that
# should be passed to the compiler e.g "-DSC_INCLUDE_FX=1".
#EXTRA_DEFINES :=

# additional include directories
#EXTRA_INCLUDES := -I/some/dir

# additional library directories and link directives
#EXTRA_LIBDIRS := -L/path/to/my/lib/dir
#EXTRA_LIBS    := -lmylib

# Compiler to use
#
# Inherits setting of simulation library config, if possible.
# Otherwise defaults to g++.
#OSSS_CXX := $(CXX)

#
# Define the minimum version of GNU C++ compiler, if necessary.
# Defaults to OSSS minimum requirement of GNU C++ 3.4.4
#
# Specify version as triplet of <MAJOR><MINOR><PATCHLEVEL>,
# each as two digits. Requiring at least g++ 4.0 means
#OSSS_MIN_CXX_VERSION := 040000
#

# Compiler settings
#
#
# The following variables are respected during the compilation
# process. Defaults are below.
# Note:
#    CXXFLAGS_DEBUG is only added, iff $(DEBUG)  = yes
#    CXXFLAGS_OPT   is only added, iff $(DEBUG) != yes
#
#OSSS_CXXFLAGS       := -Wall
#
#OSSS_CXXFLAGS_OPT   := -O3
#OSSS_CXXFLAGS_DEBUG := -g -DDEBUG=1 -DTRACE=1

# additional target to clean up current application
#
# This target is automatically invoked, if 'clean'
# is called, see below.
EXTRA_CLEAN=extra-clean

# -----------------------------------------------------------------------
# look for common build rules in generic places
OSSS_BUILD_RULES += \
  ./Makefile.osss   \
  ../Makefile.osss  \
   $(OSSS_HOME)/examples/Makefile.osss
OSSS_BUILD_RULES := $(word 1,$(wildcard $(OSSS_BUILD_RULES)))
# include generic OSSS Makefile
ifneq (,$(strip $(OSSS_BUILD_RULES)))
include $(OSSS_BUILD_RULES)
else
$(error Common OSSS Makefile 'Makefile.osss' not found! Bailing out)
endif

# -----------------------------------------------------------------------
#    local targets below
# -----------------------------------------------------------------------

$(MODULE).fossy_out.vhdl : main.cc
	fossy --topInst=$(MODULE) $<
	mv synthesised_main.vhdl $@

$(MODULE).fossy_out.cpp : main.cc
	fossy --systemc --topInst=$(MODULE) $<
	mv synthesised_main.cc $@

output_of_vhdl_after_synth.txt : $(MODULE).fossy_out.vhdl testbench.vhdl
	-rm -rf work ; vlib work
	vcom $<
	vcom testbench.vhdl
	vsim -c testbench -do "run 1000 ns ; quit -f;" -std_output $@

.PHONY: vhdl-sim
vhdl-sim : output_of_vhdl_after_synth.txt
	cat $<

output_of_systemc_before_synth.txt : $(MODULE).x
	./$< > $@

$(MODULE).fossy_out.x : main.cc Stimulus.cc
	g++ -I$(SYSTEMC_HOME)/include -DUSE_FOSSY_OUT -o $@ -L$(SYSTEMC_LIB) $^ -lsystemc

output_of_systemc_after_synth.txt : $(MODULE).fossy_out.x
	./$< > $@

diff_vhdl : output_of_systemc_before_synth.txt output_of_vhdl_after_synth.txt
	diff $^ > $@

diff_systemc : output_of_systemc_before_synth.txt output_of_systemc_after_synth.txt
	diff $^ > $@

$(MODULE).prj : $(MODULE).fossy_out.vhdl

.PHONY: synth_xst
synth_xst : $(MODULE).fossy_out.vhdl
	echo "run -ifn $< -ifmt vhdl -top AlarmClock -p virtex4 -ofn AlarmClock_xst_out" | xst

# see $(EXTRA_CLEAN)
.PHONY: extra-clean
extra-clean:
	$(call cmd-delete,$(wildcard *.awif *.vcd *.wlf *.xml *.cil *.ti *.txt *.fossy_out.* diff_* transcript *.lso *.ngc *.dot))
	-rm -rf work xst
	-rm -rf AlarmClock_xst_out_xst.xrpt xlnx_auto_0.ise xlnx_auto_0_xdb
# $Id: Makefile 1466 2007-08-31 11:32:31Z claus $
# :flavour: (all,d)
# TAF!
