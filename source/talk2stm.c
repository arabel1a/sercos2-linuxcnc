/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */
/*  0 for errors only, 1 for some additional data, 2 for the whole data    */


#include "talk2stm.h"

static uint8_t syncro_file[500000];
static size_t syncro_file_length;
static size_t syncro_file_position;
static uint8_t stable_tx_msg[5000];
static size_t stable_tx_msg_len; 
static size_t total_syncro_messages, loaded_syncro_messages;
static uint8_t msg_size;
static uint8_t sercos_sync_stage;

bool SERCOS_SYNC_DONE = false;
bool SERCOS_SYNC_DONE_prev = false;

unsigned long long timestamp_request = 0, timestamp_response = 0;

int stm_talker_init(FILE * sync_file){
	fread(&syncro_file, 500000, 1, sync_file);  
	total_syncro_messages = syncro_file[0]*256 + syncro_file[1];
	if(verbose > 0) rtapi_print("syncronization file consists of %d messages \n", total_syncro_messages );
	loaded_syncro_messages = 0;
    syncro_file_position = 2;
    msg_size = syncro_file[syncro_file_position];
    syncro_file_position++;
    sercos_sync_stage = 0;
    SERCOS_SYNC_DONE = false;
	SERCOS_SYNC_DONE_prev = false;

}

void stm_talker_reset(){
	loaded_syncro_messages = 0;
    syncro_file_position = 2;
    msg_size = syncro_file[syncro_file_position];
    syncro_file_position++;
    sercos_sync_stage = 0;
    SERCOS_SYNC_DONE = false;
    SERCOS_SYNC_DONE_prev = false;

}


int analysis(int DriveNum, SercosDrive drives[], uint8_t * eth_rx_buf, size_t eth_rx_len, uint8_t * eth_tx_buf, size_t eth_tx_len , long long eth_total_send, long long eth_total_recieved)
{
	/*    check if message is for me   */
	if(eth_rx_len == 0)
		return PASS;
		
	/*    Parsing the message         */
		eth_total_recieved++;

	/* Print packet */
	if(verbose >= 2)
	{ 
		rtapi_print("received essage # %ld \n", eth_total_recieved);
		rtapi_print("Data: \n\t");
		for (int i=0; i<eth_rx_len; i++)
			rtapi_print("%02x:", eth_rx_buf[i]);
		rtapi_print("\n");
    }
        
        
    /*   unparsible packet   */
    if( eth_rx_buf[14] != 0x33 && 
        eth_rx_buf[14] != 0x22 && 
        eth_rx_buf[14] != 0x44 && 
        eth_rx_buf[14] != 0x55 && 
        eth_rx_buf[14] != 0x66 &&
        eth_rx_buf[14] != 0xff &&
        eth_rx_buf[14] != 0xaa &&
        eth_rx_buf[14] != 0xbb)
    {
		if(verbose >= 1){
			rtapi_print("ALLERT! Got bad package # %d\n", eth_total_recieved);
			for (int i=0; i<eth_rx_len; i++) rtapi_print("%02x:", eth_rx_buf[i]);            
		}
		return UNKNOWN;
    }
        
    /*   corrupted packages   */    
    if(eth_rx_buf[14] == 0x33) 
	{
	    
		
		if(verbose >0 && eth_rx_buf[18]!= (eth_total_send-1)%256){
			rtapi_print("send package %d got back %d\n", (eth_total_send-1)%256, eth_rx_buf[18]);
			return CORRUPTED;
		}
		
		for(int j=14; j < eth_tx_len; j++)
			if(verbose >= 2 && eth_tx_buf[j]!= eth_rx_buf[j])
			{
				rtapi_print("%d, %d, %d",verbose, eth_tx_buf[j], eth_rx_buf[j]);
				rtapi_print("Corrupted package! Send: \n");
				for (int i=0; i<eth_tx_len; i++) 
					rtapi_print("%02x:", eth_tx_buf[i]);
				rtapi_print("\n Got back: \n");
				for (int i=0; i<eth_rx_len; i++) 
					rtapi_print("%02x:", eth_rx_buf[i]);
				rtapi_print("\n");
				return CORRUPTED;
			}
		return PASS;

	}
	/*		Quick drive's AT	 */
	if(eth_rx_buf[14] == 0xff)
		return AT;

	/*		Quick drive's crc mismatch	 */
	if(eth_rx_buf[14] == 0xaa)
		return CRC_MISMATCH;

	if(eth_rx_buf[14] == 0xbb)
		return TIMESTAMP;
        
    /*     Answer from drives     */
    if(	eth_rx_buf[14] == 0x44 ) 
    {
    	if(eth_rx_len < 269) return CORRUPTED;
		timestamp_response = (uint32_t)eth_rx_buf[265] + (uint32_t)eth_rx_buf[266]*256 + (uint32_t)eth_rx_buf[267]*256*256 + (uint32_t)eth_rx_buf[268]*256*256*256;

		if( SERCOS_SYNC_DONE){
			int error = sercos_dt_read(DriveNum, drives, eth_rx_buf+15, 250);
			error = sercos_dt_read(DriveNum, drives, eth_rx_buf+15, 250);

			if(error != 0){
			//rtapi_print("RAW DT: %d\n", eth_rx_len);
			//for (int i=0; i<eth_rx_len; i++) 
			//	rtapi_print("%02x:", eth_rx_buf[i]);
			//rtapi_print("\n");
				return MISS_TG;
			}
			return OK;
		} 
		return PASS;
    }
               
    /*    STM required restarting  */
    if(eth_rx_buf[14] == 0x55) 
    {
		if(verbose > 0) rtapi_print("\n Restart request achieved, restarting \n");
       	return RESTART;
    }
    

    /*    STM reported an error    */    
    if(eth_rx_buf[14] == 0x66) 
    {
		if( verbose > 0 ){
			rtapi_print(" STM found an error: ");
       	}
       	switch(eth_rx_buf[15]){
        	case 1:
				if( verbose > 0 ){
					rtapi_print("unknown message\n");
				}
				if( verbose > 1){
					for (int i=0; i<eth_rx_len; i++) 
						rtapi_print("%02x:", eth_rx_buf[i]);
					rtapi_print("\n");
				}
				return UNKNOWN;
			case 2:
				if( verbose > 0 ){
					rtapi_print("bad package number\n");
				}
			   	if( verbose > 1){
					for (int i=0; i<eth_rx_len; i++) 
						rtapi_print("%02x:", eth_rx_buf[i]);
					rtapi_print("\n");
				}
				return CORRUPTED;
       	}
    }

    if(eth_rx_buf[14] == 0x22){
		timestamp_request = (int)eth_rx_buf[15] + (int)eth_rx_buf[16]*256 + (int)eth_rx_buf[17]*256*256 + (int)eth_rx_buf[18]*256*256*256;
       	return REQUEST;
    }

    return UNKNOWN;
}


size_t synthesis(int DriveNum, SercosDrive drives[], uint8_t * eth_tx_buf, long long eth_total_send){
	if(SERCOS_SYNC_DONE)
	{
		memset(eth_tx_buf, 0, BUF_SIZE);
		memcpy(eth_tx_buf, stable_tx_msg, stable_tx_msg_len);				
		eth_tx_buf[26] = sercos_mdt_gen( DriveNum, drives, eth_tx_buf + 27);
		eth_tx_buf[26] = sercos_mdt_gen( DriveNum, drives, eth_tx_buf + 27);

		
		/*
		if( drives[2].idn_out[0] <0 ){
			rtapi_print("len: %d\n",eth_tx_buf[26]);
			for(int i = 0; i < eth_tx_buf[26]; i++) 
			{
				rtapi_print("%02x:", eth_tx_buf[27 + i]);
			}
			rtapi_print("\n");
		}
		*/
		eth_tx_buf[16] = 34;          			    // delete this ??
		eth_tx_buf[18] = eth_total_send%256;		// packet num
				
		return stable_tx_msg_len + eth_tx_buf[26] - 26;
	}
	
	/*    Syncronization from file    */	
	size_t tx_len = ETH_HEADER_SIZE;
    size_t pload_len = 0;
    uint8_t sync_phaze = 0, bitlen = 0;
    uint8_t eth_tx_playload[MAX_MESSAGE_SIZE];
    memset( eth_tx_buf + tx_len, 0, BUF_SIZE - tx_len );
    
    /*    PLAYLOAD     */
    /*    TODO:
    	make it in one action, just change generation code
    */
    loaded_syncro_messages++;
    memset(eth_tx_playload, 0, MAX_MESSAGE_SIZE);

    eth_tx_playload[pload_len] = 0x33;
    pload_len++;
    
    bitlen = syncro_file[syncro_file_position];
    syncro_file_position++;
    eth_tx_playload[pload_len] = bitlen;
    pload_len++;

    sercos_sync_stage = syncro_file[syncro_file_position];
    syncro_file_position++;
    eth_tx_playload[pload_len] = sercos_sync_stage;
    pload_len++;
    
    eth_tx_playload[pload_len] = msg_size;
    pload_len++;
    eth_tx_playload[pload_len] = eth_total_send%256;
    pload_len++;    
    memcpy(eth_tx_playload + pload_len, syncro_file + syncro_file_position, msg_size);
    syncro_file_position+=msg_size;
    pload_len += msg_size;
    msg_size = syncro_file[syncro_file_position];
    syncro_file_position++;
    if(msg_size > 8)
    {
		loaded_syncro_messages++;
        eth_tx_playload[pload_len] = msg_size;
        pload_len ++;
        memcpy(eth_tx_playload + pload_len, syncro_file + syncro_file_position, msg_size);
		syncro_file_position += msg_size;
        pload_len += msg_size;
        msg_size = syncro_file[syncro_file_position];
		syncro_file_position++;

    }
    
    memcpy(eth_tx_buf + tx_len, eth_tx_playload, pload_len);
    tx_len +=pload_len; 


    /*    switch to my message generation      */
	if( loaded_syncro_messages >= total_syncro_messages - 100 )
	{
			if(verbose > 0) {
				rtapi_print("Stable message:\n");
			}

			/// debug

	/*    replace with constant message ?? or generated with msg    */

			memcpy( stable_tx_msg, eth_tx_buf, tx_len);

			stable_tx_msg_len = tx_len;
			if(verbose >= 1){
				for (int i=0; i<tx_len; i++) 
					rtapi_print("%02x:", stable_tx_msg[i]);		
				rtapi_print("\n"); 
			}

			if(verbose > 0) rtapi_print("Syncronization OK\n");
			SERCOS_SYNC_DONE = true;

	}
    return tx_len;
}
