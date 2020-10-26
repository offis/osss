/*------------------------------------------------------------------------------
 Copyright (c) 2005 Kuratorium OFFIS e.V., Oldenburg, Germany
  
 Name of the Library: OSSS (A library for synthesisable system level models 
                            in SystemC) 

 Created for Project: ICODES (http://icodes.offis.de)

 Created by: See AUTHORS (file)

 
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact:
 Kuratorium OFFIS e.V. (http://offis.de)
 Escherweg 2
 D-26121 Oldenburg
 PHONE: *49 (441) 9722-0
 FAX: *49 (441) 9722-102
 -----------------------------------------------------------------------------*/

/*
 * Brief description:
 *
 * This simple example connects a camera and a filter module through an 
 * osss channel which enables a simple point-to-point communication. The
 * camera continuously writes image objects to the channel, which are read by
 * the filter module.
 * While the camera and the filter module perform a method based communication
 * with the channel, transactors inside the channel enable the definition of
 * user defined communication protocols using SystemC signal ports.
 * In this example the channel is used to serialise a "big" image object in
 * order to transfer it via an 8 bit signal inside the channel.
 *
 */

#ifndef _CHANNEL_INCLUDE_
#define _CHANNEL_INCLUDE_

#include <systemc.h>

#define OSSS_GREEN

#include "osss.h"       // osss simulation library

#include "Image.h"      // data transferred through the channel


//------------------------------------------------------------------------------
// method interface section
//------------------------------------------------------------------------------

/*
 * method interface of a transactor which supplies a method to write an Image
 * object to the channel
 */
class method_write_if : public osss_method_if
{
public:
  virtual void write(Image i) = 0;
  
  virtual ~method_write_if() {}
};

/*
 * method interface of a transactor which supplies a method to read an Image
 * object from the channel
 */
class method_read_if : public osss_method_if
{
public:
  virtual void read(Image &i) = 0;

  virtual ~method_read_if() {}
};

//------------------------------------------------------------------------------
// signal interface section
//------------------------------------------------------------------------------

/*
 * signal interface of a transactor which declares two sc_out ports
 * and how these ports are connected to the shared signals inside the channel
 */
class signal_write_if : public osss_signal_if
{
public:
  sc_out< sc_uint<8> > data_out;
  sc_out<bool> valid_out;

  OSSS_GENERATE {
    osss_connect(osss_reg_port(data_out), osss_shared_signal("data_lines"));
    osss_connect(osss_reg_port(valid_out), osss_shared_signal("valid_line"));
  }
};

/*
 * signal interface of a transactor which declares two sc_in ports
 * and how these ports are connected to the shared signals inside the channel
 */
class signal_read_if : public osss_signal_if
{
public:
  sc_in< sc_uint<8> > data_in; 
  sc_in<bool> valid_in;

  OSSS_GENERATE {
    osss_connect(osss_shared_signal("data_lines"), osss_reg_port(data_in));
    osss_connect(osss_shared_signal("valid_line"), osss_reg_port(valid_in));
  }
};

//------------------------------------------------------------------------------
// channel (with transactors) declaration
//------------------------------------------------------------------------------

/*
 * channel of kind osss_basic_channel which enables simple point-to-point
 * connections with a single writer (and possibly multiple readers)
 */
class Channel : public osss_basic_channel
{
public:

  /*
   * transactor which implements the method_write_if interface and
   * uses the signal_write_if interface to write to the channel
   */
  class Transactor_write : public method_write_if,
			   public signal_write_if
  {
  public:

    virtual void reset()
    {
      valid_out.write(0);
      data_out.write(0);
    }
        
    virtual void write(Image i)
    {
      valid_out = true;
      wait(2);
      valid_out = false;
 	   
      for (int n=0; n<Image::numPixels(); ++n)
      {
	data_out = i.getPixel(n);
	wait();
      }
    }		
  };

   /*
   * transactor which implements the method_read_if interface and
   * uses the signal_read_if interface to read from the channel
   */
  class Transactor_read : public method_read_if,
			  public signal_read_if
  {
  public:

    virtual void reset()
    {
      // nothing to reset here!
    }
        	
    virtual void read(Image &i)
    {
      do { wait(); } while (valid_in == false);
      do { wait(); } while (valid_in == true);
          
      for (int n=0; n<Image::numPixels(); ++n)
      {
	i.setPixel( n, data_in.read() );
	wait();
      }
    }	     
  };   
     
private:
      
};

/*
 * registers a transactor at a mathod interface 
 * this allocation is needed to generate the appropriate transactor 
 * when binding a module to an osss_port
 */
OSSS_REGISTER_TRANSACTOR(Channel::Transactor_write, method_write_if);
OSSS_REGISTER_TRANSACTOR(Channel::Transactor_read, method_read_if);

#endif
