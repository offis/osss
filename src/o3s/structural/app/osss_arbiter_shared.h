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
#ifndef OSSS_ARBITER_SHARED_CLASS_H_INCLUDED_
#define OSSS_ARBITER_SHARED_CLASS_H_INCLUDED_

#include "o3s/algorithms/osss_no_scheduler.h"
#include "o3s/structural/app/osss_arbiter_shared_base.h"
#include "o3s/datatypes/osss_vector.h"
#include "o3s/utils/osss_process_handle.h"
#include "o3s/utils/osss_unchecked_signal.h"
#include <list>

namespace osss {
namespace osssi {

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements the arbiter needed for shared objects.
///
/// \note Class template. Requires to pass a scheduler class as template
///       argument on instantiation.
///
/// \todo Get rid of the multiple driver problem.
///
////////////////////////////////////////////////////////////////////////////

template< class SchedulerType >
class osss_arbiter_shared
  : public osss_arbiter_shared_base
{

public:

  //! \brief Default constructor
  //!
  //! Creates a new arbiter with zero clients connected to it.
  osss_arbiter_shared();

  virtual
  ~osss_arbiter_shared();

  //! \brief Subscription of a client with a certain priority.
  //!
  //! This method allows to pass
  //! an argument of type osss_priority to specify a certain priority for
  //! a client. More than one client can request the same priority. The
  //! priority determines the way, the clients are ordered in the internal
  //! buffers - the higher the priority, the higher the index in the buffer.
  //! The way the priority is interpreted depends on the scheduler itself.
  void
  subscribe( const IDType clientID, osss_priority priority );

  //! \brief Sets a request for a client
  //!
  //! This method sets the request for the client specified by the first
  //! argument to the Boolean value specified by the second value. Passing
  //! "true" means, the appropriate client requests a service.
  void
  setRequest( const IDType clientID, bool value );

  //! \brief Sets a grant for a client
  //!
  //! This method sets the grant for the client specified by the first
  //! argument to the Boolean value specified by the second value. Passing
  //! "true" means, the appropriate client will be granted.
  void
  setGrant( const IDType clientID, bool value );

  //! \brief Sets a guard for a client
  //!
  //! This method sets the guard for the client specified by the first
  //! argument to the Boolean value specified by the second value. Passing
  //! "true" means, that the guard expression of the guarded method called
  //! by the appropriate client is actually evaluated "true".
  void
  setGuard( const IDType clientID, bool value );

  //! \brief Returns whether a client is granted
  //!
  //! Returns true, if the request of the client specified by the passed
  //! argument is granted. Only one client at a time will be granted.
  //! This method will write a message to cerr and exit, iff the value
  //! of m_reset is "false" when called.
  bool
  getGrant( const IDType clientID );

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
  updateGrant();

  virtual
  bool
  dummy() { return false; }

  //! \brief Resets the arbiter.
  //!
  //! This method brings the arbiter back to a stable state, where all grants
  //! are cleared. This method also calls the constructor of the scheduler,
  //! and brings it thefore back to an initial state.
  void
  reset();

protected:

  //! The struct Subscriber is only for internal use. The struct stores for
  //! every client that subscribes, its index in internal buffers, its ID,
  //! that is taken from the SystemC simulation kernel, and its priority.
  struct Subscriber
  {
    osss_index_type m_index;
    IDType         m_clientID;
    osss_priority  m_priority; //!< 0, iff none requested

    //! Overloading the operator< is necessary for using the sort() method of
    //! the STL list class.
    inline
    bool
    operator<( const Subscriber &compareWith );
  };

  //! This method checks, if the client that is identified by the passed
  //! argument has already subscribed to the arbiter and returns true in that
  //! case, and false otherwise.
  bool
  hasAlreadySubscribed( IDType clientID );

  //! This method returns for a given client id its position in the request
  //! array and in requestSignal.
  osss_index_type
  getIndex( const IDType clientID );

  //! Method grantIsRunning returns true, if at least one entry of the
  //! grantSignal-array is set to true.
  bool
  grantIsRunning();

  //! Stores the number of clients that have subscribed to the arbiter.
  osss_index_type m_numberOfClients;

  //! A list to store an element of type Subscriber for every client that
  //! has subscribed to the arbiter. The list is sorted in order of
  //! increasing priority of the subscribers. The order of subscribers with
  //! the same priority must be regarded non-deterministic.
#ifndef SYNTHESIS
  std::list<Subscriber> m_subscribersList;
#endif

  //! This array of Boolean elements stores the values that will be taken on
  //! by the requestSignal, next time the simulation kernel updates the
  //! signal. The array is modifed by setRequest.
  osss_vector<bool> m_request;

  //! This array of Boolean elements stores the values that will be taken on
  //! by the grantSignal, next time the simulation kernel updates the
  //! signal. The array is modifed by setGrant.
  osss_vector<bool> m_grant;

  //! This array of Boolean elements stores the actual state of the guard
  //! expression for each requesting client. The array is modified by
  //! setGuard.
  osss_vector<bool> m_guard;

  //! This signal is used to indicate the request of all clients. A "true"
  //! value means, the corresponding client request a service.
  //! Here we use osss_unchecked_signal<T> that does not perform the
  //! multiple witer check.
  osss_unchecked_signal<osss_vector<bool> > m_requestSignal;

  //! This signal is used to indicate the grants for all clients. A "true"
  //! value means, the corresponding client is granted.
  //! Here we use osss_unchecked_signal<T> that does not perform the
  //! multiple witer check.
  osss_unchecked_signal<osss_vector<bool> > m_grantSignal;

private:

  //! The method updateAttributes is an auxiliary function, which resizes the
  //! request-, grant-, guard-, requestSignal- and grantSignal-arrays. The
  //! new size of these arrays is given by the actual value of
  //! m_numberOfClients. If m_numberOfClients is greater than the original
  //! size, all original values of the field-entries will be kept.
  void
  updateAttributes();

  //! This attributes store instances of the scheduler, that is specified
  //! by the template parameter of osss_arbiter_shared. The speculative scheduler
  //! is used to perform intermediate calcualtions. Only after all requesting
  //! clients had the chance to indicate their requests, the speculative
  //! scheduler is assigned to the "normal" scheduler.
  SchedulerType m_scheduler, m_speculative_scheduler;

  //! This attribute has value "true", iff the reset method has been called
  //! at least once, and "false" otherwise.
  bool m_resetFlag;

  //! This attribute has value "true", until getGrant is called the first
  //! time, and "false" afterwards.
  bool m_firstAccess;

  //! This attribute is used for speeding up internal calculations.
  //! It indicates that this object _knows_ that all grants are set to false
  //! (and may skip setting them this way again).
  bool m_allGrantsFalse;
};

////////////////////////////////////////////////////////////////////////////
///
/// \brief This class implements the a dummy arbiter needed for 
///        Shared Objects with no scheduler (osss_no_scheduler).
///
/// \note Partial specialisation of osss_arbiter_shared.
///
////////////////////////////////////////////////////////////////////////////

template<>
class osss_arbiter_shared< osss_no_scheduler >
  : public osss_arbiter_shared_base
{

public:

  osss_arbiter_shared() : osss_arbiter_shared_base() 
  {}

  virtual
  ~osss_arbiter_shared()
  {}

  //------------------------------------------------------------------------
  // methods from osss_arbiter_shared_base (they all do nothing)
  //------------------------------------------------------------------------
  void
  subscribe( const IDType clientID, osss_priority priority ) {}

  void
  setRequest( const IDType clientID, bool value ) {}

  void
  setGrant( const IDType clientID, bool value ) {}

  void
  setGuard( const IDType clientID, bool value ) {}

  bool
  getGrant( const IDType clientID ) { return true; }

  virtual
  void
  updateGrant() {}

  virtual
  bool
  dummy() { return true; }

  void
  reset() {}

};

} // namespace ossi
} // namespace osss

#include "o3s/structural/app/osss_arbiter_shared.tpp"

#endif // OSSS_ARBITER_SHARED_CLASS_H_INCLUDED_
// $Id: osss_arbiter_shared.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
