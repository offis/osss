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

#ifndef OSSS_CONNECTION_LIST_ELEMENT_HH
#define OSSS_CONNECTION_LIST_ELEMENT_HH

#include "o3s/communication/arch/osss_channel/osss_via.h"
#include "o3s/communication/arch/osss_channel/osss_vector_port_list_element.h"

namespace osss {

enum osss_connection_list_element_type {OSSS_NO_CONNECTION,
                                        OSSS_OUT_CONNECTION,
                                        OSSS_INOUT_CONNECTION,
                                        OSSS_IN_CONNECTION};

class osss_connection_list_element
{
public: 
  friend std::ostream& operator<< (std::ostream& o, 
				   osss_connection_list_element& cle);

  osss_connection_list_element( osss_port_list_element_base * port,
				osss_via_base * via,
				osss_signal_base * signal);

  osss_connection_list_element( osss_vector_port_list_element_base * vectorPort,
				osss_via_base * via,
				osss_signal_base * signal);

  //---------------------------------------------------------------------------
  // get "general" information about the connection list element
  //---------------------------------------------------------------------------
  
  osss_connection_list_element_type connectionType();

  bool isVector();
  unsigned int vectorSize();
  bool hasVia();
  std::string signalName();
  bool sharedSignal();
  osss_port_list_element_base * getPort();
  osss_vector_port_list_element_base * getVectorPort();

  //---------------------------------------------------------------------------
  // get information about the via
  //---------------------------------------------------------------------------

  osss_via_type viaType();
  std::string viaGuard();
  std::string viaName();

  // does not work yet ?
  osss_signal_base * getSelectSignal();
  osss_port_list_element_base * getSelectPort();
  virtual ~osss_connection_list_element();

private:
  osss_connection_list_element();

  osss_port_list_element_base * m_port;
  osss_vector_port_list_element_base * m_vectorPort;
  osss_via_base * m_via;
  osss_signal_base * m_signal;

  bool m_isVector;
  unsigned int m_vectorSize;

};

std::ostream& operator<< (std::ostream& o, 
			  osss_connection_list_element& cle);

} // namespace osss

#endif
// $Id: osss_connection_list_element.h 2924 2008-12-16 12:54:50Z philipph $
// :flavour: (osss,s) (osss_recon,s) (osss_icodes,s) (osss_full,s)
