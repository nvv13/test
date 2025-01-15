Источник
https://community.milkv.io/t/milk-v-duo-tinydrm-ili9488-st7789/1223

*******************************************************************************
Milk-V Duo tinydrm driver screen (ili9488/st7789)

firstman   1 Jan 2024


This time, we will introduce the tinydrm driver screen.
 The detailed process will be much simpler than the last fbtft driver screen method.
                   https://community.milkv.io/t/milk-v-duo-spi-st7789/131
 For detailed operation process, please refer to the previous post.


The tinydrm driver has long been incorporated into the current Linux kernel mainline.
 The original author also introduced the advantages of tinydrm over fbtft.
 You can learn from it and write drivers for other models of screens by yourself.


The following is based on the latest sdk1.0.7 to drive the ili9488 screen:
1. Create the file ili9488.c in the directory /linux_5.10/drivers/gpu/drm/tiny/

----------------------------ili9488.c-----------------------------------------------------
// SPDX-License-Identifier: GPL-2.0
/*
 * DRM driver for Ilitek ILI9488 panels
 *
 * Copyright 2020 Kamlesh Gurudasani <kamlesh.gurudasani@gmail.com>
 */

#include <linux/delay.h>
#include <linux/dma-buf.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/spi/spi.h>
#include <video/mipi_display.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_damage_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_cma_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_mipi_dbi.h>
#include <drm/drm_rect.h>

#define ILI9488_VCOM_CONTROL_1			0xC5
#define ILI9488_COLUMN_ADDRESS_SET		0x2A
#define ILI9488_PAGE_ADDRESS_SET		0x2B
#define ILI9488_MEMORY_WRITE			0x2C
#define ILI9488_POSITIVE_GAMMA_CORRECTION	0xE0
#define ILI9488_NEGATIVE_GAMMA_CORRECTION	0xE1
#define ILI9488_POWER_CONTROL_1			0xC0
#define ILI9488_POWER_CONTROL_2			0xC1

#define ILI9488_MEMORY_ACCESS_CONTROL		0x36
#define ILI9488_COLMOD_PIXEL_FORMAT_SET		0x3A
#define ILI9488_INTERFACE_MODE_CONTROL		0xB0
#define ILI9488_FRAME_RATE_CONTROL_PARTIAL	0xB3
#define ILI9488_DISPLAY_INVERSION_CONTROL	0xB4
#define ILI9488_SET_IMAGE_FUNCTION		0xE9
#define ILI9488_ADJUST_CONTROL_3		0xF7
#define ILI9488_ADJUST_CONTROL_3		0xF7
#define ILI9488_DISPLAY_ON			0x29
#define ILI9488_DISPLAY_OFF			0x28
#define ILI9488_ENTER_SLEEP_MODE		0x10
#define ILI9488_DBI_BPP18			0x06
#define ILI9488_DPI_BPP18			0x60
#define ILI9488_FRAME_RATE_CONTROL_NORMAL	0xB1
#define ILI9488_SLEEP_OUT			0x11

#define ILI9488_MADCTL_BGR	BIT(3)
#define ILI9488_MADCTL_MV	BIT(5)
#define ILI9488_MADCTL_MX	BIT(6)
#define ILI9488_MADCTL_MY	BIT(7)

static void ili9488_rgb565_to_rgb666_line(u8 *dst, u16 *sbuf,
					  unsigned int pixels)
{
	unsigned int x;

	for (x = 0; x < pixels; x++) {
		*dst++ = ((*sbuf & 0xF800) >> 8);
		*dst++ = ((*sbuf & 0x07E0) >> 3);
		*dst++ = ((*sbuf & 0x001F) << 3);
		sbuf++;
	}
}

static void ili9488_rgb565_to_rgb666(u8 *dst, void *vaddr,
				     struct drm_framebuffer *fb,
				     struct drm_rect *rect)
{
	size_t linepixels = rect->x2 - rect->x1;
	size_t src_len = linepixels * sizeof(u16);
	size_t dst_len = linepixels * 3;
	unsigned int y, lines = rect->y2 - rect->y1;
	u16 *sbuf;

	/*
	 * The cma memory is write-combined so reads are uncached.
	 * Speed up by fetching one line at a time.
	 */
	sbuf = kmalloc(src_len, GFP_KERNEL);
	if (!sbuf)
		return;

	vaddr += rect->y1 * fb->pitches[0] + rect->x1 * sizeof(u16);
	for (y = 0; y < lines; y++) {
		memcpy(sbuf, vaddr, src_len);
		ili9488_rgb565_to_rgb666_line(dst, sbuf, linepixels);
		vaddr += fb->pitches[0];
		dst += dst_len;
	}
	kfree(sbuf);
}

static int ili9488_buf_copy(void *dst, struct drm_framebuffer *fb,
			    struct drm_rect *rect)
{
	struct drm_gem_cma_object *cma_obj = drm_fb_cma_get_gem_obj(fb, 0);
	struct dma_buf_attachment *import_attach = cma_obj->base.import_attach;
	struct drm_format_name_buf format_name;
	void *src = cma_obj->vaddr;
	int ret = 0;

	if (import_attach) {
		ret = dma_buf_begin_cpu_access(import_attach->dmabuf,
					       DMA_FROM_DEVICE);
		if (ret)
			return ret;
	}

	switch (fb->format->format) {
	case DRM_FORMAT_RGB565:
		ili9488_rgb565_to_rgb666(dst, src, fb, rect);
		break;
	default:
		dev_err_once(fb->dev->dev, "Format is not supported: %s\n",
			     drm_get_format_name(fb->format->format,
						 &format_name));
		return -EINVAL;
	}

	if (import_attach)
		ret = dma_buf_end_cpu_access(import_attach->dmabuf,
					     DMA_FROM_DEVICE);
	return ret;
}

static void ili9488_fb_dirty(struct drm_framebuffer *fb, struct drm_rect *rect)
{
	struct drm_gem_cma_object *cma_obj = drm_fb_cma_get_gem_obj(fb, 0);
	struct mipi_dbi_dev *dbidev = drm_to_mipi_dbi_dev(fb->dev);
	struct mipi_dbi *dbi = &dbidev->dbi;
	int idx, ret = 0;
	void *tr;
	bool full;
	unsigned int height = rect->y2 - rect->y1;
	unsigned int width = rect->x2 - rect->x1;

	// if (!dbidev->enabled)
	// 	return;

	if (!drm_dev_enter(fb->dev, &idx))
		return;

	full = width == fb->width && height == fb->height;

	DRM_DEBUG_KMS("Flushing [FB:%d] " DRM_RECT_FMT "\n", fb->base.id,
		      DRM_RECT_ARG(rect));

	/* Always invoke copy buffer routine as the display supports
	 * only RGB666 format which is not implemented in DRM
	 */
	if (!dbi->dc || !full ||
	    fb->format->format == DRM_FORMAT_RGB565) {
		tr = dbidev->tx_buf;
		ret = ili9488_buf_copy(dbidev->tx_buf, fb, rect);
		if (ret)
			goto err_msg;
	} else {
		tr = cma_obj->vaddr;
	}

	mipi_dbi_command(dbi, ILI9488_COLUMN_ADDRESS_SET,
			 (rect->x1 >> 8) & 0xFF, rect->x1 & 0xFF,
			 (rect->x2 >> 8) & 0xFF, (rect->x2 - 1) & 0xFF);

	mipi_dbi_command(dbi, ILI9488_PAGE_ADDRESS_SET,
			 (rect->y1 >> 8) & 0xFF, rect->y1 & 0xFF,
			 (rect->y2 >> 8) & 0xFF, (rect->y2 - 1) & 0xFF);

	ret = mipi_dbi_command_buf(dbi, ILI9488_MEMORY_WRITE, tr,
				   width * height * 3);

 err_msg:
	if (ret)
		dev_err_once(fb->dev->dev, "Failed to update display %d\n", ret);

	drm_dev_exit(idx);
}

static void ili9488_pipe_update(struct drm_simple_display_pipe *pipe,
				struct drm_plane_state *old_state)
{
	struct drm_plane_state *state = pipe->plane.state;
	struct drm_rect rect;

	if (drm_atomic_helper_damage_merged(old_state, state, &rect))
		ili9488_fb_dirty(state->fb, &rect);
}

static void ili9488_pipe_enable(struct drm_simple_display_pipe *pipe,
				struct drm_crtc_state *crtc_state,
				struct drm_plane_state *plane_state)
{
	struct mipi_dbi_dev *dbidev = drm_to_mipi_dbi_dev(pipe->crtc.dev);
	struct drm_framebuffer *fb = plane_state->fb;
	struct mipi_dbi *dbi = &dbidev->dbi;
	u8 addr_mode;
	int ret, idx;
	struct drm_rect rect = {
		.x1 = 0,
		.x2 = fb->width,
		.y1 = 0,
		.y2 = fb->height,
	};

	if (!drm_dev_enter(pipe->crtc.dev, &idx))
		return;

	DRM_DEBUG_KMS("\n");

	ret = mipi_dbi_poweron_conditional_reset(dbidev);
	if (ret < 0)
		goto out_exit;
	if (ret == 1)
		goto out_enable;

	mipi_dbi_command(dbi, ILI9488_POSITIVE_GAMMA_CORRECTION,
			 0x00, 0x03, 0x09, 0x08, 0x16,
			 0x0a, 0x3f, 0x78, 0x4c, 0x09,
			 0x0a, 0x08, 0x16, 0x1a, 0x0f);

	mipi_dbi_command(dbi, ILI9488_NEGATIVE_GAMMA_CORRECTION,
			 0x00, 0x16, 0x19, 0x03, 0x0f,
			 0x05, 0x32, 0x45, 0x46, 0x04,
			 0x0e, 0x0d, 0x35, 0x37, 0x0f);

	mipi_dbi_command(dbi, ILI9488_POWER_CONTROL_1, 0x17, 0x15);

	mipi_dbi_command(dbi, ILI9488_POWER_CONTROL_2, 0x41);

	mipi_dbi_command(dbi, ILI9488_VCOM_CONTROL_1, 0x00, 0x12, 0x80);

	mipi_dbi_command(dbi, ILI9488_COLMOD_PIXEL_FORMAT_SET,
			 ILI9488_DBI_BPP18 | ILI9488_DPI_BPP18);

	mipi_dbi_command(dbi, ILI9488_INTERFACE_MODE_CONTROL, 0x80);

	mipi_dbi_command(dbi, ILI9488_FRAME_RATE_CONTROL_NORMAL, 0xa0);

	mipi_dbi_command(dbi, ILI9488_DISPLAY_INVERSION_CONTROL, 0x02);

	mipi_dbi_command(dbi, ILI9488_SET_IMAGE_FUNCTION, 0x00);

	mipi_dbi_command(dbi, ILI9488_ADJUST_CONTROL_3,
			 0xa9, 0x51, 0x2c, 0x82);

	mipi_dbi_command(dbi, ILI9488_SLEEP_OUT);

	msleep(120);

	mipi_dbi_command(dbi, ILI9488_DISPLAY_ON);

	// dbidev->enabled = true;
	ili9488_fb_dirty(fb, &rect);

 out_enable:
	switch (dbidev->rotation) {
	default:
		addr_mode = ILI9488_MADCTL_MX;
		break;
	case 90:
		addr_mode = ILI9488_MADCTL_MV;
		break;
	case 180:
		addr_mode = ILI9488_MADCTL_MY;
		break;
	case 270:
		addr_mode = ILI9488_MADCTL_MV | ILI9488_MADCTL_MY |
			ILI9488_MADCTL_MX;
		break;
	}
	addr_mode |= ILI9488_MADCTL_BGR;
	mipi_dbi_command(dbi, MIPI_DCS_SET_ADDRESS_MODE, addr_mode);
 out_exit:
	drm_dev_exit(idx);
}

static void ili9488_pipe_disable(struct drm_simple_display_pipe *pipe)
{
	struct mipi_dbi_dev *dbidev = drm_to_mipi_dbi_dev(pipe->crtc.dev);

	/*
	 * This callback is not protected by drm_dev_enter/exit since we want to
	 * turn off the display on regular driver unload. It's highly unlikely
	 * that the underlying SPI controller is gone should this be called
	 * after unplug.
	 */

	DRM_DEBUG_KMS("\n");

	// if (!dbidev->enabled)
	// 	return;

	mipi_dbi_command(&dbidev->dbi, MIPI_DCS_SET_DISPLAY_OFF);
	// dbidev->enabled = false;
}

static const u32 ili9488_formats[] = {
	DRM_FORMAT_RGB565,
};

static const struct drm_simple_display_pipe_funcs ili9488_pipe_funcs = {
	.enable = ili9488_pipe_enable,
	.disable = ili9488_pipe_disable,
	.update = ili9488_pipe_update,
	.prepare_fb = drm_gem_fb_simple_display_pipe_prepare_fb,
};

static const struct drm_display_mode ili9488_mode = {
	DRM_SIMPLE_MODE(320, 480, 49, 73),
};

DEFINE_DRM_GEM_CMA_FOPS(ili9488_fops);

static struct drm_driver ili9488_driver = {
	.driver_features	= DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	.fops			= &ili9488_fops,
	DRM_GEM_CMA_DRIVER_OPS_VMAP,
	.debugfs_init		= mipi_dbi_debugfs_init,
	.name			= "ili9488",
	.desc			= "Ilitek ILI9488",
	.date			= "20200607",
	.major			= 1,
	.minor			= 0,
};

static const struct of_device_id ili9488_of_match[] = {
	{ .compatible = "eastrising,er-tft035-6" },
	{ }
};
MODULE_DEVICE_TABLE(of, ili9488_of_match);

static const struct spi_device_id ili9488_id[] = {
	{ "er-tft035-6", 0 },
	{ }
};
MODULE_DEVICE_TABLE(spi, ili9488_id);

static int ili9488_probe(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	struct mipi_dbi_dev *dbidev;
	struct drm_device *drm;
	struct mipi_dbi *dbi;
	struct gpio_desc *dc;
	u32 rotation = 0;
	size_t bufsize;
	int ret;

	dbidev = devm_drm_dev_alloc(dev, &ili9488_driver,
				    struct mipi_dbi_dev, drm);
	if (IS_ERR(dbidev))
		return PTR_ERR(dbidev);

	dbi = &dbidev->dbi;
	drm = &dbidev->drm;

	// ret = devm_drm_dev_init(dev, drm, &ili9488_driver);
	// if (ret) {
	// 	kfree(dbidev);
	// 	return ret;
	// }

	drm_mode_config_init(drm);

	bufsize = ili9488_mode.vdisplay * ili9488_mode.hdisplay * 3;

	dbi->reset = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(dbi->reset)) {
		DRM_DEV_ERROR(dev, "Failed to get gpio 'reset'\n");
		return PTR_ERR(dbi->reset);
	}

	dc = devm_gpiod_get(dev, "dc", GPIOD_OUT_LOW);
	if (IS_ERR(dc)) {
		DRM_DEV_ERROR(dev, "Failed to get gpio 'dc'\n");
		return PTR_ERR(dc);
	}

	dbidev->backlight = devm_of_find_backlight(dev);
	if (IS_ERR(dbidev->backlight))
		return PTR_ERR(dbidev->backlight);

	device_property_read_u32(dev, "rotation", &rotation);

	ret = mipi_dbi_spi_init(spi, dbi, dc);
	if (ret)
		return ret;

	dbidev->drm.mode_config.preferred_depth = 16;

	ret = mipi_dbi_dev_init_with_formats(dbidev, &ili9488_pipe_funcs,
					     ili9488_formats,
					     ARRAY_SIZE(ili9488_formats),
					     &ili9488_mode, rotation, bufsize);
	if (ret)
		return ret;

	drm_mode_config_reset(drm);

	ret = drm_dev_register(drm, 0);
	if (ret)
		return ret;

	spi_set_drvdata(spi, drm);

	drm_fbdev_generic_setup(drm, 0);

	return 0;
}

static int ili9488_remove(struct spi_device *spi)
{
	struct drm_device *drm = spi_get_drvdata(spi);

	drm_dev_unplug(drm);
	drm_atomic_helper_shutdown(drm);

	return 0;
}

static void ili9488_shutdown(struct spi_device *spi)
{
	drm_atomic_helper_shutdown(spi_get_drvdata(spi));
}

static struct spi_driver ili9488_spi_driver = {
	.driver = {
		.name = "ili9488",
		.owner = THIS_MODULE,
		.of_match_table = ili9488_of_match,
	},
	.id_table = ili9488_id,
	.probe = ili9488_probe,
	.remove = ili9488_remove,
	.shutdown = ili9488_shutdown,
};
module_spi_driver(ili9488_spi_driver);

MODULE_DESCRIPTION("Ilitek ILI9488 DRM driver");
MODULE_AUTHOR("Kamlesh Gurudasani <kamlesh.gurudasani@gmail.com>");
MODULE_LICENSE("GPL");
------------------------ili9488.c---------------------------------------------------------



2. Modify the /linux_5.10/drivers/gpu/drm/tiny/Kconfig file and add at the bottom
----------------------------------------Kconfig-----------------------------------------
config TINYDRM_ILI9488
	tristate "DRM support for ILI9488 display panels"
	depends on DRM && SPI
	select DRM_KMS_HELPER
	select DRM_KMS_CMA_HELPER
	select DRM_MIPI_DBI
	select BACKLIGHT_CLASS_DEVICE
	help
	  DRM driver for the following Ilitek ILI9488 panel
	  If M is selected the module will be called ili9488.
-----------------------------------------Kconfig----------------------------------------


3. Modify the /linux_5.10/drivers/gpu/drm/tiny/Makefile file and add at the bottom
----------------------------------------Makefile-----------------------------------------
obj-$(CONFIG_TINYDRM_ILI9488)		+= ili9488.o
----------------------------------------Makefile-----------------------------------------


4. Modify the /build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig file and add
------------------------------cvitek_cv1800b_milkv_duo_sd_defconfig---------------------------------------------------
CONFIG_TTY=y
CONFIG_VT=y
CONFIG_CONSOLE_TRANSLATIONS=y
CONFIG_VT_CONSOLE=y
CONFIG_HW_CONSOLE=y
CONFIG_VT_HW_CONSOLE_BINDING=y
CONFIG_FB_CMDLINE=y
CONFIG_FB_NOTIFY=y
CONFIG_FONT_SUPPORT=y
CONFIG_FONTS=y
CONFIG_FONT_8x16=y
CONFIG_VGA_CONSOLE=y
CONFIG_DUMMY_CONSOLE=y
CONFIG_DUMMY_CONSOLE_COLUMNS=80
CONFIG_DUMMY_CONSOLE_ROWS=25
CONFIG_FRAMEBUFFER_CONSOLE=y
CONFIG_FRAMEBUFFER_CONSOLE_DETECT_PRIMARY=y

CONFIG_DRM=y
CONFIG_DRM_MIPI_DBI=yCONFIG_DRM_KMS_HELPER=y
CONFIG_DRM_KMS_FB_HELPER=y
CONFIG_DRM_FBDEV_EMULATION=y
CONFIG_DRM_FBDEV_OVERALLOC=100
CONFIG_DRM_GEM_CMA_HELPER=y
CONFIG_DRM_KMS_CMA_HELPER=y
CONFIG_DRM_PANEL=y
CONFIG_DRM_BRIDGE=y
CONFIG_DRM_PANEL_BRIDGE=y
CONFIG_DRM_PANEL_ORIENTATION_QUIRKS=y
CONFIG_TINYDRM_ILI9488=y
-------------------------------cvitek_cv1800b_milkv_duo_sd_defconfig--------------------------------------------------


5. Modify /build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts file
------------------------------cv1800b_milkv_duo_sd.dts---------------------------------------------------
&spi2 {
	status = "okay";
	/delete-node/ spidev@0;
	display@0{
		compatible = "eastrising,er-tft035-6";  //Match the driver name. If using st7789, change it to sitronix,st7789v_240x320
		reg = <0>;
		status = "okay";
		spi-max-frequency = <40000000>;
		dc-gpios = <&porta 24 GPIO_ACTIVE_HIGH>;
		reset-gpios = <&porta 23 GPIO_ACTIVE_HIGH>;
		rotation = <180>;
	};
};
-------------------------------cv1800b_milkv_duo_sd.dts--------------------------------------------------



The above is the file modification part for ili9488.
 For the wiring part, just refer to the introduction of fbtft driver.
 ili9488 needs to be connected to BL, and VCC line can be used.

Note: The new version of sdk does not need to modify /u-boot-2021.10/board/cvitek/cv180x/board.c, it has been configured


Provide st7789 driver
--------------------------------st7789-------------------------------------------------
// SPDX-License-Identifier: GPL-2.0+
/*
 * DRM driver for display panels connected to a Sitronix ST7789V
 * display controller in SPI mode.
 * Copyright 2021 CNflysky
 */
#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_gem_cma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_mipi_dbi.h>
#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/dma-buf.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/spi/spi.h>
#include <video/mipi_display.h>

#define ST7789V_MY BIT(7)
#define ST7789V_MX BIT(6)
#define ST7789V_MV BIT(5)
#define ST7789V_RGB BIT(3)

static void st7789v_pipe_enable(struct drm_simple_display_pipe *pipe,
                                struct drm_crtc_state *crtc_state,
                                struct drm_plane_state *plane_state) {
  struct mipi_dbi_dev *dbidev = drm_to_mipi_dbi_dev(pipe->crtc.dev);
  struct mipi_dbi *dbi = &dbidev->dbi;
  int ret,idx;
  uint8_t addr_mode;
  if (!drm_dev_enter(pipe->crtc.dev, &idx)) return;
  DRM_DEBUG_KMS("\n");
  ret = mipi_dbi_poweron_reset(dbidev);
  if (ret) drm_dev_exit(idx);
  // init seq begin
  mipi_dbi_command(dbi, MIPI_DCS_SET_PIXEL_FORMAT, 0x05);
  // Porch setting
  mipi_dbi_command(dbi, 0xB2, 0x0C, 0x0C, 0x00, 0x33, 0x33);
  // Gate control
  mipi_dbi_command(dbi, 0xB7, 0x35);
  // VCOM settings
  mipi_dbi_command(dbi, 0xBB, 0x19);
  // LCM Control
  mipi_dbi_command(dbi, 0xC0, 0x2C);
  // VDV and VRH Command Enable
  mipi_dbi_command(dbi, 0xC2, 0x01);
  // VRH Set
  mipi_dbi_command(dbi, 0xC3, 0x12);
  // VDV Set
  mipi_dbi_command(dbi, 0xC4, 0x20);
  // Frame Rate Control in Normal Mode
  mipi_dbi_command(dbi, 0xC6, 0x0F);
  // Power control 1
  mipi_dbi_command(dbi, 0xD0, 0xA4, 0xA1);
  // Positive Voltage Gamma Control
  mipi_dbi_command(dbi, 0xE0, 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54,
                   0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23);
  // Negative Voltage Gamma Control
  mipi_dbi_command(dbi, 0xE1, 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44,
                   0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23);
  mipi_dbi_command(dbi, MIPI_DCS_ENTER_INVERT_MODE);
  switch (dbidev->rotation) {
    default:
      addr_mode = 0;
      break;
    case 90:
      addr_mode = ST7789V_MX | ST7789V_MV;
      break;
    case 180:
      addr_mode = ST7789V_MX | ST7789V_MY;
      break;
    case 270:
      addr_mode = ST7789V_MY | ST7789V_MV;
      break;
  }
  //if colors were inverted(blue as red),then uncomment the following line:
  //addr_mode |= ST7789V_RGB;
  mipi_dbi_command(dbi, MIPI_DCS_SET_ADDRESS_MODE, addr_mode);
  mipi_dbi_command(dbi, MIPI_DCS_EXIT_SLEEP_MODE);
  mipi_dbi_command(dbi, MIPI_DCS_SET_DISPLAY_ON);
  msleep(20);
  // init seq end
  mipi_dbi_enable_flush(dbidev, crtc_state, plane_state);
}

static const struct drm_simple_display_pipe_funcs st7789v_pipe_funcs = {
    .enable = st7789v_pipe_enable,
    .disable = mipi_dbi_pipe_disable,
    .update = mipi_dbi_pipe_update,
    .prepare_fb = drm_gem_fb_simple_display_pipe_prepare_fb,
};

static const struct drm_display_mode st7789v_mode = {
    DRM_SIMPLE_MODE(240, 320, 26, 26),
};

DEFINE_DRM_GEM_CMA_FOPS(st7789v_fops);

static struct drm_driver st7789v_driver = {
    .driver_features = DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
    .fops = &st7789v_fops,
    DRM_GEM_CMA_DRIVER_OPS_VMAP,
    .debugfs_init = mipi_dbi_debugfs_init,
    .name = "st7789v",
    .desc = "Sitronix ST7789V",
    .date = "20211022",
    .major = 1,
    .minor = 0,
};

static const struct of_device_id st7789v_of_match[] = {
    {.compatible = "sitronix,st7789v_240x320"},
    {},
};

MODULE_DEVICE_TABLE(of, st7789v_of_match);

static const struct spi_device_id st7789v_id[] = {
    {"st7789v_240x320", 0},
    {},
};

MODULE_DEVICE_TABLE(spi, st7789v_id);

static int st7789v_probe(struct spi_device *spi) {
  struct device *dev = &spi->dev;
  struct drm_device *drm;
  struct mipi_dbi *dbi;
  struct gpio_desc *dc;
  uint32_t rotation = 0;
  int ret;
  struct mipi_dbi_dev *dbidev =
      devm_drm_dev_alloc(dev, &st7789v_driver, struct mipi_dbi_dev, drm);
  if (IS_ERR(dbidev)) return PTR_ERR(dbidev);
  dbi = &dbidev->dbi;
  drm = &dbidev->drm;
  drm_mode_config_init(drm);
  dbi->reset = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_HIGH);
  if (IS_ERR(dbi->reset)) {
    DRM_DEV_ERROR(dev, "Failed to get gpio 'reset'\n");
    return PTR_ERR(dbi->reset);
  }
  dc = devm_gpiod_get_optional(dev, "dc", GPIOD_OUT_LOW);
  if (IS_ERR(dc)) {
    DRM_DEV_ERROR(dev, "Failed to get gpio 'dc'\n");
    return PTR_ERR(dc);
  }
  dbidev->backlight = devm_of_find_backlight(dev);
  if (IS_ERR(dbidev->backlight)) return PTR_ERR(dbidev->backlight);
  device_property_read_u32(dev, "rotation", &rotation);
  ret = mipi_dbi_spi_init(spi, dbi, dc);
  spi->mode = SPI_MODE_3;
  if (ret) return ret;
  ret = mipi_dbi_dev_init(dbidev, &st7789v_pipe_funcs, &st7789v_mode, rotation);
  if (ret) return ret;
  drm_mode_config_reset(drm);
  ret = drm_dev_register(drm, 0);
  if (ret) return ret;
  spi_set_drvdata(spi, drm);
  drm_fbdev_generic_setup(drm, 0);
  return 0;
}

static int st7789v_remove(struct spi_device *spi) {
  struct drm_device *drm = spi_get_drvdata(spi);
  drm_dev_unplug(drm);
  drm_atomic_helper_shutdown(drm);
  return 0;
}

static void st7789v_shutdown(struct spi_device *spi) {
  drm_atomic_helper_shutdown(spi_get_drvdata(spi));
}

static struct spi_driver st7789v_spi_driver = {
    .driver =
        {
            .name = "st7789v",
            .of_match_table = st7789v_of_match,
        },
    .id_table = st7789v_id,
    .probe = st7789v_probe,
    .remove = st7789v_remove,
    .shutdown = st7789v_shutdown,
};

module_spi_driver(st7789v_spi_driver);
MODULE_DESCRIPTION("Sitronix ST7789V DRM driver");
MODULE_AUTHOR("CNflysky <cnflysky@qq.com>");
MODULE_LICENSE("GPL");
--------------------------------st7789-------------------------------------------------


Rendering
https://community.milkv.io/uploads/default/optimized/1X/2790172eff8b3b90e8ea24d0cf72d3a7bfc4bc08_2_375x500.jpeg

In fact, the Linux kernel source code already supports many screen drivers.
 The existing drivers in the tiny folder can be used directly. I have also tried ili9341.

Finally, I would like to thank the original author of the driver.
 I only made a small modification. Enjoy!



--------------------------------------------------------------------------------------------------------------

PS: see coments https://community.milkv.io/t/milk-v-duo-tinydrm-ili9488-st7789/1223


