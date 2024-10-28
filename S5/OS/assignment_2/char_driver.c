#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/utsname.h>

// Module parameters
static char *kernel_version = NULL;
module_param(kernel_version, charp, 0000);
MODULE_PARM_DESC(kernel_version, "Kernel version");

// Device info
static int major;
static int minor = 0;
#define DEVICE_NAME "char_driver"
static char message[256] = {0};
static short read_pos = 0;

// Device read
static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset) 
{
    int bytes_read = 0;
    if (read_pos >= strlen(message)) 
    {
        read_pos = 0;
        return 0;
    }
    while (len && (read_pos < strlen(message))) 
    {
        put_user(message[read_pos], buffer++);
        len--;
        bytes_read++;
        read_pos++;
    }
    printk(KERN_INFO "Read function called\n");
    return bytes_read;
}

// Device write
static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *offset) 
{
    copy_from_user(message, buffer, len);
    message[len] = '\0';
    printk(KERN_INFO "Write function called\n");
    return len;
}

static struct file_operations fops = 
{
    .read = device_read,
    .write = device_write,
};

// Module initialization
static int __init char_driver_init(void) 
{
    struct new_utsname *uts;
    uts = utsname();
    if (!kernel_version || strcmp(kernel_version, uts->release) != 0) 
    {
        printk(KERN_ERR "Kernel version mismatch\n");
        return -EINVAL;
    }

    printk(KERN_INFO "Kernel version matched\n");

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) 
    {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }

    printk(KERN_INFO "Device registered with major number %d and minor number %d\n", major, minor);
    return 0;
}

// Module cleanup
static void __exit char_driver_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

// Macro Initialization
module_init(char_driver_init);
module_exit(char_driver_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Praval Pattam");
MODULE_DESCRIPTION("A simple character device driver made for my college's OS assignment");
