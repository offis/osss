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

#define OSSS_GREEN

#include <systemc.h>

#include "Image.h"
#include "Testbench.h"

#include "osss.h"

typedef unsigned int Address;
const Address filter1_addr = 0;
const Address filter2_addr = 32;

class method_write_if : public osss_method_if
{
public:
  virtual void write(Address addr, Image i)=0;

  virtual ~method_write_if() {}
};


class method_read_if : public osss_method_if
{
public:
  virtual void read(Address addr, Image &i)=0;

  virtual ~method_read_if() {}
};

//------------------------------------------------------------------------------


class opb_master_if : public osss_master_if
{
public:

  sc_out< bool > M_request;
  sc_out< bool > M_select;
  sc_out< bool > M_RNW;
 
  sc_out< sc_uint< 32 > > M_ABus;
  sc_out< sc_uint< 32 > > M_DBus;
  sc_out< bool > M_DBusEn;

  sc_in< bool > OPB_MGrant;
  
  sc_in< sc_uint< 32 > > OPB_DBus;

  OSSS_GENERATE {
    // arbiter request signal
    osss_connect(osss_reg_port(M_request), osss_multi_signal("M_request"));

    osss_connect(osss_reg_port(M_select),  
		 osss_or(), 
		 osss_shared_signal("OPB_select"));

    osss_connect(osss_reg_port(M_RNW), 
		 osss_and_or(osss_reg_port(M_select)), 
		 osss_shared_signal("OPB_RNW"));

    osss_connect(osss_reg_port(M_ABus), 
		 osss_and_or(osss_reg_port(M_select)), 
		 osss_shared_signal("OPB_ABus"));

    osss_connect(osss_reg_port(M_DBus), 
		 osss_and_or(osss_reg_port(M_DBusEn)), 
		 osss_shared_signal("OPB_DBus"));

    // arbiter grant signal
    osss_connect(osss_multi_signal("OPB_MGrant"), osss_reg_port(OPB_MGrant));
 
    osss_connect(osss_shared_signal("OPB_DBus"), osss_reg_port(OPB_DBus));
  }

};

class opb_slave_if : public osss_slave_if
{
public:
 
  sc_out< bool > Sl_DBusEn;
  sc_out< sc_uint< 32 > > Sl_DBus;

  sc_in< bool > OPB_select;
  sc_in< bool > OPB_RNW;
 
  sc_in< sc_uint< 32 > > OPB_ABus;
  sc_in< sc_uint< 32 > > OPB_DBus;

  OSSS_GENERATE {
    osss_connect(osss_reg_port(Sl_DBus),
		 osss_and_or(osss_reg_port(Sl_DBusEn)), 
		 osss_shared_signal("OPB_DBus"));

    osss_connect(osss_shared_signal("OPB_select"), osss_reg_port(OPB_select));
    osss_connect(osss_shared_signal("OPB_RNW"), osss_reg_port(OPB_RNW));
  
    osss_connect(osss_shared_signal("OPB_ABus"), osss_reg_port(OPB_ABus));
    osss_connect(osss_shared_signal("OPB_DBus"), osss_reg_port(OPB_DBus));
  }

};


class opb_arbiter_if
  : public osss_arbiter_if
{
public:

  osss_vector_out< bool > OPB_MGrant;

  osss_vector_in< bool > M_request;
  sc_in< bool > OPB_select;

  OSSS_GENERATE {
    osss_connect(osss_reg_vector(OPB_MGrant), osss_multi_signal("OPB_MGrant"));

    osss_connect(osss_multi_signal("M_request"), osss_reg_vector(M_request));
    osss_connect(osss_shared_signal("OPB_select"), osss_reg_port(OPB_select));
  }
};

template< class SchedulerType >
class opb_arbiter : public opb_arbiter_if
{
public:
  
  OSSS_CONTROLLER_CTOR(opb_arbiter)
  {
    m_scheduler = new SchedulerType(getChannel()->getNumMasters());
    m_granted_master = 0;
    for(int i=0; i<M_request.size(); i++)
      m_last_requestVector.push_back(false);

    SC_METHOD(arbiter_process);
    sensitive << M_request;
  }

  void arbiter_process()
  {
    updateRequestVector();
  }

  void updateRequestVector()
  {
    
    cout << "update requestVector: num_masters: " 
	 << getChannel()->getNumMasters() << endl;

    bool * tmp_requestVector = new bool[getChannel()->getNumMasters()];
    for(int i=0; i<M_request.size(); ++i)
    {
      if (M_request.read(i) == true)
      {
	cout << "true  ";
	tmp_requestVector[i] = true;
      }
      else
      {
	cout << "false ";
	tmp_requestVector[i] = false;
      }
    }
    cout << endl;
    
    if (!m_last_requestVector.empty())
    {
      for(unsigned int i=0; i<getChannel()->getNumMasters(); ++i)
      {
	if (i == 0)
	  cout << "last: ";
	cout << m_last_requestVector[i] << " ";
      }
      cout << endl;
    }

    for(unsigned int i=0; i<getChannel()->getNumMasters(); ++i)
    {
      if (i == 0)
	cout << "tmp:  ";
      cout << tmp_requestVector[i] << " ";
    }
    cout << endl;

    // check if the request vector has changed
    if (m_last_requestVector[m_granted_master] != tmp_requestVector[m_granted_master])
    {
      //check if there is a request to schedule and call the scheduler
      for(unsigned int i=0; i<getChannel()->getNumMasters(); ++i)
      {
	if (tmp_requestVector[i] == true)
	{
	  m_granted_master = m_scheduler->schedule( tmp_requestVector );
	  cout << "scheduler called !!!" << endl << endl;
	  updateGrantVector();
	  break;
	}
      }
    }
    cout << "granted master: " << m_granted_master << endl;

    m_last_requestVector.clear();
    for(unsigned int i=0; i<getChannel()->getNumMasters(); ++i)
    {
      m_last_requestVector.push_back(tmp_requestVector[i]);
    }

    delete[] tmp_requestVector;
  }

  void updateGrantVector()
  {
    setSelect(m_granted_master);
    for(int i=0; i<OPB_MGrant.size(); i++)
    {
      OPB_MGrant.write(i ,false);
    }
    OPB_MGrant.write(m_granted_master, true);
  }

private:
  unsigned int m_granted_master;
  std::vector<bool> m_last_requestVector;
  osss_scheduler * m_scheduler;

};


//------------------------------------------------------------------------------  

class Channel
  : sc_core::sc_module
  , public osss_channel< opb_arbiter< osss_modified_round_robin > >
{
  typedef osss_channel< opb_arbiter< osss_modified_round_robin > >
          channel_type;
public:
    Channel( sc_core::sc_module_name name
               = sc_core::sc_gen_unique_name("Channel") )
      : sc_module( name )
      , channel_type( sc_module::name() )
    { }

    class Transactor_write : public method_write_if,
                             public opb_master_if
    {
      public:

        virtual void reset()
	{
	  M_request.write(false);
	  M_select.write(false);
	  M_RNW.write(false);
 
	  M_ABus.write(0);
	  M_DBus.write(0);
	  M_DBusEn.write(false);
	}
        
        virtual void write(Address addr, Image i)
	{
	  M_request.write(true);
	  while(OPB_MGrant.read() == false)
	    wait();
	  
	  M_select.write(true);
	  M_ABus.write(addr);
	  M_RNW.write(false);
 	   
	  for (int n=0; n<Image::numPixels(); ++n)
	  {
	    M_DBusEn.write(true);
	    M_DBus.write(i.getPixel(n));
	    wait();
	  }
	  
	  M_select.write(false);
	  M_DBusEn.write(false);
	  M_request.write(false);
	  wait();
	}
    };
    
    class Transactor_read : public method_read_if,
                                 public opb_slave_if
    {
      public:

        virtual void reset()
	{
	  Sl_DBusEn.write(false);
	  Sl_DBus.write(0);
	}
        	
	virtual void read(Address addr, Image &i)
        {
	  while((OPB_select.read() == false) || 
		(OPB_ABus.read() != addr))
	    wait();
	  
	  if (OPB_RNW.read() == false)
	  {
	  
	    for (int n=0; n<Image::numPixels(); ++n)
	    {
	      i.setPixel(n, OPB_DBus.read());
	      //cout << "read: data      = " << OPB_DBus.read() << endl;
	      wait();
	    }
	  }
	}
    }; 
    
  private:
      
};

OSSS_REGISTER_TRANSACTOR(Channel::Transactor_write, method_write_if)
OSSS_REGISTER_TRANSACTOR(Channel::Transactor_read, method_read_if)

//------------------------------------------------------------------------------

SC_MODULE(Filter)
{
  sc_in<bool> clock;
  sc_in<bool> reset; 

  //osss_port_to_channel<method_read_if> input;
  osss_port<osss_channel_if<method_read_if> > input;

  Filter(sc_module_name moduleName, Address addr, unsigned int waitCycles = 1);

  private:
  
    SC_HAS_PROCESS(Filter);
    
    void main();
     
    unsigned int m_waitCycles;  	
    Address m_addr;
};

Filter::Filter(sc_module_name moduleName, 
	       Address addr,
	       unsigned int waitCycles) : sc_module(moduleName), 
					  m_waitCycles(waitCycles),
					  m_addr(addr)
{
    SC_CTHREAD(main, clock.pos());
    reset_signal_is(reset, true);
}

void Filter::main()
{
  Image i;
  input->reset();
  wait();

  while(true)
  {
    cout << sc_time_stamp() << " " << name() << " filter unread ";
    i.print();
    cout << endl;
    
    input->read(m_addr, i);
  
    cout << sc_time_stamp() << " " << name() << " filter read ";
    i.print();
    cout << endl;
    
    i.clear();
    //wait(m_waitCycles);
  }
}

//-----------------------------------------------------------------------------

SC_MODULE(Camera)
{
  
  sc_in<bool> clock;
  sc_in<bool> reset; 
  
  // interface to the channel
  //osss_port_to_channel<method_write_if> output;
  osss_port<osss_channel_if<method_write_if> > output;

  void main();

  SC_CTOR( Camera )
  {
    SC_CTHREAD(main, clock.pos());
    reset_signal_is(reset, true);
  }

};

void Camera::main()
{
  Image i, i2;
  output->reset();
  wait(); // for resetting the system
  
  while(true)
  {  
    i.clear();

    i.setImage();

    cout << sc_clock::time_stamp() << " camera setImage i ";
    i.print();
    cout << endl;
  
    // writing to the channel
    output->write(filter1_addr, i);
    wait();
    
    i2.setImage();
    i2 = i + i2;

    cout << sc_clock::time_stamp() << " camera setImage i2 ";
    i2.print();
    cout << endl;

    // writing to the channel
    output->write(filter2_addr, i2);
    wait();
    
  }
}

//------------------------------------------------------------------------------

SC_MODULE(Camera2)
{
  
  sc_in<bool> clock;
  sc_in<bool> reset; 
  
  // interface to the channel
  //osss_port_to_channel<method_write_if> output;
  osss_port<osss_channel_if<method_write_if> > output;

  void main();

  SC_CTOR( Camera2 )
  {
    SC_CTHREAD(main, clock.pos());
    reset_signal_is(reset, true);
  }

};

void Camera2::main()
{
  Image i;
  sc_uint<8> cnt = 0;
  output->reset();
  wait(); // for resetting the system

  while(true)
  {  
    i.clear();

    ++cnt;
    i.fillImage(cnt);

    cout << sc_clock::time_stamp() << " camera2 setImage i ";
    i.print();
    cout << endl;
  
    // writing to the channel
    output->write(filter1_addr, i);
    wait(); 
  }
}

//-----------------------------------------------------------------------------

SC_MODULE(Top)
{
  
  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;

  Camera * cam;
  Camera2 * cam2;
  Filter * filter;
  Filter * filter2;
  Channel channel;


  SC_CTOR( Top )
  { 
    channel.clock_port(pi_bClk);
    channel.reset_port(pi_bReset);
    
    cam = new Camera( "cam" );
    cam->clock(pi_bClk);
    cam->reset(pi_bReset);
    cam->output(channel);

    cam2 = new Camera2( "cam2" );
    cam2->clock(pi_bClk);
    cam2->reset(pi_bReset);
    cam2->output(channel); 
  
    filter = new Filter( "filter", filter1_addr );
    filter->clock(pi_bClk);
    filter->reset(pi_bReset);
    filter->input(channel);

    filter2 = new Filter( "filter2", filter2_addr );
    filter2->clock(pi_bClk);
    filter2->reset(pi_bReset);
    filter2->input(channel);  

    channel.complete();
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
  
  cout << endl << endl 
       << top.channel
       << endl << endl;
  
  sc_trace_file* tf = sc_create_vcd_trace_file( "trace" );
  sc_trace( tf, s_clock, "clock" );
  sc_trace( tf, s_reset, "reset" );
  top.channel.trace_all(tf);

  // tracing of osss_vector's
  sc_trace( tf, top.channel.getArbiter()->M_request, "arbiterM_request");
  sc_trace( tf, top.channel.getArbiter()->OPB_MGrant, "arbiterOPB_MGrant");

  osss_print_channel_graph(top.channel, "bus.dot");
  
  sc_start(sc_time (1000,SC_NS));
  
  return 0;
  
}
