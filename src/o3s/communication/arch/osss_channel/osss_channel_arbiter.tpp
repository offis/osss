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

namespace osss {

//-------------------------------------------------------------------------//

template< class SchedulerType >
osss_channel_arbiter< SchedulerType >::
osss_channel_arbiter( sc_core::sc_module_name moduleName ) 
  : sc_module(moduleName)
{
  m_number_masters = 0;
  m_granted_master = 0;
  m_first_use = true;

  mux_proc_handle = osssi::osss_create_method_process(
    "mux_proc", 
     SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, mux_proc ), 
     this );
  /*
  sensitive << mux_proc_handle;
  sensitive_pos << mux_proc_handle;
  sensitive_neg << mux_proc_handle;
  */

  request_proc_handle = osssi::osss_create_method_process(
    "request_proc", 
    SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, request_proc ),
    this );
  /*
  sensitive << request_proc_handle;
  sensitive_pos << request_proc_handle;
  sensitive_neg << request_proc_handle;
  */
}

template< class SchedulerType >
const char*
osss_channel_arbiter< SchedulerType >::kind() const
{
  return "osss_channel_arbiter";
}

//-------------------------------------------------------------------------//

//template< class SchedulerType >
//void 
//osss_channel_arbiter< SchedulerType >::
//subscribe(osss_master_if * _osss_ll_master_if)

template< class SchedulerType >
void 
osss_channel_arbiter< SchedulerType >::subscribe(sc_out<bool> * request_port_ptr,
					 sc_in<bool> * grant_port_ptr,
					 std::vector< 
						 osss_port_list_element_base * 
						 > * portList_ptr,
					 std::vector< 
						 osss_signal_list_element_base * 
						 > * signalList_ptr)
{

  // build vector of request signals
  //sc_out<bool> * request_port_ptr = _osss_ll_master_if->getRequestPort();
  if (request_port_ptr != NULL)
  {
    sc_signal<bool> * request_signal_ptr = new sc_signal<bool>;
    request_port_ptr->bind(*request_signal_ptr);
    m_request_signals.push_back(request_signal_ptr);

    // make request_proc sensitive to this request signal
    sensitive << request_proc_handle;
    sensitive << *request_signal_ptr;
  }
  else
  {
    std::cerr << "no request port specified" << std::endl;
    return;
  }

  // build vector of grant signals
  //sc_in<bool> * grant_port_ptr = _osss_ll_master_if->getGrantPort();
  if (grant_port_ptr != NULL)
  {
    sc_signal<bool> * grant_signal_ptr = new sc_signal<bool>;
    grant_port_ptr->bind(*grant_signal_ptr);
    m_grant_signals.push_back(grant_signal_ptr);
  }
  else
  {
    std::cerr << "no grant port specified" << std::endl;
    return;
  }
   
  // build vector of scheduled signals (data, addr, etc signals)
  //std::vector< PortListBase* > * portList_ptr = _osss_ll_master_if->getPortList();
  if (portList_ptr != NULL)
  {
    std::vector< osss_signal_list_element_base * > * signalList = 
      new std::vector< osss_signal_list_element_base * >;
    for(unsigned int i=0; i<portList_ptr->size(); ++i)
    {
      (*portList_ptr)[i]->generateSignal(*signalList);
      (*portList_ptr)[i]->bind(signalList->back());
    }
    m_scheduled_signals.push_back(signalList);

    // make mux_proc sensitive to signals in signalList
    sensitive << mux_proc_handle;
    for(unsigned int i=0; i<signalList->size(); ++i)
    {
      (*signalList)[i]->addToSensitivity(&sensitive);
    }
  }
  else
  {
    std::cerr << "no port(s) to schedule specified" << std::endl;
    return;
  }

  ++m_number_masters;

  // bind arbiter to signals in channel
  if (m_number_masters == 1)
  {
    std::cout << "BINDING arbiter to slaves !!!" << std::endl << std::endl;
      
    // copy port list of first master transactor in the channel
    for(unsigned int i=0; i<portList_ptr->size(); ++i)
    {
      m_arbiter_portList.push_back((*portList_ptr)[i]->clone());
    }


    ////////////////////////////////////////////////////////////////
    // CAUTION: Q&D (Quick & Dirty)
    ////////////////////////////////////////////////////////////////

    std::vector< osss_port_list_element_base * >::iterator ports_iter;
    int signal_cnt = 0;
    for(ports_iter = m_arbiter_portList.begin(); 
	ports_iter != m_arbiter_portList.end(); 
	++ports_iter)
    {
      (*ports_iter)->print();
	 
      std::cout << "m_ports: "   << m_arbiter_portList.size() << std::endl;
      std::cout << "m_signals: " << signalList_ptr->size() << std::endl;

      if (!(*ports_iter)->hasAlias()) // binding by position
      {

	/*
	// check for binding incosistencies
	if (c.getBindingType() == NONE)
	c.setBindingType(POSITION);
	else if (c.getBindingType() == ALIAS)
	std::cerr << "error: binding by position mixed with binding by alias!" << std::endl;

	*/

	if ((signalList_ptr->empty()) || 
	    (m_arbiter_portList.size() > signalList_ptr->size()))
	{
	  (*ports_iter)->generateSignal(*signalList_ptr);
	  (*ports_iter)->bind((*signalList_ptr)[signal_cnt]);
	}
	else 
	{
	  (*ports_iter)->bind((*signalList_ptr)[signal_cnt]);
	
	  std::cout << "binding by position: IN LIST" << std::endl;
	}
	
	++ signal_cnt;
      }
      else // binding by name
      {

	/*
	// check for binding inconsistencies
	if (c.getBindingType() == NONE)
	c.setBindingType(ALIAS);
	else if (c.getBindingType() == POSITION)
	std::cerr << "error: binding by alias mixed with binding by position!" << std::endl;
	*/

	if (signalList_ptr->empty() == true)
	{
	  (*ports_iter)->generateSignal(*signalList_ptr);
	  (*ports_iter)->bind((*signalList_ptr)[0]);
	}
	else
	{
	  // search for equivalent signal
	  for(unsigned int i=0; i<signalList_ptr->size(); ++i)
	  {
	    if ((*ports_iter)->isequal((*signalList_ptr)[i]))
	    {
	      (*ports_iter)->bind((*signalList_ptr)[i]);
	      std::cout << "binding by alias: IN LIST" << std::endl;
	      break;
	    }
	    else if (i == signalList_ptr->size() - 1)
	    {
	      (*ports_iter)->generateSignal(*signalList_ptr);
	      (*ports_iter)->bind((*signalList_ptr)[i+1]);
	      break;
	    }
	  }
	}
      }

      std::cout << "--------------------------------" << std::endl << std::endl;
	    
    }
  }
}

//-------------------------------------------------------------------------//

 
template< class SchedulerType >
void 
osss_channel_arbiter< SchedulerType >::mux_proc()
{
  // only one master connected 
  if (m_number_masters == 1)
  {
    for(unsigned int i=0; i<m_arbiter_portList.size(); ++i)
    {
      m_arbiter_portList[i]->write((*m_scheduled_signals[0])[i]);
      //(*m_scheduled_signals[0])[i]->print();
    }
  }
  // more than on master connected
  else if (m_number_masters > 1)
  {
    for(unsigned int i=0; i<m_arbiter_portList.size(); ++i)
    {
      m_arbiter_portList[i]->write((*m_scheduled_signals[m_granted_master])[i]);
      //(*m_scheduled_signals[0])[i]->print();
    }
  }
}

//-------------------------------------------------------------------------//

template< class SchedulerType >
void 
osss_channel_arbiter< SchedulerType >::request_proc()
{
  if (m_first_use == true)
  {
    m_first_use = false;
    m_scheduler = new SchedulerType( m_number_masters );
    std::cout << "scheduler built!" << std::endl << std::endl;
  }
  updateRequestVector();
}

//-------------------------------------------------------------------------//

template< class SchedulerType >
void 
osss_channel_arbiter< SchedulerType >::updateGrantVector()
{
  for (unsigned int i=0; i<m_grant_signals.size(); ++i)
  {
    if (i == m_granted_master)
      m_grant_signals[i]->write(true);
    else
      m_grant_signals[i]->write(false);
  }
  std::cout << "grant vector updated" << std::endl;
}

//-------------------------------------------------------------------------//

template< class SchedulerType >
void 
osss_channel_arbiter< SchedulerType >::updateRequestVector()
{
    
  std::cout << "update requestVector: num_masters: " << m_number_masters << std::endl;

  bool * tmp_requestVector = new bool[m_number_masters];
  for(unsigned int i=0; i<m_request_signals.size(); ++i)
  {
    if (m_request_signals[i]->delayed() == true)
    {
      std::cout << "true  ";
      tmp_requestVector[i] = true;
    }
    else
    {
      std::cout << "false ";
      tmp_requestVector[i] = false;
    }
  }
  std::cout << std::endl;
    
  if (!m_last_requestVector.empty())
  {
    for(unsigned int i=0; i<m_number_masters; ++i)
    {
      if (i == 0)
	std::cout << "last: ";
      std::cout << m_last_requestVector[i] << " ";
    }
    std::cout << std::endl;
  }

  for(unsigned int i=0; i<m_number_masters; ++i)
  {
    if (i == 0)
      std::cout << "tmp:  ";
    std::cout << tmp_requestVector[i] << " ";
  }
  std::cout << std::endl;

  // check if construction phase is done & the request vector has changed
  if ((m_last_requestVector.size() == m_request_signals.size()) &&
      (m_last_requestVector[m_granted_master] != tmp_requestVector[m_granted_master])) 
  {
    //check if there is a request to schedule and call the scheduler
    for(unsigned int i=0; i<m_number_masters; ++i)
    {
      if (tmp_requestVector[i] == true)
      {
	m_granted_master = m_scheduler->schedule( tmp_requestVector );
	std::cout << "scheduler called !!!" << std::endl << std::endl;
	updateGrantVector();
	break;
      }
    }
  }
  std::cout << "granted master: " << m_granted_master << std::endl;

  m_last_requestVector.clear();
  for(unsigned int i=0; i<m_number_masters; ++i)
  {
    m_last_requestVector.push_back(tmp_requestVector[i]);
  }

  delete[] tmp_requestVector;
}

} // end namespace osss

//-------------------------------------------------------------------------//
// $Id: osss_channel_arbiter.tpp 2832 2008-12-04 09:29:07Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
