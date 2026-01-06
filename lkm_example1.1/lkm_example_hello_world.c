#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MajorX234");
MODULE_DESCRIPTION("A minimal Hello World example");
MODULE_VERSION("0.01");


// initalization
int init_module() {
  printk(KERN_INFO "hello_world loaded\n");
  return 0;
}

void cleanup_module() {
    printk(KERN_INFO "hello_world unloaded\n");
}
