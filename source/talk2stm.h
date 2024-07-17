#ifndef TALKIN2STM
	#include <stddef.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <string.h>
	#include <stdio.h>
	#ifndef MAX_MESSAGE_SIZE
		#define MAX_MESSAGE_SIZE 500
	#endif

	#define TALKIN2STM
	#define ETH_HEADER_SIZE    	14
	#define RESTART 		   	1
	#define OK				   	2
	#define PASS			   	4
	#define CORRUPTED   	   	8
	#define UNKNOWN     	   	16
	#define MISS_TG				32
	#define REQUEST				64
	#define AT                  128
	#define CRC_MISMATCH		256
	#define TIMESTAMP			512
	
	#ifndef BUF_SIZE
		#define BUF_SIZE		 1024
	#endif

	#include "sercos-mdt-gen.h"
	
	int stm_talker_init(FILE * sync_file);
	int analysis(int DriveNum, SercosDrive drives[], uint8_t * eth_rx_buf, size_t eth_rx_len, uint8_t * eth_tx_buf, size_t eth_tx_len , long long eth_total_send, long long eth_total_recieved);
	size_t synthesis(int DriveNum, SercosDrive drives[], uint8_t * eth_tx_buf, long long eth_total_send);
	void stm_talker_reset(void);

	
#endif
