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
osss_serialisable_archive::
osss_serialisable_archive() : m_bitVectorArray(),
			      m_chunkSize(0)
{
  //CAUTION:
  //  when using this constructor both inerators: m_outerIter and m_innerIter
  //  stay uninitialized
}

inline
osss_serialisable_archive::
osss_serialisable_archive(osss_serialisable_object & obj) 
{
  obj.serialise_obj();
  // adds bit vector of obj to the internal bit vector array
  m_bitVectorArray.push_back(std::vector<bool>(obj.m_osss_bitVector));
  // set iterators to begin of internal bit vector array
  m_outerIter = m_bitVectorArray.begin();
  m_innerIter = m_bitVectorArray.begin()->begin();
  m_chunkSize = 0;
}

inline
osss_serialisable_archive& 
osss_serialisable_archive::operator=(const osss_serialisable_object & object)
{
  // cast away constness (ugly way but neccesary due to serialiseObj())
  osss_serialisable_object* obj = 
    const_cast<osss_serialisable_object*>(&object);
  obj->serialise_obj();
  // delete internal bit vector array
  clear();
  // adds bit vector of obj to the internal bit vector array
  m_bitVectorArray.push_back(obj->m_osss_bitVector);
  // set iterators to begin of internal bit vector array
  m_outerIter = m_bitVectorArray.begin();
  m_innerIter = m_bitVectorArray.begin()->begin();
  m_chunkSize = 0;

  return *this;
}

inline
bool 
osss_serialisable_archive::empty() 
{ 
  return m_bitVectorArray.empty();
}

inline
void 
osss_serialisable_archive::clear() 
{ 
  m_bitVectorArray.clear();
  m_outerIter = m_bitVectorArray.begin();
  //m_innerIter is undefined !!!
  m_chunkSize = 0;
}

inline
bool 
osss_serialisable_archive::complete() 
{ 
  return (m_outerIter == m_bitVectorArray.end());
}

inline
unsigned int 
osss_serialisable_archive::get_chunk_size()
{
  return m_chunkSize;
}

inline
void 
osss_serialisable_archive::prepare_rebuild()
{
  m_outerIter = m_bitVectorArray.begin();
  if (!m_bitVectorArray.empty())
    m_innerIter = m_bitVectorArray.begin()->begin();
}

inline
void 
osss_serialisable_archive::prepare_read()
{
  prepare_rebuild();
}

inline  
void 
osss_serialisable_archive::prepare_write()
{
  clear();
  m_bitVectorArray.push_back(std::vector<bool>());
  m_outerIter = m_bitVectorArray.begin();
  m_innerIter = m_bitVectorArray.begin()->begin();
}

inline
void 
osss_serialisable_archive::rebuild(osss_serialisable_object & obj)
{
  if (!m_bitVectorArray.empty())
  {
    obj.serialise_obj();
    // copy bit_vector from current position until length
    // of obj's bit_vector
    std::vector<bool>::iterator obj_iter = 
      obj.m_osss_bitVector.begin();
    while (obj_iter != obj.m_osss_bitVector.end())
    {
      *obj_iter = *m_innerIter;
      obj_iter++;
      m_innerIter++;
    }

    //std::cout << "m_bitVectorArray.size(): " << m_bitVectorArray.size() << std::endl;
    //std::cout << "m_chunkSize: " << m_chunkSize << std::endl;

    // after rebuild from single bit vector
    if (m_bitVectorArray.size() == 1)
    {
      // skip padding resulting from reception port width
      if (m_chunkSize != 0)
      {
	unsigned int padding =
	  //obj.m_osss_bitVector.size() % m_chunkSize;
	  m_bitVectorArray[0].size() % m_chunkSize;
	m_innerIter += padding;
	//std::cout << "padding: " << padding << std::endl;
      }
    }
    else // after rebuild from bit vector array
    {
      // switch to next column
      m_outerIter++;
      if (m_outerIter != m_bitVectorArray.end())
	m_innerIter = m_outerIter->begin();
    }
    obj.deserialise_obj();
  }
}

inline
void 
osss_serialisable_archive::add(osss_serialisable_object& ser_obj)
{
  ser_obj.serialise_obj();
  // adds bit vector of ser_obj to the internal bit vector array
  m_bitVectorArray.push_back(std::vector<bool>(ser_obj.m_osss_bitVector));
  // set iterators to begin of internal bit vector array
  m_outerIter = m_bitVectorArray.begin();
  m_innerIter = m_bitVectorArray.begin()->begin();
  m_chunkSize = 0;
}

inline
void 
osss_serialisable_archive::concat(const osss_serialisable_archive& ser_obj)
{
  if ((ser_obj.m_bitVectorArray.size() == 1) && 
      (m_bitVectorArray.size() == 1))
  {
    std::vector<bool>::const_iterator ser_obj_iter;
    for(ser_obj_iter = ser_obj.m_bitVectorArray.begin()->begin();
	ser_obj_iter != ser_obj.m_bitVectorArray.begin()->end();
	ser_obj_iter++)
    {
      m_bitVectorArray[0].push_back(*ser_obj_iter);
    }
  }
}

template<class T>
inline
void 
osss_serialisable_archive::
write_chunk_to_port(sc_core::sc_out< T >& out_port)
{
  T result = this->read_chunk<T>();
  out_port.write(result);
}

template<class T>
inline
void 
osss_serialisable_archive::
write_chunk_to_port(sc_core::sc_inout< T >& inout_port)
{
  T result = this->read_chunk<T>();
  inout_port.write(result);
}

template<class T>
inline
T 
osss_serialisable_archive::read_chunk()
{
  T result;
  size_t bitWidth = GET_SIZE(T);

  if (!m_bitVectorArray.empty())
  {
    size_t i;
    for ( i=0; i < bitWidth; i++ )
    {
      BitHelper<T>::setBit(result, i, *m_innerIter);
      m_innerIter++;
      if (m_innerIter == m_outerIter->end())
      {
        m_outerIter++;
        if (m_outerIter != m_bitVectorArray.end())
          m_innerIter = m_outerIter->begin();
        else
          break;
      }
    }
    // fill the reset of the result value with zeros
    for (size_t rest = i; rest < bitWidth; rest++)
      BitHelper<T>::setBit(result, rest, false); 
  }
  return result;
}

template<class T>
inline
void 
osss_serialisable_archive::
read_chunk_from_port(sc_core::sc_in< T >& in_port)
{
  m_chunkSize = GET_SIZE(T);
  
  for(unsigned int i=0; i<m_chunkSize; i++)
  {
    m_bitVectorArray[0].push_back(
      BitHelper<T>::getBit(const_cast<T&>(in_port.read()), i) );
  }
}

template<class T>
inline
void 
osss_serialisable_archive::
read_chunk_from_port(sc_core::sc_inout< T >& inout_port)
{
  m_chunkSize = GET_SIZE(T);

  for(unsigned int i=0; i<m_chunkSize; i++)
  {
    m_bitVectorArray[0].push_back(
      BitHelper<T>::getBit(const_cast<T&>(inout_port.read()), i) );
  }
}

template<class T>
inline
void 
osss_serialisable_archive::write_chunk(T value)
{ 
  m_chunkSize = GET_SIZE(T);
  
  for(unsigned int i=0; i<m_chunkSize; i++)
  {
    m_bitVectorArray[0].push_back(BitHelper<T>::getBit(value, i) );
  }
}

inline
void 
osss_serialisable_archive::print_vector()
{
  for(unsigned int i=0; i<m_bitVectorArray.size(); i++)
  {
    unsigned int count = 0;
    std::cout << "vector " << i << ":" << std::endl;
    for (unsigned int j=0; j<m_bitVectorArray[i].size(); j++)
    {
      std::cout << m_bitVectorArray[i][j] << " ";
      count++;
      if(count == 32)
      {
	count = 0;
	std::cout << std::endl;
      }
    }
    std::cout << std::endl;
  }
}

#undef GET_SIZE
#undef BUILD_ADD_METHOD_FOR_BASIC_TYPE
#undef BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_ADD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE
#undef BUILD_REBUILD_METHOD_FOR_BASIC_TYPE
#undef BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_TYPE
#undef BUILD_REBUILD_METHOD_FOR_TEMPLATED_SYSTEMC_FX_TYPE

inline
std::ostream &
operator<<( std::ostream & os, const osss_serialisable_archive& ser_arch )
{
  for(unsigned int i=0; i<ser_arch.m_bitVectorArray.size(); i++)
  {
    unsigned int count = 0;
    os << "vector " << i << ":" << std::endl;
    for (unsigned int j=0; j<ser_arch.m_bitVectorArray[i].size(); j++)
    {
      os << ser_arch.m_bitVectorArray[i][j] << " ";
      count++;
      if(count == 32)
      {
	count = 0;
	os << std::endl;
      }
    }
    os << std::endl;
  }
  return os;
}

} // namespace osss

// $Id: osss_serialisable_archive.tpp 2994 2009-01-30 14:50:19Z henning $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
