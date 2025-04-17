
#include "rumpfs.h"

/* Return in *OFFSET and *SIZE the minimum valid address the pager will
   accept and the size of the object.  */
error_t
pager_report_extent (struct user_pager_info *pager,
		     vm_address_t *offset, vm_size_t *size)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Grow the disk allocated to locked node NODE to be at least SIZE bytes, and
   set NODE->allocsize to the actual allocated size.  (If the allocated size
   is already SIZE bytes, do nothing.)  CRED identifies the user responsible
   for the call.  */
error_t
diskfs_grow (struct node *node, off_t size, struct protid *cred)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* This syncs a single file (NODE) to disk.  Wait for all I/O to complete
   if WAIT is set.  NODE->lock must be held.  */
void
diskfs_file_update (struct node *node, int wait)
{
  // XXX TODO
}

/* Call this to create a FILE_DATA pager and return a send right.
   NODE must be locked.  */
mach_port_t
diskfs_get_filemap (struct node *node, vm_prot_t prot)
{
  // XXX TODO
  return 0;
}

/* Call this to find out the struct pager * corresponding to the
   FILE_DATA pager of inode IP.  This should be used *only* as a subsequent
   argument to register_memory_fault_area, and will be deleted when
   the kernel interface is fixed.  NODE must be locked.  */
struct pager *
diskfs_get_filemap_pager_struct (struct node *node)
{
  // XXX TODO
  return NULL;
}

/* Return the bitwise or of the maximum prot parameter (the second arg to
   diskfs_get_filemap) for all active user pagers. */
vm_prot_t
diskfs_max_user_pager_prot (void)
{
  // XXX TODO
  return 0;
}

/* Tell diskfs if there are pagers exported, and if none, then
   prevent any new ones from showing up.  */
int
diskfs_pager_users (void)
{
  // XXX TODO
  return 0;
}

/* Sync all the pagers. */
void
diskfs_sync_everything (int wait)
{
  // XXX TODO
}

/* Shutdown all the pagers (except the disk pager). */
void
diskfs_shutdown_pager (void)
{
  // XXX TODO
}

/* This will be called when the ports library wants to drop weak references.
   The pager library creates no weak references itself.  If the user doesn't
   either, then it's OK for this function to do nothing.  */
void
pager_dropweak (struct user_pager_info *upi)
{
  // XXX TODO
}

/* Satisfy a pager write request for either the disk pager or file pager
   PAGER, from the page at offset PAGE from BUF.  */
error_t
pager_write_page (struct user_pager_info *pager, vm_offset_t page,
		  vm_address_t buf)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Satisfy a pager read request for either the disk pager or file pager
   PAGER, to the page at offset PAGE into BUF.  WRITELOCK should be set if
   the pager should make the page writeable.  */
error_t
pager_read_page (struct user_pager_info *pager, vm_offset_t page,
		 vm_address_t *buf, int *writelock)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* Make page PAGE writable, at least up to ALLOCSIZE.  This function and
   diskfs_grow are the only places that blocks are actually added to the
   file.  */
error_t
pager_unlock_page (struct user_pager_info *pager, vm_offset_t page)
{
  // XXX TODO
  return EOPNOTSUPP;
}

/* This is called when a pager is being deallocated after all extant send
   rights have been destroyed.  */
void
pager_clear_user_data (struct user_pager_info *upi)
{
  // XXX TODO
}

void
pager_notify_evict (struct user_pager_info *pager, vm_offset_t page)
{
  // XXX TODO
}

