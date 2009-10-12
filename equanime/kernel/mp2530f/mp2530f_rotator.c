#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <asm/arch/hardware.h>

#define DRIVER_NAME "mp2530f_rotator"
#define DRIVER_VERSION "0.1"

struct mp2530f_rotator
{
	/* keep the uio_info here */
	/* store every dma memory allocated */
};

/*============================================================================*
 *                                 Platform                                   *
 *============================================================================*/
static int mp2530f_rotator_probe(struct platform_device *pdev)
{
	struct uio_info *info;
	struct resource *r;
	int ret;
	unsigned int size;
	dma_addr_t map_dma;

	printk(KERN_INFO "[MP2530F] Rotator UIO Driver\n");

	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	/* Rotator registers */
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r)
	{
		ret = -ENXIO;
		goto out_get;
	}

	if (!request_mem_region(r->start, r->end - r->start + 1, pdev->name))
	{
		ret = -ENXIO;
		goto out_get;
	}
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
out_irq:
	/* TODO release the interrupt */
out_unmap:
	release_mem_region(r->start, r->end - r->start + 1);
out_get:
	kfree(info);
	return ret;
}

static int mp2530f_rotator_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	uio_unregister_device(info);

	kfree (info);
	return 0;
}

#ifdef CONFIG_PM
static int mp2530f_rotator_suspend(struct platform_device *dev, pm_message_t state)
{

}

static int mp2530f_rotator_resume(struct platform_device *dev)
{

}

#else
#define mp2530f_rotator_suspend  NULL
#define mp2530f_rotator_resume   NULL
#endif /* CONFIG_PM */


static struct platform_driver mp2530f_rotator_driver = {
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= mp2530f_rotator_probe,
	.remove		= mp2530f_rotator_remove,
	.suspend	= mp2530f_rotator_suspend,
	.resume		= mp2530f_rotator_resume,
};
/*============================================================================*
 *                                  Module                                    *
 *============================================================================*/
static int __init mp2530f_rotator_init_module(void)
{
	return platform_driver_register(&mp2530f_rotator_driver);
}

static void __exit mp2530f_rotator_exit_module(void)
{
	platform_driver_unregister(&mp2530f_rotator_driver);
}

module_init(mp2530f_rotator_init_module);
module_exit(mp2530f_rotator_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jorge Luis Zapata");
