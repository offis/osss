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

#ifndef OSSS_SW_SHARED_HH
#define OSSS_SW_SHARED_HH

#include "o3s/structural/app/osss_sw_object_if.h"
#include "o3s/utils/osss_sw_shared_registry.h"
#include "o3s/structural/app/osss_sw_shared_if.h"

namespace osss {

/****************************************************************************
CLASS
      osss_sw_shared

      This class implements a software shared object, that means a container 
      for a user-defined class, that allows accesses only by the grant of
      a software task.

IMPORTANT NOTES
      Class template. Requires to pass any kind of user defined class, 
      as template argument on instantiation.

****************************************************************************/

template<class ObjectType>
class osss_sw_shared
  : public sc_core::sc_module
  , public osss_sw_shared_if<>
  , public osss_sw_object_if
{
public:

  // unused 
  sc_core::sc_in< bool > clock_port;
  
  ///-----------------------------------------------------------------------
  // Reset port. When the signal (bound to this port) changes, the 
  // reset_process (below) is executed once
  sc_core::sc_in< bool > reset_port;
 
  ///-----------------------------------------------------------------------
  // Needed for the reset_process (below)
  SC_HAS_PROCESS(osss_sw_shared);

  void reset_sensitivity(const osss_sensitivity sens);

  ///-----------------------------------------------------------------------
  // Default constructor. Calls the default constructor of the embedded
  // user-defined class instance and registers 
  // the reset_process (as SC_METHOD) at the SystemC scheduler.
  //
  // The optional name parameter enables the specification of a module
  // name as known from SystemC modules.
  explicit
  osss_sw_shared( sc_core::sc_module_name =
                    sc_core::sc_gen_unique_name( "osss_sw_shared" ) );

  ///-----------------------------------------------------------------------
  // Destructor. Frees memory allocated for m_objectPointer.
  ~osss_sw_shared();

  ///-----------------------------------------------------------------------
  // This operator must be used for calling methods on a software shared
  // object.
  // The calls are redirected to m_objectPointer. This allows to specifiy
  // m_objectPointer protected.
  inline
  ObjectType *
  operator->() const;

  inline
  sc_core::sc_interface*
  get_interface() const;

  ObjectType & 
  plugged_object();

  const char* swObjectKind() const
  {
    return "osss_sw_shared";
  }

  //****************************
  // hw interface
  //****************************

  ///------------------------------------------------------------------------
  // registers the associated sc_module
  void  registerHWModule();

  void setRequest();
  bool accessGranted();
  void cancelRequest();
  sc_core::sc_time getPeriod();

  //****************************
  // sw interface
  //****************************

  ///-----------------------------------------------------------------------
  // registers the associated osss_software_task
  void
  registerSWTask( osss_priority priority );

  bool pendingRequest(); 
  void grantAccess(bool access);
  void setPeriod(const sc_core::sc_time period);

  osss_priority getPriority();

protected:

  virtual void before_end_of_elaboration();

  ///-----------------------------------------------------------------------
  // Reset Process. Is called once when the signal bound to the reset_port
  // changes.
  void reset_process();
  void reset_method();

  ///-----------------------------------------------------------------------
  // This pointer points to an instance of the user-defined class that was
  // passed as template argument to the software shared object at declaration.
  ObjectType *m_objectPointer;

private:

  osssi::osss_method_handle m_reset_process_handle;

  osss_sensitivity m_reset_sensitivity;

  bool             m_swTaskRegistered;
  sc_core::sc_time m_period;
  bool             m_hwModuleRegistered;
  osss_priority    m_priority;

  bool m_request;
  bool m_access_granted;

  ///-----------------------------------------------------------------------
  // This method is called by the reset process
  inline
  void
  reset();

};

} // namespace osss

#include "o3s/structural/app/osss_sw_shared.tpp"

#endif /* OSSS_SW_SHARED_HH */
// $Id: osss_sw_shared.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
