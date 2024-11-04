/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <nrfs_backend_ipc_service.h>
#include <nrfs_gdpwr.h>

/* Required to power off the GD2 and GD3 domains
 * Will be removed when GD handling
 * is implemented in sdk-zephyr
 */
static void gdpwr_handler(nrfs_gdpwr_evt_t const *p_evt, void *context)
{
	switch (p_evt->type) {
	case NRFS_GDPWR_REQ_APPLIED:
		printk("GDPWR handler - response received: 0x%x, CTX=%d\n", p_evt->type,
		       (uint32_t)context);
		break;
	case NRFS_GDPWR_REQ_REJECTED:
		printk("GDPWR handler - request rejected: 0x%x, CTX=%d\n", p_evt->type,
		       (uint32_t)context);
		break;
	default:
		printk("GDPWR handler - unexpected event: 0x%x, CTX=%d\n", p_evt->type,
		       (uint32_t)context);
		break;
	}
}

/* Required to power off the GD2 and GD3 domains
 * Will be removed when GD handling
 * is implemented in sdk-zephyr
 */
static void clear_global_power_domains_requests(void)
{
	int service_status;
	int tst_ctx = 1;

	service_status = nrfs_gdpwr_init(gdpwr_handler);
	printk("Response: %d\n", service_status);
	printk("Sending GDPWR DISABLE request for: GDPWR_POWER_DOMAIN_ACTIVE_SLOW\n");
	service_status = nrfs_gdpwr_power_request(GDPWR_POWER_DOMAIN_ACTIVE_SLOW,
						  GDPWR_POWER_REQUEST_CLEAR, (void *)tst_ctx++);
	printk("Response: %d\n", service_status);
	printk("Sending GDPWR DISABLE request for: GDPWR_POWER_DOMAIN_ACTIVE_FAST\n");
	service_status = nrfs_gdpwr_power_request(GDPWR_POWER_DOMAIN_ACTIVE_FAST,
						  GDPWR_POWER_REQUEST_CLEAR, (void *)tst_ctx++);
	printk("Response: %d\n", service_status);
	printk("Sending GDPWR DISABLE request for: GDPWR_POWER_DOMAIN_MAIN_SLOW\n");
	service_status = nrfs_gdpwr_power_request(GDPWR_POWER_DOMAIN_MAIN_SLOW,
						  GDPWR_POWER_REQUEST_CLEAR, (void *)tst_ctx);
	printk("Response: %d\n", service_status);
}

int main(void)
{
	nrfs_backend_wait_for_connection(K_FOREVER);
	clear_global_power_domains_requests();
	k_sleep(K_FOREVER);

	return 0;
}
