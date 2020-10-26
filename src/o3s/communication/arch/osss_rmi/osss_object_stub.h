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

#ifndef OSSS_OBJECT_STUB_HH
#define OSSS_OBJECT_STUB_HH

#include "o3s/datatypes/osss_serialisable_archive.h"
#include "o3s/communication/arch/osss_rmi/osss_port_to_rmi.h"

//#define DEBUG_OSSS_OBJECT_SOCKET

// TODO: implement handling of overloaded methods
#ifdef DEBUG_OSSS_OBJECT_SOCKET

#define OSSS_PREPARE_METHOD_CALL \
   ::osss::OSSS_ObjectID_t _object_id = _m_access_port->getObjectID(); \
   std::cout << "objectID: " << _object_id << std::endl; \
   std::string _clearedSignature; \
   agm::reflection::Class::MethodList _methodList = \
     this->getClass().getAllMethods(_method_name.c_str(), false); \
   agm::reflection::Class::MethodList::const_iterator _method_iter = _methodList.begin(); \
   const agm::reflection::Method *_tmp_method = *_method_iter; \
   if (_methodList.size() == 1) \
   { \
     _clearedSignature = ::osss::osssi::removeClassName(_tmp_method->getSignature(), \
                                                        this->getClass().getFullName()); \
     std::cout << _clearedSignature << std::endl; \
   } \
   else \
   { \
     std::cerr << "overloaded method: not implemented!" << std::endl; \
   } \
   ::osss::OSSS_MethodID_t _method_id = _m_access_port-> \
                              getMethodID( _clearedSignature ); \
                              /* trimSignature(__PRETTY_FUNCTION__)); */ \
   std::cout << "methodID: " << _method_id << std::endl; \
   std::cout << "=========================================" \
             << "==================================" << std::endl; \
   std::cout << __PRETTY_FUNCTION__ << " at: " \
             << sc_core::sc_time_stamp() << std::endl; \
   std::cout << "=========================================" \
             << "==================================" << std::endl; \
   ::osss::osss_serialisable_archive _argument_archive; \
   _argument_archive.clear();

#else

#define OSSS_PREPARE_METHOD_CALL \
   ::osss::OSSS_ObjectID_t _object_id = _m_access_port->getObjectID(); \
   std::string _clearedSignature; \
   agm::reflection::Class::MethodList _methodList = \
     this->getClass().getAllMethods(_method_name.c_str(), false); \
   agm::reflection::Class::MethodList::const_iterator _method_iter = _methodList.begin(); \
   const agm::reflection::Method *_tmp_method = *_method_iter; \
   if (_methodList.size() == 1) \
   { \
     _clearedSignature = ::osss::osssi::removeClassName(_tmp_method->getSignature(), \
                                                        this->getClass().getFullName()); \
     /* std::cout << _clearedSignature << std::endl; */ \
   } \
   else \
   { \
     std::cerr << "overloaded method: not implemented!" << std::endl; \
   } \
   ::osss::OSSS_MethodID_t _method_id = _m_access_port-> \
                              getMethodID( _clearedSignature ); \
                              /* trimSignature(__PRETTY_FUNCTION__)); */ \
   ::osss::osss_serialisable_archive _argument_archive; \
   _argument_archive.clear();

#endif /* DEBUG_OSSS_OBJECT_SOCKET */

#define OSSS_SERIALISE_ARGUMENT( _arg_ ) \
  _argument_archive.add( *_arg_ ); 

/** 
 * Calls a procedure.
 * Has no return value in contrast to a function call. 
 * 
 */
#define OSSS_PROCEDURE_CALL \
  _m_access_port->getTransactor()->call_procedure(_CLIENT_ID_, \
                                                  _object_id,  \
                                                  _method_id,  \
                                                  _argument_archive);

/** 
 * Calls a function.
 * 
 * @param _return_type_ The return-type of the called function.
 * 
 * @return The returned value.
 */
#define OSSS_FUNCTION_CALL( _return_type_ ) \
  osss::osssi::osss_serialisable_wrapper< _return_type_ >* _return_value = \
     new osss::osssi::osss_serialisable_wrapper< _return_type_ >(); \
  _return_value->serialise_obj(); \
  /* TODO: is a size of byte the right granularity here (p2p and bus)? */ \
  unsigned int _return_value_size = \
    (unsigned int)ceil((double)(_return_value->size()/8)); \
  /* std::cout << "OSSS_FUNCTION_CALL: _return_value_size = " << _return_value_size << std::endl; */ \
  osss::osss_serialisable_archive _return_archive = \
     _m_access_port->getTransactor()->call_function(_CLIENT_ID_, \
                                                    _object_id, \
                                                    _method_id, \
                                                    _argument_archive, \
                                                    _return_value_size); \
  _return_archive.prepare_rebuild(); \
  _return_archive.rebuild(*_return_value); \
  return _return_value;

/** 
 * Constructor wrapper of object stubs.
 * This is the only valid constructor for an osss rmi stub.
 *
 * @param ... The type of the interface this stub is used for.
 * 
 */
#define OSSS_OBJECT_STUB_CTOR( ... ) \
 public: \
    typedef osss_rmi_if< __VA_ARGS__ > this_type; \
    CLASS( osss_rmi_if, NullClass ); \
    /* default constructor */ \
    osss_rmi_if() : _m_access_port(0) {} \
    explicit osss_rmi_if(osss::osss_port<this_type> * _port) \
    { \
      _m_access_port = _port; \
    } \
    void reset() { _m_access_port->getTransactor()->reset(); } \
  protected: \
    osss::osss_port<this_type>* _m_access_port; \
  public:

#endif
// $Id: osss_object_stub.h 2762 2008-11-28 09:54:10Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
