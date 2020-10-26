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
#ifndef OSSS_SHARED_IF_H_INCLUDED
#define OSSS_SHARED_IF_H_INCLUDED

#include "o3s/structural/app/osss_arbiter_shared.h"
#include "o3s/utils/osss_types.h"

namespace osss {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a shared object interface class used
///        as interface for osss_ports
///
/// \note Interface class
///
////////////////////////////////////////////////////////////////////////////

template<class IF = sc_core::sc_interface>
class osss_shared_if
{
  
};

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements a shared object interface class used
///        as base class for osss_shared
///
/// \note Interface class
///
////////////////////////////////////////////////////////////////////////////

template<>
class osss_shared_if<sc_core::sc_interface>
  : virtual public sc_core::sc_interface
{
protected:
  typedef osss_id_type IDType;

public:

  //! \brief Specify the reset sensitivity of the shared object
  //!
  //! The reset sensitivity can be:
  //!   - OSSS_POS   : sensitive on the positive edge of the reset signal
  //!   - OSSS_NEG   : sensitive on the negative edge of the reset signal
  //!   - OSSS_LEVEL : sensitive on the level (high or low) of the 
  //!                  reset signal 
  //!
  //! The default value is: OSSS_POS_EDGE
  virtual
  void 
  reset_sensitivity(const osss_sensitivity sens) = 0;

  //! Calls the subscribe method on the embedded arbiter.
  //! \see osss_arbiter_shared
  virtual
  void
  subscribe( const IDType portID, osss_priority priority = 0 ) = 0;

  //! Returns a pointer to the arbiter of the shared object.
  virtual
  osss_arbiter_shared_base * 
  getArbiter() = 0;

  //! Returns the interface of the object inside the shared object.
  //! If the object has no interface of type sc_interface it returns NULL.
  virtual
  sc_interface *
  get_interface() const = 0;

  //! Registers the address of an osss_port_to_shared (paddr) together with the
  //! process ID (clientID) of the client process using the osss_port_to_shared.
  virtual
  void 
  registerPortUsage(const IDType paddr, 
		    const osssi::OSSS_Process_ID_t proc_id) = 0;

  //! Returns the process ID registered for the address of an 
  //! osss_port_to_shared (paddr).
  virtual
  IDType 
  lookupPortID(const osssi::OSSS_Process_ID_t proc_id) = 0;

  //! Prints the osss_port_to_shared address to process ID mapping.
  //! \note This is used for debugging only
  virtual
  void 
  printPortMap() = 0;

};

} //namespace osss

#endif // OSSS_SHARED_IF_H_INCLUDED
// $Id: osss_shared_if.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
