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
   This i2c channel has been inspired and derived from the PhD thesis:

   "Durchgängiges Kommunikationsdesign für den strukturalen, 
    objektorientierten Hardware-Entwurf"

   by Wolfram Putzke-Röming
   April 2001
*/

#define OSSS_GREEN

#include <string>

#include <systemc.h>
#include "Testbench.h"

#include "osss.h"

using namespace std;

#define HIGH true
#define LOW false

typedef sc_uint<7> Address;
typedef sc_uint<8> CompleteAddress;
typedef sc_uint<8> SendData;
typedef sc_uint<8> ReceiveData;

//-----------------------------------------------------------------------------

class master_method_if : public osss_method_if
{
public:
  virtual void master_send_byte(Address addr, SendData sendData) = 0;
  virtual void master_receive_byte(Address addr, ReceiveData &recData) = 0;

  virtual ~master_method_if() {}
};

class slave_method_if : public osss_method_if
{
public:
  virtual void slave_action_byte(Address &addr, 
				 SendData &sendData, ReceiveData &recData) = 0;

  virtual ~slave_method_if() {}
};

//------------------------------------------------------------------------------

class master_signal_if : public osss_signal_if
{
public:
  sc_inout<bool> shiftData;
  sc_out<bool> shiftClock;

  OSSS_GENERATE {
    osss_connect(osss_reg_port(shiftData), osss_shared_signal("shiftData"));
    osss_connect(osss_reg_port(shiftClock), osss_shared_signal("shiftClock"));
  }
};

class slave_signal_if : public osss_signal_if
{
public:
  sc_inout<bool> shiftData;
  sc_in<bool> shiftClock;

  OSSS_GENERATE {
    osss_connect(osss_shared_signal("shiftData"), osss_reg_port(shiftData));
    osss_connect(osss_shared_signal("shiftClock"), osss_reg_port(shiftClock));
  }
};

//------------------------------------------------------------------------------

enum State
{
  waiting, listen_for_address, address_accepted, 
  read_data, write_data, wait_for_stop_or_restart,
  stop_condition, restart_condition
};

class Channel : public osss_basic_channel
{
public:
       
  class Transactor_master: public master_signal_if, 
                           public master_method_if
  {
  public:

    virtual void reset()
    {
      shiftData.write(0);
      shiftClock.write(0);
    }
	              
    // intentional private methods for master
    void set_start_condition()
    {
      shiftData.write(HIGH);
      shiftClock.write(HIGH);
      wait();
      shiftData.write(LOW);
      wait();
      shiftClock.write(LOW);
      wait();       
    }
	
    void write_address(CompleteAddress &completeAddress, bool &sendStatus)
    {
      for (int i=0; i<8; i++)
      {
	wait();
	shiftData.write(completeAddress[i]);
	wait();
	shiftClock.write(HIGH);
	wait(2);
	shiftClock.write(LOW);
	wait();
      }
      //wait for acknowledge
      shiftData.write(HIGH);
      wait();
      shiftClock.write(HIGH);
      wait();
      if ( shiftData.read() == 0)
      {
	sendStatus = true; //sending ok
	wait();
	shiftClock.write(LOW);      
      }
      else
      {
	sendStatus = false;
	wait();
	shiftClock.write(LOW);      
      }
    }
	
    void write_byte(SendData &sendData, bool &sendStatus)
    {
      for(int i=0; i<8; i++)
      {
	wait();
	shiftData.write(sendData[i]);
	wait();
	shiftClock.write(HIGH);
	wait(2);
	shiftClock.write(LOW);
	wait();     
      }
      // wait for acknowledge
      shiftData.write(HIGH);
      wait();
      shiftClock.write(HIGH);
      wait();
      if ( shiftData.read() == 0)
      {
	sendStatus = true; //sending ok
	wait();
	shiftClock.write(LOW);      
      }
      else
      {
	sendStatus = false;
	wait();
	shiftClock.write(LOW);      
      }
    }
	
    void receive_byte(ReceiveData &receiveData)
    {   
      for (int i=0; i<8; i++)
      {
	wait();
	shiftClock.write(HIGH);
	wait();
	receiveData[i] = shiftData.read();
	cout << "  DEBUG  " << " receive_byte " << receiveData[i] << endl;
	wait();
	shiftClock.write(LOW);
	wait();
      }
      // set acknowledge  
      shiftData.write(HIGH);
      wait();
      shiftClock.write(HIGH);
      wait(2);
      shiftClock.write(LOW);
      wait();
    }
	
    void set_stop_condition()
    {
      wait();
      shiftData.write(LOW);
      wait();
      shiftClock.write(HIGH);
      wait();
      shiftData.write(HIGH);
      wait();
      wait();
    }

    void set_rec_stop_condition()
    {
      shiftData.write(LOW);
      wait();
      shiftClock.write(HIGH);
      wait();
      shiftData.write(HIGH);
      wait();
    }
	
    void set_restart_condition()
    {
      wait();
      shiftClock.write(HIGH);
      shiftData.write(HIGH);
      wait();
      shiftData.write(LOW);
      wait();
      shiftClock.write(LOW);
      wait();
    }
	
    void switch_off()
    {
      shiftClock.write(HIGH);
      shiftData.write(HIGH);
      wait();
    }
	
    // public methods for master
    virtual void master_send_byte(Address addr, SendData sendData)
    {
      sc_uint< 8 > completeAddr;
      bool sendStatus = false;  // false =  error, true = ok

      cout << "master_send_byte: Addresse: "<<addr <<endl;
      cout << "master_send_byte: sendData: "<<sendData <<endl;
	        
      completeAddr.range(6,0) = addr;
      completeAddr[7] = 0;  

      cout << "master_send_byte: Addresse_complete: "<<completeAddr <<endl;
	        
      set_start_condition();
      write_address(completeAddr, sendStatus);
      write_byte(sendData, sendStatus);
      set_stop_condition();
      switch_off();
    }
	
    virtual void master_receive_byte(Address addr, ReceiveData &recData)
    {
      CompleteAddress completeAddr;
      bool sendStatus = false;  // false =  error, true = ok
      
      cout << "master_receive_byte: Addresse: "<<addr <<endl;
      cout << "master_receive_byte: receiveData: "<<recData <<endl;
	            
      completeAddr.range(6,0) = addr;
      completeAddr[7] = 1;

      cout << "master_receive_byte: Addresse_complete: "<<completeAddr <<endl;
	        
      set_start_condition();
      write_address(completeAddr, sendStatus);
      receive_byte(recData);
      set_stop_condition();  
      switch_off();
    }  
  };
    
 
  //===========================================================================

  class Transactor_slave: public slave_signal_if,
                          public slave_method_if
  {
  public:
        
    State current_state;	
    
    bool sda_old, sda_new, scl_old, scl_new;   
    CompleteAddress completeAddress;                              
    bool rwMode;                                                  
        
    bool not_stop_loop;                                                                
    bool reading_ok;                                                                  
      
    // next version maybe an image case                                                 
    ReceiveData data; 

    virtual void reset() {}
  
    // public method for slave
    virtual void slave_action_byte(Address &addr, SendData &sendData, 
				   ReceiveData &recData)
    {
      bool finished = false;
          
      current_state = waiting;
	                    
      while (!finished)
      {

	switch ( current_state ) 
	{
	  case waiting:
	    // waiting for start_condition                                                  
	    // if start condition listen for address                                        
	  
	    sda_old = shiftData.read();                                                     
	    scl_old = shiftClock.read();                                                    
	    wait();                                                                         
	    sda_new = shiftData.read();                                                     
	    scl_new = shiftClock.read();                                                    
  	    
	    if ( ((sda_old == 1) && (sda_new == 0)) && 
		 ((scl_old == 1) && (scl_new == 1)) ) 
	    {                                                                               
	      current_state = listen_for_address;                                              
	    }                                                                               
	    else                                                                            
	    {                                                                               
	      current_state = waiting;                                                         
	    }                                                                               
	    break;                                                                          
 
            
	case listen_for_address:                                                        
	  // if address equals own address  -> address_accepted()                       
	  // otherwise -> waiting()                                                     
	  // listen as well for the R/W signal                                          

	  for (int i=0; i<8; i++)                                                       
	  {                                                                             
	    while(shiftClock.read() != 0)                                               
	    {                                                                           
	      wait();                                                                   
	    }                                                                           
	    while(shiftClock.read() != 1)                                               
	    {                                                                           
	      wait();                                                                   
	    }                                                                           
	    completeAddress[i] = shiftData.read();                                      
	  }     
                
	  cout << "DEBUG " << sc_time_stamp() << completeAddress << 
	          "  OWNaddress" << addr << endl; 
                                                                                        
	  rwMode = completeAddress[7];                                                  
	  if (completeAddress.range(6,0) == addr)                                       
	  {                                                                             
	    current_state = address_accepted;                                           
	  }                                                                             
	  else                                                                          
	    {                                                                             
	      current_state = waiting;                                                       
	    }                                                                             
	  break;                                                                        
              
 
	case address_accepted:
	  // analyse the R/W bit  -> read_data  or write_data  
	                                                                   
	  //confirm received address                           
	  while(shiftClock.read() != 0)                        
	  {                                                    
	    wait();                                            
	  }                                                    
	  wait();                                              
	  shiftData.write(LOW); //acknowledge                  
	  wait();                                              
	  while(shiftClock.read() != 1)                        
	  {                                                    
	    wait();                                            
	  }                                                    
	  while(shiftClock.read() == 1)                        
	  {                                                    
	    wait();                                            
	  }                                                    
	                                                                   
	  cout <<" RW MODE "<< rwMode <<endl;                  
	                                                                   
	  // data from master to slave                         
	  if (rwMode == 0)                                     
	  {                                                    
	    current_state = read_data;          
	  }                                                    
	  // data from slave to master                         
	  else                                                 
	  {                                                    
	    current_state = write_data;         
	  }                                                    
	  break;
            

	case read_data:
	  reading_ok = false;                                                                  
	  not_stop_loop = true;                                                                
                
	  cout << " READ_DATA "<< endl;                                                                    
	  for(int i=0; i<8; i++)                                                                    
	  {                                                                                         
	    while(shiftClock.read() != 0)                                                           
	    {                                                                                       
		wait();                                                                               
	    }                                                                                       
	    while(shiftClock.read() != 1)                                                           
	    {                                                                                       
	      wait();                                                                               
	    }                                                                                       
	    while( (shiftClock.read()==1) && not_stop_loop )                                        
	    {                                                                                       
	      sda_old = shiftData.read();                                                         
	      scl_old = shiftClock.read();                                                        
	      wait();                                                                             
	      sda_new = shiftData.read();                                                         
	      scl_new = shiftClock.read();                                                        
                                                                                                          
	      if ( ((sda_old == 0) && (sda_new == 1)) && 
		   ((scl_old == 1) && (scl_new == 1)) )     
	      {                                                                                   
		cout << "STOP_CONDITION"  << endl;
                        
		not_stop_loop = false;                                                              
		current_state = stop_condition;
	      }                                                                                   
	    }                                                                                       
	    if (not_stop_loop == false)                                                             
	    {                                                                                       
	      current_state = stop_condition;
	      break;                                                                                
	    }                                                                                       
	    else                                                                                    
	    {                                                                                       
		data[i] = sda_old;                                                                    
	    }                                                                                       
	  }                                                                                         
	  if (current_state != stop_condition)
	  {                                                                                         
	    //confirm received data                                                                 
	    while(shiftClock.read() != 0)                                                           
	    {                                                                                       
	      wait();                                                                               
	    }                                                                                       
	    shiftData.write(LOW);                                                                   
	    wait();                                                                                 
	    reading_ok =  true;                                                                     
	    while(shiftClock.read() != 1)                                                           
	    {                                                                                       
	      wait();                                                                               
	    }                                                                                       
	    while(shiftClock.read() == 1)                                                           
	    {                                                                                       
	      wait();                                                                               
	    }                                                                                       
	    if (reading_ok)                                                                         
	    {                                                                                       
	      recData = data;                                                                       
	      cout << " REC_DATA in READ_DATA action byte " << recData << endl;
	    }                                                                                       
	  }                                                                                         
	  break;
                
	case write_data:                                                                              
	  for (int i=0; i<8; i++)                                              
	  {                                                                    
	    while(shiftClock.read() != 0)                                      
	    {                                                                  
	      wait();                                                            
	    }                                                                  
	    wait();                                                            
	    shiftData.write( sendData[i] );                                    
	    wait();                                                            
	    while(shiftClock.read() != 1)                                      
	    {                                                                  
	      wait();                                                            
	    }                                                                  
	  }                                                                    
	  while(shiftClock.read() != 0)                                        
	  {                                                                    
	    wait();                                                            
	  }                                                                    
	  while(shiftClock.read() != 1)                                        
	  {                                                                    
	    wait();                                                            
	  }                                                                    
	  wait();                                                              
	  if (shiftData.read() == 0)                                           
	  {                                                                    
	    //stays in write_data                                              
	    cout << "Stays in WRITE" << endl;
	    current_state = write_data;                                          
	  }                                                                    
	  else                                                                 
	  {                                                                    
	      current_state = wait_for_stop_or_restart;           
	  }                                                                    
	  break;
            
	case wait_for_stop_or_restart:
	  not_stop_loop = true;                                                            
	  
	  while(shiftClock.read() != 0)                                                         
	  {                                                                                     
	    wait();                                                                             
	  }                                                                                     
	  while(shiftClock.read() != 1)                                                         
	  {                                                                                     
	    wait();                                                                             
	  }                                                                                     
	  while( (shiftClock.read() == 1) && not_stop_loop )                                    
	  {                                                                                     
	    sda_old = shiftData.read();                                                         
	    scl_old = shiftClock.read();                                                        
	    wait(),                                                                             
	      sda_new = shiftData.read();                                                         
	    scl_new = shiftClock.read();                                                        
	    if ( ((scl_old == 1) && (scl_new == 1)) && 
		 ((sda_old == 0) && (sda_new == 1)) )     
	    {                                                                                   
	      cout << "STOP STOP"<< endl;
	      current_state = stop_condition;
	      not_stop_loop = false;                                                            
	    }                                                                                   
	    else                                                                                
	    {                                                                                   
	      if ( ((scl_old == 1) && (scl_new == 1)) && 
		   ((sda_old == 1) && (sda_new == 0)) )   
	      {                                                                                 
		current_state = restart_condition;                             
		not_stop_loop = false;                                                                
	      }                                                                                 
	    }                                                                                   
	  }                                                                                     
	  break;
     
	case stop_condition:
	  current_state = waiting;
	  finished = true;
	  break;

	case restart_condition:
	  current_state = listen_for_address;
	  break;
	} 
      }
    }
	     
  };  
     
private:
    
};

OSSS_REGISTER_TRANSACTOR(Channel::Transactor_master, master_method_if)
OSSS_REGISTER_TRANSACTOR(Channel::Transactor_slave,  slave_method_if)

//------------------------------------------------------------------------------

SC_MODULE(Slave)
{
  sc_in<bool> clock;
  sc_in<bool> reset; 

  //osss_port_to_channel<slave_method_if> input;
  osss_port<osss_channel_if<slave_method_if> > input;

  Slave(sc_module_name moduleName,
            sc_uint< 7 > address=1);

 private:
  
  SC_HAS_PROCESS(Slave);
    
  void main();
     
  sc_uint< 7 > m_address;  		
};


Slave::Slave(sc_module_name moduleName,
               sc_uint< 7 > address) 
	       : 
	       sc_module(moduleName), 
	       m_address(address)
{
  SC_CTHREAD(main, clock.pos());
  reset_signal_is(reset, true);
}

void Slave::main()
{
  SendData sendData;
  ReceiveData recData, i;

  i = 0; 
  sendData = 130;
  input->reset();
  
  wait();
  cout << endl << name() << " m_address " << m_address << endl << endl;
  while(true)
  {
    cout << sc_time_stamp() << " " << name() << " filter unread ";
    cout <<i;
    cout << endl << endl;
    
    input->slave_action_byte(m_address, sendData, recData);
    
    i = recData;
      
    cout << sc_time_stamp() << " " << name() << " filter read ";
    cout << i;
    cout << endl;
  
    i = 0;  
    wait();
  }
}

//------------------------------------------------------------------------------

SC_MODULE(Master)
{
  
  sc_in<bool> clock;
  sc_in<bool> reset; 
  
  // interface to the channel
  //osss_port_to_channel<master_method_if> output;
  osss_port<osss_channel_if<master_method_if> > output;

  void main();

  SC_CTOR( Master )
  {
    SC_CTHREAD(main, clock.pos());
    reset_signal_is(reset, true);
  }

};

void Master::main()
{
  SendData i, i2, i3;
  sc_uint< 7 > address1 = 10;
  sc_uint< 7 > address2 = 1;
  
  i  = 129;
  i2 = 8;
  i3 = 0;
  output->reset();
  
  wait(); // for resetting the system
  while(true)
  {  
    cout << sc_clock::time_stamp() << " camera setImage i ";
    cout << i;
    cout << endl;
  
    // writing to the channel
    output->master_send_byte(address1, i);
    wait();
    
    cout << sc_clock::time_stamp() << " camera setImage i2 ";
    cout << i2;
    cout << endl;

    // writing to the channel
    output->master_send_byte(address2, i2);
    wait();
    
    cout << sc_clock::time_stamp() << " camera setImage i3 ";
    cout << i3;
    cout << endl;
    
    // reading from the slave via the channel
    output->master_receive_byte(address2, i3);

    cout << sc_clock::time_stamp() << " camera READ i3 ";
    cout << i3;
    cout << endl;

    wait();
  }
}

//------------------------------------------------------------------------------


SC_MODULE(Top)
{
  
  sc_in<bool>   pi_bClk;
  sc_in<bool>   pi_bReset;

  Master *master;
  Slave *slave;
  Slave *slave2;
  Channel channel;


  SC_CTOR( Top )
  {  
    channel.clock_port(pi_bClk);
    channel.reset_port(pi_bReset);

    master = new Master( "master" );
    master->clock(pi_bClk);
    master->reset(pi_bReset);
    master->output(channel);
  
    slave = new Slave( "slave", 1 );
    slave->clock(pi_bClk);
    slave->reset(pi_bReset);
    slave->input(channel);

    slave2 = new Slave( "slave2", 10 );
    slave2->clock(pi_bClk);
    slave2->reset(pi_bReset);
    slave2->input(channel);  
  }

};


//------------------------------------------------------------------------------

int sc_main(int, char*[])
{

  sc_report_handler::set_actions (SC_ID_MORE_THAN_ONE_SIGNAL_DRIVER_ ,
                                  SC_DO_NOTHING);
 
  
  Top top("top");
  Testbench test("test");

  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;

  test.po_bClk(s_clock);
  test.po_bReset(s_reset);
  
  top.pi_bClk(s_clock);
  top.pi_bReset(s_reset);

  // some debug information about the internals of the channel
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
  //  to generate the graph type "make; make dot" it generates the file
  //  channel.png)
  osss_print_channel_graph(top.channel, "channel.dot");
 
  sc_start(sc_time (10000,SC_NS));
  
  return 0;
}
