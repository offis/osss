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
#ifndef OSSS_RMI_SLAVE_IF_HH
#define OSSS_RMI_SLAVE_IF_HH

#include "o3s/communication/arch/osss_rmi/osss_rmi_types.h"

namespace osss {

class osss_rmi_slave_if
{
public:

  virtual 
  void 
  listen_for_action(OSSS_ClientID_t &clientID,
		    OSSS_ObjectID_t &objectID,
		    OSSS_MethodID_t &methodID,
		    osss_rmi::Command_ID_t &command_id) = 0;

  virtual
  void
  return_grant(OSSS_ClientID_t clientID,
	       bool granted = true,
               bool hasParameters = true) = 0;

  virtual 
  bool 
  receive_in_params(OSSS_Archive_t &arch) = 0;

  virtual 
  void 
  return_params_idle(OSSS_ClientID_t clientID,
		     bool is_busy = true,
                     bool is_void = true ) = 0;
    
  virtual 
  bool 
  provide_return_params(OSSS_ClientID_t clientID,
			OSSS_Archive_t &arch) = 0;


  virtual ~osss_rmi_slave_if() {}

};

} // namespace osss

#endif
// $Id: osss_rmi_slave_if.h 2991 2009-01-23 14:49:29Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
