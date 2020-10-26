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

#ifndef OSSS_RECON_TYPES_H_INCLUDED_
#define OSSS_RECON_TYPES_H_INCLUDED_

#include "o3s/utils/osss_types.h"
#include "o3s/datatypes/osss_vector.h"

namespace osss {

namespace osssi {
  class osss_context_base;
  class osss_recon_base;
  class osss_type_descriptor;
  class osss_user_process_descriptor;
}

OSSS_SYNTHESISABLE_BEGIN_ // Visible to user code in schedulers

  class osss_slot_descriptor;
  class osss_recon_job_descriptor;
  class osss_device_job_descriptor;

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_( osss_slot_descriptor       );
OSSS_SYNTHESISABLE_EXPORT_( osss_recon_job_descriptor  );
OSSS_SYNTHESISABLE_EXPORT_( osss_device_job_descriptor );

// We build pools of these
typedef osssi::osss_context_base *            osss_context_handle;
typedef osssi::osss_recon_base *              osss_recon_handle;
typedef osssi::osss_user_process_descriptor * osss_user_process_handle;
//typedef osssi::osss_type_descriptor         * osss_type_handle;

typedef osssi::osss_context_base const *      osss_context_const_handle;
typedef osssi::osss_type_descriptor const *   osss_type_const_handle;

// We don't want pool forward declarations here, so we define
// abstract index/size types
typedef osss_index_type osss_user_process_idx;
typedef osss_index_type osss_context_idx;

// We build vectors of these (user visible)
typedef osss_vector< osss_recon_job_descriptor >::size_type   osss_recon_job_idx;
typedef osss_vector< osss_device_job_descriptor >::size_type  osss_device_job_idx;
typedef osss_vector< osssi::osss_type_descriptor >::size_type osss_type_idx;
typedef std::vector< osss_slot_descriptor >::size_type        osss_slot_idx;
typedef std::vector< osss_recon_handle >::size_type           osss_recon_idx;

namespace osssi
{
  // helper functions to avoid inclusion of osss_user_process_descriptor

  // read-access to m_current_permission
  osss_context_base *
  whichCurrentPermission(osss_user_process_descriptor *);

  // read access to m_context
  osss_context_base *
  whichLastContext(osss_user_process_descriptor *);
} // namespace osssi

} // namespace osss

#endif // OSSS_RECON_TYPES_H_INCLUDED_

// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
