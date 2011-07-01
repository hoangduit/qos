
#include <stdlib.h>

#include "os_cfg.h"
#include "os_cpu.h"
#include "qos.h"

static os_tcb_t os_tcb_table[OS_TASK_NUM];
// the running task pointer, because the pointer maybe use in interrupt, 
// it should use between qos_enter_critical() & qos_exit_critical()
static os_tcb_t *os_tcb_run = NULL;
static int32u_t os_tick = 0;


extern void qos_enter_critical(void);
extern void qos_exit_critical(void);

// task management----------------------
static void qos_tcb_init(void){
    int16u_t i = 0;

    for(i=0; i<OS_TASK_NUM; i++){
        const os_tcb_t os_tcb_default = OS_TCB_DEFAULT;

        os_tcb_table[i] = os_tcb_default;
    }
}




void qos_task_create( task_entry_t entry, int16u_t task_id ){
    qos_verify( NULL != entry );
    qos_verify( OS_TASK_NUM > task_id );

    os_tcb_table[task_id].task_id = task_id;
    os_tcb_table[task_id].entry = entry;
    os_tcb_table[task_id].state = TASK_STATE_READY;
    os_tcb_table[task_id].delay_tick = 0;
}




void qos_task_tcb( os_tcb_t * const tcb, int16u_t task_id ){
    qos_verify( NULL != tcb );
    qos_verify( OS_TASK_NUM > task_id );

    qos_enter_critical();
    *tcb = os_tcb_table[task_id];
    qos_exit_critical();
}



// qos_tick() must be call in tick timer
// tick timer implement in qos port
void qos_tick(void){
    qos_enter_critical();
    os_tick++;
    qos_exit_critical();
}



static void run_task( os_tcb_t * const os_tcb ){
    qos_verify( NULL != os_tcb );

    os_tcb->state = TASK_STATE_RUN;

    qos_enter_critical();
    qos_verify( NULL == os_tcb_run );
    os_tcb_run = os_tcb; 
    qos_exit_critical();

    os_tcb->entry();

    qos_enter_critical();
    qos_verify( NULL != os_tcb_run );
    os_tcb_run = NULL;
    qos_exit_critical();

    // task didn't change state
    if( os_tcb->state == TASK_STATE_RUN ){
        os_tcb->state = TASK_STATE_READY;
    }
}





static void task_delay_tick( os_tcb_t * const os_tcb, int32u_t tick ){
    qos_verify( NULL != os_tcb );

    if( os_tcb->delay_tick <= tick ){
        os_tcb->delay_tick = 0;
    }
    else{
        os_tcb->delay_tick -= tick;
        //printf( "os_tcb->delay_tick = %u\n", (unsigned int)os_tcb->delay_tick );
    }

    if( 0 == os_tcb->delay_tick ){
        if( TASK_STATE_WAIT == os_tcb->state ){
            os_tcb->state = TASK_STATE_READY;
        }
    }
}




static void qos_task_tick( os_tcb_t * const os_tcb, int32u_t tick ){
    qos_verify( NULL != os_tcb );

    // check if os_tcb initialed?
    if( TASK_INVALID_TASK_ID == os_tcb->task_id ){
        return;
    }

    // dec task's delay_tick 
    task_delay_tick( os_tcb, tick );

    // if task ready, run it
    if( os_tcb->state == TASK_STATE_READY ){
        run_task( os_tcb );
    }

    return;
}





#if OS_TICK_HOOK_EN > 0
extern void user_tick_hook( int32u_t tick );
#endif
static void qos_schedule(void){
    int16u_t i = 0;
    int32u_t tick = 0;      // save os_tick, reduce disable interrupt time.

    qos_enter_critical();

    if( 0 == os_tick ){
        qos_exit_critical();
        return;
    }

    tick = os_tick;
    os_tick = 0;

    qos_exit_critical();

#if OS_TICK_HOOK_EN > 0
    user_tick_hook( tick );
#endif

    // go through all TCBs in TCB list
    for( i=0; i<OS_TASK_NUM; i++ ){
        qos_task_tick( os_tcb_table + i, tick );
    }

    return;
}



void qos_task_entry( task_entry_t entry ){
    qos_verify( NULL != entry );

    qos_enter_critical();
    qos_verify( NULL != os_tcb_run );
    os_tcb_run->entry = entry;
    qos_exit_critical();
}



void qos_task_stop( int16u_t task_id ){
    qos_verify( OS_TASK_NUM > task_id );

    os_tcb_table[task_id].state = TASK_STATE_STOP;
}



void qos_task_ready( int16u_t task_id ){
    qos_verify( OS_TASK_NUM > task_id );

    os_tcb_table[task_id].state = TASK_STATE_READY;
}




// time management----------------------
void qos_sleep_tick( int32u_t ticks ){
    qos_enter_critical();
    qos_verify( NULL != os_tcb_run );
    os_tcb_run->delay_tick = ticks;
    os_tcb_run->state = TASK_STATE_WAIT;
    qos_exit_critical();
}






// miscellaneous------------------------
void qos_init(void){
    qos_tcb_init();
}




extern bool qos_port_init(void);
void qos_start(void){
    if( !qos_port_init() ){
        qos_verify( false );
        return;
    }

    for(;;){
        qos_schedule();
    }
}



