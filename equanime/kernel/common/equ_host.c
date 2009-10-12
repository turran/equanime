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

struct equanime_host
{
	/* keep the uio_info here */
	/* store every dma memory allocated */
};

/*============================================================================*
 *                                 Platform                                   *
 *============================================================================*/
#if 0
static int mp2530f_host_probe(struct platform_device *pdev)
{
	struct uio_info *info;
	struct resource *r;
	int ret;
	unsigned int size;
	dma_addr_t map_dma;

	printk(KERN_INFO "[Equanime] Host UIO Driver\n");

	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	/* TODO read the memory size */
	/* check the boundings */
	/*  block_virtp = (unsigned long) ioremap_nocache(block_start, length); */
	info->mem[0].addr = r->start;
	info->mem[0].size = r->end - r->start + 1;
	info->mem[0].memtype = UIO_MEM_PHYS;
	info->mem[0].internal_addr = ioremap(info->mem[0].addr, info->mem[0].size);

	if (!info->mem[0].internal_addr)
	{
	                printk(KERN_ERR "Failed to remap mem 0\n");
	                ret = -ENOMEM;
	                goto out_get;
	}
	/* TODO Rotator interrupt */


	info->version = DRIVER_VERSION;
	info->name = DRIVER_NAME;

	if (uio_register_device(&pdev->dev, info))
	{
		ret = -ENODEV;
		goto out_irq;
	}

	platform_set_drvdata(pdev, info);

	return 0;
out_unmap:
	release_mem_region(r->start, r->end - r->start + 1);
out_get:
	kfree(info);
	return ret;
}

static int mp2530f_host_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	uio_unregister_device(info);

	kfree (info);
	return 0;
}

static struct platform_driver mp2530f_host_driver = {
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mp2530f_host_probe,
	.remove		= mp2530f_host_remove,
};
#endif
/*============================================================================*
 *                                  Module                                    *
 *============================================================================*/
static int __init equ_host_init_module(void)
{
	unsigned long kmem_end;

	/* get the kernel memory end */
	kmem_end = virt_to_phys((void *)PAGE_OFFSET) +
               (num_physpages << PAGE_SHIFT);
	printk("KERNEL END = %016lx\n", kmem_end);

	return -EINVAL;
	/* TODO Instead of using a platform driver just do the probe here */
#if 0
	return platform_driver_register(&equ_host_driver);
#endif
}

static void __exit equ_host_exit_module(void)
{
#if 0
	platform_driver_unregister(&equ_host_driver);
#endif
}

module_init(equ_host_init_module);
module_exit(equ_host_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jorge Luis Zapata");
