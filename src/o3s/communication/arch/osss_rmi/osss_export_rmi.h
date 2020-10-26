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

#ifndef OSSS_EXPORT_RMI_HH
#define OSSS_EXPORT_RMI_HH

#include "o3s/communication/arch/osss_rmi/osss_object_socket_if.h"

namespace osss {

template< class InterfaceType >
class osss_export<osss_rmi_if<InterfaceType> > 
  : public osss_port<osss_channel_if<osss_rmi_channel_server_if> >
{
public:

  template<class ObjectType, class ChannelInterfaceType>
  friend class osss_object_socket_port_base;

  // typedefs

  typedef osss_channel_if<>                                       if_type;
  typedef osss_port<osss_channel_if<osss_rmi_channel_server_if> > base_type;
  typedef osss_export<osss_rmi_if<InterfaceType> >                this_type;

public:

  // constructors

  osss_export()
    : base_type(), 
      m_Transactor( NULL )
  {
    std::cout << "Warning: You are using the default constructor of "
              << "osss_export<osss_rmi_if<...> >." << std::endl
              << "For synthesis it is required that the name of this export "
              << "matches its identifier. " << std::endl
              << "Please use osss_export( const char* ) constructor for exosss_port "
              << name() << " instead."<< std::endl
              << std::endl;
  }

  explicit osss_export( const char* name_ )
    : base_type(name_), 
      m_Transactor( NULL )
    {}

  // port -> if
  explicit osss_export( if_type& interface_ )
    : base_type(interface_), 
      m_Transactor( NULL )
  {
    std::cout << "Warning: You are using the osss_export( osss_channel_if<>& ) " 
              << "constructor of osss_export<osss_rmi_if<...> >." << std::endl
              << "For synthesis it is required that the name of this export "
              << "matches its identifier. " << std::endl
              << "Please use osss_export( const char*, osss_channel_if<>& ) "
              << "constructor for exosss_port "
              << name() << " instead."<< std::endl
              << std::endl;
  }

  // port -> if
  osss_export( const char* name_, if_type& interface_ )
    : base_type(name_, interface_), 
      m_Transactor( NULL )
    {}

  /*
    port -> port (parent) binding is not allowed !!!

  // port -> port (parent)
  explicit osss_export( base_type& parent_ )
    : base_type(parent_), 
      m_Transactor( NULL )
    {}

  // port -> port (parent)
  osss_export( const char* name_, base_type& parent_ )
    : base_type(name_, parent_), 
      m_Transactor( NULL )
    {}
  */

  // port -> port (this)
  explicit osss_export( this_type& parent_ )
    : base_type(parent_), 
      m_Transactor( NULL )
  {
    std::cout << "Warning: You are using the osss_export( osss_export<osss_rmi_if<...> >& ) " 
              << "constructor of osss_export<osss_rmi_if<...> >." << std::endl
              << "For synthesis it is required that the name of this export "
              << "matches its identifier. " << std::endl
              << "Please use osss_export( const char*, osss_export<osss_rmi_if<...> >& ) "
              << "constructor for exosss_port "
              << name() << " instead."<< std::endl
              << std::endl;

    m_Transactor = parent_.m_Transactor;
  }

  // port -> port (this)
  osss_export( const char* name_, this_type& parent_ )
    : base_type(name_, parent_), 
      m_Transactor( NULL)
  {
    m_Transactor = parent_.m_Transactor;
  }

  osss_export( const char* name_, osss_object_socket_if& obj_socket )
    : base_type(name_), 
      m_Transactor( NULL)
  {
    obj_socket.bind(*this);
  }

  // destructor
  virtual ~osss_export()
    {}

  virtual const char* kind() const
  { 
    return "osss_export"; 
  }

  void 
  operator()( const char* name, osss_object_socket_if& obj_socket);
 
  void 
  operator()(osss_object_socket_if& obj_socket, unsigned int dummy);
   
  // port -> if (any ???)
  template<class ChannelType>
  void bind(ChannelType &channel);

  template<class ChannelType>
  void bind(ChannelType &channel,
	    const typename ChannelType::address_type &low, 
	    const typename ChannelType::address_type &high);

  // port -> if (any ???)
  template<class ChannelType>
  void operator()(ChannelType &channel);

  template<class ChannelType>
  void operator()(ChannelType &channel,
		  const typename ChannelType::address_type &low, 
		  const typename ChannelType::address_type &high);
  
  osss_rmi_channel_server_if* operator ->();
  
  template<class ChannelType> 
  void setAddressRange(const typename ChannelType::address_type &low, 
		       const typename ChannelType::address_type &high);
  
  osssi::osss_object_registry* getObjectRegistry();

  template<class ChannelType>
  void trace(std::string name);

  template<class ChannelType>
  void trace(sc_core::sc_trace_file* tf, std::string name);

protected:

  virtual void end_of_elaboration()
  {
    // debug information
    //std::cout << "name: " << base_type::name()
    //          << " kind: " << kind()
    //          << " port address: " << this 
    //          << std::endl;
  }
    
  osss_rmi_channel_server_if *m_Transactor;

private:
  // disabled copy-constructor
  osss_export( const this_type& );
 
};

#include "o3s/communication/arch/osss_rmi/osss_export_rmi.tpp"

} // namespace osss

#endif
// $Id: osss_export_rmi.h 2586 2008-08-26 13:01:24Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
