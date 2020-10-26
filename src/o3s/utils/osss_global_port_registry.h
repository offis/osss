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
#ifndef OSSS_GLOBAL_PORT_REGISTRY_HH
#define OSSS_GLOBAL_PORT_REGISTRY_HH

#include "osss_systemc.h"

namespace osss {
//namespace osssi {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a registry for global clock and reset port
/// 
///
/// \note Static class.
///
////////////////////////////////////////////////////////////////////////////

class osss_global_port_registry
{
public:

  static 
  bool
  register_clock_port(sc_core::sc_in<bool>& clock_port);

  static  
  bool
  register_reset_port(sc_core::sc_in<bool>& reset_port);

  static
  bool
  is_clock_port_registered();

  static
  bool
  is_reset_port_registered();

  static
  sc_core::sc_in<bool>&
  get_clock_port();

  static
  sc_core::sc_in<bool>&
  get_reset_port();

 private:

  static sc_core::sc_in<bool>* m_clock_port;
  static sc_core::sc_in<bool>* m_reset_port;
};

//} // namespace osssi
} // namespace osss

#endif // OSSS_GLOBAL_PORT_REGISTRY_HH
// :flavour: (osss,h) (osss_icodes,h) (osss_recon,h) (osss_full,h)
