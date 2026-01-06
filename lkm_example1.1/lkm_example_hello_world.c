#include <linux/module.h>
#include <linux/kernel.h>

// initalization
int init_module() {
  printk(KERN_INFO "hello_world loaded\n");
  return 0;
}

void cleanup_module() {
    printk(KERN_INFO "hello_world unloaded\n");
}
