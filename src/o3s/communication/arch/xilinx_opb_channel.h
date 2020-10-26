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

#ifndef XILINX_OPB_CHANNEL_HH_INCLUDED
#define XILINX_OPB_CHANNEL_HH_INCLUDED

#include "xilinx_opb_channel_impl.h"
#include "xilinx_opb_traits.h"
#include "xilinx_opb.h"

namespace osss {
namespace xilinx {
/**
 *  
 * \section HowTheBusIsUsed How the bus is used
 *
 * This section will give a short overview of how the Xilinx version of the 
 * OPB-Bus is used by this module. 
 *
 *
 *
 * \subsection WritingDataFromMaster How the master writes
 *
 * The method \c write_blocking is called by the master. The transactor takes
 * the given vector and sends it to the slave via the bus. In order to do so,
 * the transactor first requests the bus. If the master receives the \c grand
 * signal from the arbiter it starts the transfer. If the given vector contains
 * more than four bytes the bus is locked, in order to assure, that the
 * transfer is not interrupted. Besides the \c busLock signal the signal
 * \c seqAddr is also set. This indicates the slave, that the current transfer
 * is a bursttransfer and that more data will follow. After the datasignals
 * have been set up, the transactor waits for the acknowledge from the slave,
 * which should be asserted in the same cycle.
 *
 * As long as there are more than three bytes to be send left, the whole
 * bus-width is used. The bits 0 to 7 contain the first byte, the bits 8 to
 * 15 contain the second one and so on. If there are less that four bytes to
 * be send only the first datalines of the bus are used. Again the first byte
 * is send using the bits 0 to 7 and so on. The remaining bits of the bus are
 * unused and fixed to 0. The \c byteenable is set correctly to indicate which
 * lines are used for the transfer.
 *
 * If the current cycle is the last one, e.G. there are no more bytes to send,
 * the \c seqAddr is deasserted to indicate that this is the last cycle. If the
 * checksum feature is enabled (see \ref Checksum) \c seqAddr keeps asserted,
 * because, the checksum will be transfered in the last cycle.
 *
 * If the \c errAck or \c retry signal is asserted by the slave, the transfer
 * is aborted and the transactor returns \c false. A successful transfer will
 * always return \c true.
 *
 *
 *
 *
 * \subsection WritingDataToSlave How the slave is been written
 *
 * The slave has a slightly different view on the transfer. The Slave must
 * have called \c wait_for_action. Within this method the slave process will
 * be blocked until the slave is accessed. If so, this is detected on the
 * rising edge of the clock signal, therefore the slave could not response
 * immediately to the access. It has at least a one cycle latency. The \c toutsup
 * signal is asserted immediately in order to prevent a bus timeout. 
 *
 * If the transactor is accessed it will return the control to the Slave. The
 * slave is now able to start the appropriate action. If data s written to the
 * slave it musst call \c read_blockin, in order to read the data from the bus.
 * At the end of each cycle (at the rising edge) the data from the bus is saved
 * in the given vector. The \c byteenable signal is used to determine which
 * bytes contain vaild information. The transactor assumes the first byte in the
 * first datalines that contain valid information regarding to \c byteenable.
 * The data might contain \e gabs. The following transfer is valid: \n
 * <TABLE>
 *    <TR><TD>databits</TD><TD> BE </TD><TD>Databyte</TD></TR>
 *    <TR><TD> 0 to  7</TD><TD> 1  </TD><TD>   0    </TD></TR>
 *    <TR><TD> 8 to 15</TD><TD> 0  </TD><TD>   -    </TD></TR>
 *    <TR><TD>16 to 23</TD><TD> 1  </TD><TD>   1    </TD></TR>
 *    <TR><TD>24 to 32</TD><TD> 0  </TD><TD>   -    </TD></TR>
 * </TABLE>
 *
 * The \c byteenable signal might differ in each cycle. As long as the
 * \c seqAddr is asserted the transactor expects more data to come. If the
 * signal is deasserted, the current cycle is threaded as the last one and
 * the method will return the control to the slave.
 *
 * If the checksum feature is enabled, the slave will check the checksum. It
 * is assumed, that the last four bytes of the vector contain the checksum.
 * These bytes are removed and the checksum is verified. If the checksum is
 * valid, the slave will return \c true, otherwise \c false is returned.
 *
 *
 *
 *
 * \subsection ReadingToMaster How the master reads
 * In order to let the master be able to read from the slave, the method
 * interface had to be changed. The master needs to know the size of the
 * object to be read from the salve. Otherwise the transactor does not know
 * the number of bytes to read from the slave and therefore it can not perform
 * the correct number of cycles.
 *
 * The transfer will be like mentioned above, except, that the direction is
 * reversed. If the checksum feature is enabled, the master will check if the
 * received checksum and the calculated one are equal. If the checksums differ,
 * the method will return \c false. In all other cases the method will return
 * \c true.
 *
 *
 *
 * \subsection ReadingFromSlave How the slave is been read
 * Reading from the slave is performed like writing to the slave. Of course
 * the direction of the data is reverted. If the master tries to read more
 * data than the slave can offer, the slave fails an assertion. In case the
 * checksum features is enabled the last transfercycle is used by the slave
 * in order to transfer the checksum.
 */


template<bool DynamicPriority = true, bool RegisteredGrants = false>
class xilinx_opb_channel
  : public xilinx_opb
  , public internal::xilinx_opb_traits
           <DynamicPriority,RegisteredGrants>::channel_type
{
public:
  typedef internal::xilinx_opb_traits<
      DynamicPriority, RegisteredGrants>     traits_type;

  typedef typename traits_type::channel_type base_type;

  typedef typename base_type::data_chunk     data_chunk;
  typedef typename base_type::data_type      data_type;
  typedef typename base_type::address_type   address_type;
  typedef typename base_type::action_type    action_type;

  explicit
  xilinx_opb_channel(sc_core::sc_module_name name_)
    : xilinx_opb(name_, DynamicPriority, RegisteredGrants)
    , base_type( sc_core::sc_module::name() )
  {}

protected:

  virtual void before_end_of_elaboration()
  {
    // call base type elaboration
    xilinx_opb::before_end_of_elaboration();

    base_type::check_clock_reset_port();
  }

}; /* class xilinx_opb_channel */

} // namespace xilinx
} // namespace osss

#endif // XILINX_OPB_CHANNEL_HH_INCLUDED
// $Id: xilinx_opb_channel.h 2839 2008-12-04 09:29:59Z philipph $
// :flavour: (osss,h) (osss_recon,h) (osss_icodes,h) (osss_full,h)
