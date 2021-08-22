#include "m2sxxx.h"
#include "conf_eth.h"
#include "UV_define.h"

const uint8_t * sys_cfg_get_mac_address(void)
{
	uint8_t SW1_Buff;
	uint8_t SW2_Buff;

	SW1_Buff = SW1_READ;
	SW2_Buff = SW2_READ;

    static uint8_t mac_address[6];
    
    mac_address[0] = ETHERNET_CONF_ETHADDR0;
    mac_address[1] = ETHERNET_CONF_ETHADDR1;
    mac_address[2] = ETHERNET_CONF_ETHADDR2;
    mac_address[3] = ETHERNET_CONF_ETHADDR3;
    mac_address[4] = ETHERNET_CONF_ETHADDR4;
    //mac_address[5] = ETHERNET_CONF_ETHADDR5;

	mac_address[5] = ETHERNET_CONF_ETHADDR5 + (SW1_Buff + (SW2_Buff << 1));	//(-/+) 210531 CJKIM, 20 -> ETHERNET_CONF_IPADDR3,  기존값 동일

	
    return (const uint8_t *)mac_address;
}
