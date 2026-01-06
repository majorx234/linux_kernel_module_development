#include <linux/init.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MajorX234");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static int __init lkm_example_init(void) {
  printk(KERN_INFO "LKM Example1\n");
  return 0;
}

static void __exit lkm_example_exit(void) {
  printk(KERN_INFO "LKM Example1 exit!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
