This directory contains a benchmark in OSSS+R (version 2006-03).
It consists of several modules competing for a reconfigurable
area and performing different tasks on it.

Some files:

Top.cpp
Contains the top level module. Start here to look deeper.

mat
Is an simulation executable with some output. Don't be
suprised when it seems to "hang" for some time after
printing "no more speech samples available" and
"I'd like to enter OSSS_KEEP_ENABLED". It will continue.
The executable takes 8 minutes on an Athlon XP 1700 machine.

