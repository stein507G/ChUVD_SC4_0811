/*******************************************************************************
 * (c) Copyright 2011-2015 Microsemi SoC Products Group.  All rights reserved.
 *
 * Micrel KSZ8051MNL PHY interface driver implementation.
 *
 * SVN $Revision: 7574 $
 * SVN $Date: 2015-07-22 12:08:12 +0530 (Wed, 22 Jul 2015) $
 */
#include "phy.h"
#include "mss_ethernet_mac_types.h"
#include "mss_ethernet_mac.h"
#include "../../CMSIS/mss_assert.h"

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************************************/
/* Preprocessor Macros                                                    */
/**************************************************************************/

#define BMSR_AUTO_NEGOTIATION_COMPLETE  0x0020u

/***************************************************************************//**
 * Address of the PHY on the MII management interface.
 */
static uint8_t g_phy_addr = 0u;

/**************************************************************************//**
 * 
 */
#define ANEG_REQUESTED          0x80000000u
#define FORCED_CFG_REQUESTED    0x40000000u


/**************************************************************************//**
 * 
 */
void MSS_MAC_phy_init(uint8_t phy_addr)
{
    g_phy_addr = phy_addr;
}

/**************************************************************************//**
 * 
 */
void MSS_MAC_phy_set_link_speed(uint32_t speed_duplex_select)
{
    uint16_t phy_reg;
    uint32_t inc;
    uint32_t speed_select;
    const uint16_t mii_advertise_bits[4] = {ADVERTISE_10FULL, ADVERTISE_10HALF,
                                            ADVERTISE_100FULL, ADVERTISE_100HALF};
    
    /* Set auto-negotiation advertisement. */
    
    /* Set 10Mbps and 100Mbps advertisement. */
    phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_ADVERTISE);
    phy_reg &= ~(ADVERTISE_10HALF | ADVERTISE_10FULL |
                 ADVERTISE_100HALF | ADVERTISE_100FULL);
                 
    speed_select = speed_duplex_select;
    for(inc = 0u; inc < 4u; ++inc)
    {
        uint32_t advertise;
        advertise = speed_select & 0x00000001u;
        if(advertise != 0u)
        {
            phy_reg |= mii_advertise_bits[inc];
        }
        speed_select = speed_select >> 1u;
    }
    
    MSS_MAC_write_phy_reg(g_phy_addr, MII_ADVERTISE, phy_reg);
}

/**************************************************************************//**
 * 
 */
void MSS_MAC_phy_autonegotiate(void)
{
    uint16_t phy_reg;
    uint16_t autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 1000000u;
    
    /* Enable auto-negotiation. */
    phy_reg = 0x1300;
    MSS_MAC_write_phy_reg(g_phy_addr, MII_BMCR, phy_reg);
    
    /* Wait for copper auto-negotiation to complete. */
    do {
        phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_BMSR);
        autoneg_complete = phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE;
        --copper_aneg_timeout;
    } while((!autoneg_complete && (copper_aneg_timeout != 0u)) || (0xFFFF == phy_reg));
}

/**************************************************************************//**
 * 
 */
uint8_t MSS_MAC_phy_get_link_status
(
    mss_mac_speed_t * speed,
    uint8_t *     fullduplex
)
{
    uint16_t phy_reg;
    uint16_t link_up;
    uint8_t link_status;

    phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_BMSR);
    link_up = phy_reg & BMSR_LSTATUS;
    
    if(link_up != MSS_MAC_LINK_DOWN)
    {
        uint16_t op_mode;
        
        /* Link is up. */
        link_status = MSS_MAC_LINK_UP;
        
        phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, 0x1E);
        op_mode = phy_reg & 0x0007u;
        switch(op_mode)
        {
            case 1:
                *fullduplex = MSS_MAC_HALF_DUPLEX;
                *speed = MSS_MAC_10MBPS;
            break;
            
            case 2:
                *fullduplex = MSS_MAC_HALF_DUPLEX;
                *speed = MSS_MAC_100MBPS;
            break;
            
            case 5:
                *fullduplex = MSS_MAC_FULL_DUPLEX;
                *speed = MSS_MAC_10MBPS;
            break;
            
            case 6:
                *fullduplex = MSS_MAC_FULL_DUPLEX;
                *speed = MSS_MAC_100MBPS;
            break;
            
            default:
                link_status = MSS_MAC_LINK_DOWN;
            break;
        }
    }
    else
    {
        /* Link is down. */
        link_status = MSS_MAC_LINK_DOWN;
    }
    
    return link_status;
}

#ifdef __cplusplus
}
#endif

/******************************** END OF FILE ******************************/






