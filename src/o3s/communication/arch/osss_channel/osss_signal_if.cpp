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
#include "o3s/communication/arch/osss_channel/osss_signal_if.h"
#include "o3s/communication/arch/osss_channel/osss_connection_list_element.h"

#include <algorithm>

using namespace sc_core;

namespace osss {

//-------------------------------------------------------------------------//

osss_signal_if::osss_signal_if()
{
  m_name = std::string("");
  m_connected = false;
}

//-------------------------------------------------------------------------//

bool
osss_signal_if::isMaster()
{
  return false;
}

bool
osss_signal_if::isSlave()
{
  return false;
}

/*
void
osss_signal_if::reset()
{
  //do nothing here
}
*/

std::vector< osss_port_list_element_base* > *
osss_signal_if::getPortRegistry()
{
  return &m_portRegistry;
}

 // "new" connection concept
std::vector< osss_connection_list_element * > *
osss_signal_if::getConnectionList()
{
  return &m_connectionList;
}

//------------------------------------------------------------------------
// "new" connection concept
//------------------------------------------------------------------------

void
osss_signal_if::connect(osss_basic_channel &c)
{

  m_channel = &c;

  // consider connections in an osss_basic_channel only
  if (!(this->isMaster()) && !(this->isSlave()))
  {
#if DEBUG_CONNECTION
    std::cout << "new binding: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    m_channel->incNumTransactors();

    std::vector< osss_connection_list_element * >::iterator connection_iter;

    for(connection_iter = m_connectionList.begin();
        connection_iter != m_connectionList.end();
        ++connection_iter)
    {
      if (c.m_sharedSignalList.empty() == true)
      {
        (*connection_iter)->getPort()->generateSignal(c.m_sharedSignalList);
        (*connection_iter)->getPort()->bind(c.m_sharedSignalList[0]);
      }
      else
      {
        // search for equivalent signal
        for(unsigned int i=0; i<c.m_sharedSignalList.size(); ++i)
        {
          if ((*connection_iter)->getPort()->isequal(c.m_sharedSignalList[i]))
          {
            (*connection_iter)->getPort()->bind(c.m_sharedSignalList[i]);
#if DEBUG_CONNECTION
            std::cout << "signal already in list: binding to existing signal" << std::endl;
#endif
            break;
          }
          else if (i == c.m_sharedSignalList.size() - 1)
          {
            (*connection_iter)->getPort()->generateSignal(c.m_sharedSignalList);
            (*connection_iter)->getPort()->bind(c.m_sharedSignalList[i+1]);
            break;
          }
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
}

//-------------------------------------------------------------------------//

void
osss_signal_if::trace_simple(std::string name)
{
  if (m_connected)
  {
    m_name = name;

    std::vector< osss_port_list_element_base* >::iterator ports_iter;
    int signal_cnt = 0;
    for(ports_iter = m_portRegistry.begin();
        ports_iter != m_portRegistry.end();
        ++ports_iter)
    {
      (*ports_iter)->setTraceScope(m_name);
      (*ports_iter)->trace(m_channel->m_sharedSignalList[signal_cnt]);
      ++signal_cnt;
    }

  }
  else
    std::cerr << "Error: not connected!" << " name: " << name << std::endl;

}

//-------------------------------------------------------------------------//

void
osss_signal_if::trace_in_file(sc_trace_file* tf, std::string name)
{
  if (m_connected)
  {
    m_name = name;

    std::vector< osss_port_list_element_base* >::iterator ports_iter;
    int signal_cnt = 0;
    for(ports_iter = m_portRegistry.begin();
        ports_iter != m_portRegistry.end();
        ++ports_iter)
    {
      (*ports_iter)->setTraceScope(m_name);
      (*ports_iter)->trace(tf);
      ++signal_cnt;
    }

  }
  else
    std::cerr << "Error: not connected!" << " name: " << name << std::endl;
}

//-------------------------------------------------------------------------//

osss_signal_if::~osss_signal_if()
{
  std::vector< osss_port_list_element_base* >::iterator ports_iter;
  for(ports_iter = m_portRegistry.begin();
      ports_iter != m_portRegistry.end();
      ++ports_iter)
  {
    delete (*ports_iter);
  }
  m_portRegistry.clear();

  // "new" connection concept
  std::vector< osss_connection_list_element * >::iterator conn_iter;
  for(conn_iter = m_connectionList.begin();
      conn_iter != m_connectionList.end();
      ++conn_iter)
  {
    delete (*conn_iter);
  }
  m_connectionList.clear();
}

//-------------------------------------------------------------------------//
// "new" connection concept                                                //
//-------------------------------------------------------------------------//

osss_port_list_element_base * osss_signal_if::addToPortRegistry(
  osss_port_list_element_base * pleb)
{
  std::vector< osss_port_list_element_base* >::iterator ports_iter;
  for(ports_iter = m_portRegistry.begin();
      ports_iter != m_portRegistry.end();
      ++ports_iter)
  {
    if ((*ports_iter)->name() == pleb->name())
    {
      delete pleb;
      return (*ports_iter);
    }
  }
  m_portRegistry.push_back(pleb);
  return pleb;
}

void
osss_signal_if::osss_connect(osss_port_list_element_base * outPort,
                             osssi::osss_shared_signal * sharedSignal)
{
  if ((outPort->kind() == std::string("sc_out")) ||
      (outPort->kind() == std::string("sc_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(addToPortRegistry(outPort),
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
osss_signal_if::osss_connect(osssi::osss_shared_signal * sharedSignal,
                             osss_port_list_element_base * inPort)
{
  if ((inPort->kind() == std::string("sc_in")) ||
      (inPort->kind() == std::string("sc_inout")))
  {
    m_connectionList.push_back(
      new osss_connection_list_element(addToPortRegistry(inPort),
                                       new osssi::osss_no_via(),
                                       sharedSignal));
  }
  else
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__
         << " sc_port_base must be of kind sc_in or sc_inout" << std::endl;
  }
}

//-------------------------------------------------------------------------//
// helper methods for "new" connection concept                             //
//-------------------------------------------------------------------------//

osssi::osss_shared_signal *
osss_signal_if::osss_shared_signal(std::string name)
{
  return new osssi::osss_shared_signal(name);
}

//-----------------------------------------------------------------------------

namespace osssi {

// TO DO: include arbiter(s) and address decoder(s)
std::ostream&
print_channel_graph (std::ostream& o, osss_basic_channel const & ch)
{
  o << "digraph bus_graph {" << std::endl
    << "graph [center = true rankdir = LR];" << std::endl;

  unsigned int numMasters = 0;
  unsigned int numSlaves  = 0;

  //do master/slave alignment
  o << "node [shape=box style=bold];" << std::endl;

  std::list< std::string > masterList;
  std::list< std::string > slaveList;
  for(unsigned int i=0; i<ch.m_transactorList.size(); i++)
  {
    osss_signal_if * sig_if = ch.m_transactorList[i];
    if ((sig_if->isMaster()) && (!sig_if->isSlave()))
    {
      numMasters++;

      std::ostringstream ostr;
      ostr << "\"Master " << numMasters << "\"";
      masterList.push_back(ostr.str());
    }

    if ((!sig_if->isMaster()) && (sig_if->isSlave()))
    {
      numSlaves++;

      std::ostringstream ostr;
      ostr << "\"Slave " << numSlaves << "\"";
      slaveList.push_back(ostr.str());
    }
  }

  std::list< std::string >::iterator str_list_iter;
  if (!masterList.empty())
  {
    o << "{ rank = source; ";
    for(str_list_iter = masterList.begin();
        str_list_iter != masterList.end();
        str_list_iter++)
    {
      o << (*str_list_iter) << "; ";
    }
    o << "}" << std::endl;
  }

  if (!slaveList.empty())
  {
    o << "{ rank = sink; ";
    for(str_list_iter = slaveList.begin();
        str_list_iter != slaveList.end();
        str_list_iter++)
    {
      o << (*str_list_iter) << "; ";
    }
    o << "}" << std::endl;
  }

  if ((numMasters != 0) && (numSlaves != 0))
  {
    //define helper nodes for shared and multi signals
    o << "node [shape = box, style = dashed] { \"shared signal\" }" << std::endl;
    o << "node [shape = box, style = dashed] { \"multi signal\" }" << std::endl;
  }

  numMasters = 0;
  numSlaves  = 0;
  std::list< std::string > muxList;
  std::list< std::string > orList;
  std::list< std::string > andOrList;

  unsigned int numTransactors = ch.m_transactorList.size();
  unsigned int numSimpleTransactors = 0;
  bool inoutDrawn = false;

  for(unsigned int i=0; i<ch.m_transactorList.size(); i++)
  {
    osss_signal_if * sig_if = ch.m_transactorList[i];

    // draw all master connection
    if ((sig_if->isMaster()) && (!sig_if->isSlave()))
    {
      numMasters++;

      std::vector< osss_connection_list_element * > * connectionList =
        sig_if->getConnectionList();
      for (unsigned int j=0; j<connectionList->size(); j++)
      {
        if ((connectionList->at(j)->connectionType() == OSSS_OUT_CONNECTION) ||
            (connectionList->at(j)->connectionType() == OSSS_INOUT_CONNECTION))
        {
          if (connectionList->at(j)->hasVia())
          {
            switch(connectionList->at(j)->viaType())
            {
              case OSSS_MUX_VIA :
              {
                std::ostringstream muxName;
                muxName <<  "\"MUX (" << connectionList->at(j)->viaName() << ")\"";

                o << "node [shape = trapezium, orientation = 270.0, style = solid]" << std::endl;
                o << "\"Master " << numMasters << "\" -> "
                  << muxName.str()
                  << " [ label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;

                bool generateOutput = false;
                if (muxList.empty())
                {
                  muxList.push_back(muxName.str());
                  generateOutput = true;
                }
                else // list not empty search for mux with the same name
                {
                  std::list< std::string >::const_iterator iter
                    = std::find(muxList.begin(), muxList.end(), muxName.str());
                  if (iter == muxList.end())
                  {
                    muxList.push_back(muxName.str());
                    generateOutput = true;
                  }
                }

                if (generateOutput == true)
                {
                  // generate MUX output signal
                  o << "\"MUX (" << connectionList->at(j)->viaName() << ")\""
                    << " -> "
                    << "\"shared signal\""
                    << ";" << std::endl;
                }
                break;
              }

              case OSSS_OR_VIA :
              {
                std::ostringstream orName;
                orName <<  "\"OR (" << connectionList->at(j)->viaName() << ")\"";

                o << "node [shape = box, style = solid]" << std::endl;
                o << "\"Master " << numMasters << "\" -> "
                  << orName.str()
                  << " [ label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;

                bool generateOutput = false;
                if (orList.empty())
                {
                  orList.push_back(orName.str());
                  generateOutput = true;
                }
                else // list not empty search for and_or with the same name
                {
                  std::list< std::string >::const_iterator iter
                    = std::find(orList.begin(), orList.end(), orName.str());
                  if (iter == orList.end())
                  {
                    orList.push_back(orName.str());
                    generateOutput = true;
                  }
                }

                if (generateOutput == true)
                {
                  // generate or output signal
                  o << "\"OR (" << connectionList->at(j)->viaName() << ")\""
                    << " -> "
                    << "\"shared signal\""
                    << ";" << std::endl;
                }
                break;
              }

              // not supported up to now
              case OSSS_AND_VIA :
                break;

              case OSSS_AND_OR_VIA :
              {
                std::ostringstream andOrName;
                andOrName <<  "\"AND OR (" << connectionList->at(j)->viaName() << ")\"";

                o << "node [shape = box, style = solid]" << std::endl;
                o << "\"Master " << numMasters << "\" -> "
                  << andOrName.str()
                  << " [ label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;

                if (connectionList->at(j)->getSelectPort() != NULL)
                {
                  o << "node [shape = box, style = solid]" << std::endl;
                  o << "\"Master " << numMasters << "\" -> "
                    << andOrName.str()
                    << " [ label = \""
                    << connectionList->at(j)->getSelectPort()->name() << " (sel)"
                    << "\" style=dashed ]"
                    << ";" << std::endl;
                }

                bool generateOutput = false;
                if (andOrList.empty())
                {
                  andOrList.push_back(andOrName.str());
                  generateOutput = true;
                }
                else // list not empty search for or with the same name
                {
                  std::list< std::string >::const_iterator iter
                    = std::find(andOrList.begin(), andOrList.end(), andOrName.str());
                  if (iter == andOrList.end())
                  {
                    andOrList.push_back(andOrName.str());
                    generateOutput = true;
                  }
                }

                if (generateOutput == true)
                {
                  // generate and_or output signal
                  o << "\"AND OR (" << connectionList->at(j)->viaName() << ")\""
                    << " -> "
                    << "\"shared signal\""
                    << ";" << std::endl;
                }
                break;
              }

              default :
                break;
            }
          }
          else // no via
          {
            o << "\"Master " << numMasters << "\" -> ";

            if (connectionList->at(j)->sharedSignal())
            {
              o << "\"shared signal\"";
            }
            else // multi_signal
            {
              o << "\"multi signal\"";
            }

            o << " [ label = \""
              << connectionList->at(j)->signalName()
              << "\" ]"
              << ";" << std::endl;
          }
        }
        else // OSSS_IN_CONNECTION
        {
          if (connectionList->at(j)->sharedSignal())
          {
            o << "\"shared signal\"";
          }
          else // multi_signal
          {
            o << "\"multi signal\"";
          }

          o << " -> "
            << "\"Master " << numMasters << "\""
            << " [ label = \""
            << connectionList->at(j)->signalName()
            << "\" ]"
            << ";" << std::endl;
        }
      }
    }

    // draw all slave connections
    if ((sig_if->isSlave()) && (!sig_if->isMaster()))
    {
      numSlaves++;

      std::vector< osss_connection_list_element * > * connectionList =
        sig_if->getConnectionList();
      for (unsigned int j=0; j<connectionList->size(); j++)
      {
        if ((connectionList->at(j)->connectionType() == OSSS_OUT_CONNECTION) ||
            (connectionList->at(j)->connectionType() == OSSS_INOUT_CONNECTION))
        {
          if (connectionList->at(j)->hasVia())
          {
            switch (connectionList->at(j)->viaType())
            {
              case OSSS_MUX_VIA :
              {
                std::ostringstream muxName;
                muxName <<  "\"MUX (" << connectionList->at(j)->viaName() << ")\"";

                o << "node [shape = trapezium, orientation = 90.0, style = solid]" << std::endl;
                o << "\"Slave " << numSlaves << "\" -> "
                  << muxName.str()
                  << " [ label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;

                bool generateOutput = false;
                if (muxList.empty())
                {
                  muxList.push_back(muxName.str());
                  generateOutput = true;
                }
                else // list not empty search for mux with the same name
                {
                  std::list< std::string >::const_iterator iter
                    = std::find(muxList.begin(), muxList.end(), muxName.str());
                  if (iter == muxList.end())
                  {
                    muxList.push_back(muxName.str());
                    generateOutput = true;
                  }
                }

                if (generateOutput == true)
                {
                  //MUX output signal
                  o << "\"MUX (" << connectionList->at(j)->viaName() << ")\""
                    << " -> "
                    << "\"shared signal\""
                    << ";" << std::endl;
                }
                break;
              }

              case OSSS_OR_VIA :
              {
                std::ostringstream orName;
                orName <<  "\"OR (" << connectionList->at(j)->viaName() << ")\"";

                o << "node [shape = box, style = solid]" << std::endl;
                o << "\"Slave " << numSlaves << "\" -> "
                  << orName.str()
                  << " [ label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;

                bool generateOutput = false;
                if (orList.empty())
                {
                  orList.push_back(orName.str());
                  generateOutput = true;
                }
                else // list not empty search for or with the same name
                {
                  std::list< std::string >::const_iterator iter
                    = std::find(orList.begin(), orList.end(), orName.str());
                  if (iter == orList.end())
                  {
                    orList.push_back(orName.str());
                    generateOutput = true;
                  }
                }

                if (generateOutput == true)
                {
                  //OR output signal
                  o << "\"OR (" << connectionList->at(j)->viaName() << ")\""
                    << " -> "
                    << "\"shared signal\""
                    << ";" << std::endl;
                }
                break;
              }

              // not supported up to now
              case OSSS_AND_VIA :
                break;

              case OSSS_AND_OR_VIA :
              {
                std::ostringstream andOrName;
                andOrName <<  "\"AND OR (" << connectionList->at(j)->viaName() << ")\"";

                o << "node [shape = box, style = solid]" << std::endl;
                o << "\"Slave " << numSlaves << "\" -> "
                  << andOrName.str()
                  << " [ label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;

                if (connectionList->at(j)->getSelectPort() != NULL)
                {
                  o << "node [shape = box, style = solid]" << std::endl;
                  o << "\"Slave " << numSlaves << "\" -> "
                    << andOrName.str()
                    << " [ label = \""
                    << connectionList->at(j)->getSelectPort()->name() << " (sel)"
                    << "\" style=dashed ]"
                    << ";" << std::endl;
                }

                bool generateOutput = false;
                if (andOrList.empty())
                {
                  andOrList.push_back(andOrName.str());
                  generateOutput = true;
                }
                else // list not empty search for and_or with the same name
                {
                  std::list< std::string >::const_iterator iter
                    = std::find(andOrList.begin(), andOrList.end(), andOrName.str());
                  if (iter == andOrList.end())
                  {
                    andOrList.push_back(andOrName.str());
                    generateOutput = true;
                  }
                }

                if (generateOutput == true)
                {
                  //AND OR output signal
                  o << "\"AND OR (" << connectionList->at(j)->viaName() << ")\""
                    << " -> "
                    << "\"shared signal\""
                    << ";" << std::endl;
                }
                break;
              }

              default :
                break;
            }
          }
          else // no via
          {
            o << "\"Slave " << numSlaves << "\" -> ";

            if (connectionList->at(j)->sharedSignal())
            {
              o << "\"shared signal\"";
            }
            else // multi_signal
            {
              o << "\"multi signal\"";
            }

            o << " [ label = \""
              << connectionList->at(j)->signalName()
              << "\" ]"
              << ";" << std::endl;
          }
        }
        else // OSSS_IN_CONNECTION
        {
          if (connectionList->at(j)->sharedSignal())
          {
            o << "\"shared signal\"";
          }
          else // multi_signal
          {
            o << "\"multi signal\"";
          }

          o << " -> "
            << "\"Slave " << numSlaves << "\""
            << " [ label = \""
            << connectionList->at(j)->signalName()
            << "\" ]"
            << ";" << std::endl;
        }
      }
    }

    //------------------------------------------------------------------------//
    // drawn connections in osss_basic_channel start                          //
    //------------------------------------------------------------------------//

    // draw connections in osss_basic_channel
    if ((!sig_if->isSlave()) && (!sig_if->isMaster()))
    {
      numSimpleTransactors++;

      std::vector< osss_connection_list_element * > * connectionList =
        sig_if->getConnectionList();

      //cout << "connectionList->size(): " << connectionList->size() << std::endl;
      //cout << "numSimpleTransactors: " << numSimpleTransactors << std::endl;
      //cout << "numTransactors: " << numTransactors << std::endl;

      for (unsigned int j=0; j<connectionList->size(); j++)
      {
        // an out connection only appears once per transactor
        if (connectionList->at(j)->connectionType() == OSSS_OUT_CONNECTION)
        {
          for (unsigned int transCounter = 1; transCounter<=numTransactors; transCounter++)
          {
            if (transCounter != numSimpleTransactors)
            {
              o << "\"Transactor " << numSimpleTransactors << "\" -> ";

              o << "\"Transactor " << transCounter << "\"";

              o << " [ label = \""
                << connectionList->at(j)->signalName()
                << "\" ]"
                << ";" << std::endl;
            }
          }
        }
        // inout connection can ocour multiple times and have to be drawn only once
        if (!inoutDrawn)
        {
          if (connectionList->at(j)->connectionType() == OSSS_INOUT_CONNECTION)
          {
            inoutDrawn = true;
            for (unsigned int transCounter = 1; transCounter<=numTransactors; transCounter++)
            {
              if (transCounter != numSimpleTransactors)
              {
                o << "\"Transactor " << numSimpleTransactors << "\" -> ";

                o << "\"Transactor " << transCounter << "\"";

                o << " [ dir=both label = \""
                  << connectionList->at(j)->signalName()
                  << "\" ]"
                  << ";" << std::endl;
              }
            }
          }
        }
      }
    }

    //------------------------------------------------------------------------//
    // drawn connections in osss_basic_channel end                            //
    //------------------------------------------------------------------------//

  }

  o << "}" << std::endl << std::endl;

  return o;
}

} // namespace osssi

void osss_print_channel_graph(osss_basic_channel &ch, const std::string& file_name)
{
  // when simulation is running (i.e. we have already performed the
  // elaboration the channel's internal structure is built and
  // we can directly start printing the channel graph
  if (sc_is_running())
  {
    std::ofstream dot_file;
    dot_file.open (file_name.c_str());
    osssi::print_channel_graph( dot_file, ch );
    dot_file.close();
  }
  // the channel structure has not been built. Therefore, we register the
  // request for channel printing in the basic channel. The channel printing
  // in a file named "file_name" is performed during end of elaboration
  else
    ch.print_channel_graph(file_name);
}

} // namespace osss

// $Id: osss_signal_if.cpp 2839 2008-12-04 09:29:59Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
