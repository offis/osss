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

  //----------------------------------------------------------------------------
  // get sizes
  //----------------------------------------------------------------------------
  
  template<unsigned int DataWidth, unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  data_width() const
  {
    return DataWidth;
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  address_width() const
  {
    return AddressWidth;
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  size() const
  {
    return m_available_address_lines;
  }

  //----------------------------------------------------------------------------
  // registration
  //----------------------------------------------------------------------------

  // registration for all kinds of serialise objects (except osss_arrays)
  template<unsigned int DataWidth, unsigned int AddressWidth>
  template<class T>
  void
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  add_element( osss_memory_element<T>& element )
  {
    osss_memory_element<T> mem_element(this, m_element_index);

    osssi::osss_serialisable_wrapper<T> data_buffer = T();
    double tmp_offset = ceil((double)data_buffer.size() / (double)DataWidth);
    unsigned int old_element_offset = m_element_offset;
    if ((tmp_offset == 0.0f) && (data_buffer.size() != 0))
      m_element_offset = m_element_offset + 1;
    else
      m_element_offset = m_element_offset + (unsigned int)tmp_offset;

    if (m_element_offset > m_available_address_lines)
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__
	<< ": required_address_lines (" << m_element_offset 
	<< ") > available_address_lines (" << m_available_address_lines << ")"
	<< std::endl;
      OSSS_ABORT();
    }

    m_element_offset_map[m_element_index] = old_element_offset;
    
    m_element_index++;
    element.initialize_with(mem_element);
  }

  // registration for osss_arrays
  template<unsigned int DataWidth, unsigned int AddressWidth>
  template<class T, unsigned int Size>
  void
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  add_element( osss_memory_element<osss_array<T, Size> >& element )
  {
    osss_memory_element<osss_array<T, Size> > 
      mem_element(this, m_element_index);

    osssi::osss_serialisable_wrapper<T> data_buffer = T();
    double tmp_offset = ceil((double)data_buffer.size() / (double)DataWidth);
    unsigned int old_element_offset = m_element_offset;
    if ((tmp_offset == 0.0f) && (data_buffer.size() != 0))
      m_element_offset = m_element_offset + Size;
    else
      m_element_offset = m_element_offset + ((unsigned int)tmp_offset * Size);

    if (m_element_offset > m_available_address_lines)
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__
	<< ": required_address_lines (" << m_element_offset 
	<< ") > available_address_lines (" << m_available_address_lines << ")"
	<< std::endl;
      OSSS_ABORT();
    }

    m_element_offset_map[m_element_index] = old_element_offset;
    
    m_element_index++;
    element.initialize_with(mem_element);
  }

  //----------------------------------------------------------------------------
  // I/O functionality
  //----------------------------------------------------------------------------
  
  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  init_from_file(std::string in_file_name)
  {
    std::ifstream file(in_file_name.c_str());
    if (file.is_open())
    {
      scan(file);
      file.close();
    }
    else std::cerr << "Error in " << __PRETTY_FUNCTION__ 
		   << ": Unable to open file "  
		   << in_file_name << std::endl; 
    OSSS_ABORT(); 
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  scan(std::istream& is)
  {
    sc_dt::sc_biguint<DataWidth> buffer;
    unsigned int addr = 0;
    
    std::string line;
    while (! is.eof() )
    {
      std::getline (is,line);
      if (line.length() != DataWidth)
      {
	std::cerr << "Error in " << __PRETTY_FUNCTION__
		  << ": line " << addr-1
		  << " has length " << line.length()
		  << ", excpected length " << DataWidth
		  << std::endl;
	OSSS_ABORT();
      }
      else
      {
	for(unsigned int i=0; i<line.length(); i++)
	  buffer.bit(DataWidth-1-i) = atoi(line.substr(i,1).c_str());
      }
      m_bit_storage[addr] = buffer;

      if (addr == osss::osssi::pow<2, AddressWidth>::value-1)
	break;

      addr++;
    }
  }

  // Dumps the actual content of the memory into an ostream.
  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  dump( ::std::ostream& os ) const
  {
    for( unsigned int i = 0; i < this->size(); ++i )
    {
      sc_dt::sc_biguint<DataWidth> tmp_line  = this->m_bit_storage[i];
      for(int j = tmp_line.length() - 1; j >= 0; --j)
	os << tmp_line.bit(j);
      os << std::endl;
    }
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  print( ::std::ostream& os  ) const
  {
    os << *this << ::std::endl;
  }

  //----------------------------------------------------------------------------
  // access
  //----------------------------------------------------------------------------

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  write(::osss::osss_serialisable_object* ser_obj, 
	unsigned int index, unsigned int element_offset )
  {
    unsigned int data_offset = map_index_to_memory_address(index);

    ser_obj->serialise_obj();

    while (!ser_obj->complete())
    {
      m_bit_storage[data_offset + element_offset] = 
        ser_obj->template read_chunk< sc_dt::sc_biguint<DataWidth> >();

      if (sc_core::sc_start_of_simulation_invoked())
      {
	if (data_offset == map_index_to_memory_address(index))
	  wait_first_write();
	else
	  wait_write();
      }

      data_offset++;
    }
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  read(::osss::osss_serialisable_object* ser_obj, 
       unsigned int index, unsigned int element_offset ) const
  {
    unsigned int data_offset = map_index_to_memory_address(index);
      
    while (!ser_obj->complete())
    { 
      ser_obj->write_chunk(m_bit_storage[data_offset + element_offset]); 

      if (sc_core::sc_start_of_simulation_invoked())
      {
	if (data_offset == map_index_to_memory_address(index))
	  wait_first_read();
	else
	  wait_read();
      }

      data_offset++;
    }
    // finallises the deserialisation of m_data_buffer
    ser_obj->deserialise_obj();
  }

  //----------------------------------------------------------------------------
  // specify timing of the memory 
  //----------------------------------------------------------------------------

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  wait_first_read() const
  {
    // the osss_memory is a perfect memory where acceses do not take any time
    // usually a certain amount of wait statements would follow, e.g.
    /*
    for (unsigned int i=0; i<NUMBER_OF_WAITS; i++)
      sc_core::wait();
    */
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  wait_read() const
  {
    // see above
  }
  
  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  wait_first_write() const
  {
    // see above
  }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  void 
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  wait_write() const
  {
    // see above
  }

 
  template<unsigned int DataWidth, unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_serialisable_object, DataWidth, AddressWidth>::
  map_index_to_memory_address(unsigned int index) const
  { 
    std::map<const unsigned int, unsigned int>::const_iterator map_iter;
    map_iter = m_element_offset_map.find(index);
    if (map_iter != m_element_offset_map.end())
      return (*map_iter).second;
    else
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__ 
		<< " accessing invalid memory element" << std::endl;
      OSSS_ABORT();
    }
  }


  //------------------------------------------------------------------------------
  // osss_memory_element
  //------------------------------------------------------------------------------

  template<class DataType>
  osss_memory_element<DataType>::
  osss_memory_element(const osss_memory_element& mem_element)
  {
    m_memory_ptr = mem_element.m_memory_ptr;
    m_element_index = mem_element.m_element_index;
    m_data = mem_element.m_data;

    // copy m_data to bit memory
    if (m_memory_ptr != NULL)
      m_memory_ptr->write(&m_data, m_element_index); 
  }
  
  template<class DataType>
  osss_memory_element<DataType>& 
  osss_memory_element<DataType>::
  operator=(const osss_memory_element& mem_element)
  {
    if (&mem_element != this)
    {
      m_memory_ptr = mem_element.m_memory_ptr;
      m_element_index = mem_element.m_element_index;
      m_data = mem_element.m_data;

      // copy m_data to bit memory
      if (m_memory_ptr != NULL)
	m_memory_ptr->write(&m_data, m_element_index);
    }
    return *this;
  }

  template<class DataType>
  void
  osss_memory_element<DataType>::
  initialize_with(const osss_memory_element& mem_element)
  {
    if (&mem_element != this)
    {
      m_memory_ptr = mem_element.m_memory_ptr;
      m_element_index = mem_element.m_element_index;
      m_data = mem_element.m_data;
    }
  }

  template<class DataType>
  osss_memory_element<DataType>& 
  osss_memory_element<DataType>::operator=(const DataType& data)
  {
    m_data = data;
    if (m_memory_ptr != NULL)
      m_memory_ptr->write(&m_data, m_element_index);
    return *this;
  }

  template<class DataType>
  osss_memory_element<DataType>::
  operator DataType& () const
  { 
    if (m_memory_ptr != NULL)
      m_memory_ptr->read(&m_data, m_element_index);
    return m_data;
  }

  //------------------------------------------------------------------------------
  // partial specialisation for osss_arrays
  //------------------------------------------------------------------------------

  template<class DataType, unsigned int Size>
  osss_memory_element<osss_array<DataType, Size> >::
  osss_memory_element(const osss_memory_element& mem_element)
  {
    m_memory_ptr = mem_element.m_memory_ptr;
    m_element_index = mem_element.m_element_index;
    m_array_index_increment = mem_element.m_array_index_increment;
    m_data = mem_element.m_data;
    m_ref = ::osss::osssi::osss_memory_array_accessor<
      osss_array<DataType, Size>, 0, 0 >(this, 0);
    m_const_ref = ::osss::osssi::osss_memory_array_accessor<
      osss_array<DataType, Size>, 0, 0 >(this, 0);

    // copy m_data into the bit memory
    if (m_memory_ptr != NULL)
    {
      if (m_array_index_increment == 0)
	m_array_index_increment = calculate_array_increment();
      unsigned int array_index = 0;
      osssi::osss_serialisable_wrapper<DataType> tmp_data;
      for (typename osss_array<DataType, Size>::size_type i=0; i<Size; i++)
      {
        tmp_data = m_data[i];
        m_memory_ptr->write(&tmp_data, m_element_index, array_index);
	array_index = array_index + m_array_index_increment;
      }
    }
  }

  template<class DataType, unsigned int Size>
  osss_memory_element<osss_array<DataType, Size> >&
  osss_memory_element<osss_array<DataType, Size> >::
  operator=(const osss_memory_element& mem_element)
  {
    if (&mem_element != this)
    {
      m_memory_ptr = mem_element.m_memory_ptr;
      m_element_index = mem_element.m_element_index;
      m_array_index_increment = mem_element.m_array_index_increment;
      m_data = mem_element.m_data;
      m_ref = ::osss::osssi::osss_memory_array_accessor<
	osss_array<DataType, Size>, 0, 0 >(this, 0);
      m_const_ref = ::osss::osssi::osss_memory_array_accessor<
      osss_array<DataType, Size>, 0, 0 >(this, 0);

      // copy m_data into the bit memory
      if (m_memory_ptr != NULL)
      {
	if (m_array_index_increment == 0)
	  m_array_index_increment = calculate_array_increment();
	unsigned int array_index = 0;
	osssi::osss_serialisable_wrapper<DataType> tmp_data;
	for (typename osss_array<DataType, Size>::size_type i=0; i<Size; i++)
	{
	  tmp_data = m_data[i];
	  m_memory_ptr->write(&tmp_data, m_element_index, array_index);
	  array_index = array_index + m_array_index_increment;
	}
      }
    }
    return *this;
  }

  template<class DataType, unsigned int Size>
  void
  osss_memory_element<osss_array<DataType, Size> >::
  initialize_with(const osss_memory_element& mem_element)
  {
    if (&mem_element != this)
    {
      m_memory_ptr = mem_element.m_memory_ptr;
      m_element_index = mem_element.m_element_index;
      m_array_index_increment = mem_element.m_array_index_increment;
      m_data = mem_element.m_data;
      m_ref = ::osss::osssi::osss_memory_array_accessor<
	osss_array<DataType, Size>, 0, 0 >(this, 0);
      m_const_ref = ::osss::osssi::osss_memory_array_accessor<
      osss_array<DataType, Size>, 0, 0 >(this, 0);
    }
  }

  template<class DataType, unsigned int Size>
  osss_memory_element<osss_array<DataType, Size> >&
  osss_memory_element<osss_array<DataType, Size> >::
  operator=(const osss_array<DataType, Size>& array)
  {
    if (m_memory_ptr != NULL)
    {
      if (m_array_index_increment == 0)
	m_array_index_increment = calculate_array_increment();
      unsigned int array_index = 0;
      osssi::osss_serialisable_wrapper<DataType> tmp_data;
      for (typename osss_array<DataType, Size>::size_type i=0; i<Size; i++)
      {
        tmp_data = array[i];
        m_memory_ptr->write(&tmp_data, m_element_index, array_index);
	array_index = array_index + m_array_index_increment;
      }
    }
    else
      m_data = array;

    return *this;
  }

  template<class DataType, unsigned int Size>
  osss_memory_element<osss_array<DataType, Size> >::
  operator osss_array<DataType, Size>& () const
  { 
    if (m_memory_ptr != NULL)
    {
      if (m_array_index_increment == 0)
	m_array_index_increment = calculate_array_increment();
      unsigned int array_index = 0;
      osssi::osss_serialisable_wrapper<DataType> tmp_data;
      for (typename osss_array<DataType, Size>::size_type i=0; i<Size; i++)
      {
	m_memory_ptr->read(&tmp_data, m_element_index, array_index);
	m_data[i] = tmp_data;
	array_index = array_index + m_array_index_increment;
      }
    }
    return m_data;
  }

  template<class DataType, unsigned int Size>
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>, 0, 0 >&
  osss_memory_element<osss_array<DataType, Size> >::
  operator[](size_type index)
  {
    if (m_array_index_increment == 0)
      m_array_index_increment = calculate_array_increment();
    m_ref.set_index(index);
    return m_ref;
  }
  
  template<class DataType, unsigned int Size>
  const 
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>, 0, 0 >&
  osss_memory_element<osss_array<DataType, Size> >::
  operator[](size_type index) const
  {
    if (m_array_index_increment == 0)
      m_array_index_increment = calculate_array_increment();
    m_const_ref.set_index(index);
    return m_const_ref;
  }

  template<class DataType, unsigned int Size>
  unsigned int 
  osss_memory_element<osss_array<DataType, Size> >::
  calculate_array_increment() const
  {
    osssi::osss_serialisable_wrapper<DataType> data_buffer = DataType();
    double tmp_increment = 
      ceil((double)data_buffer.size() / (double)m_memory_ptr->data_width());
    if ((tmp_increment == 0.0f) && (data_buffer.size() != 0))
      return 1;
    else
      return (unsigned int)tmp_increment;
  }

 
  // Dumps the actual content of the array into an ostream.
  // Overloading of this operator is required by SystemC.
  template< class T, unsigned int Size >
  inline
  ::std::ostream &
  operator<<( ::std::ostream & os, 
              const osss::osss_memory_element<osss_array<T, Size> > & object )
  {
    for( unsigned int i = 0; i < Size; ++i )
    {
      os << object[i] << " ";
    }
    return os;
  }

  //------------------------------------------------------------------------------
  // partial specialisation for osss_arrays
  //------------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // get sizes
  //----------------------------------------------------------------------------
  
  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  typename 
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>::size_type
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  array_size() const
  {
    return Size;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  data_width() const
  {
    return DataWidth;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  address_width() const
  {
    return AddressWidth;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  size() const
  {
    return osss::osssi::pow<2,AddressWidth>::value;
  }

  //----------------------------------------------------------------------------
  // access
  //----------------------------------------------------------------------------

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>,
                                            DataWidth, AddressWidth>&
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  operator[](size_type index)
  {
    m_ref.set_index(index);
    return m_ref;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  write(const DataType& data, size_type index)
  {
    m_data_buffer = data;
    unsigned int data_offset = map_index_to_memory_address(index);

    // prepares the m_data_buffer for serialisation
    m_data_buffer.serialise_obj();

    // writes the whole object m_data_buffer in chunks to the memory
    while (!m_data_buffer.complete())
    {
      m_bit_storage[data_offset] = 
        m_data_buffer.template read_chunk< sc_dt::sc_biguint<DataWidth> >();

      if (sc_core::sc_start_of_simulation_invoked())
      {
	if (data_offset == map_index_to_memory_address(index))
	  wait_first_write();
	else
	  wait_write();
      }

      data_offset++;
    }
  }

  //! Allows to read from instances of class osss_array like from every 
  //! other array. If the reading is performed on an index which is not in the
  //! range of the actual array, an error will occur.

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  const ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>,
                                                  DataWidth, AddressWidth>&
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  operator[](size_type index) const
  {
    m_const_ref.set_index(index);
    return m_const_ref;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  const DataType&
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  read(size_type index) const
  {
    unsigned int data_offset = map_index_to_memory_address(index);

    m_data_buffer = DataType();

    while (!m_data_buffer.complete())
    {

      m_data_buffer.write_chunk(m_bit_storage[data_offset]); 

      if (sc_core::sc_start_of_simulation_invoked())
      {
	if (data_offset == map_index_to_memory_address(index))
	  wait_first_read();
	else
	  wait_read();
      }

      data_offset++;
    }
    // finallises the deserialisation of m_data_buffer
    m_data_buffer.deserialise_obj();

    return m_data_buffer;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>&
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  operator=(const osss_array<DataType, Size>& array)
  {
    for(size_type i=0; i<Size; i++)
    {
      this->write(array[i], i);
    }
    return *this;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  operator osss_array<DataType, Size>& () const
  { 
    for(size_type i=0; i<Size; i++)
    {
      m_array[i] = this->read(i);
    }
    return m_array;
  }

  //----------------------------------------------------------------------------
  // I/O functionality
  //----------------------------------------------------------------------------
  
  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  init_from_file(std::string in_file_name)
  {
    std::ifstream file(in_file_name.c_str());
    if (file.is_open())
    {
      scan(file);
      file.close();
    }
    else 
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__ 
                << ": Unable to open file "  
                << in_file_name << std::endl; 
      OSSS_ABORT(); 
    }
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  scan(std::istream& is)
  {
    sc_dt::sc_biguint<DataWidth> buffer;
    unsigned int addr = 0;
    
    std::string line;
    while (! is.eof() )
    {
      std::getline (is,line);
      if (line.length() != DataWidth)
      {
	std::cerr << "Error in " << __PRETTY_FUNCTION__
		  << ": line " << addr-1
		  << " has length " << line.length()
		  << ", excpected length " << DataWidth
		  << std::endl;
	OSSS_ABORT();
      }
      else
      {
	for(unsigned int i=0; i<DataWidth; i++)
        {
	  buffer.bit(DataWidth-1-i) = atoi(line.substr(i,1).c_str());
//          std::cout << atoi(line.substr(i,1).c_str());
        }
//        std::cout << std::endl;
      }
      m_bit_storage[addr] = buffer;
      
      //std::cout << std::hex << buffer.to_uint() << std::endl;

      if (addr == osss::osssi::pow<2, AddressWidth>::value-1)
	break;

      addr++;
    }
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  dump( ::std::ostream& os ) const
  {
    for( size_type i = 0; i < this->size(); ++i )
    {
      sc_dt::sc_biguint<DataWidth> tmp_line  = this->m_bit_storage[i];
      for(int j = tmp_line.length() - 1; j >= 0; --j)
	os << tmp_line.bit(j);
      os << std::endl;
    }
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  print( ::std::ostream& os ) const
  {
    os << *this << ::std::endl;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  initialize()
  {
    m_data_buffer = DataType();
    double tmp_offset = ceil((double)m_data_buffer.size() / (double)DataWidth);
    if ((tmp_offset == 0.0f) && (m_data_buffer.size() != 0))
      m_element_offset = 1;
    else
      m_element_offset = (unsigned int)tmp_offset;

    unsigned int required_address_lines = m_element_offset * Size;

    unsigned int available_address_lines =
      osss::osssi::pow<2, AddressWidth>::value;

    #if defined(OSSS_MEMORY_DEBUG)
    std::cout << "OSSS memory (for arrays) info for: " << name() << std::endl;
    std::cout << "----------------------------------" << std::endl;
      std::cout << "Data width: " << DataWidth 
		<< " Address width: " << AddressWidth << std::endl;
      std::cout << "Element size: " << m_data_buffer.size() << std::endl;
      std::cout << "Number of elements: " << Size << std::endl;
      std::cout << "Element offset: " << m_element_offset << std::endl;
      std::cout << "Required address lines: " << required_address_lines 
		<< " Available address lines: " << available_address_lines 
		<< std::endl << std::endl;
    #endif

    if (required_address_lines > available_address_lines)
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__
	<< ": required_address_lines (" << required_address_lines 
	<< ") > available_address_lines (" << available_address_lines << ")"
	<< std::endl;
      OSSS_ABORT();
    }
  }

  //----------------------------------------------------------------------------
  // specify timing of the memory 
  //----------------------------------------------------------------------------

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  wait_first_read() const
  {
    // the osss_memory is a perfect memory where acceses do not take any time
    // usually a certain amount of wait statements would follow, e.g.
    /*
      for (unsigned int i=0; i<NUMBER_OF_WAITS; i++)
        sc_core::wait();
    */
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  wait_read() const
  {
    // see above
  }
  
  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  wait_first_write() const
  {
    // see above
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  wait_write() const
  {
    // see above
  }
  
  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  unsigned int
  osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>:: 
  map_index_to_memory_address(unsigned int index) const
  { 
    return index * m_element_offset; 
  }

  template<class DataType, unsigned int DataWidth, unsigned int AddressWidth>
  inline
  ::std::ostream &
  operator<<( ::std::ostream & os, 
              const ::osss::osss_memory<DataType, 
                                        DataWidth, AddressWidth> & memory)
  {
    for(unsigned int i = 0; i < memory.size(); ++i)
    {
      sc_dt::sc_biguint<DataWidth> tmp_line  = memory.m_bit_storage[i];
      os << i << ": " << tmp_line.to_string(sc_dt::SC_BIN) << std::endl;
    }
    return os;
  }

//------------------------------------------------------------------------------
// internal helper classes for array accesses
//------------------------------------------------------------------------------

namespace osssi {

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  void 
  osss_memory_array_accessor<osss_array<DataType, Size>, 
                             DataWidth, AddressWidth>::
  set_index( typename 
	     ::osss::osss_memory<osss_array<DataType, Size>,
	     DataWidth, AddressWidth>::size_type index )
  {
    m_index = index;
  }

   template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  osss_memory_array_accessor<osss_array<DataType, Size>, 
                             DataWidth, AddressWidth>&
  osss_memory_array_accessor<osss_array<DataType, Size>, 
                             DataWidth, AddressWidth>::
  operator=(const DataType& data)
  {
    m_data_buffer = data;
    //store m_data_buffer into the memory
    m_memory_ptr->write(m_data_buffer, m_index);
    return *this;
  }

  template<class DataType, unsigned int Size, 
           unsigned int DataWidth,
           unsigned int AddressWidth>
  osss_memory_array_accessor<osss_array<DataType, Size>, 
                             DataWidth, AddressWidth>::
  operator DataType& () const
  {  
    //rebuild m_data_buffer from the memory
    m_data_buffer = m_memory_ptr->read(m_index);
    return m_data_buffer;
  }

  //----------------------------------------------------------------------------
  // partial specialisation for osss_arrays
  //----------------------------------------------------------------------------

  template<class DataType, unsigned int Size>
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >::
  osss_memory_array_accessor(const osss_memory_array_accessor& accessor)
  {
    m_data_buffer = accessor.m_data_buffer;
    m_mem_element_ptr = accessor.m_mem_element_ptr;
    m_index = accessor.m_index;
  }

  template<class DataType, unsigned int Size>
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >&
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >::  
  operator=(const osss_memory_array_accessor& accessor)
  {
    if (this != &accessor)
    {
      m_data_buffer = accessor.m_data_buffer;
      m_mem_element_ptr = accessor.m_mem_element_ptr;
      m_index = accessor.m_index; 
    }
    return *this;
  }

  template<class DataType, unsigned int Size>
  void
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >::  
  set_index( unsigned int index )
  {
    m_index = index;
  }

  template<class DataType, unsigned int Size>
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >&
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >::  
  operator=(const DataType& data)
  {
    if (m_mem_element_ptr != NULL)
    {
      osssi::osss_serialisable_wrapper<DataType> data_buffer;
      data_buffer = data;
      //store m_data_buffer into the memory
      unsigned int array_index = 	
	m_index * m_mem_element_ptr->m_array_index_increment;
      m_mem_element_ptr->
	m_memory_ptr->write(&data_buffer, 
			    m_mem_element_ptr->m_element_index,
			    array_index);
      return *this;
    }
    else
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__
		<< ": m_mem_element_ptr is NULL" << std::endl;
      OSSS_ABORT();
    }
  }

  template<class DataType, unsigned int Size>
  osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 >::  
  operator DataType& () const
  { 
    if (m_mem_element_ptr != NULL)
    {
      osssi::osss_serialisable_wrapper<DataType> data_buffer;
      //rebuild m_data_buffer from the memory
      unsigned int array_index = 	
	m_index * m_mem_element_ptr->m_array_index_increment;
      m_mem_element_ptr->
	m_memory_ptr->read(&data_buffer, 
			   m_mem_element_ptr->m_element_index, 
			   array_index);
      m_data_buffer = data_buffer;
      return m_data_buffer;
    }
    else
    {
      std::cerr << "Error in " << __PRETTY_FUNCTION__
		<< ": m_mem_element_ptr is NULL" << std::endl;
      OSSS_ABORT();
    }
  }

} // namespace osssi

} // namespace osss

// $Id: osss_memory.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
