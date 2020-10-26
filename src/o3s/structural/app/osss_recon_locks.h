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

#ifndef OSSS_RECON_LOCKS_H_INCLUDED
#define OSSS_RECON_LOCKS_H_INCLUDED

#include "o3s/utils/block_wrapper.h"

/**
 * \file  osss_recon_locks.h
 * \brief Reconfiguration control mechanisms based on locks
 *
 * \see   OSSS_KEEP_ENABLED(), OSSS_KEEP_PERMISSION()
 */

/**
 * \def OSSS_KEEP_ENABLED()
 * \brief Externally lock context from being disabled
 *
 * With an OSSS_KEEP_ENABLED() annotation, an enclosed code section
 * can rely on the fact, that a given context is not being disabled.
 * This can be useful to reduce thrashing effects due to competing
 * reconfiguration requests.
 *
 * \param NamedContext  Context, that should stay enabled.
 *
 * Forcing the context to stay enabled does \em NOT guarantee
 * exclusive access to the context itself.  This means, that requests
 * from other processes to the same context are scheduled and granted
 * independently by the currently scheduler.  Only requests, that
 * would require the current context to be disabled (due to
 * reconfiguration or context switch) are blocked, once the
 * selected context became enabled.
 *
 * Example:
 * \code
 *   osss_context< user_class > ctx;
 *   // ...
 *   OSSS_KEEP_ENABLED( ctx )   // Non-blocking, one cycle to enter.
 *   {
 *
 *     // do something
 *     ctx = some_class();
 *     ctx->some_method();
 *
 *   } // Context may become disabled again.
 *     // Leaving this block lasts one clock cycle.
 * \endcode
 *
 * As soon as the block is left, the context may become disabled
 * again, if the scheduler decides to.
 *
 * \note Entering such a section does not enable the context.
 *
 * \see OSSS_KEEP_PERMISSION()
 */
#define OSSS_KEEP_ENABLED( NamedContext ) \
  OSSS_IMPL_BLOCK_( ::osss::osssi::keep_enabled_block, NamedContext )

/**
 * \def OSSS_KEEP_PERMISSION()
 * \brief return a granted context permission as late as possible
 *
 * \param ContextOrRecon  Named Context of whole Recon-Object for which
 *                        the permission handling should change
 *
 * The OSSS_KEEP_PERMISSION() annotation changes the client-side behaviour
 * with respect to permission handling.  Usually, the granted permission
 * for a Named Context or Recon-Object request is returned immediately
 * after the corresponding action is finished.  This ensures, that multiple
 * clients can access the Context (and maybe even the shared Recon-Object)
 * as soon as possible.
 *
 * On the other hand, within an OSSS_KEEP_PERMISSION() block, the client
 * \em keeps the permission as long as possible.  This means, that once a
 * particular grant is given, this grant is not returned to the access
 * controller after the current request, but kept until
 *
 *   \li the OSSS_KEEP_PERMISSION() block is left, or
 *   \li another Context (on the same Recon-Object) is accessed
 *
 * Example:
 * \code
 *   osss_recon<>               recon1;
 *   osss_context< user_class > ctx1, ctx2; // bound to recon1
 *   // ...
 *   OSSS_KEEP_PERMISSION( recon1 ) // change permission handling of recon1
 *   {                              //   (not blocking)
 *
 *     // do something with the contexts
 *     ctx1 = some_class();         // blocking, until permission received
 *     ctx1->some_method();         //   we still have the permission
 *
 *     // do something with another context
 *                                  // return kept permission of ctx1
 *     ctx2->do_something();        // request permission of ctx2
 *                                  //    (scheduler kicks in here!)
 *     ctx2->do_more();             //   we still have the permission
 *
 *   } // unset "keep permission" mode
 * \endcode
 *
 * The major benefit of such sections is the reduced communication overhead
 * due to unneeded interaction with the access controller upon each
 * request.  The can speedup tight loops with multiple requests to the same
 * Recon-Object or Context.
 *
 * \see OSSS_KEEP_ENABLED()
 */
#define OSSS_KEEP_PERMISSION( ContextOrRecon ) \
    OSSS_IMPL_BLOCK_( ::osss::osssi::keep_permission_block, ContextOrRecon )

namespace osss {
namespace osssi {

/* forward declarations */
class osss_context_base;
class osss_recon_base;

/// internal helper class for OSSS_KEEP_ENABLED() blocks
struct keep_enabled_block
  : delegate_block_wrapper
{
  explicit keep_enabled_block( osss_context_base& );
//  explicit keep_enabled_block( osss_recon_base& );
};

/// internal helper class for OSSS_KEEP_PERMISSION() blocks
struct keep_permission_block
  : delegate_block_wrapper
{
  explicit keep_permission_block( osss_recon_base& );
  explicit keep_permission_block( osss_context_base& );
};

} // namespace osssi
} // namespace osss

#endif  // OSSS_RECON_LOCKS_H_INCLUDED
// $Id: osss_recon_locks.h 2875 2008-12-09 12:08:34Z andreas $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
