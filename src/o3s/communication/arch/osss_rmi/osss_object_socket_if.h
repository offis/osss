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

#ifndef OSSS_OBJECT_SOCKET_IF_HH
#define OSSS_OBJECT_SOCKET_IF_HH

#include "o3s/structural/app/osss_shared_if.h"
#include "o3s/communication/osss_port.h"
#include "o3s/communication/arch/osss_channel/osss_basic_channel.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_server_if.h"

namespace osss {

class osss_object_socket_if : public sc_core::sc_interface
{
public:

/*
  virtual void bind(osss_export<
		      osss_rmi_if<
		        osss_rmi_channel_server_if> >& export_) = 0;
*/

  virtual void bind(osss_port<
                      osss_channel_if<
                        osss_rmi_channel_server_if> >& port) = 0;

  virtual const osssi::osss_object_registry& object_registry() const = 0;

  virtual 
  const std::vector<sc_core::sc_port_base*>& get_ports() const = 0;

  virtual osss_shared_if<>* get_shared_if() const = 0;

};

}; // namespace osss

#endif
// $Id: osss_object_socket_if.h 2843 2008-12-04 09:30:18Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
