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
#ifndef __TOP_HH__
#define __TOP_HH__

#define OSSS_BLUE // Application Layer Model

#include "Master.h"
#include "Slave.h"
#include "APBMaster.h"

template< class Scheduler >
SC_MODULE( Top )
{
  //PORTS
  sc_in< bool > clock;
  sc_in< bool > resetn;
  
  sc_signal< sc_uint< 32 > > out;
  sc_signal< sc_uint< 32 > > out2;
  sc_signal< bool > pwrite;
  sc_signal< bool > penable;
  sc_signal< bool > psel0;
  sc_signal< bool > psel1;
  sc_signal< bool > psel2;
  sc_signal< sc_uint< 32 > > pwdata;
  sc_signal< sc_uint< 32 > > paddr;
  sc_signal< sc_uint< 32 > > prdata0;
  sc_signal< sc_uint< 32 > > prdata1;
  sc_signal< sc_uint< 32 > > prdata2;
  sc_signal< sc_uint< 32 > > prdata;
  
  //module instantiation
  Master< 1 > *master1;
  Master< 2 > *master2;
  
  osss_shared< Bridge, Scheduler > *bridge;
  
  APBMaster *apbmaster;
 
  Slave *slave1;
  Slave *slave2;
  Slave *slave3;
  
  SC_CTOR( Top ) :
    clock("clock"),
    resetn("resetn"),
    out("out"),
    out2("out2"),
    pwrite("pwrite"),
    penable("penable"),
    psel0("psel0"),
    psel1("psel1"),
    psel2("psel2"),
    pwdata("pwdata"),
    paddr("paddr"),
    prdata0("prdata0"),
    prdata1("prdata1"),
    prdata2("prdata2"),
    prdata("prdata")
  {
    SC_METHOD(mux_proc);
    sensitive << prdata0 << prdata1 << prdata2
              << psel0 << psel1 << psel2;

    bridge = new osss_shared< Bridge, Scheduler >( "bridge" );
    bridge->clock_port( clock );
    bridge->reset_port( resetn );

    master1 = new Master< 1 >( "master1" );
    master1->clock( clock );
    master1->resetn( resetn );
    master1->output( out );
    master1->bridge_write_port( *bridge );
    master1->bridge_write_port.setPriority(1);
    master1->bridge_read_port( *bridge );
    master1->bridge_read_port.setPriority(1);
     
    master2 = new Master< 2 >( "master2" );
    master2->clock ( clock );
    master2->resetn( resetn );
    master2->output( out2 );
    master2->bridge_write_port( *bridge );
    master2->bridge_write_port.setPriority(2);
    master2->bridge_read_port( *bridge );
    master2->bridge_read_port.setPriority(2);
  
    apbmaster = new APBMaster( "apbmaster" );
    apbmaster->pclk( clock );
    apbmaster->presetn( resetn );
    apbmaster->pwrite( pwrite );
    apbmaster->penable( penable );
    apbmaster->psel0( psel0 );
    apbmaster->psel1( psel1 );
    apbmaster->psel2( psel2 );
    apbmaster->pwdata( pwdata );
    apbmaster->paddr ( paddr );
    apbmaster->prdata( prdata );
    apbmaster->bridge_port( *bridge );
    apbmaster->bridge_port.setPriority(10);
  
    slave1 = new Slave("slave1");
    slave1->pclk( clock );
    slave1->presetn( resetn );
    slave1->pwrite( pwrite );
    slave1->penable( penable );
    slave1->psel( psel0 );
    slave1->pwdata( pwdata );
    slave1->paddr( paddr );
    slave1->prdata ( prdata0 );
  
    slave2 = new Slave("slave2");
    slave2->pclk( clock );
    slave2->presetn( resetn );
    slave2->pwrite( pwrite );
    slave2->penable( penable );
    slave2->psel( psel1 );
    slave2->pwdata( pwdata );
    slave2->paddr( paddr );
    slave2->prdata ( prdata1 );
  
    slave3 = new Slave("slave3");
    slave3->pclk( clock );
    slave3->presetn( resetn );
    slave3->pwrite( pwrite );
    slave3->penable( penable );
    slave3->psel( psel2 );
    slave3->pwdata( pwdata );
    slave3->paddr( paddr );
    slave3->prdata( prdata2 );
  }

 protected:

  void mux_proc()
  {
    if(psel0.read())
      prdata = prdata0.read();
    else if (psel1.read())
      prdata = prdata1.read();
    else if (psel2.read())
      prdata = prdata2.read();
  }

};

#endif // __TOP_HH__

