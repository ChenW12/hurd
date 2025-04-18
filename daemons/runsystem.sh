#!/bin/sh
#
# This program is run by /hurd/init at boot time after the essential
# servers are up.  It does some initialization of its own and then
# execs /hurd/init or any other roughly SysV init-compatible program
# to bring up the "userland" parts of a normal system.
#


###
### Where to find programs, etc.
###

PATH=/bin:/sbin
export PATH

umask 022

# If we lose badly, try to exec each of these in turn.
fallback_shells='/bin/bash /bin/sh /bin/dash /bin/csh /bin/ash /bin/shd'

# Shell used for normal single-user startup.
SHELL=/bin/sh

# The init program to call.
#
# Can be overridden using init=something in the kernel command line.
init=/hurd/init

###


# If we get a SIGLOST, attempt to reopen the console in case
# our console ports were revoked.  This lets us print messages.
reopen_console ()
{
  exec 1>/dev/console 2>&1 || exit 3
}
trap 'reopen_console' 32 # SIGLOST = server died on GNU


# Call this when we are losing badly enough that we want to punt normal
# startup entirely.  We exec a single-user shell, so we will not come back
# here.  The only way to get to multi-user from that shell will be
# explicitly exec this script or something like that.
singleuser ()
{
  test $# -eq 0 || echo "$0: $*"
  for try in ${fallback_shells}; do
    SHELL=${try}
    exec ${SHELL}
  done
  exit 127
}

# Print a newline and banner.
echo
echo Starting runsystem

# See whether pflocal is set up already, and do so if not (install case)
#
# Normally this should be the case, but we better make sure since
# without the pflocal server, pipe(2) does not work.
if ! test -c /servers/socket/1 && command -v settrans >/dev/null ; then
  echo Setting up pflocal
  # The root filesystem should be read-only at this point.
  if fsysopts / --update --writable ; then
    settrans -c /servers/socket/1 /hurd/pflocal
  else
    singleuser "Failed to create /servers/socket/1."
  fi
fi

# We expect to be started by console-run, which gives us no arguments and
# puts FALLBACK_CONSOLE=file-name in the environment if our console is
# other than a normal /dev/console.

if [ "${FALLBACK_CONSOLE+set}" = set ]; then
  singleuser "Running on fallback console ${FALLBACK_CONSOLE}"
fi


###
### Normal startup procedures
###

# Parse the multiboot command line.  We only pay attention to -s and -f.
flags=
single=
while [ $# -gt 0 ]; do
  arg="$1"
  shift
  case "$arg" in
  --*) ;;
  init=*)
    eval "${arg}"
    ;;
  *=*) ;;
  -*)
    flags="${flags}${arg#-}"
    ;;
  'single')
    single="-s"
    ;;
  'fastboot'|'emergency')
    ;;
  esac
done

# Check boot flags.
case "$flags" in
*s*)
  single="-s"			# force single-user
  ;;
esac

# This is necessary to make stat / return the correct device ids.
fsysopts / --update --readonly

# Finally, start the actual SysV init.
echo Starting ${init}
exec ${init} ${single} -a
