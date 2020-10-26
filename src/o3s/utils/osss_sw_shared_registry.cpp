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

#include "osss_sw_shared_registry.h"

namespace osss {

//namespace osssi {

osss_sw_shared_hw_if* 
osss_sw_shared_registry::getSwSharedObject(const void* object)
{
  std::map<const void*, osss_sw_shared_hw_if*>::iterator map_iter;
  map_iter = osss_sw_shared_map.find(object);
  if (map_iter != osss_sw_shared_map.end())
    return (*map_iter).second;
  else
    return NULL;
}

void 
osss_sw_shared_registry::addPluggedObject(osss_sw_shared_hw_if* sghwif, const void* sgo)
{
  osss_sw_shared_map[sgo] = sghwif;
}

void 
osss_sw_shared_registry::deletePluggedObject(const void* sgo)
{
  osss_sw_shared_map.erase(sgo);
}

void 
osss_sw_shared_registry::print()
{
  using std::cout; using std::endl;

  cout << endl;
  cout << "osss software shared registry" << endl;
  cout << "embedded object address -> software shared object address" << endl;
  cout << "---------------------------------------------------------" << endl;
  for (std::map<const void*, osss_sw_shared_hw_if*>::iterator it = 
	 osss_sw_shared_map.begin();
       it != osss_sw_shared_map.end();
       ++it)
  {
    cout << (*it).first << " -> " << (*it).second << endl;
  }
  cout << endl;
}

std::map<const void*, osss_sw_shared_hw_if*> 
osss_sw_shared_registry::osss_sw_shared_map = 
  std::map<const void*, osss_sw_shared_hw_if*>();

//} // namespace osssi

} // namespace osss
// $Id: osss_sw_shared_registry.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_full,s) (osss_icodes,s)
