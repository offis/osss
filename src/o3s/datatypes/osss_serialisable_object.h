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

#ifndef OSSS_SERIALISABLE_OBJECT_HH
#define OSSS_SERIALISABLE_OBJECT_HH

#include "o3s/utils/osss_systemc.h"
#include "o3s/utils/osss_report.h"
#include "o3s/extern/reflection/reflection.h"

#include "o3s/datatypes/osss_serialisable_macros.h"
#include "o3s/utils/osss_report.h"

namespace osss {

//------------------------------------------------------------------------------

#define OSSS_SERIALISABLE_OBJECT_SYNTH

//------------------------------------------------------------------------------
// Size helper classes
//------------------------------------------------------------------------------

//general case
template<class T>
struct SizeHelper
{
  static size_t getSize() 
  { 
    std::cerr << "Error in " << __PRETTY_FUNCTION__
              << ": no such type" << std::endl;
    OSSS_ABORT();
    // return guessed size
    return sizeof(T)*8; 
  }
};

//special case macro for non-templated types
#define SIZE_HELPER_FOR_BASIC_TYPE(_type_) \
  template<> \
  struct SizeHelper<_type_ > \
  { \
    static size_t getSize() { return sizeof(_type_)*8; } \
  };

//special case macro for templated types
#define SIZE_HELPER_FOR_TEMPLATED_TYPE(_type_) \
  template<int W> \
  struct SizeHelper<_type_<W> > \
  { \
    static size_t getSize() { return W; } \
  };

#define GET_SIZE(_type_) (SizeHelper<_type_>::getSize())

//=====================================================
// C++ built-in types:
//
// interger types (singned & unsigned) are:
//
// type      | size (in bytes) | byte alignment
// ------------------------------------------------
// char      | 1               | 1
// short     | 2               | 2
// int       | 2 or 4          | 2 or 4
// long      | 4 or 8          | 4 or 8
// long long | 8 or 16         | 8 or 16
//           |                 |
// bool      | 1 bit           | 1
//
// floating types are:
//
// type        | size (in bytes) | byte alignment
// -----------------------------------------------
// float       | 4               | 4
// double      | 8               | 4 or 8
// long double | 12 or 16        | 4 or 8 or 16
//
//=====================================================

SIZE_HELPER_FOR_BASIC_TYPE(char)
SIZE_HELPER_FOR_BASIC_TYPE(unsigned char)
SIZE_HELPER_FOR_BASIC_TYPE(signed char)
SIZE_HELPER_FOR_BASIC_TYPE(short) // == signed short
SIZE_HELPER_FOR_BASIC_TYPE(unsigned short)
SIZE_HELPER_FOR_BASIC_TYPE(int) // == signed int
SIZE_HELPER_FOR_BASIC_TYPE(unsigned int)
SIZE_HELPER_FOR_BASIC_TYPE(long) // == signed long
SIZE_HELPER_FOR_BASIC_TYPE(unsigned long)
SIZE_HELPER_FOR_BASIC_TYPE(long long) // == signed long long
SIZE_HELPER_FOR_BASIC_TYPE(unsigned long long)

//bool - since the vector takes boolean values return the size of 
//       Bytes (1) instead of bit
template<>
struct SizeHelper<bool>
{
  static size_t getSize() { return sizeof(bool)*8; }
};

SIZE_HELPER_FOR_BASIC_TYPE(float)
SIZE_HELPER_FOR_BASIC_TYPE(double)
SIZE_HELPER_FOR_BASIC_TYPE(long double)

//==============================================================================
// SystemC built-in types
//
// type                       | size (in bit)                       | remark
// -----------------------------------------------------------------------------
// sc_bit                     | 1                                   |
// sc_bv<int W>               | W                                   |
// sc_logic                   | 2                                   | no support
// sc_lv<int W>               | 2*W                                 | no support
// sc_fix                     | sizeof(sc_fix)*8                    | not synth
// sc_fixed<int W, ...>       | W                                   |
// sc_fix_fast                | sizeof(sc_fix_fast)*8               | not synth
// sc_fixed_fast<int W, ...>  | W [1-53]                            | not synth
// sc_ufix                    | sizeof(sc_ufix)*8                   | not synth
// sc_ufixed<int W, ...>      | W                                   |
// sc_ufix_fast               | sizeof(sc_ufix_fast)*8              | not synth
// sc_ufixed_fast<int W, ...> | W [1-53]                            | not synth
// sc_bigint<int W>           | W                                   |
// sc_biguint<int W>          | W                                   |
// sc_int<int W>              | W [1-64]                            |
// sc_uint<int W>             | W [1-64]                            |
// 
//==============================================================================

//sc_bit
template<>
struct SizeHelper<sc_dt::sc_bit>
{
  static size_t getSize() { return 1; }
};

SIZE_HELPER_FOR_TEMPLATED_TYPE(sc_dt::sc_bv)

/* 
to do:

  // sc_logic
  template<>
  struct SizeHelper<sc_logic>
  {
    static size_t getSize() { return 2; }
  };

  //sc_lv
  template<int W>
  struct SizeHelper<sc_lv<W> >
  {
    static size_t getSize() { return (2*W); }
  };
*/

#ifdef SC_INCLUDE_FX

#ifndef OSSS_SERIALISABLE_OBJECT_SYNTH
  SIZE_HELPER_FOR_BASIC_TYPE(sc_dt::sc_fix)
  SIZE_HELPER_FOR_BASIC_TYPE(sc_dt::sc_fix_fast)
  SIZE_HELPER_FOR_BASIC_TYPE(sc_dt::sc_fixed_fast)
  SIZE_HELPER_FOR_BASIC_TYPE(sc_dt::sc_ufix)
  SIZE_HELPER_FOR_BASIC_TYPE(sc_dt::sc_ufix_fast)
  SIZE_HELPER_FOR_BASIC_TYPE(sc_dt::sc_ufixed_fast)
#endif // OSSS_SERIALISABLE_OBJECT_SYNTH

  //sc_fixed
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N>
  struct SizeHelper<sc_dt::sc_fixed<W,I,Q,O,N> >
  {
    static size_t getSize() { return W; }
  };

  //sc_ufixed
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N>
  struct SizeHelper<sc_dt::sc_ufixed<W,I,Q,O,N> >
  {
    static size_t getSize() { return W; }
  };

#endif // SC_INCLUDE_FX

SIZE_HELPER_FOR_TEMPLATED_TYPE(sc_dt::sc_bigint)
SIZE_HELPER_FOR_TEMPLATED_TYPE(sc_dt::sc_biguint)

//sc_int
template<int W>
struct SizeHelper<sc_dt::sc_int<W> >
{
  static size_t getSize() 
  { 
    if (W > 64)
      return 64; 
    else
      return W;
  }
};

//sc_uint
template<int W>
struct SizeHelper<sc_dt::sc_uint<W> >
{
  static size_t getSize()
  { 
    if (W > 64)
      return 64; 
    else
      return W;
  }
};

#undef SIZE_HELPER_FOR_BASIC_TYPE
#undef SIZE_HELPER_FOR_TEMPLATED_TYPE

//##############################################################################

//------------------------------------------------------------------------------
// bit helper; to set/get the Bits of a type T 
//------------------------------------------------------------------------------

//general case
template<class T>
struct BitHelper
{
  static void setBit(T &value, size_t BitNumber, bool bitValue) 
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__
              << ": no such type" << std::endl;
    OSSS_ABORT();
  }
  static bool getBit(T &value, size_t BitNumber) 
  { 
    std::cerr << "Error in " << __PRETTY_FUNCTION__
              << ": no such type" << std::endl;
    OSSS_ABORT();
    return false;
  }
};

#define BIT_HELPER_FOR_BASIC_TYPE1(_type_) \
  template<> \
  struct BitHelper<_type_ > \
  { \
    static void setBit(_type_ &value, size_t BitNumber, bool bitValue) \
    { \
      _type_ mask = 1; \
      mask = mask << BitNumber; \
      if (bitValue == true) \
        value = value | mask; \
      else \
        value = value & ~(value & mask); \
    } \
    static bool getBit(_type_ &value, size_t BitNumber) \
    { \
      _type_ mask = 1; \
      mask = mask << BitNumber; \
      return (value & mask); \
    } \
  };

#define BIT_HELPER_FOR_BASIC_TYPE2(_type_) \
  template<> \
  struct BitHelper<_type_ > \
  { \
    static void setBit(_type_ &value, size_t BitNumber, bool bitValue) \
    { \
      size_t count = BitNumber/(sizeof(int)*8); \
      _type_ *pointerOfValue = &value; \
      int *pointerOfInt = reinterpret_cast<int*>(pointerOfValue); \
      if (BitNumber >= (sizeof(int)*8)) \
      { \
        for (int i=count; i>0; i--) \
          pointerOfInt++; \
        BitNumber = BitNumber % (sizeof(int)*8); \
      } \
      BitHelper<int>::setBit(*pointerOfInt, BitNumber, bitValue); \
    } \
    static bool getBit(_type_ &value, size_t BitNumber) \
    { \
      size_t count = BitNumber/(sizeof(int)*8); \
      _type_ *pointerOfValue = &value; \
      int *pointerOfInt = reinterpret_cast<int*>(pointerOfValue); \
      if (BitNumber >= (sizeof(int)*8)) \
      { \
        for (int i=count; i>0; i--) \
          pointerOfInt++; \
        BitNumber = BitNumber % (sizeof(int)*8); \
      } \
      return (BitHelper<int>::getBit(*pointerOfInt, BitNumber)); \
    } \
  };

#define BIT_HELPER_FOR_SYSTEMC_TYPE(_type_) \
  template<> \
  struct BitHelper<_type_ > \
  { \
    static void setBit(_type_ &value, size_t BitNumber, bool bitValue) \
    { \
      value[BitNumber] = bitValue; \
    } \
    static bool getBit(_type_ &value, size_t BitNumber) \
    { \
      return value[BitNumber]; \
    } \
  };

#define BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE(_type_) \
  template<int W> \
  struct BitHelper<_type_<W> > \
  { \
    static void setBit(_type_<W> &value, size_t BitNumber, bool bitValue) \
    { \
      value[BitNumber] = bitValue; \
    } \
    static bool getBit(_type_<W> &value, size_t BitNumber) \
    { \
      return value[BitNumber]; \
    } \
  };

//=====================================================
// C++ built-in types:
//=====================================================

BIT_HELPER_FOR_BASIC_TYPE1(char)
BIT_HELPER_FOR_BASIC_TYPE1(unsigned char)
BIT_HELPER_FOR_BASIC_TYPE1(signed char)
BIT_HELPER_FOR_BASIC_TYPE1(short) // == signed short
BIT_HELPER_FOR_BASIC_TYPE1(unsigned short)
BIT_HELPER_FOR_BASIC_TYPE1(int) // == signed int
BIT_HELPER_FOR_BASIC_TYPE1(unsigned int)
BIT_HELPER_FOR_BASIC_TYPE1(long) // == signed long
BIT_HELPER_FOR_BASIC_TYPE1(unsigned long)
BIT_HELPER_FOR_BASIC_TYPE1(long long) // == signed long long
BIT_HELPER_FOR_BASIC_TYPE1(unsigned long long)

//bool
template<>
struct BitHelper<bool>
{
   static void setBit(bool &value, size_t BitNumber, bool bitValue)
   {
     value = bitValue;
   }

   static bool getBit(bool &value, size_t BitNumber)
   {
     return value;
   }
};

BIT_HELPER_FOR_BASIC_TYPE2(float)
BIT_HELPER_FOR_BASIC_TYPE2(double)
BIT_HELPER_FOR_BASIC_TYPE2(long double)

//==============================================================================
// SystemC built-in types
//
// type                       | size (in bit)                       | remark
// -----------------------------------------------------------------------------
// sc_bit                     | 1                                   |
// sc_bv<int W>               | W                                   |
// sc_logic                   | 2                                   | no support
// sc_lv<int W>               | 2*W                                 | no support
// sc_fix                     | sizeof(sc_fix)*8                    | not synth
// sc_fixed<int W, ...>       | W                                   |
// sc_fix_fast                | sizeof(sc_fix_fast)*8               | not synth
// sc_fixed_fast<int W, ...>  | W [1-53]                            | not synth
// sc_ufix                    | sizeof(sc_ufix)*8                   | not synth
// sc_ufixed<int W, ...>      | W                                   |
// sc_ufix_fast               | sizeof(sc_ufix_fast)*8              | not synth
// sc_ufixed_fast<int W, ...> | W [1-53]                            | not synth
// sc_bigint<int W>           | W                                   |
// sc_biguint<int W>          | W                                   |
// sc_int<int W>              | W [1-64]                            |
// sc_uint<int W>             | W [1-64]                            |
// 
//==============================================================================

//sc_bit
template<>
struct BitHelper<sc_dt::sc_bit>
{
  static void setBit(sc_dt::sc_bit &value, size_t BitNumber, bool bitValue)
  {
    value = bitValue;
  }

  static bool getBit(sc_dt::sc_bit &value, size_t BitNumber)
  {
    return value.to_bool();
  }
};

BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)

/*
TODO:

//sc_logic
//sc_lv

*/

#ifdef SC_INCLUDE_FX

#ifndef OSSS_SERIALISABLE_OBJECT_SYNTH
  BIT_HELPER_FOR_SYSTEMC_TYPE(sc_dt::sc_fix)
  BIT_HELPER_FOR_SYSTEMC_TYPE(sc_dt::sc_fix_fast)
  BIT_HELPER_FOR_SYSTEMC_TYPE(sc_dt::sc_fixed_fast)
  BIT_HELPER_FOR_SYSTEMC_TYPE(sc_dt::sc_ufix)
  BIT_HELPER_FOR_SYSTEMC_TYPE(sc_dt::sc_ufix_fast)
  BIT_HELPER_FOR_SYSTEMC_TYPE(sc_dt::sc_ufixed_fast)
#endif // OSSS_SERIALISABLE_OBJECT_SYNTH

  //sc_fixed
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N>
  struct BitHelper<sc_dt::sc_fixed<W,I,Q,O,N> >
  {
    static void setBit(sc_dt::sc_fixed<W,I,Q,O,N> &value, 
		       size_t BitNumber, bool bitValue)
    {
      value[BitNumber] = bitValue;
    }
    static bool getBit(sc_dt::sc_fixed<W,I,Q,O,N> &value, size_t BitNumber)
    {
      return value[BitNumber];
    }
  };

  //sc_ufixed
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N>
  struct BitHelper<sc_dt::sc_ufixed<W,I,Q,O,N> >
  {
    static void setBit(sc_dt::sc_ufixed<W,I,Q,O,N> &value, 
		       size_t BitNumber, bool bitValue)
    {
      value[BitNumber] = bitValue;
    }
    static bool getBit(sc_dt::sc_ufixed<W,I,Q,O,N> &value, size_t BitNumber)
    {
      return value[BitNumber];
    }
  };

#endif // SC_INCLUDE_FX

BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#undef BIT_HELPER_FOR_BASIC_TYPE1
#undef BIT_HELPER_FOR_BASIC_TYPE2
#undef BIT_HELPER_FOR_SYSTEMC_TYPE
#undef BIT_HELPER_FOR_TEMPLATED_SYSTEMC_TYPE

//##############################################################################

#define BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE( _type_ ) \
  osss_serialisable_object(const _type_ & value) \
  { \
    m_osss_bitVector.clear(); \
    store_element(value); \
    isSerialised = true; \
    iter2 = m_osss_bitVector.begin(); \
  }

#define BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
  template<int W> \
  osss_serialisable_object( const _type_<W> & value ) \
  { \
    m_osss_bitVector.clear(); \
    store_element(value); \
    isSerialised = true; \
    iter2 = m_osss_bitVector.begin(); \
  }

#define BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  osss_serialisable_object( const _type_<W,I,Q,O,N> & value ) \
  { \
    m_osss_bitVector.clear(); \
    store_element(value); \
    isSerialised = true; \
    iter2 = m_osss_bitVector.begin(); \
  }

//##############################################################################

#define BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE( _type_ ) \
  osss_serialisable_object& operator=(const _type_ & value) \
  { \
    m_osss_bitVector.clear(); \
    store_element(value); \
    isSerialised = true; \
    iter2 = m_osss_bitVector.begin(); \
    return *this; \
  }

#define BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
  template<int W> \
  osss_serialisable_object& operator=(const _type_<W> & value) \
  { \
    m_osss_bitVector.clear(); \
    store_element(value); \
    isSerialised = true; \
    iter2 = m_osss_bitVector.begin(); \
    return *this; \
  }

#define BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  osss_serialisable_object& operator=(const _type_<W,I,Q,O,N> & value) \
  { \
    m_osss_bitVector.clear(); \
    store_element(value); \
    isSerialised = true; \
    iter2 = m_osss_bitVector.begin(); \
    return *this; \
  }

//##############################################################################

#define BUILD_STORE_ELEMENT_FOR_BASIC_TYPE( _type_ ) \
  void store_element( _type_ value) \
  { \
    size_t bitWidth = GET_SIZE( _type_ ); \
    for(size_t i=0; i<bitWidth; i++) \
    { \
      m_osss_bitVector.push_back( BitHelper< _type_ >::getBit(value, i) ); \
    } \
  }

#define BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
  template<int W> \
  void store_element( _type_<W> value) \
  { \
    size_t bitWidth = GET_SIZE( _type_<W> ); \
    for(size_t i=0; i<bitWidth; i++) \
    { \
      m_osss_bitVector.push_back( BitHelper< _type_<W> >::getBit(value, i) ); \
    } \
  }

#define BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  void store_element( _type_<W,I,Q,O,N> value) \
  {\
    size_t bitWidth = SizeHelper< _type_<W,I,Q,O,N> >::getSize(); \
    for(size_t i=0; i<bitWidth; i++) \
    { \
      m_osss_bitVector.push_back( BitHelper< _type_<W,I,Q,O,N> >::getBit(value, i) ); \
    } \
  }

//#############################################################################

#define BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE( _type_ ) \
  void restore_element( _type_ &value) \
  { \
    _type_ result = 0; \
    size_t bitWidth = GET_SIZE( _type_ ); \
    bool tmp; \
    for ( size_t i=0; i < bitWidth; i++ ) \
    { \
      tmp = *iter2; \
      BitHelper< _type_ >::setBit(result, i, tmp); \
      iter2++; \
    } \
    value = result; \
  }

#define BUILD_RESTORE_ELEMENT_FOR_NON_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
  void restore_element( _type_ &value) \
  { \
    _type_ result = _type_(); \
    size_t bitWidth = GET_SIZE( _type_ ); \
    bool tmp; \
    for ( size_t i=0; i < bitWidth; i++ ) \
    { \
      tmp = *iter2; \
      BitHelper< _type_ >::setBit(result, i, tmp); \
      iter2++; \
    } \
    value = result; \
  }

#define BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
  template<int W> \
  void restore_element( _type_<W> &value) \
  { \
    _type_<W> result = _type_<W>(); \
    size_t bitWidth = GET_SIZE( _type_<W> ); \
    bool tmp; \
    for ( size_t i=0; i < bitWidth; i++ ) \
    { \
      tmp = *iter2; \
      BitHelper< _type_<W> >::setBit(result, i, tmp); \
      iter2++; \
    } \
    value = result; \
  }

#define BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  void restore_element( _type_<W,I,Q,O,N> &value) \
  { \
    _type_<W,I,Q,O,N> result = _type_<W,I,Q,O,N>(); \
    size_t bitWidth = SizeHelper< _type_<W,I,Q,O,N> >::getSize(); \
    bool tmp; \
    for ( size_t i=0; i < bitWidth; i++ ) \
    { \
      tmp = *iter2; \
      BitHelper< _type_<W,I,Q,O,N> >::setBit(result, i, tmp); \
      iter2++; \
    } \
    value = result; \
  }

//==============================================================================
// class osss_serialisable_object
//==============================================================================

namespace osssi {
  template<typename T>
  class osss_serialisable_wrapper;
}

/**
 * \class osss_serialisable_object
 *
 * \brief Base class for all user-defined classes that should be serialised
 *        into a bitvector representation.
 *
 * This class provides services for writing and reading members of user-defined
 * classes to a bitvector. It is used for the serialisation of parameters of
 * method calls when using the OSSS RMI (Remote Method Invocation) on the
 * Virtual Target Architecture Layer.
 *
 */
class osss_serialisable_object
{

  friend class osss_serialisable_archive;
  template<typename T> friend class ::osss::osssi::osss_serialisable_wrapper;

  friend
  std::ostream &
  operator<<( std::ostream & os, const osss_serialisable_object& ser_obj );

public:
  CLASS_NEW(osss_serialisable_object, NullClass);

  // default constructor
  CONSTRUCTOR(public, osss_serialisable_object, ());

  // copy constructor
  osss_serialisable_object(const osss_serialisable_object & object);

  // copy constructor for all supported basic C/C++ data types
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(char)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(unsigned char)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(signed char)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(short) // == signed short
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(unsigned short)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(int) // == signed int
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(unsigned int)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(long) // == signed long
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(unsigned long)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(long long) // == signed long long
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(unsigned long long)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(bool)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(float)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(double)
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(long double)

  // copy constructor for all supported SystemC data types
  BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE(sc_dt::sc_bit)
  BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
  BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
  BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
  BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
  BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
    BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
    BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

  // assignment operator
  osss_serialisable_object& operator=(const osss_serialisable_object & object);

  // assignment operator for all supported basic C/C++ data types
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(char)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(unsigned char)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(signed char)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(short) // == signed short
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(unsigned short)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(int) // == signed int
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(unsigned int)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(long) // == signed long
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(unsigned long)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(long long) // == signed long long
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(unsigned long long)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(bool)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(float)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(double)
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(long double)

  // assignment operator for all supported SystemC data types
  BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE(sc_dt::sc_bit)
  BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
  BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
  BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
  BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
  BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
    BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
    BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

  virtual ~osss_serialisable_object() {}

  /**
   * Prints the bitvector representation to stdout.
   * 
   * /warning this method is deprecated use operator<< instead
   * /see operator<<()
   *
   */
  void print_vector();

  unsigned int size() const
  {
    return m_osss_bitVector.size();
  }

  //--------------------------------------------------------
  // these methods are used inside transactors only
  //--------------------------------------------------------

  virtual void serialise_obj();
  virtual void deserialise_obj();

  /**
   * Reads a chunk of data from the bitvector representation and 
   * writes it to the specified port.
   * Template type defines the size of the chunk.
   */
  template<class T>
  void write_chunk_to_port(sc_core::sc_out< T >& out_port);
 
  /**
   * Reads a chunk of data from the bitvector representation and 
   * writes it to the specified port.
   * Template type defines the size of the chunk.
   */
  template<class T>
  void write_chunk_to_port(sc_core::sc_inout< T >& inout_port);
 
  /**
   * Reads a chunk of data from the bitvector representation and 
   * returns it.
   * Template type defines the size of the chunk.
   */
  template<class T>
  T read_chunk();

  /**
   * Reads value from the specified port and appends it to
   * the internal bitvector representation.
   */
  template<class T>
  void read_chunk_from_port(sc_core::sc_in< T >& in_port);
 
  /**
   * Reads value from the specified port and appends it to
   * the internal bitvector representation.
   */
  template<class T>
  void read_chunk_from_port(sc_core::sc_inout< T >& inout_port);

  /**
   * Reads specified value and appends it to
   * the internal bitvector representation.
   */
  template<class T>
  void write_chunk(T value); 
  
  bool complete();
   
  void clear();

protected:

  std::vector<bool> m_osss_bitVector;
  std::vector<bool>::iterator iter2;

  bool isSerialised;

  //------------------------------------------------------------------
  // these methods need to be implemented inside user-defined classes
  //------------------------------------------------------------------

  // due to C++ reflection-lib this class can not be
  // pure virtual (virtual void serialise() = 0)
  virtual void serialise() {};

  // due to C++ reflection-lib this class can not be
  // pure virtual (virtual void deserialise() = 0)
  virtual void deserialise() {};

  //------------------------------------------------------------------
  // these methods need to be used to store/restore members 
  // the store_ methods are only allowed to be used inside the 
  // serialise method
  // the restore_ methods are only allowed to be used inside the
  // deserialise methods
  //------------------------------------------------------------------

  //------------------------
  // store_ methods
  //------------------------

  /*
  template<class T>
  void store_element(T value);
  */
  // store_element method for all supported basic C/C++ data types
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(char)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(unsigned char)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(signed char)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(short) // == signed short
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(unsigned short)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(int) // == signed int
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(unsigned int)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(long) // == signed long
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(unsigned long)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(long long) // == signed long long
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(unsigned long long)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(bool)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(float)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(double)
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(long double)

  // store_element method for all supported SystemC data types
  BUILD_STORE_ELEMENT_FOR_BASIC_TYPE(sc_dt::sc_bit)
  BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
  BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
  BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
  BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
  BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
    BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
    BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

  // for (user defined) serialisable objects
  void store_element(osss_serialisable_object &ser_obj);

  // for arrays
  template<class T>
  void store_array(T *beginArray, unsigned int arrayLength);
 
  //------------------------
  // restore_ methods
  //------------------------

  /*
  void restore_element(sc_dt::sc_bit &value);

  template<class T>
  void restore_element(T &value);
  */
  // restore_element method for all supported basic C/C++ data types
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(char)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(unsigned char)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(signed char)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(short) // == signed short
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(unsigned short)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(int) // == signed int
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(unsigned int)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(long) // == signed long
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(unsigned long)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(long long) // == signed long long
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(unsigned long long)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(bool)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(float)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(double)
  BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE(long double)

  // restore_element method for all supported SystemC data types
  BUILD_RESTORE_ELEMENT_FOR_NON_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bit)
  BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
  BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
  BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
  BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
  BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
    BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
    BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

  // for (user defined) serialisable objects
  void restore_element(osss_serialisable_object &ser_obj);
  
  // for arrays
  template<class T>
  void restore_array(T *beginArray, unsigned int arrayLength);
};

} // namespace osss

#include "o3s/datatypes/osss_serialisable_object.tpp"

#endif
// $Id: osss_serialisable_object.h 2840 2008-12-04 09:30:05Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
