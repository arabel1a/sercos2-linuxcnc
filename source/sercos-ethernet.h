#ifndef SERCOS_ETHERNET
	#define SERCOS_ETHERNET

 	#include <linux/ip.h>
	#include <linux/udp.h>
	#include <arpa/inet.h>
	#include <linux/if_packet.h>
	#include <sys/ioctl.h>
	#include <sys/socket.h>
	#include <net/if.h>
	#include <netinet/ether.h>
	
	#include <stddef.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>

	void sercos_eth_init(uint8_t* eth_tx_buff, const uint8_t * stm_mac, const uint8_t * pc_mac);
	int sercos_eth_receive(uint8_t* buf, size_t size);
	int sercos_eth_send(uint8_t* sendbuf, int tx_len);	
	void sercos_eth_deinit();

#endif
