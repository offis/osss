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

#include "osss_recon_scheduler.h"
#include "o3s/structural/app/osss_recon_base.h"

namespace osss {

osss_recon_handle
osss_recon_scheduler::current_recon_object_handle = NULL;

osss_recon_scheduler::osss_recon_scheduler()
  : jobs(*this)
  , m_for_recon_object( current_recon_object_handle )
{}

//----------------------------------------------------------------------------//

osss_recon_scheduler::~osss_recon_scheduler()
{}

//----------------------------------------------------------------------------//

osss_context_idx
osss_recon_scheduler::getNumberOfContexts() const
{
  return m_for_recon_object->getNumberOfContexts();
}

osss_type_idx
osss_recon_scheduler::getNumberOfClasses() const
{
  return m_for_recon_object->getNumberOfClasses();
}

osss_recon_job_idx
osss_recon_scheduler::getNumberOfUserProcesses() const
{
  return m_for_recon_object->getNumberOfUserProcesses();
}

bool
osss_recon_scheduler::isConfigurationControllerBusy() const
{
  return m_for_recon_object->isConfigurationControllerBusy();
}

osss::osss_polymorphic< osss_cycle_count >
osss_recon_scheduler::getCycleValue( sc_dt::uint64 value ) const
{
  return m_for_recon_object->getCycleValue( value );
}


osss_recon_job_descriptor const&
osss_recon_scheduler::getJob( osss_recon_job_idx index ) const
{
  return m_for_recon_object->getJob(index);
}


//----------------------------------------------------------------------------//
// Methods for "jobs" member
//----------------------------------------------------------------------------//

osss_recon_job_descriptor const&
osss_recon_scheduler::osss_job_array::operator[](osss_recon_job_idx index) const
{
  return m_scheduler.getJob( index );
}

//----------------------------------------------------------------------------//

osss_recon_scheduler::osss_job_array::osss_job_array(osss_recon_scheduler& sched)
  : m_scheduler( sched )
{}


//----------------------------------------------------------------------------//

} // end namespace osss
// $Id: osss_recon_scheduler.cpp 2431 2008-07-08 13:38:27Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
