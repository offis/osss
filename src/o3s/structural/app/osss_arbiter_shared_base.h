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
#ifndef OSSS_ARBITER_SHARED_BASE_H_INCLUDED_
#define OSSS_ARBITER_SHARED_BASE_H_INCLUDED_

#include "o3s/utils/osss_types.h"

namespace osss {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class declares the interface of the osss_arbiter_shared 
///        class.
///
/// \note Interface class
///
////////////////////////////////////////////////////////////////////////////

class osss_arbiter_shared_base
{
protected:
  typedef osss_id_type    IDType;
public:

  //! \brief Sets a request for a client
  //!
  //! This method sets the request for the client specified by the first
  //! argument to the Boolean value specified by the second value. Passing
  //! "true" means, the appropriate client requests a service.
  virtual
  void 
  setRequest( const IDType clientID, bool value )  = 0;

  //! \brief Sets a grant for a client
  //! 
  //! This method sets the grant for the client specified by the first
  //! argument to the Boolean value specified by the second value. Passing
  //! "true" means, the appropriate client will be granted.
  virtual 
  void 
  setGrant( const IDType clientID, bool value ) = 0;

  //! \brief Sets a guard for a client
  //!
  //! This method sets the guard for the client specified by the first
  //! argument to the Boolean value specified by the second value. Passing
  //! "true" means, that the guard expression of the guarded method called
  //! by the appropriate client is actually evaluated "true".
  virtual
  void 
  setGuard( const IDType clientID, bool value ) = 0;

  //! \brief Returns whether a client is granted
  //!
  //! Returns true, if the request of the client specified by the passed 
  //! argument is granted. Only one client at a time will be granted.
  //! This method will write a message to cerr and exit, iff the value
  //! of m_reset is "false" when called.
  virtual
  bool 
  getGrant( const IDType clientID ) = 0;

  //! \brief Updates the grants for all clients
  //!
  //! Only one client
  //! at a time will be granted. Must be called whenever a client has stated
  //! a new request, that means calls setRequest with "true" as second 
  //! argument.
  //! This method will write a message to cerr and exit, iff the schedule
  //! method of the scheduler returns a value out of range, that means
  //! not within range 0 to m_numberOfClients - 1.
  virtual
  void 
  updateGrant() = 0;

  //! \brief Indicates whether this is a real or a dummy arbiter
  //!
  //! Returns true, if the implementation of this interface is a dummy arbiter.
  //! Otherwise this method returns false.
  virtual
  bool
  dummy() = 0;

  virtual
  ~osss_arbiter_shared_base() {}

};

} // namespace osss

#endif // OSSS_ARBITER_SHARED_BASE_H_INCLUDED_
// $Id: osss_arbiter_shared_base.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
