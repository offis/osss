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

#ifndef OSSS_HW_MACROS_GREEN_INCLUDED
#define OSSS_HW_MACROS_GREEN_INCLUDED

#include "o3s/utils/osss_common.h"

//joins

//==============================================================================
// creates guard methods for shared objects
//==============================================================================

#define _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, () ) \
  { \
    return ( _guard_condition_ ); \
  }

//---

#define _OSSS_MAKE_GUARD1( _method_name_, \
                           _param_type1_, _param1_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_ ); \
  }

//---

#define _OSSS_MAKE_GUARD2( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_ ); \
  }

//---

#define _OSSS_MAKE_GUARD3( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_ ); \
  }

//---

#define _OSSS_MAKE_GUARD4( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _param_type4_, _param4_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_ ); \
  }

//---

#define _OSSS_MAKE_GUARD5( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _param_type4_, _param4_, \
                           _param_type5_, _param5_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_, \
                                         _param_type5_ _param5_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_, \
                                     *_param5_ ); \
  }

//---

#define _OSSS_MAKE_GUARD6( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _param_type4_, _param4_, \
                           _param_type5_, _param5_, \
                           _param_type6_, _param6_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_, \
                                         _param_type5_ _param5_, \
                                         _param_type6_ _param6_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_, \
                                     *_param5_, \
                                     *_param6_ ); \
  }

//---

#define _OSSS_MAKE_GUARD7( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _param_type4_, _param4_, \
                           _param_type5_, _param5_, \
                           _param_type6_, _param6_, \
                           _param_type7_, _param7, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_, \
                                         _param_type5_ _param5_, \
                                         _param_type6_ _param6_, \
                                         _param_type7_ _param7_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_, \
                                     *_param5_, \
                                     *_param6_, \
                                     *_param7_ ); \
  }

//---

#define _OSSS_MAKE_GUARD8( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _param_type4_, _param4_, \
                           _param_type5_, _param5_, \
                           _param_type6_, _param6_, \
                           _param_type7_, _param7_, \
                           _param_type8_, _param8_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_, \
                                         _param_type5_ _param5_, \
                                         _param_type6_ _param6_, \
                                         _param_type7_ _param7_, \
                                         _param_type8_ _param8_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type8_ >* _param8_, ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_, \
                                     *_param5_, \
                                     *_param6_, \
                                     *_param7_, \
                                     *_param8_ ); \
  }

//---

#define _OSSS_MAKE_GUARD9( _method_name_, \
                           _param_type1_, _param1_, \
                           _param_type2_, _param2_, \
                           _param_type3_, _param3_, \
                           _param_type4_, _param4_, \
                           _param_type5_, _param5_, \
                           _param_type6_, _param6_, \
                           _param_type7_, _param7_, \
                           _param_type8_, _param8_, \
                           _param_type9_, _param9_, \
                           _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_, \
                                         _param_type5_ _param5_, \
                                         _param_type6_ _param6_, \
                                         _param_type7_ _param7_, \
                                         _param_type8_ _param8_, \
                                         _param_type9_ _param9_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type8_ >* _param8_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type9_ >* _param9_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_, \
                                     *_param5_, \
                                     *_param6_, \
                                     *_param7_, \
                                     *_param8_, \
                                     *_param9_ ); \
 }

//---

#define _OSSS_MAKE_GUARD10( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_, \
                            _param_type3_, _param3_, \
                            _param_type4_, _param4_, \
                            _param_type5_, _param5_, \
                            _param_type6_, _param6_, \
                            _param_type7_, _param7_, \
                            _param_type8_, _param8_, \
                            _param_type9_, _param9_, \
                            _param_type10_, _param10_, \
                            _guard_condition_ ) \
  virtual bool _guarded_##_method_name_( _param_type1_ _param1_, \
                                         _param_type2_ _param2_, \
                                         _param_type3_ _param3_, \
                                         _param_type4_ _param4_, \
                                         _param_type5_ _param5_, \
                                         _param_type6_ _param6_, \
                                         _param_type7_ _param7_, \
                                         _param_type8_ _param8_, \
                                         _param_type9_ _param9_, \
                                         _param_type10_, _param10_ ) \
  { \
    return ( _guard_condition_ ); \
  } \
  VIRTUAL_METHOD( public, bool, _guarded_stub_##_method_name_, \
                          ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type8_ >* _param8_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type9_ >* _param9_, \
                            ::osss::osssi::osss_serialisable_wrapper< _param_type10_ >* _param10_ ) ) \
  { \
    return _guarded_##_method_name_( *_param1_, \
                                     *_param2_, \
                                     *_param3_, \
                                     *_param4_, \
                                     *_param5_, \
                                     *_param6_, \
                                     *_param7_, \
                                     *_param8_, \
                                     *_param9_, \
                                     *_param10_ ); \
  }

//==============================================================================
// OSSS_METHOD (guarded & unguarded) with return type
//==============================================================================

#define _OSSS_METHOD0( _return_type_, _method_name_ ) \
  ::osss::osssi::osss_serialisable_wrapper< _return_type_ > \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, () ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_(); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_()

#define _OSSS_GUARDED_METHOD0( _return_type_, _method_name_, \
                               _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD0( _return_type_, _method_name_ )

#define _OSSS_GUARDED_METHOD_SIMPLE0( _return_type_, _method_name_, \
                                      _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD0( _return_type_, _method_name_ )

//---

#define _OSSS_METHOD1( _return_type_, _method_name_, \
                                      _param_type1_, _param1_ ) \
  ::osss::osssi::osss_serialisable_wrapper< _return_type_ > \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_ )

#define _OSSS_GUARDED_METHOD1( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD1( _method_name_, \
                     _param_type1_, _param1_, \
                     _guard_condition_ ) \
  _OSSS_METHOD1( _return_type_, _method_name_, \
                                _param_type1_, _param1_ )

#define _OSSS_GUARDED_METHOD_SIMPLE1( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD1( _return_type_, _method_name_, \
                                _param_type1_, _param1_ )

//---

#define _OSSS_METHOD2( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_ ) \
  ::osss::osssi::osss_serialisable_wrapper< _return_type_ > \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_, \
                                                                *_param2_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_, \
                               _param_type2_ _param2_ )

#define _OSSS_GUARDED_METHOD2( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD2( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _guard_condition_ ) \
  _OSSS_METHOD2( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_ )

#define _OSSS_GUARDED_METHOD_SIMPLE2( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD2( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_ )

//---

#define _OSSS_METHOD3( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_ ) \
  ::osss::osssi::osss_serialisable_wrapper<_return_type_> \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_, \
                                                                *_param2_, \
                                                                *_param3_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_, \
                               _param_type2_ _param2_, \
                               _param_type3_ _param3_ )

#define _OSSS_GUARDED_METHOD3( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD3( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _guard_condition_ ) \
  _OSSS_METHOD3( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_ )

#define _OSSS_GUARDED_METHOD_SIMPLE3( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD3( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_ )

//---

#define _OSSS_METHOD4( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_ ) \
  ::osss::osssi::osss_serialisable_wrapper<_return_type_> \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_, \
                                                                *_param2_, \
                                                                *_param3_, \
                                                                *_param4_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_, \
                               _param_type2_ _param2_, \
                               _param_type3_ _param3_, \
                               _param_type4_ _param4_)

#define _OSSS_GUARDED_METHOD4( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD4( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _guard_condition_ ) \
  _OSSS_METHOD4( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_ )

#define _OSSS_GUARDED_METHOD_SIMPLE4( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD4( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_ )

//---

#define _OSSS_METHOD5( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
                                      _param_type5_, _param5_ ) \
  ::osss::osssi::osss_serialisable_wrapper<_return_type_> \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_, \
                                                                *_param2_, \
                                                                *_param3_, \
                                                                *_param4_, \
                                                                *_param5_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_, \
                               _param_type2_ _param2_, \
                               _param_type3_ _param3_, \
                               _param_type4_ _param4_, \
                               _param_type5_ _param5_)

#define _OSSS_GUARDED_METHOD5( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _param_type5_, _param5_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD5( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _param_type5_, _param5_, \
                     _guard_condition_ ) \
  _OSSS_METHOD5( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_, \
                                _param_type5_, _param5_ )

#define _OSSS_GUARDED_METHOD_SIMPLE5( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
                                      _param_type5_, _param5_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD5( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_, \
                                _param_type5_, _param5_ )

//---

#define _OSSS_METHOD6( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
                                      _param_type5_, _param5_, \
                                      _param_type6_, _param6_ ) \
  ::osss::osssi::osss_serialisable_wrapper<_return_type_> \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_, \
                                                                *_param2_, \
                                                                *_param3_, \
                                                                *_param4_, \
                                                                *_param5_, \
                                                                *_param6_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_, \
                               _param_type2_ _param2_, \
                               _param_type3_ _param3_, \
                               _param_type4_ _param4_, \
                               _param_type5_ _param5_, \
                               _param_type6_ _param6_ )

#define _OSSS_GUARDED_METHOD6( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _param_type5_, _param5_, \
                               _param_type6_, _param6_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD6( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _param_type5_, _param5_, \
                     _param_type6_, _param6_, \
                     _guard_condition_ ) \
  _OSSS_METHOD6( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_, \
                                _param_type5_, _param5_, \
                                _param_type6_, _param6_ )

#define _OSSS_GUARDED_METHOD_SIMPLE6( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
                                      _param_type5_, _param5_, \
                                      _param_type6_, _param6_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD6( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_, \
                                _param_type5_, _param5_, \
                                _param_type6_, _param6_ )

//---

#define _OSSS_METHOD7( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
                                      _param_type5_, _param5_, \
                                      _param_type6_, _param6_, \
                                      _param_type7_, _param7_ ) \
  ::osss::osssi::osss_serialisable_wrapper<_return_type_> \
  OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  METHOD( public, ::osss::osssi::osss_serialisable_wrapper<_return_type_>*, \
                  _stub_##_method_name_, \
                  ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >::type_check(); \
    OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__) = _method_name_( *_param1_, \
                                                                *_param2_, \
                                                                *_param3_, \
                                                                *_param4_, \
                                                                *_param5_, \
                                                                *_param6_, \
                                                                *_param7_ ); \
    return & OSSS_IMPL_CONCAT_(_tmp_ ## _method_name_, __LINE__); \
  } \
  _return_type_ _method_name_( _param_type1_ _param1_, \
                               _param_type2_ _param2_, \
                               _param_type3_ _param3_, \
                               _param_type4_ _param4_, \
                               _param_type5_ _param5_, \
                               _param_type6_ _param6_, \
                               _param_type7_ _param7_ )

#define _OSSS_GUARDED_METHOD7( _return_type_, _method_name_, \
                               _param_type1_, _param1_, \
                               _param_type2_, _param2_, \
                               _param_type3_, _param3_, \
                               _param_type4_, _param4_, \
                               _param_type5_, _param5_, \
                               _param_type6_, _param6_, \
                               _param_type7_, _param7_, \
			       _guard_condition_ ) \
  _OSSS_MAKE_GUARD7( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _param_type5_, _param5_, \
                     _param_type6_, _param6_, \
                     _param_type7_, _param7_, \
                     _guard_condition_ ) \
  _OSSS_METHOD7( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_, \
                                _param_type5_, _param5_, \
                                _param_type6_, _param6_, \
                                _param_type7_, _param7_ )

#define _OSSS_GUARDED_METHOD_SIMPLE7( _return_type_, _method_name_, \
                                      _param_type1_, _param1_, \
                                      _param_type2_, _param2_, \
                                      _param_type3_, _param3_, \
                                      _param_type4_, _param4_, \
                                      _param_type5_, _param5_, \
                                      _param_type6_, _param6_, \
                                      _param_type7_, _param7_, \
			              _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD7( _return_type_, _method_name_, \
                                _param_type1_, _param1_, \
                                _param_type2_, _param2_, \
                                _param_type3_, _param3_, \
                                _param_type4_, _param4_, \
                                _param_type5_, _param5_, \
                                _param_type6_, _param6_, \
                                _param_type7_, _param7_ )

//---

#define OSSS_VOID \
  0

#define OSSS_NO_PARAMS() \
  0

#define OSSS_PARAMS(N, ...) \
  N, ## __VA_ARGS__

#define _OSSS_METHOD_TMP( _return_type_, _method_name_, N, ...) \
  _OSSS_METHOD##N( _return_type_, _method_name_, ## __VA_ARGS__ )

#define _OSSS_GUARDED_METHOD_TMP( _return_type_, _method_name_, N, ...) \
  _OSSS_GUARDED_METHOD##N( _return_type_, _method_name_, ## __VA_ARGS__ )

#define _OSSS_GUARDED_METHOD_TMP_SIMPLE( _return_type_, _method_name_, N, ...) \
  _OSSS_GUARDED_METHOD_SIMPLE##N( _return_type_, _method_name_, ## __VA_ARGS__ )

#define OSSS_METHOD( _return_type_, _method_name_, ...) \
  _OSSS_METHOD_TMP( _return_type_, _method_name_, __VA_ARGS__ )

#if defined(OSSS_SIMPLE_GUARDS)
#define OSSS_GUARDED_METHOD( _return_type_, _method_name_, ...) \
  _OSSS_GUARDED_METHOD_TMP_SIMPLE( _return_type_, _method_name_, __VA_ARGS__ )
#else
#define OSSS_GUARDED_METHOD( _return_type_, _method_name_, ...) \
  _OSSS_GUARDED_METHOD_TMP( _return_type_, _method_name_, __VA_ARGS__ )
#endif

//==============================================================================
// OSSS_METHOD_VOID (guarded & unguarded) with NO return type
//==============================================================================

#define _OSSS_METHOD0_VOID( _method_name_ ) \
  METHOD( public, void, _stub_##_method_name_, () ) \
  { \
    _method_name_(); \
  } \
  void _method_name_()

#define _OSSS_GUARDED_METHOD0_VOID( _method_name_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD0_VOID( _method_name_ )

#define _OSSS_GUARDED_METHOD0_VOID_SIMPLE( _method_name_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD0_VOID( _method_name_ )

//---

#define _OSSS_METHOD1_VOID( _method_name_, \
                            _param_type1_, _param1_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    _method_name_( *_param1_ ); \
  } \
  void _method_name_( _param_type1_ _param1_ )

#define _OSSS_GUARDED_METHOD1_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _guard_condition_ ) \
  _OSSS_MAKE_GUARD1( _method_name_, \
                     _param_type1_, _param1_, \
                     _guard_condition_ ) \
  _OSSS_METHOD1_VOID( _method_name_, \
                      _param_type1_, _param1_ )

#define _OSSS_GUARDED_METHOD1_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD1_VOID( _method_name_, \
                      _param_type1_, _param1_ )

//---

#define _OSSS_METHOD2_VOID( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    _method_name_( *_param1_, \
                   *_param2_ ); \
  } \
  void _method_name_( _param_type1_ _param1_, \
                      _param_type2_ _param2_ )

#define _OSSS_GUARDED_METHOD2_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD2( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _guard_condition_ ) \
  _OSSS_METHOD2_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_ )

#define _OSSS_GUARDED_METHOD2_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD2_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_ )

//---

#define _OSSS_METHOD3_VOID( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_, \
                            _param_type3_, _param3_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    _method_name_( *_param1_, \
                   *_param2_, \
                   *_param3_ ); \
  } \
  void _method_name_( _param_type1_ _param1_, \
                      _param_type2_ _param2_, \
                      _param_type3_ _param3_ )

#define _OSSS_GUARDED_METHOD3_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD3( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _guard_condition_ ) \
  _OSSS_METHOD3_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_ )

#define _OSSS_GUARDED_METHOD3_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD3_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_ )

//--

#define _OSSS_METHOD4_VOID( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_, \
                            _param_type3_, _param3_, \
                            _param_type4_, _param4_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    _method_name_( *_param1_, \
                   *_param2_, \
                   *_param3_, \
                   *_param4_ ); \
  } \
  void _method_name_( _param_type1_ _param1_, \
                      _param_type2_ _param2_, \
                      _param_type3_ _param3_, \
                      _param_type4_ _param4_ )

#define _OSSS_GUARDED_METHOD4_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD4( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _guard_condition_ ) \
  _OSSS_METHOD4_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_ )

#define _OSSS_GUARDED_METHOD4_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD4_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_ )

//---

#define _OSSS_METHOD5_VOID( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_, \
                            _param_type3_, _param3_, \
                            _param_type4_, _param4_, \
                            _param_type5_, _param5_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    _method_name_( *_param1_, \
                   *_param2_, \
                   *_param3_, \
                   *_param4_, \
                   *_param5_ ); \
  } \
  void _method_name_( _param_type1_ _param1_, \
                      _param_type2_ _param2_, \
                      _param_type3_ _param3_, \
                      _param_type4_ _param4_, \
                      _param_type5_ _param5_ )

#define _OSSS_GUARDED_METHOD5_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _param_type5_, _param5_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD5( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _param_type5_, _param5_, \
                     _guard_condition_ ) \
  _OSSS_METHOD5_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_, \
                      _param_type5_, _param5_ )

#define _OSSS_GUARDED_METHOD5_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
                                           _param_type5_, _param5_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD5_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_, \
                      _param_type5_, _param5_ )

//---

#define _OSSS_METHOD6_VOID( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_, \
                            _param_type3_, _param3_, \
                            _param_type4_, _param4_, \
                            _param_type5_, _param5_, \
                            _param_type6_, _param6_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    _method_name_( *_param1_, \
                   *_param2_, \
                   *_param3_, \
                   *_param4_, \
                   *_param5_, \
                   *_param6_ ); \
  } \
  void _method_name_( _param_type1_ _param1_, \
                      _param_type2_ _param2_, \
                      _param_type3_ _param3_, \
                      _param_type4_ _param4_, \
                      _param_type5_ _param5_, \
                      _param_type6_ _param6_ )

#define _OSSS_GUARDED_METHOD6_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _param_type5_, _param5_, \
                                    _param_type6_, _param6_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD6( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _param_type5_, _param5_, \
                     _param_type6_, _param6_, \
                     _guard_condition_ ) \
  _OSSS_METHOD6_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_, \
                      _param_type5_, _param5_, \
                      _param_type6_, _param6_ )

#define _OSSS_GUARDED_METHOD6_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
                                           _param_type5_, _param5_, \
                                           _param_type6_, _param6_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD6_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_, \
                      _param_type5_, _param5_, \
                      _param_type6_, _param6_ )

//---

#define _OSSS_METHOD7_VOID( _method_name_, \
                            _param_type1_, _param1_, \
                            _param_type2_, _param2_, \
                            _param_type3_, _param3_, \
                            _param_type4_, _param4_, \
                            _param_type5_, _param5_, \
                            _param_type6_, _param6_, \
                            _param_type7_, _param7_ ) \
  METHOD( public, void, _stub_##_method_name_, \
                        ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                          ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_ ) ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >::type_check(); \
    _method_name_( *_param1_, \
                   *_param2_, \
                   *_param3_, \
                   *_param4_, \
                   *_param5_, \
                   *_param6_, \
                   *_param7_ ); \
  } \
  void _method_name_( _param_type1_ _param1_, \
                      _param_type2_ _param2_, \
                      _param_type3_ _param3_, \
                      _param_type4_ _param4_, \
                      _param_type5_ _param5_, \
                      _param_type6_ _param6_, \
                      _param_type7_ _param7_ )

#define _OSSS_GUARDED_METHOD7_VOID( _method_name_, \
                                    _param_type1_, _param1_, \
                                    _param_type2_, _param2_, \
                                    _param_type3_, _param3_, \
                                    _param_type4_, _param4_, \
                                    _param_type5_, _param5_, \
                                    _param_type6_, _param6_, \
                                    _param_type7_, _param7_, \
				    _guard_condition_ ) \
  _OSSS_MAKE_GUARD7( _method_name_, \
                     _param_type1_, _param1_, \
                     _param_type2_, _param2_, \
                     _param_type3_, _param3_, \
                     _param_type4_, _param4_, \
                     _param_type5_, _param5_, \
                     _param_type6_, _param6_, \
                     _param_type7_, _param7_, \
                     _guard_condition_ ) \
  _OSSS_METHOD7_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_, \
                      _param_type5_, _param5_, \
                      _param_type6_, _param6_, \
                      _param_type7_, _param7_ )

#define _OSSS_GUARDED_METHOD7_VOID_SIMPLE( _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
                                           _param_type5_, _param5_, \
                                           _param_type6_, _param6_, \
                                           _param_type7_, _param7_, \
				           _guard_condition_ ) \
  _OSSS_MAKE_GUARD0( _method_name_, _guard_condition_ ) \
  _OSSS_METHOD7_VOID( _method_name_, \
                      _param_type1_, _param1_, \
                      _param_type2_, _param2_, \
                      _param_type3_, _param3_, \
                      _param_type4_, _param4_, \
                      _param_type5_, _param5_, \
                      _param_type6_, _param6_, \
                      _param_type7_, _param7_ )

//---

#define _OSSS_METHOD_VOID_TMP( _method_name_, N, ...) \
  _OSSS_METHOD##N##_VOID( _method_name_, ## __VA_ARGS__ )

#define _OSSS_GUARDED_METHOD_VOID_TMP( _method_name_, N, ...) \
  _OSSS_GUARDED_METHOD##N##_VOID( _method_name_, ## __VA_ARGS__ )

#define _OSSS_GUARDED_METHOD_VOID_TMP_SIMPLE( _method_name_, N, ...) \
  _OSSS_GUARDED_METHOD##N##_VOID_SIMPLE( _method_name_, ## __VA_ARGS__ )

#define OSSS_METHOD_VOID( _method_name_, ...) \
  _OSSS_METHOD_VOID_TMP( _method_name_, __VA_ARGS__ )

#if defined(OSSS_SIMPLE_GUARDS)
#define OSSS_GUARDED_METHOD_VOID( _method_name_, ...) \
  _OSSS_GUARDED_METHOD_VOID_TMP_SIMPLE( _method_name_, __VA_ARGS__ )
#else
#define OSSS_GUARDED_METHOD_VOID( _method_name_, ...) \
  _OSSS_GUARDED_METHOD_VOID_TMP( _method_name_, __VA_ARGS__ )
#endif

//==============================================================================
// non-void method wrappers used as stubs for RMI 
//==============================================================================

#define _OSSS_METHOD0_STUB( _return_type_, _method_name_ ) \
 VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                          _stub_##_method_name_, () ) \
 { \
   /* bool _hasReturnValue = true; */ \
   std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
   std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
   /* unsigned int _num_params = 0; */ \
   OSSS_PREPARE_METHOD_CALL; \
   OSSS_FUNCTION_CALL( _return_type_ ); \
 } \
 public : \
 virtual _return_type_ _method_name_() \
 { \
   ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
   ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
     _stub_##_method_name_(); \
   ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
     *return_value_ptr; \
   delete return_value_ptr; \
   return return_value; \
 }

//---

#define _OSSS_METHOD1_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 1; */ \
    /* std::string  _params_type[1] = { std::string(#_param_type1_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    delete _param1_; \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD2_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 2; */ \
    /* std::string  _params_type[2] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    delete _param1_; \
    delete _param2_;  \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_, \
                             _tmp_param2_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD3_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 3; */ \
    /* std::string  _params_type[3] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_, \
                             _tmp_param2_ptr_, \
                             _tmp_param3_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD4_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 4; */ \
    /* std::string  _params_type[4] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_, \
                             _tmp_param2_ptr_, \
                             _tmp_param3_ptr_, \
                             _tmp_param4_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD5_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
                                           _param_type5_, _param5_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 5; */ \
    /* std::string  _params_type[5] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*"), */ \
    /*                                 std::string(#_param_type5_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    OSSS_SERIALISE_ARGUMENT( _param5_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    delete _param5_; \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_, \
                                       _param_type5_ _param5_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _tmp_param5_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >(_param5_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_, \
                             _tmp_param2_ptr_, \
                             _tmp_param3_ptr_, \
                             _tmp_param4_ptr_, \
                             _tmp_param5_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD6_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
                                           _param_type5_, _param5_, \
                                           _param_type6_, _param6_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 6; */ \
    /* std::string  _params_type[6] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*"), */ \
    /*                                 std::string(#_param_type5_) + std::string("*"), */ \
    /*                                 std::string(#_param_type6_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    OSSS_SERIALISE_ARGUMENT( _param5_ ); \
    OSSS_SERIALISE_ARGUMENT( _param6_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    delete _param5_; \
    delete _param6_; \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_, \
                                       _param_type5_ _param5_, \
                                       _param_type6_ _param6_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _tmp_param5_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >(_param5_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _tmp_param6_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >(_param6_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_, \
                             _tmp_param2_ptr_, \
                             _tmp_param3_ptr_, \
                             _tmp_param4_ptr_, \
                             _tmp_param5_ptr_, \
                             _tmp_param6_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD7_STUB( _return_type_, _method_name_, \
                                           _param_type1_, _param1_, \
                                           _param_type2_, _param2_, \
                                           _param_type3_, _param3_, \
                                           _param_type4_, _param4_, \
                                           _param_type5_, _param5_, \
                                           _param_type6_, _param6_, \
                                           _param_type7_, _param7_ ) \
  VIRTUAL_METHOD( private, ::osss::osssi::osss_serialisable_wrapper< _return_type_ >*, \
                           _stub_##_method_name_, \
                           ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                             ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_ ) ) \
  { \
    /* bool _hasReturnValue = true; */ \
    std::string  _return_type = std::string(#_return_type_) + std::string("*"); \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 6; */ \
    /* std::string  _params_type[6] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*"), */ \
    /*                                 std::string(#_param_type5_) + std::string("*"), */ \
    /*                                 std::string(#_param_type6_) + std::string("*"), */ \
    /*                                 std::string(#_param_type7_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    OSSS_SERIALISE_ARGUMENT( _param5_ ); \
    OSSS_SERIALISE_ARGUMENT( _param6_ ); \
    OSSS_SERIALISE_ARGUMENT( _param7_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    delete _param5_; \
    delete _param6_; \
    delete _param7_; \
    OSSS_FUNCTION_CALL( _return_type_ ); \
  } \
  public : \
  virtual _return_type_ _method_name_( _param_type1_ _param1_, \
                                       _param_type2_ _param2_, \
                                       _param_type3_ _param3_, \
                                       _param_type4_ _param4_, \
                                       _param_type5_ _param5_, \
                                       _param_type6_ _param6_, \
                                       _param_type7_ _param7_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _tmp_param5_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >(_param5_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _tmp_param6_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >(_param6_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _tmp_param7_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >(_param7_); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >* return_value_ptr = \
      _stub_##_method_name_( _tmp_param1_ptr_, \
                             _tmp_param2_ptr_, \
                             _tmp_param3_ptr_, \
                             _tmp_param4_ptr_, \
                             _tmp_param5_ptr_, \
                             _tmp_param6_ptr_, \
                             _tmp_param7_ptr_ ); \
    ::osss::osssi::osss_serialisable_wrapper< _return_type_ >  return_value = \
      *return_value_ptr; \
    delete return_value_ptr; \
    return return_value; \
  }

//---

#define _OSSS_METHOD_STUB_TMP( _return_type_, _method_name_, N, ...) \
  _OSSS_METHOD##N##_STUB( _return_type_, _method_name_, ## __VA_ARGS__ )

#define OSSS_METHOD_STUB( _return_type_, _method_name_, ...) \
  _OSSS_METHOD_STUB_TMP( _return_type_, _method_name_, __VA_ARGS__ )


//==============================================================================
// void method wrappers used as stubs for RMI 
//==============================================================================

#define _OSSS_METHOD0_VOID_STUB( _method_name_ ) \
 VIRTUAL_METHOD( private, void, _stub_##_method_name_, () ) \
 { \
   /* bool _hasReturnValue = false; */ \
   std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
   /* unsigned int _num_params = 0; */ \
   OSSS_PREPARE_METHOD_CALL; \
   OSSS_PROCEDURE_CALL; \
 } \
 public : \
 virtual void _method_name_() \
 { \
   _stub_##_method_name_(); \
 }

//---

#define _OSSS_METHOD1_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_, \
                                 ( ::osss::osssi::osss_serialisable_wrapper<_param_type1_>* _param1_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 1; */ \
    /* std::string  _params_type[1] = { std::string(#_param_type1_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    delete _param1_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    _stub_##_method_name_( _tmp_param1_ptr_ ); \
  }

//---

#define _OSSS_METHOD2_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_, \
                                 _param_type2_, _param2_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_ , \
                                 ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 2; */ \
    /* std::string  _params_type[2] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    delete _param1_; \
    delete _param2_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_, \
                              _param_type2_ _param2_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    _stub_##_method_name_( _tmp_param1_ptr_, \
                           _tmp_param2_ptr_ ); \
  }

//---

#define _OSSS_METHOD3_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_, \
                                 _param_type2_, _param2_, \
                                 _param_type3_, _param3_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_ , \
                                 ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 3; */ \
    /* std::string  _params_type[3] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_, \
                              _param_type2_ _param2_, \
                              _param_type3_ _param3_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    _stub_##_method_name_( _tmp_param1_ptr_, \
                           _tmp_param2_ptr_, \
                           _tmp_param3_ptr_ ); \
  }

//---

#define _OSSS_METHOD4_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_, \
                                 _param_type2_, _param2_, \
                                 _param_type3_, _param3_, \
                                 _param_type4_, _param4_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_ , \
                                 ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 4; */ \
    /* std::string  _params_type[4] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_, \
                              _param_type2_ _param2_, \
                              _param_type3_ _param3_, \
                              _param_type4_ _param4_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    _stub_##_method_name_( _tmp_param1_ptr_, \
                           _tmp_param2_ptr_, \
                           _tmp_param3_ptr_, \
                           _tmp_param4_ptr_ ); \
  }

//---

#define _OSSS_METHOD5_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_, \
                                 _param_type2_, _param2_, \
                                 _param_type3_, _param3_, \
                                 _param_type4_, _param4_, \
                                 _param_type5_, _param5_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_ , \
                                 ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 5; */ \
    /* std::string  _params_type[5] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*"), */ \
    /*                                 std::string(#_param_type5_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    OSSS_SERIALISE_ARGUMENT( _param5_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    delete _param5_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_, \
                              _param_type2_ _param2_, \
                              _param_type3_ _param3_, \
                              _param_type4_ _param4_, \
                              _param_type5_ _param5_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _tmp_param5_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >(_param5_); \
    _stub_##_method_name_( _tmp_param1_ptr_, \
                           _tmp_param2_ptr_, \
                           _tmp_param3_ptr_, \
                           _tmp_param4_ptr_, \
                           _tmp_param5_ptr_ ); \
  }

//---

#define _OSSS_METHOD6_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_, \
                                 _param_type2_, _param2_, \
                                 _param_type3_, _param3_, \
                                 _param_type4_, _param4_, \
                                 _param_type5_, _param5_, \
                                 _param_type6_, _param6_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_ , \
                                 ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 6; */ \
    /* std::string  _params_type[6] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*"), */ \
    /*                                 std::string(#_param_type5_) + std::string("*"), */ \
    /*                                 std::string(#_param_type6_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    OSSS_SERIALISE_ARGUMENT( _param5_ ); \
    OSSS_SERIALISE_ARGUMENT( _param6_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    delete _param5_; \
    delete _param6_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_, \
                              _param_type2_ _param2_, \
                              _param_type3_ _param3_, \
                              _param_type4_ _param4_, \
                              _param_type5_ _param5_, \
                              _param_type6_ _param6_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _tmp_param5_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >(_param5_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _tmp_param6_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >(_param6_); \
    _stub_##_method_name_( _tmp_param1_ptr_, \
                           _tmp_param2_ptr_, \
                           _tmp_param3_ptr_, \
                           _tmp_param4_ptr_, \
                           _tmp_param5_ptr_, \
                           _tmp_param6_ptr_ ); \
  }

//---

#define _OSSS_METHOD7_VOID_STUB( _method_name_, \
                                 _param_type1_, _param1_, \
                                 _param_type2_, _param2_, \
                                 _param_type3_, _param3_, \
                                 _param_type4_, _param4_, \
                                 _param_type5_, _param5_, \
                                 _param_type6_, _param6_, \
                                 _param_type7_, _param7_ ) \
  VIRTUAL_METHOD( private, void, _stub_##_method_name_ , \
                                 ( ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _param1_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _param2_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _param3_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _param4_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _param5_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _param6_, \
                                   ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _param7_ ) ) \
  { \
    /* bool _hasReturnValue = false; */ \
    std::string  _method_name = std::string("_stub_") + std::string(#_method_name_); \
    /* unsigned int _num_params = 7; */ \
    /* std::string  _params_type[7] = { std::string(#_param_type1_) + std::string("*"), */ \
    /*                                 std::string(#_param_type2_) + std::string("*"), */ \
    /*                                 std::string(#_param_type3_) + std::string("*"), */ \
    /*                                 std::string(#_param_type4_) + std::string("*"), */ \
    /*                                 std::string(#_param_type5_) + std::string("*"), */ \
    /*                                 std::string(#_param_type6_) + std::string("*"), */ \
    /*                                 std::string(#_param_type7_) + std::string("*") }; */ \
    OSSS_PREPARE_METHOD_CALL; \
    OSSS_SERIALISE_ARGUMENT( _param1_ ); \
    OSSS_SERIALISE_ARGUMENT( _param2_ ); \
    OSSS_SERIALISE_ARGUMENT( _param3_ ); \
    OSSS_SERIALISE_ARGUMENT( _param4_ ); \
    OSSS_SERIALISE_ARGUMENT( _param5_ ); \
    OSSS_SERIALISE_ARGUMENT( _param6_ ); \
    OSSS_SERIALISE_ARGUMENT( _param7_ ); \
    delete _param1_; \
    delete _param2_; \
    delete _param3_; \
    delete _param4_; \
    delete _param5_; \
    delete _param6_; \
    delete _param7_; \
    OSSS_PROCEDURE_CALL; \
  } \
  public : \
  virtual void _method_name_( _param_type1_ _param1_, \
                              _param_type2_ _param2_, \
                              _param_type3_ _param3_, \
                              _param_type4_ _param4_, \
                              _param_type5_ _param5_, \
                              _param_type6_ _param6_, \
                              _param_type7_ _param7_ ) \
  { \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >::type_check(); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >* _tmp_param1_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type1_ >(_param1_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >* _tmp_param2_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type2_ >(_param2_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >* _tmp_param3_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type3_ >(_param3_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >* _tmp_param4_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type4_ >(_param4_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >* _tmp_param5_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type5_ >(_param5_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >* _tmp_param6_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type6_ >(_param6_); \
    ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >* _tmp_param7_ptr_ = \
      new ::osss::osssi::osss_serialisable_wrapper< _param_type7_ >(_param7_); \
    _stub_##_method_name_( _tmp_param1_ptr_, \
                           _tmp_param2_ptr_, \
                           _tmp_param3_ptr_, \
                           _tmp_param4_ptr_, \
                           _tmp_param5_ptr_, \
                           _tmp_param6_ptr_, \
                           _tmp_param7_ptr_ ); \
  }

//---

#define _OSSS_METHOD_VOID_STUB_TMP( _method_name_, N, ...) \
  _OSSS_METHOD##N##_VOID_STUB( _method_name_, ## __VA_ARGS__ )

#define OSSS_METHOD_VOID_STUB( _method_name_, ...) \
  _OSSS_METHOD_VOID_STUB_TMP( _method_name_, __VA_ARGS__ )

//==============================================================================

#define OSSS_EXPORTED(signature) signature

#endif
// $Id: osss_hw_macros_green.h 2762 2008-11-28 09:54:10Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
