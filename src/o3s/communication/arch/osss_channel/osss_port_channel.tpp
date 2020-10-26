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

#include "o3s/communication/arch/osss_channel/osss_master_if.h"
#include "o3s/communication/arch/osss_channel/osss_slave_if.h"

namespace osss {

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
bind(if_type &interface_)
{
  base_type::bind(interface_);
  
  osss_basic_channel* channel = 
    dynamic_cast<osss_basic_channel*>(base_type::operator->());

  if ((channel == NULL) or (m_Transactor != NULL))
  {
    std::cerr << "Error: " << kind() << " "
              << base_type::name() 
              << " internal error" << std::endl;
  } 

  m_Transactor = osss_basic_channel::create<InterfaceType>();
  
  typedef typename getTransactorType<InterfaceType>::return_t transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (transactor == NULL)
  {
   std::cerr << "Error: " << kind() << " " << base_type::name() 
             << " internal error" << std::endl;
  }

  channel->m_transactorList.push_back(transactor);
  
  transactor->buildConnectionList();
  transactor->connect(*channel);
}

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
bind(this_type &port)
{
  base_type::bind(port);

  m_firstAccess = port.m_firstAccess;
  m_proc_id = port.m_proc_id;
  m_Transactor = port.m_Transactor;
}

//-------------------------------------------------------------------------//

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
operator()(if_type &interface_)
{
  this->bind(interface_);
}

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
operator()(this_type &port)
{
  this->bind(port);
}

//-------------------------------------------------------------------------//

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
trace(std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " not bound" << std::endl;
  }

  typedef typename getTransactorType<InterfaceType>::return_t transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);
  
  if (transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  } 
  
  transactor->trace(name);
}

//-------------------------------------------------------------------------//

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
trace(sc_core::sc_trace_file* tf, std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " not bound" << std::endl;
  }

  typedef typename getTransactorType<InterfaceType>::return_t transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  } 
  
  transactor->trace(tf, name);
}

//-------------------------------------------------------------------------//

template< class InterfaceType >
inline
InterfaceType*
osss_port< osss_channel_if<InterfaceType> >::
operator->()
{
  if (m_firstAccess)
  {
    m_proc_id = osssi::osss_process_handle::current().get_proc_id(); 
    m_firstAccess = false;
  }
  else
  {
    if ( m_proc_id != osssi::osss_process_handle::current().get_proc_id())
    {
      std::cerr << "Error: This " << kind() << " "
	   << base_type::name()
	   << " has been accessed from within another process before."
	   << std::endl;
      // stop simulation due to error
      sc_core::sc_stop();
    }
  }

  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  }
  
  return m_Transactor; 
}

//-------------------------------------------------------------------------//

template < class InterfaceType >
template < class ADDRESS > 
inline
void 
osss_port< osss_channel_if<InterfaceType> >::
setAddressRange( const ADDRESS &low, const ADDRESS &high )
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
    return;
  }

  typedef typename getTransactorType<InterfaceType>::return_t transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (transactor->isMaster() && !transactor->isSlave())
  {
    std::cout << "Warning: master transactors (which use osss_master_if) "
              << "have no address range!" << std::endl;
  }

  if (transactor->isSlave())
  {
    (dynamic_cast< osss_slave_if *>(transactor))
      ->setAddressRange<ADDRESS>(low, high);
  }
}

//-------------------------------------------------------------------------//

template< class InterfaceType >
inline
osssi::osss_object_registry* 
osss_port< osss_channel_if<InterfaceType> >::
getObjectRegistry()
{
  return base_type::operator->()->getObjectRegistry();
}

//-------------------------------------------------------------------------//

template< class InterfaceType >
inline
void
osss_port< osss_channel_if<InterfaceType> >::
setPriority(osss_priority priority)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " internal error" << std::endl;
  }
  
  typedef typename getTransactorType<InterfaceType>::return_t transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (!transactor->isMaster() && transactor->isSlave())
  {
    std::cout << "Warning: slave transactors (which use osss_slave_if) "
	 << "do not have priorities!" << std::endl;
  }

  if (transactor->isMaster())
  {
    (dynamic_cast< osss_master_if *>(transactor))
      ->setPriority(priority);
  }
}

//------------------------------------------------------------------------------
// partial specialisation for
//  osss_rmi_channel_client_if
//------------------------------------------------------------------------------

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
bind(ChannelType &channel)
{
  base_type::bind(channel);

  if(m_rmi_channel_ptr == NULL)
    m_rmi_channel_ptr = &channel;
  else
  {
    std::cerr << "Error in " << this->kind() 
	      << ": This port has already been bound to a channel!" 
	      << std::endl;
    OSSS_ABORT();
  }

  osss_basic_channel* basic_channel = 
    dynamic_cast<osss_basic_channel*>(base_type::operator->());

  if ((basic_channel == NULL) or (m_Transactor != NULL))
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	      << " internal error" << std::endl;
  } 

  m_Transactor = new client_transactor_accessor();
  channel.register_transactor_accessor(m_Transactor);
}

inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
bind_to_port(this_type &port)
{
  base_type::bind(port);

  m_Transactor = port.m_Transactor;
  m_rmi_channel_ptr = port.m_rmi_channel_ptr;
}

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
operator()(ChannelType &channel)
{
  this->bind(channel);
}

inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
bind_to_rmi(this_type &port, bool register_for_rmi)
{
  port.bind_to_port(*this);
  if (register_for_rmi)
    port.register_at_rmi_channel(m_rmi_channel_ptr);
}

/*
inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
operator()(this_type &port)
{
  this->bind(port);
}
*/

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
trace(std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " not bound" << std::endl;
  }

  m_Transactor->trace(name);
}

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
trace(sc_core::sc_trace_file* tf, std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " not bound" << std::endl;
  }

  m_Transactor->trace(tf, name);
}

//-------------------------------------------------------------------------//

inline
osss_rmi_channel_client_if*
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
operator->()
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  }
  
  if(m_Transactor->get_transactor() == NULL)
    std::cout << "Error in: " << __PRETTY_FUNCTION__ << std::endl;

  return m_Transactor->get_transactor(); 
}

//-------------------------------------------------------------------------//

inline
osssi::osss_object_registry* 
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
getObjectRegistry()
{
  return base_type::operator->()->getObjectRegistry();
}

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_client_if> >::
setPriority(osss_priority priority)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  }
  
  m_Transactor->set_priority(priority);
}

//------------------------------------------------------------------------------
// partial specialisation for
//  osss_rmi_channel_server_if
//------------------------------------------------------------------------------

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
bind(ChannelType &channel)
{
  base_type::bind(channel);
  
  osss_basic_channel* basic_channel = 
    dynamic_cast<osss_basic_channel*>(base_type::operator->());

  if ((basic_channel == NULL) or (m_Transactor != NULL))
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	      << " internal error" << std::endl;
  } 

  m_Transactor = new server_transactor_accessor();
  channel.register_transactor_accessor(m_Transactor);
}

inline
void
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
bind_to_port(this_type &port)
{
  base_type::bind(port);
  
  m_Transactor = port.m_Transactor;
}

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
operator()(ChannelType &channel)
{
  this->bind(channel);
}

/*
inline
void
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
operator()(this_type &port)
{
  this->bind(port);
}
*/

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
trace(std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " not bound" << std::endl;
  }

  m_Transactor->trace(name);
}

//-------------------------------------------------------------------------//

template<class ChannelType>
inline
void
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
trace(sc_core::sc_trace_file* tf, std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " not bound" << std::endl;
  }

  m_Transactor->trace(tf, name);
}

//-------------------------------------------------------------------------//

inline
osss_rmi_channel_server_if*
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
operator->()
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " internal error" << std::endl;
  }

  if(m_Transactor->get_transactor() == NULL)
    std::cout << "Error in: " << __PRETTY_FUNCTION__ << std::endl; 
  
  return m_Transactor->get_transactor(); 
}

//-------------------------------------------------------------------------//

template<class ChannelType> 
inline
void 
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
setAddressRange(const typename ChannelType::address_type &low, 
		const typename ChannelType::address_type &high)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " internal error" << std::endl;
    return;
  }

  m_Transactor->set_address_range(low, high);
}

//-------------------------------------------------------------------------//

inline
osssi::osss_object_registry* 
osss_port< osss_channel_if<osss_rmi_channel_server_if> >::
getObjectRegistry()
{
  return base_type::operator->()->getObjectRegistry();
}

} // namespace osss

// $Id: osss_port_channel.tpp 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
