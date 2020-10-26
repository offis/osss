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

#include "osss_context_base.h"
#include "osss_recon_base.h"
#include "o3s/utils/osss_process_handle.h"
#include "o3s/utils/time_consumer.h"
#include "o3s/utils/osss_assert.h"
#include "o3s/utils/osss_report.h"

#include <vector>
#include <utility>

namespace osss {
namespace osssi {

class osss_context_base::pending_client_registration_list
  : private std::vector< std::pair< osss_process_handle, int > >
{
  typedef std::vector< std::pair< osss_process_handle, int > > base_type;
public:

  using base_type::value_type;

  using base_type::iterator;
  using base_type::reverse_iterator;
  using base_type::const_iterator;
  using base_type::const_reverse_iterator;

  using base_type::begin;
  using base_type::back;
  using base_type::end;

  using base_type::empty;
  using base_type::push_back;
  using base_type::pop_back;

};

osss_context_base::osss_context_base( const char * name )
  : object_container()
  , sc_core::sc_object( name )
  , m_recon_object_ptr(0)
  , pending_client_registrations(0)
{}

osss_context_base::~osss_context_base()
{
  delete pending_client_registrations;
}

time_consumer_base*
osss_context_base::time_consumer() const
{
  OSSS_ASSERT( m_recon_object_ptr );
  return m_recon_object_ptr->time_consumer();
}

void
osss_context_base::signInUserProcess( osss_process_handle const & client_process,
                                      osss_priority               priority )
{
  if (NULL != m_recon_object_ptr)
  {
    // This context is already bound to a reconfigurable
    // object. We can directly pass things forward.
    m_recon_object_ptr->signInUserProcess(client_process, priority);
  }
  else
  {
    if( !pending_client_registrations ) {
      pending_client_registrations = new pending_client_registration_list;
    }

    // We are not bound yet. Therefore we don't know which
    // reconfigurable object will be used. We buffer things
    // and pass it on later.
    pending_client_registrations->push_back(
      std::make_pair(client_process, priority)
    );
  }
}

void
osss_context_base::do_bind( osss_recon_base& recon )
{
 // Sanity check: No duplicate binding
  if (m_recon_object_ptr != NULL)
  {
    std::cerr << "ERROR: named context with interface class "
              << this->if_typename() << std::endl
              << "       has already been bound to a recon object" << std::endl;
    OSSS_ABORT();
  }
  m_recon_object_ptr = &recon;
  recon.signInNamedContext(this);

  // Make registerClient() calls that were impossible before
  // knowing the recon object.
  if( pending_client_registrations )
  {
    while( false == pending_client_registrations->empty() )
    {
      recon.signInUserProcess(pending_client_registrations->back().first,
                              pending_client_registrations->back().second);
      pending_client_registrations->pop_back();
    }
    delete pending_client_registrations;
    pending_client_registrations = 0;
  }
}

void uses( osss_context_base & ci, osss_priority priority  )
{
  osss_uses( ci, priority );
}

void osss_uses( osss_context_base & ci, osss_priority priority )
{
  ci.signInUserProcess( osss_process_handle::last_created(), priority );
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_context_base.cpp 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
