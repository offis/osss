#!/bin/bash
# Copyright (c) 2006  OFFIS e.V., Oldenburg, Germany
# All rights reserved.
# -------------------------------------------------------------------
# Simple configure script for the OSSS library
# -------------------------------------------------------------------
#
# This file is part of the OSSS simulation library.
# Copyright (c) 2006 OFFIS e.V. Institute for Information Technology
#
# See the files COPYING and LICENSE included with the OSSS library
# for copyright and license details. If you have not received these
# files, please contact OFFIS e.V. for further information.
#
# OFFIS e.V.
# Institute for Information Technology
#   Escherweg 2
#   D-26121 Oldenburg
#   Germany
# www  : http://offis.de
# phone: +49 (441) 9722-0
# fax  : +49 (441) 9722-102
# -------------------------------------------------------------------
# Author(s): Philipp A. Hartmann
# Version  : $Revision: 2976 $
# Lasted   : $Date: 2009-01-20 10:35:24 +0100 (Tue, 20 Jan 2009) $
# -------------------------------------------------------------------

# debugging
#set -x

TARGET=$1

SETTINGS="VERBOSE CXX CXXFLAGS DEBUG PROFILE
          SYSTEMC_HOME SYSTEMC_PTHREADS SYSTEMC_LIB"

#
# Check GCC (version >= 3.4.2)
#
check_gcc() {
  echo -n "Looking for C++ compiler..."
  if [ -n "${OSSS_CFG_CXX_CMD}" ] ; then
    # get from command line
    OSSS_CFG_CXX=${OSSS_CFG_CXX_CMD}
  elif [ -n "${OSSS_CFG_CXX}" ] ; then
    # get cached value - i.e. do nothing and return
    echo -n "(cache)"
  else
    echo -n "(guessed)"
    OSSS_CFG_CXX=${OSSS_CFG_CXX_DEFAULT}
  fi
  WHICH_CXX=`which ${OSSS_CFG_CXX}`
  if [ -x "${OSSS_CFG_CXX}" ] ; then
   echo -n " ${OSSS_CFG_CXX} "
  elif [ -n "${WHICH_CXX}" ] ; then
    OSSS_CFG_CXX="${OSSS_CFG_CXX}"
    echo -n " ${WHICH_CXX} "
  else
    echo " NO!"
    echo "[ERR] '${OSSS_CFG_CXX}' not found, or not executable."
    echo "      Please run 'make config cxx=/path/to/gcc'"
    echo "      or set the environment variable CXX appropriately."
    return 1
  fi
  GCC_MAJOR=`echo __GNUC__ | \
               ${OSSS_CFG_CXX} -E -xc++ - 2>/dev/null | \
               egrep -v  '^(#|__)' `
  GCC_MINOR=`echo __GNUC_MINOR__ | \
               ${OSSS_CFG_CXX} -E -xc++ - 2>/dev/null | \
               egrep -v '^(#|__)' `
  GCC_PATCH=`echo __GNUC_PATCHLEVEL__ | \
               ${OSSS_CFG_CXX} -E -xc++ - 2>/dev/null | \
               egrep -v '^(#|__)' `
  if [ -z "$GCC_MAJOR" ] ; then
    echo "NO!"
    echo "[ERR] '${OSSS_CFG_CXX}' seems to be no GNU C/C++ compiler!"
    echo "      Please run 'make config cxx=/path/to/gcc'"
    echo "      or set the environment variable CXX appropriately."
    return 1
  elif [ "`printf '%02d%02d%02d' $GCC_MAJOR $GCC_MINOR $GCC_PATCH`" -lt 30402 ] ; then
    echo "($GCC_MAJOR.$GCC_MINOR.$GCC_PATCH) NO!"
    echo "[ERR] At least version 3.4.2 of GNU C/C++ compiler required."
    echo "      Please run 'make config cxx=/path/to/gcc'"
    echo "      or set the environment variable CXX appropriately."
    return 1
  fi
  echo "($GCC_MAJOR.$GCC_MINOR.$GCC_PATCH)."
  return 0
}

#
# Look for SystemC header installation
#
SYSTEMC_TARGETS="/usr/local /usr /opt"
SYSTEMC_DIRS="systemc systemc-2.2.0 systemc-2.2.05jun06_beta  ."

check_systemc_inc() {
  if [ -n "$1" ] ; then
    echo -n "  '${OSSS_CFG_SYSTEMC_HOME}/include' ($1)..."
  else
    echo -n "  '${OSSS_CFG_SYSTEMC_HOME}/include'..."
  fi
  if [ -f "${OSSS_CFG_SYSTEMC_HOME}/include/systemc" ] ; then
    # found - OK
    echo "yes."
    return 0
  fi
  echo "no."
  return 1
}

find_systemc_inc() {
  echo "Looking for SystemC headers ..."
  # check SYSTEMC_HOME
  if [ -n "${OSSS_CFG_SYSTEMC_HOME_CMD}" ] ; then
    OSSS_CFG_SYSTEMC_HOME="${OSSS_CFG_SYSTEMC_HOME_CMD}"
    OSSS_CHECK_SYSTEMC_LINK=yes
    check_systemc_inc && return $?
  elif [ -n "${OSSS_CFG_SYSTEMC_HOME}" ] ; then
    check_systemc_inc "cache" && return $?
  elif [ -n "${SYSTEMC_HOME}" ] ; then
    OSSS_CFG_SYSTEMC_HOME=${SYSTEMC_HOME}
    check_systemc_inc "env" && return $?
    OSSS_CHECK_SYSTEMC_LINK=yes
  elif [ -n "${OSSS_CFG_SYSTEMC_LIB_CMD}" ] ; then
    OSSS_CFG_SYSTEMC_HOME=`dirname "${OSSS_CFG_SYSTEMC_LIB_CMD}"`
    check_systemc_inc "lib" && return $?
  elif [ -n "${SYSTEMC_LIB}" ] ; then
    OSSS_CFG_SYSTEMC_HOME=`dirname "${SYSTEMC_LIB}"`
    check_systemc_inc "lib,env" && return $?
  else
    for __target in ${SYSTEMC_TARGETS} ; do
      for __dir in ${SYSTEMC_DIRS} ; do
        if [ "${__dir}" = "." ] ; then
          OSSS_CFG_SYSTEMC_HOME=${__target}
        else
          OSSS_CFG_SYSTEMC_HOME=${__target}/${__dir}
        fi
        check_systemc_inc && return $?
      done
    done
  fi
  # not found - error
  echo "[ERR] SystemC header files not found."
  echo "      Please run 'make config systemc=/path/to/systemc/install'"
  echo "      or set the environment variable SYSTEMC_HOME appropriately."
  SYSTEMC_HOME=
  return 1
}

translate_arch() {
  local __system=`${OSSS_CFG_CXX} -dumpmachine 2>/dev/null`
  local __short
  case "${__system}" in
    sparc-sun-solaris*)
        __short="gccsparcOS5"
        ;;
    x86_64*linux*)
        __short="linux64"
        ;;
    *linux*)
        __short="linux"
        ;;
  esac
  echo "${__system} ${__short}"
}


find_systemc_lib() {
  echo "Looking for SystemC library ..."
  if [ -n "${OSSS_CFG_SYSTEMC_LIB_CMD}" ] ; then
    # explicit location given
    OSSS_CFG_SYSTEMC_LIB=${OSSS_CFG_SYSTEMC_LIB_CMD}
    OSSS_CHECK_SYSTEMC_LINK=yes
    check_systemc_lib && return $?
  else
    # check cache
    if [ -n "${OSSS_CFG_SYSTEMC_LIB}" ] ; then
      check_systemc_lib "cache" && return $?
    fi
    OSSS_CHECK_SYSTEMC_LINK=yes
    if [ -n "${SYSTEMC_LIB}" ] ; then
      OSSS_CFG_SYSTEMC_LIB=${SYSTEMC_LIB}
      check_systemc_lib "env" && return $?
    fi
    # check for compiler specific library dirs
    local __archdirs="`translate_arch`"
    local __compiler="g++ gcc"
    local __versions="$GCC_MAJOR.$GCC_MINOR.$GCC_PATCH \
                       $GCC_MAJOR.$GCC_MINOR $GCC_MAJOR"
    for v in ${__versions} ""; do
     for c in ${__compiler} ""; do
      for a in ${__archdirs} ; do
       OSSS_CFG_SYSTEMC_LIB=${OSSS_CFG_SYSTEMC_HOME}/lib-$a
       if [ -n "$c" ] ; then
        OSSS_CFG_SYSTEMC_LIB=${OSSS_CFG_SYSTEMC_LIB}-$c
       fi
       if [ -n "$v" ] ; then
        OSSS_CFG_SYSTEMC_LIB=${OSSS_CFG_SYSTEMC_LIB}-$v
       fi
       check_systemc_lib && return $?
      done
     done
    done
  fi
  # not found - error
  echo "[ERR] No suitable SystemC library for current compiler found."
  echo "      Please run 'make config systemc-lib=/path/to/dir/with/libsystemc.a'"
  echo "      or set the environment variable SYSTEMC_LIB appropriately."
  SYSTEMC_LIB=
  return 1

}

check_systemc_lib() {
  if [ -n "$1" ] ; then
    echo -n "  '${OSSS_CFG_SYSTEMC_LIB}' ($1)..."
  else
    echo -n "  '${OSSS_CFG_SYSTEMC_LIB}'..."
  fi
  local LINK=0
  # library exists
  if [ -f "${OSSS_CFG_SYSTEMC_LIB}/libsystemc.a" ] ; then
    if [ "${OSSS_CHECK_SYSTEMC_LINK}" = "yes" ] ; then
      # check linking
      if [ "${OSSS_CFG_SYSTEMC_PTHREADS}" = "yes" ] ; then
        echo -n "link+pthread..."
        LDFLAGS=" ${LDFLAGS} -lsystemc -lpthread -lm"
      else
        echo -n "link..."
        LDFLAGS=" ${LDFLAGS} -lsystemc -lm"
      fi
      ${OSSS_CFG_CXX} -xc++ - -o conftest.tmp \
        -I${OSSS_CFG_SYSTEMC_HOME}/include \
        -L${OSSS_CFG_SYSTEMC_LIB} $LDFLAGS 2>/dev/null <<ENDOFSYSTEMC
#include <systemc>
int sc_main( int, char*[] ) {
  /* explicitly instantiate template */
  sc_core::sc_signal< sc_dt::sc_uint<16> > sig;
}
ENDOFSYSTEMC
      LINK=$?
      rm -f conftest*
    fi
    if [ $LINK -eq 0 ] ; then
      echo "yes."
      return 0
    fi
  fi
  echo "no."
  return 1
}

OSSS_CFG_VERBOSE_MSG="Verbose compilation ..."
OSSS_CFG_CXXFLAGS_MSG="Compilation flags..."
OSSS_CFG_DEBUG_MSG="Include debugging symbols ..."
OSSS_CFG_PROFILE_MSG="Include profiling information ..."
OSSS_CFG_SYSTEMC_PTHREADS_MSG="Using SystemC with pthreads..."

check_cached_switch() {
  local MSG=OSSS_CFG_$1_MSG
  local OLDVAR=OSSS_CFG_$1
  local NEWVAR=OSSS_CFG_$1_CMD
  local DEFVAR=OSSS_CFG_$1_DEFAULT
  local NEWVAL=no
  echo -n ${!MSG}
  if [ -n "${!NEWVAR}" ] ; then
    NEWVAL="${!NEWVAR}"
  elif [ -n "${!OLDVAR}" ] ; then
    echo -n "(cache) "
    NEWVAL="${!OLDVAR}"
  elif [ -n "${!1}" ] ; then
    echo -n "(env) "
    NEWVAL="${!1}"
  else
    echo -n "(default) "
    NEWVAL="${!DEFVAR}"
  fi
  if [ "x$2" == "x" ] ; then # yes|no switch
    if [ "$NEWVAL" != "no" ] ; then
         NEWVAL=yes
    fi
  fi
  eval ${OLDVAR}="'"${NEWVAL}"'"
  echo "${!OLDVAR}".
}

check_config() {
    case "$1" in
    VERBOSE|DEBUG|PROFILE|SYSTEMC_PTHREADS)
        check_cached_switch $1
        return 0
        ;;
    CXXFLAGS)
        check_cached_switch $1 literal
        return 0
        ;;
    CXX)
        check_gcc
        return $?
        ;;
    SYSTEMC_HOME)
        find_systemc_inc
        return $?
        ;;
    SYSTEMC_LIB)
        find_systemc_lib
        return $?
        ;;
    *)
        return 0
    esac
}

write_config_header() {
cat <<EOHEADER
# -------------------------------------------------------------------
# Configuration file for OSSS library
# -------------------------------------------------------------------
#          THIS FILE IS AUTOMATICALLY GENERATED - DO NOT EDIT
# -------------------------------------------------------------------
# To change the settings made in this file, please run 'make config'
# with the appropriate arguments. See 'make help' for details.
#
EOHEADER
}

sed_escape()
{
  sed 's/\([\\/\\&$\\\\]\)/\\\1/g'
}

write_config_line() {
  local COMMENT=
  local VARNAME=OSSS_CFG_$1
  if [ -z "${!VARNAME}" ] ; then
    COMMENT="#"
  fi
  echo "${COMMENT}${VARNAME} := ${!VARNAME}"
}

write_config() {
  write_config_header > ${TARGET}.tmp
  for __setting in ${SETTINGS} ; do
    write_config_line ${__setting} >>  ${TARGET}.tmp
  done
  if cmp -s ${TARGET} ${TARGET}.tmp ; then
    rm -f ${TARGET}.tmp
  else
    mv ${TARGET}.tmp ${TARGET}
  fi
  # check, if this script is newer than the target
  if [ "$0" -nt "${TARGET}" ] ; then
    touch "${TARGET}"
  fi
}

delete_and_exit() {
  rm -f ${TARGET}
  exit 1
}

for __setting in ${SETTINGS} ; do
  check_config ${__setting} || delete_and_exit
done
write_config

# $Id: configure.sh 2976 2009-01-20 09:35:24Z philipph $
# :flavour: (all,d)
