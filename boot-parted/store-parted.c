#include <hurd/store.h>
#include "libmachdev/machdev.h"

struct store *bootstrap_part_store;
struct store *bootstrap_disk_store;

static void
store_parted_device_init (void)
{
  mach_port_t bootstrap;
  error_t err;
  device_t disk_device;

  task_get_bootstrap_port (mach_task_self (), &bootstrap);
//  device_open (bootstrap, D_READ, "wd0", &disk_device);
  err = store_device_open ("wd0", STORE_READONLY, &bootstrap_disk_store);
}

static void
store_parted_device_dealloc (void *d)
{
}

static mach_port_t
store_parted_dev_to_port (void *nd)
{
    return 0;
}

static io_return_t
store_parted_device_open (mach_port_t reply_port, mach_msg_type_name_t reply_port_type,
		      dev_mode_t mode, const char *name, device_t * devp,
		      mach_msg_type_name_t * devicePoly)
{
  error_t err;
  err = store_part_create (bootstrap_disk_store, 1, 0, &bootstrap_part_store);
  return err;
}

static io_return_t
store_parted_device_close (void *d)
{
  store_close_source (bootstrap_part_store);
  return 0;
}

static io_return_t
store_parted_device_write (void *d, mach_port_t reply_port,
		       mach_msg_type_name_t reply_port_type, dev_mode_t mode,
		       recnum_t bn, io_buf_ptr_t data, unsigned int count,
		       int *bytes_written)
{
  store_write (bootstrap_part_store, );
}

static io_return_t
store_parted_device_read (void *d, mach_port_t reply_port,
		      mach_msg_type_name_t reply_port_type, dev_mode_t mode,
		      recnum_t bn, int count, io_buf_ptr_t * data,
		      unsigned *bytes_read)
{
  store_read (bootstrap_part_store, );
}

static io_return_t
store_parted_device_set_status (void *d, dev_flavor_t flavor, dev_status_t status,
			    mach_msg_type_number_t status_count)
{
    return 0;
}

static io_return_t
store_parted_device_get_status (void *d, dev_flavor_t flavor, dev_status_t status,
			    mach_msg_type_number_t * count)
{
    return 0;
}

static void
store_parted_device_sync (void)
{
}


static struct machdev_device_emulation_ops store_parted_emulation_ops = {
  store_parted_device_init,
  NULL,
  store_parted_device_dealloc,
  store_parted_dev_to_port,
  store_parted_device_open,
  store_parted_device_close,
  store_parted_device_write,
  NULL,
  store_parted_device_read,
  NULL,
  store_parted_device_set_status,
  store_parted_device_get_status,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  store_parted_device_sync
};

void
store_parted_register_block (void)
{
  machdev_register (&store_parted_emulation_ops);
}
