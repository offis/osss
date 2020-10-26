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

#ifndef OSSS_PORT_CHANNEL_HH
#define OSSS_PORT_CHANNEL_HH

#include <string>

#include "o3s/utils/osss_types.h"
#include "o3s/communication/arch/osss_channel/osss_method_if.h"
#include "o3s/communication/arch/osss_channel/osss_address_map.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_client_if.h"
#include "o3s/communication/arch/osss_rmi/osss_transactor_accessor.h"

namespace osss {

template<class IF> class osss_channel_if;
class osss_basic_channel;

template< class InterfaceType >
class osss_port<osss_channel_if<InterfaceType> > 
  : public sc_core::sc_port<osss_channel_if<>, 1 >
{
public:

  // typedefs
  
  typedef osss_channel_if<>                          if_type;
  typedef sc_core::sc_port<if_type, 1>               base_type;
  typedef osss_port<osss_channel_if<InterfaceType> > this_type;

public:

  // constructors

  osss_port()
    : base_type(), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  explicit osss_port( const char* name_ )
    : base_type(name_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  explicit osss_port( if_type& interface_ )
    : base_type(interface_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  osss_port( const char* name_, if_type& interface_ )
    : base_type(name_, interface_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  explicit osss_port( base_type& parent_ )
    : base_type(parent_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  osss_port( const char* name_, base_type& parent_ )
    : base_type(name_, parent_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  explicit osss_port( this_type& parent_ )
    : base_type(parent_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL )
    {}

  osss_port( const char* name_, this_type& parent_ )
    : base_type(name_, parent_), 
      m_firstAccess( true ),
      m_proc_id( 0 ),
      m_Transactor( NULL)
    {}

  // destructor
  virtual ~osss_port()
    {}

  virtual const char* kind() const
  { 
    return "osss_port_channel"; 
  }
    
  void bind (if_type &interface_);
  void operator() (if_type &interface_);

  void bind (this_type &port);
  void operator() (this_type &port);
  
  InterfaceType* operator ->();
  
  template < class ADDRESS > 
  void setAddressRange( const ADDRESS &low, const ADDRESS &high );
  
  osssi::osss_object_registry* getObjectRegistry();

  void setPriority(osss_priority priority);
  
  void trace(std::string name);
  void trace(sc_core::sc_trace_file* tf, std::string name);

protected:

  virtual void end_of_elaboration()
  {
    /*
    // debug information
    std::cout << "name: " << base_type::name()
              << " kind: " << kind()
              << " port address: " << this 
              << std::endl;
    */

    // check if the complete() method of an osss_channel has been called
    osss_basic_channel* channel = 
      dynamic_cast<osss_basic_channel*>(base_type::operator->());
    if(std::string(channel->kind()) == std::string("osss_channel"))
    {
      if (!channel->m_complete_method_called)
      {
        std::cerr << "ERROR: complete() method of channel "
                  << channel->name() << " has not been called."
                  << std::endl;
        OSSS_ABORT();
      }
    }

    // check whether InterfaceType is derived from osss_method_if
    if (dynamic_cast<osss::osss_method_if*>(m_Transactor) == NULL)
    {
      std::cerr << "ERROR: Interface of " 
                << this->kind() << " "
                << this->name() 
                << " needs to be derived from osss::osss_method_if."
                << std::endl;
      OSSS_ABORT();
    }
  }

  bool m_firstAccess;
  osssi::OSSS_Process_ID_t m_proc_id;
  InterfaceType *m_Transactor;

private:
  // disabled copy-constructor
  osss_port( const this_type& ) {}
 
};

//------------------------------------------------------------------------------
// partial specialisation for
//   osss_rmi_channel_client_if
//------------------------------------------------------------------------------

template<>
class osss_port<osss_channel_if<osss_rmi_channel_client_if> > 
  : public sc_core::sc_port<osss_channel_if<>, 0 >  // here we need to allow
                                                    // at least more than a
                                                    // single binding due to the
                                                    // fact that multiple ports
                                                    // from a software task can
                                                    // be bound to this port
{
public:

  // typedefs
  
  typedef osss_channel_if<>                                       if_type;
  typedef sc_core::sc_port<if_type, 0>                            base_type;
  typedef osss_port<osss_channel_if<osss_rmi_channel_client_if> > this_type;

public:

  // constructors

  osss_port()
    : base_type(), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  explicit osss_port( const char* name_ )
    : base_type(name_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  explicit osss_port( if_type& interface_ )
    : base_type(interface_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  osss_port( const char* name_, if_type& interface_ )
    : base_type(name_, interface_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  explicit osss_port( base_type& parent_ )
    : base_type(parent_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  osss_port( const char* name_, base_type& parent_ )
    : base_type(name_, parent_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  explicit osss_port( this_type& parent_ )
    : base_type(parent_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  osss_port( const char* name_, this_type& parent_ )
    : base_type(name_, parent_), 
      m_Transactor( NULL ),
      m_rmi_channel_ptr( NULL )
    {}

  // destructor
  virtual ~osss_port()
    {}

  virtual const char* kind() const
  { 
    return "osss_port_channel"; 
  }
  
  template<class ChannelType>
  void bind(ChannelType &channel);

  template<class ChannelType>
  void operator()(ChannelType &channel);

  void bind_to_rmi(this_type &port, bool register_for_rmi = true);
 
  void bind_to_port (this_type &port);
  //void operator() (this_type &port);
  
  osss_rmi_channel_client_if* operator ->();
  
  osssi::osss_object_registry* getObjectRegistry();

  template<class ChannelType>
  void setPriority(osss_priority priority);
  
  template<class ChannelType>
  void trace(std::string name);

  template<class ChannelType>
  void trace(sc_core::sc_trace_file* tf, std::string name);

protected:

  virtual
  void 
  register_at_rmi_channel(osss_rmi_channel_registry_if* rmi_channel_pointer)
  {
    /* this method does nothing
       it is implemented in osss_port_rmi.h */
  }

  virtual void end_of_elaboration()
  {
    // debug information
    //std::cout << "name: " << base_type::name()
    //          << " kind: " << kind()
    //          << " port address: " << this 
    //          << std::endl;

    // check if the complete() method of an osss_channel has been called
    osss_basic_channel* channel = 
      dynamic_cast<osss_basic_channel*>(base_type::operator->());
    if(std::string(channel->kind()) == std::string("osss_channel"))
    {
      if (!channel->m_complete_method_called)
      {
        std::cerr << "ERROR: complete() method of channel "
                  << channel->name() << " has not been called."
                  << std::endl;
        OSSS_ABORT();
      }
    }
  }

  client_transactor_accessor* m_Transactor;
  osss_rmi_channel_registry_if* m_rmi_channel_ptr;

private:
  // disabled copy-constructor
  osss_port( const this_type& ) : base_type() {}
 
};

//------------------------------------------------------------------------------
// partial specialisation for
//   osss_rmi_channel_server_if
//------------------------------------------------------------------------------

template<>
class osss_port<osss_channel_if<osss_rmi_channel_server_if> > 
  : public sc_core::sc_port<osss_channel_if<>, 1 >
{
public:

  // typedefs
  
  typedef osss_channel_if<>                                       if_type;
  typedef sc_core::sc_port<if_type, 1>                            base_type;
  typedef osss_port<osss_channel_if<osss_rmi_channel_server_if> > this_type;

public:

  // constructors

  osss_port()
    : base_type(), 
      m_Transactor( NULL )
    {}

  explicit osss_port( const char* name_ )
    : base_type(name_), 
      m_Transactor( NULL )
    {}

  explicit osss_port( if_type& interface_ )
    : base_type(interface_), 
      m_Transactor( NULL )
    {}

  osss_port( const char* name_, if_type& interface_ )
    : base_type(name_, interface_), 
      m_Transactor( NULL )
    {}

  explicit osss_port( base_type& parent_ )
    : base_type(parent_), 
      m_Transactor( NULL )
    {}

  osss_port( const char* name_, base_type& parent_ )
    : base_type(name_, parent_), 
      m_Transactor( NULL )
    {}

  explicit osss_port( this_type& parent_ )
    : base_type(parent_), 
      m_Transactor( NULL )
    {}

  osss_port( const char* name_, this_type& parent_ )
    : base_type(name_, parent_), 
      m_Transactor( NULL)
    {}

  // destructor
  virtual ~osss_port()
    {}

  virtual const char* kind() const
  { 
    return "osss_port_channel"; 
  }
  
  template<class ChannelType>
  void bind(ChannelType &channel);

  template<class ChannelType>
  void operator()(ChannelType &channel);

  void bind_to_port (this_type &port);
  //void operator() (this_type &port);
  
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

    // check if the complete() method of an osss_channel has been called
    osss_basic_channel* channel = 
      dynamic_cast<osss_basic_channel*>(base_type::operator->());
    if(std::string(channel->kind()) == std::string("osss_channel"))
    {
      if (!channel->m_complete_method_called)
      {
        std::cerr << "ERROR: complete() method of channel "
                  << channel->name() << " has not been called."
                  << std::endl;
        OSSS_ABORT();
      }
    }
  }

  server_transactor_accessor* m_Transactor;

private:
  // disabled copy-constructor
  osss_port( const this_type& ):base_type() {}
 
};

} // namespace osss

#include "o3s/communication/arch/osss_channel/osss_port_channel.tpp"

#endif

// $Id: osss_port_channel.h 2915 2008-12-15 18:25:28Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
