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

#ifndef OSSS_CONTROLLER_IF_HH
#define OSSS_CONTROLLER_IF_HH

#include <string>
#include <vector>

#include "o3s/utils/osss_types.h"
#include "o3s/communication/arch/osss_channel/osss_address_map.h"
#include "o3s/communication/arch/osss_channel/osss_slave_if.h"
#include "o3s/structural/arch/osss_clock_reset_if.h"

namespace osss {

class osss_port_list_element_base;
class osss_vector_port_list_element_base;
class osss_slave_if;
template< class ADDRESS > class osss_address_range;
template< class ADDRESS > class osss_address_map;

class osss_controller_if
  : public sc_core::sc_module
  , public osssi::osss_clock_reset_if
{
protected:

  osss_controller_if()
    : sc_core::sc_module()
    , osssi::osss_clock_reset_if()
    , m_connected(false)
  {}

  explicit
  osss_controller_if( sc_core::sc_module_name module_name )
    : sc_core::sc_module(module_name)
    , osssi::osss_clock_reset_if()
    , m_connected(false)
  {}

public:
  // "new" connection concept
  void connect(osss_basic_channel &c);

  // "new" connection concept
  std::vector< osss_connection_list_element * > * getConnectionList();

  osss_basic_channel * getChannel();

  virtual ~osss_controller_if();

protected: 
  // "new" connection concept:
  //--------------------------

  // connects an sc_out<T> port directly to an osss_shared_signal
  void osss_connect(osss_port_list_element_base * outPort, 
		    osssi::osss_shared_signal * sharedSignal);


  // connects an osss_shared_signal directly to an sc_in<T> port
  void osss_connect(osssi::osss_shared_signal * sharedSignal, 
		    osss_port_list_element_base * inPort);

  osssi::osss_shared_signal * osss_shared_signal(std::string name);

  void osss_connect(osss_vector_port_list_element_base * outVectorPort,
		    osssi::osss_multi_signal * multiSignal);

  void osss_connect(osssi::osss_multi_signal * multiSignal,
		    osss_vector_port_list_element_base * inVectorPort);

  osssi::osss_multi_signal * osss_multi_signal(std::string name);
 
  // "new" connection concept:
  std::vector< osss_connection_list_element * > m_connectionList;

private:
  bool m_connected;
  osss_basic_channel * m_channel;

};

//=============================================================================

class osss_arbiter_if
  : public osss_controller_if
{
protected:

  osss_arbiter_if()
    : osss_controller_if()
  {}

  explicit
  osss_arbiter_if( sc_core::sc_module_name module_name )
    : osss_controller_if(module_name)
  {}

public:

  virtual const char* kind() const { return "osss_arbiter_if"; }

  virtual bool isDummy()
  {
    return false;
  }
   
  unsigned int getSelect()
  {
    return getChannel()->getArbiterSelect()->read();
  }

  void setSelect(unsigned int i)
  {
    getChannel()->getArbiterSelect()->write(i);
  }

  osss_priority getPriority( osss_index_type index)
  {
    typedef std::vector< osss_signal_if*>::size_type size_type;

    std::vector< osss_signal_if * > transactorList
      = getChannel()->m_transactorList;

    //size_type masterCounter = 0;
    for(size_type i=0; i<transactorList.size(); i++)
    { /*
      if (transactorList[i]->isMaster())
      {
        osss_master_if * mif = 
          dynamic_cast< osss_master_if * >(transactorList[i]);

        if (masterCounter == index)
          return mif->getPriority();

        masterCounter++;
      }
      */
    }
    return 0;
  }

};

class osss_no_arbiter
  : public osss_arbiter_if
{
public:

  explicit
  osss_no_arbiter( sc_core::sc_module_name module_name
                     = sc_core::sc_gen_unique_name( "osss_no_arbiter" ) )
    : osss_arbiter_if(module_name)
  {}

  virtual const char* kind() const
    { return "osss_no_arbiter"; }

  bool isDummy()
  {
    return true;
  }

  // dummy to prevent the compiler from complaining
  void buildConnectionList()
  { }

  // dummy to prevent the compiler from complaining
  void __osss_buildProcess()
  { }

};

class osss_address_decoder_if
  : public osss_controller_if
{
protected:

  osss_address_decoder_if()
    : osss_controller_if()
    , m_address_map( NULL )
  {}

  explicit
  osss_address_decoder_if( sc_core::sc_module_name module_name )
    : osss_controller_if(module_name)
    , m_address_map( NULL )
  {}

  ~osss_address_decoder_if()
  {
    delete m_address_map;
  }

public:
  virtual const char* kind() const { return "osss_address_decoder_if"; }
 
  unsigned int getSelect()
  {
    return getChannel()->getAddressDecoderSelect()->read();
  }

  void setSelect(unsigned int i)
  {
    getChannel()->getAddressDecoderSelect()->write(i);
  }

  virtual bool isDummy()
  {
    return false;
  }

  template< class ADDRESS >
  osss_address_map< ADDRESS > * getAddressMap()
  {
    if (m_address_map == NULL)
    {
      osss_address_map< ADDRESS > * tmp_address_map = 
	new osss_address_map<ADDRESS>();
      
      std::vector< osss_signal_if * > transactorList =  
	getChannel()->m_transactorList;

      unsigned int slaveCounter = 0;
      for(unsigned int i=0; i<transactorList.size(); i++)
      {
	if (transactorList[i]->isSlave())
	{
	  osss_slave_if * sif = 
	    dynamic_cast< osss_slave_if * >(transactorList[i]);
	  
	  osss_address_range<ADDRESS> * ar = 
	    sif->template getAddressRange<ADDRESS>();

	  if (ar != NULL)
	  {
	    tmp_address_map->add_to_map(*ar, slaveCounter);
	  }
	  slaveCounter++;
	}
      }
      m_address_map = tmp_address_map;
      return tmp_address_map;
    }
    
    return dynamic_cast< osss_address_map< ADDRESS > * >(m_address_map);
  }

private:

  osss_address_map_base * m_address_map;

};

class osss_no_address_decoder
  : public osss_address_decoder_if
{
public:

  explicit
  osss_no_address_decoder( sc_core::sc_module_name module_name
                             = sc_core::sc_gen_unique_name
                                 ("osss_no_address_decoder") )
    : osss_address_decoder_if(module_name)
  {}

  virtual const char* kind() const { return "osss_no_address_decoder"; }

  bool isDummy()
  {
    return true;
  }

  // dummy to prevent the compiler from complaining
  void buildConnectionList()
  { }

  // dummy to prevent the compiler from complaining
  void __osss_buildProcess()
  { }

};

} // namespace osss

#endif
// $Id: osss_controller_if.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
