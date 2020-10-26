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
#ifndef OSSS_SW_SHARED_IF_HH
#define OSSS_SW_SHARED_IF_HH

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_types.h"

namespace osss {

class osss_software_task;

/****************************************************************************
CLASS
      osss_sw_shared_hw_if

      This class implements a software shared hardware interface

IMPORTANT NOTES
      

****************************************************************************/

class osss_sw_shared_hw_if
  : virtual public sc_core::sc_interface
{
public:

  virtual
  void 
  registerHWModule() = 0;

  virtual
  void 
  setRequest() = 0;

  virtual
  bool 
  accessGranted() = 0;

  virtual
  void 
  cancelRequest() = 0;

  /*
  virtual
  sc_core::sc_time
  getPeriod() = 0;
  */

};

/****************************************************************************
CLASS
      osss_sw_shared_sw_if

      This class implements a software shared software interface

IMPORTANT NOTES
      

****************************************************************************/

class osss_sw_shared_sw_if
  : virtual public sc_core::sc_interface
{
public:

  virtual
  void 
  registerSWTask( osss_priority priority ) = 0;

  virtual
  void
  setPeriod( const sc_core::sc_time period) = 0;

  virtual
  bool
  pendingRequest() = 0;

  virtual
  void 
  grantAccess(bool access) = 0;

};

/****************************************************************************
CLASS
      osss_sw_shared_if

      This class implements a software shared interface

IMPORTANT NOTES
      Class template. Requires to pass a scheduler class and any kind of
      other class as template argument on instantiation.

****************************************************************************/

template<class IF = sc_core::sc_interface>
class osss_sw_shared_if
{

};

template<>
class osss_sw_shared_if<sc_core::sc_interface>
  : public osss_sw_shared_hw_if
  , public osss_sw_shared_sw_if
{
public:

  ///-----------------------------------------------------------------------
  // This operator must be used for calling methods on a software shared object.
  // The calls are redirected to m_objectPointer. This allows to specifiy
  // m_objectPointer protected.
  virtual
  sc_core::sc_interface *
  get_interface() const = 0;

  virtual
  osss_priority getPriority() = 0;

};

} // namespace osss

#endif
// $Id: osss_sw_shared_if.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
