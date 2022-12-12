/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/zephyr.h>

void main(void)
{
	while(1)
	{
		printk("hello world\r\n");
		k_msleep(100);
	}
}
