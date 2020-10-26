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

#include <systemc.h>

#define OSSS_GREEN

#include "osss.h"       // osss simulation library

#include "Testbench.h"  // testbench generates the reset and clock signal for 
                        // the top module of the system

#include "Top.h"


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
  //  to generate the graph type "make; make dot" it generates the file
  //  channel.png)
  osss_print_channel_graph(top.channel, "channel.dot");

  sc_start(sc_time (1000,SC_NS));
  
  return 0;
}
