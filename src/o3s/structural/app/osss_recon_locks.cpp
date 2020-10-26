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

#include "osss_recon_locks.h"
#include "osss_context_base.h"
#include "osss_recon_base.h"

namespace osss {
namespace osssi {

keep_enabled_block::keep_enabled_block( osss_context_base& ctx )
  : delegate_block_wrapper(
      new handle_t< osss_context_base
                  , &osss_context_base::lock
                  , &osss_context_base::unlock >( ctx ) )
{}

#if 0
keep_enabled_block::keep_enabled_block( osss_recon_base& recon )
  : delegate_block_wrapper(
      new handle_t< osss_recon_base
                  , &osss_recon_base::lock
                  , &osss_recon_base::unlock >( recon ) )
{}
#endif

keep_permission_block::keep_permission_block( osss_context_base& ctx )
  : delegate_block_wrapper(
      new handle_t< osss_context_base
                  , &osss_context_base::startLazy
                  , &osss_context_base::endLazy >( ctx ) )
{}

keep_permission_block::keep_permission_block( osss_recon_base& recon )
  : delegate_block_wrapper(
      new handle_t< osss_recon_base
                  , &osss_recon_base::startLazy
                  , &osss_recon_base::endLazy >( recon ) )
{}

} // namespace osssi
} // namespace osss

// $Id: osss_recon_locks.cpp 2427 2008-07-07 16:56:30Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
