#include <rtems.h>
#include "sys_time.h"

//struct sys_time_timer timers[SYS_TIME_NB_TIMER];
struct sys_time sys_time;





//Call back function for RTEMS. We will provide this function to RTEMS API and
//will call application provided callbacks when this function is called.
static rtems_timer_service_routine timer_callback(rtems_id timerid, void *data);
static char timer_name[SYS_TIME_NB_TIMER][4]=
						{
								{'T','I','M','0'},
								{'T','I','M','1'},
								{'T','I','M','2'},
								{'T','I','M','3'},
								{'T','I','M','4'},
								{'T','I','M','5'},
								{'T','I','M','6'},
								{'T','I','M','7'}
						};


void sys_time_init(void){
	int i;
	/*rtems_clock_get(RTEMS_CLOCK_GET_TICKS_PER_SECOND,&sys_time.ticks_per_sec);*/
	sys_time.ticks_per_sec=SYS_TIME_FREQUENCY;
	sys_time.resolution = 1./(sys_time.ticks_per_sec);

	//Just traverse over all the timers and initialize each entry.
	for(i= 0; i<SYS_TIME_NB_TIMER ;i++){
		sys_time.timer[i].index_array=-1;
		sys_time.timer[i].duration=0;
		sys_time.timer[i].cb=NULL;
		sys_time.timer[i].elapsed=false;
		sys_time.timer[i].sys_id = 0;
	}

}

//For arguments detail look in the header file.
//This function will be called by the applications
//and our task is to delegate this function call to rtems timer
//API.

int sys_time_register_timer(float duration, sys_time_cb cb) {

	rtems_status_code status;
	rtems_id timerID;
	uint8_t empty_slot;
	//create the timer.
	for(empty_slot= 0;empty_slot<SYS_TIME_NB_TIMER;empty_slot++){
		if(sys_time.timer[empty_slot].index_array==-1)
			break;

	}
	if(empty_slot==SYS_TIME_NB_TIMER)//Timers occupancy are full
		return -1;

	status = rtems_timer_create(rtems_build_name(timer_name[empty_slot][0],timer_name[empty_slot][1],
			timer_name[empty_slot][2],timer_name[empty_slot][3]),&timerID);

	if(status)
		return -1;

	//initialize the particular timer
	sys_time.timer[empty_slot].cb          = cb;
	sys_time.timer[empty_slot].duration    = (duration*(sys_time.ticks_per_sec*1.))/*ticks_per_second*/;
	sys_time.timer[empty_slot].elapsed     = false;
	sys_time.timer[empty_slot].index_array = empty_slot;
	sys_time.timer[empty_slot].sys_id      = timerID;


	status = rtems_timer_fire_after(sys_time.timer[empty_slot].sys_id,sys_time.timer[empty_slot].duration,timer_callback,(void *)(&sys_time.timer[empty_slot]));
	if(status){
		//initialize the particular timer
		sys_time.timer[empty_slot].cb          = NULL;
		sys_time.timer[empty_slot].duration    = 0;
		sys_time.timer[empty_slot].elapsed     = false;
		sys_time.timer[empty_slot].index_array = -1;
		sys_time.timer[empty_slot].sys_id      = 0;
		return -1;
	}
	return empty_slot;
}

//Paparazzi cancel means just delete the timer from RTEMS
void sys_time_cancel_timer(tid_t id){
	rtems_timer_delete(sys_time.timer[id].sys_id);
	sys_time.timer[id].cb          = (sys_time_cb)NULL;
	sys_time.timer[id].duration    = 0;
	sys_time.timer[id].elapsed     = false;
	sys_time.timer[id].index_array = -1;
	sys_time.timer[id].sys_id      = 0;
}

bool_t sys_time_check_and_ack_timer(tid_t id){
	bool_t result=false;
	if(sys_time.timer[id].elapsed==true){
		sys_time.timer[id].elapsed=false;
		result=true;
	}
	return result;
}
//Here we need to first cancel the timer and again
//initialise it through "rtems_timer_fire_after".
void sys_time_update_timer(tid_t id, float duration){
	rtems_timer_cancel(sys_time.timer[id].sys_id);
	//then update the duration
	sys_time.timer[id].duration = (duration*sys_time.ticks_per_sec*1.0) /*ticks_per_second*/;
	rtems_timer_fire_after(sys_time.timer[id].sys_id,sys_time.timer[id].duration,timer_callback,(void *)(&sys_time.timer[id]));
	return;
}
//Callback: Don't call user level api functions from the
//following method because this function will be called
//in rtems_tick context not in the applicatios. I need
//to do more investigation about this.But for now it is working.
rtems_timer_service_routine timer_callback(rtems_id timerid, void *data){

	/*first at every callback update the number of ticks and seconds since boot.*/
	sys_time.nb_tick = rtems_clock_get_ticks_since_boot();
	sys_time.nb_sec = (sys_time.nb_tick)*(sys_time.resolution);
	struct sys_time_timer *timer = (struct sys_time_timer *)data;
	timer->elapsed = true;
	if(timer->cb)
		timer->cb(timer->index_array);
	//Now, reset the timer
	rtems_timer_reset(timerid);
}
