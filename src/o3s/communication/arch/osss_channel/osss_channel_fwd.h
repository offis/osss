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

#ifndef OSSS_CHANNEL_FORWARD_H_INCLUDED
#define OSSS_CHANNEL_FORWARD_H_INCLUDED

namespace osss {

enum osss_via_type
{
  OSSS_NO_VIA,
  OSSS_MUX_VIA,
  OSSS_OR_VIA,
  OSSS_AND_VIA,
  OSSS_AND_OR_VIA,
};

enum osss_mux_controller
{
  OSSS_ARBITER,
  OSSS_ADDRESS_DECODER,
};

class osss_port_list_element_base;
class osss_connection_list_element;

class osss_via_base;
class osss_via_logic;

namespace osssi {

class osss_no_via;
class osss_or;
class osss_and;
class osss_and_or;
class osss_mux;

class osss_shared_signal;
class osss_multi_signal;

} // end namespace osssi
} // end namespace osss

#endif
// $Id: osss_channel_fwd.h 2923 2008-12-16 10:32:25Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
