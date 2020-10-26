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

#define OSSS_GREEN

#include <systemc.h>

#include "osss.h"       // osss simulation library

#include "Image.h"      // data transferred through the channel
#include "Testbench.h"  // testbench generates the reset and clock signal for 
                        // the top module of the system

//transfer_type defines the type of the data signal
typedef sc_uint<8> transfer_type;


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
  virtual void write(Image &i) = 0;

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
  sc_out< transfer_type > data_out;
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
  sc_in< transfer_type > data_in; 
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
        
    virtual void write(Image &i)
    {
      valid_out = true;
      wait(2);
      valid_out = false;
      
      // prepares the object i for serialisation
      i.serialise_obj();
     
      // writes the whole object i in chunks to the data_out port
      while (!i.complete())
      {

	// writes a chunk of the object i to the data_out port
	// the size of the chunk read from i depends on the 
	// size of the port data_out
	i.write_chunk_to_port( data_out );

	/* alernative way:
         * reads a chunk of size transfer_type (which is the type of the
         * interface of port data_out) from i and writes it to data_out
         *
	 * data_out = i.read_chunk< transfer_type >();
         */

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

    virtual void reset() {}
        	
    virtual void read(Image &i)
    {
      do { wait(); } while (valid_in == false);
      do { wait(); } while (valid_in == true);

      // reads the whole object i in chunks to the data_in port
      while (!i.complete())
      { 
	// reads a chunk from the data_in port and stores it in the object
	// the size of the chunk stored in i depends on the size of the 
	// port data_in
	i.read_chunk_from_port( data_in );
	
	/* alternative way:
	 * reads from the data_in port and writes the data to the object i 
	 *
	 * i.write_chunk( data_in.read() );
	 */

	wait();
      } 
      // finallises the deserialisation of object i
      i.deserialise_obj();
    }	     
  };   
     
private:
      
};

/*
 * registers a transactor at a mathod interface 
 * this allocation is needed to generate the appropriate transactor 
 * when binding a module to an osss_port_to_channel
 */
OSSS_REGISTER_TRANSACTOR(Channel::Transactor_write, method_write_if);
OSSS_REGISTER_TRANSACTOR(Channel::Transactor_read, method_read_if);

//------------------------------------------------------------------------------
// declaration of modules which are connected to the channel
//------------------------------------------------------------------------------

SC_MODULE(Filter)
{
  sc_in<bool> clock;
  sc_in<bool> reset; 

  // interface to the channel
  //osss_port_to_channel< method_read_if > input;
  osss_port<osss_channel_if<method_read_if> > input;

  Filter(sc_module_name moduleName, unsigned int waitCycles=1);

 private:
  
  SC_HAS_PROCESS(Filter);
    
  void main();
     
  unsigned int m_waitCycles;  		
};

Filter::Filter(sc_module_name moduleName, unsigned int waitCycles) 
  : sc_module(moduleName),
    m_waitCycles(waitCycles)
{
  SC_CTHREAD(main, clock.pos());
  reset_signal_is(reset, true);
}

void 
Filter::main()
{
  Image i;
  input->reset();
  
  wait(); // for resetting the system
  while(true)
  {
    cout << sc_time_stamp() << " " << name() << " filter unread ";
    i.print();
    cout << endl;
    
    // reading from the channel (calls the read method of the method_read_if
    // which is implemented in the Channel::Transactor_read class)
    input->read(i);
  
    cout << sc_time_stamp() << " " << name() << " filter read ";
    i.print();
    cout << endl;
    
    i.clear();
    wait(m_waitCycles);
  }
}

//-----------------------------------------------------------------------------

SC_MODULE(Camera)
{
  
  sc_in<bool> clock;
  sc_in<bool> reset; 
  
  // interface to the channel
  //osss_port_to_channel< method_write_if > output;
  osss_port<osss_channel_if<method_write_if> > output;

  void main();


  SC_CTOR( Camera )
  {
    SC_CTHREAD(main, clock.pos());
    reset_signal_is(reset, true);
  }

};

void 
Camera::main()
{
  Image i, i2;
  i.setImage();
  i2.clear();
  output->reset();
  
  wait(); // for resetting the system
  while(true)
  {  
    cout << sc_clock::time_stamp() << " camera setImage i ";
    i.print();
    cout << endl;
  
    // writing to the channel (calls the write method of the method_write_if
    // which is implemented in the Channel::Transactor_write class)
    output->write(i);
    wait();
    
    i2 = i + i2;

    cout << sc_clock::time_stamp() << " camera setImage i2 ";
    i2.print();
    cout << endl;

    // writing to the channel
    output->write(i2);
    wait();
    
  }
}

//-----------------------------------------------------------------------------
// top module declaration
//   ( the declaration of a top module is very important, because the
//     it embeds the channel inside a module and thus enables the usage of
//     wait() statements inside transactors )
//-----------------------------------------------------------------------------

SC_MODULE(Top)
{
  
  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;

  Camera *cam;
  Filter *filter;
  Channel channel;  // channel instantiation

  SC_CTOR( Top )
  {  
    channel.clock_port(pi_bClk);
    channel.reset_port(pi_bReset);

    cam = new Camera( "cam" );
    cam->clock(pi_bClk);
    cam->reset(pi_bReset);
    
    // binds the cam module to the channel
    //
    // This binding generates the Channel::Transactor_write transactor because
    // it has been registered for the method_write_if through the
    // OSSS_REGISTER_TRANSACTOR(Channel::Transactor_write, method_write_if)
    // macro. Additionally this binding connects the generated transactor to
    // the shared signals inside the channel as denoted inside the OSSS_GENERATE
    // block of the signal_write_if
    cam->output(channel);
  
    filter = new Filter( "filter" );
    filter->clock(pi_bClk);
    filter->reset(pi_bReset);
    
    // binds the filter module to the channel
    //
    // This binding generates the Channel::Transactor_read transactor because
    // it has been registered for the method_read_if through the
    // OSSS_REGISTER_TRANSACTOR(Channel::Transactor_read, method_read_if)
    // macro. Additionally this binding connects the generated transactor to
    // the shared signals inside the channel as denoted inside the OSSS_GENERATE
    // block of the signal_read_if
    filter->input(channel);
  }
};

//-----------------------------------------------------------------------------

int sc_main(int, char*[])
{
  Top top("top");
  Testbench test("test");

  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;

  test.po_bClk(s_clock);
  test.po_bReset(s_reset);
  
  top.pi_bClk(s_clock);
  top.pi_bReset(s_reset);

  // some debug information about the generated internal connections of the channel
  cout << endl << endl 
       << top.channel
       << endl << endl;
  
  sc_trace_file* tf = sc_create_vcd_trace_file( "trace" );
  sc_trace( tf, s_clock, "clock" );
  sc_trace( tf, s_reset, "reset" );

  // traces all signals generated inside the channel
  top.channel.trace_all(tf);

  // visualise the channel by a graph
  // (you need to have dot installed [http://www.graphviz.org/]
  //  to generate the graph type "make dot" it generates the file
  //  channel.png)
  osss_print_channel_graph(top.channel, "channel.dot");

  sc_start(sc_time (1000,SC_NS));
  
  return 0;
}
