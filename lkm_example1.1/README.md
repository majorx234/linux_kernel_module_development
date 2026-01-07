# Info
- simple `Hello World` minimal kernel driver example

# HowTo
- build: `make -C /lib/modules/$(uname -r)/build/` M=$PWD
- view kernel output: `sudo dmesg -w`
- load: `sudo insmod lkm_example_hello_world.ko`
- unload: `sudo rmmod lkm_example_hello_world.ko`

