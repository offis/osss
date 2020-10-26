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

#include "o3s/utils/osss_common.h"
#include "o3s/utils/osss_systemc.h"

#include "osss_multi_signal_if.h"

#include "osss_via.h"
#include "osss_via_list_element.h"
#include "osss_connection_list_element.h"

namespace osss {

//-------------------------------------------------------------------------//
// "new" connection concept                                                //
//-------------------------------------------------------------------------//

void
osss_multi_signal_if::connect_mux(osss_basic_channel &c, 
				  std::vector< 
				    osss_connection_list_element * 
				  >::iterator 
				  &connection_iter)
{
  osss_signal_list_element_base * output_signal_ptr = NULL;

  // check if we are going to place the first mux inside the channel
  if (c.getMuxViaList()->empty() == true)
  { 
    // check if mux output signal is already in the sharedSignalList
    if (c.getSharedSignalList()->empty() == true)
    {
      (*connection_iter)->getPort()->
	generateSignal(*(c.getSharedSignalList()));
      output_signal_ptr = c.getSharedSignalList()->at(0);
    }
    else
    {
      // search for equivalent signal
      for(unsigned int i=0; i<c.getSharedSignalList()->size(); ++i)
      {
	if ((*connection_iter)->getPort()->
	    isequal(c.getSharedSignalList()->at(i)))
	{
	  output_signal_ptr = c.getSharedSignalList()->at(i);
#if DEBUG_CONNECTION
	  std::cout << "signal already in list: "
	            << "binding to existing signal" << std::endl;
#endif
	  break;
	}
	else if (i == c.getSharedSignalList()->size() - 1)
	{
	  (*connection_iter)->getPort()->
	    generateSignal(*(c.getSharedSignalList()));
	  output_signal_ptr = c.getSharedSignalList()->at(i+1);
	  break;
	}
      }
    }
	      	      
    // generate MUX element
    if (output_signal_ptr != NULL)
    {
      osss_mux_via_list_element * mux = NULL;

      if ((*connection_iter)->viaGuard() == std::string("OSSS_ARBITER"))
      {
	mux = 
	  new osss_mux_via_list_element((*connection_iter)->viaName().c_str(),
					c.getArbiterSelect(),
					output_signal_ptr);
      }
      else if ((*connection_iter)->viaGuard() == std::string(
		                                 "OSSS_ADDRESS_DECODER"))
      {
	mux =
	  new osss_mux_via_list_element((*connection_iter)->viaName().c_str(),
					c.getAddressDecoderSelect(),
					output_signal_ptr);
      }
      else
      {
        std::cerr << "Error: " << (*connection_iter)->viaGuard()
                  << " is unknown!" << std::endl;
      }

      if (mux != NULL)
      {
	// add input signal for mux
	osss_signal_list_element_base * tmp_sig = 
	  (*connection_iter)->getPort()->newSignal();
	(*connection_iter)->getPort()->bind(tmp_sig);
	mux->addInputSignal(tmp_sig);

	// add mux to channel
	c.getMuxViaList()->push_back(mux);
      }
    }
    else
      std::cerr << "Error: no output signal for MUX was generated" << std::endl;
  }
  else // c.getMuxViaList()->empty() == false
  {
    // MUX list is not empty
    // search for MUX with appropriate output signal
    for(unsigned int i=0; i<c.getMuxViaList()->size(); ++i)
    {
      if ((*connection_iter)->viaName() == 
	  c.getMuxViaList()->at(i)->via_name())
      {
#if DEBUG_CONNECTION
	std::cout << "mux already in list: "
	     << "adding input signal to existing mux" << std::endl;
#endif
	osss_mux_via_list_element * mux = 
	  dynamic_cast< osss_mux_via_list_element * >(c.getMuxViaList()->at(i));
	osss_signal_list_element_base * tmp_sig = 
	  (*connection_iter)->getPort()->newSignal();
	(*connection_iter)->getPort()->bind(tmp_sig);
	mux->addInputSignal(tmp_sig);
	break;
      }
      else if (i == c.getMuxViaList()->size() - 1)
      {
	output_signal_ptr = NULL;

	// check if mux output signal is already in the sharedSignalList
	if (c.getSharedSignalList()->empty() == true)
	{
	  (*connection_iter)->getPort()->
	    generateSignal(*(c.getSharedSignalList()));
	  output_signal_ptr = c.getSharedSignalList()->at(i + 1);
	}
	else
	{
	  // search for equivalent signal
	  for(unsigned int j=0; j<c.getSharedSignalList()->size(); ++j)
	  {
	    if ((*connection_iter)->getPort()->
		isequal(c.getSharedSignalList()->at(j)))
	    {
	      output_signal_ptr = c.getSharedSignalList()->at(j);
#if DEBUG_CONNECTION
	      std::cout << "signal already in list: "
		   << "binding to existing signal" << std::endl;
#endif
	      break;
	    }
	    else if (j == c.getSharedSignalList()->size() - 1)
	    {
	      (*connection_iter)->getPort()->
		generateSignal(*(c.getSharedSignalList()));
	      output_signal_ptr = c.getSharedSignalList()->at(j+1);
	      break;
	    }
	  }
	}
	      	      
	// generate MUX element
	if (output_signal_ptr != NULL)
	{
	  osss_mux_via_list_element * mux = NULL;

	  if ((*connection_iter)->viaGuard() == std::string("OSSS_ARBITER"))
	  {
	    mux = 
	      new osss_mux_via_list_element((*connection_iter)->viaName().c_str(),
					    c.getArbiterSelect(),
					    output_signal_ptr);
	  }
	  else if ((*connection_iter)->viaGuard() == std::string(
		     "OSSS_ADDRESS_DECODER"))
	  {
	    mux =
	      new osss_mux_via_list_element((*connection_iter)->viaName().c_str(),
					    c.getAddressDecoderSelect(),
					    output_signal_ptr);
	  }
	  else
	  {
	    std::cerr << "Error: " << (*connection_iter)->viaGuard() << " is unknown!"
		 << std::endl;
	  }

	  if (mux != NULL)
	  {
	    // add input signal for mux
	    osss_signal_list_element_base * tmp_sig = 
	      (*connection_iter)->getPort()->newSignal();
	    (*connection_iter)->getPort()->bind(tmp_sig);
	    mux->addInputSignal(tmp_sig);

	    // add mux to channel
	    c.getMuxViaList()->push_back(mux);
	    break;
	  }
	}
	else
	  std::cerr << "Error: no output signal for MUX was generated" << std::endl;
      }
    }
  }
}

//-------------------------------------------------------------------------//

void
osss_multi_signal_if::connect_or(osss_basic_channel &c, 
				 std::vector<
				   osss_connection_list_element * 
				 >::iterator 
				 &connection_iter)
{
  osss_signal_list_element_base * output_signal_ptr = NULL;

  // check if we are going to place the first or_via inside the channel
  if (c.getOrViaList()->empty() == true)
  { 
    // check if or_via output signal is already in the sharedSignalList
    if (c.getSharedSignalList()->empty() == true)
    {
      (*connection_iter)->getPort()->
	generateSignal(*(c.getSharedSignalList()));
      output_signal_ptr = c.getSharedSignalList()->at(0);
    }
    else
    {
      // search for equivalent signal
      for(unsigned int i=0; i<c.getSharedSignalList()->size(); ++i)
      {
	if ((*connection_iter)->getPort()->
	    isequal(c.getSharedSignalList()->at(i)))
	{
	  output_signal_ptr = c.getSharedSignalList()->at(i);
#if DEBUG_CONNECTION
	  std::cout << "signal already in list: "
	       << "binding to existing signal" << std::endl;
#endif
	  break;
	}
	else if (i == c.getSharedSignalList()->size() - 1)
	{
	  (*connection_iter)->getPort()->
	    generateSignal(*(c.getSharedSignalList()));
	  output_signal_ptr = c.getSharedSignalList()->at(i+1);
	  break;
	}
      }
    }
	      	      
    // generate or_via element
    if (output_signal_ptr != NULL)
    {
      osss_or_via_list_element * or_via =  
	new osss_or_via_list_element((*connection_iter)->viaName().c_str(),
				      output_signal_ptr);
      if (or_via != NULL)
      {
	// add input signal for or_via
	osss_signal_list_element_base * tmp_sig = 
	  (*connection_iter)->getPort()->newSignal();
	(*connection_iter)->getPort()->bind(tmp_sig);
	or_via->addInputSignal(tmp_sig);

	// add or_via to channel
	c.getOrViaList()->push_back(or_via);
      }
    }
    else
      std::cerr << "Error: no output signal for osss_or_via was generated" << std::endl;
  }
  else // c.getOrViaList()->empty() == false
  {
    // or_via list is not empty
    // search for or_via with appropriate output signal
    for(unsigned int i=0; i<c.getOrViaList()->size(); ++i)
    {
      if ((*connection_iter)->viaName() == 
	  c.getOrViaList()->at(i)->via_name())
      {
#if DEBUG_CONNECTION
	std::cout << "osss_or_via already in list: "
	     << "adding input signal to existing mux" << std::endl;
#endif
	osss_or_via_list_element * or_via = 
	  dynamic_cast< osss_or_via_list_element * >(c.getOrViaList()->at(i));
	osss_signal_list_element_base * tmp_sig = 
	  (*connection_iter)->getPort()->newSignal();
	(*connection_iter)->getPort()->bind(tmp_sig);
	or_via->addInputSignal(tmp_sig);
	break;
      }
      else if (i == c.getOrViaList()->size() - 1)
      {
	output_signal_ptr = NULL;

	// check if or_via output signal is already in the sharedSignalList
	if (c.getSharedSignalList()->empty() == true)
	{
	  (*connection_iter)->getPort()->
	    generateSignal(*(c.getSharedSignalList()));
	  output_signal_ptr = c.getSharedSignalList()->at(i + 1);
	}
	else
	{
	  // search for equivalent signal
	  for(unsigned int j=0; j<c.getSharedSignalList()->size(); ++j)
	  {
	    if ((*connection_iter)->getPort()->
		isequal(c.getSharedSignalList()->at(j)))
	    {
	      output_signal_ptr = c.getSharedSignalList()->at(j);
#if DEBUG_CONNECTION
	      std::cout << "signal already in list: "
		   << "binding to existing signal" << std::endl;
#endif
	      break;
	    }
	    else if (j == c.getSharedSignalList()->size() - 1)
	    {
	      (*connection_iter)->getPort()->
		generateSignal(*(c.getSharedSignalList()));
	      output_signal_ptr = c.getSharedSignalList()->at(j+1);
	      break;
	    }
	  }
	}
	      	      
	// generate or_via element
	if (output_signal_ptr != NULL)
	{
	  osss_or_via_list_element * or_via =
	    new osss_or_via_list_element((*connection_iter)->viaName().c_str(),
					 output_signal_ptr);
	 
	  if (or_via != NULL)
	  {
	    // add input signal for or_via
	    osss_signal_list_element_base * tmp_sig = 
	      (*connection_iter)->getPort()->newSignal();
	    (*connection_iter)->getPort()->bind(tmp_sig);
	    or_via->addInputSignal(tmp_sig);

	    // add or_via to channel
	    c.getOrViaList()->push_back(or_via);
	    break;
	  }
	}
	else
	  std::cerr << "Error: no output signal for osss_or_via was generated" << std::endl;
      }
    }
  }
}

//-------------------------------------------------------------------------//

void
osss_multi_signal_if::connect_and_or(osss_basic_channel &c, 
				     std::vector< 
				       osss_connection_list_element * 
				     >::iterator 
				     &connection_iter)
{
  osss_signal_list_element_base * output_signal_ptr = NULL;

  // check if we are going to place the first and_or_via inside the channel
  if (c.getAndOrViaList()->empty() == true)
  { 
    // check if and_or_via output signal is already in the sharedSignalList
    if (c.getSharedSignalList()->empty() == true)
    {
      (*connection_iter)->getPort()->
	generateSignal(*(c.getSharedSignalList()));
      output_signal_ptr = c.getSharedSignalList()->at(0);
    }
    else
    {
      // search for equivalent signal
      for(unsigned int i=0; i<c.getSharedSignalList()->size(); ++i)
      {
	if ((*connection_iter)->getPort()->
	    isequal(c.getSharedSignalList()->at(i)))
	{
	  output_signal_ptr = c.getSharedSignalList()->at(i);
#if DEBUG_CONNECTION
	  std::cout << "signal already in list: "
	       << "binding to existing signal" << std::endl;
#endif
	  break;
	}
	else if (i == c.getSharedSignalList()->size() - 1)
	{
	  (*connection_iter)->getPort()->
	    generateSignal(*(c.getSharedSignalList()));
	  output_signal_ptr = c.getSharedSignalList()->at(i+1);
	  break;
	}
      }
    }
	      	      
    // generate or_via element
    if (output_signal_ptr != NULL)
    {
      osss_and_or_via_list_element * and_or_via =  
	new osss_and_or_via_list_element((*connection_iter)->viaName().c_str(),
					 output_signal_ptr);
      if (and_or_via != NULL)
      {
	// add input signal for and_or_via
	osss_signal_list_element_base * tmp_input_sig = 
	  (*connection_iter)->getPort()->newSignal();
	(*connection_iter)->getPort()->bind(tmp_input_sig);
	and_or_via->addInputSignal(tmp_input_sig);

	// add select signal for and_or_via
	osss_signal_list_element_base * tmp_select_sig = 
	  (*connection_iter)->getSelectPort()->getBoundSignal();
	and_or_via->addSelectSignal(tmp_select_sig);

	// add or_via to channel
	c.getAndOrViaList()->push_back(and_or_via);
      }
    }
    else
      std::cerr << "Error: no output signal for osss_and_or_via was generated" << std::endl;
  }
  else // c.getAndOrViaList()->empty() == false
  {
    // and_or_via list is not empty
    // search for and_or_via with appropriate output signal
    for(unsigned int i=0; i<c.getAndOrViaList()->size(); ++i)
    {
      if ((*connection_iter)->viaName() == 
	  c.getAndOrViaList()->at(i)->via_name())
      {
#if DEBUG_CONNECTION
	std::cout << "osss_and_or_via already in list: "
	     << "adding input signal to existing mux" << std::endl;
#endif
	osss_and_or_via_list_element * and_or_via = 
	  dynamic_cast< osss_and_or_via_list_element * >(c.getAndOrViaList()->at(i));

	// add input signal
	osss_signal_list_element_base * tmp_input_sig = 
	  (*connection_iter)->getPort()->newSignal();
	(*connection_iter)->getPort()->bind(tmp_input_sig);
	and_or_via->addInputSignal(tmp_input_sig);

	// add select signal
	osss_signal_list_element_base * tmp_select_sig = 
	  (*connection_iter)->getSelectPort()->getBoundSignal();
	and_or_via->addSelectSignal(tmp_select_sig);

	break;
      }
      else if (i == c.getAndOrViaList()->size() - 1)
      {
	output_signal_ptr = NULL;

	// check if and_or_via output signal is already in the sharedSignalList
	if (c.getSharedSignalList()->empty() == true)
	{
	  (*connection_iter)->getPort()->
	    generateSignal(*(c.getSharedSignalList()));
	  output_signal_ptr = c.getSharedSignalList()->at(i + 1);
	}
	else
	{
	  // search for equivalent signal
	  for(unsigned int j=0; j<c.getSharedSignalList()->size(); ++j)
	  {
	    if ((*connection_iter)->getPort()->
		isequal(c.getSharedSignalList()->at(j)))
	    {
	      output_signal_ptr = c.getSharedSignalList()->at(j);
#if DEBUG_CONNECTION
	      std::cout << "signal already in list: "
		   << "binding to existing signal" << std::endl;
#endif
	      break;
	    }
	    else if (j == c.getSharedSignalList()->size() - 1)
	    {
	      (*connection_iter)->getPort()->
		generateSignal(*(c.getSharedSignalList()));
	      output_signal_ptr = c.getSharedSignalList()->at(j+1);
	      break;
	    }
	  }
	}
	      	      
	// generate and_or_via element
	if (output_signal_ptr != NULL)
	{
	  osss_and_or_via_list_element * and_or_via =
	    new osss_and_or_via_list_element((*connection_iter)->viaName().c_str(),
					     output_signal_ptr);
	 
	  if (and_or_via != NULL)
	  {
	    // add input signal for and_or_via
	    osss_signal_list_element_base * tmp_input_sig = 
	      (*connection_iter)->getPort()->newSignal();
	    (*connection_iter)->getPort()->bind(tmp_input_sig);
	    and_or_via->addInputSignal(tmp_input_sig);

	    // add select signal for and_or_via
	    osss_signal_list_element_base * tmp_select_sig = 
	      (*connection_iter)->getSelectPort()->getBoundSignal();
	    and_or_via->addSelectSignal(tmp_select_sig);

	    // add and_or_via to channel
	    c.getAndOrViaList()->push_back(and_or_via);
	    break;
	  }
	}
	else
	  std::cerr << "Error: no output signal for osss_and_or_via was generated" << std::endl;
      }
    }
  }
}

//-------------------------------------------------------------------------//

// overwiritten from osss_signal_if
void 
osss_multi_signal_if::connect(osss_basic_channel &c)
{
  m_channel = &c;

  // consider connection in an osss_channel only
  if ((this->isMaster()) || (this->isSlave()))
  {

#if DEBUG_CONNECTION
    std::cout << "------------------------------------------------------------"
	 << "-----------------------" << std::endl << std::endl; 
    std::cout << "new binding: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    // maintain counter for the number of master and slave transactors
    // connected to the channel (can be used as a unique ID for each
    // transactor)
    if (this->isMaster())
      c.incNumMasters();
    if (this->isSlave())
      c.incNumSlaves();

    // initialize vector for the osss_multi_signals of this transactor
    c.getMultiSignalList()->
      push_back(new std::vector< osss_signal_list_element_base * >());

    // step through all osss_connection_list_elements of this transactor
    std::vector< osss_connection_list_element * >::iterator connection_iter;
    for(connection_iter = m_connectionList.begin(); 
	connection_iter != m_connectionList.end(); 
	++connection_iter)
    {
      // connections which have no via
      if (!(*connection_iter)->hasVia())
      {
	// connections to shared signals (without via)
	if ((*connection_iter)->sharedSignal() == true)
	{
	  if (c.getSharedSignalList()->empty() == true)
	  {
	    (*connection_iter)->getPort()->
	      generateSignal(*(c.getSharedSignalList()));
	    (*connection_iter)->getPort()->
	      bind(c.getSharedSignalList()->at(0));
	  }
	  else
	  {
	    // search for equivalent signal
	    for(unsigned int i=0; i<c.getSharedSignalList()->size(); ++i)
	    {
	      if ((*connection_iter)->getPort()->
		  isequal(c.getSharedSignalList()->at(i)))
	      {
		(*connection_iter)->getPort()->
		  bind(c.getSharedSignalList()->at(i));
#if DEBUG_CONNECTION
		std::cout << "signal already in list: binding to existing signal" 
		     << std::endl;
#endif
		break;
	      }
	      else if (i == c.getSharedSignalList()->size() - 1)
	      {
		(*connection_iter)->getPort()->
		  generateSignal(*(c.getSharedSignalList()));
		(*connection_iter)->getPort()->
		  bind(c.getSharedSignalList()->at(i+1));
		break;
	      }
	    }
	  }
	}
	// connections to multi signals (without via)
	else
	{
	  if (c.getMultiSignalList()->back()->empty() == true)
	  {
	    (*connection_iter)->getPort()->
	      generateSignal(*(c.getMultiSignalList()->back()));
	    (*connection_iter)->getPort()->
	      bind(c.getMultiSignalList()->back()->at(0));
	  }
	  else
	  {
	    // search for equivalent signal
	    for(unsigned int i=0; i<c.getMultiSignalList()->back()->size(); ++i)
	    {
	      if ((*connection_iter)->getPort()->
		  isequal(c.getMultiSignalList()->back()->at(i)))
	      {
		std::cerr << "osss_multi_signal with the same name "
		     << " is already registred for this transactor!" 
		     << std::endl;
		break;
	      }
	      else if (i == c.getMultiSignalList()->back()->size() - 1)
	      {
		(*connection_iter)->getPort()->
		  generateSignal(*(c.getMultiSignalList()->back()));
		(*connection_iter)->getPort()->
		  bind(c.getMultiSignalList()->back()->at(i+1));
		break;
	      }
	    }
	  }
	}
      }
      else // connections which have a via
      {
	switch ((*connection_iter)->viaType())
	{
	  // an OSSS_MUX_VIA can only be connected to an osss_shared_signal
	  case OSSS_MUX_VIA :
	    connect_mux(c, connection_iter);
	    break;

	  // an OSSS_OR_VIA can only be connected to an osss_shared_signal
	  case OSSS_OR_VIA :
	    connect_or(c, connection_iter);
	    break;

	  // TO DO:
	  // only makes sense with a multi_signal as output !!!
          case OSSS_AND_VIA :
	    break;

	  // an OSSS_AND_OR_VIA can only be connected to an osss_shared_signal
          case OSSS_AND_OR_VIA :
	    connect_and_or(c, connection_iter);
	    break;

	  // this will never happen
	  default:
	    break;
	}
      }
      
      // debugging info
#if DEBUG_CONNECTION
      std::cout << *(*connection_iter) << std::endl;
      std::cout << "# masters: " << c.getNumMasters()
	   << " # slaves: " << c.getNumSlaves()
	   << " # transactors: " << c.getNumTransactors() << std::endl;
      std::cout << "# ports: "   << m_connectionList.size() << std::endl;
      std::cout << "# shared signals: " << c.getSharedSignalList()->size() 
	   << " # multi signals: " << c.getMultiSignalList()->size() << std::endl;
      std::cout << "# mux: " << c.getMuxViaList()->size() 
	   << " # or: " << c.getOrViaList()->size()
	   << " # and: " << c.getAndViaList()->size()
	   << " # and-or: " << c.getAndOrViaList()->size() << std::endl;

      std::cout << "---------------------------------------------------------------" 
	   << "--------------------" << std::endl << std::endl; 
#endif
    }
    m_connected = true;
  }
}

//-------------------------------------------------------------------------//

// overwritten from osss_signal_if
void 
osss_multi_signal_if::osss_connect(osss_port_list_element_base * outPort, 
				   osssi::osss_shared_signal * sharedSignal)
{
  std::cerr << "Error: " << __PRETTY_FUNCTION__
       << " method can only be used in osss_signal_if" << std::endl;
}

//-------------------------------------------------------------------------//

void 
osss_multi_signal_if::osss_connect(osssi::osss_shared_signal * sharedSignal, 
				   osss_port_list_element_base * inPort)
{
  // call method in super class
  osss_signal_if::osss_connect(sharedSignal, inPort);
}

void
osss_multi_signal_if::osss_connect(osss_port_list_element_base * outPort, 
				   osssi::osss_multi_signal * multiSignal)
{
  if ((outPort->kind() == std::string("sc_out")) ||
      (outPort->kind() == std::string("sc_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(addToPortRegistry(outPort),
				       new osssi::osss_no_via(),
				       multiSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " sc_port_base must be of kind sc_out or sc_inout" << std::endl;
  }
}

//-------------------------------------------------------------------------//

void
osss_multi_signal_if::osss_connect(osss_port_list_element_base * outPort, 
				   osss_via_base * via, 
				   osssi::osss_shared_signal * sharedSignal)
{
 if ((outPort->kind() == std::string("sc_out")) ||
      (outPort->kind() == std::string("sc_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(addToPortRegistry(outPort),
				       via,
				       sharedSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " sc_port_base must be of kind sc_out or sc_inout" << std::endl;
  }
}

//-------------------------------------------------------------------------//

void
osss_multi_signal_if::osss_connect(osssi::osss_multi_signal * multiSignal, 
				   osss_port_list_element_base * inPort)
{
  if ((inPort->kind() == std::string("sc_in")) ||
      (inPort->kind() == std::string("sc_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(addToPortRegistry(inPort),
				       new osssi::osss_no_via(),
				       multiSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " sc_port_base must be of kind sc_in" << std::endl;
  }
}

//-------------------------------------------------------------------------//

/*
// This kind of signal combination should be done inside each transactor

void
osss_multi_signal_if::osss_connect(osss_signal_base * signal, 
                                   osss_via_base * via, 
			           osss_port_list_element_base * inPort)
{
  if (inPort->kind() == std::string("sc_in"))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(addToPortRegistry(inPort),
				       via,
				       signal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
	 << " sc_port_base must be of kind sc_in" << std::endl;
  }
}
*/

//-------------------------------------------------------------------------//
// hepler methods for "new" connection concept                             //
//-------------------------------------------------------------------------//

osssi::osss_multi_signal * 
osss_multi_signal_if::osss_multi_signal(std::string name)
{
  return new osssi::osss_multi_signal(name);
}

osssi::osss_mux * 
osss_multi_signal_if::osss_mux(osss_mux_controller muxController)
{
  return new osssi::osss_mux(muxController);
}

osssi::osss_or * 
osss_multi_signal_if::osss_or()
{
  return new osssi::osss_or();
}

osssi::osss_and * 
osss_multi_signal_if::osss_and(osss_port_list_element_base * outPort)
{
  return new osssi::osss_and(addToPortRegistry(outPort));
}

osssi::osss_and * 
osss_multi_signal_if::osss_and(osssi::osss_shared_signal * signal)
{
  return new osssi::osss_and(signal);
}

osssi::osss_and * 
osss_multi_signal_if::osss_and(osssi::osss_multi_signal * signal)
{
  return new osssi::osss_and(signal);
}

osssi::osss_and_or * 
osss_multi_signal_if::osss_and_or(osss_port_list_element_base * outPort)
{
  return new osssi::osss_and_or(addToPortRegistry(outPort));
}

osssi::osss_and_or * 
osss_multi_signal_if::osss_and_or(osssi::osss_shared_signal * signal)
{
  return new osssi::osss_and_or(signal);
}

osssi::osss_and_or * 
osss_multi_signal_if::osss_and_or(osssi::osss_multi_signal * signal)
{
  return new osssi::osss_and_or(signal);
}

} // end namespace osss

// $Id: osss_multi_signal_if.cpp 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
