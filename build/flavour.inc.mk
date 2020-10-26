# Copyright (c) 2006  OFFIS e.V., Oldenburg, Germany
# -------------------------------------------------------------------
# Rules for flavour specific targets.
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
# Version  : $Revision: 2986 $
# Lasted   : $Date: 2009-01-22 10:21:52 +0100 (Thu, 22 Jan 2009) $
#
# -------------------------------------------------------------------

ifeq (,$(FLAVOUR))
$(error "No or invalid flavour specified!")
endif
ifndef OSSS_MAKE_FLAVOUR_INCLUDED
OSSS_MAKE_FLAVOUR_INCLUDED := 1

# flavour's config directory
$(FLAVOUR)_DIR     := $(OSSS_SCRIPTDIR)/$(FLAVOUR)

# initialize some file list variables
$(FLAVOUR)_SRCS :=
$(FLAVOUR)_HDRS :=
$(FLAVOUR)_DIST :=

# include flavour specific Makefile?
ifneq (,$(wildcard $($(FLAVOUR)_DIR)/flavour.mk))
include $($(FLAVOUR)_DIR)/flavour.mk
endif

# do we need the file lists?
ifeq (yes,$(OSSS_REQUIRE_FILES))

$(FLAVOUR)_FILELIST := $($(FLAVOUR)_DIR)/files.mk
$(FLAVOUR)_DISTDIR := $(OSSS_TEMPDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION)

# load flavour specific file list
-include $($(FLAVOUR)_FILELIST)

# add automatically found files
$(FLAVOUR)_SRCS += $($(FLAVOUR)_AUTO_SRC_FILES)
$(FLAVOUR)_HDRS += $($(FLAVOUR)_AUTO_HDR_FILES)
$(FLAVOUR)_DIST += $($(FLAVOUR)_AUTO_DST_FILES)

endif

# generate file list, if necessary
$($(FLAVOUR)_FILELIST): %/files.mk: $(OSSS_SCRIPTDIR)/filelist.sh
	$(call cmd-create-filelist,$(*F))

# set flavour dependent variables
$(FLAVOUR)_LIB  := $(call flavour-to-library,$(FLAVOUR))

$(FLAVOUR)_OBJS := $(flavour-find-objects)
$(FLAVOUR)_DEPS := $(flavour-find-depends)

# rule to create distribution tarball
dist: distclean
	$(call cmd-rdelete,$(wildcard $($(FLAVOUR)_DISTDIR)))
	$(Q)mkdir -p $($(FLAVOUR)_DISTDIR)
	$(call cmd-dist-add-sources,$($(FLAVOUR)_DISTDIR),\
	        $($(FLAVOUR)_SRCS),$(wildcard $($(FLAVOUR)_DIR)/header.h))
	$(call cmd-dist-add-files,$($(FLAVOUR)_DISTDIR),$($(FLAVOUR)_DIST))
	$(call cmd-dist-add-extra,$($(FLAVOUR)_DISTDIR),\
	        $(OSSS_XTRA_FILES),$($(FLAVOUR)_DIR))
	$(call cmd-compact-dir,$(OSSS_TEMPDIR),$(FLAVOUR)-$($(FLAVOUR)_VERSION))
PHONY += dist

# rule to create self extracting archive
installer: dist
	@echo "[SCR] $(FLAVOUR)-$($(FLAVOUR)_VERSION).sh"
	$(Q)cat $(firstword $(wildcard $($(FLAVOUR)_DIR)/LICENSE \
                                   $(OSSS_BASEDIR)/LICENSE)) \
	    | gzip -c > $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).lic.gz
	$(Q)( cat $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).lic.gz | wc -c ; \
	      cat $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).tar.gz | wc -c ) \
	    | xargs echo > $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sizes
	$(Q)cat $(OSSS_SCRIPTDIR)/installer.sh.in                  \
	    | sed -e 's/@OSSS_LIBNAME@/"$(FLAVOUR)"/'              \
	          -e 's/@OSSS_LIBDESC@/"$($(FLAVOUR)_DESC)"/'      \
	          -e 's/@OSSS_VERSION@/"$($(FLAVOUR)_VERSION)"/'         \
	          -e "s/@OSSS_ARCHIVE_SIZES@/\"` \
	      cat $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sizes`\"/" \
	    > $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sh
	$(Q)cat $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).lic.gz \
	    >> $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sh
	$(Q)cat $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).tar.gz \
	    >> $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sh
	$(Q)$(DEL) $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).lic.gz \
	          $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sizes
	$(Q)chmod a+x $(OSSS_BUILDDIR)/$(FLAVOUR)-$($(FLAVOUR)_VERSION).sh
PHONY += installer

## build all library flavours and install their headers
all: lib header
PHONY += all

# convenience rule to create library
lib: $(OSSS_CONFIG) depend $($(FLAVOUR)_LIB)
PHONY += lib

# explicitly build dependencies, usually not necessary
depend: $(OSSS_CONFIG) $($(FLAVOUR)_DEPS) ;
PHONY += depend

# are there some objects for this flavour?
ifneq (,$(strip $($(FLAVOUR)_OBJS)))

# actual rule to create library
$($(FLAVOUR)_LIB): $($(FLAVOUR)_OBJS)
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Creating archive for '$(FLAVOUR)'")
	@echo "[AR ] $(@F)"
	$(Q)$(DEL) $@
	$(Q)mkdir -p $(dir $(@))
	$(Q)$(AR) -crs $(@) $(filter-out %.mk,$^)
	$(Q)ranlib $@

else
# empty rule, since nothing is in the library
$($(FLAVOUR)_LIB): ;
endif

# are there some public headers for this flavour?
ifneq (,$(strip $($(FLAVOUR)_HDRS)))

$(FLAVOUR)_HDRS_DEST := $(patsubst $(OSSS_SRCDIR)/%,\
                                   $(OSSS_BUILDDIR)/include/%,\
                                   $($(FLAVOUR)_HDRS))

header: $($(FLAVOUR)_HDRS_DEST) ;
PHONY += header

OSSS_HDRS_NEW := $(filter-out $(OSSS_HDRS_ALL),$($(FLAVOUR)_HDRS_DEST))

$(OSSS_HDRS_NEW): $(OSSS_BUILDDIR)/include/%: $(OSSS_SRCDIR)/%
	@echo "[HDR] $(*)"
	$(Q)$(call cmd-install,$<,$@)

OSSS_HDRS_ALL += $(OSSS_HDRS_NEW)

else
# empty rules, since no headers found
header: ;
endif

# ensure, that config exists if we try to build a library
$($(FLAVOUR)_LIB): $(OSSS_CONFIG)

# build flavour specific documentation
doc: FORCE
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Building documentation for '$(FLAVOUR)'")
	@export OSSS_DOC_FLAVOUR_NAME='$($(FLAVOUR)_DESC)' ; \
	 export OSSS_FLAVOUR='$(FLAVOUR)' ; \
	 export OSSS_SRCDIR='$(OSSS_SRCDIR)'; \
	 export OSSS_BUILDDIR='$(OSSS_BUILDDIR)'; \
	 export OSSS_DOC_INPUT='$(filter-out $(OSSS_SRCDIR)/o3s/extern/%,$($(FLAVOUR)_SRCS))' ; \
	 export OSSS_DOXYGEN_PREDEFINED='$($(FLAVOUR)_DOXYGEN_PREDEFINED)' ; \
	 mkdir -p $(OSSS_BUILDDIR)/doc/$(FLAVOUR) ; \
	 $(DOXYGEN) $(OSSS_SCRIPTDIR)/Doxyfile.in
PHONY += doc

##
## Clean OSSS tree
##

# clean object files for given flavour
objclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning object files of '$(FLAVOUR)'...")
	$(call cmd-delete,$(wildcard $(sort $($(FLAVOUR)_OBJS))),src/o3s/)
PHONY += objclean

# clean dependency files for given flavour
depclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning dependency files of '$(FLAVOUR)'...")
	$(call cmd-delete,$(wildcard $(sort $($(FLAVOUR)_DEPS))),src/o3s/)
PHONY += depclean

# clean header files for given flavour
hdrclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning public header files of '$(FLAVOUR)'...")
	$(call cmd-pdelete,$(wildcard $(call inverse-sort,$($(FLAVOUR)_HDRS_DEST))))
PHONY += hdrclean

# clean library file for given flavour
libclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning library file of '$(FLAVOUR)'...")
	$(call cmd-pdelete,$(wildcard $($(FLAVOUR)_LIB)),lib/)

# clean doxygen documentation for given flavour
docclean:
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning Doxygen files of '$(FLAVOUR)'...")
	$(call cmd-rdelete,\
	  $(wildcard $(OSSS_BUILDDIR)/doc/$(FLAVOUR)),$(OSSS_BUILDDIR)/)
	$(call cmd-delete,\
	  $(wildcard $(OSSS_BUILDDIR)/doc/$(FLAVOUR).warnings.log),$(OSSS_BUILDDIR)/)
	$(call cmd-delete,\
	  $(wildcard $(OSSS_BUILDDIR)/doc/$(FLAVOUR).tag),$(OSSS_BUILDDIR)/)
PHONY+=docclean

# clean given flavour
clean: objclean hdrclean libclean
PHONY += clean

# clean even more files for current flavour
distclean: clean depclean docclean
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning archive file of '$(FLAVOUR)'...")
	$(call cmd-delete,$(wildcard $(FLAVOUR)-*.tar.gz \
	       $(FLAVOUR)-*.sh $(FLAVOUR)-*.lic.gz $(FLAVOUR)-*.sizes ),)
	$(call cmd-rdelete,$(wildcard $($(FLAVOUR)_DISTDIR)),)
	$(call cmd-ddelete,$(wildcard $(OSSS_TEMPDIR)),$(OSSS_BUILDDIR)/,\
	        --ignore-fail-on-non-empty)

PHONY += distclean

# clean all autogenerated files for current flavour
ultraclean: distclean
	@$(if $(findstring yes,$(OSSS_CFG_VERBOSE)),\
		echo "*** Cleaning file list of '$(FLAVOUR)'...")
	$(call cmd-delete,$(wildcard $(OSSS_SCRIPTDIR)/$(FLAVOUR)/files.mk))

PHONY += ultraclean

endif # OSSS_MAKE_FLAVOUR_INCLUDED
# $Id: flavour.inc.mk 2986 2009-01-22 09:21:52Z philipph $
# :flavour: (all,d)
