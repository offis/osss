#include "EncodedSample.h"

#include <osss.h>
using std::exit;
using std::cout;
using std::endl;

EncodedSample::~EncodedSample()
{
}

osss_polymorphic< Codec >
EncodedSample::getCodec() const
{
#ifndef SYNTHESIS
    cout << sc_time_stamp() << ": "
         << __PRETTY_FUNCTION__ << " was called!" << endl;
    exit(EXIT_FAILURE);
#endif
}
