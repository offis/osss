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

#include "o3s/utils/osss_report.h"

#ifdef SYNTHESIS
  // leave this at 0
  #define CHECK_ARBITER 0
#else
  // set to 0 for speed and to 1 for debug
  #define CHECK_ARBITER 1
#endif

namespace osss {
namespace osssi {
//----------------------------------------------------------------------------//

template< class SchedulerType >
osss_arbiter_shared< SchedulerType >::osss_arbiter_shared()
  : m_numberOfClients(0)
  , m_request(0)
  , m_grant(0)
  , m_guard(0)
  , m_requestSignal("requestSignal")
  , m_grantSignal("grantSignal")
  , m_scheduler( 0 )
  , m_speculative_scheduler( 0 )
  , m_resetFlag(false)
  , m_firstAccess(true)
  , m_allGrantsFalse( false ) // We know nothing about the grants. Save value.
{}

//----------------------------------------------------------------------------//

template< class SchedulerType >
osss_arbiter_shared< SchedulerType >::~osss_arbiter_shared() {}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::
 subscribe( const IDType clientID, osss_priority priority )
{
  if ( !hasAlreadySubscribed(clientID) )
  {
    updateAttributes();

    Subscriber new_one = Subscriber();
    new_one.m_index = m_numberOfClients - 1;
    new_one.m_clientID = clientID;
    new_one.m_priority = priority;

    m_subscribersList.insert( m_subscribersList.begin(), new_one );
    m_subscribersList.sort();
  } else {
    setRequest( clientID, false );
  }
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::setRequest( const IDType clientID
                                                , bool value )
{
  m_request[getIndex( clientID )] = value;
  m_requestSignal.write( m_request );
  if ( !value )
  {
    m_scheduler = m_speculative_scheduler;
  }
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::setGrant( const IDType clientID
                                              , bool value )
{
  m_grant[getIndex( clientID )] = value;
#if CHECK_ARBITER == 1
  if (value && !m_request[getIndex( clientID )])
  {
     std::cerr << __PRETTY_FUNCTION__
               << ": ERROR! grant without request"
               << std::endl;
     OSSS_ABORT();
  }
#endif
  m_grantSignal.write( m_grant );
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::setGuard( const IDType clientID
                                              , bool value )
{
  m_guard[getIndex( clientID )] = value;
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
bool
osss_arbiter_shared< SchedulerType >::getGrant( const IDType clientID )
{

  if ( !m_resetFlag ) {
    osssi::osss_process_handle client =
      osssi::osss_process_handle::current();

    switch (client.proc_kind())
    {
      case sc_core::SC_CTHREAD_PROC_:
        OSSS_REPORT_WARNING( osss::report::access_before_reset )
          % client.name();
        break;
      default:
        /* ignore warning */
        break;
    }
    while(!m_resetFlag)
      sc_core::wait();

  } else {
    if ( m_firstAccess )
    {
      m_firstAccess = false;
      m_scheduler   = SchedulerType( m_numberOfClients );
      m_speculative_scheduler = m_scheduler;
    }
  }
#if CHECK_ARBITER == 1
  if ( m_numberOfClients == 1 )
  {
    /*
    if (m_grantSignal.read()[0] == false)
    {
       std::cerr << __PRETTY_FUNCTION__
                 << ": WARNING! Having only one client and a guard "
		 << "that is false is a deadlock "
		 << "(asuming a side effect free guard)!"
                 << std::endl;
       return false;
    }
    */
    if (m_grantSignal.read()[0] && !m_requestSignal.read()[0])
    {
       std::cerr << __PRETTY_FUNCTION__
                 << ": ERROR! grant without request"
                 << std::endl;
       OSSS_ABORT();
    }
   return( true );
  }
  else
#endif
  {
#if CHECK_ARBITER == 1
    if (m_grantSignal.read()[getIndex( clientID )] &&
	!m_requestSignal.read()[getIndex( clientID )])
    {
       std::cerr << __PRETTY_FUNCTION__
                 << ": ERROR! grant without request"
                 << std::endl;
       OSSS_ABORT();
    }
#endif
    return( m_grantSignal.read()[getIndex( clientID )] );
  }
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
inline
bool
osss_arbiter_shared< SchedulerType >::Subscriber::
operator<( const Subscriber &compareWith )
{
  return( m_priority < compareWith.m_priority );
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::reset()
{
  m_resetFlag = true;
  m_scheduler = SchedulerType( m_numberOfClients );
  m_speculative_scheduler = m_scheduler;
  // clear all grants:
  typename std::list<Subscriber>::iterator it = m_subscribersList.begin();
  while ( it != m_subscribersList.end() )
  {
    setGrant( it->m_clientID, false );
    ++it;
  }
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
bool
osss_arbiter_shared< SchedulerType >::
hasAlreadySubscribed( const IDType clientID )
{
  for(typename std::list<Subscriber>::iterator it = m_subscribersList.begin();
      it != m_subscribersList.end();
      ++it)
  {
    if ( it->m_clientID == clientID )
    {
      return( true );
    }
  }
  return( false );
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
osss_index_type
osss_arbiter_shared< SchedulerType >::getIndex( const IDType clientID )
{
  typename std::list< Subscriber >::iterator
    subscribersListIterator = m_subscribersList.begin();

  if ( m_subscribersList.empty() )
  {
    return( 0 );
  }
  do
  {
    if ( subscribersListIterator->m_clientID == clientID )
    {
      return( subscribersListIterator->m_index );
    }
    ++subscribersListIterator;
  } while ( subscribersListIterator != m_subscribersList.end() );
  std::cerr << "ERROR; The process "
            << osssi::osss_process_handle::current().name()
            << " didn't subscribe, exiting ..."
            << std::endl;
  OSSS_ABORT();
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
bool
osss_arbiter_shared< SchedulerType >::grantIsRunning()
{
  osss_index_type i = 0;
  while ( i < m_numberOfClients )
  {
    if ( m_grantSignal.read()[i] == true ) // using == IS NECESSARY!
    {
      return( true );
    }
    ++i;
  }
  return( false );
}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::updateAttributes()
{
  ++m_numberOfClients;

  m_request[m_numberOfClients - 1] = false;
  m_requestSignal.write(m_request);

  m_grant[m_numberOfClients - 1] = false;
  m_grantSignal.write(m_grant);

  m_guard[m_numberOfClients - 1] = false;

}

//----------------------------------------------------------------------------//

template< class SchedulerType >
void
osss_arbiter_shared< SchedulerType >::updateGrant()
{
  if ( !grantIsRunning() && !m_subscribersList.empty() )
  {
    if ( m_numberOfClients == 1 )
    {
      setGrant(
        m_subscribersList.begin()->m_clientID,
        m_guard[m_subscribersList.begin()->m_index] &&
            m_requestSignal.read()[m_subscribersList.begin()->m_index] );
    }
    else
    {
      bool * requestVector = new bool[m_numberOfClients];
      //std::vector<bool> requestVector( m_numberOfClients );

      osss_index_type grantedClient;
      typename std::list<Subscriber>::iterator
      nextGrant = m_subscribersList.begin();
      osss_index_type i = 0;
      // is there at least one request which is true AND
      // the guard is true, too?
      bool grant_possible = false;

      while ( nextGrant != m_subscribersList.end() )
      {
        requestVector[i] = m_guard[nextGrant->m_index] &&
                             m_requestSignal.read()[nextGrant->m_index];
        grant_possible = grant_possible || requestVector[i];
        ++i;
        ++nextGrant;
      }

      if (grant_possible)
      {
        // yes, we may call the scheduler
        m_speculative_scheduler = m_scheduler;
        grantedClient = m_speculative_scheduler.schedule( &requestVector[0] );

#if CHECK_ARBITER == 1
        if ( grantedClient >= m_numberOfClients )
        {
          std::cerr << "ERROR: Your scheduler grants a non-existing client, "
		    << "exiting ..." << std::endl;
          OSSS_ABORT();
        }
#endif
        i = 0;
        nextGrant = m_subscribersList.begin();
        while ( nextGrant != m_subscribersList.end() )
        {
          if ( i == grantedClient )
          {
            setGrant( nextGrant->m_clientID, true);
          } else {
            setGrant( nextGrant->m_clientID, false );
          }
          ++i;
          ++nextGrant;
          m_allGrantsFalse = false;
        }
      }
      else
      {
        // There is no such client.
        grantedClient = m_numberOfClients;
        // Check, if there is something to do
        if (false == m_allGrantsFalse)
        {
          // set all grants to false
          nextGrant = m_subscribersList.begin();
          while ( nextGrant != m_subscribersList.end() )
          {
            setGrant( nextGrant->m_clientID, false );
            ++nextGrant;
          }
          m_allGrantsFalse = true;
        }
      }

      delete[] requestVector;
    } // else noc!=1
  }
}

//----------------------------------------------------------------------------//
} // namespace osssi
} // namespace osss

// $Id: osss_arbiter_shared.tpp 3078 2009-03-27 13:10:42Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
