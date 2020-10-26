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
#ifndef OSSS_RMI_MASTER_IF_HH
#define OSSS_RMI_MASTER_IF_HH

#include "o3s/communication/arch/osss_rmi/osss_rmi_types.h"

namespace osss {

class osss_rmi_master_if
{
public:

  virtual
  void
  request_for_method(OSSS_ObjectID_t& objectID,
		     OSSS_MethodID_t& methodID) = 0;

  virtual
  void
  wait_for_grant(OSSS_ObjectID_t& objectID,
		 OSSS_MethodID_t& methodID) = 0;
    
  virtual 
  void 
  start_parameter_transfer(OSSS_ObjectID_t& objectID,
			   OSSS_MethodID_t& methodID) = 0;
  
  virtual 
  void 
  end_of_transfer(OSSS_ObjectID_t& objectID,
		  OSSS_MethodID_t& methodID,
		  bool last_transfer = false) = 0;
  
  virtual 
  void 
  idle() = 0;
  
  virtual 
  void 
  submit_parameters(OSSS_ObjectID_t& objectID, 
		    OSSS_Archive_t& archive) = 0;
  
  virtual 
  void 
  wait_for_completion(OSSS_ObjectID_t& objectID) = 0;

  /** 
   * 
   * 
   * @param objectID 
   * @param return_archive 
   * @param size 
   */
  virtual void 
  receive_return_parameter(OSSS_ObjectID_t& objectID, 
                           OSSS_Archive_t& return_archive,
                           unsigned int size) = 0;

  virtual ~osss_rmi_master_if() {}
   
};

} // namespace osss

#endif
// $Id: osss_rmi_master_if.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
