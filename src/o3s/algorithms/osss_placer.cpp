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

#include "osss_placer.h"
#include "o3s/structural/app/osss_recon_base.h"

namespace osss {

osss_recon_handle osss_placer::current_recon_object_handle = NULL;

osss_placer::osss_placer()
  : slots(this)
  , m_for_recon_object( current_recon_object_handle )
{ }

//----------------------------------------------------------------------------//

osss_placer::~osss_placer()
{
}

//----------------------------------------------------------------------------//

const osss_context_idx
osss_placer::getNumberOfContexts() const
{
  return m_for_recon_object->getNumberOfContexts();
}

const osss_type_idx
osss_placer::getNumberOfClasses() const
{
  return m_for_recon_object->getNumberOfClasses();
}

const osss_slot_idx
osss_placer::getNumberOfSlots() const
{
  return m_for_recon_object->getNumberOfSlots();
}


const bool
osss_placer::isConfigurationControllerBusy() const
{
  return m_for_recon_object->isConfigurationControllerBusy();
}



//----------------------------------------------------------------------------//
// Methods for "jobs" member
//----------------------------------------------------------------------------//

const osss_slot_descriptor&
osss_placer::osss_slot_array::operator[](const osss_slot_idx index) const
{
  return *(m_placer->m_for_recon_object->resolveSlotIdx(index));
}

//----------------------------------------------------------------------------//

osss_placer::osss_slot_array::osss_slot_array(osss_placer * placer)
{
  m_placer = placer;
}

//----------------------------------------------------------------------------//

} // end namespace osss
// $Id: osss_placer.cpp 2394 2008-07-01 20:45:28Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
