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

#include "o3s/utils/osss_systemc.h"
#include "o3s/communication/arch/osss_channel/osss_basic_channel.h"
#include "o3s/utils/osss_global_port_registry.h"
#include "o3s/communication/arch/osss_rmi/osss_object_registry.h"
#include "o3s/communication/arch/osss_channel/osss_via_list_element.h"

namespace osss {

osss_basic_channel::osss_basic_channel(const std::string& name )
  : osssi::osss_clock_reset_if()
//  , m_reset_module( new osss_channel_reset_module )
  , m_object_registry( new osssi::osss_object_registry )
  , m_complete_method_called( false )
  , m_name( name )
  , m_numTransactors(0)
//  , m_reset_module_clock_bound( false )
//  , m_reset_module_reset_bound( false )
  , m_trace_file(NULL)
  , m_trace_all_file(NULL)
  , m_channel_graph_file_name()
{}

osss_basic_channel::~osss_basic_channel()
{
  delete m_object_registry;
//  delete m_reset_module;

  while( m_sharedSignalList.begin() != m_sharedSignalList.end() )
  {
    delete m_sharedSignalList.back();
    m_sharedSignalList.pop_back();
  }
}

void
osss_basic_channel::trace(sc_core::sc_trace_file* tf)
{
  if ((m_trace_file == NULL) && (m_trace_all_file == NULL))
    m_trace_file = tf;
  else
  {
    std::cout << "Warning: Channel " 
              << this->name()
              << " is already registered for tracing!" << std::endl;
  }
}

void
osss_basic_channel::trace_all(sc_core::sc_trace_file* tf)
{
  if ((m_trace_file == NULL) && (m_trace_all_file == NULL))
    m_trace_all_file = tf;
  else
  {
    std::cout << "Warning: Channel " 
              << this->name()
              << " is already registered for tracing!" << std::endl;
  }
}

void
osss_basic_channel::register_trace()
{
  if(m_trace_file)
  {
    std::vector< osss_signal_list_element_base* >::iterator signal_iter;
    for(signal_iter = m_sharedSignalList.begin(); 
        signal_iter != m_sharedSignalList.end(); 
        ++signal_iter)
    { 
      (*signal_iter)->trace(m_trace_file);
    }
  }
}

void
osss_basic_channel::register_trace_all()
{
  if(m_trace_all_file)
  {
    //multi signal list
    if ((getMultiSignalList() != NULL) && 
        (!getMultiSignalList()->empty()))
    {
      for(unsigned int i=0; i<getMultiSignalList()->size(); i++)
      {
        std::ostringstream ostr;
        ostr << name() << "_transactor_" << i << "_";
        std::string prefix = ostr.str();   

        for(unsigned int j=0; j<getMultiSignalList()->at(i)->size(); j++)
        {
          std::string str = prefix + 
            getMultiSignalList()->at(i)->at(j)->getAlias();
          getMultiSignalList()->at(i)->at(j)->trace(m_trace_all_file, str);
        }
      }
    }

    //shared signal list
    if ((getSharedSignalList() != NULL) && 
        (!getSharedSignalList()->empty()))
    {
      for(unsigned int i=0; i<getSharedSignalList()->size(); i++)
      {
        std::string str = name() + std::string("_shared_")  + 
          getSharedSignalList()->at(i)->getAlias();
        getSharedSignalList()->at(i)->trace(m_trace_all_file, str);
      }
    }
  
    //mux via list
    if ((getMuxViaList() != NULL) &&
        (!getMuxViaList()->empty()))
    {
      for(unsigned int i=0; i<getMuxViaList()->size(); i++)
      {
        getMuxViaList()->at(i)->trace_all(m_trace_all_file, name());
      }
    }

    //or via list
    if ((getOrViaList() != NULL) &&
        (!getOrViaList()->empty()))
    {
      for(unsigned int i=0; i<getOrViaList()->size(); i++)
      {
        getOrViaList()->at(i)->trace_all(m_trace_all_file, name());
      }
    }

    //and via list
    if ((getAndViaList() != NULL) &&
        (!getAndViaList()->empty()))
    {
      for(unsigned int i=0; i<getAndViaList()->size(); i++)
      {
        getAndViaList()->at(i)->trace_all(m_trace_all_file, name());
      }
    }

    //and-or via list
    if ((getAndOrViaList() != NULL) &&
        (!getAndOrViaList()->empty()))
    {
      for(unsigned int i=0; i<getAndOrViaList()->size(); i++)
      {
        getAndOrViaList()->at(i)->trace_all(m_trace_all_file, name());
      }
    }
  }
}

#if 0
void
osss_basic_channel::check_clock_reset_port()
{
  osssi::osss_clock_reset_if::check_clock_reset_port();
  if (m_reset_module)
  {
    //perform the reset module binding if necessary
    if (! m_reset_module_clock_bound)
    {
      if (osss_global_port_registry::is_clock_port_registered())
        m_reset_module->clock_port_in(osssi::osss_clock_reset_if::clock_port_port);
    }

    if(! m_reset_module_reset_bound)
    {
      if (osss_global_port_registry::is_reset_port_registered())
        m_reset_module->reset_port_in(osssi::osss_clock_reset_if::reset_port_port);
    }
  }
}
#endif

/**
 * Prints the internal structure of the OSSS-Channel:
 *  - number of connected masters
 *  - number of connected slaves
 *  - total number of transactors
 *  - multi signal list (signals that are used exclusively, i.e. not shared)
 *  - shared signal list
 *  - via list (of type MUX, or, and-or, and)
 */
std::ostream&
operator<< (std::ostream& o, osss_basic_channel & ch)
{
  o << "osss-channel info for: " << ch.name() << std::endl
    << "-----------------------";
  o << std::endl;
  o << "# masters: " << ch.getNumMasters() 
    << " # slaves: " << ch.getNumSlaves()
    << " # transactors: " << ch.getNumTransactors() 
    << std::endl << std::endl;

  o << "multi signal list:" << std::endl
    << "------------------" << std::endl;
  if (ch.getMultiSignalList() != NULL)
  {
    if (ch.getMultiSignalList()->empty())
      o << "  empty" << std::endl;
    else
    {
      for(unsigned int i=0; i<ch.getMultiSignalList()->size(); i++)
      {
        o << "transactor " << i << ":" << std::endl;
        for(unsigned int j=0; j<ch.getMultiSignalList()->at(i)->size(); j++)
        {
          o << "  (alias) " << ch.getMultiSignalList()->at(i)->at(j)->getAlias()
            << std::endl;
        }
        o << std::endl;
      }
    }
  }
  else
    o << "  none" << std::endl;
  o << std::endl;

  o << "shared signal list:" << std::endl
    << "--------------------" << std::endl;
  if (ch.getSharedSignalList() != NULL)
  {
    if (ch.getSharedSignalList()->empty())
      o << "  empty" << std::endl;
    else
    {
      for(unsigned int i=0; i<ch.getSharedSignalList()->size(); i++)
      {
        o << "  (alias) " << ch.getSharedSignalList()->at(i)->getAlias()
          << std::endl;
      }
    }
  }
  else
    o << "  none" << std::endl;
  o << std::endl;

  o << "mux via list:" << std::endl
    << "-------------" << std::endl;
  if (ch.getMuxViaList() != NULL)
  {
    if (ch.getMuxViaList()->empty())
      o << "  empty" << std::endl;
    else
    {
      for(unsigned int i=0; i<ch.getMuxViaList()->size(); i++)
      {
        o << (*(ch.getMuxViaList()->at(i)))
          << std::endl;
      }
    }
  }
  else
    o << "  none" << std::endl;
  o << std::endl;

  o << "or via list:" << std::endl
    << "------------" << std::endl;
  if (ch.getOrViaList() != NULL)
  {
    if (ch.getOrViaList()->empty())
      o << "  empty" << std::endl;
    else
    {
      for(unsigned int i=0; i<ch.getOrViaList()->size(); i++)
      {
        o << (*(ch.getOrViaList()->at(i)))
          << std::endl;
      }
    }
  }
  else
    o << "  none" << std::endl;
  o << std::endl;

  o << "and_or via list:" << std::endl
    << "----------------" << std::endl;
  if (ch.getAndOrViaList() != NULL)
  {
    if (ch.getAndOrViaList()->empty())
      o << "  empty" << std::endl;
    else
    {
      for(unsigned int i=0; i<ch.getAndOrViaList()->size(); i++)
      {
        o << (*(ch.getAndOrViaList()->at(i)))
          << std::endl;
      }
    }
  }
  else
    o << "  none" << std::endl;
  o << std::endl;

  o << "and via list:" << std::endl
    << "-------------" << std::endl;
  if (ch.getAndViaList() != NULL)
  {
    if (ch.getAndViaList()->empty())
      o << "  empty" << std::endl;
    else
    {
      for(unsigned int i=0; i<ch.getAndViaList()->size(); i++)
      {
        o << (*(ch.getAndViaList()->at(i)))
          << std::endl;
      }
    }
  }
  else
    o << "  none" << std::endl;
  o << std::endl;

  return o;
}
//-------------------------------------------------------------------------//

void
osss_basic_channel::print_channel_graph(const std::string& file_name)
{
  if ( m_channel_graph_file_name.empty() )
    m_channel_graph_file_name = file_name;
  else
  {
    std::cout << "Warning: Channel " 
              << this->name()
              << " is already registered for graph printing!" << std::endl;
  }
}

void 
osss_basic_channel::print_channel_graph_on(std::ofstream& ostr) const
{
  osssi::print_channel_graph( ostr, *this );
}

void 
osss_basic_channel::print_channel_graph_on() const
{
  if ( !m_channel_graph_file_name.empty() )
  {
    std::ofstream dot_file;
    dot_file.open (m_channel_graph_file_name.c_str());
    osssi::print_channel_graph( dot_file, *this );
    dot_file.close(); 
  }
}

} // namespace osss

// $Id: osss_basic_channel.cpp 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
