#include <osss.h>

// We moved the "user defined classes" here, since
// they didn't change from "no_thrashing_audio" example.
#include "user_classes.h"

/* We now want to influence the online scheduling algorithm.
   So far, we modified the requests to the recon object.
   Whenever there were two alternative choices, the osss_recon
   object used a modified round robin strategy to detemine the
   next job. Now we want a different approach: We want to avoid
   reconfiguration times at all costs.
   Therefore we specify and use our own, simple scheduler which
   doesn't even honor fairness. Introducing fairness would be
   a nice reader's example :)
   Note, that this is an alternative to the OSSS_KEEP_ENABLED()
   statements used in the last example. Therefore we removed
   them here. Of course, you could combine both methods.
 */

// A simple, reconfiguration time aware scheduler
class SimpleScheduler : public osss_recon_scheduler
{
  virtual
  ~SimpleScheduler()
  {}

  virtual
  osss_recon_job_idx schedule()
  {
    bool found = false;
    osss_polymorphic< osss::osss_cycle_count > shortest_time = getCycleValue(0);
    osss_recon_job_idx best = 0;

    // Inspect all jobs
    //for (osss_recon_job_idx idx = 0; idx < jobs.size(); ++idx)
    for (osss_recon_job_idx idx = 0; idx < getNumberOfUserProcesses(); ++idx)
    {
      // We can rely on the fact, that at least one job is valid.
      // Otherwise we wouldn't have been called.
      if (jobs[idx].m_valid)
      {
        // If this is the first valid job
        // or a better one than before
        osss_polymorphic< osss::osss_cycle_count > duration;
        duration = jobs[idx].m_configuration_cycles
                 + jobs[idx].m_save_cycles
                 + jobs[idx].m_restore_cycles;
        if (!found || (duration < shortest_time))
        {
          // remember it
          shortest_time = duration;
          best = idx;
          found = true; // We've found at least one valid job
        }
      } // if
    } // for
    return best;
  }
};

// This is the same as before, execpt an additional statement
// in the constructor and the removal of OSSS_KEEP_ENABLED().
#include "module.h"

/* ------------------------------------------------------ */

int sc_main(int /*argc*/, char * /*argv*/[])
{
  sc_clock clock("main_clock", sc_time(10, SC_NS));
  sc_signal< bool > reset;
  DUT dut("design_under_test");
  dut.clock(clock);
  dut.reset(reset);

  osss::osss_device_type my_device_type("My FPGA device type");

  OSSS_DECLARE_TIME(my_device_type, PlayerBase<Memory>, sc_time(20, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, MP3Player<Memory>, sc_time(23, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, OggVorbisPlayer<Memory>, sc_time(22, SC_US), sc_time(200, SC_US));
  OSSS_DECLARE_TIME(my_device_type, AACPlayer<Memory>, sc_time(24, SC_US), sc_time(200, SC_US));

  osss::osss_device my_device(my_device_type, "my_device");
  my_device.clock_port(clock);
  my_device.reset_port(reset);

  dut.decoder( my_device );
  OSSS_MESSAGE(true, "Starting simulation");
  sc_start(sc_time(2, SC_MS));

  return EXIT_SUCCESS;
}

// see explanation.txt for further information
