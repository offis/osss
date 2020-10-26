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

#include "osss_registry_shared.h"
#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_report.h"

namespace osss {
namespace osssi {

//----------------------------------------------------------------------------//

::osss::osss_shared_if<sc_core::sc_interface>* 
osss_registry_shared::get_shared_object(const void* const object)
{
  std::map<const void*, 
           ::osss::osss_shared_if<sc_core::sc_interface>*>::iterator map_iter;
  map_iter = osss_shared_map.find(object);
  if (map_iter != osss_shared_map.end())
    return (*map_iter).second;
  else
  {
    std::cerr << "Shared object not found" << std::endl;
    OSSS_ABORT();
    return 0;
  }
}

//----------------------------------------------------------------------------//

const void* 
osss_registry_shared::
get_plugged_object(const ::osss::osss_shared_if<sc_core::sc_interface>* sif)
{
  for(std::map<const void*, 
        ::osss::osss_shared_if<sc_core::sc_interface>*>::iterator iter = 
        osss_shared_map.begin(); iter != osss_shared_map.end(); iter++)
  {
    if( iter->second == sif)
      return iter->first;
  }
  return NULL;
}

//----------------------------------------------------------------------------//

void 
osss_registry_shared::
add_plugged_object(::osss::osss_shared_if<sc_core::sc_interface>* so, 
		   const void* po)
{
  osss_shared_map[po] = so;
}

//----------------------------------------------------------------------------//

void 
osss_registry_shared::delete_plugged_object(const void* po)
{
  osss_shared_map.erase(po);
}

//----------------------------------------------------------------------------//

void 
osss_registry_shared::print()
{
  using std::cout; using std::endl;
  cout << endl;
  cout << "osss registry for shared objects" << endl;
  cout << "embedded object address -> shared object address" << endl;
  cout << "------------------------------------------------" << endl;
  for (std::map<const void*, 
	        ::osss::osss_shared_if<sc_core::sc_interface>*>::iterator it = 
	 osss_shared_map.begin();
       it != osss_shared_map.end();
       ++it)
  {
    cout << (*it).first << " -> " << (*it).second << endl;
  }
  cout << endl;
}

//----------------------------------------------------------------------------//

std::map<const void*, ::osss::osss_shared_if<sc_core::sc_interface>*> 
osss_registry_shared::osss_shared_map = 
  std::map<const void*, ::osss::osss_shared_if<sc_core::sc_interface>*>();

//----------------------------------------------------------------------------//

} // namespace osssi

} // namespace osss
// $Id: osss_registry_shared.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
