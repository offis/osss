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

#ifndef OSSS_SERIALISABLE_ARCHIVE_HH
#define OSSS_SERIALISABLE_ARCHIVE_HH

#include "o3s/datatypes/osss_serialisable_object.h"

namespace osss {

#define BUILD_ADD_METHOD_FOR_BASIC_TYPE( _type_ ) \
 void add( _type_& value ) \
  { \
    osss_serialisable_object* ser_obj = new osss_serialisable_object(value); \
    m_bitVectorArray.push_back(std::vector<bool>(ser_obj->m_osss_bitVector)); \
    delete ser_obj; \
    m_outerIter = m_bitVectorArray.begin(); \
    m_innerIter = m_bitVectorArray.begin()->begin(); \
    m_chunkSize = 0; \
  }

#define BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
 template<int W> \
 void add( _type_< W > & value ) \
 { \
   osss_serialisable_object* ser_obj = new osss_serialisable_object(value); \
   m_bitVectorArray.push_back(std::vector<bool>(ser_obj->m_osss_bitVector)); \
   delete ser_obj; \
   m_outerIter = m_bitVectorArray.begin(); \
   m_innerIter = m_bitVectorArray.begin()->begin(); \
   m_chunkSize = 0; \
 }

#define BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
 template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
 void add( _type_< W,I,Q,O,N > & value ) \
 { \
   osss_serialisable_object* ser_obj = new osss_serialisable_object(value); \
   m_bitVectorArray.push_back(std::vector<bool>(ser_obj->m_osss_bitVector)); \
   delete ser_obj; \
   m_outerIter = m_bitVectorArray.begin(); \
   m_innerIter = m_bitVectorArray.begin()->begin(); \
   m_chunkSize = 0; \
 }

//------------------------------------------------------------------------------

#define BUILD_REBUILD_METHOD_FOR_BASIC_TYPE( _type_ ) \
 void rebuild(_type_ & value) \
 { \
   if (!m_bitVectorArray.empty()) \
   { \
     osss_serialisable_object* obj = new osss_serialisable_object(value); \
      /* copy bit_vector from current position until length \
         of obj's bit_vector */ \
     std::vector<bool>::iterator obj_iter = \
       obj->m_osss_bitVector.begin(); \
     while (obj_iter != obj->m_osss_bitVector.end()) \
     { \
       *obj_iter = *m_innerIter; \
       obj_iter++; \
       m_innerIter++; \
     } \
     /* after rebuild from single bit vector */ \
     if (m_bitVectorArray.size() == 1) \
     { \
       /* skip padding resulting from reception port width */ \
       if (m_chunkSize != 0) \
       { \
	 unsigned int padding = \
	   obj->m_osss_bitVector.size() % m_chunkSize; \
	 m_innerIter += padding; \
       } \
     } \
     else /* after rebuild from bit vector array */ \
     { \
       /* switch to next column */ \
       m_outerIter++; \
       if (m_outerIter != m_bitVectorArray.end()) \
         m_innerIter = m_outerIter->begin(); \
     } \
     obj->iter2 = obj->m_osss_bitVector.begin(); \
     obj->restore_element(value); \
     delete obj; \
   } \
 }

#define BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE( _type_ ) \
 template<int W> \
 void rebuild(_type_< W > & value) \
 { \
   if (!m_bitVectorArray.empty()) \
   { \
     osss_serialisable_object* obj = new osss_serialisable_object(value); \
      /* copy bit_vector from current position until length \
         of obj's bit_vector */ \
     std::vector<bool>::iterator obj_iter = \
       obj->m_osss_bitVector.begin(); \
     while (obj_iter != obj->m_osss_bitVector.end()) \
     { \
       *obj_iter = *m_innerIter; \
       obj_iter++; \
       m_innerIter++; \
     } \
     /* after rebuild from single bit vector */ \
     if (m_bitVectorArray.size() == 1) \
     { \
       /* skip padding resulting from reception port width */ \
       if (m_chunkSize != 0) \
       { \
	 unsigned int padding = \
	   obj->m_osss_bitVector.size() % m_chunkSize; \
	 m_innerIter += padding; \
       } \
     } \
     else /* after rebuild from bit vector array */ \
     { \
       /* switch to next column */ \
       m_outerIter++; \
       if (m_outerIter != m_bitVectorArray.end()) \
         m_innerIter = m_outerIter->begin(); \
     } \
     obj->iter2 = obj->m_osss_bitVector.begin(); \
     obj->restore_element(value); \
     delete obj; \
   } \
 }

#define BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE( _type_ ) \
 template<int W, int I, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N> \
 void rebuild(_type_< W,I,Q,O,N > & value) \
 { \
   if (!m_bitVectorArray.empty()) \
   { \
     osss_serialisable_object* obj = new osss_serialisable_object(value); \
      /* copy bit_vector from current position until length \
         of obj's bit_vector */ \
     std::vector<bool>::iterator obj_iter = \
       obj->m_osss_bitVector.begin(); \
     while (obj_iter != obj->m_osss_bitVector.end()) \
     { \
       *obj_iter = *m_innerIter; \
       obj_iter++; \
       m_innerIter++; \
     } \
     /* after rebuild from single bit vector */ \
     if (m_bitVectorArray.size() == 1) \
     { \
       /* skip padding resulting from reception port width */ \
       if (m_chunkSize != 0) \
       { \
	 unsigned int padding = \
	   obj->m_osss_bitVector.size() % m_chunkSize; \
	 m_innerIter += padding; \
       } \
     } \
     else /* after rebuild from bit vector array */ \
     { \
       /* switch to next column */ \
       m_outerIter++; \
       if (m_outerIter != m_bitVectorArray.end()) \
         m_innerIter = m_outerIter->begin(); \
     } \
     obj->iter2 = obj->m_osss_bitVector.begin(); \
     obj->restore_element(value); \
     delete obj; \
   } \
 }

//------------------------------------------------------------------------------

/**
 * \class osss_serialisable_archive
 *
 * \brief This class is some kind of container for objects of type
 *        osss_serialisable_object.
 *
 * This class is used to represent all parameters of a method as a uniform
 * stream of data (bitvector). It is used for the serialisation of method
 * parameters during an OSSS RMI (Remote Method Invocation) on the Virtual
 * Target Architecture Layer only.
 *
 * \see osss_serialisable_object
 *
 * \todo move this class to the osssi namespace (see \ticket{27})
 */
class osss_serialisable_archive
{

 friend
 std::ostream &
 operator<<( std::ostream & os, const osss_serialisable_archive& ser_arch );

public:
  osss_serialisable_archive();

  explicit osss_serialisable_archive(osss_serialisable_object & obj);
  
  osss_serialisable_archive& operator=(const osss_serialisable_object & object);

  ~osss_serialisable_archive() {}
 
  bool empty();

  void clear();
 
  bool complete();

  unsigned int get_chunk_size();

  void prepare_rebuild();
 
  void prepare_read();
   
  void prepare_write();
 
  void rebuild(osss_serialisable_object & obj);
 
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(char)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(unsigned char)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(signed char)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(short) // == signed short
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(unsigned short)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(int) // == signed int
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(unsigned int)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(long) // == signed long
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(unsigned long)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(long long) // == signed long long
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(unsigned long long)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(bool)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(float)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(double)
  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(long double)

  BUILD_REBUILD_METHOD_FOR_BASIC_TYPE(sc_dt::sc_bit)
  BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
  BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
  BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
  BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
  BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
    BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
    BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

  void add(osss_serialisable_object& ser_obj);
 
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(char)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(unsigned char)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(signed char)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(short) // == signed short
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(unsigned short)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(int) // == signed int
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(unsigned int)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(long) // == signed long
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(unsigned long)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(long long) // == signed long long
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(unsigned long long)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(bool)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(float)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(double)
  BUILD_ADD_METHOD_FOR_BASIC_TYPE(long double)

  BUILD_ADD_METHOD_FOR_BASIC_TYPE(sc_dt::sc_bit)
  BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bv)
  BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_bigint)
  BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_biguint)
  BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_int)
  BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE(sc_dt::sc_uint)

#ifdef SC_INCLUDE_FX
    BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_fixed)
    BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE(sc_dt::sc_ufixed)
#endif

  void concat(const osss_serialisable_archive& ser_obj);
   
  template<class T>
  void write_chunk_to_port(sc_core::sc_out< T >& out_port);
 
  template<class T>
  void write_chunk_to_port(sc_core::sc_inout< T >& inout_port);
 
  template<class T>
  T read_chunk();

  template<class T>
  void read_chunk_from_port(sc_core::sc_in< T >& in_port);

  template<class T>
  void read_chunk_from_port(sc_core::sc_inout< T >& inout_port);
 
  template<class T>
  void write_chunk(T value);

  /**
   * Prints internal bitvector representation to stdout.
   *
   * \warning thid method is deprecated use operator<<() instead
   * \see operator<<()
   */
  void print_vector();

private:
  std::vector< std::vector<bool> >           m_bitVectorArray;
  std::vector< std::vector<bool> >::iterator m_outerIter;
  std::vector<bool>::iterator                m_innerIter;
  unsigned int m_chunkSize;

};

} // namespace osss

#include "o3s/datatypes/osss_serialisable_archive.tpp"

#endif
// $Id: osss_serialisable_archive.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
