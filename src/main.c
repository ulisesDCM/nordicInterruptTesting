/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <drivers/gpio.h>

/* BTN Nodes */
#define BTN0_NODE		DT_ALIAS(btn0)
#define BTN1_NODE		DT_ALIAS(btn1)
#define BTN2_NODE		DT_ALIAS(btn2)
#define BTN3_NODE		DT_ALIAS(btn3)

/* LED Nodes */
#define LED0_NODE 	DT_ALIAS(led0)
#define LED1_NODE 	DT_ALIAS(led1)
#define LED2_NODE 	DT_ALIAS(led2)
#define LED3_NODE 	DT_ALIAS(led3)

/* User LED device structures */
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led4 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

/* User buttons device structures  */
static const struct gpio_dt_spec btn0 = GPIO_DT_SPEC_GET(BTN0_NODE, gpios);
static const struct gpio_dt_spec btn1 = GPIO_DT_SPEC_GET(BTN1_NODE, gpios);
static const struct gpio_dt_spec btn2 = GPIO_DT_SPEC_GET(BTN2_NODE, gpios);
static const struct gpio_dt_spec btn3 = GPIO_DT_SPEC_GET(BTN3_NODE, gpios);


static int usr_led_init(void)
{
	int ret = 0;
	
	//No sure about this... 
	device_is_ready(&led1);
	device_is_ready(&led2);
	device_is_ready(&led3);
	device_is_ready(&led4);
	//Test communication
	//Configure GPIO pins as single outputs
	ret = gpio_pin_configure_dt(&led1,  GPIO_OUTPUT | GPIO_OUTPUT_LOW);
	ret |= gpio_pin_configure_dt(&led2, GPIO_OUTPUT | GPIO_OUTPUT_LOW);
	ret |= gpio_pin_configure_dt(&led3, GPIO_OUTPUT | GPIO_OUTPUT_LOW);
	ret |= gpio_pin_configure_dt(&led4, GPIO_OUTPUT | GPIO_OUTPUT_LOW);

	return ret;
}

static int user_btn_init(void)
{

}

void main(void)
{

	if(usr_led_init())
	{
		return;	//Error
	}

	while(1)
	{
		printk("hello world\r\n");
		gpio_pin_toggle_dt(&led1);
		gpio_pin_toggle_dt(&led2);
		gpio_pin_toggle_dt(&led3);
		gpio_pin_toggle_dt(&led4);
		k_msleep(1000);
	}
}
