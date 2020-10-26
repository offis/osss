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
# Version  : $Revision: 514 $
# Date     : $Date: 2006-11-09 21:59:01 +0100 (Thu, 09 Nov 2006) $
#
# -------------------------------------------------------------------

# we've been called directly
ifeq (,$(OSSS_SUB_MAKE))

# store current directory
OSSS_BASEDIR := $(CURDIR)

# initialize directory traversation
OSSS_CURDIR  := $(OSSS_BASEDIR)

# start new parent directory search
OSSS_SUB_MAKE:=1

endif # initialization

# raise current directory variable
OSSS_CURDIR := $(patsubst %/,%,$(dir $(OSSS_CURDIR)))
# include parent Makefile
include $(OSSS_CURDIR)/Makefile

# $Id: Makefile 514 2006-11-09 20:59:01Z philipph $
# Taf!
