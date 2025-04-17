#include "rumpfs.h"

struct dirstat
{
  // XXX TODO
};

const size_t diskfs_dirstat_size = sizeof (struct dirstat);

/* Implement the diskfs_lookup from the diskfs library.  See
   <hurd/diskfs.h> for the interface specification.  */
error_t
diskfs_lookup_hard (struct node *dp, const char *name, enum lookup_type type,
		    struct node **npp, struct dirstat *ds, struct protid *cred)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Following a lookup call for CREATE, this adds a node to a directory.
   DP is the directory to be modified; NAME is the name to be entered;
   NP is the node being linked in; DS is the cached information returned
   by lookup; CRED describes the user making the call.  This call may
   only be made if the directory has been held locked continuously since
   the preceding lookup call, and only if that call returned ENOENT. */
error_t
diskfs_direnter_hard (struct node *dp, const char *name, struct node *np,
		      struct dirstat *ds, struct protid *cred)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Following a lookup call for RENAME, this changes the inode number
   on a directory entry.  DP is the directory being changed; NP is
   the new node being linked in; DP is the cached information returned
   by lookup.  This call is only valid if the directory has been locked
   continuously since the call to lookup, and only if that call
   succeeded.  */
error_t
diskfs_dirrewrite_hard (struct node *dp, struct node *np, struct dirstat *ds)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Following a lookup call for REMOVE, this removes the link from the
   directory.  DP is the directory being changed and DS is the cached
   information returned from lookup.  This call is only valid if the
   directory has been locked continuously since the call to lookup, and
   only if that call succeeded.  */
error_t
diskfs_dirremove_hard (struct node *dp, struct dirstat *ds)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Make DS an invalid dirstat. */
error_t
diskfs_drop_dirstat (struct node *dp, struct dirstat *ds)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Initialize DS such that diskfs_drop_dirstat will ignore it. */
void
diskfs_null_dirstat (struct dirstat *ds)
{
  // XXX TODO
}

/* Implement the diskfs_get_directs callback as described in
   <hurd/diskfs.h>. */
error_t
diskfs_get_directs (struct node *dp,
		    int entry,
		    int nentries,
		    char **data,
		    mach_msg_type_number_t *datacnt,
		    vm_size_t bufsiz,
		    int *amt)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Tell if DP is an empty directory (has only "." and ".." entries).
   This routine must be called from inside a catch_exception ().  */
int
diskfs_dirempty (struct node *dp, struct protid *cred)
{
  // XXX TODO
  return 0;
}
