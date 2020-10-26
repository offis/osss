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

inline
osss_serialisable_object::
osss_serialisable_object()
{
#ifdef OSSS_GREEN
  isSerialised = false;
  m_osss_bitVector.clear();
  iter2 = m_osss_bitVector.begin();
#endif
}

inline
osss_serialisable_object::
osss_serialisable_object(const osss_serialisable_object & object)
{
#ifdef OSSS_GREEN
  osss_serialisable_object* obj = 
    const_cast<osss_serialisable_object*>(&object);
  obj->serialise_obj();
    
  // copy all members
  m_osss_bitVector = obj->m_osss_bitVector;
  iter2 = m_osss_bitVector.begin();
  isSerialised = true;
#endif
}

inline  
osss_serialisable_object& 
osss_serialisable_object::operator=(const osss_serialisable_object & object)
{
#ifdef OSSS_GREEN
  if (this == &object)
    return *this;

  osss_serialisable_object* obj = 
    const_cast<osss_serialisable_object*>(&object);
  obj->serialise_obj();

  // copy all members
  m_osss_bitVector = obj->m_osss_bitVector;
  iter2 = m_osss_bitVector.begin();
  isSerialised = true;
#endif

  return *this;
}

inline
void osss_serialisable_object::serialise_obj()
{
  if (isSerialised == true)
  {
    m_osss_bitVector.clear();
    iter2 = m_osss_bitVector.begin();
  }
  // call user defined serialise method
  serialise();
  isSerialised = true;
  iter2 = m_osss_bitVector.begin();
}

inline
void osss_serialisable_object::deserialise_obj()
{
  iter2 = m_osss_bitVector.begin();
  // call user defined deserialise method
  deserialise();
  iter2 = m_osss_bitVector.begin();
}

inline
void osss_serialisable_object::print_vector()
{
  int count=0;
  std::vector<bool>::iterator iter;
  for (iter = m_osss_bitVector.begin(); 
       iter != m_osss_bitVector.end(); 
       iter++)
  {
    std::cout << *iter << " ";
    count++;
    if(count == 32)
    {
      count = 0;
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

 
template<class T>
inline
void 
osss_serialisable_object::
write_chunk_to_port(sc_core::sc_out< T >& out_port)
{
  T result = 0;
  size_t bitWidth = GET_SIZE(T);
  bool tmp;
    
  for ( size_t i=0; i < bitWidth; i++ )
  {
    if (iter2 == m_osss_bitVector.end())
    {
      break;
    }
    tmp = *iter2;

    BitHelper<T>::setBit(result, i, tmp);

    ++iter2;
  }
  out_port.write(result);
}
 
template<class T>
inline
void 
osss_serialisable_object::
write_chunk_to_port(sc_core::sc_inout< T >& inout_port)
{
  T result = 0;
  size_t bitWidth = GET_SIZE(T);
  bool tmp;

  for ( size_t i=0; i < bitWidth; i++ )
  {
    if (iter2 == m_osss_bitVector.end())
    {
      break;
    }
    tmp = *iter2;

    BitHelper<T>::setBit(result, i, tmp);

    ++iter2;
  }
  inout_port.write(result);
}
 
template<class T>
inline
T 
osss_serialisable_object::read_chunk()
{
  T result = 0;                            
  size_t bitWidth = GET_SIZE(T);              
  bool tmp;                                

  for ( size_t i=0; i < bitWidth; i++ )       
  {                                        
    if (iter2 == m_osss_bitVector.end())
    {
      break;
    }
    tmp = *iter2;                              
        
    BitHelper< T >::setBit(result, i, tmp);

    iter2++;                              
  }                                        
  return result;                            
}

template<class T>
inline
void 
osss_serialisable_object::
read_chunk_from_port(sc_core::sc_in< T >& in_port)
{ 
  if(m_osss_bitVector.empty())
    serialise_obj(); // init m_osss_bitVector size
    
  size_t bitWidth = GET_SIZE(T);
    
  for(size_t i=0; i<bitWidth; i++)
  {            
    if (iter2 == m_osss_bitVector.end())
    {
      break;
    }
      
    *iter2 = BitHelper<T>::getBit(const_cast<T&>(in_port.read()), i);

    iter2++;
  }       
}

template<class T>
inline
void 
osss_serialisable_object::
read_chunk_from_port(sc_core::sc_inout< T >& inout_port)
{ 
  if(m_osss_bitVector.empty())
    serialise_obj(); // init m_osss_bitVector size
    
  size_t bitWidth = GET_SIZE(T);

  for(size_t i=0; i<bitWidth; i++)
  {            
    if (iter2 == m_osss_bitVector.end())
    {
      break;
    }
      
    *iter2 = BitHelper<T>::getBit(const_cast<T&>(inout_port.read()), i);

    iter2++;
  }       
}

template<class T>
inline
void 
osss_serialisable_object::write_chunk(T value)
{ 
  size_t bitWidth = GET_SIZE(T);

  for(size_t i=0; i<bitWidth; i++)
  {            
    if (iter2 == m_osss_bitVector.end())
    {
      break;
    }
      
    *iter2 = BitHelper<T>::getBit(value, i) ;     

    iter2++;
  }       
}
  
inline
bool osss_serialisable_object::complete()
{
  return ( iter2 == m_osss_bitVector.end()
	       && ! m_osss_bitVector.empty() );
}

inline   
void osss_serialisable_object::clear()
{
  m_osss_bitVector.clear();
} 

//------------------------------------------------
// serialise functions
//------------------------------------------------

template<class T>
inline
void 
osss_serialisable_object::
store_array(T *beginArray, unsigned int arrayLength)
{
  for(unsigned int i=0; i<arrayLength; i++)
  {
    store_element(*beginArray);
    beginArray++;
  }
}

inline
void
osss_serialisable_object::
store_element(osss_serialisable_object &ser_obj)
{
  ser_obj.serialise_obj();
  size_t bitWidth = ser_obj.m_osss_bitVector.size();
  for(size_t i=0; i<bitWidth; i++)
  {
    m_osss_bitVector.push_back( ser_obj.m_osss_bitVector[i] );
  }
}

/*
template<class T>
inline
void 
osss_serialisable_object::
store_element(T value)
{ 
  size_t bitWidth = GET_SIZE(T);

  for(size_t i=0; i<bitWidth; i++)
  {                  
    m_osss_bitVector.push_back( BitHelper<T>::getBit(value, i) );     
  }       
}
*/

//------------------------------------------------
// deserialise functions 
//------------------------------------------------

template<class T>
inline  
void osss_serialisable_object::
restore_array(T *beginArray, unsigned int arrayLength)
{
  for(unsigned int i=0; i<arrayLength; i++)
  {
    restore_element(*beginArray);
    beginArray++;
  }
}

inline
void
osss_serialisable_object::
restore_element(osss_serialisable_object &ser_obj)
{
  ser_obj.serialise_obj();
  size_t bitWidth = ser_obj.m_osss_bitVector.size();

  for(size_t i=0; i<bitWidth; i++)
  {
    ser_obj.m_osss_bitVector[i] = *iter2;
    iter2++;
  }
  ser_obj.deserialise_obj();
}

/*
inline
void 
osss_serialisable_object::
restore_element(sc_dt::sc_bit &value)
{
  sc_dt::sc_bit result;
  size_t bitWidth = GET_SIZE(sc_dt::sc_bit);
  bool tmp;

  for ( size_t i=0; i < bitWidth; i++ )
  {
    tmp = *iter2;

    BitHelper< sc_dt::sc_bit >::setBit(result, i, tmp);

    iter2++;
  }
  value = result;
}

template<class T>
inline
void 
osss_serialisable_object::
restore_element(T &value)
{
  T result = 0;
  size_t bitWidth = GET_SIZE(T);
  bool tmp;

  for ( size_t i=0; i < bitWidth; i++ )
  {
    tmp = *iter2;
      
    BitHelper< T >::setBit(result, i, tmp);

    iter2++;
  }
  value = result;
}
*/

//------------------------------------------------------------------------------

#undef BUILD_COPY_CONSTRUCTOR_FOR_BASIC_TYPE
#undef BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_COPY_CONSTRUCTOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE
#undef BUILD_ASSIGNMENT_OPERATOR_FOR_BASIC_TYPE
#undef BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_ASSIGNMENT_OPERATOR_FOR_TEMPLATED_SYSTEMC_FX_TYPE

#undef BUILD_STORE_ELEMENT_FOR_BASIC_TYPE
#undef BUILD_STORE_ELEMENT_FOR_NON_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_STORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE
#undef BUILD_RESTORE_ELEMENT_FOR_BASIC_TYPE
#undef BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_RESTORE_ELEMENT_FOR_TEMPLATED_SYSTEMC_FX_TYPE

inline
std::ostream &
operator<<( std::ostream & os, const osss_serialisable_object& ser_obj )
{
  int count=0;
  std::vector<bool>::const_iterator iter;
  for (iter = ser_obj.m_osss_bitVector.begin();
       iter != ser_obj.m_osss_bitVector.end();
       iter++)
  {
    os << *iter << " ";
    count++;
    if(count == 32)
    {
      count = 0;
      os << std::endl;
    }
  }
  os << std::endl;
  return os;
}

} // namespace osss

// $Id: osss_serialisable_object.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
