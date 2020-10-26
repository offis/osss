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
#ifndef OSSS_H_INCLUDED
#define OSSS_H_INCLUDED

// include OSSS
#include "osss"

// import SystemC namespace
// NOTE: SystemC itself is already included by OSSS,
//       the next line only imports the public symbols into the main
//       namespace
#include <systemc.h>

// o3s/algorithms
using ::osss::osss_scheduler;
using ::osss::osss_no_scheduler;
using ::osss::osss_least_recently_used;
using ::osss::osss_ceiling_priority;
using ::osss::osss_static_priority;
using ::osss::osss_round_robin;
using ::osss::osss_modified_round_robin;

#if defined(OSSS_INCLUDE_RECON)

  using ::osss::osss_recon;
  using ::osss::osss_context;
  using ::osss::osss_context_descriptor;

  using ::osss::osss_recon_job_idx;
  using ::osss::osss_device_job_idx;
  using ::osss::osss_type_idx;
  using ::osss::osss_slot_idx;
  using ::osss::osss_recon_idx;

  // OSSS+R algorithms
  using ::osss::osss_device_scheduler;
  using ::osss::osss_device_round_robin;

  using ::osss::osss_recon_scheduler;
  using ::osss::osss_recon_round_robin;
  using ::osss::osss_recon_modified_round_robin;
  using ::osss::osss_device_modified_round_robin;

  using ::osss::osss_placer;
  using ::osss::osss_first_fit_placer;
#endif

// o3s/datatypes
using ::osss::osss_object;
using ::osss::osss_polymorphic;
using ::osss::osss_array;
using ::osss::osss_vector;
using ::osss::osss_circ_buffer;
using ::osss::osss_enum;

using ::osss::osss_serialisable_object;

// do not be too restrictive here since serialisable
// datatypes should be usable in Application Layer models as well
using ::osss::osss_serialisable_array;
using ::osss::osss_serialisable_vector;
#if defined(OSSS_GREEN)
  using ::osss::osss_serialisable_archive;
#endif

// o3s/utils
using ::osss::osss_start;
using ::osss::osss_global_port_registry;
// Enum: osss_sensitivity
using ::osss::OSSS_POS;
using ::osss::OSSS_NEG;
using ::osss::OSSS_LEVEL;

// o3s/structural/arch
using ::osss::osss_memory;
using ::osss::osss_ram;

// o3s/communication
using ::osss::osss_port;
using ::osss::osss_export;

// o3s/communication/arch
using ::osss::osss_simple_point_to_point_channel;
using ::osss::osss_rmi_point_to_point_channel;
using ::osss::osss_socket;

// o3s/communication/arch/osss_channel
using ::osss::osss_method_if;
using ::osss::osss_master_if;
using ::osss::osss_slave_if;
using ::osss::osss_vector_in;
using ::osss::osss_vector_out;
using ::osss::osss_arbiter_if;
using ::osss::osss_address_decoder_if;
using ::osss::osss_address_map;
using ::osss::osss_channel;
using ::osss::osss_channel_if;
using ::osss::osss_port_to_channel;
using ::osss::osss_signal_if;
using ::osss::osss_basic_channel;
// Enum: osss_mux_controller
using ::osss::OSSS_ARBITER;
using ::osss::OSSS_ADDRESS_DECODER;

// o3s/communication/arch/osss_rmi
using ::osss::osss_rmi_channel;
using ::osss::osss_object_socket;
using ::osss::osss_port_to_rmi;

// o3s/communication/app
using ::osss::osss_port_to_shared;
using ::osss::osss_sw_port;

// o3s/communication/predefined

#if defined( OSSS_BLUE ) || defined( OSSS_GREEN )

using ::osss::osss_shared_fifo;
using ::osss::osss_fifo_port;
using ::osss::osss_fifo_in;
using ::osss::osss_fifo_out;
using ::osss::osss_fifo_if;
using ::osss::osss_fifo_put_if;
using ::osss::osss_fifo_get_if;
using ::osss::osss_fifo_buf;

using ::osss::osss_fifo_source;
using ::osss::osss_fifo_sink;

#endif // OSSS_BLUE || OSSS_GREEN

// o3s/structural/app
using ::osss::osss_shared;
using ::osss::osss_shared_if;
using ::osss::osss_sw_object_if;
using ::osss::osss_software_task;
using ::osss::osss_sw_shared_hw_if;
using ::osss::osss_sw_shared_port;
using ::osss::osss_sw_shared;

// o3s/structural/arch/memory
using ::osss::osss_memory;
using ::osss::osss_memory_element;

// o3s/structural/arch/system
using ::osss::osss_system;

// o3s/structural/arch/sw_processor
using ::osss::osss_processor;

#if defined( OSSS_INCLUDE_RECON )
  // o3s/structural/arch/system
  using ::osss::osss_device_type;
  using ::osss::osss_device;

  // o3s/algorithms
  using ::osss::osss_cycle_count;
  using ::osss::osss_precise_cycle_count;
#endif // OSSS_INCLUDE_RECON

#if defined( OSSS_INCLUDE_ELAB )
  using ::osss::osss_graph_elaborator;
  using ::osss::osss_print_elaborator;
  using ::osss::osss_aci_elaborator;
#endif

#if defined( OSSS_GREEN )
  using ::osss::xilinx::xilinx_opb_channel;
#endif

#if defined(OSSS_INCLUDE_XILINX)
  using ::osss::xilinx::xilinx_system;
  using ::osss::xilinx::xilinx_processor;
  using ::osss::xilinx::xilinx_microblaze;
  using ::osss::xilinx::xilinx_blockram;
//  using ::osss::xilinx::xilinx_external_port;
//  using ::osss::xilinx::xilinx_external_memory;
//  using ::osss::xilinx::xilinx_opb_hardware_block;
#endif

#if defined(OSSS_INCLUDE_ARM)
  using ::osss::arm::arm_architecture_object;
  using ::osss::arm::arm_ahb_channel;
#endif

#endif // OSSS_H_INCLUDED
// $Id: osss.h 3082 2009-03-27 13:10:54Z philipph $
// :flavour:(osss,h) (osss_icodes,h) (osss_recon,h) (osss_full,h)
