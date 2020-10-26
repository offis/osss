# Copyright (c) 2006 OFFIS e.V., Oldenburg, Germany
# Some rights reserved.
# -------------------------------------------------------------------
# Recursive Makefile for the OSSS library examples
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
# Version  : $Revision:$
# Date     : $Date:$
#
# -------------------------------------------------------------------

# we've been called directly
ifeq (,$(OSSS_SUB_MAKE))

# store current directory
OSSS_BASEDIR := $(CURDIR)

# initialize directory traversation
OSSS_CURDIR  := $(OSSS_BASEDIR)

# start new parent directory search
OSSS_SUB_MAKE:=0

endif # initialization

# raise current directory variable
OSSS_CURDIR := $(patsubst %/,%,$(dir $(OSSS_CURDIR)))
# include parent Makefile
include $(OSSS_CURDIR)/Makefile

# $Id:$
# Taf!
