/*
 * Copyright (c) 2021 Telink Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rail.h"

#ifndef ZEPHYR_DRIVERS_IEEE802154_IEEE802154_ERF32_H
#define ZEPHYR_DRIVERS_IEEE802154_IEEE802154_ERF32_H

/* RAIL config defines */
#define TX_FIFO_SIZE  (128)  // Any power of 2 from [64, 4096] on the EFR32

/* RAIL Power Amplifier defines */

/* Address filter index define */
#define DEFAULT_ADDRESS_INDEX (0)


#define ERF32_IEEE_ADDRESS_SIZE					(8u)

/* Generic */
#define ERF32_RX_BUFFER_LENGTH					(256u)
#define ERF32_TX_BUFFER_LENGTH					(256u)
#define IEEE802154_MAX_LENGTH 					(256u)
#define EFR32_FCS_LENGTH	  					(2u)

/* TX power lookup table */

/* RAIL macro functions */
#define DBM_TO_DECI_DBM(n) (n*10u)

/* ieee802154 RAIL enums */
typedef enum e_efr32_state
{
    EFR32_STATE_RX,
    EFR32_STATE_TX,
    EFR32_STATE_DISABLED,
    EFR32_STATE_CCA,
    EFR32_STATE_SLEEP,
} efr32_state;

/* data structure */
struct erf32_data {
	struct  net_if *iface;
	uint8_t mac_addr[ERF32_IEEE_ADDRESS_SIZE];
	
	RAIL_Handle_t rail_handle;
	RAIL_RxPacketHandle_t packet_handle;

	uint8_t rx_buffer[ERF32_RX_BUFFER_LENGTH];
	uint8_t tx_buffer[ERF32_TX_BUFFER_LENGTH];

    uint16_t rx_buffer_size;


	K_THREAD_STACK_MEMBER(rx_stack, CONFIG_IEEE802154_EFR32_RX_STACK_SIZE);
    struct k_thread rx_thread;
	
	/* CCA complete sempahore. Unlocked when CCA is complete. */
    struct k_sem cca_wait;
    /* RX synchronization semaphore. Unlocked when frame has been
	 * received.
	 */
    struct k_sem rx_wait;
    /* TX synchronization semaphore. Unlocked when frame has been
	 * sent or CCA failed.
	 */
    struct k_sem tx_wait;

    /* TX result. Set to 1 on success, 0 otherwise. */
    bool tx_success;

    uint8_t channel;
};

#endif
