#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <mach/hardware.h>

/* This driver registers several media blocks registers and interrupts
 * for the DM6446 driver:
 * venc
 * osd
 */

#define DM6446_OSD_START 0x0c172600
#define DM6446_OSD_END  0x01c726FC

#define DM6446_VENC_START 0X01c72400
#define DM6446_VENC_END 0X01c725F4

#define DRIVER_NAME "dm6446_media"
#define DRIVER_VERSION "0.1"

/*============================================================================*
 *                                 Platform                                   *
 *============================================================================*/
static int dm6446_media_probe(struct platform_device *pdev)
{
	struct uio_info *info;

	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	/* osd reg */
	info->mem[0].addr = DM6446_OSD_START;
	info->mem[0].size = DM6446_OSD_END - DM6446_OSD_END + 1;
	info->mem[0].memtype = UIO_MEM_PHYS;
	/* venc reg */
	info->mem[1].addr = DM6446_VENC_START;
	info->mem[1].size = DM6446_VENC_END - DM6446_VENC_END + 1;
	info->mem[1].memtype = UIO_MEM_PHYS;

	info->version = DRIVER_VERSION;
	info->name = DRIVER_NAME;

	if (uio_register_device(&pdev->dev, info))
	{
		kfree(info);
		return -ENODEV;
	}
	platform_set_drvdata(pdev, info);

	printk(KERN_INFO "[DM6446] Media UIO Driver\n");

	return 0;
}

static int dm6446_media_remove(struct platform_device *pdev)
{
	struct uio_info *info = platform_get_drvdata(pdev);

	uio_unregister_device(info);
	platform_set_drvdata(pdev, NULL);
	kfree (info);

	return 0;
}

static struct platform_device dm6446_media_device = {
	.name           = DRIVER_NAME,
	.id             = -1,
	.dev = {
		.coherent_dma_mask      =  0xffffffffUL,
	},
};

static struct platform_driver dm6446_media_driver = {
	.driver		=
	{
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= dm6446_media_probe,
	.remove		= dm6446_media_remove,
};
/*============================================================================*
 *                                  Module                                    *
 *============================================================================*/
static int __init dm6446_media_init_module(void)
{

	int ret;

	ret = platform_device_register(&dm6446_media_device);
	if (ret)
		return ret;
	ret = platform_driver_register(&dm6446_media_driver);
	if (ret)
	{
		platform_device_unregister(&dm6446_media_device);
		return ret;
	}
	return ret;
}

static void __exit dm6446_media_exit_module(void)
{
	platform_driver_unregister(&dm6446_media_driver);
	platform_device_del(&dm6446_media_device);
}

module_init(dm6446_media_init_module);
module_exit(dm6446_media_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jorge Luis Zapata");
