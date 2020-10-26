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

#ifndef OSSS_OBJECT_SOCKET_PORT_HH
#define OSSS_OBJECT_SOCKET_PORT_HH

#include "o3s/utils/osss_systemc.h"
#include <queue>

#include "o3s/communication/arch/osss_rmi/osss_rmi_types.h"
#include "o3s/communication/arch/osss_rmi/osss_object_registry.h"
#include "o3s/communication/arch/osss_rmi/osss_rmi_channel_server_if.h"

//#define DEBUG_RMI_PORT

namespace osss {

//namespace osssi {

//------------------------------------------------------------------------------
// osss_object_socket_port_base
//------------------------------------------------------------------------------

template<class Channel_if, int N> class osss_port;

template<class ObjectType,
         class ChannelInterfaceType>
class osss_object_socket_port_base : 
  public osss_port<osss_channel_if<ChannelInterfaceType> >,
  public osss_port_rmi_information_if
{
public:

  typedef osss_port<osss_channel_if<ChannelInterfaceType> > channel_port;

  osss_object_socket_port_base();

  explicit osss_object_socket_port_base(/* osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& export_*/
    osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port);
  
  virtual ~osss_object_socket_port_base();

  virtual const char* kind() const
  { 
    return "osss_object_socket_port_port"; 
  }

  template<class ChannelType>
  void bind(ChannelType &channel);

  template<class ChannelType>
  void operator()(ChannelType &channel);
 
  void protocol_process();
 
  virtual void behaviour_process() = 0;

protected:

  #ifdef DEBUG_RMI_PORT
  std::ofstream log_file;
  #endif

public:

  virtual void register_at_rmi_channel();
 
protected:

  void invoke_method(osss_method_call_item* actual_mci_ptr);
 
  OSSS_RMI_Server_State_t m_server_state;
  OSSS_MethodID_Record_t  m_reception_record;

  std::queue<OSSS_ClientID_t> m_ready_queue;
  sc_core::sc_event m_ready_queue_changed;
  
  ///-----------------------------------------------------------------------
  // This pointer points to an instance of the user-defined class that was
  // passed as second template argument to the osss_object_socket_port at 
  // declaration.
  ObjectType *m_objectPointer;
  osss_rmi_channel_registry_if *m_rmi_channelPointer;

 private:
  // this port can NOT be copied
  osss_object_socket_port_base(const osss_object_socket_port_base& pb);
  osss_object_socket_port_base& 
    operator=(const osss_object_socket_port_base& pb);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template<class, class> class osss_object_socket;

//------------------------------------------------------------------------------
// osss_object_socket_port
//------------------------------------------------------------------------------

template<class ObjectType,
	 class ChannelInterfaceType = osss_rmi_channel_server_if>
class osss_object_socket_port 
  : public sc_core::sc_module, 
    public osss_object_socket_port_base<ObjectType, ChannelInterfaceType>
{

public:
  
  typedef osss_object_socket_port_base<ObjectType, 
				       ChannelInterfaceType> base_type;
  
  sc_core::sc_in< bool > clock_port;

  ///-----------------------------------------------------------------------
  // Reset port. When the signal (bound to this port changes), the 
  // reset_process (below) is executed once
  sc_core::sc_in< bool > reset_port;
 
  ///-----------------------------------------------------------------------
  // Needed for the processes (below)
  SC_HAS_PROCESS(osss_object_socket_port);

  ///-----------------------------------------------------------------------
  // Default constructor. Calls the default constructor of the user defined 
  // class instance registers its address at the osss_object_registry and
  // registers the reset_process (as SC_METHOD) at the SystemC scheduler.
  osss_object_socket_port(osss_object_socket<ObjectType, 
                                             ChannelInterfaceType>* obj_socket);

  osss_object_socket_port(osss_object_socket<ObjectType, 
			                     ChannelInterfaceType>* obj_socket,
			  /*osss_export<
			    osss_rmi_if<
                            osss_rmi_channel_server_if> >& export_*/
                          osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port);
  
  ///-----------------------------------------------------------------------
  // Same as default constructor but enables the specification of a module 
  // name as known from SystemC modules
  osss_object_socket_port(sc_core::sc_module_name name,
			  osss_object_socket<ObjectType, 
                                             ChannelInterfaceType>* obj_socket);
  
  osss_object_socket_port(sc_core::sc_module_name name,
			  osss_object_socket<ObjectType, 
			                     ChannelInterfaceType>* obj_socket,
                          /*osss_export<
			    osss_rmi_if<
                            osss_rmi_channel_server_if> >& export_*/
                          osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port);

  virtual const char* kind() const
  { 
    return "osss_object_socket_port"; 
  } 

  //
  // osss_port_rmi_information_if methods
  //
  virtual sc_core::sc_object* bound_channel() const;
  virtual sc_core::sc_object* bound_object_socket() const;
  virtual bool is_hierarchical() const;
  virtual bool is_accessor() const;
    
  virtual void updateObjectRegistry();

  ///-----------------------------------------------------------------------
  // This operator must be used for calling methods on an osss_object_socket_port. 
  // The calls are redirected to m_objectPointer. This allows to specifiy
  // m_objectPointer protected.
  virtual ObjectType * operator->() const;

  void updateObjectRegistry_process();

  void protocol_process();
  
  virtual void behaviour_process();
 

protected:

  virtual void declare_processes();
 
  osss_object_socket<ObjectType, ChannelInterfaceType>* m_obj_socket;

private:
  // this port can NOT be copied
  osss_object_socket_port(const osss_object_socket_port& pb);
  osss_object_socket_port& 
    operator=(const osss_object_socket_port& pb);

};

//------------------------------------------------------------------------------
// osss_object_socket_port (specialisation for shared objects)
//------------------------------------------------------------------------------

template<class ObjectType, class SchedulerType,
	 class ChannelInterfaceType>
class osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>
  : public sc_core::sc_module,
    public osss_object_socket_port_base<ObjectType, ChannelInterfaceType>
{
public:

  typedef osss_object_socket_port_base<ObjectType, 
				       ChannelInterfaceType> base_type;
  
  sc_core::sc_in< bool > clock_port;

  ///-----------------------------------------------------------------------
  // Reset port. When the signal (bound to this port changes), the 
  // reset_process (below) is executed once
  sc_core::sc_in< bool > reset_port;
 
  ///-----------------------------------------------------------------------
  // Needed for the processes (below)
  SC_HAS_PROCESS(osss_object_socket_port);

  ///-----------------------------------------------------------------------
  // Default constructor. Calls the default constructor of the user defined 
  // class instance registers its address at the osss_object_registry and
  // registers the reset_process (as SC_METHOD) at the SystemC scheduler.
  osss_object_socket_port( osss_object_socket< 
			     osss_shared< ObjectType, SchedulerType >,
                               ChannelInterfaceType>* obj_socket );

  osss_object_socket_port( osss_object_socket< 
			     osss_shared< ObjectType, SchedulerType >,
			       ChannelInterfaceType>* obj_socket,
			   /*osss_export<
			     osss_rmi_if<
                             osss_rmi_channel_server_if> >& export_*/
                           osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port);
  
  ///-----------------------------------------------------------------------
  // Same as default constructor but enables the specification of a module 
  // name as known from SystemC modules
  osss_object_socket_port( sc_core::sc_module_name name, 
			   osss_object_socket< 
			     osss_shared< ObjectType, SchedulerType >,
                               ChannelInterfaceType>* obj_socket );
    
  osss_object_socket_port( sc_core::sc_module_name name, 
			   osss_object_socket< 
			     osss_shared< ObjectType, SchedulerType >,
			       ChannelInterfaceType>* obj_socket, 
			   /*osss_export<
			     osss_rmi_if<
                             osss_rmi_channel_server_if> >& export_*/
                           osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port);

  virtual const char* kind() const
  { 
    return "osss_object_socket_port"; 
  }

  //
  // osss_port_rmi_information_if methods
  //
  virtual sc_core::sc_object* bound_channel() const;
  virtual sc_core::sc_object* bound_object_socket() const;
  virtual bool is_hierarchical() const;
  virtual bool is_accessor() const;
   
  virtual void updateObjectRegistry();

  virtual osss_shared<ObjectType, SchedulerType>* operator->() const;
 
  void updateObjectRegistry_process();
 
  void protocol_process();
 
//  #if defined(OSSS_SIMPLE_GUARDS)
  void behaviour_process();
 
//  #else
  #ifdef FOO
  virtual void behaviour_process();
  #endif

protected:

  bool invoke_guard(osss_method_call_item actual_mci);

  virtual void declare_processes();

 public:

  virtual void register_at_rmi_channel();

 protected:
 
  osss_shared<ObjectType, SchedulerType>* m_shared_objectPointer;

  osss_object_socket<osss_shared<ObjectType, SchedulerType>, 
		     ChannelInterfaceType >* m_obj_socket;

 private:
  // this port can NOT be copied
  osss_object_socket_port(const osss_object_socket_port& pb);
  osss_object_socket_port& 
    operator=(const osss_object_socket_port& pb);

};

//------------------------------------------------------------------------------

//} // namespace osssi

} //namespace osss

#include "o3s/communication/arch/osss_rmi/osss_object_socket_port.tpp"

#endif
// $Id: osss_object_socket_port.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
