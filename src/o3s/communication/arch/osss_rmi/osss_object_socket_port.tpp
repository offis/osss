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

//namespace osssi {

//------------------------------------------------------------------------------
//repeat helper macros for listed method agruments
#define _REPEAT_LIST0(M)         
#define _REPEAT_LIST1(M)    , M[0]
#define _REPEAT_LIST2(M)    _REPEAT_LIST1(M), M[1] 
#define _REPEAT_LIST3(M)    _REPEAT_LIST2(M), M[2]
#define _REPEAT_LIST4(M)    _REPEAT_LIST3(M), M[3]
#define _REPEAT_LIST5(M)    _REPEAT_LIST4(M), M[4]
#define _REPEAT_LIST6(M)    _REPEAT_LIST5(M), M[5]
#define _REPEAT_LIST7(M)    _REPEAT_LIST6(M), M[6]
#define _REPEAT_LIST8(M)    _REPEAT_LIST7(M), M[7]
#define _REPEAT_LIST9(M)    _REPEAT_LIST8(M), M[8] 
#define _REPEAT_LIST10(M)   _REPEAT_LIST9(M), M[9] 
#define _REPEAT_LIST11(M)   _REPEAT_LIST10(M), M[10]
#define _REPEAT_LIST12(M)   _REPEAT_LIST11(M), M[11] 
#define _REPEAT_LIST13(M)   _REPEAT_LIST12(M), M[12]
#define _REPEAT_LIST14(M)   _REPEAT_LIST13(M), M[13] 
#define _REPEAT_LIST15(M)   _REPEAT_LIST14(M), M[15] 
#define _REPEAT_LIST16(M)   _REPEAT_LIST15(M), M[15] 
#define _REPEAT_LIST17(M)   _REPEAT_LIST16(M), M[16] 
#define _REPEAT_LIST18(M)   _REPEAT_LIST17(M), M[17] 
#define _REPEAT_LIST19(M)   _REPEAT_LIST18(M), M[18]
#define _REPEAT_LIST20(M)   _REPEAT_LIST19(M), M[19]
#define _REPEAT_LIST(N, M)  _REPEAT_LIST##N(M)

#define _OSSS_OBJECT_SOCKET_CASE_VOID(_object_, _obj_ptr_, _num_, _in_params_) \
  case _num_ : \
  { \
    _object_->invokeVoid(_obj_ptr_ _REPEAT_LIST(_num_, _in_params_)); \
    break; \
  }

#define _OSSS_OBJECT_SOCKET_CASE(_object_, _return_value_, _obj_ptr_, _num_, _in_params_) \
  case _num_ : \
  { \
    _object_->invoke(_return_value_, _obj_ptr_ _REPEAT_LIST(_num_, _in_params_)); \
    break; \
  }
  
  
  
//macros to clean up logging
#ifdef DEBUG_RMI_PORT
#define LOG( message ) \
  log_file << "@" << sc_core::sc_time_stamp() << ", client " << clientID\
           << ": " << ( message ) << std::endl;
#else
#define LOG( message ) 
#endif

#define ASSERT_CMD_IS( id )\
  if(command_id != id)\
  {\
    std::cerr << "Error in " << __PRETTY_FUNCTION__\
              << " offending client was client " << clientID\
              << " in state " << state_iter->second\
              << ", got command_id : " << command_id\
              << " while expecting " << ( id ) << std::endl;\
              OSSS_ABORT();\
  }

//------------------------------------------------------------------------------
// osss_object_socket_port_base
//------------------------------------------------------------------------------

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
osss_object_socket_port_base() : 
  channel_port(),
  m_rmi_channelPointer(NULL)
{
#ifdef DEBUG_RMI_PORT
  std::string log_file_name = std::string(this->name()) + ".log";
  log_file.open(log_file_name.c_str(), std::ios::out | std::ios::trunc);
#endif
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
/*osss_object_socket_port_base(osss_export<osss_rmi_if<osss_rmi_channel_server_if> >& 
                             export_ ) : 
*/
osss_object_socket_port_base(
  osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port) :
  channel_port()
{
  channel_port::m_Transactor = NULL; //export_.m_Transactor;

#ifdef DEBUG_RMI_PORT
  std::string log_file_name = std::string(this->name()) + ".log";
  log_file.open(log_file_name.c_str(), std::ios::out | std::ios::trunc);
#endif
} 

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
~osss_object_socket_port_base()
{
#ifdef DEBUG_RMI_PORT
  log_file.close();
#endif
}

template<class ObjectType, class ChannelInterfaceType>
template<class ChannelType>
void 
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
bind(ChannelType &channel)
{
  channel_port::template bind<ChannelType>(channel);
  if (m_rmi_channelPointer == NULL)
    m_rmi_channelPointer = &channel;
  else
  {
    std::cerr << "Error in osss_object_socket_port: " 
              << "This port has already been bound!" << std::endl;
    OSSS_ABORT();
  }
}

template<class ObjectType, class ChannelInterfaceType>
template<class ChannelType>
void 
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
operator()(ChannelType &channel)
{
  channel_port::operator()(channel);
  if (m_rmi_channelPointer == NULL)
    m_rmi_channelPointer = &channel;
  else
  {
    std::cerr << "Error in osss_object_socket_port: " 
              << "This port has already been bound!" << std::endl;
    OSSS_ABORT();
  }
}

template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
protocol_process()
{
  OSSS_ClientID_t clientID = 0;
  OSSS_ObjectID_t objectID = 0;
  OSSS_MethodID_t methodID = 0;
  osss_rmi::Command_ID_t command_id = osss_rmi::error_id;

  channel_port::operator->()->reset();
  sc_core::wait();
    
  while(true)
  {

    objectID = 
      channel_port::getObjectRegistry()->getObjectID(m_objectPointer);
#ifdef DEBUG_RMI_PORT
    log_file << "listening @" << sc_core::sc_time_stamp() << std::endl;
#endif

    // listen for action from connected channel
    channel_port::operator->()->listen_for_action(clientID, 
                                                  objectID, 
                                                  methodID,
                                                  command_id);

#ifdef DEBUG_RMI_PORT
    log_file << "Action @" << sc_core::sc_time_stamp();
    log_file << "; clientID: " << clientID;
    log_file << "; methodID: " << methodID;
    log_file << "; command_id: " << command_id << std::endl;
#endif

    // ckeck if method call has to be served by the embedded object 
    // of this socket (this is allways true when using a bus,
    // but this needs to be checked when using a p2p connection)
    if(objectID == 
       channel_port::getObjectRegistry()->
       getObjectID(m_objectPointer))
    {
      // search for past reception of actual client
      OSSS_MethodID_Record_t::iterator method_record_iter;
      method_record_iter = m_reception_record.find(clientID);

      OSSS_RMI_Server_State_t::iterator state_iter;
      state_iter = m_server_state.find(clientID);

      // reception for actual client has been found
      if ((method_record_iter != m_reception_record.end()) &&
          (state_iter != m_server_state.end()))
      {
#ifdef DEBUG_RMI_PORT
        log_file << "=found in reception record==================" << std::endl;
        log_file << method_record_iter->second << std::endl;
        log_file << state_iter->second << std::endl;
        log_file << "============================================" << std::endl;
#endif

        switch(state_iter->second)
        {
          case osss_rmi::wait_for_access :
          {
            ASSERT_CMD_IS(osss_rmi::receive_id);
            bool methodHasArguments = channel_port::getObjectRegistry()->
              getMethodRegistry(m_objectPointer)->
                hasParameters(method_record_iter->second.methodID);
            if (!method_record_iter->second.access_granted)
            {
              //HK: Just return the grant to the client if we need to
              //and we only need to if we have to read parameters!
              //otherwise we only need to talk to the client if we are done
              LOG("not access granted");
              channel_port::operator->()->return_grant(clientID, 
                                                       false, 
                                                       methodHasArguments);
            }
            else // method_record_iter->second.access_granted
            {
              LOG("access granted");
              
              if(methodHasArguments)
              {
                //next state is receive arguments
                state_iter->second = osss_rmi::access_granted;
                channel_port::operator->()->return_grant(clientID, 
                                                       true, 
                                                       methodHasArguments);
              }
              else
              {
                channel_port::operator->()->return_grant(clientID, 
                                                       false, //not done, yet!
                                                       methodHasArguments);
                //since we have no arguments, next state is executing
                OSSS_Archive_t empty_params;
                method_record_iter->second.in_params = empty_params;
                method_record_iter->second.in_params_complete = true;

                m_ready_queue.push(clientID);     // <--- support this ???
                m_ready_queue_changed.notify();   // <--- support this ???

                state_iter->second = osss_rmi::method_executing;
              }
            }
          }
          break;

          case osss_rmi::access_granted :
          {
            ASSERT_CMD_IS(osss_rmi::submit_id);
            LOG("receiving parameters");
            OSSS_Archive_t tmp_in_params;
            channel_port::operator->()->receive_in_params(tmp_in_params);

            method_record_iter->second.in_params = tmp_in_params;
            method_record_iter->second.in_params_complete = true;

            m_ready_queue.push(clientID);     // <--- support this ???
            m_ready_queue_changed.notify();   // <--- support this ???

            state_iter->second = osss_rmi::method_executing;
          }
          break;
	     
          case osss_rmi::method_executing :
          {
            ASSERT_CMD_IS(osss_rmi::receive_id);
            bool methodIsVoid = channel_port::getObjectRegistry()->
              getMethodRegistry(m_objectPointer)->
                isVoid(method_record_iter->second.methodID);
            if (!method_record_iter->second.call_done)
            {
              LOG("method executing");
              channel_port::operator->()->return_params_idle(
              clientID, true, methodIsVoid);
            }
            else
            {
              LOG("method execution completed!");
              channel_port::operator->()->return_params_idle(
              clientID, false, methodIsVoid);
              if(methodIsVoid)
              {
                LOG("no return values, switching to idle");
                state_iter->second = osss_rmi::idle;
                m_reception_record.erase(clientID);
                m_server_state.erase(clientID);
              }
              else
              {
                state_iter->second = osss_rmi::method_execution_done;
              }
            }
          }
          break;

          //a better name for this state would be "provide_return_values"
          //since it is skipped if method does not have any return values.
          case osss_rmi::method_execution_done :
          {
            ASSERT_CMD_IS(osss_rmi::receive_id);
            LOG("providing return Values");

            if (!method_record_iter->second.return_params_complete)
            {
              method_record_iter->second.return_params->prepare_read();
              method_record_iter->second.return_params_complete = true;
            }

            bool return_complete = 
              channel_port::operator->()->provide_return_params(
                clientID, 
                *(method_record_iter->second.return_params));
            if (return_complete)
            {
              LOG("return values send, going to idle!");
              state_iter->second = osss_rmi::idle;

              m_reception_record.erase(clientID);
              m_server_state.erase(clientID);
            }
          }
          break;

          case osss_rmi::idle :
          {
            std::cerr << "client is in reception record, and state is idle, this should never happen"
                      << std::endl;
            OSSS_ABORT();
          }
          break;

          default :
          {
            std::cerr << "Error in " << __PRETTY_FUNCTION__ << ": " 
                      << "unknown state!" << std::endl;
            OSSS_ABORT();
          }
          break;

        }
      }
      // no past receptions of actual client found
      else
      {
#ifdef DEBUG_RMI_PORT
        log_file << "client " << clientID << ": start at "
                 << sc_core::sc_time_stamp() << std::endl;
#endif

        if (command_id == osss_rmi::start_id)
        {
#ifdef DEBUG_RMI_PORT
          log_file << "start_id OK" << std::endl;
#endif

          osss_method_call_item initial_mci;
          initial_mci.clientID = clientID;
          initial_mci.objectID = objectID;
          initial_mci.methodID = methodID;

          // add new method_call_item to the reception record
          m_reception_record[clientID] = initial_mci;
          m_server_state[clientID] = osss_rmi::wait_for_access;
        }
        else if (command_id != osss_rmi::end_id)
        {
          std::cerr << "Error in " << __PRETTY_FUNCTION__ << ": "
                    << "caused by client " << clientID 
                    << " actual clientID is not in the reception record "
                    << "and no start_id." << " ID is: " << command_id << std::endl;
          OSSS_ABORT();
        }
      }
    }
    else // method call has not to be served by 
      // the embedded object of this socket
    {
      // doing nothing
#ifdef DEBUG_RMI_PORT
      log_file << "at: " << sc_core::sc_time_stamp() << std::endl;
      log_file << "do not need to serve method call" << std::endl;
      log_file << std::endl;
#endif
    }
  }
}


template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
register_at_rmi_channel()
{
#ifdef DEBUG_RMI_PORT
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << "---------------------------------------------------------" << std::endl;

  std::cout << channel_port::name() 
            << ": register plugged object at osss_object_registry"
            << std::endl;
#endif

  channel_port::getObjectRegistry()->addClass(m_objectPointer);

  //debugging
#ifdef DEBUG_RMI_PORT
  std::cout << std::endl;
  std::cout << *(channel_port::getObjectRegistry()) << std::endl;
#endif
}

template<class ObjectType, class ChannelInterfaceType>
void 
osss_object_socket_port_base<ObjectType, ChannelInterfaceType>::
invoke_method(osss_method_call_item* actual_mci_ptr)
{
  using namespace agm::reflection;
  using osss::osssi::removeClassName;
  using osss::osssi::extractTypesFromParameterList;

  std::string requested_method = 
    channel_port::getObjectRegistry()->
    getMethodRegistry(m_objectPointer)->getSignature(actual_mci_ptr->methodID);

#ifdef DEBUG_RMI_PORT
  log_file << "(invoke method) requested_method: " << requested_method << std::endl;
#endif

  if (requested_method == std::string(""))
  {
    std::cerr << "Error: Inavlid method ID" << std::endl;
#ifdef DEBUG_RMI_PORT
    log_file << "Error: Inavlid method ID" << std::endl;
#endif
  }
  else
  {
    Class::MethodList embedded_obj_method_list;
      
    // get method list of embedded object
    Class::MethodList methodList = m_objectPointer->getClass().getMethods();
    for (Class::MethodList::const_iterator method_iter = methodList.begin();
         method_iter != methodList.end();
         ++method_iter)
    {
      embedded_obj_method_list.push_back(*method_iter);
    }

    //
    // CAUTION: this is only experimental code !!!
    //
    bool embedded_obj_has_super = false;
    std::string embedded_obj_super_name;
    if (m_objectPointer->getClass().hasSuper())
    {
      embedded_obj_has_super = true;
      const Class* super_class = m_objectPointer->getClass().super();
      embedded_obj_super_name = super_class->getFullName();
      Class::MethodList methodList = super_class->getMethods();
      for (Class::MethodList::const_iterator iter = methodList.begin();
           iter != methodList.end();
           ++iter)
      {
        embedded_obj_method_list.push_back(*iter);
      }
    }


    // get method list of embedded object
    //Class::MethodList methodList = m_objectPointer->getClass().getMethods();
    for (Class::MethodList::const_iterator method_iter = embedded_obj_method_list.begin();
         method_iter != embedded_obj_method_list.end();
         ++method_iter)
    {
      const Method *tmp_method = *method_iter;

      std::string methodSignature = tmp_method->getSignature();
      int argsCount = tmp_method->getArgsCount();
      bool hasReturnType = tmp_method->hasReturnType();

	

      // search for the requested method
      if((removeClassName(methodSignature, 
                          m_objectPointer->getClass().getFullName()) == requested_method) ||
         (removeClassName(methodSignature, embedded_obj_super_name) == requested_method))
      {

        /*
         * this is what we would like to do:
         * e.g. calling method write(Image* img) on 
         * the object pointed by m_objectPointer
         *-------------------------------------------------
         * Image* img_tmp = new Imgage(actual_mci_ptr->in_params);
         * m_objectPointer->write(img_tmp);
         * delete img_tmp;
         */

#ifdef DEBUG_RMI_PORT
        log_file << "calling method: " 
                 << removeClassName(methodSignature, m_objectPointer->getClass().getFullName()) 
                 << std::endl;
        log_file << "argsCount: " << argsCount << std::endl
                 << "hasReturnType: " << hasReturnType << std::endl;
#endif

        actual_mci_ptr->in_params.prepare_rebuild();

#ifdef DEBUG_RMI_PORT
        log_file << "actual_mci_ptr->in_params: "; 
        log_file << actual_mci_ptr->in_params;
        log_file << std::endl;
#endif
	  
        std::vector<std::string> parameter_type_list =
          extractTypesFromParameterList(requested_method);
	  
#ifdef DEBUG_RMI_PORT
        log_file << "analyse parameters of requested_method: " 
                 << requested_method << std::endl;
        for(unsigned int p=0; p<parameter_type_list.size(); p++)
          log_file << p << ": " << parameter_type_list[p] << std::endl;
        log_file << "build temporary parameters for method call" << std::endl;
#endif

        osss_serialisable_object* ser_obj[20];

        for(int argCounter=0; argCounter<argsCount; argCounter++)
        {
          const Class* img_cls = Class::forName(parameter_type_list[argCounter]);
          const Class::ConstructorList& constructors = img_cls->getConstructors();
          for (Class::ConstructorList::const_iterator iter = constructors.begin();
               iter != constructors.end();
               ++iter)
          {
            const ConstructorMethod& cm = **iter;
            // we will look for <classname>(osss_serialisable_archive*) constructor
            if ((cm.getArgsCount() == 1) && 
                (*cm.getArgsTypes()[0] == typeid(osss_serialisable_archive*)))
            {
#ifdef DEBUG_RMI_PORT
              log_file << "found constructor with signature: " << cm.getArgs() << std::endl;
#endif
  
              // TODO: check for exceptions
              cm.invoke(ser_obj[argCounter], &(actual_mci_ptr->in_params));
		      
#ifdef DEBUG_RMI_PORT
              log_file << "construction of temporary done" << std::endl;
#endif
            }
          }
        }

#ifdef DEBUG_RMI_PORT
        log_file << "performing method call" << std::endl;
#endif
	      
        if (!hasReturnType) // method has no return type
        {
          try
          {
            // depending on the number of method arguments
            // call the right invokeVoid method
            switch (argsCount)
            {
		  
              _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 0, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 1, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 2, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 3, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 4, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 5, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 6, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 7, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 8, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 9, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 10, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 11, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 12, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 13, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 14, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 15, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 16, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 17, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 18, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 19, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE_VOID(tmp_method, m_objectPointer, 20, ser_obj)
		    		    
	      default :
                std::cerr << "Error: number of method arguments exceeds limit" << std::endl;   
            }

            // do some cleanup
            for(int argCounter=0; argCounter<argsCount; argCounter++)
            {
              if (ser_obj[argCounter] != NULL)
              {
                delete ser_obj[argCounter];
                ser_obj[argCounter] = 0;

#ifdef DEBUG_RMI_PORT
                log_file << "deleting temporary done" << std::endl;
#endif
              }
            }
          }
          catch (std::runtime_error const & err)
          {
            std::cerr << err.what() << std::endl;
            OSSS_ABORT();
          }
#ifdef DEBUG_RMI_PORT
          log_file << "invocation of void method done!" << std::endl;
#endif

          actual_mci_ptr->call_done = true;
          if (actual_mci_ptr->return_params != NULL)
            delete actual_mci_ptr->return_params;
          actual_mci_ptr->return_params = new osss_serialisable_archive();
          break; // breaks for-loop
        }
        else // method has return type
        {
          osss_serialisable_object *return_value;
		
          try
          {
            // depending on the number of method arguments
            // call the right invokeVoid method
            switch (argsCount)
            {

              _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 0, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 1, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 2, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 3, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 4, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 5, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 6, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 7, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 8, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 9, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 10, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 11, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 12, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 13, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 14, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 15, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 16, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 17, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 18, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 19, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, m_objectPointer, 20, ser_obj)
	          
	      default :
                std::cerr << "Error: number of method arguments exceeds limit" << std::endl;
            }

            // do some cleanup
            for(int argCounter=0; argCounter<argsCount; argCounter++)
            {
              if (ser_obj[argCounter] != NULL)
              {
                delete ser_obj[argCounter];
                ser_obj[argCounter] = 0;

#ifdef DEBUG_RMI_PORT
                log_file << "deleting temporary done" << std::endl;
#endif
              }
            }
          }
          catch (std::runtime_error const & err)
          {
            std::cerr << err.what() << std::endl;
            OSSS_ABORT();
          }
#ifdef DEBUG_RMI_PORT
          log_file << "invocation of non-void method done!" << std::endl;
#endif

          return_value->serialise_obj();

#ifdef DEBUG_RMI_PORT
          log_file << "return value is (serialised):" << std::endl;
          log_file << *return_value;
          log_file << std::endl;
#endif

          actual_mci_ptr->call_done = true;
          if (actual_mci_ptr->return_params != NULL)
            delete actual_mci_ptr->return_params;
          actual_mci_ptr->return_params = new osss_serialisable_archive(*return_value);
          break; // breaks for-loop
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
// osss_object_socket_port
//------------------------------------------------------------------------------

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
osss_object_socket_port(osss_object_socket<ObjectType, 
                                           ChannelInterfaceType>* obj_socket) : 
  sc_core::sc_module( ),
//    sc_core::sc_module_name(
//                        sc_core::sc_gen_unique_name("osss_object_socket_port")) ),
  base_type()
{
  m_obj_socket = obj_socket;
  base_type::m_objectPointer = obj_socket->operator->();
  declare_processes();
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
osss_object_socket_port(osss_object_socket<ObjectType, 
                                           ChannelInterfaceType>* obj_socket,
			/*  osss_export<
			    osss_rmi_if<
                            osss_rmi_channel_server_if> >& export_ */
                        osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port
 ) : 
  sc_core::sc_module( ),
  //sc_core::sc_module_name(
//                        sc_core::sc_gen_unique_name("osss_object_socket_port")) ),
  base_type( port /*export_*/ )
{
  m_obj_socket = obj_socket;
  base_type::m_objectPointer = obj_socket->operator->();
  declare_processes();
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
osss_object_socket_port(sc_core::sc_module_name name,
                        osss_object_socket<ObjectType, 
			                   ChannelInterfaceType>* obj_socket) : 
  sc_core::sc_module( name ),
  base_type()
{
  m_obj_socket = obj_socket;
  base_type::m_objectPointer = obj_socket->operator->();
  declare_processes();
}

template<class ObjectType, class ChannelInterfaceType>
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
osss_object_socket_port(sc_core::sc_module_name name,
			  osss_object_socket<ObjectType, 
			                     ChannelInterfaceType>* obj_socket,
                        /*osss_export<
			    osss_rmi_if<
                            osss_rmi_channel_server_if> >& export_ */
                        osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port
) : 
  sc_core::sc_module( name ),
  base_type( port /*export_*/ )
{
  m_obj_socket = obj_socket;
  base_type::m_objectPointer = obj_socket->operator->();
  declare_processes();
}

template<class ObjectType, class ChannelInterfaceType>
sc_core::sc_object*
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
bound_channel() const
{
  return dynamic_cast<sc_core::sc_object*>(base_type::m_rmi_channelPointer);
}

template<class ObjectType, class ChannelInterfaceType>
sc_core::sc_object*
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
bound_object_socket() const
{
  return m_obj_socket;
}

template<class ObjectType, class ChannelInterfaceType>
bool
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
is_hierarchical() const { return false; }

template<class ObjectType, class ChannelInterfaceType>
bool
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
is_accessor() const { return true; }

//TODO: is the m_objectPointer really needed 
//      (or can we change it to m_shared_objectPointer ???)
template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
updateObjectRegistry()
{
  base_type::m_objectPointer = &(m_obj_socket->plugged_object());
}

template<class ObjectType, class ChannelInterfaceType>
ObjectType *
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
operator->() const
{
  return base_type::m_objectPointer;
}

//TODO: it this process really needed (see above)
template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
updateObjectRegistry_process()
{
  // the updateObjectRegistry method needs to be called 
  // asynchronous to the clock
  // this is a very ugly hack that only works for reset sensitivities:
  // OSSS_POS & OSSS_NEG AND iff the reset signal is deasserted 
  // on the falling edge of the clock
  updateObjectRegistry();
}

template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
protocol_process()
{
  base_type::protocol_process();
}

template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
behaviour_process()
{
  std::map<const OSSS_ClientID_t, osss_method_call_item>::iterator map_iter =
    base_type::m_reception_record.find(base_type::m_ready_queue.front());
  base_type::m_ready_queue.pop();
    
  base_type::invoke_method(&((*map_iter).second));
}

template<class ObjectType, class ChannelInterfaceType>
void
osss_object_socket_port<ObjectType, ChannelInterfaceType>::
declare_processes()
{
  // declares the protocol process
  osssi::osss_cthread_handle protocol_process_handle =
    osssi::osss_create_cthread_process(
      sc_core::sc_gen_unique_name("protocol_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, protocol_process ),
      this );

  sensitive.operator() ( protocol_process_handle, clock_port.pos() );
  reset_signal_is(reset_port, true);

  // declares the behaviour process
  osssi::osss_method_handle behaviour_process_handle =
    osssi::osss_create_method_process(
      sc_core::sc_gen_unique_name("behaviour_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, behaviour_process ),
      this );

  sensitive << behaviour_process_handle;
  sensitive << base_type::m_ready_queue_changed;
  dont_initialize();

  // declares the updateObjectRegistry process 
  osssi::osss_method_handle updateObjectRegistry_process_handle =
    osssi::osss_create_method_process(
      sc_core::sc_gen_unique_name("updateObjectRegistry_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, updateObjectRegistry_process ),
      this );

  sensitive << updateObjectRegistry_process_handle;
  sensitive << reset_port;
  dont_initialize();
}

//------------------------------------------------------------------------------
// osss_object_socket_port (specialisation for shared objects)
//------------------------------------------------------------------------------

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
osss_object_socket_port( osss_object_socket< 
                           osss_shared< ObjectType, SchedulerType >,
                             ChannelInterfaceType>* obj_socket ) : 
  sc_core::sc_module( ),
  //sc_core::sc_module_name(
//                        sc_core::sc_gen_unique_name("osss_object_socket_port")) ),
  base_type()
{
  m_obj_socket = obj_socket;
  m_shared_objectPointer = &(obj_socket->plugged_object());
  base_type::m_objectPointer = m_shared_objectPointer->operator->();
  declare_processes();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
osss_object_socket_port( osss_object_socket< 
                           osss_shared< ObjectType, SchedulerType >,
                             ChannelInterfaceType>* obj_socket,
                         /* osss_export<
                           osss_rmi_if<
                           osss_rmi_channel_server_if> >& export_*/
                         osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port
) : 
  sc_core::sc_module( ),
//sc_module_name(
  //                      sc_core::sc_gen_unique_name("osss_object_socket_port")) ),
  base_type( port /*export_*/ )
{
  m_obj_socket = obj_socket;
  m_shared_objectPointer = &(obj_socket->plugged_object());
  base_type::m_objectPointer = m_shared_objectPointer->operator->();
  declare_processes();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
osss_object_socket_port( sc_core::sc_module_name name, 
                           osss_object_socket< 
                             osss_shared< ObjectType, SchedulerType >,
                               ChannelInterfaceType>* obj_socket ) : 
  sc_core::sc_module( name ),
  base_type()
{
  m_obj_socket = obj_socket;
  m_shared_objectPointer = &(obj_socket->plugged_object());
  base_type::m_objectPointer = m_shared_objectPointer->operator->();
  declare_processes();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
osss_object_socket_port( sc_core::sc_module_name name, 
                           osss_object_socket< 
                             osss_shared< ObjectType, SchedulerType >,
                               ChannelInterfaceType>* obj_socket, 
                         /*osss_export<
                             osss_rmi_if<
                             osss_rmi_channel_server_if> >& export_*/
                         osss_port<osss_channel_if<osss_rmi_channel_server_if> >& port
) : 
  sc_core::sc_module( name ),
  base_type( port /*export_*/ )
{
  m_obj_socket = obj_socket;
  m_shared_objectPointer = &(obj_socket->plugged_object());
  base_type::m_objectPointer = m_shared_objectPointer->operator->();
  declare_processes();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
sc_core::sc_object*
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
bound_channel() const
{
  return dynamic_cast<sc_core::sc_object*>(base_type::m_rmi_channelPointer);
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
sc_core::sc_object*
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
bound_object_socket() const
{
  return m_obj_socket;
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
bool
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
is_hierarchical() const { return false; }

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
bool
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
is_accessor() const { return true; }

//TODO: is this really needed (see above)
template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
updateObjectRegistry()
{
  base_type::m_objectPointer = m_obj_socket->plugged_object().operator->();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
osss_shared<ObjectType, SchedulerType>*
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
operator->() const
{
  return m_shared_objectPointer;
}

//TODO: is this really needed (see above)
template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
updateObjectRegistry_process()
{
  // the updateObjectRegistry method needs to be called 
  // asynchronous to the clock
  // this is a very ugly hack that only works for reset sensitivities:
  // OSSS_POS & OSSS_NEG AND iff the reset signal is deasserted 
  // on the falling edge of the clock
  updateObjectRegistry();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
protocol_process()
{
  base_type::protocol_process();
}

//  #if defined(OSSS_SIMPLE_GUARDS)
template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
behaviour_process()
{
  OSSS_MethodID_Record_t::iterator method_record_iter;
  std::vector<osss_method_call_item*> pending_list;
  sc_core::wait();
 
  while(true)
  {   
    pending_list.clear();
    // look through list of receptions for method_items waiting
    // their guard to become true
    for(method_record_iter = base_type::m_reception_record.begin();
        method_record_iter != base_type::m_reception_record.end(); 
        method_record_iter++)
    {
      if (!((*method_record_iter).second.access_granted) &&
          !((*method_record_iter).second.is_executing) &&
          !((*method_record_iter).second.call_done))
      {
        pending_list.push_back(&((*method_record_iter).second));
      }
    }

    /*
      #ifdef DEBUG_RMI_PORT
      std::cout << std::endl;
      std::cout << "pending_list.size(): " << pending_list.size() << " of " << _CLIENT_ID_ << std::endl;
      for(unsigned int j=0; j<pending_list.size(); j++)
      std::cout << j << ": " << *(pending_list[j]) << std::endl;
      std::cout << std::endl;
      #endif
    */
      
    if (pending_list.empty())
    {
      sc_core::wait();
      continue;
    }

    bool performed_method_call = false;
    for(unsigned int i=0; i<pending_list.size(); i++)
    {
      if (m_shared_objectPointer->getArbiter()->
          getGrant(m_shared_objectPointer->
                   lookupPortID(pending_list[i]->clientID)))
      {
        //----------------------------------------------------
        // keep on requesting for access to the shared object
        // for other "virtual" clients
        //----------------------------------------------------
        for(unsigned int j=0; j<pending_list.size(); j++)
        {
          if (j != i)
          {
            m_shared_objectPointer->getArbiter()->
              setRequest(m_shared_objectPointer->
                         lookupPortID(pending_list[j]->clientID), true);
            m_shared_objectPointer->getArbiter()->
              setGuard(m_shared_objectPointer->
                       lookupPortID(pending_list[j]->clientID), 
                       invoke_guard(*(pending_list[j])));
          }
        }
	 
        //------------------------------------------------
        // method invocation of granted "virtual" client
        //------------------------------------------------
        m_shared_objectPointer->getArbiter()->
          setRequest(m_shared_objectPointer->
                     lookupPortID(pending_list[i]->clientID), false);
	  
        // notify the protocol_process that access has been granted
        pending_list[i]->access_granted = true;

        // wait until protocol_process has received the input parameters
        while(!pending_list[i]->in_params_complete)
        {
#ifdef DEBUG_RMI_PORT
          std::cout << "wait for in_params_complete: " 
                    << sc_core::sc_time_stamp() << std::endl
                    << *(pending_list[i]) << std::endl;
#endif

          sc_core::wait();
        }

        // guard is true && the input parameters are complete 
        //   => perform method call
        pending_list[i]->is_executing = true;
        base_type::invoke_method(pending_list[i]);

        m_shared_objectPointer->getArbiter()->
          setGrant(m_shared_objectPointer->
                   lookupPortID(pending_list[i]->clientID), false);

        performed_method_call = true;

        m_shared_objectPointer->getArbiter()->updateGrant();
        sc_core::wait();
        //wait();
        break;
      }
    }

    // no method call has been performed
    //  -> keep on requesting for access to the shared object
    if (!performed_method_call)
    {
      for(unsigned int i=0; i<pending_list.size(); i++)
      {
        m_shared_objectPointer->getArbiter()->
          setRequest(m_shared_objectPointer->
                     lookupPortID(pending_list[i]->clientID), true);
        m_shared_objectPointer->getArbiter()->
          setGuard(m_shared_objectPointer->
                   lookupPortID(pending_list[i]->clientID), 
                   invoke_guard(*(pending_list[i])));
      }
      m_shared_objectPointer->getArbiter()->updateGrant();
      sc_core::wait();
    }
  }
}
//  #else
#ifdef FOO
template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
behaviour_process()
{
   
  OSSS_MethodID_Record_t::iterator method_record_iter;
  std::vector<osss_method_call_item*> complete_list;
  sc_core::wait();
 
  while(true)
  {   
    complete_list.clear();
    // look through list of receptions for complete method_items
    for(method_record_iter = base_type::m_reception_record.begin();
        method_record_iter != base_type::m_reception_record.end(); 
        method_record_iter++)
    {
      if (((*method_record_iter).second.in_params_complete) && 
          !((*method_record_iter).second.call_done))
      {
        complete_list.push_back(&((*method_record_iter).second));
      }
    }

    /*
      std::cout << std::endl;
      std::cout << "complete_list.size(): " << complete_list.size() << std::endl;
      std::cout << std::endl;
    */

    if (complete_list.empty())
    {
      sc_core::wait();
      continue;
    }
    
    // check if arbiter of shared object grants access
    // and perform method call if access is granted
    bool performed_method_call = false;
    for(unsigned int i=0; i<complete_list.size(); i++)
    {
      if (m_shared_objectPointer->getArbiter()->
          getGrant(m_shared_objectPointer->
                   lookupPortID(complete_list[i]->clientID)))
      {
        //----------------------------------------------------
        // keep on requesting for access to the shared object
        // for other "virtual" clients
        //----------------------------------------------------
        for(unsigned int j=0; j<complete_list.size(); j++)
        {
          if (j != i)
          {
            m_shared_objectPointer->getArbiter()->
              setRequest(m_shared_objectPointer->
                         lookupPortID(complete_list[j]->clientID), true);
            m_shared_objectPointer->getArbiter()->
              setGuard(m_shared_objectPointer->
                       lookupPortID(complete_list[j]->clientID), 
                       invoke_guard(*(complete_list[j])));
          }
        }
	 
        //------------------------------------------------
        // method invocation of granted "virtual" client
        //------------------------------------------------
        m_shared_objectPointer->getArbiter()->
          setRequest(m_shared_objectPointer->
                     lookupPortID(complete_list[i]->clientID), false);
	  
        base_type::invoke_method(complete_list[i]);

        m_shared_objectPointer->getArbiter()->
          setGrant(m_shared_objectPointer->
                   lookupPortID(complete_list[i]->clientID), false);

        performed_method_call = true;

        m_shared_objectPointer->getArbiter()->updateGrant();
        sc_core::wait();
        //wait();
        break;
      }
    }

    // no method call has been performed
    //  -> keep on requesting for access to the shared object
    if (!performed_method_call)
    {
      for(unsigned int i=0; i<complete_list.size(); i++)
      {
        m_shared_objectPointer->getArbiter()->
          setRequest(m_shared_objectPointer->
                     lookupPortID(complete_list[i]->clientID), true);
        m_shared_objectPointer->getArbiter()->
          setGuard(m_shared_objectPointer->
                   lookupPortID(complete_list[i]->clientID), 
                   invoke_guard(*(complete_list[i])));
      }
      m_shared_objectPointer->getArbiter()->updateGrant();
      sc_core::wait();
    }
  }
}
#endif

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
bool
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
invoke_guard(osss_method_call_item actual_mci)
{
  using namespace agm::reflection;
  using osss::osssi::removeClassName;
  using osss::osssi::transformToGuard;
  using osss::osssi::extractTypesFromParameterList;

  std::string requested_method = 
    base_type::channel_port::getObjectRegistry()->
//    getMethodRegistry(base_type::m_objectPointer)->
    getMethodRegistry(m_shared_objectPointer)->
    getSignature(actual_mci.methodID);

  std::string guard_methodName;
  bool return_value = false;
    
#ifdef DEBUG_RMI_PORT
  base_type::log_file << "(invoke guard) requested_method: " 
                      << requested_method << std::endl;
#endif

  if (requested_method == std::string(""))
  {
    std::cerr << "Error: Inavlid method ID" << std::endl;
  }
  else
  {
#ifdef DEBUG_RMI_PORT
    base_type::log_file << "looking for requested method" << std::endl;
#endif

    // get method list of embedded object
//    Class::MethodList methodList = base_type::m_objectPointer->
    Class::MethodList methodList = m_shared_objectPointer->operator->()->
      getClass().getMethods();


    for (Class::MethodList::const_iterator method_iter = methodList.begin();
         method_iter != methodList.end();
         ++method_iter)
    {
      const Method *tmp_method = *method_iter;

      std::string methodSignature = tmp_method->getSignature();

      // search for the requested method
      if(removeClassName(methodSignature, 
                         //base_type::m_objectPointer->
                         m_shared_objectPointer->operator->()->
                         getClass().getFullName()) == requested_method)
      {
        /*
	  std::string methodName = tmp_method->getName();
	  std::string old_prefix = "_stub_";
	  std::string new_prefix = "bool _guarded_";
	  methodName.erase(0, old_prefix.length());
	  guard_methodName = new_prefix + methodName + "()";
        */

#ifdef DEBUG_RMI_PORT
        base_type::log_file << "requested method foud..." << std::endl
                            << "transforming requested method to guard" << std::endl;
#endif

        guard_methodName = transformToGuard(methodSignature, 
                                            //base_type::m_objectPointer->
                                            m_shared_objectPointer->operator->()->
                                            getClass().getFullName(),
                                            tmp_method->getName());

#ifdef DEBUG_RMI_PORT
        base_type::log_file << "guard method: " << guard_methodName << std::endl;
#endif
      }
    }

    if (guard_methodName != std::string(""))
    {
      // get method list of embedded object
//      Class::MethodList methodList = base_type::m_objectPointer->
    Class::MethodList methodList = m_shared_objectPointer->operator->()->
        getClass().getMethods();
      for (Class::MethodList::const_iterator method_iter = methodList.begin();
           method_iter != methodList.end();
           ++method_iter)
      {
        const Method *tmp_method = *method_iter;

        std::string methodSignature = tmp_method->getSignature();

        // search for the requested method
        if(removeClassName(methodSignature, 
                           //base_type::m_objectPointer->
                           m_shared_objectPointer->operator->()->
                           getClass().getFullName()) == guard_methodName)
        {

          int argsCount = tmp_method->getArgsCount();
	    
          std::vector<std::string> parameter_type_list = 
            extractTypesFromParameterList(guard_methodName);
	  
#ifdef DEBUG_RMI_PORT
          base_type::log_file << "analyse parameters of guard method: " 
                              << guard_methodName << std::endl;
          for(unsigned int p=0; p<parameter_type_list.size(); p++)
            base_type::log_file << p << ": " << parameter_type_list[p] << std::endl;
          base_type::log_file << "build temporary parameters for method call" << std::endl;
#endif

          osss_serialisable_object* ser_obj[20];

          for(int argCounter=0; argCounter<argsCount; argCounter++)
          {
            const Class* img_cls = Class::forName(parameter_type_list[argCounter]);
            const Class::ConstructorList& constructors = img_cls->getConstructors();
            for (Class::ConstructorList::const_iterator iter = constructors.begin();
                 iter != constructors.end();
                 ++iter)
            {
              const ConstructorMethod& cm = **iter;
              // we will look for <classname>(osss_serialisable_archive*) constructor
              if ((cm.getArgsCount() == 1) && 
                  (*cm.getArgsTypes()[0] == 
                   typeid(osss_serialisable_archive*)))
              {
#ifdef DEBUG_RMI_PORT
                base_type::log_file << "found constructor with signature: " 
                                    << cm.getArgs() << std::endl;
#endif
  
                // todo: check for exceptions
                cm.invoke(ser_obj[argCounter], &actual_mci.in_params);
		      
#ifdef DEBUG_RMI_PORT
                base_type::log_file << "construction of temporary done" << std::endl;
#endif
              }
            }
          }

#ifdef DEBUG_RMI_PORT
          base_type::log_file << "performing guard method call" << std::endl;
#endif

          try
          {
            // depending on the number of method arguments
            // call the right invokeVoid method
            switch (argsCount)
            {

              _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 0, ser_obj)
              _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 1, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 2, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 3, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 4, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 5, ser_obj)
              _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 6, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 7, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 8, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 9, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 10, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 11, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 12, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 13, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 14, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 15, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 16, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 17, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 18, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 19, ser_obj)
	      _OSSS_OBJECT_SOCKET_CASE(tmp_method, return_value, base_type::m_objectPointer, 20, ser_obj)
		    
	      default :
                std::cerr << "Error: number of method arguments exceeds limit" 
                          << std::endl;
            }

            // do some cleanup
            for(int argCounter=0; argCounter<argsCount; argCounter++)
            {
              if (ser_obj[argCounter] != NULL)
              {
                delete ser_obj[argCounter];
                ser_obj[argCounter] = 0;

#ifdef DEBUG_RMI_PORT
                base_type::log_file << "deleting temporary done" << std::endl;
#endif
              }
            }
          }
          catch (std::runtime_error const & err)
          {
            std::cerr << err.what() << std::endl;
            OSSS_ABORT();
          }
	   	    
          /*
	    #ifdef DEBUG_RMI_PORT
	    base_type::log_file << "calling guard for method: ";
	    base_type::log_file << methodSignature << std::endl;
	    #endif
	    
	    tmp_method->invoke(return_value, base_type::m_objectPointer);
          */

#ifdef DEBUG_RMI_PORT
          base_type::log_file << "calling guard done" << std::endl;
#endif
        }
      }
    }
    else
    {
      std::cerr << "Error: No guard found for method " 
                << requested_method << std::endl;
    }
  }
#ifdef DEBUG_RMI_PORT
  base_type::log_file << "~~~~> guard evaluated to: " << return_value << std::endl;
#endif
  return return_value;
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
declare_processes()
{
  // declares the protocol process
  osssi::osss_cthread_handle protocol_process_handle =
    osssi::osss_create_cthread_process(
      sc_core::sc_gen_unique_name("protocol_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, protocol_process ),
      this );

  sensitive.operator() ( protocol_process_handle, clock_port.pos() );
  reset_signal_is(reset_port, true);

  // declares the behaviour process
  osssi::osss_cthread_handle behaviour_process_handle =
    osssi::osss_create_cthread_process(
      sc_core::sc_gen_unique_name("behaviour_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, behaviour_process ),
      this );

  sensitive.operator() ( behaviour_process_handle, clock_port.pos() );
  reset_signal_is(reset_port, true);

  // declares the updateObjectRegistry process 
  osssi::osss_method_handle updateObjectRegistry_process_handle =
    osssi::osss_create_method_process(
      sc_core::sc_gen_unique_name("updateObjectRegistry_process"),
      SC_MAKE_FUNC_PTR( SC_CURRENT_USER_MODULE, updateObjectRegistry_process ),
      this );

  sensitive << updateObjectRegistry_process_handle;
  sensitive << reset_port;
  dont_initialize();
}

template<class ObjectType, class SchedulerType, class ChannelInterfaceType>
void
osss_object_socket_port<osss_shared<ObjectType, SchedulerType>, ChannelInterfaceType>::
register_at_rmi_channel()
{
  base_type::register_at_rmi_channel();

  osss_rmi::server_info s_info;
  s_info.object_id = 
    base_type::channel_port::getObjectRegistry()->
    getObjectID(base_type::m_objectPointer);
  s_info.object_name = m_shared_objectPointer->name();
  s_info.num_clients = 0; //we do not know the number of clients for that 
  //RMI-Channel yet !
  s_info.server_pointer = m_shared_objectPointer;
    
  if (base_type::m_rmi_channelPointer != NULL)
    base_type::m_rmi_channelPointer->add_server_info(s_info);
  else
  {
    std::cerr << "Error in osss_object_socket: Port not bound!" << std::endl;
    OSSS_ABORT();
  }
}

//------------------------------------------------------------------------------

#undef _REPEAT_LIST0
#undef _REPEAT_LIST1
#undef _REPEAT_LIST2
#undef _REPEAT_LIST3
#undef _REPEAT_LIST4
#undef _REPEAT_LIST5
#undef _REPEAT_LIST6
#undef _REPEAT_LIST7
#undef _REPEAT_LIST8
#undef _REPEAT_LIST9
#undef _REPEAT_LIST10
#undef _REPEAT_LIST11
#undef _REPEAT_LIST12
#undef _REPEAT_LIST13
#undef _REPEAT_LIST14
#undef _REPEAT_LIST15
#undef _REPEAT_LIST16
#undef _REPEAT_LIST17
#undef _REPEAT_LIST18
#undef _REPEAT_LIST19
#undef _REPEAT_LIST20
#undef _REPEAT_LIST

#undef _OSSS_OBJECT_SOCKET_CASE_VOID
#undef _OSSS_OBJECT_SOCKET_CASE
#undef LOG

//} // namespace osssi

} //namespace osss

// $Id: osss_object_socket_port.tpp 2991 2009-01-23 14:49:29Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
