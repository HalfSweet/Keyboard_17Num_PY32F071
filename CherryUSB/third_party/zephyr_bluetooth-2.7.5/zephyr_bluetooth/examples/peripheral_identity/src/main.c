/* main.c - Application main entry point */

/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

int init_peripheral(void);

void demo_peripheral_identity(void)
{
	(void)init_peripheral();
}
