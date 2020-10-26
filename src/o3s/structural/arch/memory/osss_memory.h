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
#ifndef OSSS_MEMORY_INCLUDED
#define OSSS_MEMORY_INCLUDED

//#include "o3s/structural/arch/osss_architecture_object.h"
#include "o3s/structural/arch/memory/osss_memory_if.h"

#include <iostream>
#include <fstream>

namespace osss {
namespace osssi {

  template <unsigned int Base, unsigned int Exponent>
  struct pow
  {
    enum { value = Base * pow<Base, Exponent-1>::value };
  };

  template <unsigned int Base>
  struct pow<Base, 1> { enum { value = Base }; };

  template <unsigned int Base>
  struct pow<Base, 0> { enum { value = 1 }; };

  #define MIN_BIT_WIDTH(X) (::osss::osssi::min_bit_width<X>::value + 1)

  template <unsigned int N>
  struct min_bit_width
  {
    enum { value = 1 + min_bit_width<N/2>::value };
  };

  template <>
  struct min_bit_width<0> 
  {
    enum { value = 0 };
  };

  #define MIN_ARRAY_BIT_WIDTH(dataType, numElements, dataBitWidth) \
    (::osss::osssi::min_array_bit_width \
        <dataType, numElements, dataBitWidth>::value + 1)

  template<typename T, unsigned int numElements, unsigned int dataBitWidth>
  struct min_array_bit_width 
  {
    enum { 
      value = min_bit_width<(sizeof(T)*4*numElements)/dataBitWidth>::value 
    };
  };

  template<unsigned int numElements, unsigned int dataBitWidth>
  struct min_array_bit_width<bool, numElements, dataBitWidth>
  {
   enum { 
      value = min_bit_width<numElements/dataBitWidth>::value 
    }; 
  };

  // forward declaration of osss_memory_array_accessors

  template<class DataType, unsigned int DataWidth,
                           unsigned int AddressWidth>
  class osss_memory_array_accessor;

  template<class DataType, unsigned int Size, 
                           unsigned int DataWidth,
                           unsigned int AddressWidth>
  class osss_memory_array_accessor<osss_array<DataType, Size>, 
                                   DataWidth, AddressWidth>;

  template<class DataType, unsigned int Size>
  class osss_memory_array_accessor<osss_array<DataType, Size>, 0, 0 >;

} // namespace osssi

//#define OSSS_MEMORY_DEBUG 

//------------------------------------------------------------------------------
// osss_memory ( all Data Types used with OSSS memories need to be 
//               serialisable ! )
//------------------------------------------------------------------------------

template<class DataType, unsigned int DataWidth,
                         unsigned int AddressWidth>                
class osss_memory 
// this can not be an achitecture object because an osss_memory
// can possibly be a member of a Shared Object and thus be destroyed
// and rebuild during simulation. This dynamic behaviour is not allowed
// for SC Modules :-(
// A possible solution would be to use the osss_socket which provides
// a port to an osss_memory that is outside of the Shared Object itself.
//: public osss_architecture_object
{
  public:

  osss_memory() //: osss_architecture_object()
  {
    std::cerr << "Error: " << __PRETTY_FUNCTION__ 
              << " can only be used with serialisable objects!" << std::endl;
  }

  virtual ~osss_memory()
  {}
 
  private:

  osss_memory(const osss_memory& mem)
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__ 
              << ": calling the copy-constructor is not allwowed!" 
              << std::endl;
  }

  osss_memory& operator=(const osss_memory& mem)
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__
              << ": calling the assignment operator is not allowed!"
              << std::endl;
  }
};

//------------------------------------------------------------------------------
// partial specialisation for osss_serialisable_objects
//------------------------------------------------------------------------------

//forward declaration
template<class Type>
class osss_memory_element;

//forward declaration
template<class DataType, unsigned int Size>
class osss_memory_element<osss_array<DataType, Size> >;

template<unsigned int DataWidth, unsigned int AddressWidth>
class osss_memory<osss_serialisable_object, DataWidth, AddressWidth> : 
// @note: see above
//  public osss_architecture_object,
  public osss_memory_if,
  public osss_memory_timing_if
{

  template<class _DataType, unsigned int _DataWidth, 
                            unsigned int _AddressWidth>
  friend
  ::std::ostream &
  operator<<( ::std::ostream & os, 
	      const osss_memory<_DataType, 
                                _DataWidth, _AddressWidth> & memory);

  template<class DataType>
  friend
  class osss_memory_element;

  public:

  explicit osss_memory() : 
// @note: see above
//    osss_architecture_object(),
//      sc_core::sc_module_name(sc_core::sc_gen_unique_name("osss_memory"))),
    osss_memory_if(), 
    osss_memory_timing_if(),
    m_name( sc_core::sc_gen_unique_name("osss_memory") ),
    m_element_offset(0),
    m_element_index(1),
    m_element_offset_map(),
    m_bit_storage()
  {}

  explicit osss_memory( const char* name ) : 
// @note: see above
//sc_core::sc_module_name name) : 
//    osss_architecture_object(name), 
    osss_memory_if(), 
    osss_memory_timing_if(),
    m_name( name ),
    m_element_offset(0),
    m_element_index(1),
    m_element_offset_map(),
    m_bit_storage()
  {}

  explicit osss_memory( const char* name,
                        std::string file_name) :
// @note: see above
//sc_core::sc_module_name name, 
//std::string file_name) : 
//    osss_architecture_object(name), 
    osss_memory_if(), 
    osss_memory_timing_if(),
    m_name( name ),
    m_element_offset(0),
    m_element_index(1),
    m_element_offset_map(),
    m_bit_storage()
  {
    init_from_file(file_name);
  }

  virtual ~osss_memory()
  {}

  const std::string& name() const { return m_name; }

  virtual const char* kind() const { return "osss_memory"; }

  //----------------------------------------------------------------------------
  // get sizes
  //----------------------------------------------------------------------------
  
  virtual unsigned int data_width() const;
 

  virtual unsigned int address_width() const;
 

  virtual unsigned int size() const;
 

  //----------------------------------------------------------------------------
  // registration
  //----------------------------------------------------------------------------

  // registration for all kinds of serialise objects (except osss_arrays)
  template<class T>
  void add_element( osss_memory_element<T>& element );

  // registration for osss_arrays
  template<class T, unsigned int Size>
  void add_element( osss_memory_element<osss_array<T, Size> >& element );

  //----------------------------------------------------------------------------
  // I/O functionality
  //----------------------------------------------------------------------------
  
  void init_from_file(std::string in_file_name);

  void scan(::std::istream& is = ::std::cin);
 
  // Dumps the actual content of the memory into an ostream.
  void dump( ::std::ostream& os = ::std::cout ) const;
 
  void print( ::std::ostream& os = ::std::cout ) const;
 
 protected:

  //----------------------------------------------------------------------------
  // access
  //----------------------------------------------------------------------------

  virtual
  void write(::osss::osss_serialisable_object* ser_obj, 
             unsigned int index, unsigned int element_offset = 0);
 
  virtual
  void  read(::osss::osss_serialisable_object* ser_obj, 
             unsigned int index, unsigned int element_offset = 0) const;
 

  //----------------------------------------------------------------------------
  // specify timing of the memory 
  //----------------------------------------------------------------------------

  virtual void wait_first_read() const;
  virtual void wait_read() const;
  virtual void wait_first_write() const;
  virtual void wait_write() const;
 
 private:

  osss_memory(const osss_memory& mem)
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__ 
              << ": calling the copy-constructor is not allwowed!" 
              << std::endl;
  }

  osss_memory& operator=(const osss_memory& mem)
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__
              << ": calling the assignment operator is not allowed!"
              << std::endl;
  }

  unsigned int map_index_to_memory_address(unsigned int index) const;

  static const unsigned int m_available_address_lines = 
    osss::osssi::pow<2, AddressWidth>::value;

  std::string m_name;

  unsigned int m_element_offset;
  unsigned int m_element_index;
  std::map<const unsigned int, unsigned int> m_element_offset_map;

  osss_array<sc_dt::sc_biguint<DataWidth>, 
             osss::osssi::pow<2,AddressWidth>::value > m_bit_storage; 
};

//------------------------------------------------------------------------------
// osss_memory_element
//------------------------------------------------------------------------------

template<class DataType>
class osss_memory_element
{
 public:

  osss_memory_element()
    : m_memory_ptr(NULL),
      m_element_index(0),
      m_data()
  { }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  explicit 
  osss_memory_element(osss_memory<osss_serialisable_object, 
		                  DataWidth, AddressWidth>* mem_ptr,
		      unsigned int element_index)
    : m_memory_ptr(mem_ptr),
      m_element_index(element_index),
      m_data()
  { }

  osss_memory_element(const osss_memory_element& mem_element);
 
  osss_memory_element& operator=(const osss_memory_element& mem_element);
 
  osss_memory_element& operator=(const DataType& data);

  void initialize_with(const osss_memory_element& mem_element);
 
  operator DataType& () const;
 
 protected:

  osss_memory_if* m_memory_ptr;
  unsigned int m_element_index;

  mutable osssi::osss_serialisable_wrapper<DataType> m_data;

};

//------------------------------------------------------------------------------
// partial specialisation for osss_arrays
//------------------------------------------------------------------------------

template<class DataType, unsigned int Size>
class osss_memory_element<osss_array<DataType, Size> >
{

  template<class _DataType, unsigned int _DataWidth,
                            unsigned int _AddressWidth>
  friend
  class ::osss::osssi::osss_memory_array_accessor;

 public:

  typedef typename ::osss::osss_array<DataType, Size>::size_type size_type;

  osss_memory_element()
    : m_memory_ptr(NULL),
      m_element_index(0),
      m_array_index_increment(0),
      m_data(),
      m_ref(this, 0),
      m_const_ref(this, 0)
  { }

  template<unsigned int DataWidth, unsigned int AddressWidth>
  explicit 
  osss_memory_element(osss_memory<osss_serialisable_object, 
		                  DataWidth, AddressWidth>* mem_ptr,
		      unsigned int element_index)
    : m_memory_ptr(mem_ptr),
      m_element_index(element_index),
      m_array_index_increment(0),
      m_data(),
      m_ref(this, 0),
      m_const_ref(this, 0)
  { 
    m_array_index_increment = calculate_array_increment();
  }

  osss_memory_element(const osss_memory_element& mem_element);
 
  osss_memory_element& operator=(const osss_memory_element& mem_element);

  void initialize_with(const osss_memory_element& mem_element);
  
  osss_memory_element& operator=(const osss_array<DataType, Size>& array);

  operator osss_array<DataType, Size>& () const;
  
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>, 0, 0 >&
      operator[](size_type index);
 
  const 
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>, 0, 0 >&
    operator[](size_type index) const;
 
 protected:

  unsigned int calculate_array_increment() const;
 
  osss_memory_if* m_memory_ptr;
  unsigned int m_element_index;
  mutable unsigned int m_array_index_increment;

  mutable osss::osss_array<DataType, Size> m_data;
  
  ::osss::osssi::osss_memory_array_accessor<
      osss_array<DataType, Size>, 0, 0 > m_ref;
  
  mutable ::osss::osssi::osss_memory_array_accessor<
    osss_array<DataType, Size>, 0, 0 > m_const_ref;

};

//------------------------------------------------------------------------------
// partial specialisation for osss_arrays
//------------------------------------------------------------------------------

template<class DataType, unsigned int Size, 
         unsigned int DataWidth,
         unsigned int AddressWidth>
class osss_memory<osss_array<DataType, Size>, DataWidth, AddressWidth>  : 
// @note: see a above
//public osss_architecture_object, 
  public osss_memory_timing_if
{

  template<class _DataType, unsigned int _DataWidth, 
                            unsigned int _AddressWidth>
  friend
  ::std::ostream &
  operator<<( ::std::ostream & os, 
	      const osss_memory<_DataType, 
                                _DataWidth, _AddressWidth> & memory);

 public:

  typedef typename osss_array<DataType, Size>::size_type size_type;
  typedef DataType data_type;

  explicit osss_memory() : 
//    osss_architecture_object(),
//      sc_core::sc_module_name(sc_core::sc_gen_unique_name("osss_memory"))), 
    osss_memory_timing_if(),
    m_name( sc_core::sc_gen_unique_name("osss_memory") ),
    m_data_buffer(),
    m_bit_storage(),
    m_ref(this, 0),
    m_const_ref(this, 0),
    m_array()
  {
    this->initialize();
  }

  explicit osss_memory( const char* name ) ://sc_core::sc_module_name name) : 
//    osss_architecture_object(name), 
    osss_memory_timing_if(),
    m_name( name ),
    m_data_buffer(),
    m_bit_storage(),
    m_ref(this, 0),
    m_const_ref(this, 0),
    m_array()
  {
    this->initialize();
  }

  explicit osss_memory( const char* name,
                        std::string file_name) :
// @note: see above
//sc_core::sc_module_name name, 
//std::string file_name) : 
//    osss_architecture_object(name), 
    osss_memory_timing_if(),
    m_name( name ),
    m_data_buffer(),
    m_bit_storage(),
    m_ref(this, 0),
    m_const_ref(this, 0),
    m_array()
  {
    this->initialize();
    this->init_from_file(file_name);
  }

  virtual ~osss_memory()
  {}

  const std::string& name() const { return m_name; }

  virtual const char* kind() const { return "osss_memory"; }

  //----------------------------------------------------------------------------
  // get sizes
  //----------------------------------------------------------------------------
  
  //! \brief Returns the size of the array. 
  //!
  //! The size is determined by the 'Size' template argument
  size_type array_size() const;
  
  unsigned int data_width() const;
 
  unsigned int address_width() const;
 
  unsigned int size() const;
 
  //----------------------------------------------------------------------------
  // access
  //----------------------------------------------------------------------------

  //! Allows to assign values to instances of class osss_array like to 
  //! any other array. If a value is assigned to an index which is not in the 
  //! range of the actual array, an error will occur. 
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>,
                                            DataWidth, AddressWidth>&
  operator[](size_type index);
 
  void
  write(const DataType& data, size_type index);
 

  //! Allows to read from instances of class osss_array like from every 
  //! other array. If the reading is performed on an index which is not in the
  //! range of the actual array, an error will occur.
  const ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>,
                                                  DataWidth, AddressWidth>&
  operator[](size_type index) const;

  const DataType& read(size_type index) const;
 
  osss_memory& operator=(const osss_array<DataType, Size>& array);
 
  operator osss_array<DataType, Size>& () const;
 
  //----------------------------------------------------------------------------
  // I/O functionality
  //----------------------------------------------------------------------------
  
  void init_from_file(std::string in_file_name);
 
  void scan(::std::istream& is = ::std::cin );
 
  // Dumps the actual content of the memory into an ostream.
  void dump( ::std::ostream& os = ::std::cout ) const;

  void print( ::std::ostream& os = ::std::cout ) const;
 
 protected:

  void initialize();
 
  //----------------------------------------------------------------------------
  // specify timing of the memory 
  //----------------------------------------------------------------------------

  virtual void wait_first_read() const;
 
  virtual void wait_read() const;
   
  virtual void wait_first_write() const;
 
  virtual void wait_write() const;
 
 private:

  osss_memory(const osss_memory& mem)
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__ 
              << ": calling the copy-constructor is not allwowed!" 
              << std::endl;
  }

  osss_memory& operator=(const osss_memory& mem)
  {
    std::cerr << "Error in " << __PRETTY_FUNCTION__
              << ": calling the assignment operator is not allowed!"
              << std::endl;
  }

  unsigned int map_index_to_memory_address(unsigned int index) const;

  std::string m_name;
 
  mutable osssi::osss_serialisable_wrapper<DataType> m_data_buffer;

  osss_array<sc_dt::sc_biguint<DataWidth>, 
             osss::osssi::pow<2,AddressWidth>::value > m_bit_storage;

  unsigned int m_element_offset;
  
  ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>,
      DataWidth, AddressWidth> m_ref;

  const ::osss::osssi::osss_memory_array_accessor<osss_array<DataType, Size>,
    DataWidth, AddressWidth> m_const_ref;

  osss_array<DataType, Size> m_array;
};

//------------------------------------------------------------------------------
// internal helper classes for array accesses
//------------------------------------------------------------------------------

namespace osssi {

template<class DataType, unsigned int DataWidth = 0,
                         unsigned int AddressWidth = 0>                
class osss_memory_array_accessor
{

};

template<class DataType, unsigned int Size, 
         unsigned int DataWidth,
         unsigned int AddressWidth>
class osss_memory_array_accessor<osss_array<DataType, Size>, 
                                 DataWidth, AddressWidth> 
{

 public:

  explicit 
  osss_memory_array_accessor(
    ::osss::osss_memory<osss_array<DataType, Size>,
      DataWidth, AddressWidth>* mem_ptr,
    typename ::osss::osss_memory<osss_array<DataType, Size>,
      DataWidth, AddressWidth>::size_type index)
  : m_data_buffer(),
    m_memory_ptr(mem_ptr),
    m_index(index)
  {}

  void 
  set_index( typename 
	     ::osss::osss_memory<osss_array<DataType, Size>,
	     DataWidth, AddressWidth>::size_type index );
 
  osss_memory_array_accessor& operator=(const DataType& data);
 
  operator DataType& () const;
 
 private:

  osss_memory_array_accessor(const osss_memory_array_accessor& accessor)
  {}

  mutable DataType m_data_buffer;

  ::osss::osss_memory<osss_array<DataType, Size>, 
    DataWidth, AddressWidth>* m_memory_ptr;

  typename ::osss::osss_memory<osss_array<DataType, Size>,
    DataWidth, AddressWidth>::size_type m_index;

};

template<class DataType, unsigned int Size>
class osss_memory_array_accessor<osss_array<DataType, Size>, 0 , 0 > 
{

 public:

  osss_memory_array_accessor()
    :  m_data_buffer(),
       m_mem_element_ptr(NULL),
       m_index(0)
  {}

  explicit 
  osss_memory_array_accessor(
    ::osss::osss_memory_element<osss_array<DataType, Size> >* mem_element_ptr,
    unsigned int index)
  : m_data_buffer(),
    m_mem_element_ptr(mem_element_ptr),
    m_index(index)
  {}

  osss_memory_array_accessor(const osss_memory_array_accessor& accessor);
 
  osss_memory_array_accessor& 
  operator=(const osss_memory_array_accessor& accessor);
 
  void 
  set_index( unsigned int index );
 
  osss_memory_array_accessor& operator=(const DataType& data);
 
  operator DataType& () const;
 
 private:

  mutable DataType m_data_buffer;
  
  ::osss::osss_memory_element<osss_array<DataType, Size> >* m_mem_element_ptr;

  unsigned int m_index;

};

} // namespace osssi

} // namespace osss

#include "o3s/structural/arch/memory/osss_memory.tpp"

#endif
// $Id: osss_memory.h 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
