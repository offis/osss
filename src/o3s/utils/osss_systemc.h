/* -----------------------------------------------------------------------
 * Copyright (c) 2005-2008  OFFIS Institute for Information Technology,
 *                          Carl von Ossietzky University,
 *                          Oldenburg, Germany
 * All rights reserved.
 *
 * This file is directly or indirectly part of the OSSS simulation
 * library, a library for synthesisable system level models in
 * SystemC.
 *
 * Created for the projects:
 *   - ICODES  (1)
 *   - PolyDyn (2)
 *   - ANDRES  (3)
 *
 * 1) http://icodes.offis.de/
 * 2) http://ehs.informatik.uni-oldenburg.de/en/research/projects/polydyn/
 * 3) http://andres.offis.de/
 *
 * A list of authors and contributors can be found in the accompanying
 * AUTHORS file.  For detailed copyright information, please refer
 * to the COPYING file.
 *
 * -----------------------------------------------------------------------
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * Contact information:
 *  OFFIS
 *  Institute for Information Technology
 *    Escherweg 2
 *    D-26121 Oldenburg
 *    Germany
 *  www  : http://offis.de/
 *  phone: +49 (441) 9722-0
 *  fax  : +49 (441) 9722-102
 * -----------------------------------------------------------------------
 */

/**
 * \file  osss_systemc.h
 * \brief common SystemC inclusion header
 *
 * Include this file, iff you want to use SystemC inside the OSSS library.
 *
 * \note  This file does not introduce any SystemC construct into the
 *        top-level namespace. You need to explicitly prefix SystemC
 *        types etc. with their namespace.
 */
#ifndef OSSS_SYSTEMC_H_INCLUDED
#define OSSS_SYSTEMC_H_INCLUDED

#include "osss_common.h"

/* include SystemC header, without namespace pollution */
#include <systemc>

/* simple version check */
#ifdef SYSTEMC_VERSION

/*
 * if SYSTEMC_VERSION is defined, we think, that we are using an
 * OSCI version of the SystemC kernel
 */

/** macro, that contains current SystemC version, if any */
#define OSSS_SYSTEMC_VERSION SYSTEMC_VERSION

/** macro, that gets defined, if an OSCI kernel is used */
#define OSSS_SYSTEMC_OSCI         OSSS_SYSTEMC_VERSION

#if OSSS_SYSTEMC_VERSION == 20050714
// we found OSCI SystemC kernel 2.1.v1 - at least we think so
#define OSSS_SYSTEMC_OSCI_2_1_v1  OSSS_SYSTEMC_VERSION
#define OSSS_SYSTEMC_2_1          OSSS_SYSTEMC_VERSION

#elif OSSS_SYSTEMC_VERSION == 20060505
// we found OSCI SystemC kernel 2.2.05jun06_beta - at least we think so
#define OSSS_SYSTEMC_OSCI_2_2_05jun06_beta  OSSS_SYSTEMC_VERSION
#define OSSS_SYSTEMC_2_2                    OSSS_SYSTEMC_VERSION

#elif OSSS_SYSTEMC_VERSION == 20070314
// we found OSCI SystemC kernel 2.2.0 - at least we think so
#define OSSS_SYSTEMC_OSCI_2_2_0  OSSS_SYSTEMC_VERSION
#define OSSS_SYSTEMC_2_2         OSSS_SYSTEMC_VERSION

#endif

#else
/** unknown SystemC version */
#define OSSS_SYSTEMC_VERSION  0

#endif // SYSTEMC_VERSION

#if !defined(OSSS_SYSTEMC_2_1) && !defined(OSSS_SYSTEMC_2_2)
#error Only OSCI implementations of SystemC, \
       starting from version 2.1.v1, are supported.
#endif

#endif // OSSS_SYSTEMC_H_INCLUDED
// $Id: osss_systemc.h 2769 2008-11-28 09:54:32Z philipph $
// :flavour:(all,H)
