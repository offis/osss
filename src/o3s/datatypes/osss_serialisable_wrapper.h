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

#ifndef OSSS_SERIALISABLE_WRAPPER_HH
#define OSSS_SERIALISABLE_WRAPPER_HH

#include <iosfwd>
#include "o3s/datatypes/osss_serialisable_archive.h"

namespace osss {

namespace osssi {

//----------------------------------------------------------------------------
// osss_serialisable_object and objects derived from it 
// (i.e. osss_enum<T>, osss_array<T,size>, user defined classes derived 
// from osss_serialisable_object)
//----------------------------------------------------------------------------

template<typename T>
class osss_serialisable_wrapper : public osss_serialisable_object
{

  typedef osss_serialisable_object base_type;

public:
  CLASS( osss_serialisable_wrapper, osss_serialisable_object );

  osss_serialisable_wrapper() : osss_serialisable_object()
  {
    if (!isSerialisable())
    {
      generateError();
    }
  }

  osss_serialisable_wrapper(const T& _value)
  {
    if (isSerialisable())
    {
      value = _value;
      value.serialise_obj();
      copyBitVector();
    }
    else
    {
      generateError();
    }
  }

  osss_serialisable_wrapper(const osss_serialisable_wrapper& _ser_wrapper)
  {
    value = _ser_wrapper.value;
    value.serialise_obj();
    copyBitVector();
  }

  CONSTRUCTOR(public, osss_serialisable_wrapper, (osss_serialisable_archive* arch))
  {
    if (isSerialisable())
    {
      arch->rebuild(value);
      copyBitVector();
    }
    else
    {
      generateError();
    }
  }

  osss_serialisable_wrapper& operator=(const osss_serialisable_wrapper& _wrapper)
  {
    if (this == &_wrapper)
      return *this;

    value = _wrapper.value;
    value.serialise_obj();
    copyBitVector();  

    return *this;
  }

  osss_serialisable_wrapper& operator=(const T& _value)
  {
    value = _value;
    value.serialise_obj();
    copyBitVector();

    return *this;
  }

  operator T& ()
  {
    this->deserialise_obj();
    return value;
  }

  static void type_check() {}

  ///////////////////////////////////////////////////////////////////////
  // overwrite the functionality of osss_serialisable_object
  ///////////////////////////////////////////////////////////////////////

  virtual void serialise_obj()
  {
    value.serialise_obj();
    copyBitVector();
  }

  virtual void deserialise_obj()
  {
    restoreBitVector();
    value.deserialise_obj();
  }

protected:

  bool isSerialisable()
  {
    // check if value is an instance of osss_serialisable_object
    osss_serialisable_object* check_obj = 
      dynamic_cast<osss_serialisable_object*>(&value);
    return (check_obj != NULL);
  }

  void generateError()
  {
    std::cerr << "This type is neither a C++ nor a SystemC built-in type"
              << " and no instance of osss_serialisable_object."
              << std::endl;
    sc_core::sc_stop();
  }

  void copyBitVector()
  {
    // copy all members from _value to the base class
    base_type::m_osss_bitVector = value.m_osss_bitVector;
    base_type::iter2 = base_type::m_osss_bitVector.begin();
    base_type::isSerialised = true;
  }

  void restoreBitVector()
  {
    value.m_osss_bitVector = base_type::m_osss_bitVector;
  }

  T value;
  
};

/*
 * partial specialisation for pointers
 * 
 * pointers are not supported
 */
template<typename T>
class osss_serialisable_wrapper<T*>
{
public:
  
  static void type_check()
  {
    std::cerr << "Error using: " 
              << __PRETTY_FUNCTION__ << std::endl
              << "\t Pointer types are not supported." << std::endl;
    OSSS_ABORT();
  }

};

/*
 * partial specialisation for references
 *
 * references are not supported
 */
template<typename T>
class osss_serialisable_wrapper<T&>
{
public:

  static void type_check()
  {
    std::cerr << "Error using: " 
	 << __PRETTY_FUNCTION__ << std::endl
	 << "\t Reference types are not supported." << std::endl;
    OSSS_ABORT();
  }

};

//------------------------------------------------------------------------------

#define BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE( _type_ ) \
  inline ::std::ostream& \
           operator<<(::std::ostream& s, \
                      const class osss_serialisable_wrapper<_type_>& x); \
  inline ::std::istream& \
           operator>>(::std::istream& s, \
                      class osss_serialisable_wrapper<_type_>& x); \
  \
  template<> \
  class osss_serialisable_wrapper< _type_ > : public osss_serialisable_object \
  { \
  public: \
    CLASS( osss_serialisable_wrapper, osss_serialisable_object ); \
    osss_serialisable_wrapper () : osss_serialisable_object() \
    { \
      serialise_obj(); \
    } \
    osss_serialisable_wrapper(const _type_& _value) \
    { \
      value = _value; \
      serialise_obj(); \
    } \
    osss_serialisable_wrapper(const osss_serialisable_wrapper& _ser_wrapper) : osss_serialisable_object(_ser_wrapper) \
    { \
      value = _ser_wrapper.value; \
      serialise_obj(); \
    } \
    CONSTRUCTOR(public, osss_serialisable_wrapper, (osss_serialisable_archive* arch)) \
    { \
      arch->rebuild(*this); \
    } \
    osss_serialisable_wrapper& operator= \
      (const osss_serialisable_wrapper& _wrapper) \
    { \
      if (this == &_wrapper) \
        return *this; \
      value = _wrapper.value; \
      serialise_obj(); \
      return *this; \
    } \
    osss_serialisable_wrapper& operator=(const _type_& _value) \
    { \
      value = _value; \
      serialise_obj(); \
      return *this; \
    } \
    operator _type_& () \
    { \
      deserialise_obj(); \
      return value; \
    } \
    static void type_check() {} \
    virtual void serialise() \
    { \
      store_element(value); \
    } \
    virtual void deserialise() \
    { \
      restore_element(value); \
    } \
  protected: \
    _type_ value; \
  friend ::std::ostream& \
           operator<<(::std::ostream& s, \
                      const class osss_serialisable_wrapper<_type_>& x); \
  friend ::std::istream& \
           operator>>(::std::istream& s, \
                      class osss_serialisable_wrapper<_type_>& x); \
  }; \
  \
  ::std::ostream& operator<<(::std::ostream& s, \
                             const class osss_serialisable_wrapper<_type_>& x)\
  { s << x.value; return s; } \
  ::std::istream& operator>>(::std::istream& s, \
                             class osss_serialisable_wrapper<_type_>& x) \
  { s >> x.value; return s; }


//------------------------------------------------------------------------------

#define BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
  template<int W> inline ::std::ostream& \
  operator<< (::std::ostream& s, \
              const class osss_serialisable_wrapper<_type_<W> >& x); \
  template<int W> inline ::std::istream& \
  operator>> (::std::istream& s, \
              const class osss_serialisable_wrapper<_type_<W> >& x); \
  \
  template<int W> \
  class osss_serialisable_wrapper< _type_<W> > \
    : public osss_serialisable_object \
  { \
  public: \
    CLASS( osss_serialisable_wrapper, osss_serialisable_object ); \
    osss_serialisable_wrapper() : osss_serialisable_object() \
    { \
      serialise_obj(); \
    } \
    osss_serialisable_wrapper(const _type_<W>& _value) \
    { \
      value = _value; \
      serialise_obj(); \
    } \
    osss_serialisable_wrapper(const osss_serialisable_wrapper& _ser_wrapper) \
    { \
      value = _ser_wrapper.value; \
      serialise_obj(); \
    } \
    CONSTRUCTOR(public, osss_serialisable_wrapper, (osss_serialisable_archive* arch)) \
    { \
      arch->rebuild(*this); \
    } \
    osss_serialisable_wrapper& operator= \
      (const osss_serialisable_wrapper& _wrapper) \
    { \
      if (this == &_wrapper) \
        return *this; \
      value = _wrapper.value; \
      serialise_obj(); \
      return *this; \
    } \
    osss_serialisable_wrapper& operator=(const _type_<W>& _value) \
    { \
      value = _value; \
      serialise_obj(); \
      return *this; \
    } \
    operator _type_<W>& () \
    { \
      deserialise_obj(); \
      return value; \
    } \
    static void type_check() {} \
    virtual void serialise() \
    { \
      store_element(value); \
    } \
    virtual void deserialise() \
    { \
      restore_element(value); \
    } \
  protected: \
    _type_<W> value; \
  friend std::ostream& \
     operator<< <W>(std::ostream& s, \
                const osss_serialisable_wrapper<_type_<W> >& x); \
  friend std::istream& \
     operator>> <W>(std::istream& s, \
                const osss_serialisable_wrapper<_type_<W> >& x); \
  }; \
  \
  template<int W> ::std::ostream& \
  operator<< (::std::ostream& s, \
              const class osss_serialisable_wrapper<_type_<W> >& x) \
  { s << x.value; return s; } \
  template<int W> ::std::istream& \
  operator>> (::std::istream& s, \
              const class osss_serialisable_wrapper<_type_<W> >& x) \
  { s >> x.value; return s; }

//------------------------------------------------------------------------------

#define BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  inline ::std::ostream& \
  operator<< (::std::ostream& s, \
              const class osss_serialisable_wrapper<_type_<W,I,Q,O,N> >& x); \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  inline ::std::istream& \
  operator>> (::std::istream& s, \
              const class osss_serialisable_wrapper<_type_<W,I,Q,O,N> >& x); \
  \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  class osss_serialisable_wrapper< _type_<W,I,Q,O,N> > \
    : public osss_serialisable_object \
  { \
  public: \
    CLASS( osss_serialisable_wrapper, osss_serialisable_object ); \
    osss_serialisable_wrapper() : osss_serialisable_object() \
    { \
      serialise_obj(); \
    } \
    osss_serialisable_wrapper(const _type_<W,I,Q,O,N>& _value) \
    { \
      value = _value; \
      serialise_obj(); \
    } \
    osss_serialisable_wrapper(const osss_serialisable_wrapper& _ser_wrapper) \
    { \
      value = _ser_wrapper.value; \
      serialise_obj(); \
    } \
    CONSTRUCTOR(public, osss_serialisable_wrapper, (osss_serialisable_archive* arch)) \
    { \
      arch->rebuild(*this); \
    } \
    osss_serialisable_wrapper& operator= \
      (const osss_serialisable_wrapper& _wrapper) \
    { \
      if (this == &_wrapper) \
        return *this; \
      value = _wrapper.value; \
      serialise_obj(); \
      return *this; \
    } \
    osss_serialisable_wrapper& operator=(const _type_<W,I,Q,O,N>& _value) \
    { \
      value = _value; \
      serialise_obj(); \
      return *this; \
    } \
    operator _type_<W,I,Q,O,N>& () \
    { \
      deserialise_obj(); \
      return value; \
    } \
    static void type_check() {} \
    virtual void serialise() \
    { \
      store_element(value); \
    } \
    virtual void deserialise() \
    { \
      restore_element(value); \
    } \
  protected: \
    _type_<W,I,Q,O,N> value; \
  friend std::ostream& \
     operator<< <W,I,Q,O,N>(std::ostream& s, \
                const osss_serialisable_wrapper<_type_<W,I,Q,O,N> >& x); \
  friend std::istream& \
     operator>> <W,I,Q,O,N>(std::istream& s, \
                const osss_serialisable_wrapper<_type_<W,I,Q,O,N> >& x); \
  }; \
  \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  ::std::ostream& \
  operator<< (::std::ostream& s, \
              const class osss_serialisable_wrapper<_type_<W,I,Q,O,N> >& x) \
  { s << x.value; return s; } \
  template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
  ::std::istream& \
  operator>> (::std::istream& s, \
              const class osss_serialisable_wrapper<_type_<W,I,Q,O,N> >& x) \
  { s >> x.value; return s; }

//------------------------------------------------------------------------------

BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(char)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(unsigned char)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(signed char)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(short) // == signed short
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(unsigned short)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(int) // == signed int
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(unsigned int)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(long) // == signed long
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(unsigned long)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(long long) // == signed long long
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(unsigned long long)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(bool)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(float)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(double)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(long double)

BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE(sc_dt::sc_bit)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

//------------------------------------------------------------------------------

template<class ElementType, unsigned int Size>
class osss_serialisable_wrapper<osss_array<ElementType, Size> >
  : public osss_serialisable_object
{

  typedef osss_serialisable_object base_type;

public:
  CLASS( osss_serialisable_wrapper, osss_serialisable_object );

  osss_serialisable_wrapper() : osss_serialisable_object()
  { }

  osss_serialisable_wrapper(const osss_array<ElementType, Size>& _value)
  {
    value = _value;
    for(unsigned int i=0; i<Size; i++)
    {
      serialisableArray[i] = value[i];
      serialisableArray[i].serialise_obj();
    }
    copyBitVector();
  }

  osss_serialisable_wrapper(const osss_serialisable_wrapper& _ser_wrapper)
  {
    value = _ser_wrapper.value;
    for(unsigned int i=0; i<Size; i++)
    {
      serialisableArray[i] = value[i];
      serialisableArray[i].serialise_obj();
    }
    copyBitVector();
  }

  CONSTRUCTOR(public, osss_serialisable_wrapper, (osss_serialisable_archive* arch))
  {
    for(unsigned int i=0; i<Size; i++)
      arch->rebuild(serialisableArray[i]);
    copyBitVector();   
  }

  osss_serialisable_wrapper& operator=(const osss_serialisable_wrapper& _wrapper)
  {
    if (this == &_wrapper)
      return *this;

    value = _wrapper.value;
    for(unsigned int i=0; i<Size; i++)
    {
      serialisableArray[i] = value[i];
      serialisableArray[i].serialise_obj();
    }
    copyBitVector();
    
    return *this;
  }

  osss_serialisable_wrapper& operator=(const osss_array<ElementType, Size>& _value)
  {
    value = _value;
    for(unsigned int i=0; i<Size; i++)
    {
      serialisableArray[i] = value[i];
      serialisableArray[i].serialise_obj();
    }
    copyBitVector();
   
    return *this;
  }

  operator osss_array<ElementType, Size>& ()
  {
    this->deserialise_obj();
    return value;
  }

  static void type_check() {}

  ///////////////////////////////////////////////////////////////////////
  // overwrite the functionality of osss_serialisable_object
  ///////////////////////////////////////////////////////////////////////

  virtual void serialise_obj()
  {
    for(unsigned int i=0; i<Size; i++)
      serialisableArray[i].serialise_obj();
    copyBitVector();
  }

  virtual void deserialise_obj()
  {
    restoreBitVector();
    for(unsigned int i=0; i<Size; i++)
    {
      serialisableArray[i].deserialise_obj();
      value[i] = serialisableArray[i];
    }
  }

protected:
 
  void copyBitVector()
  {
    base_type::m_osss_bitVector.clear();
    for(unsigned int i=0; i<Size; i++)
    {
      for(unsigned int j=0;
	  j<serialisableArray[i].m_osss_bitVector.size(); 
	  j++)
      {
	base_type::m_osss_bitVector.
	  push_back(serialisableArray[i].m_osss_bitVector[j]);
      }
    }
    base_type::iter2 = base_type::m_osss_bitVector.begin();
    base_type::isSerialised = true;
  }

  void restoreBitVector()
  {
    unsigned int j = 0;
    unsigned int nr = 0;
    unsigned int array_size = serialisableArray[0].m_osss_bitVector.size();
    for(unsigned int i=0; i<base_type::m_osss_bitVector.size(); i++)
    {
      if ((i != 0) && (i%array_size == 0))
      {
	j = 0;
	nr++;
      }
      else if (i != 0)
	j++;
      
      serialisableArray[nr].m_osss_bitVector[j] = base_type::m_osss_bitVector[i];
    }
    /*
      this->print_vector();
      for(unsigned int i=0; i<=nr; i++)
        serialisableArray[i].print_vector();
    */
  }

  osss_serialisable_wrapper<ElementType> serialisableArray[Size];
  osss_array<ElementType, Size> value;

};

//------------------------------------------------------------------------------

#undef BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_BASIC_TYPE
#undef BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_OSSS_SERIALISABLE_WRAPPER_FOR_TEMPLATED_SYSTEMC_FX_TYPE

} // namespace osssi

} // namespace osss

#endif
// $Id: osss_serialisable_wrapper.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
