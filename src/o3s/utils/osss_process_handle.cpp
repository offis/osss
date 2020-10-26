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

/**
 * \file  osss_process_handle.cpp
 *
 */

#include "osss_process_handle.h"
#include "osss_process_registry.h"

namespace osss {
namespace osssi {

osss_process_handle::id_type
osss_process_handle::get_proc_id() const
{
#if defined(OSSS_SYSTEMC_2_2)
  if(!this->dynamic())
  {
    return osss_process_registry::get_proc_id(*this);
  }
  return 0;
#elif defined(OSSS_SYSTEMC_2_1)
  id_type id = osss_process_registry::get_proc_id(*this);
  if (0 == id)
  {
    std::cerr << "ERROR! "
              << __PRETTY_FUNCTION__ 
              << " returns illegal ID 0, should never happen!"
              << std::endl;
    OSSS_ABORT();
  }
  return id;
#endif
}

osss_process_handle
osss_process_handle::last_created()
{
  return sc_core::sc_get_last_created_process_handle();
}

#if defined(OSSS_SYSTEMC_2_1)

#define OSSS_CREATE_PROCESS_HELPER( process_type )             \
    osss_ ## process_type ## _handle                           \
    osss_create_ ## process_type ## _process                   \
        ( const char*               name_p,                    \
          sc_core::SC_ENTRY_FUNC    method_p,                  \
          sc_core::sc_process_host* host_p   )                 \
    {                                                          \
        sc_core::sc_module* host =                             \
          dynamic_cast< sc_core::sc_module* >( host_p );       \
        if (!host ) {                                          \
            std::cerr << "ERROR: Invalid process host: "       \
                      << host_p->name() << std::endl;          \
            OSSS_ABORT();                                      \
        }                                                      \
        sc_core::sc_simcontext* simc =                         \
            sc_core::sc_get_curr_simcontext();                 \
        sc_core::sc_ ## process_type ## _handle type_handle =  \
             simc->register_ ## process_type ## _process       \
                  ( name_p, method_p, host );                  \
        sc_core::sc_process_b* raw_handle =                    \
            osss_process_handle::last_created();               \
        return osss_ ## process_type ## _handle                \
                                  ( raw_handle, type_handle ); \
    }


#elif defined(OSSS_SYSTEMC_2_2)

#define OSSS_CREATE_PROCESS_HELPER( process_type )             \
    osss_ ## process_type ## _handle                           \
    osss_create_ ## process_type ## _process                   \
        ( const char*               name_p,                    \
          sc_core::SC_ENTRY_FUNC    method_p,                  \
          sc_core::sc_process_host* host_p   )                 \
    {                                                          \
        sc_core::sc_simcontext* simc =                         \
            sc_core::sc_get_curr_simcontext();                 \
        return simc->create_ ## process_type ## _process       \
                 ( name_p, false, method_p, host_p, 0 );       \
    }

#endif

OSSS_CREATE_PROCESS_HELPER( method  );
OSSS_CREATE_PROCESS_HELPER( thread  );
OSSS_CREATE_PROCESS_HELPER( cthread );

} // namespace osssi
} // namespace osss

// $Id: osss_process_handle.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
