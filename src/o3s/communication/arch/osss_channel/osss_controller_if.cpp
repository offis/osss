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

//------------------------------------------------------------------------
// "new" connection concept
//------------------------------------------------------------------------

#include "o3s/communication/arch/osss_channel/osss_controller_if.h"

#include "osss_connection_list_element.h"

namespace osss {

void 
osss_controller_if::connect(osss_basic_channel &c)
{
#if DEBUG_CONNECTION
  std::cout << "new binding: " << __PRETTY_FUNCTION__ << std::endl;
#endif

  m_channel = &c;
 
  std::vector< osss_connection_list_element * >::iterator connection_iter;
    
  for(connection_iter = m_connectionList.begin(); 
      connection_iter != m_connectionList.end(); 
      ++connection_iter)
  {
    if (! (*connection_iter)->isVector() )
    {
      if ( (*connection_iter)->sharedSignal() )
      {
	// search for equivalent signal in m_sharedSignalList
	for(unsigned int i=0; i<c.m_sharedSignalList.size(); ++i)
	{
	  if ((*connection_iter)->getPort()->isequal(c.m_sharedSignalList[i]))
	  {
	    (*connection_iter)->getPort()->bind(c.m_sharedSignalList[i]);
#if DEBUG_CONNECTION
	    std::cout << "signal " << (*connection_iter)->getPort()->name()
		 << " already in list: binding to existing signal" << std::endl;
#endif
	    break;
	  }
	  else if (i == c.m_sharedSignalList.size() - 1)
	  {
	    std::cerr << "Error: " << (*connection_iter)->getPort()->name()
		 << " is not in the shared signal list" << std::endl;
	    break;
	  }
	}
      }
      else
      {
	std::cerr << "Error: sc_in, sc_out and sc_inout ports can"
	     << "only be connected to shared signals" << std::endl;
      }
    }
    else // (*connection_iter)->isVector() == true
    {
      if ( !(*connection_iter)->sharedSignal() )
      {
#if DEBUG_CONNECTION
	 std::cout << "name " << (*connection_iter)->getVectorPort()->name() << std::endl;
	 std::cout << "alias " <<  (*connection_iter)->getVectorPort()->getAlias() << std::endl;
	 // print multi signal list
	 for(unsigned int i=0; i<c.getMultiSignalList()->size(); ++i)
	 {
	   for(unsigned int j=0; j<c.getMultiSignalList()->at(i)->size(); ++j)
	   {
	     std::cout << i << "," << j << ": " << c.getMultiSignalList()->at(i)->at(j)->getAlias() << std::endl;
	   }
	 }
#endif
	
	// search for equivalent signals in m_multiSignalList
	for(unsigned int i=0; i<c.getMultiSignalList()->size(); ++i)
	{
	  for(unsigned int j=0; j<c.getMultiSignalList()->at(i)->size(); ++j)
	  {
#if DEBUG_CONNECTION
	    std::cout << i << ", " << j << std::endl;
#endif

	    if ((*connection_iter)->getVectorPort()->
		isequal(c.getMultiSignalList()->at(i)->at(j)))
	    {
	       (*connection_iter)->getVectorPort()->bind(
		 c.getMultiSignalList()->at(i)->at(j));
#if DEBUG_CONNECTION
	       std::cout << "signal " << (*connection_iter)->getVectorPort()->name()
		    << " already in list: binding to existing signal" << std::endl;
#endif
	       break;
	    }
	    else if (j == c.getMultiSignalList()->at(i)->size() - 1)
	    {
#if DEBUG_CONNECTION
	      std::cout << "Info: " << (*connection_iter)->getVectorPort()->name()
		   << " is not in the multi signal list of transactor "
		   << i << std::endl;
#endif
	      break;
	    }
	  }
	}
      }
      else
      {
	std::cerr << "Error: osss_vector_in, osss_vector_out and osss_vector_inout"
	     << "can only be connected to multi signals" << std::endl;
      }
    }
      
    // debugging
#if DEBUG_CONNECTION
    std::cout << *(*connection_iter) << std::endl;
    std::cout << "m_ports: "   << m_connectionList.size() << std::endl;
    std::cout << "m_sharedSignals: " << c.m_sharedSignalList.size() << std::endl;
    std::cout << "----------------------------------------------------------"
	 << "-------------------------" << std::endl << std::endl; 
#endif
  }  
  m_connected = true;
}

//-------------------------------------------------------------------------//

// "new" connection concept
std::vector< osss_connection_list_element * > * 
osss_controller_if::getConnectionList()
{
  return &m_connectionList;
}

osss_basic_channel *
osss_controller_if::getChannel()
{
  return m_channel;
}

//-------------------------------------------------------------------------//

osss_controller_if::~osss_controller_if()
{
  // "new" connection concept
  std::vector< osss_connection_list_element * >::iterator conn_iter;
  for(conn_iter = m_connectionList.begin(); 
      conn_iter != m_connectionList.end(); 
      ++conn_iter)
    
    delete (*conn_iter);
  m_connectionList.clear();
}

//-------------------------------------------------------------------------//
// "new" connection concept                                                //
//-------------------------------------------------------------------------//

void
osss_controller_if::osss_connect(osss_port_list_element_base * outPort, 
			     osssi::osss_shared_signal * sharedSignal)
{
  if ((outPort->kind() == std::string("sc_out")) ||
      (outPort->kind() == std::string("sc_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(outPort,
				       new osssi::osss_no_via(),
				       sharedSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " sc_port_base must be of kind sc_out or sc_inout" << std::endl;
  }
}

void
osss_controller_if::osss_connect(osssi::osss_shared_signal * sharedSignal, 
			     osss_port_list_element_base * inPort)
{
  if (inPort->kind() == std::string("sc_in"))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(inPort,
				       new osssi::osss_no_via(),
				       sharedSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " sc_port_base must be of kind sc_in" << std::endl;
  }
}

void 
osss_controller_if::osss_connect(osss_vector_port_list_element_base * outVectorPort,
			      osssi::osss_multi_signal * multiSignal)
{
  if ((outVectorPort->kind() == std::string("osss_vector_out")) ||
      (outVectorPort->kind() == std::string("osss_vector_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(outVectorPort,
				       new osssi::osss_no_via(),
				       multiSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " osss_vector_port must be of kind osss_vector_out"
	 << "or osss_vector_inout" << std::endl;
  }
}

void 
osss_controller_if::osss_connect(osssi::osss_multi_signal * multiSignal,
			      osss_vector_port_list_element_base * inVectorPort)
{
  if (inVectorPort->kind() == std::string("osss_vector_in"))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(inVectorPort,
				       new osssi::osss_no_via(),
				       multiSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
              << " osss_vector_port must be of kind osss_vector_in"
              << std::endl;
  }
}

//-------------------------------------------------------------------------//
// hepler methods for "new" connection concept                             //
//-------------------------------------------------------------------------//

osssi::osss_shared_signal * 
osss_controller_if::osss_shared_signal(std::string name)
{
  return new osssi::osss_shared_signal(name);
}

osssi::osss_multi_signal *
osss_controller_if::osss_multi_signal(std::string name)
{
  return new osssi::osss_multi_signal(name);
}

}
// $Id: osss_controller_if.cpp 2842 2008-12-04 09:30:13Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
