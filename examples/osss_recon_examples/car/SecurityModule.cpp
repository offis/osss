#include "SecurityModule.h"

#define OK_KEY 0x08154711

#include <systemc.h> //bring library symbols to scope

// This thread uses recon to check if the key is ok
void SecurityModule::securityThread()
{
  wait();
  while (pi_reset.read() == false){wait();}
#ifndef SYNTHESIS
  OSSS_MESSAGE(true, "reset high seen");
#endif
  po_unlocked.write(false);
  wait();
  while (pi_reset.read() == true) { wait(); }
#ifndef SYNTHESIS
  OSSS_MESSAGE(true, "reset low seen");
  OSSS_MESSAGE(true, "constructing context");
#endif
  crypto_context = Crypto();
#ifndef SYNTHESIS
  OSSS_MESSAGE(true, "context created, entering forever loop");
#endif
  while (po_unlocked.read() == false)
  {
    // new key: first do not trust
    po_unlocked.write(false);
    // fetch new key
    sc_dt::sc_uint< 32 > key = pi_keyword.read();
    bool unlock;
    // request a processing
#ifndef SYNTHESIS
    OSSS_MESSAGE(true, "wanting to decode new key " << unlock);
#endif
    unlock = crypto_context->keyOk(key, OK_KEY);
    // make result public
    po_unlocked.write(unlock);

#ifndef SYNTHESIS
    OSSS_MESSAGE(true, "lock state is now " << unlock);
#endif
    do
    {
      // wait for a change
      wait();
    }
    while (key == pi_keyword.read());
  }
}
