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

#ifndef OSSS_CHANNEL_MACROS_HH
#define OSSS_CHANNEL_MACROS_HH

#include "o3s/utils/osss_systemc.h"
#include "o3s/communication/arch/osss_channel/osss_port_list_element.h"

#define OSSS_BIND void buildPortList()

#define OSSS_GENERATE void buildConnectionList()

#define OSSS_CONTROLLER_CTOR( ClassName )                     \
   SC_HAS_PROCESS( ClassName );                               \
   explicit                                                   \
   ClassName( sc_core::sc_module_name name =                  \
                 sc_core::sc_gen_unique_name( #ClassName ) )  \
   {}                                                         \
   void __osss_buildProcess()

#define osss_reg_port(_port_) \
        ::osss::make_port_list_element \
	  ( &( _port_ ), std::string( #_port_ ) )

#define osss_reg_vector(_vectorPort_) \
        ::osss::make_vector_port_list_element \
          ( &( _vectorPort_), std::string( #_vectorPort_ ) )

//------------------------------------------------------------------------------

#define OSSS_REGISTER_TRANSACTOR( Class, Interface ) \
  namespace osss {                                   \
    template<>                                       \
    struct getTransactorType < Interface >           \
    {                                                \
      typedef Class return_t;                        \
    }; \
  } /* namespace osss */

//------------------------------------------------------------------------------

#define OSSS_BEGIN_TRACE {

#define OSSS_BEGIN_NEW_TRACE(_trace_type_, _trace_file_name_)  \
        { \
          sc_trace_file* tf = \
            sc_create_ ##_trace_type_## _trace_file( #_trace_file_name_ ); \

#define OSSS_TRACE(_module_, _port_) \
          _module_->_port_.trace(_module_->name()); \

#define OSSS_END_TRACE(_channel_module_, _trace_file_) \
          _channel_module_.trace(_trace_file_); \
        } \

#define OSSS_END_NEW_TRACE(_channel_module_) \
          _channel_module_.trace(tf); \
        } \

#endif
// $Id: osss_channel_macros.h 2832 2008-12-04 09:29:07Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
