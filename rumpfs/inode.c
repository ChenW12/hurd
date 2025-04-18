/* Copyright (C) 2025  Zhaoming Luo <zhmingluo@163.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */


#include "rumpfs.h"

error_t
diskfs_set_statfs (struct statfs *st)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Implement the diskfs_set_translator callback from the diskfs
   library; see <hurd/diskfs.h> for the interface description. */
error_t
diskfs_set_translator (struct node *np, const char *name,
		       mach_msg_type_number_t namelen, struct protid *cred)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Implement the diskfs_get_translator callback from the diskfs library.
   See <hurd/diskfs.h> for the interface description. */
error_t
diskfs_get_translator (struct node *np, char **namep,
		       mach_msg_type_number_t *namelen)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Reload all data specific to NODE from disk, without writing anything.
   Always called with DISKFS_READONLY true.  */
error_t
diskfs_node_reload (struct node *node)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* The last hard reference to a node has gone away. */
void
diskfs_lost_hardrefs (struct node *np)
{
  // XXX TODO
}

/* A new hard reference to a node has been created; it's now OK to
   have unused weak references. */
void
diskfs_new_hardrefs (struct node *np)
{
  // XXX TODO
}

/* The user must define this function if she wants to use the node
   cache.  The last hard reference to a node has gone away; arrange to
   have all the weak references dropped that can be.  */
void
diskfs_user_try_dropping_softrefs (struct node *np)
{
  // XXX TODO
}

/* The last reference to a node has gone away; drop
   it from the hash table and clean all state in the dn structure. */
void
diskfs_node_norefs (struct node *np)
{
  // XXX TODO
}

/* Sync the info in NP->dn_stat and any associated format-specific
   information to disk.  If WAIT is true, then return only after the
   physicial media has been completely updated.  */
void
diskfs_write_disknode (struct node *np, int wait)
{
  // XXX TODO
}
