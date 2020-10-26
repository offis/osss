# Copyright (c) 2006  OFFIS e.V., Oldenburg, Germany
# All rights reserved.
# -------------------------------------------------------------------
# Common make rules for the OSSS library
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
# Author(s): Philipp A. Hartmann
# Version  : $Revision: 3058 $
# Lasted   : $Date: 2009-03-18 09:17:18 +0100 (Wed, 18 Mar 2009) $
# -------------------------------------------------------------------
ifeq (undefined,$(origin OSSS_MAKE_COMMON_INCLUDED))
OSSS_MAKE_COMMON_INCLUDED := 1

##
## Some variables
##

# variable to hold phony targets
PHONY :=

# explicitly use bash as shell
SHELL := $(shell if [ -x "$${BASH}" ] ; then echo $${BASH} ;                \
                 else if [ -n "`which bash`" ] ; then echo "`which bash`" ; \
                 else if [ -x /bin/bash ] ; then echo /bin/bash ;           \
                 fi ; fi ; fi )
ifeq (,$(SHELL))
$(error Required Bourne-again shell (bash) not found. Bailing out )
endif

# command to start doxygen
DOXYGEN?=doxygen

# filename patterns, that are ignored while searching for source files
OSSS_DISTIGNORE := '*.o' '*.d' '*~' '*.bak' '*.orig' '*.rej' '.svn'

OSSS_FINDIGNORE := \( $(patsubst %,-name % -o,$(OSSS_DISTIGNORE)) -false \) \
                    -prune -o

# extra filenames
# - are always included in distribution tarball
# - can have their own special version in a flavour directory
#   (build/<flavour>)
OSSS_XTRA_FILES := AUTHORS README COPYING LICENSE

OSSS_DEFINES   :=
OSSS_INCLUDES   = -I$(OSSS_SRCDIR) -isystem$(OSSS_CFG_SYSTEMC_HOME)/include

# all known library archive files
OSSS_LIBRARIES := $(call flavour-to-library,$(OSSS_FLAVOURS))

##
## Convenience rules
##

# default target (empty here)
all:

## clean configuration
cfgclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning config...")
	$(call cmd-delete,$(wildcard $(OSSS_CONFIG)))
PHONY += cfgclean

##
## Handle file lists
## i.e. enforce rebuilding file lists for the different flavours
files:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Looking for source files...")
	$(foreach __flavour,$(OSSS_FLAVOURS),\
		$(call cmd-create-filelist,$(__flavour)))
PHONY += files

lstclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Removing source file lists...")
	$(foreach __flavour,$(OSSS_FLAVOURS),\
	  $(call cmd-delete,$(wildcard $(OSSS_SCRIPTDIR)/$(__flavour)/files.mk)))
PHONY += lstclean

##
## Compilation and configuring  help
## 
help:
	@echo "-------------------------------------------------- "
	@echo " OSSS version $(OSSS_VERSION) - build system help  "
	@echo "-------------------------------------------------- "
	@echo
ifneq (1,$(words $(OSSS_FLAVOURS)))
	@echo "Library flavours: $(OSSS_FLAVOURS)"
	@echo "  Any of the targets below are built for all of these."
	@echo "  Add 'f=<flavour>' to the call of the build process to"
	@echo "  select a specific flavour only. For example:"
	@echo "  (g)make f=$(firstword $(OSSS_FLAVOURS)) all"
endif
	@echo "Source tree configuration:"
	@echo "  (g)make config [option=value]..."
	@echo "  Configuration options:"
	@echo "    cxx             : C++ compiler (>= GNU C/C++ compiler 3.4.2)"
	@echo "    cxxflags        : Custom compiler flags (default: '-Wall')"
	@echo "    systemc         : Path to SystemC installation"
	@echo "    systemc-lib     : Path to SystemC library"
	@echo "    systemc-pthreads: Link against SystemC library built with pthreads (yes|no)"
	@echo "    verbose         : Verbose compilation output (yes|no)"
	@echo "    debug           : Include debugging information in library (yes|no)"
	@echo "    profile         : Include profiling information in library (yes|no)"
	@echo "Library targets:"
	@echo "  all        : build library and headers (default target)"
	@echo "  lib        : build static library (in $(OSSS_BUILDDIR)/lib)"
	@echo "  header     : install public headers (in $(OSSS_BUILDDIR)/include)"
	@echo "  doc        : build Doxygen documentation (in $(OSSS_BUILDDIR)/doc)"
	@echo "  dist       : build a .tar.gz archive of source tree"
	@echo "  installer  : create a self-contained installer script"
	@echo "Helper targets:"
	@echo "  examples   : build examples"
ifneq (,$(wildcard $(OSSS_BASEDIR)/tests))
	@echo "  tests      : build tests"
endif
ifneq (,$(wildcard $(OSSS_BASEDIR)/designs))
	@echo "  designs    : build designs"
endif
	@echo "  depend     : build dependency information (usually handled automatically)"
	@echo "  files      : rebuild list of source files (usually not necessary)"
	@echo "  help       : this help text"
	@echo "Cleaning targets:"
	@echo "  clean      : clean object files, header files and library"
	@echo "  distclean  : remove autogenerated files (except file lists)"
	@echo "  ultraclean : remove all autogenerated files"
	@echo "  cfgclean   : remove configuration"
	@echo "  objclean   : clean object files"
	@echo "  hdrclean   : clean header files"
	@echo "  libclean   : clean library"
	@echo "  depclean   : clean dependency files"
	@echo "  expclean   : clean built examples"
ifneq (,$(wildcard $(OSSS_BASEDIR)/tests))
	@echo "  testclean  : clean built tests"
endif
ifneq (,$(wildcard $(OSSS_BASEDIR)/designs))
	@echo "  designclean: clean built designs"
endif
	@echo "  lstclean   : clean file lists"
	@echo
PHONY += help

##
## Build examples
##
examples: header lib
	@echo "*** Building examples..."
	+$(Q)$(MAKE) -C $(OSSS_BASEDIR)/examples build

# clean examples
expclean:
	@echo "*** Cleaning examples..."
	+$(Q)$(MAKE) -C $(OSSS_BASEDIR)/examples clean

ifneq (,$(wildcard $(OSSS_BASEDIR)/tests))
##
## Build tests
##
tests: header lib
	@echo "*** Building tests..."
	+$(Q)$(MAKE) -C $(OSSS_BASEDIR)/tests build

# clean tests
testclean:
	@echo "*** Cleaning tests..."
	+$(Q)$(MAKE) -C $(OSSS_BASEDIR)/tests clean

OSSS_DISTCLEAN+=testclean
endif

ifneq (,$(wildcard $(OSSS_BASEDIR)/designs))
##
## Build designs
##
designs: header lib
	@echo "*** Building designs..."
	+$(Q)$(MAKE) -C $(OSSS_BASEDIR)/designs build

# clean designs
designclean:
	@echo "*** Cleaning designs..."
	+$(Q)$(MAKE) -C $(OSSS_BASEDIR)/designs clean

OSSS_DISTCLEAN+=designclean
endif


##
## Source tree configuration
##

# if config exists, include it
ifneq (,$(wildcard $(OSSS_CONFIG)))

include $(OSSS_CONFIG)

# export current settings to the environment of the config script
export OSSS_CFG_VERBOSE
export OSSS_CFG_DEBUG
export OSSS_CFG_PROFILE
export OSSS_CFG_CXX
export OSSS_CFG_CXXFLAGS
export OSSS_CFG_SYSTEMC_HOME
export OSSS_CFG_SYSTEMC_LIB
export OSSS_CFG_SYSTEMC_PTHREADS

else # otherwise, set default values

OSSS_CFG_VERBOSE_DEFAULT          := no
OSSS_CFG_DEBUG_DEFAULT            := no
OSSS_CFG_PROFILE_DEFAULT          := no
OSSS_CFG_CXX_DEFAULT              := $(CXX)
OSSS_CFG_CXXFLAGS_DEFAULT         := $(if $(CXXFLAGS),$(CXXFLAGS),-Wall)
OSSS_CFG_SYSTEMC_PTHREADS_DEFAULT := no

export OSSS_CFG_VERBOSE_DEFAULT
export OSSS_CFG_DEBUG_DEFAULT
export OSSS_CFG_PROFILE_DEFAULT
export OSSS_CFG_CXX_DEFAULT
export OSSS_CFG_CXXFLAGS_DEFAULT
export OSSS_CFG_SYSTEMC_PTHREADS_DEFAULT

endif

# we build the configuration, currently
ifeq (yes,$(OSSS_BUILD_CONFIG))

# check for config options from the command line
# (lowercase, and without OSSS_CFG_ prefix)
ifeq (command line,$(origin verbose))
OSSS_CFG_VERBOSE_CMD := $(verbose)
export OSSS_CFG_VERBOSE_CMD
endif
ifeq (command line,$(origin debug))
OSSS_CFG_DEBUG_CMD   := $(debug)
export OSSS_CFG_DEBUG_CMD
endif
ifeq (command line,$(origin profile))
OSSS_CFG_PROFILE_CMD := $(profile)
export OSSS_CFG_PROFILE_CMD
endif
ifeq (command line,$(origin cxx))
OSSS_CFG_CXX_CMD := $(cxx)
export OSSS_CFG_CXX_CMD
endif
ifeq (command line,$(origin cxxflags))
OSSS_CFG_CXXFLAGS_CMD := $(cxxflags)
export OSSS_CFG_CXXFLAGS_CMD
endif
ifeq (command line,$(origin systemc))
OSSS_CFG_SYSTEMC_HOME_CMD := $(systemc)
export OSSS_CFG_SYSTEMC_HOME_CMD
endif
ifeq (command line,$(origin systemc-lib))
OSSS_CFG_SYSTEMC_LIB_CMD := $(systemc-lib)
export OSSS_CFG_SYSTEMC_LIB_CMD
endif
ifeq (command line,$(origin systemc-pthreads))
OSSS_CFG_SYSTEMC_PTHREADS_CMD := $(systemc-pthreads)
export OSSS_CFG_SYSTEMC_PTHREADS_CMD
endif

# export version string
export OSSS_VERSION

# target to (re-)create source tree configuration
config:
	@echo "*** (Re-)creating OSSS library configuration..."
	@$(SHELL) $(OSSS_SCRIPTDIR)/configure.sh $(OSSS_CONFIG)
	@echo "Configuration succeeded."
PHONY += config

endif # build config

# if config is strictly required, enforce building it
# (requirement additionally checked through prerequisites of
# corresponding targets)
ifeq (yes,$(OSSS_REQUIRE_CONFIG))
#
# Note: This rule always fails (deliberately)!
#
$(OSSS_CONFIG): $(OSSS_SCRIPTDIR)/configure.sh
	@echo "[ERR] OSSS source tree needs to be (re-)configured."
	@echo "      Please run \"make config\" first. Bailing out."
	@false

# if config doesn't exist yet, enforce (error by) trying to build it
ifeq (,$(wildcard $(OSSS_CONFIG)))
include $(OSSS_CONFIG)
endif

endif # OSSS_REQUIRE_CONFIG

##
## Set compiler flags according to config
##
OSSS_CXXFLAGS := $(OSSS_CFG_CXXFLAGS)

ifeq (yes,$(OSSS_CFG_DEBUG))
OSSS_CXXFLAGS += -g -O0
OSSS_DEFINES  += -DDEBUG
else
OSSS_CXXFLAGS += -O2
endif

ifeq (yes,$(OSSS_CFG_PROFILE))
OSSS_CXXFLAGS += -pg
endif

# Prefix, to suppress certain commands, if not building verbosely
Q := $(if $(filter yes,$(OSSS_CFG_VERBOSE)),,@)

## additional dependencies for some targets:
## Note: These are flavour independent, therefore
##       only consider these at top-level
ifeq ($(OSSS_TOPLEVEL),$(MAKELEVEL))
ultraclean distclean: expclean cfgclean $(OSSS_DISTCLEAN)
dist: distclean
endif

##
## Single source file rules
##

# rule to compile a single source file
%.o: %.cpp %.d $(OSSS_CONFIG)
	@echo "[CXX] $(<:src/o3s/%=%)"
	$(Q)$(OSSS_CFG_CXX) -c $(OSSS_CXXFLAGS) \
	$(OSSS_DEFINES) $(OSSS_INCLUDES) \
	-o $@ $<

# rule to determine dependencies of a single source file
%.d: %.cpp $(OSSS_CONFIG)
	@echo "[DEP] $(<:src/o3s/%=%)"
	$(Q)$(OSSS_CFG_CXX) -c $(OSSS_CXXFLAGS) -MM -MP \
	$(OSSS_DEFINES) $(OSSS_INCLUDES) \
	-MT '$(*).d $(*).o' \
	-o $@ $<

endif # OSSS_MAKE_COMMON_INCLUDED
# $Id: common.mk 3058 2009-03-18 08:17:18Z philipph $
# :flavour: (all,d)
