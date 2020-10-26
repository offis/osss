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

#include "o3s/algorithms/osss_cycle_count.h"
#include "o3s/algorithms/osss_placer.h"
#include "o3s/algorithms/osss_recon_scheduler.h"

namespace osss {
namespace osssi {

template< class CycleCountClass >
void
osss_recon_base::setCycleCounter()
{
  if (NULL == m_cycle_count_factory)
  {
    m_cycle_count_factory =
      new osss::osss_cycle_count::factory< CycleCountClass >();
  }
  else
  {
    std::cerr << "ERROR! Cycle count class already set for recon object \""
              << name() << "\"" << std::endl;
    OSSS_ABORT();
  }
}

template< class SchedulerClass >
void
osss_recon_base::setScheduler()
{
  if (NULL == m_scheduler_factory)
  {
    m_scheduler_factory =
      new osss_recon_scheduler::factory< SchedulerClass >( this );
  }
  else
  {
    std::cerr << "ERROR! Scheduler class already set for recon object \""
              << name() << "\"" << std::endl;
    OSSS_ABORT();
  }
}

template< class PlacerClass >
void
osss_recon_base::setPlacer()
{
  if (NULL == m_placer_factory)
  {
    m_placer_factory =
      new osss_placer::factory< PlacerClass >( this );
  }
  else
  {
    std::cerr << "ERROR! Placer class already set for recon object \""
              << name() << "\"" << std::endl;
    OSSS_ABORT();
  }
}

} // end namespace osssi
} // end namespace osss

// $Id: osss_recon_base.tpp 2427 2008-07-07 16:56:30Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
