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

#ifndef OSSS_HW_MACROS_BLUE_INCLUDED
#define OSSS_HW_MACROS_BLUE_INCLUDED

#ifndef SYNTHESIS
 #define _METHODCALL_(returnvalue, port, methodcall) \
            void* buffer = ::osss::osssi::functionCreateHelper(returnvalue, port->methodcall);

 #define _RETURN_(returnvalue) \
            returnvalue = ::osss::osssi::functionDestroyHelper(returnvalue, buffer);
#else
 #define _METHODCALL_(returnvalue, port, methodcall) returnvalue = port->methodcall
 #define _RETURN_(returnvalue)
#endif 

//------------------------------------------------------------------------------

namespace osss {

  namespace osssi {

    template <class T>
    struct tempType
    {
      T tempVariable;
    };

    //--------------------------------------------------------------------------

    // general
    template<class T1, class T2>
    inline
    void* functionCreateHelper(const T1 &/*value*/, T2 intermediate)
    {
      tempType<T1> *ok;
      ok = new tempType<T1>;
      ok->tempVariable = intermediate;
      return ok;
    }

    template<class T>
    inline
    T functionDestroyHelper(const T &/*value*/, void* buffer)
    {
      tempType<T> *rescued_ok = (reinterpret_cast<tempType<T>* >(buffer));
      T returnValue =  rescued_ok->tempVariable;
      delete rescued_ok; 
      return returnValue;
    }

    //--------------------------------------------------------------------------

    // sc_inout
    template<class T1, class T2>
    inline
    void* functionCreateHelper(const sc_core::sc_inout<T1> &port, T2 intermediate)
    {
      tempType<T1> *ok;
      ok = new tempType<T1>;
      ok->tempVariable = intermediate;
      return ok;
    }

    template<class T>
    inline
    T functionDestroyHelper(const sc_core::sc_inout<T> &port, void* buffer)
    {
      tempType<T> *rescued_ok = (reinterpret_cast<tempType<T>* >(buffer));
      T returnValue =  rescued_ok->tempVariable;
      delete rescued_ok; 
      return returnValue;
    }

    //--------------------------------------------------------------------------

    // sc_inout<bool>
    template<>
    inline
    void* functionCreateHelper(const sc_core::sc_inout<bool> & //port
                                                     ,
                               bool intermediate)
    {
      tempType<bool> *ok;
      ok = new tempType<bool>;
      ok->tempVariable = intermediate;
      return ok;
    }

    template<>
    inline
    bool functionDestroyHelper(const sc_core::sc_inout<bool> & //port
                               , void* buffer)
    {
      tempType<bool> *rescued_ok = (reinterpret_cast<tempType<bool>* >(buffer));
      bool returnValue =  rescued_ok->tempVariable;
      delete rescued_ok; 
      return returnValue;
    }

    //--------------------------------------------------------------------------

    // sc_out
    template<class T1, class T2>
    inline
    void* functionCreateHelper(const sc_core::sc_out<T1> &port, T2 intermediate)
    {
      tempType<T1> *ok;
      ok = new tempType<T1>;
      ok->tempVariable = intermediate;
      return ok;
    }

    template<class T>
    inline
    T functionDestroyHelper(const sc_core::sc_out<T> &port, void* buffer)
    {
      tempType<T> *rescued_ok = (reinterpret_cast<tempType<T>* >(buffer));
      T returnValue =  rescued_ok->tempVariable;
      delete rescued_ok; 
      return returnValue;
    }

  } // namespace osssi

} // namespace osss

//------------------------------------------------------------------------------

// use variadic macro because second parameter can be a template and hence contain commas
// only used for deprecated OSSS_SHARED_* defines
#define _OSSS_ACCESS_SHARED_INNER_CLASS( port, ... ) \
        std::cerr << "Use of deprecated method: _OSSS_ACCESS_SHARED_INNER_CLASS" << std::endl; \
        __VA_ARGS__* obj_if = dynamic_cast<__VA_ARGS__*>(port.operator->()); \
        if ( obj_if == NULL) \
        { \
          std::cerr << "osss_port_to_shared: " \
                    << #port \
                    << " can not access class of type: " \
                    << #__VA_ARGS__ << ::std::endl; \
          OSSS_ABORT(); \
        } \
        else

//! \brief Remote function call on a shared object issued by a client
//!
//! The OSSS_SHARED_FC macro must be used for calling  functions (methods with a
//! return type un-equal to void) on shared objects. The server argument passed
//! to the macro must denote the shared object to be accessed. The  methodcall
//! argument must denote the function to be called including all necessary
//! arguments. The returnValue argument  must denote a variable of appropriate
//! type that  will store the value returned by the called function. Using this
//! macro invokes the arbitration mechanism of a shared object.

#define OSSS_SHARED_FC( returnvalue, port, object_if, methodcall ) \
        { \
          std::cerr << "Use of deprecated method: OSSS_SHARED_FC" << std::endl; \
          _OSSS_ACCESS_SHARED_INNER_CLASS( port, object_if ) \
          { \
            if (port.getArbiter()->dummy()) \
            { \
              while ( !obj_if->_guarded_##methodcall ) { sc_core::wait(); } \
              _METHODCALL_(returnvalue, obj_if, _user_##methodcall); \
              _RETURN_( returnvalue ); \
              sc_core::wait(); \
            } \
            else \
            { \
              while ( !port.getArbiter()->getGrant( &port ) ) \
              { \
                port.getArbiter()->setRequest( &port, true ); \
                port.getArbiter()->setGuard( &port, obj_if->_guarded_##methodcall ); \
                port.getArbiter()->updateGrant(); \
                sc_core::wait(); \
              } \
              port.getArbiter()->setRequest( &port, false ); \
              _METHODCALL_(returnvalue, obj_if, _user_##methodcall); \
              port.getArbiter()->setGrant( &port, false ); \
              sc_core::wait(); \
              _RETURN_( returnvalue ); \
              sc_core::wait(); \
            } \
          } \
        }


//! \brief Remote function call on a shared object issued by the testbench
//!
//! The OSSS_TB_SHARED_FC macro is essentially the same as the OSSS_SHARED_FC
//! macro but it is intended for being used within a sc_main() routine only. The
//! additional fourth argument to be passed (clockPeriod) must be of type sc_time
//! and must specify the length of the period of the clock  driving the clients
//! of the accessed shared object.

#define OSSS_TB_SHARED_FC( returnvalue, port, object_if, methodcall, clockPeriod ) \
        { \
          std::cerr << "Use of deprecated method: OSSS_TB_SHARED_FC" << std::endl; \
          _OSSS_ACCESS_SHARED_INNER_CLASS( port, object_if ) \
          { \
             if (port.getArbiter()->dummy()) \
            { \
              while ( !obj_if->_guarded_##methodcall ) { sc_core::wait(); } \
              _METHODCALL_(returnvalue, obj_if, _user_##methodcall); \
              _RETURN_( returnvalue ); \
              sc_core::sc_start( clockPeriod ); \
            } \
            else \
            { \
              while ( !port.getArbiter()->getGrant( &port ) ) \
              { \
                port.getArbiter()->setRequest( &port, true ); \
                port.getArbiter()->setGuard( &port, obj_if->_guarded_##methodcall ); \
                port.getArbiter()->updateGrant(); \
                sc_core::sc_start( clockPeriod ); \
              } \
              port.getArbiter()->setRequest( &port, false ); \
              _METHODCALL_(returnvalue, obj_if, _user_##methodcall); \
	      port.getArbiter()->setGrant( &port, false ); \
              sc_core::sc_start( clockPeriod ); \
              _RETURN_( returnvalue ); \
              sc_core::sc_start( clockPeriod ); \
            } \
          } \
        }


//! \brief Remote procedure (void function) call on a shared object issued 
//!        by a client
//!
//! The OSSS_SHARED_PC macro must be used for calling  procedures (methods with
//! void return type) on shared objects.  The server argument passed to the macro
//! must denote  the shared object to be accessed. The methodcall  argument must
//! denote the procedure to be called including all necessary arguments. Using
//! this macro invokes  the arbitration mechanism of a shared object.

#define OSSS_SHARED_PC( port, object_if, methodcall ) \
        { \
          std::cerr << "Use of deprecated method: OSSS_SHARED_PC" << std::endl; \
          _OSSS_ACCESS_SHARED_INNER_CLASS( port, object_if ) \
          { \
            if (port.getArbiter()->dummy()) \
            { \
              while ( !obj_if->_guarded_##methodcall ) { sc_core::wait(); } \
              obj_if->_user_##methodcall; \
              sc_core::wait(); \
            } \
            else \
            { \
              while ( !port.getArbiter()->getGrant( &port ) ) \
              { \
                port.getArbiter()->setRequest( &port, true ); \
                port.getArbiter()->setGuard( &port, obj_if->_guarded_##methodcall ); \
                port.getArbiter()->updateGrant(); \
                sc_core::wait(); \
              } \
              port.getArbiter()->setRequest( &port, false ); \
              obj_if->_user_##methodcall; \
              port.getArbiter()->setGrant( &port, false ); \
              sc_core::wait(); \
              sc_core::wait(); \
            } \
          } \
        }


//! \brief Remote procedure (void function) call on a shared object issued 
//!        by the testbench
//!
//! The OSSS_TB_SHARED_PC macro is essentially the same as the OSSS_SHARED_PC
//! macro but it is intended for being used within an sc_main() routine only. The
//! additional fourth argument to be passed (clockPeriod) must be of type sc_time
//! and must specify the length of the period of the clock  driving the clients
//! of the accessed shared object.

#define OSSS_TB_SHARED_PC( port, object_if, methodcall, clockPeriod ) \
        { \
          std::cerr << "Use of deprecated method: OSSS_TB_SHARED_PC" << std::endl; \
          _OSSS_ACCESS_SHARED_INNER_CLASS( port, object_if ) \
          { \
            if (port.getArbiter()->dummy()) \
            { \
              while ( !obj_if->_guarded_##methodcall ) { sc_core::wait(); } \
              obj_if->_user_##methodcall; \
              sc_core::sc_start( clockPeriod ); \
            } \
            else \
            { \
              while ( !port.getArbiter()->getGrant( &port ) ) \
              { \
                port.getArbiter()->setRequest( &port, true ); \
                port.getArbiter()->setGuard( &port, obj_if->_guarded_##methodcall ); \
                port.getArbiter()->updateGrant(); \
                sc_core::sc_start( clockPeriod ); \
              } \
              port.getArbiter()->setRequest( &port, false ); \
              obj_if->_user_##methodcall; \
              port.getArbiter()->setGrant( &port, false ); \
              sc_core::sc_start( clockPeriod ); \
              sc_core::sc_start( clockPeriod ); \
            } \
          } \
        }


// Declare a guarded method of a shared object

// The OSSS_GUARDED macro must be used for declaring public  methods of classes,
// that shall be used as shared objects.  Every public method of such a class
// except for constructors, must be declared as guarded method. The first
// argument (head) must specify the return type of a function (void in  case of
// a procedure) and can also include specifiers like  const, virtual, inline,
// etc. The second argument must be the signature of the method, including all
// formal parameters, but excluding the return type. The last argument
// (guardExpression) must be a const Boolean expression. This expression
// "guards" the method execution, that means a requesting client is only taken
// into account for arbitration iff the guard expression is evaluated "true"
// when requesting the guarded method.

// NOT USED ANYMORE !!!
/*
#define OSSS_GUARDED( head, signature, guardExpression ) \
        virtual \
        bool _guarded_##signature \
        { \
          std::cerr << "Use of deprecated method: OSSS_GUARDED" << std::endl; \
          return ( guardExpression ); \
        } \
        \
        head signature
*/

//==============================================================================

#define OSSS_EXPORTED(signature) \
    _user_##signature

//==============================================================================

#define _OSSS_MAKE_PRE_AND_POST_METHOD(formal_method_signature, actual_method_signature, guard_condition) \
    virtual \
    bool _guarded_##formal_method_signature \
    { \
      return ( guard_condition ); \
    } \
    virtual \
    void _pre_##formal_method_signature \
    { \
       ::osss::osss_shared_if<> * sif = \
         ::osss::osssi::osss_registry_shared::get_shared_object(this); \
       if ( sif->getArbiter()->dummy() ) \
       { \
         while( ! _guarded_##actual_method_signature ) { sc_core::wait(); } \
       } \
       else \
       { \
         ::osss::osssi::osss_process_handle current_handle = ::osss::osssi::osss_process_handle::current(); \
         while ( !sif->getArbiter()->getGrant(sif->lookupPortID( current_handle.get_proc_id() ) ) ) \
         { \
           sif->getArbiter()->setRequest( sif->lookupPortID( current_handle.get_proc_id() ), true ); \
           sif->getArbiter()->setGuard( sif->lookupPortID( current_handle.get_proc_id() ), \
                                        _guarded_##actual_method_signature ); \
           sif->getArbiter()->updateGrant(); \
           sc_core::wait(); \
         } \
         sif->getArbiter()->setRequest( sif->lookupPortID( current_handle.get_proc_id() ), false ); \
       } \
    } \
    virtual \
    void _post_##formal_method_signature \
    { \
      ::osss::osss_shared_if<> * sif = \
         ::osss::osssi::osss_registry_shared::get_shared_object(this); \
      if ( ! sif->getArbiter()->dummy() ) \
      { \
        ::osss::osssi::osss_process_handle current_handle = ::osss::osssi::osss_process_handle::current(); \
        sif->getArbiter()->setGrant( sif->lookupPortID( current_handle.get_proc_id() ), false ); \
        sc_core::wait(); \
      } \
    } 

#define _OSSS_GUARDED_METHOD(return_type, formal_method_signature, actual_method_signature, guard_condition) \
    _OSSS_MAKE_PRE_AND_POST_METHOD(formal_method_signature, actual_method_signature, guard_condition) \
    virtual return_type formal_method_signature \
    { \
      _pre_##actual_method_signature; \
      return_type return_value = _user_##actual_method_signature; \
      _post_##actual_method_signature; \
      return return_value; \
    } \
    virtual return_type _user_##formal_method_signature

#define _OSSS_GUARDED_METHOD_VOID(return_type, formal_method_signature, actual_method_signature, guard_condition) \
    _OSSS_MAKE_PRE_AND_POST_METHOD(formal_method_signature, actual_method_signature, guard_condition) \
    virtual return_type formal_method_signature \
    { \
      _pre_##actual_method_signature; \
      _user_##actual_method_signature; \
      _post_##actual_method_signature; \
      /* sc_core::wait(); // this wait has to be specified by the designer */ \
    } \
    virtual return_type _user_##formal_method_signature

//==============================================================================

#define _OSSS_GUARDED_METHOD0( _return_type_, _method_name_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( void ), \
                        _method_name_(), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD1( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_ ), \
                        _method_name_( _param1_ ), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD2( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_), \
                        _method_name_( _param1_, \
                                       _param2_ ), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD3( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_), \
                        _method_name_( _param1_, \
                                       _param2_, \
                                       _param3_ ), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD4( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_), \
                        _method_name_( _param1_, \
                                       _param2_, \
                                       _param3_, \
                                       _param4_ ), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD5( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _param_type5_, _param5_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_, \
                                       _param_type5_ _param5_), \
                        _method_name_( _param1_, \
                                       _param2_, \
                                       _param3_, \
                                       _param4_, \
                                       _param5_ ), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD6( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _param_type5_, _param5_, \
                               _param_type6_, _param6_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_, \
                                       _param_type5_ _param5_, \
                                       _param_type6_ _param6_), \
                        _method_name_( _param1_, \
                                       _param2_, \
                                       _param3_, \
                                       _param4_, \
                                       _param5_, \
                                       _param6_ ), \
                        _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD7( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _param_type5_, _param5_, \
                               _param_type6_, _param6_, \
                               _param_type7_, _param7_, \
                               _guard_condition_ ) \
  _OSSS_GUARDED_METHOD( _return_type_, \
                        _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_, \
                                       _param_type5_ _param5_, \
                                       _param_type6_ _param6_, \
                                       _param_type7_ _param7_), \
                        _method_name_( _param1_, \
                                       _param2_, \
                                       _param3_, \
                                       _param4_, \
                                       _param5_, \
                                       _param6_, \
                                       _param7_ ), \
                        _guard_condition_ )

//------------------------------------------------------------------------------

#define OSSS_VOID \
  0

#define OSSS_NO_PARAMS() \
  0

#define OSSS_PARAMS(N, ...) \
  N, ## __VA_ARGS__

/*
// really needed ???
#define _OSSS_METHOD_TMP( _return_type_, _method_name_, N, ...) \
  _OSSS_METHOD##N( _return_type_, _method_name_, ## __VA_ARGS__ )
*/

#define _OSSS_GUARDED_METHOD_TMP( _return_type_, _method_name_, N, ...) \
  _OSSS_GUARDED_METHOD##N( _return_type_, _method_name_, ## __VA_ARGS__ )

/*
// really needed ???
#define OSSS_METHOD( _return_type_, _method_name_, ...) \
  _OSSS_METHOD_TMP( _return_type_, _method_name_, __VA_ARGS__ )
*/

#define OSSS_GUARDED_METHOD( _return_type_, _method_name_, ...) \
  _OSSS_GUARDED_METHOD_TMP( _return_type_, _method_name_, __VA_ARGS__ )

//==============================================================================

#define _OSSS_GUARDED_METHOD0_VOID( _method_name_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( void ), \
                             _method_name_(), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD1_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_ ), \
                             _method_name_( _param1_ ), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD2_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_, \
                                            _param_type2_ _param2_), \
                             _method_name_( _param1_, \
                                            _param2_ ), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD3_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_, \
                                            _param_type2_ _param2_, \
                                            _param_type3_ _param3_), \
                             _method_name_( _param1_, \
                                            _param2_, \
                                            _param3_ ), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD4_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_, \
                                            _param_type2_ _param2_, \
                                            _param_type3_ _param3_, \
                                            _param_type4_ _param4_), \
                             _method_name_( _param1_, \
                                            _param2_, \
                                            _param3_, \
                                            _param4_ ), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD5_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _param_type5_, _param5_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_, \
                                            _param_type2_ _param2_, \
                                            _param_type3_ _param3_, \
                                            _param_type4_ _param4_, \
                                            _param_type5_ _param5_), \
                             _method_name_( _param1_, \
                                            _param2_, \
                                            _param3_, \
                                            _param4_, \
                                            _param5_ ), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD6_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _param_type5_, _param5_, \
                                    _param_type6_, _param6_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_, \
                                            _param_type2_ _param2_, \
                                            _param_type3_ _param3_, \
                                            _param_type4_ _param4_, \
                                            _param_type5_ _param5_, \
                                            _param_type6_ _param6_), \
                             _method_name_( _param1_, \
                                            _param2_, \
                                            _param3_, \
                                            _param4_, \
                                            _param5_, \
                                            _param6_ ), \
                             _guard_condition_ )

//---

#define _OSSS_GUARDED_METHOD7_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _param_type5_, _param5_, \
                                    _param_type6_, _param6_, \
                                    _param_type7_, _param7_, \
                                    _guard_condition_ ) \
  _OSSS_GUARDED_METHOD_VOID( void, \
                             _method_name_( _param_type1_ _param1_, \
                                            _param_type2_ _param2_, \
                                            _param_type3_ _param3_, \
                                            _param_type4_ _param4_, \
                                            _param_type5_ _param5_, \
                                            _param_type6_ _param6_, \
                                            _param_type7_ _param7_), \
                             _method_name_( _param1_, \
                                            _param2_, \
                                            _param3_, \
                                            _param4_, \
                                            _param5_, \
                                            _param6_, \
                                            _param7_ ), \
                             _guard_condition_ )

//------------------------------------------------------------------------------

/*
// really needed ???
#define _OSSS_METHOD_VOID_TMP( _method_name_, N, ...) \
  _OSSS_METHOD##N##_VOID( _method_name_, ## __VA_ARGS__ )
*/

#define _OSSS_GUARDED_METHOD_VOID_TMP( _method_name_, N, ...) \
  _OSSS_GUARDED_METHOD##N##_VOID( _method_name_, ## __VA_ARGS__ )

/*
// really needed ???
#define OSSS_METHOD_VOID( _method_name_, ...) \
  _OSSS_METHOD_VOID_TMP( _method_name_, __VA_ARGS__ )
*/

#define OSSS_GUARDED_METHOD_VOID( _method_name_, ...) \
  _OSSS_GUARDED_METHOD_VOID_TMP( _method_name_, __VA_ARGS__ )

#endif
// $Id: osss_hw_macros_blue.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
