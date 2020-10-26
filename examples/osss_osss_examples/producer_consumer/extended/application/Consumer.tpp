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

#ifndef VERBOSE_CONSUMER
#define VERBOSE_CONSUMER
#endif

template<unsigned int ConsumerNr>
Consumer<ConsumerNr>::Consumer(sc_core::sc_module_name name) : 
  input("input") {
  SC_CTHREAD(cons_process, clk.pos());
  reset_signal_is(reset, true);
}
  
template<unsigned int ConsumerNr>
void 
Consumer<ConsumerNr>::cons_process() {
  const unsigned int max_nr_packets = 1000;

#ifdef PERFORM_ANALYSIS
  bool first_run = true;
  sc_core::sc_time start_time;
  sc_core::sc_time stop_time;
#endif // PERFORM_ANALYSIS

  unsigned int packet_counter = 0;
  Packet p;

  while(packet_counter < max_nr_packets) {

/*
    // something like this has to be done when using osss_round_robin
    // or osss_modified_round_robin as SO scheduler
    if (ConsumerNr == 1)
      wait(5);
    else if (ConsumerNr == 2)
      wait(7);
*/

    // this works well with osss_least_recently_used<MAX_NR_CLIENTS>
    // as SO scheduler
    wait();

    p = input->get();
    packet_counter++;
#ifdef VERBOSE_CONSUMER
    std::cout << "Consumer " << ConsumerNr << " "
              << "@ " << sc_core::sc_time_stamp() << ":"
              << " got packet nr. " << packet_counter << ": "
              << p
              << std::endl;
#endif // VERBOSE_CONSUMER

#ifdef PERFORM_ANALYSIS
    if (first_run)
    {
      first_run = false;
      start_time = sc_core::sc_time_stamp();
    }
    else if(packet_counter == max_nr_packets)
    {
      stop_time = sc_core::sc_time_stamp();
      sc_core::sc_time sim_duration = stop_time - start_time;
      for(unsigned int i=0; i<80; ++i)
        std::cout << "=";
      std::cout << std::endl;
      std::cout << "Consumer " << ConsumerNr << ": "
                << "duration for " << packet_counter << " packets: "
                << sim_duration << std::endl;
      for(unsigned int i=0; i<80; ++i)
        std::cout << "=";
      std::cout << std::endl;
    }
#endif // PERFORM_ANALYSIS
  }

  // only consumer nr 1 forces the simulation to stop
  if (ConsumerNr == 1)
    sc_core::sc_stop();
}
