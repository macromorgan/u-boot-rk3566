// SPDX-License-Identifier: GPL-2.0+

#include <abuf.h>
#include <asm/io.h>
#include <dm.h>
#include <linux/delay.h>
#include <pwm.h>
#include <rng.h>
#include <stdlib.h>

#include <i2c.h>

/* Start LED very early so user knows device is on. */
void led_setup(void)
{
	writel(0x00600060, 0xfdd6000c);
	writel(0x00600040, 0xfdd60004);
}

/* Use hardware rng to seed Linux random. */
int board_rng_seed(struct abuf *buf)
{
	struct udevice *dev;
	size_t len = 0x8;
	u64 *data;

	data = malloc(len);
	if (!data) {
		printf("Out of memory\n");
		return -ENOMEM;
	}

	if (uclass_get_device(UCLASS_RNG, 0, &dev) || !dev) {
		printf("No RNG device\n");
		return -ENODEV;
	}

	if (dm_rng_read(dev, data, len)) {
		printf("Reading RNG failed\n");
		return -EIO;
	}

	abuf_init_set(buf, data, len);

	return 0;
}

void __maybe_unused startup_buzz(void)
{
	struct udevice *dev;
	int err;

	err = uclass_get_device(UCLASS_PWM, 0, &dev);
	if (err) {
		printf("pwm not found\n");
	}

	pwm_set_enable(dev, 0, 1);
	mdelay(200);
	pwm_set_enable(dev, 0, 0);
}

int rk_board_late_init(void)
{

	writel(0x00600020, 0xfdd60004);
#if defined(CONFIG_DM_PWM)
	startup_buzz();
#endif
	return 0;
}
