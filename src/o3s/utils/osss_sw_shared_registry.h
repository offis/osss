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
#ifndef OSSS_SW_SHARED_REGISTRY_HH
#define OSSS_SW_SHARED_REGISTRY_HH

#include <iostream>
#include <map>

namespace osss {

//namespace osssi {

class osss_sw_shared_hw_if;

/****************************************************************************
CLASS
      osss_sw_shared_registry

      This class implements a registry for software shared objects
      Each user defined object which can be plugged into a software shared 
      object is associated with its surrounding software shared object 

IMPORTANT NOTES
      

****************************************************************************/

class osss_sw_shared_registry
{
public:

  static osss_sw_shared_hw_if* getSwSharedObject(const void* object);

  static void addPluggedObject(osss_sw_shared_hw_if* sghwif, const void* sgo);
  
  static void deletePluggedObject(const void* sgo);

  // for debugging
  static void print();

private:
  // first:  embedded object
  // second: software shared object containing the above object
  static std::map<const void*, osss_sw_shared_hw_if* > osss_sw_shared_map;
};

//} // namespace osssi  

} //namespace osss

#endif
// $Id: osss_sw_shared_registry.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
