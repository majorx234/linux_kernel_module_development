#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MajorX234");
MODULE_DESCRIPTION("A example for a simple USB device Linux module.");
MODULE_VERSION("0.01");

#define VENDOR_ID 0x17cc
#define PRODUCT_ID 0x1140
// used to have a driver which is compatible with this id

struct usb_device *dev;
static DEFINE_MUTEX( ulock );

// character device file
static ssize_t lkm_example4_usb_driver_read(struct file *instance, char *buffer,
                               size_t count, loff_t *ofs)
{
  char pbuf[20];
  __u16 *status = kmalloc(sizeof(__u16), GFP_KERNEL);

  mutex_lock(&ulock); // critical path, not disconnecting
  if (usb_control_msg(dev, usb_rcvctrlpipe(dev, 0), USB_REQ_GET_STATUS,
                      USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_INTERFACE,
                      0, 0, status, sizeof(*status), 5*HZ) < 0){
    count = -EIO;
    goto read_out;
  }

  snprintf(pbuf, sizeof(pbuf), "status-%d  \n", *status);
  if (strlen(pbuf) < count)
    count = strlen(pbuf);
  count -= copy_to_user(buffer, pbuf, count);
  *ofs += count;

 read_out:
  mutex_unlock(&ulock);
  kfree(status);
  return count;
}

static int lkm_example4_usb_driver_open(struct inode *devicefile, struct file *instanz)
{
  return 0;
}

static struct file_operations usb_fops = {
  .owner = THIS_MODULE,
  .open = lkm_example4_usb_driver_open,
  .read = lkm_example4_usb_driver_read,
};

static struct usb_device_id lkm_example4_usb_table[] = {
    {USB_DEVICE(VENDOR_ID,
                PRODUCT_ID)}, // create a USB device Id out of this table
    {}                        // last element need to be empty
};

struct usb_class_driver class_descr = {
  .name = "lkm_example4",
  .fops = &usb_fops,
  .minor_base = 16,
};

static int lkm_example4_probe(struct usb_interface *intf, const struct usb_device_id* id) {
  printk("lkm_example4 usb driver - probe function\n");
  dev = interface_to_usbdev(intf);
  printk("usbcheck: 0x%4.4x|0x%4.4x, if=%p\n",
         dev->descriptor.idVendor, dev->descriptor.idProduct, intf);
  if (dev->descriptor.idVendor == VENDOR_ID
      && dev->descriptor.idProduct == PRODUCT_ID) {
    if (usb_register_dev( intf, &class_descr )) {
      return EIO;
    }
    printk("got minor= %d\n", intf->minor );
    return 0;
  }
  return -ENODEV;
}

static void lkm_example4_disconnect( struct usb_interface *intf )
{
  /* need to wait for last commands in que */
  mutex_lock( &ulock );
  usb_deregister_dev( intf, &class_descr );
  mutex_unlock( &ulock );
}

static struct usb_driver lkm_example4 = {
  .name = "lkm_example4",
  .id_table = lkm_example4_usb_table,
  .probe = lkm_example4_probe,
  .disconnect = lkm_example4_disconnect,
};

static int __init lkm_example4_init(void)
{
  if (usb_register(&lkm_example4) ) {
    printk("lkm_example4: register usb driver failed\n");
    return -EIO;
  }
  return 0;
}

static void __exit lkm_example4_exit(void)
{
  usb_deregister(&lkm_example4);
}

module_init(lkm_example4_init);
module_exit(lkm_example4_exit);
MODULE_LICENSE( "GPL" );
