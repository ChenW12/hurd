#!/bin/sh
#
# Make standard devices
#

PATH=/bin:/usr/bin

ECHO=:		# Change to "echo" to echo commands.
EXEC=""		# Change to ":" to suppress command execution.
DEVDIR=`pwd`	# Reset below by -D/--devdir command line option.

while :; do
  case "$1" in
    --help|"-?")
      echo "\
Usage: $0 [OPTION...] DEVNAME...
Make filesystem nodes for accessing standard system devices

  -D, --devdir=DIR           Use DIR when a device node name must be
                             embedded in a translator; default is the cwd
  -n, --dry-run              Don't actually execute any commands
  -v, --verbose              Show what commands are executed to make the devices
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version"
      exit 0;;
    --devdir)   DEVDIR="$2"; shift 2;;
    --devdir=*) DEVDIR="`echo "$1" | sed 's/^--devdir=//'`"; shift 1;;
    -D)         DEVDIR="$2"; shift 2;;
    -D*)        DEVDIR="`echo "$1" | sed 's/^-D//'`"; shift 1;;
    --verbose|-v) ECHO=echo; shift;;
    --dry-run|-n) EXEC=:; shift;;
    -nv|-vn)      ECHO=echo; EXEC=:; shift;;
    --usage)
      echo "Usage: $0 [-V?] [-D DIR] [--help] [--usage] [--version] [--devdir=DIR] DEVNAME..."
      exit 0;;
    --version|-V)
      echo "STANDARD_HURD_VERSION_MAKEDEV_"; exit 0;;
    -*)
      echo 1>&2 "$0: unrecognized option \`$1'"
      echo 1>&2 "Try \`$0 --help' or \`$0 --usage' for more information";
      exit 1;;
    *)
      break;;
  esac
done

case  "$#" in 0)
  echo 1>&2 "Usage: $0 [OPTION...] DEVNAME..."
  echo 1>&2 "Try \`$0 --help' or \`$0 --usage' for more information"
  exit 1;;
esac

function cmd {
  eval $ECHO "$@"
  eval $EXEC "$@"
}

function st {
  local NODE="$1"
  local OWNER="$2"
  local PERM="$3"
  shift 3
  if cmd settrans -cg "$NODE"; then
    cmd chown "$OWNER" "$NODE"
    cmd chmod "$PERM" "$NODE"
    cmd settrans "$NODE" "$@"
  fi
}

function lose {
  local line
  for line; do
    echo 1>&2 "$0: $line"
  done
  exit 1
}

function mkdev {
  local I
  for I; do
    case "$I" in
      /* | */*)
        lose "Device names cannot contain directories" \
	     "Change to target directory and run $0 from there."
	;;

      std)
	mkdev console tty null zero full fd time mem klog shm
	;;
      console|tty[0-9][0-9a-f]|tty[0-9a-f]|com[0-9])
	st $I root 600 /hurd/term ${DEVDIR}/$I device $I;;
      null)
	st $I root 666 /hurd/null;;
      full)
	st $I root 666 /hurd/null --full;;
      zero)
	st $I root 666 /hurd/storeio -Tzero;;
      tty)
	st $I root 666 /hurd/magic tty;;
      fd)
	st $I root 666 /hurd/magic --directory fd
	cmd ln -f -s fd/0 stdin
	cmd ln -f -s fd/1 stdout
	cmd ln -f -s fd/2 stderr
	;;
      'time')
	st $I root 644 /hurd/storeio --no-cache time ;;
      mem)
	st $I root 660 /hurd/storeio --no-cache mem ;;
      klog)
        st $I root 660 /hurd/streamio kmsg;;
      # ptys
      [pt]ty[pqrstuvwxyzPQRS]?)
	# Make one pty, both the master and slave halves.
	local id="${I:3}"
	st pty$id root 666 /hurd/term ${DEVDIR}/pty$id \
				      pty-master ${DEVDIR}/tty$id
	st tty$id root 666 /hurd/term ${DEVDIR}/tty$id \
				      pty-slave ${DEVDIR}/pty$id
	;;
      [pt]ty[pqrstuvwxyzPQRS])
	# Make a bunch of ptys.
	local n
        for n in 0 1 2 3 4 5 6 7 8 9 \
		 a b c d e f g h i j k l m n o p q r s t u v; do
	  mkdev ${I}${n}
	done
	;;

      fd*|mt*)
	st $I root 640 /hurd/storeio $I
	;;

      [hrsc]d*)
        local n="${I#?d}"
	local major="${n%%[!0-9]*}"
	if [ -z "$major" ]; then
	  lose "$I: Invalid device name: must supply a device number"
	fi
	local minor="${n##$major}"
	case "$minor" in
	'') ;;		# Whole disk
	[a-z]) ;;	# BSD partition syntax, no slice syntax
	s[1-9]*)	# Slice syntax.
	  local slicestuff="${minor#s}"
	  local slice="${slicestuff%%[!0-9]*}"
	  local rest="${slicestuff##$slice}"
	  case "$slice" in
	  [1-9] | [1-9][0-9]) ;;
	  *)
	    lose "$I: Invalid slice number \`$slice'"
	    ;;
	  esac
	  case "$rest" in
	  '') ;;	# Whole slice
	  [a-z]) ;;	# BSD partition after slice
	  *)
	    lose "$I: Invalid partition \`$rest'"
	    ;;
	  esac
	  ;;
	*)
	  lose "$I: Invalid slice or partition syntax"
	  ;;
	esac

	# The device name passed all syntax checks, so finally use it!
	st $I root 640 /hurd/storeio $I
	;;

      # /dev/shm is used by the POSIX.1 shm_open call in libc.
      shm)
        st $I root 1777 /hurd/tmpfs
        ;;

      # Linux compatibility
      loop*)
        # In Linux an inactive "/dev/loopN" device acts like /dev/null.
	# The `losetup' script changes the translator to "activate" the device.
        st $I root 640 /hurd/null
	;;

      *)
	lose "$I: Unknown device name"
	;;
    esac
  done
}

mkdev "$@"
