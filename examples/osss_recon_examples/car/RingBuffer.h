#ifndef RING_BUFFER_H
#define RING_BUFFER_H

template< class ElementType , int size >
class RingBuffer
{
   private:
   ElementType ring_buffer[size];
   int next_put_index;
   int next_get_index;
   // to tell between full and empty if get==put
   // no meaning if get!=put
   // (gets true if last modification was a get)
   bool last_action_was_get;

   public:
   RingBuffer();

   // true == buffer is full now
   bool put(ElementType sample);
   ElementType get();
   ElementType peek();
   bool isFull() const;
   bool isEmpty() const;

   inline
   bool
   operator==(const RingBuffer< ElementType, size> other)
   {
     return    (next_put_index == other.next_put_index)
            && (next_get_index == other.next_get_index)
            && (ring_buffer    == other.ring_buffer);
   }
};

#include "RingBuffer.icc"

#endif // RING_BUFFER_H
