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

#include "osss_user_process_descriptor.h"
#include "o3s/utils/time_consumer.h"

namespace osss {
namespace osssi {

osss_user_process_descriptor::osss_user_process_descriptor()
  : m_new_class( false )
  , m_new_content( 0 )
  , m_context( 0 )
  , m_user_process_priority( 0 )
  , m_lazy_mode( false )
  , m_current_permission( 0 )
  , m_permission_obtained_anonymous( true )
  , m_id( 0 )
  , m_time_consumer( 0 )
{
  m_lock_obtain.write( false );
  m_lock_release.write( false );
  m_switch_and_permission_obtain.write( false );
  m_permission_release.write( false );
  m_create_logic.write( false );
  m_ack.write(false);
}

osss_user_process_descriptor::~osss_user_process_descriptor()
{
  delete m_time_consumer;
}

} // namespace osss
} // namespace osssi

// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
