/* Filesystem management for NFS server
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

#include <stdio.h>
#include <errno.h>
#include <hurd.h>
#include <fcntl.h>
#include <string.h>

#include "nfsd.h"

struct fsys_spec 
{
  fsys_t fsys;
  char *name;
};

static struct fsys_spec *fsystable;
static int nfsys = 0;
static int fsystablesize = 0;

file_t index_file_dir;
char *index_file_compname;

/* Read the filesystem table in from disk */
void
init_filesystems (void)
{
  int nitems;
  char *name;
  int index;
  int line;
  file_t root;
  static FILE *index_file;

  fsystable = (struct fsys_spec *) malloc ((fsystablesize = 10)
					   * sizeof (struct fsys_spec));

  if (!index_file_name)
    return;
  
  index_file = fopen (index_file_name, "r");
  if (!index_file)
    {
      fprintf (stderr, "%s: Cannot open `%s': %s\n",
	       program_invocation_name, index_file_name, strerror (errno));
      return;
    }

  for (line = 1; ; line++)
    {
      nitems = fscanf (index_file, "%d %as\n", &index, &name);
      if (nitems == EOF)
	{
	  fclose (index_file);
	  return;
	}
      
      if (nitems != 2)
	{
	  fprintf (stderr, "%s:%s:%d Bad syntax\n",
		   program_invocation_name, index_file_name, line);
	  continue;
	}

      root = file_name_lookup (name, 0, 0);
      if (!root)
	{
	  fprintf (stderr, "%s:%s:%d Filesystem `%s': %s\n",
		   program_invocation_name, index_file_name, line,
		   name, strerror (errno));
	  free (name);
	  continue;
	}

      if (index >= fsystablesize)
	fsystable = (struct fsys_spec *) realloc (fsystable,
						  (fsystablesize = index * 2)
						  * sizeof (struct fsys_spec));
      if (index > nfsys)
	nfsys = index;
      
      fsystable[index].name = name;
      file_getcontrol (root, &fsystable[nfsys].fsys);
      mach_port_deallocate (mach_task_self (), root);
    }
}

/* Write the current filesystem table to disk synchronously. */
static void
write_filesystems (void)
{
  file_t newindex;
  FILE *f;
  int i;

  if (!index_file_name)
    return;
  
  if (index_file_dir == MACH_PORT_NULL)
    {
      index_file_dir = file_name_split (index_file_name, &index_file_compname);
      if (index_file_dir == MACH_PORT_NULL)
	{
	  fprintf (stderr, "%s: `%s': %s\n",
		   program_invocation_name, index_file_name, strerror (errno));
	  index_file_name = 0;
	  return;
	}
    }

  /* Create an anonymous file in the same directory */
  errno = dir_mkfile (index_file_dir, O_WRONLY, 0666, &newindex);
  if (errno)
    {
      fprintf (stderr, "%s: `%s': %s\n",
	       program_invocation_name, index_file_name, strerror (errno));
      index_file_name = 0;
      mach_port_deallocate (mach_task_self (), index_file_dir);
      index_file_dir = MACH_PORT_NULL;
      return;
    }
  
  f = fopenport (newindex, "w");
  
  for (i = 0; i < nfsys; i++)
    if (fsystable[i].name)
      fprintf (f, "%d %s\n", i, fsystable[i].name);

  /* Link it in */
  errno = dir_link (index_file_dir, newindex, index_file_compname, 0);
  if (errno)
    fprintf (stderr, "%s: `%s': %s\n",
	     program_invocation_name, index_file_name, strerror (errno));
  fflush (f);
  file_sync (newindex, 1, 0);
  fclose (f);
}

/* From a filesystem ID number, return the fsys_t for talking to that
   filesystem; MACH_PORT_NULL if it isn't in our list. */
fsys_t
lookup_filesystem (int id)
{
  if (id >= nfsys)
    return MACH_PORT_NULL;
  return fsystable[id].fsys;
}

/* Enter a name in the table of filesystems; return its ID number.
   ROOT refers to the root of this filesystem.  */
int
enter_filesystem (char *name, file_t root)
{
  int i;

  for (i = 0; i < nfsys; i++)
    if (fsystable[i].name && !strcmp (fsystable[i].name, name))
      return i;
  
  if (nfsys == fsystablesize)
    fsystable = (struct fsys_spec *) realloc (fsystable,
					      (fsystablesize *= 2)
					      * sizeof (struct fsys_spec));

  fsystable[nfsys].name = malloc (strlen (name) + 1);
  strcpy (fsystable[nfsys].name, name);
  file_getcontrol (root, &fsystable[nfsys].fsys);
  nfsys++;

  write_filesystems ();

  return nfsys - 1;
}

