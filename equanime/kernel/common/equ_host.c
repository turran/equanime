#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <mach/hardware.h>

/* This host driver just maps a large enough memory address to be used as
 * a contiguous memory pool on user space
 * The aesop development board has 64x2 (128 MB) memory
 */

#define DRIVER_NAME "equanime_host"
#define DRIVER_VERSION "0.1"


static int size;

/*============================================================================*
 *                                 Platform                                   *
 *============================================================================*/
static int equ_probe(struct platform_device *pdev)
{
	struct uio_info *info;
	unsigned long kmem_end;
	unsigned long vmem;

	if (size <= 0)
		return -EINVAL;
	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	/* get the kernel memory end */
	kmem_end = virt_to_phys((void *)PAGE_OFFSET) +
               (num_physpages << PAGE_SHIFT);
	vmem = (unsigned long) ioremap_nocache(kmem_end, size);
	if (!vmem)
	{
		kfree(info);
		return -EINVAL;
	}

	info->mem[0].addr = kmem_end;
	info->mem[0].size = size;
	info->mem[0].memtype = UIO_MEM_PHYS;
	info->mem[0].internal_addr = vmem;

	info->version = DRIVER_VERSION;
	info->name = DRIVER_NAME;

	if (uio_register_device(&pdev->dev, info))
	{
		iounmap(vmem);
		kfree(info);
		return -ENODEV;
	}
	platform_set_drvdata(pdev, info);

	printk(KERN_INFO "[Equanime] Host UIO Driver @ %016lx (%d)\n", kmem_end, size);

	return 0;
}

static int equ_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);

	iounmap(info->mem[0].internal_addr);
	uio_unregister_device(info);
	platform_set_drvdata(pdev, NULL);
	kfree (info);

	return 0;
}

static struct platform_device equ_device = {
	.name           = DRIVER_NAME,
	.id             = -1,
	.dev = {
		.coherent_dma_mask      =  0xffffffffUL,
	}
};

static struct platform_driver equ_driver = {
	.driver		=
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= equ_probe,
	.remove		= equ_remove,
};

/*============================================================================*
 *                                  Module                                    *
 *============================================================================*/
static int __init equ_init_module(void)
{

	int ret;

	ret = platform_device_register(&equ_device);
	if (ret)
		return ret;
	ret = platform_driver_register(&equ_driver);
	if (ret)
		platform_device_unregister(&equ_device);
	return ret;
}

static void __exit equ_exit_module(void)
{
	platform_driver_unregister(&equ_driver);
	platform_device_del(&equ_device);
}

module_init(equ_init_module);
module_exit(equ_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jorge Luis Zapata");
module_param(size, int, 0);
MODULE_PARM_DESC(size, "Size of the pool in bytes");
