/* 
   Copyright (C) 1996 Free Software Foundation, Inc.
   Written by Michael I. Bushnell, p/BSG.

   This file is part of the GNU Hurd.

   The GNU Hurd is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   The GNU Hurd is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA. */

/* Written by Michael I. Bushnell, p/BSG.  */

#include "netfs.h"
#include "execserver.h"
#include "fs_S.h"
#include <sys/stat.h>
#include <fcntlbits.h>
#include <hurd/exec.h>
#include <hurd/paths.h>
#include <string.h>
#include <idvec.h>

kern_return_t 
netfs_S_file_exec (struct protid *cred,
		   task_t task,
		   int flags,
		   char *argv,
		   u_int argvlen,
		   char *envp,
		   u_int envplen,
		   mach_port_t *fds,
		   u_int fdslen,
		   mach_port_t *portarray,
		   u_int portarraylen,
		   int *intarray,
		   u_int intarraylen,
		   mach_port_t *deallocnames,
		   u_int deallocnameslen,
		   mach_port_t *destroynames,
		   u_int destroynameslen)
{
  struct node *np;
  error_t err;
  struct protid *newpi;
  int suid, sgid;
  
  if (!cred)
    return EOPNOTSUPP;

  if (_netfs_exec == MACH_PORT_NULL)
    _netfs_exec = file_name_lookup (_SERVERS_EXEC, 0, 0);
  if (_netfs_exec == MACH_PORT_NULL)
    return EOPNOTSUPP;
  
  np = cred->po->np;

  mutex_lock (&np->lock);

  if ((cred->po->openstat & O_EXEC) == 0)
    {
      mutex_unlock (&np->lock);
      return EBADF;
    }

  err = netfs_validate_stat (np, cred->credential);
  if (err)
    {
      mutex_unlock (&np->lock);
      return err;
    }
  
  if (!((np->nn_stat.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH))
	|| ((np->nn_stat.st_mode & S_IUSEUNK)
	    && (np->nn_stat.st_mode & (S_IEXEC << S_IUNKSHIFT)))))
    {
      mutex_unlock (&np->lock);
      return EACCES;
    }
  
  if ((np->nn_stat.st_mode & S_IFMT) == S_IFDIR)
    {
      mutex_unlock (&np->lock);
      return EACCES;
    }

  suid = np->nn_stat.st_mode & S_ISUID;
  sgid = np->nn_stat.st_mode & S_ISGID;
  if (suid || sgid)
    {
      int secure = 0;
      error_t get_file_ids (struct idvec *uidsvec, struct idvec *gidsvec)
	{
	  error_t err;
	  uid_t *uids, *gids;
	  int nuids, ngids;

	  netfs_interpret_credential (cred->credential, &uids, &nuids,
				      &gids, &ngids);

	  err = idvec_merge_ids (uidsvec, uids, nuids);
	  if (! err)
	    err = idvec_merge_ids (gidsvec, gids, ngids);
	  free (uids);
	  free (gids);
	  return err;
	}
      fshelp_exec_reauth (suid, np->nn_stat.st_uid, sgid, np->nn_stat.st_gid,
			  netfs_auth_server_port, get_file_ids,
			  portarray, portarraylen, fds, fdslen, &secure);
      if (secure)
	flags |= EXEC_SECURE | EXEC_NEWTASK;
    }

  /* If the user can't read the file, then we should use a new task,
     which would be inaccessible to the user.  Actually, this doesn't
     work, because the proc server will still give out the task port
     to the user.  Too many things depend on that that it can't be
     changed.  So this vague attempt isn't even worth trying.  */
#if 0
  if (diskfs_access (np, S_IREAD, cred))
    flags |= EXEC_NEWTASK;
#endif

  newpi = netfs_make_protid (netfs_make_peropen (np, O_READ, 
						 cred->po->dotdotport),
			     netfs_copy_credential (cred->credential));
  mutex_unlock (&np->lock);

  if (! err)
    {
      err = exec_exec (_netfs_exec, 
		       ports_get_right (newpi),
		       MACH_MSG_TYPE_MAKE_SEND,
		       task, flags, argv, argvlen, envp, envplen, 
		       fds, MACH_MSG_TYPE_COPY_SEND, fdslen,
		       portarray, MACH_MSG_TYPE_COPY_SEND, portarraylen,
		       intarray, intarraylen, deallocnames, deallocnameslen,
		       destroynames, destroynameslen);
      ports_port_deref (newpi);
    }

  if (! err)
    {
      unsigned int i;
      
      mach_port_deallocate (mach_task_self (), task);
      for (i = 0; i < fdslen; i++)
	mach_port_deallocate (mach_task_self (), fds[i]);
      for (i = 0; i < portarraylen; i++)
	mach_port_deallocate (mach_task_self (), portarray[i]);
    }

  return err; 
}
