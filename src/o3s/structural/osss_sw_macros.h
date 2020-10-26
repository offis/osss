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

#ifndef OSSS_SW_MACROS_HH
#define OSSS_SW_MACROS_HH

#include "o3s/utils/block_wrapper.h"

// Define a software task which must contain a main method. This method will be 
// executed as an SC_CTHREAD.
#define OSSS_SOFTWARE_TASK(name) \
    struct name : ::osss::osss_software_task

#define OSSS_SW_TASK(name) \
    struct name : ::osss::osss_software_task

// Define a task's constructor (similar to SC_CTOR)                             
#define OSSS_SW_CTOR(className) \
    className(sc_core::sc_module_name name)

#define OSSS_SOFTWARE_CTOR(className) \
    className(sc_core::sc_module_name name)

// Estimated execution time (EET). Defines the time it takes for a certain block
// to execute on the given processor. [Ideally these times could be determined
// by a tool and annotated]
// The EET is intended for simulation only. It will have no effect on the 
// generated code. The aim is to give the software code a meaningful (and more or 
// accurate) timing behaviour. It is by no way sufficient to write real-time 
// software. 
// Nesting EETs will result in a duration which is the sum of all sub-durations.
#define OSSS_EET(duration) \
   OSSS_IMPL_BLOCK_( ::osss::osss_waste_of_time, \
     (::osss::osss_timing_annotation_source_info ( \
        __FILE__, \
        __LINE__, \
        (::osss::osss_sw_shared_registry::getSwSharedObject(this) == NULL) ? \
        dynamic_cast<osss_sw_object_if*>(this) \
        : \
        dynamic_cast<osss_sw_object_if*>( \
           ::osss::osss_sw_shared_registry::getSwSharedObject(this)) ) \
     ), \
     duration )

// Required execution time. This can be seen as a kind of constraint. It 
// specifies the time it takes for a certain block of code to execute. If the
// code executes faster than the given duration the remaining time will be
// consumed. The execution times of the code within the RET block can be
// specified via EETs or RETs. 
// If the code executes slower than the given duration an error will be issued.
// RETs are intended to have an effect on the generated code.
// Nesting RETs is allowed. The resulting duration is a) the specified outer
// duration in the case there is no violation or b) the sum of the inner RETs
// (and an error message) in the case the inner RETs are greater than the outer
// duration.
#define OSSS_RET(duration) \
   OSSS_IMPL_BLOCK_( ::osss::osss_required_time, \
     (::osss::osss_timing_annotation_source_info ( \
        __FILE__, \
        __LINE__, \
        (::osss::osss_sw_shared_registry::getSwSharedObject(this) == NULL) ? \
        dynamic_cast<osss_sw_object_if*>(this) \
        : \
        dynamic_cast<osss_sw_object_if*>( \
           ::osss::osss_sw_shared_registry::getSwSharedObject(this)) ) \
     ), \
     duration)

#define OSSS_MAKE_PRE_AND_POST_METHOD_SW_SHARED(formal_method_signature, actual_method_signature) \
    void _pre_##formal_method_signature \
    { \
      osss_sw_shared_hw_if * sghwif = \
         ::osss::osss_sw_shared_registry::getSwSharedObject(this); \
      sghwif->setRequest(); \
      while ( !sghwif->accessGranted() ) \
      { \
        /* std::cout << sc_core::sc_time_stamp() <<"\t\t\t\t\t in the m_request waiting loop: " << __PRETTY_FUNCTION__ << std::endl; */ \
        sc_core::wait(); \
      } \
    } \
    void _post_##formal_method_signature \
    { \
      osss_sw_shared_hw_if * sghwif = \
         ::osss::osss_sw_shared_registry::getSwSharedObject(this); \
      sghwif->cancelRequest(); \
    }

#define OSSS_SW_SHARED_METHOD_VOID(return_type, formal_method_signature, actual_method_signature) \
    OSSS_MAKE_PRE_AND_POST_METHOD_SW_SHARED(formal_method_signature, actual_method_signature) \
    return_type formal_method_signature \
    { \
      _pre_##actual_method_signature; \
      _user_##actual_method_signature; \
      _post_##actual_method_signature; \
      sc_core::wait(); \
    } \
    return_type _user_##formal_method_signature

#define OSSS_SW_SHARED_METHOD(return_type, formal_method_signature, actual_method_signature) \
    OSSS_MAKE_PRE_AND_POST_METHOD_SW_SHARED(formal_method_signature, actual_method_signature) \
    return_type formal_method_signature \
    { \
      _pre_##actual_method_signature; \
      return_type return_value = _user_##actual_method_signature; \
      _post_##actual_method_signature; \
      return return_value; \
    } \
    return_type _user_##formal_method_signature

#define OSSS_SW_EXPORTED(methodName) \
    _user_##methodName
 
#endif
// $Id: osss_sw_macros.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
