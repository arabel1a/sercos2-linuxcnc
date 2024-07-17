/* Autogenerated by /home/tc2000r/linuxcnc-dev/bin/halcompile on Tue Jul 25 09:06:51 2023 -- do not edit */
#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"

static int comp_id;

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "component:sercos:Real-time Sercos II component");
MODULE_INFO(linuxcnc, "pin:reset:bit:0:in:Restart session (including syncronozong):None:None");
MODULE_INFO(linuxcnc, "pin:ControlWord-#:u32:4:in:Sercos drive status word:None:None");
MODULE_INFO(linuxcnc, "pin:drive-0-out-#:s32:1:in::None:None");
MODULE_INFO(linuxcnc, "pin:drive-1-out-#:s32:1:in::None:None");
MODULE_INFO(linuxcnc, "pin:drive-2-out-#:s32:1:in::None:None");
MODULE_INFO(linuxcnc, "pin:drive-3-out-#:s32:1:in::None:None");
MODULE_INFO(linuxcnc, "pin:StatusWord-#:u32:4:out:Sercos drive status word:None:None");
MODULE_INFO(linuxcnc, "pin:error_count:u32:0:out:Number of bad cycles since last syncronization:None:None");
MODULE_INFO(linuxcnc, "pin:error_per_sec:u32:0:out:Number of bad cycles during last 1 second:None:None");
MODULE_INFO(linuxcnc, "pin:cycle_count:u32:0:out:Number of sercos ii cycles modulo 2^32:None:None");
MODULE_INFO(linuxcnc, "pin:debug:u32:0:out::None:None");
MODULE_INFO(linuxcnc, "pin:sync_done:bit:0:out:Sercos sync done flag:None:None");
MODULE_INFO(linuxcnc, "pin:rx_ok-#:bit:4:out::None:None");
MODULE_INFO(linuxcnc, "pin:drive-0-in-#:s32:3:out::None:None");
MODULE_INFO(linuxcnc, "pin:drive-1-in-#:s32:3:out::None:None");
MODULE_INFO(linuxcnc, "pin:drive-2-in-#:s32:3:out::None:None");
MODULE_INFO(linuxcnc, "pin:drive-3-in-#:s32:3:out::None:None");
MODULE_INFO(linuxcnc, "param:cycle_read_expected_shift:s32:0:rw:kek:700:None");
MODULE_INFO(linuxcnc, "param:drive_at_watchdog:u32:0:r:How many mks i should wait for drive's anknowlenge telegramm before sending again:100:None");
MODULE_INFO(linuxcnc, "param:verb:u32:0:rw:verbosity:0:None");
MODULE_INFO(linuxcnc, "license:GPL");
MODULE_INFO(linuxcnc, "funct:_:0:");
MODULE_INFO(linuxcnc, "descr:\n Raw, untested and not accurate implementation of Sercos II protocol. \n Hovewer, it works. Please be careful using it.\n\n How to use this module:\n \n\t 1. You need a hardware device, connected to SERCOS II fiber ring. I used STM32F767ZI\n\t evaluation board. Unfortunately, i can not share it's code because of my contract \n\t limitations, but it's pretty simple. This module sends messages which are ready to \n\t be send to fiber, and hardware should operate this way every sercos cycle: \n\t \t* Request a message from this this module \n\t \t* Send a quick response after this request is satisfied\n\t \t* Send feedback from drives to this module\n\tYou can contact me via Telegramm @arabel1a or via github for more information.\n\n\t2. You need a captured syncronization stage from working sercos master. Sercos II \n\tis a proprietary protocol, and everyhing in this module was get by reverse-engineering.\n\tI haven't enought time to explore it's service channel, so syncronization cycles are just\n\tcaptured, analyzed, and synthezed intro messages. I used FC7501 pci card from bechoff \n\tand $10 logic analyzer for capturing. Analyzing and preparing script comes with this package.\n\n\t3. You need to configure it for yourself manually \n\t....\n\t//TODO//\n\npersonality = drive number;\n\n");
MODULE_LICENSE("GPL");
#endif // MODULE_INFO


struct __comp_state {
    struct __comp_state *_next;
    hal_bit_t *reset_p;
    hal_u32_t *ControlWord_p[4];
    hal_s32_t *drive_0_out_p[1];
    hal_s32_t *drive_1_out_p[1];
    hal_s32_t *drive_2_out_p[1];
    hal_s32_t *drive_3_out_p[1];
    hal_u32_t *StatusWord_p[4];
    hal_u32_t *error_count_p;
    hal_u32_t *error_per_sec_p;
    hal_u32_t *cycle_count_p;
    hal_u32_t *debug_p;
    hal_bit_t *sync_done_p;
    hal_bit_t *rx_ok_p[4];
    hal_s32_t *drive_0_in_p[3];
    hal_s32_t *drive_1_in_p[3];
    hal_s32_t *drive_2_in_p[3];
    hal_s32_t *drive_3_in_p[3];
    hal_s32_t cycle_read_expected_shift_p;
    hal_u32_t drive_at_watchdog_p;
    hal_u32_t verb_p;
};
struct __comp_state *__comp_first_inst=0, *__comp_last_inst=0;

static void _(struct __comp_state *__comp_inst, long period);
static int __comp_get_data_size(void);
static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg);
#undef TRUE
#define TRUE (1)
#undef FALSE
#define FALSE (0)
#undef true
#define true (1)
#undef false
#define false (0)

static int export(char *prefix, long extra_arg) {
    char buf[HAL_NAME_LEN + 1];
    int r = 0;
    int j = 0;
    int sz = sizeof(struct __comp_state) + __comp_get_data_size();
    struct __comp_state *inst = hal_malloc(sz);
    memset(inst, 0, sz);
    r = extra_setup(inst, prefix, extra_arg);
    if(r != 0) return r;
    r = hal_pin_bit_newf(HAL_IN, &(inst->reset_p), comp_id,
        "%s.reset", prefix);
    if(r != 0) return r;
    for(j=0; j < (4); j++) {
        r = hal_pin_u32_newf(HAL_IN, &(inst->ControlWord_p[j]), comp_id,
            "%s.ControlWord-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (1); j++) {
        r = hal_pin_s32_newf(HAL_IN, &(inst->drive_0_out_p[j]), comp_id,
            "%s.drive-0-out-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (1); j++) {
        r = hal_pin_s32_newf(HAL_IN, &(inst->drive_1_out_p[j]), comp_id,
            "%s.drive-1-out-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (1); j++) {
        r = hal_pin_s32_newf(HAL_IN, &(inst->drive_2_out_p[j]), comp_id,
            "%s.drive-2-out-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (1); j++) {
        r = hal_pin_s32_newf(HAL_IN, &(inst->drive_3_out_p[j]), comp_id,
            "%s.drive-3-out-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (4); j++) {
        r = hal_pin_u32_newf(HAL_OUT, &(inst->StatusWord_p[j]), comp_id,
            "%s.StatusWord-%01d", prefix, j);
        if(r != 0) return r;
    }
    r = hal_pin_u32_newf(HAL_OUT, &(inst->error_count_p), comp_id,
        "%s.error-count", prefix);
    if(r != 0) return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->error_per_sec_p), comp_id,
        "%s.error-per-sec", prefix);
    if(r != 0) return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->cycle_count_p), comp_id,
        "%s.cycle-count", prefix);
    if(r != 0) return r;
    r = hal_pin_u32_newf(HAL_OUT, &(inst->debug_p), comp_id,
        "%s.debug", prefix);
    if(r != 0) return r;
    r = hal_pin_bit_newf(HAL_OUT, &(inst->sync_done_p), comp_id,
        "%s.sync-done", prefix);
    if(r != 0) return r;
    for(j=0; j < (4); j++) {
        r = hal_pin_bit_newf(HAL_OUT, &(inst->rx_ok_p[j]), comp_id,
            "%s.rx-ok-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (3); j++) {
        r = hal_pin_s32_newf(HAL_OUT, &(inst->drive_0_in_p[j]), comp_id,
            "%s.drive-0-in-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (3); j++) {
        r = hal_pin_s32_newf(HAL_OUT, &(inst->drive_1_in_p[j]), comp_id,
            "%s.drive-1-in-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (3); j++) {
        r = hal_pin_s32_newf(HAL_OUT, &(inst->drive_2_in_p[j]), comp_id,
            "%s.drive-2-in-%01d", prefix, j);
        if(r != 0) return r;
    }
    for(j=0; j < (3); j++) {
        r = hal_pin_s32_newf(HAL_OUT, &(inst->drive_3_in_p[j]), comp_id,
            "%s.drive-3-in-%01d", prefix, j);
        if(r != 0) return r;
    }
    r = hal_param_s32_newf(HAL_RW, &(inst->cycle_read_expected_shift_p), comp_id,
        "%s.cycle-read-expected-shift", prefix);
    inst->cycle_read_expected_shift_p = 700;
    if(r != 0) return r;
    r = hal_param_u32_newf(HAL_RO, &(inst->drive_at_watchdog_p), comp_id,
        "%s.drive-at-watchdog", prefix);
    inst->drive_at_watchdog_p = 100;
    if(r != 0) return r;
    r = hal_param_u32_newf(HAL_RW, &(inst->verb_p), comp_id,
        "%s.verb", prefix);
    inst->verb_p = 0;
    if(r != 0) return r;
    rtapi_snprintf(buf, sizeof(buf), "%s", prefix);
    r = hal_export_funct(buf, (void(*)(void *inst, long))_, inst, 0, 0, comp_id);
    if(r != 0) return r;
    if(__comp_last_inst) __comp_last_inst->_next = inst;
    __comp_last_inst = inst;
    if(!__comp_first_inst) __comp_first_inst = inst;
    return 0;
}
int rtapi_app_main(void) {
    int r = 0;
    comp_id = hal_init("sercos");
    if(comp_id < 0) return comp_id;
    r = export("sercos", 0);
    if(r) {
        hal_exit(comp_id);
    } else {
        hal_ready(comp_id);
    }
    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_id);
}

#undef FUNCTION
#define FUNCTION(name) static void name(struct __comp_state *__comp_inst, long period)
#undef EXTRA_SETUP
#define EXTRA_SETUP() static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg)
#undef EXTRA_CLEANUP
#define EXTRA_CLEANUP() static void extra_cleanup(void)
#undef fperiod
#define fperiod (period * 1e-9)
#undef reset
#undef reset_ptr
#define reset_ptr (__comp_inst->reset_p)
#define reset (0+*__comp_inst->reset_p)
#undef ControlWord
#undef ControlWord_ptr
#define ControlWord_ptr(i) (__comp_inst->ControlWord_p[i])
#define ControlWord(i) (0+*(__comp_inst->ControlWord_p[i]))
#undef drive_0_out
#undef drive_0_out_ptr
#define drive_0_out_ptr(i) (__comp_inst->drive_0_out_p[i])
#define drive_0_out(i) (0+*(__comp_inst->drive_0_out_p[i]))
#undef drive_1_out
#undef drive_1_out_ptr
#define drive_1_out_ptr(i) (__comp_inst->drive_1_out_p[i])
#define drive_1_out(i) (0+*(__comp_inst->drive_1_out_p[i]))
#undef drive_2_out
#undef drive_2_out_ptr
#define drive_2_out_ptr(i) (__comp_inst->drive_2_out_p[i])
#define drive_2_out(i) (0+*(__comp_inst->drive_2_out_p[i]))
#undef drive_3_out
#undef drive_3_out_ptr
#define drive_3_out_ptr(i) (__comp_inst->drive_3_out_p[i])
#define drive_3_out(i) (0+*(__comp_inst->drive_3_out_p[i]))
#undef StatusWord
#undef StatusWord_ptr
#define StatusWord_ptr(i) (__comp_inst->StatusWord_p[i])
#define StatusWord(i) (*(__comp_inst->StatusWord_p[i]))
#undef error_count
#undef error_count_ptr
#define error_count_ptr (__comp_inst->error_count_p)
#define error_count (*__comp_inst->error_count_p)
#undef error_per_sec
#undef error_per_sec_ptr
#define error_per_sec_ptr (__comp_inst->error_per_sec_p)
#define error_per_sec (*__comp_inst->error_per_sec_p)
#undef cycle_count
#undef cycle_count_ptr
#define cycle_count_ptr (__comp_inst->cycle_count_p)
#define cycle_count (*__comp_inst->cycle_count_p)
#undef debug
#undef debug_ptr
#define debug_ptr (__comp_inst->debug_p)
#define debug (*__comp_inst->debug_p)
#undef sync_done
#undef sync_done_ptr
#define sync_done_ptr (__comp_inst->sync_done_p)
#define sync_done (*__comp_inst->sync_done_p)
#undef rx_ok
#undef rx_ok_ptr
#define rx_ok_ptr(i) (__comp_inst->rx_ok_p[i])
#define rx_ok(i) (*(__comp_inst->rx_ok_p[i]))
#undef drive_0_in
#undef drive_0_in_ptr
#define drive_0_in_ptr(i) (__comp_inst->drive_0_in_p[i])
#define drive_0_in(i) (*(__comp_inst->drive_0_in_p[i]))
#undef drive_1_in
#undef drive_1_in_ptr
#define drive_1_in_ptr(i) (__comp_inst->drive_1_in_p[i])
#define drive_1_in(i) (*(__comp_inst->drive_1_in_p[i]))
#undef drive_2_in
#undef drive_2_in_ptr
#define drive_2_in_ptr(i) (__comp_inst->drive_2_in_p[i])
#define drive_2_in(i) (*(__comp_inst->drive_2_in_p[i]))
#undef drive_3_in
#undef drive_3_in_ptr
#define drive_3_in_ptr(i) (__comp_inst->drive_3_in_p[i])
#define drive_3_in(i) (*(__comp_inst->drive_3_in_p[i]))
#undef cycle_read_expected_shift
#define cycle_read_expected_shift (__comp_inst->cycle_read_expected_shift_p)
#undef drive_at_watchdog
#define drive_at_watchdog (__comp_inst->drive_at_watchdog_p)
#undef verb
#define verb (__comp_inst->verb_p)


#line 61 "sercos.comp"

#define MAX_RT_CYCLEN		1200
#define SYNC_FILE_NAME 		"/home/tc2000r/Desktop/TC2000R/sercos2-linuxcnc/syncro-data/tc2000r/byte-raw.bin"
#define MAX_MESSAGE_SIZE 	500
#define BUF_SIZE         	1024
#define DRIVE_NUM  			4
#define DEFAULT_IF          "enp3s0"

int verbose = 0, shift=700;
#include "../source/sercos-ethernet.h"
#include "../source/sercos-mdt-gen.h"
#include "../source/talk2stm.h"

#include "../source/sercos-ethernet.c"
#include "../source/sercos-mdt-gen.c"
#include "../source/talk2stm.c"

#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

bool restart = true;
bool first_msg = true;

SercosDrive drives[DRIVE_NUM];
int error_counter = 0;

const uint8_t STM_MAC[] 	= {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
const uint8_t PC_MAC[] 		= {0x01, 0x12, 0x23, 0x34, 0x45, 0x56};

int verdict = 0;
uint8_t eth_tx_buf[BUF_SIZE];
uint8_t eth_rx_buf[BUF_SIZE];
int eth_rx_len, eth_tx_len = 0;
long long eth_total_recieved = 0;
long long eth_total_send     = 0;
bool internal_reset = false;
int error_per_sec_temp 	= 0;
int timeshift = 0;

/*	init() for sercos 				*/
EXTRA_SETUP(){
	verbose=verb;
	shift=cycle_read_expected_shift;
   	printf("Sercos II session : start\n");	
   	/* initializing variables, open files, etc*/
 	first_msg = true;
 	if(verbose > 0) sercos_eth_deinit();
 	error_counter = 0;		
 	verdict = 0;
 	eth_rx_len, eth_tx_len = 0;
 	eth_total_recieved 	= 0;
 	eth_total_send    	= 0;
 	//error_count 		= 0;
	error_per_sec_temp 	= 0;
 	/* initializing  drives. TODO : read config from file */
 	
 	/* drive configs here (auto-generated)*/ 	
	drives[0].addr =1;
	drives[0].idn_in_number =3;
	drives[0].idn_in_size[0]=2;
	drives[0].idn_in_size[1]=4;
	drives[0].idn_in_size[2]=2;
	drives[0].idn_out_number =1;
	drives[0].idn_out_size[0]=4;
	drives[1].addr =2;
	drives[1].idn_in_number =3;
	drives[1].idn_in_size[0]=2;
	drives[1].idn_in_size[1]=4;
	drives[1].idn_in_size[2]=2;
	drives[1].idn_out_number =1;
	drives[1].idn_out_size[0]=4;
	drives[2].addr =3;
	drives[2].idn_in_number =3;
	drives[2].idn_in_size[0]=2;
	drives[2].idn_in_size[1]=4;
	drives[2].idn_in_size[2]=2;
	drives[2].idn_out_number =1;
	drives[2].idn_out_size[0]=4;
	drives[3].addr =4;
	drives[3].idn_in_number =3;
	drives[3].idn_in_size[0]=2;
	drives[3].idn_in_size[1]=4;
	drives[3].idn_in_size[2]=2;
	drives[3].idn_out_number =1;
	drives[3].idn_out_size[0]=4;
	



 	/*		initializing stm32 talker	     */
 	FILE * f = fopen(SYNC_FILE_NAME,"rb");
 	stm_talker_init(f);
 	sercos_eth_init(eth_tx_buf, STM_MAC, PC_MAC);
 	fclose(f);
 	if(verbose > 0) printf("Sercos init done\n");
 	return(0);

}


/*		Microseconds since watchdog				*/
unsigned long long get_tim(unsigned long long watchdog){
	unsigned long long newtime = rtapi_get_time();
	long long ttttim = abs(newtime - watchdog);
	ttttim = ttttim > 500000000 ? 1000000000  - ttttim : ttttim;
	return ttttim / 1000;
}




/*		Correct limnuxcnc thread to achieve synxronization with sercos				*/
long long readtime_buff[] = {0,0,0,0,0,0,0,0,0,0};
long long sum_readtime, readtime_buff_pos=0;
bool readtime_buff_filled = false;
void update_readtime(long long time){
	sum_readtime += time - readtime_buff[readtime_buff_pos];
	readtime_buff[readtime_buff_pos] = time;

	if(++readtime_buff_pos > 9) {
		readtime_buff_pos = 0;
		readtime_buff_filled = true;
	}

	if(readtime_buff_filled)
		rtapi_task_pll_set_correction(10 * (sum_readtime - 10*shift));
}



FUNCTION(_)
{
	if(reset || internal_reset){
		stm_talker_reset();
		internal_reset 	= false;
		sync_done 		= false;
		error_count 	= 0;
		error_counter 	= 0;
		error_per_sec_temp = 0;
		return;
	}

	unsigned long long cycle_start = rtapi_get_time();
	unsigned long long at = 0;
	bool wait_for_AT = false;
	int	resends = 0;
	bool got_rq = false;
	bool got_fb = false;
	bool got_at = false;
	verdict = 0;

	/* read hal pins			*/
	for(int i = 0; i < DRIVE_NUM; i++)
	{
		drives[i].rx_ok = false;
		drives[i].ControlWord = ControlWord(i);
	}
	/*	autogenerated part			*/
	drives[0].idn_out[0] = drive_0_out(0);
	drives[1].idn_out[0] = drive_1_out(0);
	drives[2].idn_out[0] = drive_2_out(0);
	drives[3].idn_out[0] = drive_3_out(0);
	

	while(1){
		usleep(20);
		if(got_rq && got_fb && got_at)// && get_tim(cycle_start) > 2 * drive_at_watchdog) 
			break;

		/*     10 bad packages in a row = resync	*/
		if(error_counter > 10){
			sync_done = false;
			if(verbose > 0) 
				rtapi_print("Error: >10 bad SERCOS II packages in a row, restarting. \n");
			internal_reset = true;
			break;
		}

		if(eth_total_send % 5000 == 0){
			error_per_sec 	= error_per_sec_temp;
			error_per_sec_temp = 0;
		}


		/*		check watchdogs			*/
		if( get_tim(cycle_start) > MAX_RT_CYCLEN ){		
			if(SERCOS_SYNC_DONE){
				if(!got_rq){
					if(verbose > 0) rtapi_print("warning: no request this cycle 	\n");
					error_per_sec_temp ++;
					error_count ++;
				}
				if(!got_fb){
					if(verbose > 0) rtapi_print("warning: no feedback this cycle. \n");
					error_per_sec_temp ++;
					error_count ++;
				}
				if(!got_at){
					if(verbose > 0) rtapi_print("warning: no AT this cycle \n");
					error_per_sec_temp ++;
					error_count ++;
				}
				error_counter++;
			}
			break;
		}

		/*		drive's quick anknowledge telegramm. If in didn't arrived in some time, try to resend */
		if( wait_for_AT && get_tim(at) > drive_at_watchdog && resends < 3 ){
			/*		TODO		*/
			resends += 1;
			at=rtapi_get_time();
			sercos_eth_send(eth_tx_buf, eth_tx_len);
		}


		eth_rx_len = sercos_eth_receive( eth_rx_buf, BUF_SIZE );

		if(0 >= eth_rx_len ){
		    continue;
		}

		verdict = analysis(DRIVE_NUM, drives, eth_rx_buf, eth_rx_len, eth_tx_buf, eth_tx_len, eth_total_send, eth_total_recieved);

		
		if( verdict == RESTART ){
			if(verbose > 0) rtapi_print("Stm requested restart. Good Bye <3\n");
			internal_reset = true;
			return;
		}


		if( verdict == PASS || verdict== TIMESTAMP){
			/* do nothing	*/
			continue;
		}

		if( verdict == CRC_MISMATCH){
			/*	 send prevous telegramm again	*/
			at = rtapi_get_time();
			sercos_eth_send(eth_tx_buf, eth_tx_len);
		}

		if(verdict ==  AT) {
			/*	 drive's achieved my telegramm */
			wait_for_AT = false;
			got_at = true;
			continue;
		}

		if( verdict == UNKNOWN || verdict == CORRUPTED || verdict == MISS_TG){
			error_counter++;
			error_per_sec_temp ++;
			error_count ++;
			if(verdict != MISS_TG) continue;
		}

		

		/*		Get the fedback		*/		
		if(verdict == OK || verdict == MISS_TG)
		{	if(verdict == OK) error_counter = 0;
			if(sercos_sync_stage > 33) update_readtime(get_tim(cycle_start));
			got_fb = true;
			continue;
		}


		/*		send the command			*/
		if(verdict == REQUEST){
			first_msg = false;		
			/*   preparing the answer   */
			eth_tx_len = synthesis(DRIVE_NUM, drives, eth_tx_buf, eth_total_send); 

			/*   sending  prepared message  */
			verdict = 0;
			eth_total_send ++;
			resends = 0;
			// printf("im here \n");
			sercos_eth_send(eth_tx_buf, eth_tx_len);
			at = rtapi_get_time();
			got_rq = true;
		}
	}
	for(int i = 0; i < DRIVE_NUM; i++)
	{
		rx_ok(i) = drives[i].rx_ok;
		StatusWord(i) = drives[i].StatusWord;
	}
	drive_0_in(0) = drives[0].idn_in[0];
	drive_0_in(1) = drives[0].idn_in[1];
	drive_0_in(2) = drives[0].idn_in[2];
	drive_1_in(0) = drives[1].idn_in[0];
	drive_1_in(1) = drives[1].idn_in[1];
	drive_1_in(2) = drives[1].idn_in[2];
	drive_2_in(0) = drives[2].idn_in[0];
	drive_2_in(1) = drives[2].idn_in[1];
	drive_2_in(2) = drives[2].idn_in[2];
	drive_3_in(0) = drives[3].idn_in[0];
	drive_3_in(1) = drives[3].idn_in[1];
	drive_3_in(2) = drives[3].idn_in[2];
	



	cycle_count = eth_total_send;
	debug = timestamp_response;
	if(sync_done &&  cycle_count - debug != timeshift)
	{
		rtapi_print("warning: stm and my cyc count mismatches!\n");
		timeshift = cycle_count - debug;
	}
	if(SERCOS_SYNC_DONE && ! SERCOS_SYNC_DONE_prev)
		sync_done 	= true;
	SERCOS_SYNC_DONE_prev = SERCOS_SYNC_DONE;
	return;		
}
	




static int __comp_get_data_size(void) { return 0; }
