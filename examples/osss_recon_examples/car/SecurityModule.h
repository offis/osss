#ifndef SECURITY_MODULE_HH
#define SECURITY_MODULE_HH

#include "Defines.h"
#include "GuardedRingBuffer.h"
#include "Crypto.h"

#include <osss>

SC_MODULE( SecurityModule )
{
  // Fast system clock
  sc_core::sc_in< bool > pi_clock;
  sc_core::sc_in< bool > pi_reset;

  // The keyword from the sensor (fingerprint,
  // keycard, ...)
  sc_core::sc_in< sc_dt::sc_uint< 64 > > pi_keyword;
  // We drive this "true" if keyword is ok:
  // system unlocked
  sc_core::sc_out< bool > po_unlocked;

  osss::osss_context< Crypto > crypto_context;

  void securityThread();

  SC_CTOR( SecurityModule )
  {
    SC_CTHREAD( securityThread, pi_clock );
    reset_signal_is( pi_reset, true );
    uses( crypto_context );
  }
};

#endif //SECURITY_MODULE_HH
