## Make the makefile

```make clean
make```

## Insert the char_driver
`sudo insmod char_driver.ko kernel_version=$(uname -r)`

## Check the kernel log to see if the insertion is successful or not
`sudo dmesg | tail`

## Entering the major number and minor number 
using the command sudo mknod /dev/char_driver c <major_number> <major_number> where we enter the major and minor number that we get from the kernel logs

`sudo mknod /dev/char_driver c 238 0`

## Setting permissions
`sudo chmod 666 /dev/char_driver`

## Writing to the device
`echo PRAVAL_B220057CS > /dev/char_driver`

## Read from the device
`cat /dev/char_driver`

## Check the kernel log to see if the read/write is successful or not
`sudo dmesg | tail`

## Compiling the given user program
`gcc user_prog.c -o user_prog`

## Remove the char_driver
```sudo rm /dev/char_driver
sudo rmmod```