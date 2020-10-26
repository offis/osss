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

//---------------------------------------------------------------------------
// unidirectional point-to-point channel
//---------------------------------------------------------------------------

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, false>::writer:: 
reset()
{
  valid_out.write(0);
  data_out.write(0);
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, false>::writer:: 
write_blocking(data_chunk& data)
{
  start_transaction();

  for(size_t i=0; i<data.size(); i++)
  {
    data_out.write(data[i]);
    sc_core::wait();
  }
      
  return true;
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, false>::writer:: 
write_blocking(osss_serialisable_object& ser_obj)
{
  start_transaction();
      
  // prepares the object ser_obj for serialisation
  ser_obj.serialise_obj();
     
  // writes the whole object ser_obj in chunks to the data_out port
  while (!ser_obj.complete())
  {

    // writes a chunk of the object ser_obj to the data_out port
    // the size of the chunk read from ser_obj depends on the 
    // size of the port data_out
    ser_obj.write_chunk_to_port( data_out );

    /* alernative way:
     * reads a chunk of size transfer_type (which is the type of the
     * interface of port data_out) from ser_obj and writes it to data_out
     *
     * data_out = ser_obj.readChunk< transfer_type >();
     */

    sc_core::wait();
  }

  return true;
}

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, false>::writer:: 
start_transaction()
{
  valid_out = true;
  sc_core::wait(2);
  valid_out = false;
}

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, false>::reader::
reset()
{

}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, false>::reader:: 
read_blocking(data_chunk& data)
{
  if (data.empty())
    return false;
      
  wait_for_transaction();

  for(size_t i=0; i<data.size(); i++)
  {
    data[i] = data_in.read();
    sc_core::wait();
  }
      
  return true;
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, false>::reader:: 
read_blocking(osss_serialisable_object& ser_obj)
{
  wait_for_transaction();
        
  // reads the whole object ser_obj in chunks to the data_in port
  while (!ser_obj.complete())
  { 
    // reads a chunk from the data_in port and stores it in the object
    // the size of the chunk stored in ser_obj depends on the size of the 
    // port data_in
    ser_obj.read_chunk_from_port( data_in );
	
    /* alternative way:
     * reads from the data_in port and writes the data to the object ser_obj
     *
     * ser_obj.write_chunk( data_in.read() );
     */

    sc_core::wait();
  } 
  // finallises the deserialisation of object ser_obj
  ser_obj.deserialise_obj();

  return true;
}

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, false>::reader:: 
wait_for_transaction()
{
  // wait_until(valid_in.delayed() == true)
  while( valid_in.read() != true ) sc_core::wait();
  // wait_until(valid_in.delayed() == false)
  while( valid_in.read() != false ) sc_core::wait();
}

//---------------------------------------------------------------------------
// bidirectional point-to-point channel
//---------------------------------------------------------------------------

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
reset()
{
  valid_inout.write(0);
  data_inout.write(0);
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
write_blocking(data_chunk& data)
{
  start_transaction();
  
  for(size_t i=0; i<data.size(); i++)
  {
    data_inout.write(data->at(i));
    sc_core::wait();
  }
      
  return true;
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
write_blocking(osss_serialisable_object& ser_obj)
{
  start_transaction();
      
  // prepares the object ser_obj for serialisation
  ser_obj.serialise_obj();
     
  // writes the whole object ser_obj in chunks to the data_out port
  while (!ser_obj.complete())
  {

    // writes a chunk of the object ser_obj to the data_out port
    // the size of the chunk read from ser_obj depends on the 
    // size of the port data_out
    ser_obj.write_chunk_to_port( data_inout );

    /* alernative way:
     * reads a chunk of size transfer_type (which is the type of the
     * interface of port data_out) from ser_obj and writes it to data_out
     *
     * data_out = ser_obj.read_chunk< transfer_type >();
     */

    sc_core::wait();
  }

  return true;
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
read_blocking(data_chunk& data)
{
  if (data.empty())
    return false;
      
  wait_for_transaction();

  for(size_t i=0; i<data.size(); i++)
  {
    data[i] = data_inout.read();
    sc_core::wait();
  }
      
  return true;
}

template<unsigned int dataWidth >
inline
bool
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
read_blocking(osss_serialisable_object& ser_obj)
{
  wait_for_transaction();
        
  // reads the whole object ser_obj in chunks to the data_in port
  while (!ser_obj.complete())
  { 
    // reads a chunk from the data_in port and stores it in the object
    // the size of the chunk stored in ser_obj depends on the size of the 
    // port data_in
    ser_obj.read_chunk_from_port( data_inout );
	
    /* alternative way:
     * reads from the data_in port and writes the data to the object ser_obj
     *
     * ser_obj.write_chunk( data_in.read() );
     */

    sc_core::wait();
  } 
  // finallises the deserialisation of object ser_obj
  ser_obj.deserialise_obj();

  return true;
}

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
start_transaction()
{
  valid_inout = true;
  sc_core::wait(2);
  valid_inout = false;
}

template<unsigned int dataWidth >
inline
void
osss_simple_point_to_point_channel<dataWidth, true>::read_writer::
wait_for_transaction()
{
  // wait_until(valid_inout.delayed() == true)
  while( valid_inout.read() != true ) sc_core::wait();
  // wait_until(valid_inout.delayed() == false);
  while( valid_inout.read() != false )sc_core::wait();
}
// $Id: osss_simple_point_to_point_channel_impl.tpp 2392 2008-07-01 20:45:11Z philipph $
// :flavour:(osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
