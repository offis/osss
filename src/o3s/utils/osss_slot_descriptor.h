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

#ifndef OSSS_SLOT_DESCRIPTOR_H_INCLUDED
#define OSSS_SLOT_DESCRIPTOR_H_INCLUDED

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_recon_types.h"

namespace osss {
OSSS_SYNTHESISABLE_BEGIN_

struct osss_slot_descriptor
{
  osss_slot_descriptor()
    : m_allocated(false)
    // , m_context( ??? ) /// FIXME: no default value known?
    , m_class(NULL)
    , m_granted(false)
    , m_free(false)
  {}

  /** True, if currently used. That is, it contains
      a valid temporary or named context */
  bool                   m_allocated;

  /** Context currently configured here.
      May be the temporary context or a named one!
    */
  osss_context_idx       m_context;

  /** Type descriptor for the configured class.
      Carries NULL in case of an undefined state. */
  osss_type_const_handle m_class;

  /** Flag, if grant was given to a user process. */
  bool                   m_granted;

  /** This value is re-calculated and overwritten
      prior to each placement+scheduling.
      It indicates if this slot could be
      used for another context by the placer. */
  bool                   m_free;
};

OSSS_SYNTHESISABLE_END_
OSSS_SYNTHESISABLE_EXPORT_(osss_slot_descriptor);

} // end namespace osss

#endif // OSSS_SLOT_DESCRIPTOR_H_INCLUDED
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
