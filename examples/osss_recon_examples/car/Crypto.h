#ifndef CRYPTO_H
#define CRYPTO_H

#include <osss>

class Crypto : public osss::osss_object
{
public:
  // OSSS_TAG(Crypto);
  bool keyOk(sc_dt::sc_uint< 64 > key, sc_dt::sc_uint< 64 > decoded);
};

#endif // CRYPTO_H
