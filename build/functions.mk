# Copyright (c) 2006  OFFIS e.V., Oldenburg, Germany
# -------------------------------------------------------------------
#
# Make helper functions for the OSSS library
#
# Author(s): Philipp A. Hartmann
# Version  : $Revision: 2983 $
# Lasted   : $Date: 2009-01-20 21:40:37 +0100 (Tue, 20 Jan 2009) $
#
# -------------------------------------------------------------------
#
# Contact:
# OFFIS e.V.
#   Escherweg 2
#   D-26121 Oldenburg
#   Germany
# www  : http://offis.de
# phone: +49 (441) 9722-0
# fax  : +49 (441) 9722-102
#
# -------------------------------------------------------------------

ifeq (undefined,$(origin OSSS_MAKE_FUNCTIONS_INCLUDED))
OSSS_MAKE_FUNCTIONS_INCLUDED := 1

#
# check for GNU Make version > 3.80
#
# only GNU make is known to set builtin variable MAKE_VERSION
ifeq (,$(MAKE_VERSION))
check-make-3-80=0
else
# Check first and second part of Make version
# Expands to "1", iff version seems to be higher than 3.80
override check-make-3-80=$(shell \
   test  "`echo $(MAKE_VERSION) | cut -d. -f1`"  -ge 3  \
       -a "`echo $(MAKE_VERSION) | cut -d. -f2 | sed 's/\([0-9]\+\).*/\1/'`"\
       -ge 80 && echo 1 )
endif

#
# Determine files for current flavour
#

define flavour-find-objects
$(patsubst %.cpp,%.o,$(filter %.cpp,$($(FLAVOUR)_SRCS)))
endef

define flavour-find-depends
$(patsubst %.cpp,%.d,$(filter %.cpp,$($(FLAVOUR)_SRCS)))
endef

#
# Translate library name for flavour
#
define flavour-to-library
$(addprefix $(OSSS_BUILDDIR)/lib/lib,$(addsuffix .a,$(1:$(2)%=%)))
endef

define library-to-flavour
$(patsubst $(OSSS_BUILDDIR)/lib/lib%,%,$(basename $(1)))
endef

define get-objects
$(foreach __flavour,$(1),$($(__flavour)_OBJS))
endef

define get-depends
$(foreach __flavour,$(1),$($(__flavour)_DEPS))
endef

define get-headers
$(foreach __flavour,$(1),$($(__flavour)_HDRS))
endef

# shell command to remove a file
DEL := rm -f

# delete files
define cmd-delete
$(foreach __f,$(1),$(call cmd-delete-file,$(__f),$(2)))
endef

# delete recursively
define cmd-rdelete
$(foreach __f,$(1),$(call cmd-delete-file,$(__f),$(2),-r))
endef

# remove directories
define cmd-ddelete
$(foreach __d,$(1),$(call cmd-delete-dir,$(__d),$(2),$(3)))
endef

# delete file with path
define cmd-pdelete
$(foreach __p,$(1),$(call cmd-delete-path,$(__p),$(2)))
endef

# Note: empty line is required!
define cmd-delete-file
	@echo '[DEL] $(1:$(OSSS_BASEDIR)/$(2)%=%)'
	$(Q)$(DEL) $(3) $(1)
	
endef

define cmd-delete-path
	@echo '[DEL] $(1:$(OSSS_BUILDDIR)/$(2)%=%)'
	$(Q)$(DEL) $(1) ; \
	if [ -d "$(patsubst ./%,%,$(dir $(1)))" ] ; then \
	  rmdir -p --ignore-fail-on-non-empty $(patsubst ./%,%,$(dir $(1))) ;\
	fi
	
endef

define cmd-delete-dir
	@echo '[RMD] $(1:$(2)%=%)'
	$(Q)if [ -d "$(1)" ] ; then \
	  rmdir $(3) $(1) ; \
	fi
	
endef

#
# create/copy distribution files
#
define cmd-compact-dir
	@echo '[TGZ] $(2).tar.gz'
	$(Q)tar cf - -C $(1) $(2) | \
		gzip -c > $(2).tar.gz
	$(Q)$(DEL) -r $(1)/$(2)
	$(Q)rmdir --ignore-fail-on-non-empty $(1)
	
endef

ifneq (,$(wildcard $(OSSS_SCRIPTDIR)/distcopy.sh))
define cmd-dist-add-source-file
	@echo '[SRC] $(2:$(OSSS_SRCDIR)/%=%)'
	$(Q)$(SHELL) $(OSSS_SCRIPTDIR)/distcopy.sh \
		"$(1)" "$(2)" "$(3)"
	
endef
else
define cmd-dist-add-source-file
	@echo '[SRC] $(2:$(OSSS_SRCDIR)/%=%)'
	$(Q)mkdir -p $(1)/$(dir $(2))
	$(Q)ln -fn "$(2)" "$(1)/$(2)"
	
endef
endif

define cmd-dist-add-extra-file
	@echo '[DST] $(2)'
	$(Q)mkdir -p $(1)/$(dir $(2))
	$(Q)ln -fn "$(if $(wildcard $(3)/$(2)),$(3)/$(2),$(2))" "$(1)/$(2)"

endef


# 1: target directory
# 2: list of source files
# 3: (optional) new header
define cmd-dist-add-sources
$(foreach __f,$(2),$(call cmd-dist-add-source-file,$(1),$(__f),$(3)))
endef

# 1: target directory
# 2: list of files/directories
define cmd-dist-add-files
	$(Q)find $(2) $(OSSS_FINDIGNORE) -type f -print | \
	  ( DEST=$(1) ; \
	    while read FILE ; do \
	      echo "[DST] $${FILE}" ; \
	      mkdir -p $${DEST}/`dirname $${FILE}` ; \
	      ln -fn $${FILE} $${DEST}/$${FILE} ; \
	    done )

endef

# 1: target directory
# 2: list of extra files
# 3: (optional) directory with specialized variants of these files
define cmd-dist-add-extra
$(foreach __f,$(2),$(call cmd-dist-add-extra-file,$(1),$(__f),$(3)))
endef

#
# Build some target in a sub-process
#
define cmd-make
	@echo "*** Building target '$(1)' (flavour: $(2))..."
	+$(Q)$(MAKE) f=$(2) $(1)
	
endef

#
# Install a file
#

# BSD compatible install command
# TODO: improve command lookup
OSSS_INSTALL:=$(word 1,$(wildcard \
  /usr/local/bin/ginstall \
  /usr/local/bin/install \
  /usr/bin/ginstall \
  /usr/bin/install  \
))
ifeq (,$(OSSS_INSTALL))
OSSS_INSTALL:=$(OSSS_SCRIPTDIR)/install-sh -c
endif

define cmd-install
	$(Q)mkdir -p "$(dir $(2))" && \
	    $(OSSS_INSTALL) -m 644 "$(1)" "$(2)"
endef

#
# Create file lists
#
define cmd-create-filelist
	@echo "[LST] $(1)"
	$(Q)$(SHELL) $(OSSS_SCRIPTDIR)/filelist.sh \
	   $(1) \
	   $(OSSS_BASEDIR) \
	   $(OSSS_SCRIPTDIR)
	
endef

#
# remove duplicates and sort in inverse order
# NOTE: Make >= 3.80 required!
ifeq (1,$(check-make-3-80))
define inverse-sort
$(call inverse-sort-helper,$(sort $(1)))
endef

define inverse-sort-helper
$(if $(1),\
  $(call inverse-sort-helper,$(wordlist 2,$(words $(1)),$(1))) $(firstword $(1)))
endef
else
# GNU Make < 3.80 does not support recursive functions,
# only sort in this case
define inverse-sort
$(sort $(1))
endef
endif

define file-size
$(foreach __f,$(1),$(call file-size-helper,$(__f)) )
endef

define file-size-helper
$(if $(wildcard $1),$(shell cat $1 | wc -c),0)
endef

endif # OSSS_MAKE_FUNCTIONS_INCLUDED
# $Id: functions.mk 2983 2009-01-20 20:40:37Z philipph $
# :flavour: (all,d)
