[источник](https://community.milkv.io/t/duo256m-add-max98357a-sound-card-done/1494)

**********************************************************

~~~
[duo256m] Add max98357a sound card —— done

LangZhao  6 Mar 2024

Refer to the changes made by Duo.
  https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668/2

The main difference is that the configuration path of duo256m has changed, which is recorded here.

This article has opened the max98357a configuration for duo and duo256m.

Progress: Playing audio through max98357a successfully.

Q1: The sound card creation of the external decoder is normal, but there is no waveform in i2s.
A1: Fixed. Change eth to gpio.


1. git diff
----------------diff---------------------------------------------------------------------------
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts b/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
index e90e2c32a..ccf543e1a 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/dts_riscv/cv1800b_milkv_duo_sd.dts
@@ -29,3 +29,26 @@
 	model = "Milk-V Duo";
 };
 
+// [+] youaki, add for speaker
+&i2s2 {
+	status = "okay";
+	#sound-dai-cells = <0>;
+};
+
+/ {
+	/* codec */
+	max98357a: max98357a {
+		#sound-dai-cells = <0>;
+		compatible = "maxim,max98357a";
+		status = "okay";
+
+		sdmode-gpios = <&porta 14 0>; 
+		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
+	};
+
+	/* sound_ext3 use external codec */
+	sound {
+		compatible = "cvitek,cv1835-max98357a";
+		cvi,card_name = "cv1835_max98357a";
+	};
+};
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig b/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
index 817e771e6..29ee9841a 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/linux/cvitek_cv1800b_milkv_duo_sd_defconfig
@@ -345,3 +345,16 @@ CONFIG_EPOLL=y
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
+# CONFIG_DEBUG_INFO=y
diff --git a/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c b/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c
index 74941cb09..69eb4320d 100644
--- a/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c
+++ b/build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init.c
@@ -41,5 +41,12 @@ int cvi_board_init(void)
 	PINMUX_CONFIG(SD1_D0, SPI2_SDI);
 	PINMUX_CONFIG(SD1_D3, SPI2_CS_X);
 
+	// youkai add for max98357a speaker
+	PINMUX_CONFIG(PAD_ETH_TXP, IIS2_LRCK);
+	PINMUX_CONFIG(PAD_ETH_TXM, IIS2_BCLK);
+	PINMUX_CONFIG(PAD_ETH_RXP, IIS2_DO);
+	PINMUX_CONFIG(PAD_ETH_RXM, IIS2_DI);
+	PINMUX_CONFIG(SD0_PWR_EN, XGPIOA_14);	// sdmode
+
 	return 0;
 }
diff --git a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts
index 16734cb47..0b409e040 100644
--- a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts
+++ b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts
@@ -34,3 +34,32 @@
 	model = "Milk-V Duo256M";
 };
 
+// [+] youaki, add for speaker
+
+&i2s2 {
+	status = "okay";
+};
+
+&ethernet0 {
+	status = "disabled";
+};
+
+/ {
+	/* codec */
+	max98357a: max98357a {
+		#sound-dai-cells = <0>;
+		compatible = "maxim,max98357a";
+		status = "okay";
+
+		/* this may be necessary, otherwise driver will not load */
+		sdmode-gpios = <&porta 14 0>;
+		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
+	};
+
+	/* sound_ext3 use external codec */
+	sound_max98357a {
+		compatible = "cvitek,cv1835-max98357a";
+		cvi,card_name = "cv1835_max98357a";
+	};
+};
+
diff --git a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
index 955fdbf02..c592fd36a 100644
--- a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
+++ b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
@@ -294,3 +294,16 @@ CONFIG_EPOLL=y
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
+# CONFIG_DEBUG_INFO=y
diff --git a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/u-boot/cvi_board_init.c b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/u-boot/cvi_board_init.c
index c9c722236..789ee1ad5 100644
--- a/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/u-boot/cvi_board_init.c
+++ b/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/u-boot/cvi_board_init.c
@@ -42,5 +42,12 @@ int cvi_board_init(void)
 	PINMUX_CONFIG(ADC1, XGPIOB_3);             // GP26 (ADC1)
 	PINMUX_CONFIG(USB_VBUS_DET, XGPIOB_6);     // GP27 (ADC2)
 
+	// youkai add for max98357a speaker
+	PINMUX_CONFIG(PAD_ETH_TXP, IIS2_LRCK);
+	PINMUX_CONFIG(PAD_ETH_TXM, IIS2_BCLK);
+	PINMUX_CONFIG(PAD_ETH_RXP, IIS2_DO);
+	PINMUX_CONFIG(PAD_ETH_RXM, IIS2_DI);
+	PINMUX_CONFIG(SD0_PWR_EN, XGPIOA_14);	// sdmode
+
 	return 0;
 }
\ No newline at end of file
diff --git a/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi b/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi
index 2bcacfb3a..f91b1204a 100644
--- a/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi
+++ b/build/boards/default/dts/cv180x/cv180x_asic_bga.dtsi
@@ -5,7 +5,7 @@
 
 / {
 	/delete-node/ i2s@04110000;
-	/delete-node/ i2s@04120000;
+
 	/delete-node/ sound_ext1;
 	/delete-node/ sound_ext2;
 	/delete-node/ sound_PDM;
diff --git a/build/boards/default/dts/cv181x/cv181x_asic_bga.dtsi b/build/boards/default/dts/cv181x/cv181x_asic_bga.dtsi
index f54872935..e7cb52c55 100644
--- a/build/boards/default/dts/cv181x/cv181x_asic_bga.dtsi
+++ b/build/boards/default/dts/cv181x/cv181x_asic_bga.dtsi
@@ -9,7 +9,7 @@
 
 / {
 	/delete-node/ i2s@04110000;
-	/delete-node/ i2s@04120000;
+
 	/delete-node/ sound_ext1;
 	/delete-node/ sound_ext2;
 	/delete-node/ sound_PDM;
diff --git a/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi b/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi
index 93081c5d7..24f7f0bac 100644
--- a/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi
+++ b/build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi
@@ -104,7 +104,7 @@
 / {
 	/delete-node/ wifi-sd@4320000;
 	/delete-node/ i2s@04110000;
-	/delete-node/ i2s@04120000;
+
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
diff --git a/device/milkv-duo/overlay/mnt/system/usb-rndis.sh b/device/milkv-duo/overlay/mnt/system/usb-rndis.sh
index e0f132b3a..95972ae2f 100755
--- a/device/milkv-duo/overlay/mnt/system/usb-rndis.sh
+++ b/device/milkv-duo/overlay/mnt/system/usb-rndis.sh
@@ -7,6 +7,9 @@
 sleep 0.5
 ifconfig usb0 192.168.42.1
 
+# youkai add for tinyalsa
+ln -s /lib/ld-musl-riscv64v0p7_xthead.so.1 /lib/ld-musl-riscv64.so.1
+
 count=`ps | grep dnsmasq | grep -v grep | wc -l`
 if [ ${count} -lt 1 ] ;then
   echo "/etc/init.d/S80dnsmasq start" >> /tmp/rndis.log 2>&1
diff --git a/device/milkv-duo256m/overlay/mnt/system/usb-rndis.sh b/device/milkv-duo256m/overlay/mnt/system/usb-rndis.sh
index e0f132b3a..95972ae2f 100755
--- a/device/milkv-duo256m/overlay/mnt/system/usb-rndis.sh
+++ b/device/milkv-duo256m/overlay/mnt/system/usb-rndis.sh
@@ -7,6 +7,9 @@
 sleep 0.5
 ifconfig usb0 192.168.42.1
 
+# youkai add for tinyalsa
+ln -s /lib/ld-musl-riscv64v0p7_xthead.so.1 /lib/ld-musl-riscv64.so.1
+
 count=`ps | grep dnsmasq | grep -v grep | wc -l`
 if [ ${count} -lt 1 ] ;then
   echo "/etc/init.d/S80dnsmasq start" >> /tmp/rndis.log 2>&1
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
----------------diff---------------------------------------------------------------------------


You can save this git diff to git_diff_max98357a.txt, then apply this diff in duo-buildroot-sdk dir.

-------------------------------------------------------------------------------------------
git apply git_diff_max98357a.txt
-------------------------------------------------------------------------------------------


Add sound card driver ---- “cv1835_max98357a.c”
Build eth2i2s and run in duo.
Play audio files by tinyplay cmd


2. Codec driver path
max98357a：
duo-buildroot-sdk-develop\linux_5.10\sound\soc\codecs\max98357a.c

duo-buildroot-sdk-develop\linux_5.10\sound\soc\codecs\Makefile

------------------------------Makefile-------------------------------------------------------------
snd-soc-max98357a-objs := max98357a.o

obj-$(CONFIG_SND_SOC_MAX98357A)	+= snd-soc-max98357a.o
------------------------------Makefile-------------------------------------------------------------


duo-buildroot-sdk-develop\linux_5.10\sound\soc\Makefile

-------------------------------------------------------------------------------------------
obj-$(CONFIG_SND_SOC)	+= snd-soc-core.o
obj-$(CONFIG_SND_SOC)	+= codecs/
-------------------------------------------------------------------------------------------

duo-buildroot-sdk-develop\linux_5.10\sound\Makefile

-------------------------------------------------------------------------------------------
obj-$(CONFIG_SOUND) += soundcore.o
obj-$(CONFIG_SND) += core/ i2c/ drivers/ isa/ pci/ ppc/ arm/ sh/ synth/ usb/ \
	firewire/ sparc/ spi/ parisc/ pcmcia/ mips/ soc/ atmel/ hda/ x86/ xen/
-------------------------------------------------------------------------------------------

duo-buildroot-sdk-develop\linux_5.10\Makefile

-------------------------------------------------------------------------------------------
drivers-y	:= drivers/ sound/
-------------------------------------------------------------------------------------------


3. Add sound card driver

3.1. Makefile
linux_5.10\sound\soc\cvitek\Makefile

-------------------------------------------------------------------------------------------
# youkai add for extend speaker
obj-$(CONFIG_SND_SOC_CV1835_MAX98357A) += cv1835_max98357a.o
-------------------------------------------------------------------------------------------

3.2. Kconfig
linux_5.10\sound\soc\cvitek\Kconfig

-------------------------------------------------------------------------------------------
# youkai add for extend speaker
config SND_SOC_CV1835_MAX98357A
	tristate "Support for the max98357a card"
	help
	  max98357a codec enable.
-------------------------------------------------------------------------------------------


3.3. Defconfig
build/boards/cv181x/cv1812cp_milkv_duo256m_sd/linux/cvitek_cv1812cp_milkv_duo256m_sd_defconfig
-------------------------------------------------------------------------------------------
# youkai add for audio
CONFIG_SND_PROC_FS=y

# close dac -- dac gpio used to led
CONFIG_SND_SOC_CV182XA_CV182XADAC=n
CONFIG_SND_SOC_CV182XADAC=n

# add max98357a for speaker
CONFIG_SND_SOC_MAX98357A=y
CONFIG_SND_SOC_CV1835_MAX98357A=y
# CONFIG_DEBUG_INFO=y
-------------------------------------------------------------------------------------------


3.4. Snd card driver(machine driver)
linux_5.10\sound\soc\cvitek\cv1835_max98357a.c

----------------------------cv1835_max98357a.c---------------------------------------------------------------
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Machine driver for Max98357a on CVITEK CV1835
 *
 * Copyright 2019 CVITEK
 *
 * Author: Youkai
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

// struct card_private {
	// 	int tmp;	//save sth.
	// };

enum {
	DPCM_AUDIO_SPKR = 0,
};

static const struct snd_kcontrol_new cv1835_max98357a_dapm_controls[] = {
	SOC_DAPM_PIN_SWITCH("SPKR"),
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
		.dpcm_playback = 1,
	},
};


static struct snd_soc_card cv1835_max98357a = {
	.name = "cv1835_max98357a",	// card name
	.owner = THIS_MODULE,
	.dai_link = cv1835_max98357a_dai,
	.num_links = ARRAY_SIZE(cv1835_max98357a_dai),

	.controls = cv1835_max98357a_dapm_controls,
	.num_controls = ARRAY_SIZE(cv1835_max98357a_dapm_controls),

	.dapm_widgets		= cv1835_max98357a_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(cv1835_max98357a_dapm_widgets),

	.dapm_routes		= cv1835_max98357a_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(cv1835_max98357a_dapm_routes),
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

	int ret;

	// struct card_private *ctx;		// create private data for card
	// ctx = devm_kzalloc(&pdev->dev, sizeof(*ctx), GFP_KERNEL);
	// if (!ctx)
	// 	return -ENOMEM;

	// snd_soc_card_set_drvdata(card, ctx);	//save card info to snd_card

	if (np) {
		of_property_read_string(np, "cvi,card_name", &card->name);
		card->dev = &pdev->dev;
		dev_err(&pdev->dev,"snd card name = %s",card->name);
	}

	platform_set_drvdata(pdev, card);

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

MODULE_AUTHOR("Youkai");
MODULE_DESCRIPTION("ALSA SoC cv1835 max98357a machine driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:cv1835-max98357a");
------------------------cv1835_max98357a.c-------------------------------------------------------------------


4. DTS
build/boards/cv181x/cv1812cp_milkv_duo256m_sd/dts_riscv/cv1812cp_milkv_duo256m_sd.dts

-------------------------cv1812cp_milkv_duo256m_sd.dts------------------------------------------------------------------
// [+] youaki, add for speaker

&i2s2 {
	status = "okay";
};

/ {
	/* codec */
	max98357a: max98357a {
		#sound-dai-cells = <0>;
		compatible = "maxim,max98357a";
		status = "okay";

		sdmode-gpios = <&porta 14 0>;
		/* max98357a has gain & sd_mode gpio. but codec driver just has sdmode */
	};

	/* sound_ext3 use external codec */
	sound_max98357a {
		compatible = "cvitek,cv1835-max98357a";
		cvi,card_name = "cv1835_max98357a";
	};
};
----------------------------cv1812cp_milkv_duo256m_sd.dts---------------------------------------------------------------

build/boards/default/dts/cv181x/cv181x_asic_bga.dtsi
build/boards/default/dts/cv181x/cv181x_asic_qfn.dtsi

Remove the delete of i2s2.

-------------------------------------------------------------------------------------------
/* 
    /delete-node/ i2s@04110000;
*/
-------------------------------------------------------------------------------------------


5. Pin config
build/boards/cv181x/cv1812cp_milkv_duo256m_sd/u-boot/cvi_board_init.c
-------------------------------------------------------------------------------------------
// youkai add for max98357a speaker
	PINMUX_CONFIG(PAD_ETH_TXP, IIS2_LRCK);
	PINMUX_CONFIG(PAD_ETH_TXM, IIS2_BCLK);
	PINMUX_CONFIG(PAD_ETH_RXP, IIS2_DO);
	PINMUX_CONFIG(PAD_ETH_RXM, IIS2_DI);
	PINMUX_CONFIG(SD0_PWR_EN, XGPIOA_14);	// sdmode
-------------------------------------------------------------------------------------------

6. Add tinyalsa
device/milkv-duo256m/overlay/mnt/system/usb-rndis.sh

-------------------------------------------------------------------------------------------
# youkai add for tinyalsa
ln -s /lib/ld-musl-riscv64v0p7_xthead.so.1 /lib/ld-musl-riscv64.so.1
-------------------------------------------------------------------------------------------

buildroot-2021.05/configs/milkv-duo256m_musl_riscv64_defconfig
-------------------------------------------------------------------------------------------
# youkai add -- open audio
BR2_PACKAGE_TINYALSA=y
-------------------------------------------------------------------------------------------

7. Change eth0 gpio to i2s ---- eth2i2s
Code from    Eric Brombaugh https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668/46

-------------------------------------eth2i2s.c------------------------------------------------------
// code from https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668/46
// https://community.milkv.io/u/emeb
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define ETH_BASE_ADDRESS 0x03009000  // Base address of ETH registers on Duo

// Offsets for ETH registers
#define ETH_070 0x070
#define ETH_074 0x074
#define ETH_078 0x078
#define ETH_07C 0x07C
#define ETH_800 0x800
#define ETH_804 0x804
#define ETH_808 0x806

// word access macro
#define word(x) (x / sizeof(unsigned int))

int main() {
    int mem_fd;
    void *reg_map;

    // Open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("can't open /dev/mem");
        return 1;
    }

    // Map registers to user space memory
    reg_map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, ETH_BASE_ADDRESS);

    if (reg_map == MAP_FAILED) {
        perror("mmap error");
        close(mem_fd);
        return 1;
    }

    // Access ETH registers
    volatile unsigned int *eth_reg = (volatile unsigned int *)reg_map;
    unsigned int temp;

    // 0x03009804[0] = 1'b1 (rg_ephy_apb_rw_sel=1, use apb interface)
    temp = eth_reg[word(ETH_804)];
    printf("READ 804: 0x%08X\n", temp);
    temp |= 0x1;
    eth_reg[word(ETH_804)] = temp;
    printf("WRITE 804: 0x%08X\n", temp);

    // 0x03009808[4:0] = 5'b00001 (rg_ephy_pll_stable_cnt[4:0] = 5'd1 (10us))
    temp = eth_reg[word(ETH_808)];
    printf("READ 808: 0x%08X\n", temp);
    temp &= ~0x1F;
    temp |= 0x01;
    eth_reg[word(ETH_808)] = temp;
    printf("WRITE 808: 0x%08X\n", temp);

    // 0x03009800[2] = 0x0905 (rg_ephy_dig_rst_n=1, reset release, other keep default)
    temp = eth_reg[word(ETH_800)];
    printf("READ 800: 0x%08X\n", temp);
    temp = 0x0905;
    eth_reg[word(ETH_800)] = temp;
    printf("WRITE 808: 0x%08X\n", temp);
    
    // Delay 10us
    usleep(10);

    // 0x0300907C[12:8]= 5'b00101 (page_sel_mode0 = page 5)
    temp = eth_reg[word(ETH_07C)];
    printf("READ 07C: 0x%08X\n", temp);
    temp &= ~(0x1F << 8);
    temp |= (0x05 << 8);
    eth_reg[word(ETH_07C)] = temp;
    printf("WRITE 07C: 0x%08X\n", temp);
    
    // 0x03009078[11:0] = 0xF00 (set to gpio from top)
    temp = eth_reg[word(ETH_078)];
    printf("READ 078: 0x%08X\n", temp);
    temp &= ~0xFFF;
    temp |= 0xF00;
    eth_reg[word(ETH_078)] = temp;
    printf("WRITE 078: 0x%08X\n", temp);

    // 0x03009074[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)
    temp = eth_reg[word(ETH_074)];
    printf("READ 074: 0x%08X\n", temp);
    temp |= 0x606;
    eth_reg[word(ETH_074)] = temp;
    printf("WRITE 074: 0x%08X\n", temp);

    // 0x03009070[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)
    temp = eth_reg[word(ETH_070)];
    printf("READ 070: 0x%08X\n", temp);
    temp |= 0x606;
    eth_reg[word(ETH_070)] = temp;
    printf("WRITE 070: 0x%08X\n", temp);

    // Unmap memory
    munmap(reg_map, 4096);

    // Close /dev/mem
    close(mem_fd);

    return 0;
}
-------------------------------------eth2i2s.c------------------------------------------------------

build cmd

riscv64-unknown-linux-musl-gcc -static -o eth2i2s eth2i2s.c
-------------------------------------------------------------------------------------------

8. Upload music and eth2i2s bin

-------------------------------------------------------------------------------------------
scp alarm01.wav root@192.168.42.1:~/

scp eth2i2s root@192.168.42.1:~/
-------------------------------------------------------------------------------------------

9. Test cmd

-------------------------------------------------------------------------------------------
chmod 777 eth2i2s
./eth2i2s
tinyplay alarm01.wav -D 0 -d 0 -c 1
-------------------------------------------------------------------------------------------

10. HW connect
  https://community.milkv.io/uploads/default/optimized/1X/34f90d5b430316127175013828a5997672089008_2_670x500.png
-------------------------------------------------------------------------------------------
eth:  TXP  TXN  GND  RXP   RXN
i2s:  --   DIN   --  BCLK  LRC
-------------------------------------------------------------------------------------------

Din is for max98357a.
For milkv duo this pin is data out.

11. Test result
-------------------------------------------------------------------------------------------
 Thank you, guys, for your great work! [Milk-V Duo + MAX98357A = Audio Output] 
     https://www.youtube.com/watch?v=Ze7ocYdTn38

@dnech
Denis
Joined January 18, 2024
Dnech has uploaded a test video and provided a website for downloading audio test files.

GZ05: Multimedia Systems: Samples (ucl.ac.uk) 4

http://www0.cs.ucl.ac.uk/teaching/GZ05/samples/

You can also separate dual channel audio files into mono channels by Audacity, such as your favorite music.

End
I have received a lot of help from friends in developing the audio function of the Duo board, such as Emeb and Dnech.
Thank you very much.

Enjoy your duo to record and playback audio files.

I may develop an audio playback application by lvgl in the future, please stay tuned.

https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668/58

~~~

