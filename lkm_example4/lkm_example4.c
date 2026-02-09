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

/**
 * @brief This function is called, when device file is read
 *
 * sends via `usb_control_msg()` a status request to device, specifying a pipe as back channel
 *
 * @param instance
 * @param buffer
 * @param count
 * @param ofs
 * @return 0 when probing was succesful
 */
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

/**
 * @brief This function is called, when device file is opened
 * @param devicefile
 * @param instance - file
 */
static int lkm_example4_usb_driver_open(struct inode *devicefile, struct file *instance)
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

/**
 * @brief is called when device is attached to usb. Specifies a physical device ID as a device node property so that the bus driver can find the appropriate device driver for this device node
 * @param intf interface (see usb standard) to deal with
 * @param id pointer to the entry of the table above (could be mutiple)
 * @return 0 when probing was succesful
 */
static int lkm_example4_probe(struct usb_interface *intf, const struct usb_device_id* id) {
  printk("lkm_example4 usb driver - probe function\n");
  dev = interface_to_usbdev(intf);
  printk("lkm_example4 usb driver: 0x%4.4x|0x%4.4x, if=%p\n",
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

/**
 * @brief This function is called, when the device is diconnected or unload the driver
 * @param intf interface (see usb standard) to deal with
 */
static void lkm_example4_disconnect( struct usb_interface *intf )
{
  /* need to wait for last commands in que */
  mutex_lock( &ulock );
  usb_deregister_dev( intf, &class_descr );
  mutex_unlock( &ulock );
}

static struct usb_driver lkm_example4_usb_driver = {
  .name = "lkm_example4",
  .id_table = lkm_example4_usb_table,
  .probe = lkm_example4_probe,
  .disconnect = lkm_example4_disconnect,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 *
 * through usb_register the driver registers
 * its name, probe and disconnect function and indicates with `id_table` the suitable hardware
 *
 */
static int __init lkm_example4_init(void)
{
  int result;
  printk(KERN_INFO "lkm_example4 init\n");
  result = usb_register(&lkm_example4_usb_driver);
  if ( result ) {
    printk("lkm_example4: register usb driver failed\n");
    return -result;
  }
  return 0;
}

/**
 * @brief This function is called, when the module is rmoeved from the kernel
 */
static void __exit lkm_example4_exit(void)
{
  printk(KERN_INFO "lkm_example4 exit\n");
  usb_deregister(&lkm_example4_usb_driver);
}

module_init(lkm_example4_init);
module_exit(lkm_example4_exit);
MODULE_LICENSE( "GPL" );
