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

#include "osss_device_scheduler.h"
#include "o3s/structural/arch/system/osss_device.h"

namespace osss
{
osss_device * osss_device_scheduler::current_device_id = NULL;

osss_device_scheduler::osss_device_scheduler() : jobs(this)
{
  m_for_device = current_device_id;
}

//----------------------------------------------------------------------------//

osss_device_scheduler::~osss_device_scheduler()
{
}

//----------------------------------------------------------------------------//

osss::osss_polymorphic< osss_cycle_count >
osss_device_scheduler::getCycleValue( sc_dt::uint64 value ) const
{
  return m_for_device->getCycleValue( value );
}

//----------------------------------------------------------------------------//

const osss_device_job_descriptor&
osss_device_scheduler::osss_job_array::operator[](const osss_device_job_idx index) const
{
  return m_scheduler->m_for_device->getJob(index);
}

//----------------------------------------------------------------------------//

osss_device_scheduler::osss_job_array::osss_job_array(osss_device_scheduler * sched)
{
  m_scheduler = sched;
}

//----------------------------------------------------------------------------//


osss_device_job_idx
osss_device_scheduler::getNumberOfReconObjects() const
{
  return m_for_device->getNumberOfReconObjects();
}

//----------------------------------------------------------------------------//

/*
osss_device_scheduler::osss_job_index_type
osss_device_scheduler::osss_job_array::size() const
{
  // Warning: here we tranlate two STL size types
  return m_scheduler->m_for_device->getNumberOfReconObjects();
}
*/
} // end namespace osss
// $Id: osss_device_scheduler.cpp 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
