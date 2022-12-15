/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <drivers/gpio.h>

/* BTN Nodes */
#define BTN0_NODE		DT_ALIAS(button0)
#define BTN1_NODE		DT_ALIAS(button1)
#define BTN2_NODE		DT_ALIAS(button2)
#define BTN3_NODE		DT_ALIAS(button3)

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

/* BTN0 & BTN1 callback data structure */
static struct gpio_callback btn2_cb_data;
static struct gpio_callback btn3_cb_data;

void btn2_callback_isr(const struct device *dev, struct gpio_callback *cb,
											 gpio_port_pins_t pins)
{
	printk("button 2 falling edge ISR callback called\r\n");
}

void btn3_callback_isr(const struct device *dev, struct gpio_callback *cb,
											 gpio_port_pins_t pins)
{
	printk("button 3 falling edge ISR callback called\r\n");
}

/**
 * @brief Function to intialize the user LEDs  
 * 
 * @return int 
 */
static int usr_led_init(void)
{
	int ret = 0;
	
	/* No sure if this is the better way... */
	device_is_ready(&led1);
	device_is_ready(&led2);
	device_is_ready(&led3);
	device_is_ready(&led4);

	/* Configure GPIO pins as single outputs */
	ret = gpio_pin_configure_dt(&led1,  GPIO_OUTPUT | GPIO_OUTPUT_LOW);
	ret |= gpio_pin_configure_dt(&led2, GPIO_OUTPUT | GPIO_OUTPUT_LOW);
	ret |= gpio_pin_configure_dt(&led3, GPIO_OUTPUT | GPIO_OUTPUT_LOW);
	ret |= gpio_pin_configure_dt(&led4, GPIO_OUTPUT | GPIO_OUTPUT_LOW);

	return ret;
}

/**
 * @brief Function to initialize the user buttons btn0 &btn1 as single input
 * and btn2 & btn3 with interrupt of falling and rising edge respetively 
 * 
 * @return int 
 */
static int user_btn_init(void)
{
	int ret = 0;

	/* No sure if this is the better way... */

	/* Configure GPIO pins as singles inputs without interrupt */
	ret = gpio_pin_configure_dt(&btn0,  GPIO_INPUT| GPIO_PULL_UP );
	ret |= gpio_pin_configure_dt(&btn1, GPIO_INPUT| GPIO_PULL_UP );
	ret |= gpio_pin_configure_dt(&btn2, GPIO_INPUT| GPIO_PULL_UP );
	ret |= gpio_pin_configure_dt(&btn3, GPIO_INPUT| GPIO_PULL_UP );

	/* Configure GPIO pins as singles inputs with interrupt */
	ret |= gpio_pin_interrupt_configure_dt(&btn2, GPIO_INT_EDGE_FALLING);
	ret |= gpio_pin_interrupt_configure_dt(&btn3, GPIO_INT_EDGE_RISING);

	/* btn0 and btn1 Callback initialization  */
	gpio_init_callback(&btn2_cb_data, btn2_callback_isr, BIT(btn2.pin));
	gpio_init_callback(&btn3_cb_data, btn3_callback_isr, BIT(btn3.pin));

	/* Add the callback applications */
	ret |= gpio_add_callback(btn2.port, &btn2_cb_data);
	ret |= gpio_add_callback(btn3.port, &btn3_cb_data);

	return ret;
}

void main(void)
{
	/* Configure the user leds */
	if(usr_led_init())
	{
		return;	//Error
	}

	/* Configure the user buttons */
	if(user_btn_init())
	{
		return; //Error
	}

	/* Main loop program */
	int btn0State = 0;
	int loopCounter = 0;
	int evenFlag = 0;

	while(1)
	{
		if(evenFlag % 2)
			printk("************************\r\n");

		printk("Main Loop counter is %d\r\n",loopCounter);
		printk("Press button 0\r\n");
		btn0State = gpio_pin_get_dt(&btn0);
		printk("Button 0 state is %d\r\n",btn0State);

		gpio_pin_toggle_dt(&led1);
		gpio_pin_toggle_dt(&led2);
		gpio_pin_toggle_dt(&led3);
		gpio_pin_toggle_dt(&led4);

		evenFlag++;
		loopCounter++;
		k_msleep(500);
	}
}
