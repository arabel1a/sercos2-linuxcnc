/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */
	#include <zlib.h>
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
 	#include <fcntl.h>
    #include <poll.h>
	#include <time.h>

	#include "sercos-mdt-gen.h"

	#ifndef DEFAULT_IF
		#define DEFAULT_IF			"enp5s2"
	#endif

	#define ETHER_TYPE			0x0800	
	#define ETH_HEADER_SIZE    	14
	
	/*  0 for errors only, 1 for some additional data, 2 for the whole data    */

	
	static int sercos_ethernet_sockfd, sercos_ethernet_sockfd_r;
	static struct sockaddr_ll sercos_ethernet_socket_address;
	static uint8_t my_mac[6];
	
	
void sercos_eth_init(uint8_t* eth_tx_buff, const uint8_t * stm_mac, const uint8_t * pc_mac)
{

	struct ifreq if_idx;
	struct ifreq if_mac;

	char ifName[IFNAMSIZ];
	strcpy(ifName, DEFAULT_IF);

	int  sockopt_r;
    struct ifreq ifopts_r;	/* set promiscuous mode */
	struct ifreq if_ip_r;	/* get ip addr */

    /* Open RAW socket to send on */ 
	if ((sercos_ethernet_sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}


	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, DEFAULT_IF, IFNAMSIZ-1);
	if (ioctl(sercos_ethernet_sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, DEFAULT_IF, IFNAMSIZ-1);
	if (ioctl(sercos_ethernet_sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
   
    memset(&if_ip_r, 0, sizeof(struct ifreq));

    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((sercos_ethernet_sockfd_r = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
        perror("listener: socket");
        return;
    }
	
	/*int flags = fcntl(sercos_ethernet_sockfd_r, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(sercos_ethernet_sockfd_r, F_SETFL, flags);
		*/

    /* not neded cause made in system 
    strncpy(ifopts_r.ifr_name, ifName, IFNAMSIZ-1);
    ioctl(sercos_ethernet_sockfd, SIOCGIFFLAGS, &ifopts_r);
    ifopts_r.ifr_flags |= IFF_PROMISC;
    ioctl(sercos_ethernet_sockfd, SIOCSIFFLAGS, &ifopts_r);
    */
    	/* Index of the network device */
    sercos_ethernet_socket_address.sll_ifindex = if_idx.ifr_ifindex;
        /* Address length*/
    sercos_ethernet_socket_address.sll_halen = ETH_ALEN;
 
    /* Ethernet header */
    for(int i = 0; i < 6; i++){
		/* Destination MAC */
		sercos_ethernet_socket_address.sll_addr[i] = stm_mac[i];
		/* Source MAC      */
		my_mac[i] = pc_mac[i];
		/* ETH header */
		eth_tx_buff[i] =  stm_mac[i];
    }
    /* Ethertype field */
    eth_tx_buff[12] = ETHER_TYPE / 256;
    eth_tx_buff[13] = ETHER_TYPE % 256;


    /* timeout in recieve */
    
 //    struct timeval tv;
	// tv.tv_sec = 0;
	// tv.tv_usec = 1;
 //     if (setsockopt(sercos_ethernet_sockfd_r, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) == -1) {
 //         perror("setsockopt");
 //         close(sercos_ethernet_sockfd_r);
 //         exit(EXIT_FAILURE);
 //     }
	
    

    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(sercos_ethernet_sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt_r, sizeof sockopt_r) == -1) {
        perror("setsockopt");
        close(sercos_ethernet_sockfd);
        exit(EXIT_FAILURE);
    }
    
    /* Bind to device */
    if (setsockopt(sercos_ethernet_sockfd_r, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
		rtapi_print("%d, %d, %d\n", sercos_ethernet_sockfd_r, ifName, IFNAMSIZ-1);
        perror("socket binding to device");
        close(sercos_ethernet_sockfd_r);
        exit(EXIT_FAILURE);
    }
    
    if(verbose >= 1)
			rtapi_print("sercos-ethernet init OK\n STM MAC: \n");
		    for(int i = 0; i < 6; i++)	rtapi_print("%02x:", stm_mac[i]);
		    rtapi_print("\nPC MAC: \n");
   		    for(int i = 0; i < 6; i++)	rtapi_print("%02x:", my_mac[i]);
  		    rtapi_print("\n");


}


void sercos_eth_deinit()
{
    close(sercos_ethernet_sockfd_r);
    close(sercos_ethernet_sockfd);
}


int sercos_eth_receive(uint8_t* buf, size_t size)
{

	/*
	struct pollfd fd;
	int ret;
	size_t bytenum;

	fd.fd = sercos_ethernet_sockfd_r;
	fd.events = POLLIN;
	ret = poll(&fd, 1, 10); // 1 millisecond for timeout
	switch (ret) {
    case -1:
        return -1;
        break;
    case 0:
        return 0; 
        break;
    default:
        bytenum = recv(sercos_ethernet_sockfd_r,buf,size, 0); // get your data
        if (buf[0] == my_mac[0] &&
        buf[1] == my_mac[1] &&
        buf[2] == my_mac[2] &&
        buf[3] == my_mac[3] &&
        buf[4] == my_mac[4] &&
        buf[5] == my_mac[5]) 
    		return bytenum;	
    	return -2;
        break;
}*/
    //struct timespec ts;
	//timespec_get(&ts, TIME_UTC);
	//unsigned long long oldtime = ts.tv_nsec;
	
	int bytenum = recvfrom(sercos_ethernet_sockfd_r, buf, size, MSG_DONTWAIT, NULL, NULL);

	//timespec_get(&ts, TIME_UTC);
	//unsigned long long newtime = ts.tv_nsec;
	//unsigned long long ttttim = abs(newtime - oldtime);
	//ttttim = ttttim > 500000000 ? 1000000000  - ttttim : ttttim;
	//rtapi_print("receiving was %d mks long \n", ttttim);



	if (bytenum <= 6 && bytenum >= 0)
		return 0;
	if (bytenum < 0)
		return -1;
    if (buf[0] == my_mac[0] &&
        buf[1] == my_mac[1] &&
        buf[2] == my_mac[2] &&
        buf[3] == my_mac[3] &&
        buf[4] == my_mac[4] &&
        buf[5] == my_mac[5]) {
    	return bytenum;	

     }


        return 0;
        
}




int sercos_eth_send(uint8_t* sendbuf, int tx_len)
{
	/*   sending   */
	if(tx_len < 60)
		tx_len = 60;
	uint16_t fcs = CRC(sendbuf + 12, tx_len - 12);
	sendbuf[tx_len]	= fcs % 256;
	sendbuf[tx_len + 1]	= (fcs / 256) % 256;
	if (sendto(sercos_ethernet_sockfd, sendbuf, tx_len+2, 0, (struct sockaddr *) &sercos_ethernet_socket_address, sizeof(struct sockaddr_ll)) < 0) {
		rtapi_print("Send failed\n");
		return 1;
	}
	
	if(verbose > 1){
		rtapi_print("Packet send. ");
		rtapi_print("Data:\n");
		for (int i=0; i<tx_len + 2; i++)
			rtapi_print("%02x:", sendbuf[i]);
		rtapi_print("\n");
	}
	return 0;
}

