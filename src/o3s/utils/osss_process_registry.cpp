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


#include "osss_common.h"
#include "osss_systemc.h"
#include "osss_report.h"
#include "osss_process_registry.h"
#include "osss_process_handle.h"

namespace osss {

namespace osssi {

// local typedef to ease implementation
typedef osss_process_registry::id_type id_type;

//----------------------------------------------------------------------------//

id_type
osss_process_registry::get_proc_id(const osss_process_handle& proc_handle)
{
  if (proc_handle.valid())
  {
    sc_core::sc_object* proc_obj = proc_handle.get_process_object();
    std::map<const sc_core::sc_object*, id_type>::const_iterator map_iter
         = osss_process_id_map.find( proc_obj );
    if (map_iter != osss_process_id_map.end())
      return (*map_iter).second;
    else
    {
      osss_process_id_map.insert(std::make_pair(proc_obj,++m_id_counter));
      return m_id_counter;
    }
  } else {
    return 0;
  }
}

const char *
osss_process_registry::get_proc_name(const id_type id)
{
   std::map<const sc_core::sc_object*, id_type>::iterator pos;
   for(pos = osss_process_id_map.begin();
       pos != osss_process_id_map.end();
       ++pos)
   {
     if (pos->second == id)
     {
       return pos->first->name();
     }
   }
   std::cerr << "ERROR: " << __func__ << "() cannot lookup id " << id << std::endl;
   OSSS_ABORT();
   return 0;
}

//----------------------------------------------------------------------------//

void
osss_process_registry::print()
{
  using std::cout; using std::endl;
  cout << endl;
  cout << "osss registry for systemc processes" << endl;
  cout << "systemc process address (process name) -> unique ID" << endl;
  cout << "---------------------------------------------------" << endl;
  for (std::map<const sc_core::sc_object*,id_type>::iterator it
           = osss_process_id_map.begin();
       it != osss_process_id_map.end();
       ++it)
  {
    cout << (*it).first
         << " (" << (*it).first->name() << ")"
         << " -> " << (*it).second << endl;
  }
  cout << endl;
}

//----------------------------------------------------------------------------//

std::map<const sc_core::sc_object*, id_type>
osss_process_registry::osss_process_id_map =
  std::map<const sc_core::sc_object*, id_type>();

//----------------------------------------------------------------------------//

id_type osss_process_registry::m_id_counter = 0;

//----------------------------------------------------------------------------//

} // namespace osssi

} // namespace osss
// $Id: osss_process_registry.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
