SC_MODULE(DUT)
{
  Memory input_buffer;
  Memory output_buffer;
  
  osss::osss_recon< PlayerBase<Memory> > decoder;
  osss::osss_context< PlayerBase< Memory > > view_of_process_one;
  osss::osss_context< PlayerBase< Memory > > view_of_process_two;
  
  void process_one()
  {
    OSSS_MESSAGE(true, "process one (re-)started");
    while (true)
    {
      // This process uses MP3 and AAC
           
      OSSS_MESSAGE(true, "Switching to MP3");
      view_of_process_one = MP3Player<Memory>();

      OSSS_MESSAGE(true, "Decoding MP3");
      view_of_process_one->decode(input_buffer, output_buffer);
      
      // We do not cover the whole loop body (or the loop in general)
      // in these sections. That would cause an deadlock for process two.
      
      OSSS_MESSAGE(true, "Switching to AAC");
      view_of_process_one = AACPlayer<Memory>();

      OSSS_MESSAGE(true, "Decoding AAC");
      view_of_process_one->decode(input_buffer, output_buffer);
    };
  }

  void process_two()
  {
    OSSS_MESSAGE(true, "process two (re-)started");
    while (true)
    {
      OSSS_MESSAGE(true, "Switching to MP3");
      view_of_process_two = MP3Player<Memory>();

      OSSS_MESSAGE(true, "Decoding MP3");
      view_of_process_two->decode(input_buffer, output_buffer);
      
      OSSS_MESSAGE(true, "Switching to OGG");
      view_of_process_two = OggVorbisPlayer<Memory>();

      OSSS_MESSAGE(true, "Decoding OGG");
      view_of_process_two->decode(input_buffer, output_buffer);
    };
  }


  sc_in< bool > clock;
  sc_in< bool > reset;

  SC_CTOR(DUT) : decoder("decoder_module")
  {
    decoder.clock_port(clock);
    decoder.reset_port(reset);

    SC_CTHREAD(process_one, clock);
    reset_signal_is(reset, true);
    uses(view_of_process_one);

    SC_CTHREAD(process_two, clock);
    reset_signal_is(reset, true);
    uses(view_of_process_two);
    
    view_of_process_one(decoder);
    view_of_process_two(decoder);
    
    // This is new: We use our own scheduler
    decoder.setScheduler< SimpleScheduler >();  
  }
};
