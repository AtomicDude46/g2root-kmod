#include <linux/module.h>
#include <linux/init.h>

#include <linux/kobject.h>
#include <linux/platform_device.h>
#include "../fs/sysfs/sysfs.h"

struct kobject *pbus_kobject;
struct sysfs_dirent *sd;
struct sysfs_dirent *dir;

static void found_it(struct sysfs_dirent *dir) {
	struct kobject *kobj;
	struct device *dev;
	struct platform_device *pdev;

	kobj = dir->s_dir.kobj;	
	printk("dirent kobject: %.8x\n", (unsigned int)kobj);
	dev = container_of(kobj, struct device, kobj);
	printk("dirent dev: %.8x\n", (unsigned int)dev);
	pdev = container_of(dev, struct platform_device, dev);
	printk("dirent pdev: %.8x\n", (unsigned int)pdev);

	printk("pdev name: %s\n", pdev->name);

}

static void walk_dir(struct sysfs_dirent *dir) {

	struct sysfs_dirent *cur;
	
//	printk("dirent flags: %d\n", dir->s_flags);
//	printk("dirent name: %s\n", dir->s_name);

	if (strcmp(dir->s_name, "msm_sdcc.2") == 0) {
		printk("found msm_sdcc.2: %s\n", dir->s_name);
		found_it(dir);
		return;
	}

	if (dir->s_flags & SYSFS_DIR) {
		for (cur = dir->s_dir.children; cur->s_sibling != NULL; cur = cur->s_sibling) walk_dir(cur);
		return;
	};

	if (dir->s_flags & SYSFS_KOBJ_ATTR) {
		return;
	};

	return;
}

static int __init test_init(void) {
        printk("test module loaded\n");

	pbus_kobject = &platform_bus.kobj;
	sd = pbus_kobject->sd;

	printk("platform_bus kobject: %.8x\n", (unsigned int)pbus_kobject);

	walk_dir(sd);

	return 0;
}

static void __exit test_exit(void) {
        printk("test module unloaded\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
