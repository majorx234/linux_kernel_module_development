#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MajorX234");
MODULE_DESCRIPTION("A example for a simple USB device Linux module.");
MODULE_VERSION("0.01");

#define VENDOR_ID 0x045b
#define PRODUCT_ID 0x0025
// used to have a driver which is compatible with this id

static struct usb_device_id lkm_example3_usb_table[] = {
    {USB_DEVICE(VENDOR_ID,
                PRODUCT_ID)}, // create a USB device Id out of this table
    {}                        // last element need to be empty
};

// make usb table accesable in userspace
MODULE_DEVICE_TABLE(usb, lkm_example3_usb_table);


/**
 * @brief is called when device is attached to usb. Specifies a physical device ID as a device node property so that the bus driver can find the appropriate device driver for this device node
 * @param intf interface (see usb standard) to deal with
 * @param id pointer to the entry of the table above (could be mutiple)
 * @return 0 when probing was succesful
 */
static int lkm_example3_probe(struct usb_interface *intf, const struct usb_device_id* id) {
  printk("lkm_example3 driver - probe function\n");
  return 0;
}

/**
 * @brief This function is called, when the device is diconnected or unload the driver
 */
static void lkm_example3_disconnect(struct usb_interface *intf){
  printk("lkm_example3 - disconnect function");
}

static struct usb_driver lkm_example3_usb_driver = {
  .name = "lkm_example_usb_device_driver",
  .id_table = lkm_example3_usb_table,
  .probe = lkm_example3_probe,
  .disconnect = lkm_example3_disconnect,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init lkm_example3_init(void) {
  int result;
  printk(KERN_INFO "lkm_example3 init\n");
  result = usb_register(&lkm_example3_usb_driver);
  if(result) {
    printk("lkm_example3 driver - error during register\n");
    return -result;
  }
  return 0;
}

/**
 * @brief This function is called, when the module is rmoeved from the kernel
 */
static void __exit lkm_example3_exit(void) {
  printk(KERN_INFO "lkm_example3 exit\n");
  usb_deregister(&lkm_example3_usb_driver);
}

module_init(lkm_example3_init);
module_exit(lkm_example3_exit);
