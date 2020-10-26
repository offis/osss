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

#ifndef OSSS_DEVICE_JOB_DESCRIPTOR_H
#define OSSS_DEVICE_JOB_DESCRIPTOR_H

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_recon_types.h"
#include "o3s/datatypes/osss_polymorphic.h"
#include "o3s/algorithms/osss_cycle_count.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

struct osss_device_job_descriptor
{
  /// Is the request valid at all? If no, the other entries are undefined.
  bool                          m_valid;
  /// cycle count for re-configuring
  osss_polymorphic< osss_cycle_count > m_configuration_cycles;
  /// new class id for context and slot (after possible re-creation)
  osss_type_const_handle        m_new_logic;
  /// old class id
  osss_type_const_handle        m_old_logic;
  /// slot
  osss_slot_idx                 m_location;
};

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_(osss_device_job_descriptor);

} // end namespace osss

#endif // OSSS_DEVICE_JOB_DESCRIPTOR_H
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
