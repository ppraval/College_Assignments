# Instructions on how to use the Device driver.

## Make the makefile.

```sh
make clean
make
```

## Insert the char_driver.
```sh
sudo insmod char_driver.ko kernel_version=$(uname -r)
```

## Check the kernel log to see if the insertion is successful or not.
```sh
sudo dmesg | tail
```

## Entering the major number and minor number.
Using the command 
sudo mknod /dev/char_driver c <major_number> <major_number> 
entering the major and minor number that we get from the kernel logs.

## For my device
```sh
sudo mknod /dev/char_driver c 238 0
```

## Setting permissions for the Character driver.
```sh
sudo chmod 666 /dev/char_driver
```

## Writing to the device.
```sh
echo PRAVAL_B220057CS > /dev/char_driver
```

## Read from the device.
```sh
cat /dev/char_driver
```

## Check the kernel log to see if the read/write is successful or not.
```sh
sudo dmesg | tail
```

## Compiling the given user program.
```sh
gcc user_prog.c -o user_prog
```

## Remove the device.
```sh
sudo rmmod /dev/char_driver
sudo rm /dev/char_driver
```