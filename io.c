#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/slab.h> 
#include <linux/errno.h>
#include <linux/types.h> 
#include <linux/fs.h> 
#include <linux/proc_fs.h> 
#include <asm/uaccess.h> 
#define MAJOR_NUMBER 61 
/* forward declaration */ 
int onebyte_open(struct inode *inode, struct file *filep); 
int onebyte_release(struct inode *inode, struct file *filep); 
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos); 
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos); 
static void onebyte_exit(void); 
/* definition of file_operation structure */ 
struct file_operations onebyte_fops = { 
	read: 
	onebyte_read, 
	write: 
	onebyte_write, 
	open: 
	onebyte_open, 
	release: onebyte_release 
}; 
char *onebyte_data = NULL; 
int onebyte_open(struct inode *inode, struct file *filep) 
{ 
	return 0; // always successful 
} 
int onebyte_release(struct inode *inode, struct file *filep) 
{ 
	return 0; // always successful 
} 
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos) 
{ 
	/*please complete the function on your own*/ 
	printk(KERN_INFO "onebyte_read %s\n", buf);
	if(*f_pos !=0){
		return 0;
	}else{
		copy_to_user(buf, onebyte_data, 1);
		*f_pos += 1;
		 return 1;
	}
} 
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos) 
{
	/*please complete the function on your own*/ 
	printk(KERN_INFO "onebyte_write %s\n", buf);
	if(*f_pos !=0){
		printk(KERN_INFO "onebyte_write: NOT ENOUGH SPACE!");
		return ENOSPC;
	}else{
		printk(KERN_INFO "onebyte_write: Written to memory.");
		copy_from_user(onebyte_data, buf, 1);
		*f_pos += 1;
		return 1;
	}
} 
static int onebyte_init(void) 
{ 
	int result; 
	// register the device 
	result = register_chrdev(MAJOR_NUMBER, "onebyte", &onebyte_fops); 
	if (result < 0) { 
		return result; 
	} 
	// allocate one byte of memory for storage 
	// kmalloc is just like malloc, the second parameter is 
	// the type of memory to be all
