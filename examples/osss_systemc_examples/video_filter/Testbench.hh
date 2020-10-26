#ifndef TESTBENCH_INCLUDED
#define TESTBENCH_INCLUDED

#include <systemc.h>

// Non-synthesisable testbench code
class Testbench : public sc_module
{
  public:

    sc_out<bool>  po_bClk;
    sc_out<bool>  po_bReset;
  
    Testbench(sc_module_name moduleName,
              sc_time clockPeriod=sc_time(10.0, SC_NS),
              unsigned int numResetCylces=100);

    sc_time  GetClockCyclePeriod() const;

  private:

    SC_HAS_PROCESS(Testbench);

    void generateClockAndReset();

    sc_time       m_ClockPeriodHalf;
    unsigned int  m_NumResetCycles;
};

#endif
