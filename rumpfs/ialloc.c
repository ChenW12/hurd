#include "rumpfs.h"

/* The user must define this function.  Allocate a new node to be of
   mode MODE in locked directory DP (don't actually set the mode or
   modify the dir, that will be done by the caller); the user
   responsible for the request can be identified with CRED.  Set *NP
   to be the newly allocated node.  */
error_t
diskfs_alloc_node (struct node *dir, mode_t mode, struct node **node)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Free node NP; the on disk copy has already been synced with
   diskfs_node_update (where NP->dn_stat.st_mode was 0).  It's
   mode used to be OLD_MODE.  */
void
diskfs_free_node (struct node *np, mode_t old_mode)
{
  // XXX TODO
}
