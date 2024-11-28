#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_conf.h"
#include <string.h>
#include "lwip/netif.h"

extern ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */
extern ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */ 
extern uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */
extern uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;

err_t ethernetif_init(struct netif *netif);
err_t ethernetif_input(struct netif *netif);

#endif

