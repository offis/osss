#include "Testbench.hh"
#include <iostream>

using namespace std;

Testbench::Testbench(sc_module_name module_name,
                     sc_time clockPeriod,
                     unsigned int numResetCylces) 
                     
                     : sc_module(module_name),
                       m_ClockPeriodHalf(clockPeriod/2.0),
                       m_NumResetCycles(numResetCylces)
{
  SC_THREAD(generateClockAndReset);
}

sc_time
Testbench::GetClockCyclePeriod() const
{
  return 2.0*m_ClockPeriodHalf;
}

void
Testbench::generateClockAndReset()
{
  po_bReset.write(true);
  po_bClk.write(true);

  for (unsigned int i=0; i<m_NumResetCycles; ++i)
  {
    wait(m_ClockPeriodHalf);
    po_bClk.write(false);
    wait(m_ClockPeriodHalf);
    po_bClk.write(true);
  }

  wait(m_ClockPeriodHalf);
  po_bReset.write(false);
  po_bClk.write(false);
  
  while(true)
  {
    wait(m_ClockPeriodHalf);
    po_bClk.write(true);
    wait(m_ClockPeriodHalf);
    po_bClk.write(false);
  }
}

