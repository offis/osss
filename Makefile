# Copyright (c) 2006  OFFIS e.V., Oldenburg, Germany
# All rights reserved.
# -------------------------------------------------------------------
# Top-level Makefile for the OSSS library
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
# Version  : $Revision: 3033 $
# Lasted   : $Date: 2009-03-11 14:30:28 +0100 (Wed, 11 Mar 2009) $
# -------------------------------------------------------------------


OSSS_VERSION := 2.2.1
export OSSS_VERSION

# Some directories
# TODO: Support separate build directory
OSSS_BASEDIR   := .
OSSS_SRCDIR    := $(OSSS_BASEDIR)/src
OSSS_BUILDDIR  := $(OSSS_BASEDIR)
OSSS_TEMPDIR   := $(OSSS_BUILDDIR)/.dist-tmp
OSSS_SCRIPTDIR := $(OSSS_BASEDIR)/build

# configuration file, all build targets depend on it
OSSS_CONFIG    := $(OSSS_BUILDDIR)/config.mk

# determine known flavours
OSSS_FLAVOURS  := $(patsubst $(OSSS_SCRIPTDIR)/%,%,\
                  $(patsubst %/flavour.mk,%, \
                  $(wildcard $(OSSS_SCRIPTDIR)/*/flavour.mk )))

# do not print 'Entering directory...'
MAKEFLAGS += --no-print-directory
# no implicit rules 
MAKEFLAGS += -r

# flavour-specific targets
OSSS_FLAVOUR_TARGETS := \
	all depend header lib doc dist installer \
	clean depclean objclean libclean hdrclean docclean \
	distclean ultraclean

# Targets, that do not require a configured source tree
OSSS_NOCONFIG_TARGETS := \
	Makefile files help cfgclean installer dist \
	clean depclean objclean libclean hdrclean docclean expclean \
	lstclean distclean ultraclean

OSSS_NOFILES_TARGETS := \
	files help config cfgclean expclean docclean lstclean Makefile

# assume, that we do not build mixed
# (config, dependency and other targets)
OSSS_MIXED_TARGETS       := no
# assume, that we do not build the config
OSSS_BUILD_CONFIG        := no
# assume by default, that we need a configured source tree
OSSS_REQUIRE_CONFIG      := yes
# assume by default, that we need the file lists
OSSS_REQUIRE_FILES       := yes

OSSS_TOPLEVEL ?= $(MAKELEVEL)
export OSSS_TOPLEVEL

# Do we build some special target?
ifneq (,$(MAKECMDGOALS))

# but a simple one (that does not need an existing configuration)
ifeq (,$(filter-out $(OSSS_NOCONFIG_TARGETS),$(MAKECMDGOALS)))
# ... do not require config, therefore reset it
OSSS_REQUIRE_CONFIG := no
OSSS_MIXED_TARGETS  := no

else # require config targets present

ifneq (,$(filter config,$(MAKECMDGOALS))) # building config?
OSSS_BUILD_CONFIG   := yes
OSSS_REQUIRE_CONFIG := no
ifneq (,$(filter-out config,$(MAKECMDGOALS)))
OSSS_MIXED_TARGETS  := yes
endif # mixed targets
endif # build config

endif # config or no config?

# we do not need the file lists
ifeq (,$(filter-out $(OSSS_NOFILES_TARGETS),$(MAKECMDGOALS)))
OSSS_REQUIRE_FILES  := no
endif
# explicitly building file lists?
ifneq (,$(filter files,$(MAKECMDGOALS)))
OSSS_REQUIRE_FILES  := no
# if we build something else as well, build it separately
ifneq (,$(filter-out files,$(MAKECMDGOALS)))
OSSS_MIXED_TARGETS  := yes
endif
endif # file lists

else # default target

OSSS_REQUIRE_CONFIG := yes
OSSS_MIXED_TARGETS  := no

# add "config" target, if config file does not exist
ifeq (,$(wildcard $(OSSS_CONFIG)))

OSSS_BUILD_CONFIG   := yes
OSSS_MIXED_TARGETS  := yes

all: config

endif # check config

endif # default target

# do we build mixed targets?
ifeq (yes,$(OSSS_MIXED_TARGETS))

# explicitly set top make level to "next" one
OSSS_TOPLEVEL := $(shell expr $(MAKELEVEL) + 1 )
export OSSS_TOPLEVEL

# do not rebuild this makefile
Makefile:: ;

# but build the others one by one
%:: FORCE
	@$(MAKE) -C $(OSSS_BUILDDIR) $@

.NOTPARALLEL:

else # we build either config or something else

#
# handle different library flavours
#

# any flavours found?
ifeq (,$(OSSS_FLAVOURS))
$(error No library flavours known to build. Bailing out)
endif

# single flavour version?
ifeq (1,$(words $(OSSS_FLAVOURS)))
OSSS_CURRENT_FLAVOUR := $(OSSS_FLAVOURS)
endif
# overridden flavour from command line
ifeq (command line,$(origin f))
OSSS_CURRENT_FLAVOUR := $(filter $(f),$(OSSS_FLAVOURS))
ifeq (,$(OSSS_CURRENT_FLAVOUR))
$(error Unknown library flavour '$(f)' given. Bailing out)
endif
endif # set current flavour

# load make helper functions
include $(OSSS_SCRIPTDIR)/functions.mk

# include common make rules
include $(OSSS_SCRIPTDIR)/common.mk

# a flavour has been selected, load it.
ifneq (,$(OSSS_CURRENT_FLAVOUR))

# load flavour
FLAVOUR := $(OSSS_CURRENT_FLAVOUR)
include $(OSSS_SCRIPTDIR)/flavour.inc.mk

else # multiple flavour handling

$(OSSS_FLAVOUR_TARGETS): FORCE
	$(foreach __flavour,$(OSSS_FLAVOURS),\
		$(call cmd-make,$(@),$(__flavour)))
PHONY += $(OSSS_FLAVOUR_TARGETS)

endif # flavour handling

endif # mixed targets

ifneq (yes,$(OSSS_MIXED_TARGETS))
# load required dependencies
include $(OSSS_SCRIPTDIR)/depends.mk
endif # mixed targets

# really phony target
FORCE:  ;
PHONY += FORCE

# explicitly set default target
.DEFAULT: all
# register phony targets
.PHONY: $(sort PHONY)
# disable any implicit rules
.SUFFIXES:

# $Id: Makefile 3033 2009-03-11 13:30:28Z claus $
# :flavour: (all,d)
