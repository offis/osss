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

//------------------------------------------------------------------------------
// osss_object_socket_base
//------------------------------------------------------------------------------

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
osss_object_socket_base() : 
//  sc_core::sc_module(),
  osss_module(),
  m_object_registry(),
  m_port_vec(),
  m_shared_if_ptr(NULL)
{
  m_reset_sensitivity = OSSS_POS;
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
osss_object_socket_base( sc_core::sc_module_name name ) : 
//  sc_core::sc_module(name) ,
  osss_module(name),
  m_object_registry(),
  m_port_vec(),
  m_shared_if_ptr(NULL)
{
  m_reset_sensitivity = OSSS_POS;
}

template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
reset_sensitivity(const osss_sensitivity sens)
{
  m_reset_sensitivity = sens;
}

template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
reset_process()
{
  reset();
  wait();

  while(true) { wait(); }
}

template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
reset_method()
{
  reset();
}

template<class ObjectType, class ChannelInterfaceType>
const char* 
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
kind() const
{
  return "osss_object_socket";
}

template<class ObjectType, class ChannelInterfaceType>
const osssi::osss_object_registry& 
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
object_registry() const
{
  return m_object_registry;
}

template<class ObjectType, class ChannelInterfaceType>
osss_shared_if<>*
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
get_shared_if() const
{
  return m_shared_if_ptr;
}

template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_base<ObjectType, ChannelInterfaceType>::
before_end_of_elaboration()
{
  // call before_end_of_elaboration() of base class 
  // (manages the clock and reset port binding)
  osss_module::before_end_of_elaboration();

  switch(m_reset_sensitivity)
  {
    case OSSS_POS :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, osss_module::clock_port_port.pos());
      reset_signal_is(osss_module::reset_port_port, true);
      break;
    }
    case OSSS_NEG :
    {
      OSSS_UNIQUE_CTHREAD(reset_process, osss_module::clock_port_port.pos());
      reset_signal_is(osss_module::reset_port_port, false);
      break;
    }
    case OSSS_LEVEL :
    {
      OSSS_UNIQUE_METHOD(reset_method);
      dont_initialize();
      sensitive << osss_module::reset_port_port;
      break;
    }
    default:
    {
      OSSS_UNIQUE_CTHREAD(reset_process, osss_module::clock_port_port.pos());
      reset_signal_is(osss_module::reset_port_port, true);
      break;
    }
  }
}

//------------------------------------------------------------------------------
// osss_object_socket
//------------------------------------------------------------------------------

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket<ObjectType, ChannelInterfaceType>::
osss_object_socket() : 
  base_type(
    sc_core::sc_module_name(sc_core::sc_gen_unique_name("object_socket")) )
{
  m_objectPointer = new ObjectType("object");
  m_channel_port = NULL;

  std::cout << "Warning: You have used the default constructor of "
            << "osss_object_socket<...> ( " 
            << base_type::name() << " )." << std::endl
            << "For Synthesis it is necessary to give this Object Socket "
            << "an explicit name by calling constructor "
            << "osss_object_socket( sc_core::sc_module_name( const char* ) )." 
            << std::endl << std::endl;
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket<ObjectType, ChannelInterfaceType>::
osss_object_socket( sc_core::sc_module_name name ) : base_type( name )
{
  m_objectPointer = new ObjectType("object");
  m_channel_port = NULL;
}

template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket<ObjectType, ChannelInterfaceType>::
//bind(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_)
bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_)
{
  if (m_channel_port == NULL)
  {
    m_channel_port = 
      new port_type(sc_core::sc_gen_unique_name("osss_object_socket_port"),
                    this, port_ /*export_*/);
    m_channel_port->clock_port(base_type::clock_port_port);
    m_channel_port->reset_port(base_type::reset_port_port);
    m_channel_port->register_at_rmi_channel();
    base_type::m_port_vec.push_back( m_channel_port );
  }
  else
  {
    std::cerr << "Error: osss_object_socket has already been bound to a channel" 
              << std::endl;
  }
}

template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket<ObjectType, ChannelInterfaceType>::
//bind(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_)
bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_, unsigned int dummy)
{
  if (m_channel_port == NULL)
  {
    m_channel_port = 
      new port_type(sc_core::sc_gen_unique_name("osss_object_socket_port"),
                    this, port_ /*export_*/);
    m_channel_port->clock_port(base_type::clock_port_port);
    m_channel_port->reset_port(base_type::reset_port_port);
    m_channel_port->register_at_rmi_channel();
    base_type::m_port_vec.push_back( m_channel_port );
  }
  else
  {
    std::cerr << "Error: osss_object_socket has already been bound to a channel" 
              << std::endl;
  }
}

// TODO: propagate reset sensitivity
template<class ObjectType, class ChannelInterfaceType>
template<class ChannelType>
void
osss_object_socket<ObjectType, ChannelInterfaceType>::
bind(ChannelType& channel)
{
  if (m_channel_port == NULL)
  {
    m_channel_port = 
      new port_type(sc_core::sc_gen_unique_name("osss_object_socket_port"),
                    this);
    m_channel_port->bind(channel);
    m_channel_port->clock_port(base_type::clock_port_port);
    m_channel_port->reset_port(base_type::reset_port_port);
    m_channel_port->register_at_rmi_channel();
    base_type::m_port_vec.push_back( m_channel_port ); 
  }
  else
  {
    std::cerr << "Error: osss_object_socket has already been bound to a channel" 
              << std::endl;
  }
}

template<class ObjectType, class ChannelInterfaceType>
const std::vector<sc_core::sc_port_base*>& 
osss_object_socket<ObjectType, ChannelInterfaceType>::
get_ports() const
{
  return base_type::m_port_vec;
}

template<class ObjectType, class ChannelInterfaceType>
ObjectType& 
osss_object_socket<ObjectType, ChannelInterfaceType>::plugged_object() const
{
  return *m_objectPointer;
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket<ObjectType, ChannelInterfaceType>::
~osss_object_socket()
{
  if (m_objectPointer != NULL)
  {
    delete m_objectPointer;
  }
  m_objectPointer = NULL;
  
  if (m_channel_port != NULL)
  {
    delete m_channel_port;
  }
  m_channel_port = NULL;
}

template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket<ObjectType, ChannelInterfaceType>::
reset()
{
  if ( m_objectPointer != NULL )
  {
    delete m_objectPointer;
  }
  m_objectPointer = new ObjectType();
}

//------------------------------------------------------------------------------
// osss_object_socket (specialisation for shared objects)
//------------------------------------------------------------------------------

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
osss_object_socket() : 
  base_type( 
    sc_core::sc_module_name(sc_core::sc_gen_unique_name("object_socket")) )
{
  m_shared_objectPointer = 
    new osss_shared<ObjectType, SchedulerType>("shared_object");
  base_type::m_shared_if_ptr = m_shared_objectPointer;
  base_type::m_object_registry.addClass(m_shared_objectPointer->operator->());
  // bind the clock/reset port of the shared object 
  // to the clock/reset port of this socket
  m_shared_objectPointer->clock_port(base_type::clock_port_port);
  m_shared_objectPointer->reset_port(base_type::reset_port_port);

  std::cout << "Warning: You have used the default constructor of "
            << "osss_object_socket<osss_shared<...> > ( " 
            << base_type::name() << " )." << std::endl
            << "For Synthesis it is necessary to give this Object Socket "
            << "an explicit name by calling constructor "
            << "osss_object_socket( sc_core::sc_module_name( const char* ) )." 
            << std::endl << std::endl;
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
osss_object_socket( sc_core::sc_module_name name ) : 
  base_type( name )
{
  m_shared_objectPointer = 
    new osss_shared<ObjectType, SchedulerType>("shared_object");
  base_type::m_shared_if_ptr = m_shared_objectPointer;
  base_type::m_object_registry.addClass(m_shared_objectPointer->operator->());
  // bind the clock/reset port of the shared object 
  // to the clock/reset port of this socket

  m_shared_objectPointer->clock_port(base_type::clock_port_port);
  m_shared_objectPointer->reset_port(base_type::reset_port_port);
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
//bind(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_)
bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_)
{
  port_type* port = 
    new port_type(sc_core::sc_gen_unique_name("osss_object_socket_port"),
                  this, port_ /*export_*/);
  port->clock_port(base_type::clock_port_port);
  port->reset_port(base_type::reset_port_port);
  port->register_at_rmi_channel();
  m_channel_ports.push_back( port );
  base_type::m_port_vec.push_back( port );
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
//bind(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_)
bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_, unsigned int dummy)
{
  port_type* port = 
    new port_type(sc_core::sc_gen_unique_name("osss_object_socket_port"),
                  this, port_ /*export_*/);
  port->clock_port(base_type::clock_port_port);
  port->reset_port(base_type::reset_port_port);
  port->register_at_rmi_channel();
  m_channel_ports.push_back( port );
  base_type::m_port_vec.push_back( port );
}

// TODO: propagate reset sensitivity
// TODO: check if the same channel is bound more than once
template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
template<class ChannelType>
void 
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
bind(ChannelType& channel)
{
  port_type* port = 
    new port_type(sc_core::sc_gen_unique_name("osss_object_socket_port"), this);
  port->template bind<ChannelType>(channel);
  port->clock_port(base_type::clock_port_port);
  port->reset_port(base_type::reset_port_port);
  port->register_at_rmi_channel();
  m_channel_ports.push_back( port );
  base_type::m_port_vec.push_back( port );
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
const std::vector<sc_core::sc_port_base*>& 
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
get_ports() const
{
  return base_type::m_port_vec;
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_shared<ObjectType, SchedulerType>&
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
plugged_object() const
{
  return *m_shared_objectPointer;
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
~osss_object_socket()
{
  if (m_shared_objectPointer != NULL)
  {
    delete m_shared_objectPointer;
  }
  m_shared_objectPointer = NULL;
  
  for(unsigned int i=0; i<m_channel_ports.size(); i++)
  {
    delete m_channel_ports[i];
  }
  m_channel_ports.clear();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
unsigned int
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
numChannelPorts()
{
  return m_channel_ports.size();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
reset()
{
  // do nothing beacause reset is performed inside osss_shared
}

} // namespace osss

// $Id: osss_object_socket.tpp 2586 2008-08-26 13:01:24Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
