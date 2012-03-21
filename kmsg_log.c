#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/string.h>

#include <asm/uaccess.h>

static const char* PROCFS_NAME = "kmsg_log";

// module parameters
static int raw = 0;

static ssize_t procfile_write ( struct file *file, const char __user *buffer,
		size_t count, loff_t *data ) {
	char *text, *ptr, *tmp;

	text = (char *)vmalloc((count + 1) * sizeof(char));
	if ( text == NULL ) {
		// out of memory, bail
		return -ENOMEM;
	}

	if ( copy_from_user(text, buffer, count) ) {
		// free our memory and bail. something went wrong
		vfree(text);
		return -EFAULT;
	}
	// add a null-terminator just to be safe
	text[count] = '\0';

	if (raw) {
		// dump raw data
		printk(text);
	} else {
		// tokenize on newline characters
		ptr = text;
		tmp = strsep(&ptr, "\r\n");
		while ( tmp != NULL ) {
			// ignore empty lines
			if ( strlen(tmp) > 0 )
				printk(KERN_INFO "%s\n", tmp);

			tmp = strsep(&ptr, "\r\n");
		}
	}

	vfree(text);
	return count;
}

static struct proc_dir_entry *procfile;

static const struct file_operations procfile_fops = {
	.owner = THIS_MODULE,
	.write = procfile_write,
};

static int __init kmsg_log_init ( void ) {
	procfile = proc_create(PROCFS_NAME, S_IWUSR, NULL, &procfile_fops);

	if ( procfile == NULL ) {
		remove_proc_entry(PROCFS_NAME, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
	return 0;
}

static void __exit kmsg_log_exit ( void ) {
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Austen Dicken <cvpcsm@gmail.com>");
MODULE_DESCRIPTION("Provides a mechanism to log to the kernel ring buffer via /proc");

module_init(kmsg_log_init);
module_exit(kmsg_log_exit);

module_param(raw, bool, S_IRUSR);
MODULE_PARM_DESC(raw, "Whether to send raw output to klog, or filter it");
