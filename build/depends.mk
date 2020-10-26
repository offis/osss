#.mk Copyright (c) 2006  OFFIS e.V., Oldenburg, Germany
# -------------------------------------------------------------------
#
# Handle dependency files for the OSSS library build process
#
# Author(s): Philipp A. Hartmann
# Version  : $Revision: 631 $
# Lasted   : $Date: 2006-12-11 21:05:55 +0100 (Mon, 11 Dec 2006) $
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

ifeq (undefined,$(origin OSSS_MAKE_DEPENDS_INCLUDED))
OSSS_MAKE_DEPENDS_INCLUDED := 1

#
# Load/build required dependencies, if we do build some library
# flavour
#

# default target -> we build all flavours
ifeq (,$(MAKECMDGOALS))

OSSS_BUILD_FLAVOURS := $(OSSS_FLAVOURS)

else
# check for library targets
ifneq (,$(filter all lib depend,$(MAKECMDGOALS)))

# "all-flavour-targets"
OSSS_BUILD_FLAVOURS := $(OSSS_FLAVOURS)

else

# only selected flavours
OSSS_BUILD_FLAVOURS += $(call library-to-flavour,\
                        $(filter $(call flavour-to-library,$(OSSS_FLAVOURS)),\
                                 $(MAKECMDGOALS)))

endif # flavour specific dependencies
endif # dependency handling


# add dependencies for specific flavour targets
# and check for explicit ".o" targets
OSSS_REQUIRE_DEPS := $(call get-depends,$(OSSS_BUILD_FLAVOURS)) \
                     $(patsubst %.o,%.d,$(filter %.o,$(MAKECMDGOALS)))

# actually load/build required depends (only once)
OSSS_REQUIRE_DEPS := $(sort $(OSSS_REQUIRE_DEPS))
ifneq (,$(OSSS_REQUIRE_DEPS))
# load/build dependencies quietly
-include $(OSSS_REQUIRE_DEPS)
endif

endif # OSSS_MAKE_DEPENDS_INCLUDED
# $Id: depends.mk 631 2006-12-11 20:05:55Z philipph $
# :flavour: (all,d)
