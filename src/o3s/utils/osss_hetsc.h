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
#ifndef OSSS_HETSC_H_INCLUDED_
#define OSSS_HETSC_H_INCLUDED_
/**
 * \file  osss_hetsc.h
 * \brief common HetSC inclusion header
 *
 * Include this file, iff you want to use the SystemC modelling library
 * HetSC in combination with the OSSS library.
 *
 * \see http://www.teisa.unican.es/HetSC
 */

#include "osss_common.h"
#include "osss_systemc.h"

/**
 * \def OSSS_INCLUDE_HETSC
 * \brief define this, if HetSC is available
 *
 * This directive should be defined, if the HetSC implementation
 * is available on the current platform.  If undefined, all HetSC
 * specific code will be excluded from the OSSS library code.
 *
 * \note During hardware synthesis, this directive has no effect.
 * \see  OSSS_HAVE_HETSC_
 */
#if defined( OSSS_SYNTHESIS_ )
/// drop HetSC during synthesis
#  undef OSSS_INCLUDE_HETSC
#endif

#if defined( OSSS_INCLUDE_HETSC )

/* include HetSC header, when available */
#include <hetsc.h>

/**
 * \def OSSS_HAVE_HETSC_
 * \brief internal check, whether HETSC is available
 *
 * This macro is always defined, and has the value 1,
 * in case HetSC is available and 0 otherwise.
 *
 * It should be used within the sources, to encapsulate
 * HetSC specific code.
 */
#  define OSSS_HAVE_HETSC_ 1

/* ----------- add further HetSC specific code here ------------ */

#else // ! OSSS_INCLUDE_HETSC
#  define OSSS_HAVE_HETSC_ 0
#endif // OSSS_INCLUDE_HETSC
#endif // OSSS_HETSC_H_INCLUDED_
/* Taf!
 * :flavour:(all,H)
 */
