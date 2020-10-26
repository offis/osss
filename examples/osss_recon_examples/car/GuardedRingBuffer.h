#ifndef GUARDED_RING_BUFFER_H
#define GUARDED_RING_BUFFER_H

#include "RingBuffer.h"

#include <osss>

using sc_core::sc_interface;
using sc_core::sc_out;

template< class ElementType , int size >
class GuardedRingBuffer 
  : public RingBuffer< ElementType, size >,
    public sc_interface
{
   private:
   void
   updateStatusInterface();

   public:

   // OSSS_GUARDED methods block the caller
   // until the guard condition (third argument)
   // is true. Only when the guard is true the
   // arbiter may grant access to the calling
   // process.

   // returns true if buffer is full
   OSSS_GUARDED_METHOD( bool,
                        put,
                        OSSS_PARAMS(1, ElementType, sample),
                        !isFull() );
   OSSS_GUARDED_METHOD( ElementType,
                        get,
                        OSSS_PARAMS(0),
                        !isEmpty() );

   inline
   bool
   isFull() const
   {
     return RingBuffer< ElementType, size >::isFull();
   }

   inline
   bool
   isEmpty() const
   {
     return RingBuffer< ElementType, size >::isEmpty();
   }

   // Demand interface for this class.
   // The outside uses these ports to tell
   // if it would get blocked by calling put() or get().
   // We update them within updateStatusInterface(),
   // called by put() and get()
   OSSS_REQUIRED(state_if)
   {
      sc_out<bool>   po_full;
      sc_out<bool>   po_empty;
   };
};

#include "GuardedRingBuffer.icc"

#endif // GUARDED_RING_BUFFER_H
