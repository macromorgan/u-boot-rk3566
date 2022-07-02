// SPDX-License-Identifier: GPL-2.0+

#include <asm/io.h>

/* Start LED very early so user knows device is on.*/
void led_setup(void)
{
	writel(0xffff0020, 0xfdd6000c);
	writel(0xffff0020, 0xfdd60004);
}
