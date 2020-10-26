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
#ifndef OSSS_RECON_MACROS_HH
#define OSSS_RECON_MACROS_HH

#include "o3s/structural/app/osss_recon_locks.h"

// Use this macro in a user defined class for context content.
// It defines a new release guard (inner lock).
#define OSSS_NEW_RELEASE_GUARD( condition )\
     virtual bool _internal_lock_release_guard_method_(){ return ( condition ); }

// Use this macro in a user defined class for context content.
// It sets the guard to "condition AND inherited condition".
#define OSSS_AND_RELEASE_GUARD( condition , old_class )\
     virtual bool _internal_lock_release_guard_method_()\
     { return ( (condition) && old_class::_internal_lock_release_guard_method_() ); }

// Use this macro in a user defined class for context content.
// It sets the guard to "condition OR inherited condition".
#define OSSS_OR_RELEASE_GUARD( condition , old_class )\
     virtual bool _internal_lock_release_guard_method_()\
     { return ( (condition) || old_class::_internal_lock_release_guard_method_() ); }

// For use in SC_CTOR. Allows the user to declare a member
// attribute to be transient. "value" should receive the
// reset value for that attribute.
#define OSSS_TRANSIENT( type , name , value )\
    name = value;\
    osss::osssi::osss_clear_helper_registry::instance.add(this , &name , value);

// For use in SC_CTOR. Allows the user to declare a member
// attribute to be transient. "value" should receive the
// reset value for that attribute.
#define OSSS_TRANSIENT_ARRAY( name )
// this statement has synthesis effects only

// Declare copy and configuration times for a RTTI type on an device type
#define OSSS_DECLARE_TIME( DeviceType , TargetType,                      \
                           AttributeCopyTime, LogicReconfigurationTime ) \
   osss::osss_declare_time<TargetType,void>                              \
     ( (DeviceType), (AttributeCopyTime), (LogicReconfigurationTime) )

// Declare timing for differential bitstream
#define OSSS_DECLARE_SHORTCUT_TIME( DeviceType, TargetType, SourceType,  \
                           AttributeCopyTime, LogicReconfigurationTime ) \
  osss::osss_declare_time<TargetType,SourceType>                         \
    ( (DeviceType), (AttributeCopyTime), (LogicReconfigurationTime) )

// Declare timing, depending on recon object instance
#define OSSS_DECLARE_SHORTCUT_TIME_FOR_RECON( DeviceType                 \
                                            ,  TargetType, SourceType    \
                                            , AttributeCopyTime          \
                                            , LogicReconfigurationTime   \
                                            , ReconObject )              \
  osss::osss_declare_time<TargetType,SourceType>                         \
     ( (DeviceType), (AttributeCopyTime),                                \
       (LogicReconfigurationTime), ReconObject )

/**
 * \def   OSSS_PURE_VIRTUAL_VOID()
 * \brief mark an user-class void method as pure virtual
 *
 * Include this macro in each virtual void method in base classes
 * that should not be called directly.  Results in a run-time
 * error, if this method is called during simulation.
 *
 * During synthesis, this macro is ignored.
 */
#define OSSS_PURE_VIRTUAL_VOID() \
    OSSS_REPORT_ERROR( osss::report::pure_virtual_call ) \
      % __PRETTY_FUNCTION__

/**
 * \def   OSSS_PURE_VIRTUAL_RETURN()
 * \brief mark an user-class non-void method as pure virtual
 *
 * Include this macro in each virtual non-void method in base classes
 * that should not be called directly.  Results in a run-time
 * error, if this method is calle during simulation.
 *
 * \param ReturnValue  Default value to return during synthesis.
 *
 * During synthesis, this macro expands to
 * \code
 *   return (ReturnValue)
 * \endcode
 * to enable at least a deterministic result.
 *
 */
#define OSSS_PURE_VIRTUAL_RETURN( ReturnValue ) \
    OSSS_REPORT_ERROR( osss::report::pure_virtual_call ) \
      % __PRETTY_FUNCTION__; \
    return (ReturnValue) /* to be safe */


#endif // OSSS_RECON_MACROS_HH
// $Id: osss_recon_macros.h 3026 2009-03-04 22:55:31Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_full,h)
