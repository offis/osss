#
# OSSS simulation library
# - flavour specific additions -
#

# Note: Any of these make(1) commands/statements are evaluated
#       only during the processing of any target for this flavour.

$(FLAVOUR)_DESC    := OSSS simulation library
$(FLAVOUR)_VERSION := $(OSSS_VERSION)

# Add some additional files to distribution
$(FLAVOUR)_DIST +=                                      \
	doc/*.pdf                                       \
	doc/reflection                                  \
	examples/Makefile*                              \
	examples/osss_channel_examples                  \
	examples/osss_osss_examples                     \
	examples/osss_systemc_examples                  \
	examples/osss_recon_examples/README             \
	examples/osss_recon_examples/Makefile           \
	examples/osss_recon_examples/audio              \
	examples/osss_recon_examples/audio3             \
	examples/osss_recon_examples/car                \
	examples/osss_recon_examples/two_process_audio  \
	examples/osss_recon_examples/no_thrashing_audio \
	examples/osss_recon_examples/recon_sched_audio

$(FLAVOUR)_DOXYGEN_PREDEFINED := \
	OSSS_BLUE                \
	OSSS_GREEN               \
	OSSS_INCLUDE_RECON

# $Id: flavour.mk 2488 2008-07-23 16:20:45Z kim $
# :flavour: (osss,d)
