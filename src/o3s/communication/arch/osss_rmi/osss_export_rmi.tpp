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

template<class InterfaceType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
operator()( const char* name, osss_object_socket_if& obj_socket)
{
  base_type::name(name);
  this->operator()(obj_socket);
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
operator()(osss_object_socket_if& obj_socket, unsigned int dummy)
{
  // todo: check whether the InterfaceType is right with respect
  //       to the osss_object_socket<osss_shared<User-Class> >
  obj_socket.bind(*this);
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
template<class ChannelType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
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

  m_Transactor = new typename ChannelType::server_transactor();

  typedef typename ChannelType::server_transactor transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
              << " internal error" << std::endl;
  }

  basic_channel->m_transactorList.push_back(transactor);
  
  transactor->buildConnectionList();
  transactor->connect(*basic_channel);
}

template<class InterfaceType>
template<class ChannelType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
bind(ChannelType &channel, 
     const typename ChannelType::address_type &low, 
     const typename ChannelType::address_type &high)
{
  this->bind(channel);
  this->setAddressRange(low, high);
}


//-------------------------------------------------------------------------//

template<class InterfaceType>
template<class ChannelType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
operator()(ChannelType &channel)
{
  this->bind(channel);
}

template<class InterfaceType>
template<class ChannelType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
operator()(ChannelType &channel,
	   const typename ChannelType::address_type &low, 
	   const typename ChannelType::address_type &high)
{
  this->bind(channel, low, high);
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
template<class ChannelType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
trace(std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " not bound" << std::endl;
  }

  typedef typename ChannelType::server_transactor transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);
  
  if (transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  } 
  
  transactor->trace(name);
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
template<class ChannelType>
inline
void
osss_export< osss_rmi_if<InterfaceType> >::
trace(sc_core::sc_trace_file* tf, std::string name)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " not bound" << std::endl;
  }

  typedef typename ChannelType::server_transactor transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  } 
  
  transactor->trace(tf, name);
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
inline
osss_rmi_channel_server_if*
osss_export< osss_rmi_if<InterfaceType> >::
operator->()
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
  }
  
  return m_Transactor; 
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
template<class ChannelType> 
inline
void 
osss_export< osss_rmi_if<InterfaceType> >::
setAddressRange(const typename ChannelType::address_type &low, 
		const typename ChannelType::address_type &high)
{
  if (m_Transactor == NULL)
  {
    std::cerr << "Error: " << kind() << " " << base_type::name() 
	 << " internal error" << std::endl;
    return;
  }

  typedef typename ChannelType::server_transactor transactor_t;
  
  transactor_t* transactor = dynamic_cast<transactor_t*>(m_Transactor);

  if (transactor->isMaster() && !transactor->isSlave())
  {
    std::cout << "Warning: master transactors (which use osss_master_if) "
	 << "have no address range!" << std::endl;
  }

  if (transactor->isSlave())
  {
    (dynamic_cast< osss_slave_if *>(transactor))
      ->setAddressRange<typename ChannelType::address_type>(low, high);
  }
}

//-------------------------------------------------------------------------//

template<class InterfaceType>
inline
osssi::osss_object_registry* 
osss_export< osss_rmi_if<InterfaceType> >::
getObjectRegistry()
{
  return base_type::operator->()->getObjectRegistry();
}
// $Id: osss_export_rmi.tpp 2586 2008-08-26 13:01:24Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
