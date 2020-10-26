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
#include "o3s/communication/arch/osss_channel/osss_connection_list_element.h"

namespace osss {

osss_connection_list_element::osss_connection_list_element(
                 osss_port_list_element_base * port,
                 osss_via_base * via,
                 osss_signal_base * signal)
{
  m_port = port;
  m_isVector = false;
  m_vectorSize = 1;

  m_vectorPort = NULL;

  m_via = via;
  m_signal = signal;
  
  /*
  if (!hasVia())
    m_port->setAlias(m_signal->name());
  else
  {
    std::cout << "TO DO !!!!!!!!!!!!!" << std::endl;
  }
  */
  m_port->setAlias(m_signal->name());
}

osss_connection_list_element::osss_connection_list_element(
                                  osss_vector_port_list_element_base * vectorPort,
                                  osss_via_base * via,
                                  osss_signal_base * signal)
{
  m_port = NULL;
  m_isVector = true;
  m_vectorSize = vectorPort->size();

  m_vectorPort = vectorPort;

  m_via = via;
  m_signal = signal;
  
  /*
  if (!hasVia())
    m_vectorPort->setAlias(m_signal->name());
  else
  {
    std::cout << "TO DO !!!!!!!!!!!!!" << std::endl;
  }
  */
  m_vectorPort->setAlias(m_signal->name());
}

//---------------------------------------------------------------------------
// get "general" information about the connection list element
//---------------------------------------------------------------------------

osss_connection_list_element_type osss_connection_list_element::connectionType()
{
  if (!isVector())
  {
    if (m_port->kind() == std::string("sc_out"))
      return OSSS_OUT_CONNECTION;

    if (m_port->kind() == std::string("sc_inout"))
      return OSSS_INOUT_CONNECTION;

    if (m_port->kind() == std::string("sc_in"))
      return OSSS_IN_CONNECTION;
  }
  else // isVector() == true
  {
    if (m_vectorPort->kind() == std::string("osss_vector_out"))
      return OSSS_OUT_CONNECTION;

    if (m_vectorPort->kind() == std::string("osss_vector_inout"))
      return OSSS_INOUT_CONNECTION;

    if (m_vectorPort->kind() == std::string("osss_vector_in"))
      return OSSS_IN_CONNECTION;
  }

  return OSSS_NO_CONNECTION; // this should never happen
}

bool osss_connection_list_element::isVector()
{
  return m_isVector;
}

unsigned int osss_connection_list_element::vectorSize()
{
  return m_vectorSize;
}
 
bool osss_connection_list_element::hasVia()
{
  return (m_via->type() != OSSS_NO_VIA);
}

std::string osss_connection_list_element::signalName()
{
  return m_signal->name();
}

bool osss_connection_list_element::sharedSignal()
{
  return m_signal->isShared();
}

osss_port_list_element_base * osss_connection_list_element::getPort()
{
  return m_port;
}

osss_vector_port_list_element_base * osss_connection_list_element::getVectorPort()
{
  return m_vectorPort;
}

//---------------------------------------------------------------------------
// get information about the via
//---------------------------------------------------------------------------

osss_via_type osss_connection_list_element::viaType()
{
  return m_via->type();
}

std::string osss_connection_list_element::viaGuard()
{
  return m_via->guard();
}

std::string osss_connection_list_element::viaName()
{
  switch (connectionType())
  {
    case OSSS_NO_CONNECTION :
      return std::string("");

    case OSSS_OUT_CONNECTION   :
      return (m_signal->name() + std::string("_out_via"));

    case OSSS_INOUT_CONNECTION :
      return (m_signal->name() + std::string("_inout_via"));
	
    case OSSS_IN_CONNECTION :
      if (! isVector() )
        return (m_port->name() + std::string("_in_via"));
      else
        return (m_vectorPort->name() + std::string("_in_via"));
  }

  return std::string("");
}

// does not work yet ?
osss_signal_base * osss_connection_list_element::getSelectSignal()
{
  if (viaType() == OSSS_AND_OR_VIA)
  {
    osss_via_logic * tmp_via_logic = 
      dynamic_cast< osss_via_logic * >(m_via);
    return tmp_via_logic->osss_signal();
  }
  return NULL;
}

osss_port_list_element_base * osss_connection_list_element::getSelectPort()
{
  if (viaType() == OSSS_AND_OR_VIA)
  {
    osss_via_logic * tmp_via_logic = 
      dynamic_cast< osss_via_logic * >(m_via);
    return tmp_via_logic->port();
  }
  return NULL;
}

osss_connection_list_element::~osss_connection_list_element()
{
  if (m_port != NULL)
    delete m_port;
  m_port = NULL;

  if (m_vectorPort != NULL)
    delete m_vectorPort;
  m_vectorPort = NULL;

  if (m_via != NULL)
    delete m_via;
  m_via = NULL;

  if (m_signal != NULL)
    delete m_signal;
  m_signal = NULL;
}

osss_connection_list_element::osss_connection_list_element()
{
}

std::ostream& operator<< (std::ostream& o, 
			  osss_connection_list_element& cle)
{
  o << "port information:" << std::endl;
  o << "-----------------" << std::endl;
  if (!cle.m_isVector)
  {
    o << *(cle.m_port) << std::endl;
  }
  else // cle.m_isVector == true
  {
    o << *(cle.m_vectorPort) << std::endl;
  }

  o << "via information:" << std::endl;
  o << "----------------" << std::endl;
  o << *(cle.m_via) << std::endl;
  
  o << "signal information:" << std::endl;
  o << "-------------------" << std::endl;
  o << *(cle.m_signal) << std::endl;

  return o;
}

} // end namespace osss

// $Id: osss_connection_list_element.cpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
