#include "Crypto.h"

using sc_dt::sc_uint;

bool
Crypto::keyOk(sc_dt::sc_uint< 64 > key, sc_dt::sc_uint< 64 > decoded)
{
  return (decoded == ~key);
}
