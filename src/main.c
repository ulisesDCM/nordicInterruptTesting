/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <drivers/gpio.h>

#define LED0_NODE 	DT_ALIAS(myled)

static const struct gpio_dt_spec myLed = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void main(void)
{
	int ret = 0;

	if(!device_is_ready(myLed.port)){
		return;
	}

	ret = gpio_pin_configure_dt(&myLed, GPIO_OUTPUT_ACTIVE);
	if(ret <0){
		return;
	}


	while(1){
		ret = gpio_pin_toggle_dt(&myLed);
		if(ret <0){
			return;
		}
		printk("hello world\r\n");
		k_msleep(1000);
	}
}
