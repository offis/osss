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

#ifndef OSSS_CLEAR_HELPER_DEFINES_H
#define OSSS_CLEAR_HELPER_DEFINES_H

namespace osss{
namespace osssi{

/** This type is selected as char* because we need a datatype
    pointing to a byte-size object for offset calculations.
    Void* doesn't serve us here. Eventually replace char*
    in future by a system-dependent type of size char.
    The type does not really matter -- size matters!
    Otherwise C/C++ pointer arithmetic will cause havoc!
  */
typedef char *   address_type;

/** Here we store the difference of two pointers. The type
    must be large enough to carry all offset differences
    within one object. So, for a 32 bit signed int, we
    support objects of 2 GB size max...
    Perhaps there is some system include file which
    provides a better type here?
  */
typedef std::ptrdiff_t offset_type;

} // end namespace osssi
} // end namespace osss

#endif // OSSS_CLEAR_HELPER_DEFINES_H

// $Id: osss_clear_helper_defines.h 3022 2009-03-04 22:55:24Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
