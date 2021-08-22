/*
 * ksz8051mnl_phy.h
 *
 *  Created on: 2017. 1. 21.
 *      Author: Han
 */

#ifndef KSZ8051MNL_PHY_H_
#define KSZ8051MNL_PHY_H_

void MSS_MAC_phy_init(uint8_t phy_addr);
void MSS_MAC_phy_set_link_speed(uint32_t speed_duplex_select);
void MSS_MAC_phy_autonegotiate(void);
uint8_t MSS_MAC_phy_get_link_status(mss_mac_speed_t * speed, uint8_t * fullduplex);


#endif /* KSZ8051MNL_PHY_H_ */
