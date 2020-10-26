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

#ifndef OSSS_OBJECT_SOCKET_HH
#define OSSS_OBJECT_SOCKET_HH

#include "o3s/communication/arch/osss_rmi/osss_rmi_if.h"
#include "o3s/communication/arch/osss_rmi/osss_object_socket_port.h"
#include "o3s/communication/arch/osss_rmi/osss_object_socket_if.h"

namespace osss {

//------------------------------------------------------------------------------
// osss_object_socket_base
//------------------------------------------------------------------------------

template<class ObjectType,
         class ChannelInterfaceType>
class osss_object_socket_base : 
  //public sc_core::sc_module,
  public osss_module,
  public osss_object_socket_if
{
public:
 
  ///-----------------------------------------------------------------------
  // Needed for the processes (below)
  OSSS_HAS_PROCESS(osss_object_socket_base);

  ///-----------------------------------------------------------------------
  // Same as default constructor but enables the specification of a module 
  // name as known from SystemC modules
  osss_object_socket_base();

  explicit osss_object_socket_base( sc_core::sc_module_name name );

  virtual void reset_sensitivity(const osss_sensitivity sens);

  virtual const char* kind() const;

  virtual const osssi::osss_object_registry& object_registry() const;

  virtual osss_shared_if<>* get_shared_if() const;
 
protected:

  ///-----------------------------------------------------------------------
  // Reset Process. Is called once when the signal bound to the reset_port
  // changes. This process calls the reset() method of the osss_object_socket.
  void reset_process();
 
  void reset_method();
 
  ///-----------------------------------------------------------------------
  // Resets the osss_object_socket_base.
  virtual void reset() = 0;

  virtual void before_end_of_elaboration();
 
  osss_sensitivity m_reset_sensitivity;

  osssi::osss_object_registry m_object_registry;
  
  std::vector<sc_core::sc_port_base*> m_port_vec;

  osss_shared_if<>* m_shared_if_ptr;

};

//------------------------------------------------------------------------------
// osss_object_socket
//------------------------------------------------------------------------------

template<class ObjectType,
	 class ChannelInterfaceType = osss_rmi_channel_server_if>
class osss_object_socket 
  : public osss_object_socket_base<ObjectType, ChannelInterfaceType>
{
public:

  typedef osss_object_socket_base<ObjectType, ChannelInterfaceType> base_type;
  typedef osss_object_socket_port<ObjectType, ChannelInterfaceType> port_type;

  ///-----------------------------------------------------------------------
  // Default constructor. Calls the default constructor of the user defined 
  // class instance registers its address at the osss_object_registry and
  // registers the reset_process (as SC_METHOD) at the SystemC scheduler.
  osss_object_socket(); 

  ///-----------------------------------------------------------------------
  // Same as default constructor but enables the specification of a module 
  // name as known from SystemC modules
  explicit osss_object_socket( sc_core::sc_module_name name );

/*
  virtual 
  void bind(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_);
*/

  virtual void 
  bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_);

  void bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_, 
            unsigned int dummy);
  
  template<class ChannelType>
  void bind(ChannelType& channel);

  virtual 
  const std::vector<sc_core::sc_port_base*>& get_ports() const;

  virtual ObjectType& plugged_object() const;
 
  virtual ~osss_object_socket();

protected:

  virtual void reset();

  ///-----------------------------------------------------------------------
  // This pointer points to an instance of the user-defined class that was
  // passed as second template argument to the osss_object_socket at 
  // declaration.
  ObjectType *m_objectPointer; 

  port_type* m_channel_port;

};

//------------------------------------------------------------------------------
// osss_object_socket (specialisation for shared objects)
//------------------------------------------------------------------------------

template<class ObjectType, class SchedulerType,
	 class ChannelInterfaceType>
class osss_object_socket<osss_shared<ObjectType, SchedulerType>, 
                         ChannelInterfaceType>
  : public osss_object_socket_base<ObjectType, ChannelInterfaceType>
{
public:

  typedef osss_object_socket_base<ObjectType, 
				  ChannelInterfaceType> base_type;

  typedef osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, 
				  ChannelInterfaceType> port_type;

  ///-----------------------------------------------------------------------
  // Default constructor. Calls the default constructor of the user defined 
  // class instance registers its address at the osss_object_registry and
  // registers the reset_process (as SC_METHOD) at the SystemC scheduler.
  osss_object_socket();

  ///-----------------------------------------------------------------------
  // Same as default constructor but enables the specification of a module 
  // name as known from SystemC modules
  explicit osss_object_socket( sc_core::sc_module_name name );

/*
  virtual 
  void bind(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_);
*/

  virtual void 
  bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_);

  void bind(osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port_, 
            unsigned int dummy);

  // TODO: propagate reset sensitivity
  // TODO: check if the same channel is bound more than once
  template<class ChannelType>
  void bind(ChannelType& channel);

  virtual 
  const std::vector<sc_core::sc_port_base*>& get_ports() const;
  
  virtual osss_shared<ObjectType, SchedulerType>& plugged_object() const;

  virtual ~osss_object_socket();
 
  unsigned int numChannelPorts();
 
protected:

  virtual void reset();

  osss_shared<ObjectType, SchedulerType>* m_shared_objectPointer;
  std::vector<port_type*>                 m_channel_ports;
};

} // namespace osss

#include "o3s/communication/arch/osss_rmi/osss_object_socket.tpp"

#endif
// $Id: osss_object_socket.h 2586 2008-08-26 13:01:24Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
