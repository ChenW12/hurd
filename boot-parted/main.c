#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <mach.h>
#include <error.h>

#include "libmachdev/machdev.h"
#include "libshouldbeinlibc/wire.h"
#include "store-parted.h"

mach_port_t bootstrap_resume_task = MACH_PORT_NULL;

static const struct argp_option options[] = {
  {"host-priv-port",	'h', "PORT", 0, "Host private port PORT"},
  {"device-master-port",'d', "PORT", 0, "Device master port PORT"},
  {"next-task",		'N', "TASK", 0, "Next bootstrap task TASK"},
  {0}
};

/* Parse a command line option.  */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* We save our parsed values in this structure, hung off STATE->hook.
     Only after parsing all options successfully will we use these values.  */
  struct
  {
    int host_priv;
    int dev_master;
    int next_task;
  } *values = state->hook;

  switch (key)
    {
    case 'h':
      values->host_priv = atoi(arg);
      break;
    case 'd':
      values->dev_master = atoi(arg);
      break;
    case 'N':
      values->next_task = atoi(arg);
      break;

    case ARGP_KEY_INIT:
      /* TODO: I need to have a look at the spec of argp */
      state->child_inputs[0] = state->input;
      values = malloc (sizeof *values);
      if (values == 0)
        return ENOMEM;
      state->hook = values;
      memset (values, 0, sizeof *values);
      break;

    case ARGP_KEY_SUCCESS:
      /* All options parsed successfully */
      _hurd_host_priv = values->host_priv;
      _hurd_device_master = values->dev_master;
      bootstrap_resume_task = values->next_task;
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

static struct argp_child empty_argp_children[] = {{0}};
static struct argp boot_parted_argp = {options, parse_opt, 0, 0, empty_argp_children};
static const struct argp *boot_parted_argp_bootup = &boot_parted_argp;

static int __thread wired = 0;
static int boot_parted_demuxer (mach_msg_header_t *inp, mach_msg_header_t *outp)
{
  /* FIXME: we are not wired while receiving our first message.  */
  if (!wired)
    {
      mach_port_t self = mach_thread_self ();
      thread_wire (_hurd_host_priv, self, TRUE);
      mach_port_deallocate (mach_task_self (), self);
      wired = 1;
    }

  return machdev_demuxer (inp, outp);
}

static void *
boot_parted_multithread_server(void *arg)
{
  pthread_setname_np (pthread_self (), "server");

  do
    {
      ports_manage_port_operations_multithread (machdev_device_bucket,
						boot_parted_demuxer,
						1000 * 60 * 2,  /* 2 minute thread */
						1000 * 60 * 10, /* 10 minute server */
						0);
    } while (1);

  return NULL;
}


int
main (int argc, char **argv)
{
  mach_port_t bootstrap = MACH_PORT_NULL;
  int err;
  pthread_t t;

  err = argp_parse (boot_parted_argp_bootup, argc, argv, 0, 0, NULL);
  if (err)
    {
      error(1, err, "Missing parameters for bootstrap");
    }

  store_parted_register_block ();
  machdev_trivfs_init (argc, argv, bootstrap_resume_task, "boot-parted", "/dev/boot-parted", &bootstrap);

  /* Make sure we will not swap out, in case we drive the disk used for
     swapping.
     Do we need it? --ZhaoM */
  err = wire_task_self ();

  if (err)
    error (1, err, "cannot lock all memory");
  err = thread_wire (_hurd_host_priv, mach_thread_self (), TRUE);
  if (err != KERN_SUCCESS)
    error (1, err, "cannot get vm_privilege");

  machdev_device_init ();
  err = pthread_create (&t, NULL, boot_parted_multithread_server, NULL);
  if (err)
    return err;
  pthread_detach (t);
  machdev_trivfs_server_startup (bootstrap);
  machdev_trivfs_server_loop (NULL);
  /* Never reached */
  return 0;

}
