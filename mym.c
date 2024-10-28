#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "share.h"

static struct class* myc=NULL;
static struct device* myd=NULL;

static int major;
static int numOpen=0;
static char message[256]={0};
static short size_of_message;

static int myopen(struct inode * , struct file *);
static int myrelease(struct inode * , struct file *);
static ssize_t myread(struct file * , char * , size_t , loff_t *);
static ssize_t mywrite(struct file * , const char * , size_t , loff_t *);

struct file_operations fops={
    .open=myopen,
    .release=myrelease,
    .read=myread,
    .write=mywrite,
};

static int __init myinit(void){
    major=register_chrdev(0,DEVICE_NAME,&fops);
    if(major<0){printk(KERN_INFO "***error, not register***\n"); return major;}
    printk(KERN_INFO "device is registered with mojor number %d\n",major);

    if(IS_ERR(myc=class_create(THIS_MODULE,CLASS_NAME))){
        unregister_chrdev(major,DEVICE_NAME);
        printk(KERN_INFO "***error, could not create class***\n");
        return PTR_ERR(myc);
    }
    if(IS_ERR(myd=device_create(myc,NULL,MKDEV(major,0),NULL,DEVICE_NAME))){
        class_destroy(myc);
        unregister_chrdev(major,DEVICE_NAME);
        printk(KERN_INFO "***error, could not create device***\n");
        return PTR_ERR(myd);
    }
    return 0;
}

static void __exit myexit(void){
    printk(KERN_INFO "Goodbye");
    device_destroy(myc, MKDEV(major, 0));
    class_unregister(myc);
    class_destroy(myc);
    unregister_chrdev(major, DEVICE_NAME);
}

static int myopen(struct inode *inodep , struct file *filep){
    numOpen++;
    printk(KERN_INFO "file opened, %d file is open\n",numOpen);
    return 0;
}

static int myrelease(struct inode *inodep , struct file *filep){
    printk(KERN_INFO "file closed\n");
    return 0;
}

static ssize_t myread(struct file *filep, char *buffer, size_t len, loff_t *offset){
    int nerr=copy_to_user(buffer,message,size_of_message);
    if(nerr==0){
        printk(KERN_INFO "message [%s] readed from file and its size is %d\n",message,size_of_message);
        return (size_of_message=0);
    }
    else{
        printk(KERN_INFO "***error, could not read form file***\n");
        return 0;
    }
}

static ssize_t mywrite(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    sprintf(message,"%s",buffer,len);
    size_of_message=strlen(message);
    printk(KERN_INFO "message [%s] writed to file and its size is %d\n",message,size_of_message);
    return len;
}

module_init(myinit);
module_exit(myexit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MahdiSaieedi, ;MohammadSalehi,9427283");
MODULE_DESCRIPTION("Project1 of OS");
MODULE_VERSION("0.1");