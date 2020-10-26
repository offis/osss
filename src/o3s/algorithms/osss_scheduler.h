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
#ifndef OSSS_SCHEDULER_H_INCLUDED_
#define OSSS_SCHEDULER_H_INCLUDED_

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_types.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

////////////////////////////////////////////////////////////////////////////
///
/// \brief This is the scheduler interface class
///
/// This class provides the base class for all schedulers that shall be
///  used together with shared objects. Every scheduler that shall be used
///  with a shared object must directly or indirectly be derived from this
///  class.
///
/// \note Interface class.
///       Every dervied class must implement the schedule method correctly.
///       Every derived class must be able to be copied by the built-in
///       assignment operator and must not overload the assignment operator.
///       Every derived class must only provide the constructor, that is given
///       by this class.
///
////////////////////////////////////////////////////////////////////////////

class osss_scheduler
{

public:

  //! \brief Constructor. 
  //!
  //! The only allowed constructor for all scheduler classes.
  //! Every derived class must provide this constrctor and only this
  //! constructor. When calling the this constructor on an ancestor class,
  //! the identifier of the only constructor parameter must be passed
  //! unchanged to the constructor of the ancestor class.
  explicit
  osss_scheduler( osss_index_type noc )
    : numberOfClients( noc )
  {}

  virtual ~osss_scheduler(){}

  //! This method is called by the shared object, to which the scheduler
  //! belongs to, once each cycle, in which at least one client of the
  //! shared object requests a service (and the corresponding guard
  //! condition is true), and the shared object is not blocked by a method
  //! under execution. For each client of a shared object, the passed
  //! RequestVector has a corresponding entry of type Boolean. The value of
  //! an entry is "true", iff the corresponding client requests a service in
  //! the actual cycle, and "false" otherwise. The entries of the
  //! RequestVector are sorted in ascending order according to the priorities
  //! of the clients, where the client with the lowest priority is located at
  //! index 0. The order of clients with the same priority must be regarded
  //! non-deterministic. The schedule method must determine which client
  //! shall be granted next, when activated. Therefore it must return value
  //! of type osss_index_type within range from 0 to the number of
  //! clients - 1, where 0 means the client at index 0 of the RequestVector
  //! is granted and number of clients - 1 means the client at index number
  //! of clients - 1 of the RequestVector is granted. If a higher value is
  //! returned, the shared object will report this fact and exit simulation.
  virtual
  osss_index_type
  schedule( const RequestVector & clientRequests ) = 0;

protected:

#if 0
  osss_index_type num_clients() const
    { return numberOfClients; }

private:
#endif

  //! This member MUST NEVER BE MODIFIED in any derived class. It is
  //! automatically set by the shared object during construction. It can be
  //! used as every other const member. The value of this member represents
  //! the number of clients of a shared object, and therefor the size of the
  //! RequestVector passed to the schedule method
  osss_index_type numberOfClients;

};

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_( osss_scheduler );

} // namespace osss

#endif // OSSS_SCHEDULER_H_INCLUDED_
// $Id: osss_scheduler.h 2914 2008-12-15 18:25:24Z philipph $
// :flavour:(osss,H) (osss_recon,h) (osss_icodes,h) (osss_full,h)
