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

#ifndef OSSS_USER_PROCESS_DESCRIPTOR_H
#define OSSS_USER_PROCESS_DESCRIPTOR_H

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/noncopyable.h"
#include "o3s/utils/osss_types.h"
#include "o3s/algorithms/osss_recon_scheduler.h"
#include "o3s/utils/osss_process_handle.h"
#include "o3s/utils/object_container.h"


namespace osss {
namespace osssi {

/** This class is used to maintain information on
    user processes.
  */
struct osss_user_process_descriptor
  : private noncopyable
{
  osss_user_process_descriptor();
  ~osss_user_process_descriptor();

  /** Value from user process. Indicates re-creation of context (type change!) */
  sc_core::sc_signal<bool> m_create_logic;
  /** Value from user process. Indicates external lock desired */
  sc_core::sc_signal<bool>  m_lock_obtain;
  /** Value from user process. Indicates external lock to be removed */
  sc_core::sc_signal<bool> m_lock_release;
  /** Value from user process. Indicates access permission desired */
  sc_core::sc_signal<bool> m_switch_and_permission_obtain;
  /** Value from user process. Indicates access permission to be returned */
  sc_core::sc_signal<bool> m_permission_release;

  /// Valid if m_create_logic==true: Class to be used in future
  const osss_type_descriptor * m_new_class;
  /// if != NULL: new content object
  osss_object                * m_new_content;
  /// The context for this request/return
  osss_context_base          * m_context;
  /// Signal to user process (answer from access controller, multicycle thread)
  /// It acknowledges permission request or logic create
  sc_core::sc_signal<bool>     m_ack;
  /// user process priority
  osss_priority                m_user_process_priority;

  // User-land information. We just maintain it here for simulation.
  // Is lazy permission return mode turned on?
  bool                         m_lazy_mode;
  // Does the user-defined process hold a permission?
  // NULL == no permission. Only NULL, addresses of named contexts
  // or address of m_temporary_context are permitted here.
  // Note: This field is also updated in-between requestPermission()
  // and returnPermission() calls now, not just in case of lazy mode.
  osss_context_base          * m_current_permission;
  // This flag indicates, if the user process doesn't know which context
  // it holds permission for. It is undefined in case of m_current_permission
  // being NULL.
  // true:  the permission request was performed on an anonymous context
  // false: the permission request was performed on a named context
  bool                         m_permission_obtained_anonymous;

  // To identify process, used by logging
  OSSS_Process_ID_t            m_id;

  // time consumer, for support of calling SC_THREADs
  time_consumer_base*          m_time_consumer;
};

} // namespace osss
} // namespace osssi

#endif // OSSS_USER_PROCESS_DESCRIPTOR_H

// :flavour: (osss,s) (osss_recon,s) (osss_full,s)
