#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/string.h>

#include <asm/uaccess.h>

#define PROCFS_NAME "kmsg_log"

static ssize_t procfile_write(struct file *file, const char __user *buffer, size_t count,
		loff_t *data) {
	int ret;
	char* text;
	char* ptr;
	char* tmp;

	text = (char *)vmalloc((count + 1) * sizeof(char));
	if ( copy_from_user(text, buffer, count) ) {
		ret = -EFAULT;
		goto end;
	}
	text[count] = '\0';

	// tokenize on newline characters
	ptr = text;
	tmp = strsep(&ptr, "\r\n");
	while (tmp != NULL)
	{
		if (strlen(tmp) > 0)
			printk(KERN_INFO "%s\n", tmp);

		tmp = strsep(&ptr, "\r\n");
	}
	ret = count;

end:
	vfree(text);
	return ret;
}

static struct proc_dir_entry *procfile;

static const struct file_operations procfile_fops = {
	.owner = THIS_MODULE,
	.write = procfile_write,
};

static int __init kmsg_log_init(void) {
	procfile = proc_create(PROCFS_NAME, 0200, NULL, &procfile_fops);

	if (procfile == NULL) {
		remove_proc_entry(PROCFS_NAME, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
	return 0;
}

static void __exit kmsg_log_exit(void) {
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

module_init(kmsg_log_init);
module_exit(kmsg_log_exit);

MODULE_LICENSE("GPL");
