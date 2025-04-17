#include <error.h>
#include <stdlib.h>
#include <hurd/store.h>
#include <hurd/diskfs.h>
#include <argp.h>
#include "dir-rump.h"

// 32767 is LINK_MAX in netbsd source
int diskfs_link_max = 32767;
int diskfs_name_max = FFS_MAXNAMLEN;
// 8 is the value of _POSIX_SYMLOOP_MAX
int diskfs_maxsymlinks = 8;
// XXX not very clear about the following options
int diskfs_shortcut_symlink = 1;
int diskfs_shortcut_chrdev = 1;
int diskfs_shortcut_blkdev = 1;
int diskfs_shortcut_fifo = 1;
int diskfs_shortcut_ifsock = 1;

char *diskfs_server_name = "rumpfs";
char *diskfs_server_version = "XXX server version";
char *diskfs_extra_version = "XXX extra version";

struct node *diskfs_root_node;

int diskfs_synchronous;

struct store *store;
struct store_parsed *store_parsed;

char *diskfs_disk_name;

/* Rumpfs-specific options.  XXX Do we have?*/
static const struct argp_option
options[] =
{
  {0}
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case ARGP_KEY_INIT:
      state->child_inputs[0] = state->input;
      break;
    case ARGP_KEY_SUCCESS:
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

/* Add our startup arguments to the standard diskfs set.  */
static const struct argp_child startup_children[] =
  {{&diskfs_store_startup_argp}, {0}};
static struct argp startup_argp = {options, parse_opt, 0, 0, startup_children};

/* Similarly at runtime.  */
static const struct argp_child runtime_children[] =
  {{&diskfs_std_runtime_argp}, {0}};
static struct argp runtime_argp = {options, parse_opt, 0, 0, runtime_children};

struct argp *diskfs_runtime_argp = (struct argp *)&runtime_argp;

int
main (int argc, char **argv)
{
  error_t err;
  mach_port_t bootstrap;

  /* Initialize the diskfs library, parse arguments, and open the store.
     This starts the first diskfs thread for us.  */
  store = diskfs_init_main (&startup_argp, argc, argv,
			    &store_parsed, &bootstrap);

//  /* Map the entire disk. */
//  create_disk_pager ();
//
//  pokel_init (&global_pokel, diskfs_disk_pager, disk_cache);


  return 0;
}

error_t
diskfs_reload_global_state (void)
{
  // XXX TODO
  return EOPNOTSUPP;
}
