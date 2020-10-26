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
#ifndef OSSS_TLM_H_INCLUDED_
#define OSSS_TLM_H_INCLUDED_
/**
 * \file  osss_tlm.h
 * \brief common SystemC TLM inclusion header
 *
 * Include this file, iff you want to use the SystemC TLM library
 * inside within OSSS library.
 */

#include "osss_common.h"
#include "osss_systemc.h"

/**
 * \def OSSS_INCLUDE_TLM
 * \brief define this, if TLM is available
 *
 * This directive should be defined, if a SystemC TLM implementation
 * is available on the current platform.  If undefined, all TLM
 * specific code will be excluded from the OSSS library code.
 *
 * \note During hardware synthesis, this directive has no effect.
 * \see  OSSS_HAVE_TLM_
 */
#if defined( OSSS_SYNTHESIS_ )
/// drop TLM during synthesis
#  undef OSSS_INCLUDE_TLM
#endif

#if defined( OSSS_INCLUDE_TLM )

/* include TLM header, when available */
#include <tlm/tlm.h>

/**
 * \def OSSS_HAVE_TLM_
 * \brief internal check, whether TLM is available
 *
 * This macro is always defined, and has the value 1,
 * in case SystemC TLM is available and 0 otherwise.
 *
 * It should be used within the sources, to encapsulate
 * SystemC TLM specific code.
 */
#  define OSSS_HAVE_TLM_ 1

/* ----------- add further TLM specific code here ------------ */


#else // ! OSSS_INCLUDE_TLM
#  define OSSS_HAVE_TLM_ 0
#endif // OSSS_INCLUDE_TLM
#endif // OSSS_TLM_H_INCLUDED_
/* Taf!
 * :flavour:(all,H)
 */
