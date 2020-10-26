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
#ifndef OSSS_MACROS_INCLUDED
#define OSSS_MACROS_INCLUDED

#include "o3s/utils/osss_systemc.h"

#ifndef OSSS_SYNTHESIS_

#include "o3s/utils/osss_process_handle.h"

//------------------------------------------------------------------------------

// For internal use only
#define _CLIENT_ID_ \
        ::osss::osssi::osss_process_handle::current().get_proc_id()

//------------------------------------------------------------------------------
// some extensions to SystemC in order to register processes with 
// explicit unique names
//------------------------------------------------------------------------------

#define osss_declare_unique_method_process(handle, name, host_tag, func) \
    {                                                                    \
        ::osss::osssi::osss_method_handle handle =                       \
            ::osss::osssi::osss_create_method_process(                   \
              ::sc_core::sc_gen_unique_name( name ),                     \
              SC_MAKE_FUNC_PTR( host_tag, func ), this );                \
        sensitive << handle;               \
        sensitive_pos << handle;           \
        sensitive_neg << handle;           \
    }
 
#define osss_declare_unique_thread_process(handle, name, host_tag, func)  \
    {                                                                     \
        ::osss::osssi::osss_thread_handle handle =                        \
            ::osss::osssi::osss_create_thread_process(                    \
              ::sc_core::sc_gen_unique_name( name ),                      \
              SC_MAKE_FUNC_PTR( host_tag, func ), this );                 \
        sensitive << handle;                \
        sensitive_pos << handle;            \
        sensitive_neg << handle;            \
    }
 
#define osss_declare_unique_cthread_process(handle, name, host_tag, func, edge) \
    {                                                                           \
        ::osss::osssi::osss_cthread_handle handle =                      \
            ::osss::osssi::osss_create_cthread_process(                  \
              ::sc_core::sc_gen_unique_name( name ),                     \
              SC_MAKE_FUNC_PTR( host_tag, func ), this );                \
        sensitive.operator()( handle, edge ); \
    }

  // the same as SC_CTHREAD
  #define OSSS_UNIQUE_CTHREAD(func, edge)                                \
      osss_declare_unique_cthread_process( func ## _handle,              \
                               #func,                                    \
                               SC_CURRENT_USER_MODULE,                   \
                               func,                                     \
                               edge )

  // the same as SC_METHOD
  #define OSSS_UNIQUE_METHOD(func)                                       \
      osss_declare_unique_method_process( func ## _handle,               \
                              #func,                                     \
                              SC_CURRENT_USER_MODULE,                    \
                              func )

  // the same as SC_THREAD
  #define OSSS_UNIQUE_THREAD(func)                                       \
      osss_declare_unique_thread_process( func ## _handle,               \
                              #func,                                     \
                              SC_CURRENT_USER_MODULE,                    \
                              func )
#else
 // the same as SC_CTHREAD
  #define OSSS_UNIQUE_CTHREAD(func, edge)                                \
     SC_CTHREAD(func, edge)

  // the same as SC_METHOD
  #define OSSS_UNIQUE_METHOD(func)                                       \
     SC_METHOD(func)

  // the same as SC_THREAD
  #define OSSS_UNIQUE_THREAD(func)                                       \
     SC_THREAD(func)
#endif // OSSS_SYNTHESIS_

#endif
// $Id: osss_macros.h 2769 2008-11-28 09:54:32Z philipph $
// :flavour: (all,H)
