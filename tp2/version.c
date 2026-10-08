/* version.c */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <generated/utsrelease.h>
#include <linux/utsname.h>

/* signatures des callbacks read, write et ioctl */
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
// static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/* définition des opérations sur le fichier /dev/version */
static const struct file_operations my_fops = {
    .owner          = THIS_MODULE,
    .read           = my_read,              /* cat /dev/version */
    .write          = my_write,             /* echo "new version" > /dev/version */
    //.unlocked_ioctl = my_ioctl,             /* */
};

/* définition du périphérique misc /dev/version */
static struct miscdevice version_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "version",                      /* device name : /dev/version */
    .fops = &my_fops,                       /* opération sur /dev/version */

};


char *current_kernel_version;


/* 
    get the data from a source and writes the data to *buf.
    returns : the number of bytes read from the source or an error code.
*/
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Driver : my_read called\n");

    int err;
    size_t size = strlen(current_kernel_version);

    if(*off > 0){
        return 0; //EOF
    }

    if (len > size - *off)
    {
        len = size - *off;
    }
    
    // write the kernel version to the user space buffer buf
    err = copy_to_user(buf, current_kernel_version, len);
    if (err != 0){
        return -EFAULT;
    }
    else{
        *off += sizeof(current_kernel_version);
        return sizeof(current_kernel_version);
    }
    
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("Driver : my_write called\n");
    int err;
    size_t size = strlen(current_kernel_version);

    char *user_kernel_version = (char*)kmalloc(len, GFP_KERNEL);
    if (user_kernel_version == NULL) {
        return -ENOMEM;
    }

    // copy the data from user space buffer (buf) to kernel space (buffer user_kernel_version)
    err = copy_from_user(user_kernel_version, buf, len);
    if (err != 0) {
        return -EFAULT;
    }

    if (size > len){
        len = size;
    }

    // update the current kernel version with the new version provided by the user
    len = sprintf(current_kernel_version, "%s\n", user_kernel_version);
    kfree(user_kernel_version);

    return len; // return the number of bytes written in current_kernel_version
}


static int __init version_init(void)
{
    // get current kernel version
    pr_info("Driver : version_init %s \n", init_utsname()->release);

    // copier la version du noyau dans current_kernel_version
    current_kernel_version = init_utsname()->release;
    

    return misc_register(&version_misc_device);
}

static void __exit version_exit(void)
{
    misc_deregister(&version_misc_device);
}


module_init(version_init);
module_exit(version_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabien VERSPAN");
MODULE_DESCRIPTION("A simple version module");
