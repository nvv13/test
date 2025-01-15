
[источник](https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668)


-----
********************************************************************************************************
~~~

【i2s】Milk-V Duo adds speaker——max98357a decoder driver

LangZhao   4 Sep 2023

Previous chapter: https://community.milkv.io/t/i2s-milk-v-duo-speaker-i2s2/666


3. References
3.1 Articles
 https://blog.csdn.net/pingis58/article/details/109235204
 https://github.com/beagleboard/bb.org-overlays/issues/39
 https://cloud.tencent.com/developer/article/2225253

3.2 Manual
 https://community.milkv.io/uploads/default/original/1X/689e763aa9017ce31ee8a9c4ebfb6d287e8ee709.jpeg

max98357a uses i2s interface, we only need to use the following pins:
LRC: left and right channel selection
BCLK: sampling signal
DIN: audio data input
Vin: power supply
GND: ground

gain and sd-mode are also defined in the driver, you need to add the interface to control it yourself, and I will talk about it later.

4. Driver path
max98357a driver:
duo-buildroot-sdk-develop\linux_5.10\sound\soc\codecs\max98357a.c

duo-buildroot-sdk-develop\linux_5.10\sound\soc\codecs\Makefile
-----------------------------------------------Makefile--------------------------------------------------------
snd-soc-max98357a-objs := max98357a.o

obj-$(CONFIG_SND_SOC_MAX98357A)	+= snd-soc-max98357a.o
-----------------------------------------------Makefile--------------------------------------------------------


duo-buildroot-sdk-develop\linux_5.10\sound\soc\Makefile
-----------------------------------------------Makefile--------------------------------------------------------
obj-$(CONFIG_SND_SOC)	+= snd-soc-core.o
obj-$(CONFIG_SND_SOC)	+= codecs/
-----------------------------------------------Makefile--------------------------------------------------------

duo-buildroot-sdk-develop\linux_5.10\sound\Makefile
-----------------------------------------------Makefile--------------------------------------------------------
obj-$(CONFIG_SOUND) += soundcore.o
obj-$(CONFIG_SND) += core/ i2c/ drivers/ isa/ pci/ ppc/ arm/ sh/ synth/ usb/ \
	firewire/ sparc/ spi/ parisc/ pcmcia/ mips/ soc/ atmel/ hda/ x86/ xen/
-----------------------------------------------Makefile--------------------------------------------------------

duo-buildroot-sdk-develop\linux_5.10\Makefile
-----------------------------------------------Makefile--------------------------------------------------------
drivers-y	:= drivers/ sound/
-----------------------------------------------Makefile--------------------------------------------------------

So the config that needs to be opened is as follows
-------------------------------config------------------------------------------------------------------------
CONFIG_SND_SOC=y
CONFIG_SOUND=y
CONFIG_SND=y
-------------------------------config------------------------------------------------------------------------

5. Add codec driver - max98357a
5.1 config
 As introduced in the previous chapter, the configuration under the codec path,
 so loading the codec driver requires configuring config.
-------------------------------config------------------------------------------------------------------------
CONFIG_SND_SOC_MAX98357A=y
-------------------------------config------------------------------------------------------------------------

5.2 dtsi
duo-buildroot-sdk-develop\linux_5.10\sound\soc\codecs\max98357a.c
-------------------------------------------------------------------------------------------------------
#ifdef CONFIG_OF
static const struct of_device_id max98357a_device_id[] = {
	{ .compatible = "maxim,max98357a" },
	{ .compatible = "maxim,max98360a" },
	{}
};
MODULE_DEVICE_TABLE(of, max98357a_device_id);
#endif
-------------------------------------------------------------------------------------------------------

This node needs to be matched in dtsi, and the following configuration
 needs to be added so that the codec can be loaded normally.
-------------------------------------------------------------------------------------------------------
/* codec */
	max98357a: max98357a {
		#sound-dai-cells = <0>;
		compatible = "maxim,max98357a";
		status = "okay";
		/*sdmode-gpios = <&gpio1 14 0>;*/ 
		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
	};
-------------------------------------------------------------------------------------------------------

5.3 dai_driver
The dai_driver here will be used in the machine driver later, so pay attention to the name.
-------------------------------------------------------------------------------------------------------
static struct snd_soc_dai_driver max98357a_dai_driver = {
	.name = "HiFi",
	.playback = {
		.stream_name	= "HiFi Playback",
		.formats	= SNDRV_PCM_FMTBIT_S16 |
					SNDRV_PCM_FMTBIT_S24 |
					SNDRV_PCM_FMTBIT_S32,
		.rates		= SNDRV_PCM_RATE_8000 |
					SNDRV_PCM_RATE_16000 |
					SNDRV_PCM_RATE_32000 |
					SNDRV_PCM_RATE_44100 |
					SNDRV_PCM_RATE_48000 |
					SNDRV_PCM_RATE_88200 |
					SNDRV_PCM_RATE_96000,
		.rate_min	= 8000,
		.rate_max	= 96000,
		.channels_min	= 1,
		.channels_max	= 2,
	},
	.ops    = &max98357a_dai_ops,
};
-------------------------------------------------------------------------------------------------------


6. Check platform/cpu
duo-buildroot-sdk-develop\linux_5.10\sound\soc\cvitek\Makefile

i2s2 is used, please note that the i2s2 node needs to be opened.

Makefile
-------------------------------------------------------------------------------------------------------
# Cvitek Platform Support
obj-$(CONFIG_SND_CV1835_I2S) += cv1835_i2s.o
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
i2s2: i2s@04120000 {
		compatible = "cvitek,cv1835-i2s";
		reg = <0x0 0x04120000 0x0 0x2000>;
		clocks = <&i2s_mclk 0>;
		clock-names = "i2sclk";
		dev-id = <2>;
		#sound-dai-cells = <0>;
		dmas = <&dmac 6 1 1 /* read channel */
			&dmac 1 1 1>; /* write channel */
		dma-names = "rx", "tx";
		capability = "txrx";
		mclk_out = "false";
	};
-------------------------------------------------------------------------------------------------------


VII. Add machin driver
7.1 Imitate 182x, 183x
The cpu_dai_name defined here is deprecated because the audio dapm interface has changed.
If the following configuration is still used, it will fail to compile.
-------------------------------------------------------------------------------------------------------
/* machine -- sound card*/
#ifdef CV1835_EXT_CARD_3_EN
	/* sound_ext3 use external codec */
	sound_ext3 {
		compatible = "cvitek,cv1835-max98357a";
		cvi,model = "CV1835";
		cvi,mode = "I2S";
		cvi,fmt = "IBNF";
		cvi,card_name = "cv1835_external_card_max98357a";
		cvi,slot_no=<2>;

		dai@0 {
			cvi,dai_name = "cv1835-i2s-2";
			cvi,stream_name = "HiFi";
			cvi,cpu_dai_name = "4120000.i2s";
			cvi,codec_dai_name = "HiFi";
			cvi,platform_name = "4120000.i2s";
			cvi,codec_name = "MX98357A:00";
			cvi,role = "master";
		};
	};
#endif
-------------------------------------------------------------------------------------------------------


7.2 Redefine dai_link
-------------------------------------------------------------------------------------------------------
SND_SOC_DAILINK_DEFS(dailink_max98357a,
	DAILINK_COMP_ARRAY(COMP_CPU("4120000.i2s")),
	DAILINK_COMP_ARRAY(COMP_CODEC("max98357a", "HiFi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("4120000.i2s")));

static struct snd_soc_dai_link cv1835_max98357a_dai[] = {
	[DPCM_AUDIO_SPKR] = {
		.name = "max98357a",
		.stream_name = "max98357a i2s",	//"cv1835-max98357a",
		.init = cv1835_max98357a_asoc_init,
		.ops = &cv1835_max98357a_ops,
		.dai_fmt = SND_SOC_DAIFMT_I2S
		| SND_SOC_DAIFMT_IB_NF
		| SND_SOC_DAIFMT_CBS_CFS,
		SND_SOC_DAILINK_REG(dailink_max98357a),

		// Interface Abandonment
		// .cpu_dai_name= "4120000.i2s",
		// .codec_dai_name = "HiFi",
		// .codec_name = "maxim,max98357a",
		// .platform_name= "4120000.i2s",
	},
};
-------------------------------------------------------------------------------------------------------

7.3 Add driver
Add driver code
duo-buildroot-sdk-develop\linux_5.10\sound\soc\cvitek\cv1835_max98357a.c

Compile first to see if the driver can be added successfully
-------------------------------------------------------------------------------------------------------
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Machine driver for EVAL-ADAU1372 on CVITEK CV1835
 *
 * Copyright 2019 CVITEK
 *
 * Author: EthanChen
 *
 */

#include <linux/module.h>
#include <linux/device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <linux/io.h>
#include <linux/proc_fs.h>

// // u16 tdm_slot_no = 2;


struct card_private {
	int tmp;	//save sth.
	// struct snd_soc_jack headset;
	// struct list_head hdmi_pcm_list;
	// struct snd_soc_jack hdmi[3];
};

enum {
	DPCM_AUDIO_SPKR = 0,
};

static const struct snd_soc_dapm_widget cv1835_max98357a_dapm_widgets[] = {
	SND_SOC_DAPM_SPK("SPKR", NULL),		// diff "Speaker" in max98357a
};

static const struct snd_soc_dapm_route cv1835_max98357a_dapm_routes[] = {
	{"SPKR", NULL, "Speaker"},
};

static int cv1835_max98357a_hw_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *params)
{
	return 0;
}

static int cv1835_max98357a_asoc_init(struct snd_soc_pcm_runtime *rtd)
{
	// dual with private data


	// struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	// struct snd_soc_dai *codec_dai = rtd->codec_dai;
	// u32 ret;

	// /* Only need to set slot number while mode is TDM/PDM, otherwise default slot number is 2 */
	// if (tdm_slot_no != 2) {
	// 	ret = snd_soc_dai_set_tdm_slot(codec_dai, 0x0F, 0x0F, tdm_slot_no, 32);

	// 	if (ret < 0)
	// 		return ret;

	// 	ret = snd_soc_dai_set_tdm_slot(cpu_dai, 0x0F, 0x0F, tdm_slot_no, 32);

	// 	if (ret < 0)
	// 		return ret;
	// }

	return 0;
}

static struct snd_soc_ops cv1835_max98357a_ops = {
	.hw_params = cv1835_max98357a_hw_params,
};

SND_SOC_DAILINK_DEFS(dailink_max98357a,
	DAILINK_COMP_ARRAY(COMP_CPU("4120000.i2s")),
	DAILINK_COMP_ARRAY(COMP_CODEC("max98357a", "HiFi")),
	DAILINK_COMP_ARRAY(COMP_PLATFORM("4120000.i2s")));

static struct snd_soc_dai_link cv1835_max98357a_dai[] = {
	[DPCM_AUDIO_SPKR] = {
		.name = "max98357a",
		.stream_name = "max98357a i2s",	//"cv1835-max98357a",
		.init = cv1835_max98357a_asoc_init,
		.ops = &cv1835_max98357a_ops,
		.dai_fmt = SND_SOC_DAIFMT_I2S
		| SND_SOC_DAIFMT_IB_NF
		| SND_SOC_DAIFMT_CBS_CFS,
		SND_SOC_DAILINK_REG(dailink_max98357a),
	},
};


static struct snd_soc_card cv1835_max98357a = {
	.name = "cv1835 max98357a",	// card name
	.owner = THIS_MODULE,
	.dai_link = cv1835_max98357a_dai,
	.num_links = ARRAY_SIZE(cv1835_max98357a_dai),

	// control may don't have
	// .controls = skylake_controls,
	// .num_controls = ARRAY_SIZE(skylake_controls),

	.dapm_widgets		= cv1835_max98357a_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(cv1835_max98357a_dapm_widgets),

	.dapm_routes		= cv1835_max98357a_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(cv1835_max98357a_dapm_routes),

	// .fully_routed = true,
	// .late_probe = skylake_card_late_probe,
};

static const struct of_device_id cvi_audio_match_ids[] = {
	{
		.compatible = "cvitek,cv1835-max98357a",
		//.data = (void *) &cv1835_max98357a_dai,
	},
	{},
};
MODULE_DEVICE_TABLE(of, cvi_audio_match_ids);


static int cv1835_max98357a_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct snd_soc_card *card = &cv1835_max98357a;

	struct card_private *ctx;		// create private data for card
	int ret;

	// struct device_node *np = pdev->dev.of_node, *dai;

	// dev_info("snd card name = %s",card->name);

	ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	card->dev = &pdev->dev;
	snd_soc_card_set_drvdata(card, ctx);	//save card info to snd_card

	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret) {
		dev_err(&pdev->dev, "snd_soc_register_card failed (%d)\n",
			ret);
		return ret;
	}

	return 0;
}

static struct platform_driver cv1835_max98357a_driver = {
	.driver = {
		.name = "cv1835-max98357a",
		.pm = &snd_soc_pm_ops,
		.of_match_table = cvi_audio_match_ids,
	},
	.probe = cv1835_max98357a_probe,
};

module_platform_driver(cv1835_max98357a_driver);

MODULE_AUTHOR("EthanChen");
MODULE_DESCRIPTION("ALSA SoC cv1835 max98357a driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:cv1835-max98357a");
-------------------------------------------------------------------------------------------------------

7.4 Add Kconfig
-------------------------------------------------------------------------------------------------------
config SND_SOC_CV1835_MAX98357A
	tristate "Support for the max98357a card"
	help
	  max98357a codec enable.
-------------------------------------------------------------------------------------------------------

7.5 Adding a makefile
-------------------------------------------------------------------------------------------------------
obj-$(CONFIG_SND_SOC_CV1835_MAX98357A) += cv1800_max98357a.o
-------------------------------------------------------------------------------------------------------


7.6 Open config

CONFIG_SND_SOC_CV1835_MAX98357A to load the machine driver.
CONFIG_SND_SOC_MAX98357A to load the codec driver.
CONFIG_SND_CV1835_I2S to load the cpu/platform driver.

CONFIG_SND_PROC_FS to open the proc interface implementation for the sound card information in the
 /proc/asound directory.
-------------------------------------------------------------------------------------------------------
# sound -- max98357a
CONFIG_SND_SOC_CV1835_MAX98357A=y
CONFIG_SND_SOC_MAX98357A=y

CONFIG_SND_PROC_FS=y

CONFIG_SOUND=y
CONFIG_SND=y
CONFIG_SND_SOC=y
CONFIG_SND_CV1835_I2S=y
CONFIG_CV1835_I2S_SUBSYS=y
CONFIG_SND_SOC_CV1835_CONCURRENT_I2S=y
-------------------------------------------------------------------------------------------------------


8. Problems
8.1 /proc/asound directory is not generated
CONFIG_SND_PROC_FS

Check the final generated file of config
Path: duo-buildroot-sdk\linux_5.10\build\cv1800b_milkv_duo_sd.config

makefile
-------------------------------------------------------------------------------------------------------
ifneq ($(CONFIG_SND_PROC_FS),)
snd-y += info.o
snd-$(CONFIG_SND_OSSEMUL) += info_oss.o
endif
-------------------------------------------------------------------------------------------------------

These contents are stored in the asound directory
info.c
-------------------------------------------------------------------------------------------------------
int snd_info_check_reserved_words(const char *str)
{
	static const char * const reserved[] =
	{
		"version",
		"meminfo",
		"memdebug",
		"detect",
		"devices",
		"oss",
		"cards",
		"timers",
		"synth",
		"pcm",
		"seq",
		NULL
	};
	const char * const *xstr = reserved;

	while (*xstr) {
		if (!strcmp(*xstr, str))
			return 0;
		xstr++;
	}
	if (!strncmp(str, "card", 4))
		return 0;
	return 1;
}
-------------------------------------------------------------------------------------------------------


alsa source code
duo-buildroot-sdk-develop\linux_5.10\sound\core\sound.c

8.2 Use audio_debug to read adc, i2s0/2, the status is not open
Check dts, the status is ok

Check dts generated file
Path: duo-buildroot-sdk\u-boot-2021.10\build\cv1800b_milkv_duo_sd\arch\riscv\dts.cv1800b_milkv_duo_sd.dtb.dts.tmp

Since the implementation is not added to the driver, the displayed status is incorrect.
It is necessary to add information in dtsi and add processing in code.

8.3 mclk
May not need to be added, it depends on whether the chip supports it

duo-buildroot-sdk-develop\linux_5.10\sound\soc\cvitek\cv1835_i2s.c
Here you may need to open config and define mclk_out in dts
-------------------------------------------------------------------------------------------------------
device_property_read_string(&pdev->dev, "mclk_out", &mclk_out);

	if (!strcmp(mclk_out, "true"))
		dev->mclk_out = true;
	else
		dev->mclk_out = false;

	val = i2s_read_reg(dev->i2s_base, I2S_CLK_CTRL0);
	val &= ~(AUD_CLK_SOURCE_MASK);
	val &= ~(BCLK_OUT_FORCE_EN); /* blck_out output after transmission start */
#if defined(CONFIG_SND_SOC_CV1835_USE_AUDIO_PLL)
	if (dev->mclk_out == true)
		i2s_write_reg(dev->i2s_base, I2S_CLK_CTRL0, val | AUD_CLK_FROM_PLL | MCLK_OUT_EN | AUD_ENABLE);
	/* Turn aud_en on due to external codec might need MCLK to do register initialization */
	else
		i2s_write_reg(dev->i2s_base, I2S_CLK_CTRL0, val | AUD_CLK_FROM_PLL);
#else
	i2s_write_reg(dev->i2s_base, I2S_CLK_CTRL0, val | AUD_CLK_FROM_MCLK_IN);
#endif
-------------------------------------------------------------------------------------------------------

9. Results
9.1 Compilation
-------------------------------------------------------------------------------------------------------
CC      sound/soc/codecs/max98357a.o
CC      sound/soc/cvitek/cv1835_max98357a.o
-------------------------------------------------------------------------------------------------------

https://community.milkv.io/uploads/default/original/1X/53d3b4e9cf6d0af30302425e6a1ce9c95bbb30aa.png

Summary
This chapter introduces how to add the driver of max98357a and load it successfully.
Please read it together with the previous chapter.

-------------------------------------------------------------------------------------------------------




********************************************************************************************************




PS:


********************************************************************************************************


LangZhao
Sep 2023
 comments:

This git diff contains the lcd, audio mic and speaker.
-------------------------------------------------------------------------------------------------------
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts b/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
index 8f66e40d8..290169a33 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
@@ -8,7 +8,66 @@
 	snsr-reset = <&portc 8 GPIO_ACTIVE_LOW>, <&portc 8 GPIO_ACTIVE_LOW>, <&portc 8 GPIO_ACTIVE_LOW>;
 };
 
+&spi2 {
+	status = "okay";
+
+    /delete-node/ spidev@0;
+
+
+	st7789v: st7789v@0{
+		compatible = "sitronix,st7789v";
+		reg = <0>;
+		status = "okay";
+		spi-max-frequency = <48000000>;
+		spi-cpol;
+		spi-cpha;
+		rotate = <90>;
+		fps = <30>;
+		rgb;
+		buswidth = <8>;
+
+		//dc-gpios = <&port 21 GPIO_ACTIVE_HIGH>;	//DC
+		dc-gpios = <&porta 23 GPIO_ACTIVE_HIGH>;	//DC
+		reset-gpios = <&porta 15 GPIO_ACTIVE_HIGH>; //RES
+		led-gpios = <&porta 14 GPIO_ACTIVE_HIGH>; //BL
+
+		debug = <0x0>;
+	};
+
+};
+
+&i2s2 {
+	status = "okay";
+	#sound-dai-cells = <0>;
+};
+
+&i2s0 {
+	status = "okay";
+	#sound-dai-cells = <0>;
+};
+
+
+
 / {
 
+
+
+	/* codec */
+	max98357a: max98357a {
+		#sound-dai-cells = <0>;
+		compatible = "maxim,max98357a";
+		status = "okay";
+		/*sdmode-gpios = <&gpio1 14 0>;*/ 
+		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
+	};
+
+	/* machine -- sound card*/
+	/* #ifdef CV1835_EXT_CARD_3_EN */
+	/* sound_ext3 use external codec */
+	sound {
+		compatible = "cvitek,cv1835-max98357a";
+	};
+
+
 };
 
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig b/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
index 6c05a05df..f72a6d6ab 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
@@ -153,10 +153,10 @@ CONFIG_SND_HDA_PREALLOC_SIZE=1
 # CONFIG_SND_USB is not set
 CONFIG_SND_SOC=y
 CONFIG_SND_SOC_CV182XA_CV182XAADC=y
-CONFIG_SND_SOC_CV182XA_CV182XADAC=y
+# CONFIG_SND_SOC_CV182XA_CV182XADAC=y
 CONFIG_SND_CV1835_I2S=y
 CONFIG_SND_SOC_CV182XAADC=y
-CONFIG_SND_SOC_CV182XADAC=y
+# CONFIG_SND_SOC_CV182XADAC=y
 CONFIG_SND_SOC_CV1835_USE_AUDIO_PLL=y
 CONFIG_CV1835_I2S_SUBSYS=y
 CONFIG_USB=y
@@ -260,3 +260,88 @@ CONFIG_ADVISE_SYSCALLS=n
 CONFIG_SIGNALFD=n
 CONFIG_TIMERFD=n
 CONFIG_EPOLL=n
+
+# -----------------------------------------------------------
+# youkai add
+# CONFIG_IIO=y
+# CONFIG_BH1750=y
+# CONFIG_TCS3472=y
+# CONFIG_INV_MPU6050_IIO=y
+# CONFIG_INV_MPU6050_I2C=y
+# CONFIG_SENSORS_HMC5843=y
+# CONFIG_SENSORS_HMC5843_I2C=y
+
+# sensor bmp180
+# CONFIG_BMP280=y
+# CONFIG_BMP280_I2C=y
+
+# spi lcd -- st7735
+CONFIG_SPI=y
+CONFIG_SPI_MASTER=y
+CONFIG_SPI_DESIGNWARE=y
+CONFIG_SPI_DW_MMIO=y
+CONFIG_SPI_SPIDEV=y
+CONFIG_FB=y
+CONFIG_FB_TFT=y
+# CONFIG_FB_TFT_ST7735R=y
+CONFIG_FB_TFT_ST7789V=y
+
+# CONFIG_TINYDRM_ST7735R=y
+
+# lcd display kernel log
+CONFIG_TTY=y
+CONFIG_VT=y
+CONFIG_CONSOLE_TRANSLATIONS=y
+CONFIG_VT_CONSOLE=y
+CONFIG_HW_CONSOLE=y
+CONFIG_VT_HW_CONSOLE_BINDING=y
+CONFIG_FB=y
+CONFIG_FB_CMDLINE=y
+CONFIG_FB_NOTIFY=y
+CONFIG_FONT_SUPPORT=y
+CONFIG_FONTS=y
+CONFIG_FONT_8x16=y
+CONFIG_VGA_CONSOLE=y
+CONFIG_DUMMY_CONSOLE=y
+CONFIG_DUMMY_CONSOLE_COLUMNS=80
+CONFIG_DUMMY_CONSOLE_ROWS=25
+CONFIG_FRAMEBUFFER_CONSOLE=y
+CONFIG_FRAMEBUFFER_CONSOLE_DETECT_PRIMARY=y
+CONFIG_VGA_CONSOLE=y
+
+# # no compatible
+# CONFIG_TCS3472=y
+
+# sound -- max98357a
+CONFIG_SND_SOC_CV1835_MAX98357A=y
+CONFIG_SND_SOC_MAX98357A=y
+
+CONFIG_SND_PROC_FS=y
+
+CONFIG_SOUND=y
+CONFIG_SND=y
+CONFIG_SND_SOC=y
+CONFIG_SND_CV1835_I2S=y
+CONFIG_CV1835_I2S_SUBSYS=y
+CONFIG_SND_SOC_CV1835_CONCURRENT_I2S=y
+# CONFIG_SND_SOC_ADAU1372_SPI is not set
+# CONFIG_SND_SOC_ADAU1372=y
+# CONFIG_SND_SOC_ADAU_UTILS=y
+# CONFIG_SND_SOC_ADAU1372_I2C=y
+# CONFIG_SND_SOC_CV1835_ADAU1372=y
+# CONFIG_SND_SOC_CV1835_USE_AUDIO_PLL=y
+# CONFIG_SND_SOC_CV1835_CV1835PDM=y
+# CONFIG_SND_SOC_CV1835PDM=y
+
+# sound -- adc mic
+# CONFIG_SND_SOC_CV180X_CV180XADC=y
+# CONFIG_SND_SOC_CV1800ADC=y
+# CONFIG_SND_SOC_CV182XA_CV182XAADC=y
+# CONFIG_SND_SOC_CV182XAADC=y
+# CONFIG_SND_SOC_CV1835_CV1835ADC_V2=y
+# CONFIG_SND_SOC_CV1835_CV1835ADC=y
+# CONFIG_SND_SOC_CV1835ADC=y
+# CONFIG_SND_SOC_CV1835_CV1835DAC=y
+# CONFIG_SND_SOC_CV1835DAC=y
+
+# -----------------------------------------------------------
\ No newline at end of file
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c b/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c
index 25c95da75..817728b31 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c
@@ -15,18 +15,36 @@ int cvi_board_init(void)
 	PINMUX_CONFIG(SPINOR_CS_X, XGPIOA_24);
 	PINMUX_CONFIG(IIC0_SDA, XGPIOA_29);
 	PINMUX_CONFIG(IIC0_SCL, XGPIOA_28);
-	// sd1
-	PINMUX_CONFIG(SD1_D3, PWR_GPIO_18);
-	PINMUX_CONFIG(SD1_D2, PWR_GPIO_19);
-	PINMUX_CONFIG(SD1_D1, PWR_GPIO_20);
-	PINMUX_CONFIG(SD1_D0, PWR_GPIO_21);
-	PINMUX_CONFIG(SD1_CMD, PWR_GPIO_22);
-	PINMUX_CONFIG(SD1_CLK, PWR_GPIO_23);
+	// // sd1
+	// PINMUX_CONFIG(SD1_D3, PWR_GPIO_18);
+	// PINMUX_CONFIG(SD1_D2, PWR_GPIO_19);
+	// PINMUX_CONFIG(SD1_D1, PWR_GPIO_20);
+	// PINMUX_CONFIG(SD1_D0, PWR_GPIO_21);
+	// PINMUX_CONFIG(SD1_CMD, PWR_GPIO_22);
+	// PINMUX_CONFIG(SD1_CLK, PWR_GPIO_23);
 
 	//default to gpio
 	PINMUX_CONFIG(PAD_MIPIRX1P, XGPIOC_9);
 	PINMUX_CONFIG(PAD_MIPIRX0N, XGPIOC_10);
 	PINMUX_CONFIG(USB_VBUS_DET, XGPIOB_6);
 	PINMUX_CONFIG(PWR_SEQ2, PWR_GPIO_4);
+
+	//spi2 -- st7735
+	// PINMUX_CONFIG(SD1_D3, SPI2_CS_X);		//CS
+	// PINMUX_CONFIG(SD1_D0, SPI2_SDI);			
+	// PINMUX_CONFIG(SD1_CMD, SPI2_SDO);		//SDA
+	// PINMUX_CONFIG(SD1_CLK, SPI2_SCK);		//SCL
+	pinmux_config(PINMUX_SPI2);
+	//PINMUX_CONFIG(SD1_D0, PWR_GPIO_21);		//DC
+	PINMUX_CONFIG(SPINOR_MISO, XGPIOA_23);		//DC
+	PINMUX_CONFIG(SD0_PWR_EN, XGPIOA_14);	//BL
+	PINMUX_CONFIG(SPK_EN, XGPIOA_15);		//RES
+
+	// I2s -- max98357a
+	PINMUX_CONFIG(PAD_ETH_TXP, IIS2_LRCK);
+	PINMUX_CONFIG(PAD_ETH_TXM, IIS2_BCLK);
+	PINMUX_CONFIG(PAD_ETH_RXP, IIS2_DO);
+	PINMUX_CONFIG(PAD_ETH_RXM, IIS2_DI);
+
 	return 0;
 }
diff --git a/build/boards/default/dts/cv180x/cv180x_asic_qfn.dtsi b/build/boards/default/dts/cv180x/cv180x_asic_qfn.dtsi
index 4c5b10dfa..cde34eeca 100644
--- a/build/boards/default/dts/cv180x/cv180x_asic_qfn.dtsi
+++ b/build/boards/default/dts/cv180x/cv180x_asic_qfn.dtsi
@@ -53,7 +53,7 @@
 / {
 	/delete-node/ wifi-sd@4320000;
 	/delete-node/ i2s@04110000;
-	/delete-node/ i2s@04120000;
+	/* /delete-node/ i2s@04120000; */
 	/delete-node/ sound_ext1;
 	/delete-node/ sound_ext2;
 	/delete-node/ sound_PDM;
diff --git a/build/boards/default/dts/cv180x/cv180x_base.dtsi b/build/boards/default/dts/cv180x/cv180x_base.dtsi
index fcc07d2ff..b05f088a8 100644
--- a/build/boards/default/dts/cv180x/cv180x_base.dtsi
+++ b/build/boards/default/dts/cv180x/cv180x_base.dtsi
@@ -232,6 +232,7 @@
 		clocks = <&clk CV180X_CLK_SPI>;
 		#address-cells = <1>;
 		#size-cells = <0>;
+		bias-pull-up;	//youkai add
 	};
 
 	spi3:spi3@041B0000 {
diff --git a/linux_5.10/drivers/staging/fbtft/fb_st7789v.c b/linux_5.10/drivers/staging/fbtft/fb_st7789v.c
index 3a280cc18..58f5752b5 100644
--- a/linux_5.10/drivers/staging/fbtft/fb_st7789v.c
+++ b/linux_5.10/drivers/staging/fbtft/fb_st7789v.c
@@ -80,67 +80,112 @@ enum st7789v_command {
  *
  * Return: 0 on success, < 0 if error occurred.
  */
-static int init_display(struct fbtft_par *par)
-{
-	/* turn off sleep mode */
-	write_reg(par, MIPI_DCS_EXIT_SLEEP_MODE);
-	mdelay(120);
+// static int init_display(struct fbtft_par *par)
+// {
+// 	/* turn off sleep mode */
+// 	write_reg(par, MIPI_DCS_EXIT_SLEEP_MODE);
+// 	mdelay(120);
 
-	/* set pixel format to RGB-565 */
-	write_reg(par, MIPI_DCS_SET_PIXEL_FORMAT, MIPI_DCS_PIXEL_FMT_16BIT);
-	if (HSD20_IPS)
-		write_reg(par, PORCTRL, 0x05, 0x05, 0x00, 0x33, 0x33);
+// 	/* set pixel format to RGB-565 */
+// 	write_reg(par, MIPI_DCS_SET_PIXEL_FORMAT, MIPI_DCS_PIXEL_FMT_16BIT);
+// 	if (HSD20_IPS)
+// 		write_reg(par, PORCTRL, 0x05, 0x05, 0x00, 0x33, 0x33);
 
-	else
-		write_reg(par, PORCTRL, 0x08, 0x08, 0x00, 0x22, 0x22);
+// 	else
+// 		write_reg(par, PORCTRL, 0x08, 0x08, 0x00, 0x22, 0x22);
 
-	/*
-	 * VGH = 13.26V
-	 * VGL = -10.43V
-	 */
-	if (HSD20_IPS)
-		write_reg(par, GCTRL, 0x75);
-	else
-		write_reg(par, GCTRL, 0x35);
+// 	/*
+// 	 * VGH = 13.26V
+// 	 * VGL = -10.43V
+// 	 */
+// 	if (HSD20_IPS)
+// 		write_reg(par, GCTRL, 0x75);
+// 	else
+// 		write_reg(par, GCTRL, 0x35);
 
-	/*
-	 * VDV and VRH register values come from command write
-	 * (instead of NVM)
-	 */
-	write_reg(par, VDVVRHEN, 0x01, 0xFF);
+// 	/*
+// 	 * VDV and VRH register values come from command write
+// 	 * (instead of NVM)
+// 	 */
+// 	write_reg(par, VDVVRHEN, 0x01, 0xFF);
 
-	/*
-	 * VAP =  4.1V + (VCOM + VCOM offset + 0.5 * VDV)
-	 * VAN = -4.1V + (VCOM + VCOM offset + 0.5 * VDV)
-	 */
-	if (HSD20_IPS)
-		write_reg(par, VRHS, 0x13);
-	else
-		write_reg(par, VRHS, 0x0B);
+// 	/*
+// 	 * VAP =  4.1V + (VCOM + VCOM offset + 0.5 * VDV)
+// 	 * VAN = -4.1V + (VCOM + VCOM offset + 0.5 * VDV)
+// 	 */
+// 	if (HSD20_IPS)
+// 		write_reg(par, VRHS, 0x13);
+// 	else
+// 		write_reg(par, VRHS, 0x0B);
 
-	/* VDV = 0V */
-	write_reg(par, VDVS, 0x20);
+// 	/* VDV = 0V */
+// 	write_reg(par, VDVS, 0x20);
 
-	/* VCOM = 0.9V */
-	if (HSD20_IPS)
-		write_reg(par, VCOMS, 0x22);
-	else
-		write_reg(par, VCOMS, 0x20);
+// 	/* VCOM = 0.9V */
+// 	if (HSD20_IPS)
+// 		write_reg(par, VCOMS, 0x22);
+// 	else
+// 		write_reg(par, VCOMS, 0x20);
 
-	/* VCOM offset = 0V */
-	write_reg(par, VCMOFSET, 0x20);
+// 	/* VCOM offset = 0V */
+// 	write_reg(par, VCMOFSET, 0x20);
 
-	/*
-	 * AVDD = 6.8V
-	 * AVCL = -4.8V
-	 * VDS = 2.3V
-	 */
-	write_reg(par, PWCTRL1, 0xA4, 0xA1);
+// 	/*
+// 	 * AVDD = 6.8V
+// 	 * AVCL = -4.8V
+// 	 * VDS = 2.3V
+// 	 */
+// 	write_reg(par, PWCTRL1, 0xA4, 0xA1);
+
+// 	write_reg(par, MIPI_DCS_SET_DISPLAY_ON);
+
+// 	if (HSD20_IPS)
+// 		write_reg(par, MIPI_DCS_ENTER_INVERT_MODE);
+
+// 	return 0;
+// }
+
+// st7735
+//youkai add
+static int init_display(struct fbtft_par *par)
+{
+	par->fbtftops.reset(par);//硬复位
+
+	mdelay(50);
+	write_reg(par,0x11);//软复位
+	mdelay(100);
+	//下面添加初始化函数write_reg 参数分别为：结构体指针，写命令，写数据....(后都为数据)
+	//ST7735s Frame Rate
+	write_reg(par,0xB1,0x05,0x3c,0x3c); 
+
+	write_reg(par,0xB2,0x05,0x3c,0x3c);  
+
+	write_reg(par,0xB3,0x05,0x3c,0x3c,0x05,0x3c,0x3c); 
+ 
+	
+	write_reg(par,0xB4,0x03); //Column inversion 
+	
+	//ST7735s Power Sequence
+	write_reg(par,0xC0,0x28,0x08,0x04); 
+
+	write_reg(par,0xC1,0xc0); 
 
-	write_reg(par, MIPI_DCS_SET_DISPLAY_ON);
+	write_reg(par,0xC2,0x0d,0x00); 
+	
+	write_reg(par,0xC3,0x8d,0x2a); //VCOM 
+	
+	write_reg(par,0xc4,0x8d,0xee); //MX, MY, RGB mode  
+	write_reg(par,0xc5,0x1a);
+	write_reg(par,0x36,0xc0);
+	//ST7735s Gamma Sequence
+	write_reg(par,0xe0,0x04,0x22,0x07,0x0a,0x2e,0x30,0x25,0x2a,0x28,0x26,0x2e,0x3a,0x00,0x01,0x03,0x13); 
 
-	if (HSD20_IPS)
-		write_reg(par, MIPI_DCS_ENTER_INVERT_MODE);
+	write_reg(par,0xe1,0x04,0x16,0x06,0x0d,0x2d,0x26,0x23,0x27,0x27,0x25,0x2d,0x3b,0x00,0x01,0x04,0x13);  
+	
+	write_reg(par,0x3A,0x05); //65k mode  
+	
+	write_reg(par,0x29);//Display on
+	mdelay(100);
 
 	return 0;
 }
@@ -221,8 +266,8 @@ static int set_gamma(struct fbtft_par *par, u32 *curves)
 
 	for (i = 0; i < par->gamma.num_curves; i++) {
 		c = i * par->gamma.num_values;
-		for (j = 0; j < par->gamma.num_values; j++)
-			curves[c + j] &= gamma_par_mask[j];
+		// for (j = 0; j < par->gamma.num_values; j++)
+		// 	curves[c + j] &= gamma_par_mask[j];
 		write_reg(par, PVGAMCTRL + i,
 			  curves[c + 0],  curves[c + 1],  curves[c + 2],
 			  curves[c + 3],  curves[c + 4],  curves[c + 5],
@@ -252,8 +297,8 @@ static int blank(struct fbtft_par *par, bool on)
 
 static struct fbtft_display display = {
 	.regwidth = 8,
-	.width = 240,
-	.height = 320,
+	.width = 128,//240,
+	.height = 160,//320,
 	.gamma_num = 2,
 	.gamma_len = 14,
 	.gamma = HSD20_IPS_GAMMA,
diff --git a/linux_5.10/drivers/staging/fbtft/fbtft-core.c b/linux_5.10/drivers/staging/fbtft/fbtft-core.c
index 4f362dad4..97b139d46 100644
--- a/linux_5.10/drivers/staging/fbtft/fbtft-core.c
+++ b/linux_5.10/drivers/staging/fbtft/fbtft-core.c
@@ -30,6 +30,9 @@
 #include "fbtft.h"
 #include "internal.h"
 
+#include <linux/gpio.h> //add
+#include <linux/of_gpio.h> //add
+
 static unsigned long debug;
 module_param(debug, ulong, 0000);
 MODULE_PARM_DESC(debug, "override device debug level");
@@ -71,68 +74,151 @@ void fbtft_dbg_hex(const struct device *dev, int groupsize,
 }
 EXPORT_SYMBOL(fbtft_dbg_hex);
 
+// default
+// static int fbtft_request_one_gpio(struct fbtft_par *par,
+// 				  const char *name, int index,
+// 				  struct gpio_desc **gpiop)
+// {
+// 	struct device *dev = par->info->device;
+// 	int ret = 0;
+
+// 	*gpiop = devm_gpiod_get_index_optional(dev, name, index,
+// 					       GPIOD_OUT_HIGH);
+// 	if (IS_ERR(*gpiop)) {
+// 		ret = PTR_ERR(*gpiop);
+// 		dev_err(dev,
+// 			"Failed to request %s GPIO: %d\n", name, ret);
+// 		return ret;
+// 	}
+// 	fbtft_par_dbg(DEBUG_REQUEST_GPIOS, par, "%s: '%s' GPIO\n",
+// 		      __func__, name);
+
+// 	return ret;
+// }
+
+// static int fbtft_request_gpios(struct fbtft_par *par)
+// {
+// 	int i;
+// 	int ret;
+
+// 	ret = fbtft_request_one_gpio(par, "reset", 0, &par->gpio.reset);
+// 	if (ret)
+// 		return ret;
+// 	ret = fbtft_request_one_gpio(par, "dc", 0, &par->gpio.dc);
+// 	if (ret)
+// 		return ret;
+// 	ret = fbtft_request_one_gpio(par, "rd", 0, &par->gpio.rd);
+// 	if (ret)
+// 		return ret;
+// 	ret = fbtft_request_one_gpio(par, "wr", 0, &par->gpio.wr);
+// 	if (ret)
+// 		return ret;
+// 	ret = fbtft_request_one_gpio(par, "cs", 0, &par->gpio.cs);
+// 	if (ret)
+// 		return ret;
+// 	ret = fbtft_request_one_gpio(par, "latch", 0, &par->gpio.latch);
+// 	if (ret)
+// 		return ret;
+// 	for (i = 0; i < 16; i++) {
+// 		ret = fbtft_request_one_gpio(par, "db", i,
+// 					     &par->gpio.db[i]);
+// 		if (ret)
+// 			return ret;
+// 		ret = fbtft_request_one_gpio(par, "led", i,
+// 					     &par->gpio.led[i]);
+// 		if (ret)
+// 			return ret;
+// 		ret = fbtft_request_one_gpio(par, "aux", i,
+// 					     &par->gpio.aux[i]);
+// 		if (ret)
+// 			return ret;
+// 	}
+
+// 	return 0;
+// }
+
+//https://blog.csdn.net/qq_46604211/article/details/116449891
 static int fbtft_request_one_gpio(struct fbtft_par *par,
-				  const char *name, int index,
-				  struct gpio_desc **gpiop)
+                  const char *name, int index,
+                  struct gpio_desc **gpiop)
 {
-	struct device *dev = par->info->device;
-	int ret = 0;
-
-	*gpiop = devm_gpiod_get_index_optional(dev, name, index,
-					       GPIOD_OUT_HIGH);
-	if (IS_ERR(*gpiop)) {
-		ret = PTR_ERR(*gpiop);
-		dev_err(dev,
-			"Failed to request %s GPIO: %d\n", name, ret);
-		return ret;
-	}
-	fbtft_par_dbg(DEBUG_REQUEST_GPIOS, par, "%s: '%s' GPIO\n",
-		      __func__, name);
-
-	return ret;
+    struct device *dev = par->info->device;
+    struct device_node *node = dev->of_node;
+    int gpio, flags, ret = 0;
+    enum of_gpio_flags of_flags;
+    if (of_find_property(node, name, NULL)) {
+        gpio = of_get_named_gpio_flags(node, name, index, &of_flags);
+        if (gpio == -ENOENT)
+            return 0;
+        if (gpio == -EPROBE_DEFER)
+            return gpio;
+        if (gpio < 0) {
+            dev_err(dev,
+                "failed to get '%s' from DT\n", name);
+            return gpio;
+        }
+         //active low translates to initially low
+        flags = (of_flags & OF_GPIO_ACTIVE_LOW) ? GPIOF_OUT_INIT_LOW :
+                            GPIOF_OUT_INIT_HIGH;
+        ret = devm_gpio_request_one(dev, gpio, flags,
+                        dev->driver->name);
+        if (ret) {
+            dev_err(dev,
+                "gpio_request_one('%s'=%d) failed with %d\n",
+                name, gpio, ret);
+            return ret;
+        }
+
+        *gpiop = gpio_to_desc(gpio);
+        fbtft_par_dbg(DEBUG_REQUEST_GPIOS, par, "%s: '%s' = GPIO%d\n",
+                            __func__, name, gpio);
+    }
+
+    return ret;
 }
 
 static int fbtft_request_gpios(struct fbtft_par *par)
 {
-	int i;
-	int ret;
-
-	ret = fbtft_request_one_gpio(par, "reset", 0, &par->gpio.reset);
-	if (ret)
-		return ret;
-	ret = fbtft_request_one_gpio(par, "dc", 0, &par->gpio.dc);
-	if (ret)
-		return ret;
-	ret = fbtft_request_one_gpio(par, "rd", 0, &par->gpio.rd);
-	if (ret)
-		return ret;
-	ret = fbtft_request_one_gpio(par, "wr", 0, &par->gpio.wr);
-	if (ret)
-		return ret;
-	ret = fbtft_request_one_gpio(par, "cs", 0, &par->gpio.cs);
-	if (ret)
-		return ret;
-	ret = fbtft_request_one_gpio(par, "latch", 0, &par->gpio.latch);
-	if (ret)
-		return ret;
-	for (i = 0; i < 16; i++) {
-		ret = fbtft_request_one_gpio(par, "db", i,
-					     &par->gpio.db[i]);
-		if (ret)
-			return ret;
-		ret = fbtft_request_one_gpio(par, "led", i,
-					     &par->gpio.led[i]);
-		if (ret)
-			return ret;
-		ret = fbtft_request_one_gpio(par, "aux", i,
-					     &par->gpio.aux[i]);
-		if (ret)
-			return ret;
-	}
-
-	return 0;
+    int i;
+    int ret;
+
+    ret = fbtft_request_one_gpio(par, "reset-gpios", 0, &par->gpio.reset);
+    if (ret)
+        return ret;
+    ret = fbtft_request_one_gpio(par, "dc-gpios", 0, &par->gpio.dc);
+    if (ret)
+        return ret;
+    ret = fbtft_request_one_gpio(par, "rd-gpios", 0, &par->gpio.rd);
+    if (ret)
+        return ret;
+    ret = fbtft_request_one_gpio(par, "wr-gpios", 0, &par->gpio.wr);
+    if (ret)
+        return ret;
+    ret = fbtft_request_one_gpio(par, "cs-gpios", 0, &par->gpio.cs);
+    if (ret)
+        return ret;
+    ret = fbtft_request_one_gpio(par, "latch-gpios", 0, &par->gpio.latch);
+    if (ret)
+        return ret;
+    for (i = 0; i < 16; i++) {
+        ret = fbtft_request_one_gpio(par, "db-gpios", i,
+                         &par->gpio.db[i]);
+        if (ret)
+            return ret;
+        ret = fbtft_request_one_gpio(par, "led-gpios", i,
+                         &par->gpio.led[i]);
+        if (ret)
+            return ret;
+        ret = fbtft_request_one_gpio(par, "aux-gpios", i,
+                         &par->gpio.aux[i]);
+        if (ret)
+            return ret;
+    }
+
+    return 0;
 }
 
+
 #ifdef CONFIG_FB_BACKLIGHT
 static int fbtft_backlight_update_status(struct backlight_device *bd)
 {
@@ -222,17 +308,32 @@ static void fbtft_set_addr_win(struct fbtft_par *par, int xs, int ys, int xe,
 	write_reg(par, MIPI_DCS_WRITE_MEMORY_START);
 }
 
+//作者：Leesans https://www.bilibili.com/read/cv9947785/ 出处：bilibili
 static void fbtft_reset(struct fbtft_par *par)
 {
-	if (!par->gpio.reset)
-		return;
-	fbtft_par_dbg(DEBUG_RESET, par, "%s()\n", __func__);
-	gpiod_set_value_cansleep(par->gpio.reset, 1);
-	usleep_range(20, 40);
-	gpiod_set_value_cansleep(par->gpio.reset, 0);
-	msleep(120);
+    if (!par->gpio.reset)
+        return;
+    fbtft_par_dbg(DEBUG_RESET, par, "%s()\n", __func__);
+    gpiod_set_value_cansleep(par->gpio.reset, 1);
+    msleep(10);
+    gpiod_set_value_cansleep(par->gpio.reset, 0);
+    msleep(200);
+    gpiod_set_value_cansleep(par->gpio.reset, 1);
+    msleep(10);
 }
 
+// defalut
+// static void fbtft_reset(struct fbtft_par *par)
+// {
+// 	if (!par->gpio.reset)
+// 		return;
+// 	fbtft_par_dbg(DEBUG_RESET, par, "%s()\n", __func__);
+// 	gpiod_set_value_cansleep(par->gpio.reset, 1);
+// 	usleep_range(20, 40);
+// 	gpiod_set_value_cansleep(par->gpio.reset, 0);
+// 	msleep(120);
+// }
+
 static void fbtft_update_display(struct fbtft_par *par, unsigned int start_line,
 				 unsigned int end_line)
 {
diff --git a/linux_5.10/sound/soc/cvitek/Kconfig b/linux_5.10/sound/soc/cvitek/Kconfig
index b2540aa8e..0fec0c832 100644
--- a/linux_5.10/sound/soc/cvitek/Kconfig
+++ b/linux_5.10/sound/soc/cvitek/Kconfig
@@ -190,3 +190,16 @@ config SND_SOC_CV1835_LT9611
 	tristate "Support for the lt9611 card"
 	help
 	  lt9611 codec enable.
+
+config SND_SOC_CV1835_MAX98357A
+	tristate "Support for the max98357a card"
+	help
+	  max98357a codec enable.
+
+config CONFIG_SND_SOC_CV1800ADC
+	tristate "Support for the CV1800ADC card"
+	help
+	 Say Y or M if you want to add support for CV1800 ADC connect
+	 to CVITEK CV180 I2S device.
+	 Then select it for built in or module.
+	 Say N, if not support
diff --git a/linux_5.10/sound/soc/cvitek/Makefile b/linux_5.10/sound/soc/cvitek/Makefile
index b2a6d8b54..39922b868 100644
--- a/linux_5.10/sound/soc/cvitek/Makefile
+++ b/linux_5.10/sound/soc/cvitek/Makefile
@@ -79,3 +79,9 @@ endif
 obj-$(CONFIG_CV1835_I2S_SUBSYS) += cv1835_i2s_subsys.o
 
 obj-$(CONFIG_SND_SOC_CV1835_LT9611) += cv1835_lt9611.o
+
+# youkai add
+obj-$(CONFIG_SND_SOC_CV1835_MAX98357A) += cv1835_max98357a.o
+
+obj-$(CONFIG_SND_SOC_CV1800ADC) += cv181x_cv181xadc.o
+obj-$(CONFIG_SND_SOC_CV1800ADC) += cv181xadc.o
diff --git a/u-boot-2021.10/include/configs/cv180x-asic.h b/u-boot-2021.10/include/configs/cv180x-asic.h
index 08b70193f..5d3404817 100644
--- a/u-boot-2021.10/include/configs/cv180x-asic.h
+++ b/u-boot-2021.10/include/configs/cv180x-asic.h
@@ -280,7 +280,7 @@
 	#endif
 
 	#define SET_BOOTARGS "setenv bootargs ${root} ${mtdparts} " \
-					"console=$consoledev,$baudrate $othbootargs;"
+					"console=tty0 console=$consoledev,$baudrate $othbootargs;"
 
 	#define SD_BOOTM_COMMAND \
 				SET_BOOTARGS \
-------------------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------

********************************************************************************************************
LangZhao Feb 7 2024


Hi emeb
https://community.milkv.io/t/i2s-milk-v-duo-speaker-i2s2/666

You can check this topic.

Focus on i2s2

cv180x-asic_qfn.dtsi
-------------------------------------------------------------------------------------------------------
	/delete-node/ i2s@04110000;
	/delete-node/ sound_ext1;
	/delete-node/ sound_ext2;
	/delete-node/ sound_PDM;
-------------------------------------------------------------------------------------------------------
The erasure of i2s@04120000 has been removed in the newest SDK.

Add i2s2 dma
build\boards\default\dts\cv180x\cv180x_base.dtsi
-------------------------------------------------------------------------------------------------------
	i2s2: i2s@04120000 {
		compatible = "cvitek,cv1835-i2s";
		reg = <0x0 0x04120000 0x0 0x2000>;
		clocks = <&i2s_mclk 0>;
		clock-names = "i2sclk";
		dev-id = <2>;
		#sound-dai-cells = <0>;
		dmas = <&dmac 6 1 1 /* read channel */
			&dmac 1 1 1>; /* write channel */
		dma-names = "rx", "tx";
		capability = "txrx";
		mclk_out = "false";
	};
-------------------------------------------------------------------------------------------------------
The failure to create the sound card may be due to the i2s node.

My audio topics flow:

【i2s】Milk-V Duo添加speaker——i2s2  
       https://community.milkv.io/t/i2s-milk-v-duo-speaker-i2s2/666/1
【i2s】Milk-V Duo使用analog mic - #6 by emeb
       https://community.milkv.io/t/i2s-milk-v-duo-analog-mic/811/6
【i2s】Milk-V Duo添加speaker——max98357a解码器驱动 - #8 by emeb
       https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668/8
【I2S-未解决】Milk-V Duo添加tinyplay工具（i2s2 TX Reset失败） - #6 by Reset 8
       https://community.milkv.io/t/i2s-milk-v-duo-tinyplay-i2s2-tx-reset/734/6
Thanks~


********************************************************************************************************


-------------------------------------------------------------------------------------------------------


LangZhao Feb 22 2024

git diff for mic&max98357a driver
-------------------------------------------------------------------------------------------------------
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts b/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
index e90e2c32a..d6941c2ce 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
@@ -29,3 +29,29 @@
 	model = "Milk-V Duo";
 };
 
+// [+] youaki, add for speaker
+
+/*
+&i2s2 {
+	status = "okay";
+	#sound-dai-cells = <0>;
+};
+*/
+
+/ {
+	/* codec */
+	max98357a: max98357a {
+		#sound-dai-cells = <0>;
+		compatible = "maxim,max98357a";
+		status = "okay";
+		/*sdmode-gpios = <&gpio1 14 0>;*/ 
+		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
+	};
+
+	/* sound_ext3 use external codec */
+	sound {
+		compatible = "cvitek,cv1835-max98357a";
+		cvi,card_name = "cv1835_max98357a";
+	};
+
+};
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig b/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
index 4c60136fc..1e9bc704a 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
@@ -345,3 +345,15 @@ CONFIG_EPOLL=n
 CONFIG_ELF_CORE=y
 CONFIG_COREDUMP=y
 CONFIG_PROC_SYSCTL=y
+
+# youkai add for audio
+CONFIG_SND_PROC_FS=y
+# CONFIG_SND_SOC_CV1835_CONCURRENT_I2S=y  # question
+
+# close dac -- dac gpio used to led
+CONFIG_SND_SOC_CV182XA_CV182XADAC=n
+CONFIG_SND_SOC_CV182XADAC=n
+
+# add max98357a for speaker
+CONFIG_SND_SOC_MAX98357A=y
+CONFIG_SND_SOC_CV1835_MAX98357A=y
diff --git a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts
index 16734cb47..f08c768ed 100644
--- a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts
+++ b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts
@@ -34,3 +34,22 @@
 	model = "Milk-V Duo256M";
 };
 
+// [+] youaki, add for speaker
+
+/ {
+	/* codec */
+	max98357a: max98357a {
+		#sound-dai-cells = <0>;
+		compatible = "maxim,max98357a";
+		status = "okay";
+		/*sdmode-gpios = <&gpio1 14 0>;*/ 
+		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
+	};
+
+	/* sound_ext3 use external codec */
+	sound {
+		compatible = "cvitek,cv1835-max98357a";
+		cvi,card_name = "cv1835_max98357a";
+	};
+
+};
diff --git a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
index 4bfbc511c..f26305398 100644
--- a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
+++ b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
@@ -294,3 +294,15 @@ CONFIG_EPOLL=n
 CONFIG_ELF_CORE=y
 CONFIG_COREDUMP=y
 CONFIG_PROC_SYSCTL=y
+
+# youkai add for audio
+CONFIG_SND_PROC_FS=y
+# CONFIG_SND_SOC_CV1835_CONCURRENT_I2S=y  # question
+
+# close dac -- dac gpio used to led
+CONFIG_SND_SOC_CV182XA_CV182XADAC=n
+CONFIG_SND_SOC_CV182XADAC=n
+
+# add max98357a for speaker
+CONFIG_SND_SOC_MAX98357A=y
+CONFIG_SND_SOC_CV1835_MAX98357A=y
diff --git a/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi b/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi
index 2bcacfb3a..8d5b60514 100644
--- a/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi
+++ b/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi
@@ -5,7 +5,9 @@
 
 / {
 	/delete-node/ i2s@04110000;
-	/delete-node/ i2s@04120000;
+	/*
+		/delete-node/ i2s@04120000;
+	*/
 	/delete-node/ sound_ext1;
 	/delete-node/ sound_ext2;
 	/delete-node/ sound_PDM;
diff --git a/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi b/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi
index 93081c5d7..7d915d6dd 100644
--- a/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi
+++ b/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi
@@ -104,7 +104,9 @@
 / {
 	/delete-node/ wifi-sd@4320000;
 	/delete-node/ i2s@04110000;
-	/delete-node/ i2s@04120000;
+	/*
+		/delete-node/ i2s@04120000;
+	*/
 	/delete-node/ sound_ext1;
 	/delete-node/ sound_ext2;
 	/delete-node/ sound_PDM;
diff --git a/buildroot-2021.05/configs/milkv-duo256m_musl_riscv64_defconfig b/buildroot-2021.05/configs/milkv-duo256m_musl_riscv64_defconfig
index 20fa4431e..aee4dbef4 100644
--- a/buildroot-2021.05/configs/milkv-duo256m_musl_riscv64_defconfig
+++ b/buildroot-2021.05/configs/milkv-duo256m_musl_riscv64_defconfig
@@ -510,3 +510,6 @@ BR2_PACKAGE_NCNN=y
 BR2_PACKAGE_NCNN_BENCH=y
 BR2_PACKAGE_NCNN_EXAMPLE=y
 BR2_PACKAGE_CMATRIX=y
+
+# youkai add -- open audio
+BR2_PACKAGE_TINYALSA=y
diff --git a/buildroot-2021.05/configs/milkv-duo_musl_riscv64_defconfig b/buildroot-2021.05/configs/milkv-duo_musl_riscv64_defconfig
index 2bc8cd5e3..f0c014b9c 100644
--- a/buildroot-2021.05/configs/milkv-duo_musl_riscv64_defconfig
+++ b/buildroot-2021.05/configs/milkv-duo_musl_riscv64_defconfig
@@ -519,3 +519,6 @@ BR2_PACKAGE_IPERF3=y
 BR2_PACKAGE_NTP=y
 BR2_PACKAGE_NTP_NTPDATE=y
 BR2_PACKAGE_NTP_NTPTIME=y
+
+# youkai add -- open audio
+BR2_PACKAGE_TINYALSA=y
diff --git a/linux_5.10/sound/soc/cvitek/Kconfig b/linux_5.10/sound/soc/cvitek/Kconfig
index b2540aa8e..62bd74cac 100644
--- a/linux_5.10/sound/soc/cvitek/Kconfig
+++ b/linux_5.10/sound/soc/cvitek/Kconfig
@@ -190,3 +190,9 @@ config SND_SOC_CV1835_LT9611
 	tristate "Support for the lt9611 card"
 	help
 	  lt9611 codec enable.
+
+# youkai add for extend speaker
+config SND_SOC_CV1835_MAX98357A
+	tristate "Support for the max98357a card"
+	help
+	  max98357a codec enable.
diff --git a/linux_5.10/sound/soc/cvitek/Makefile b/linux_5.10/sound/soc/cvitek/Makefile
index 93060ca2b..f538b0e7f 100644
--- a/linux_5.10/sound/soc/cvitek/Makefile
+++ b/linux_5.10/sound/soc/cvitek/Makefile
@@ -79,3 +79,6 @@ endif
 obj-$(CONFIG_CV1835_I2S_SUBSYS) += cv1835_i2s_subsys.o
 
 obj-$(CONFIG_SND_SOC_CV1835_LT9611) += cv1835_lt9611.o
+
+# youkai add for extend speaker
+obj-$(CONFIG_SND_SOC_CV1835_MAX98357A) += cv1835_max98357a.o
-------------------------------------------------------------------------------------------------------

~~~

********************************************************************************************************


