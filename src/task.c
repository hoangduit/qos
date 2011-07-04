
#include <stdlib.h>

#include "os_cfg.h"
#include "os_cpu.h"
#include "qos.h"
#include "task.h"

static os_tcb_t os_tcb_table[OS_TASK_NUM];
// the running task pointer, because the pointer maybe use in interrupt, 
// it should use between qos_enter_critical() & qos_exit_critical()
static os_tcb_t *os_tcb_run = NULL;


void qos_task_init(void){
    int16u_t i = 0;

    for(i=0; i<OS_TASK_NUM; i++){
        const os_tcb_t os_tcb_default = OS_TCB_DEFAULT;

        os_tcb_table[i] = os_tcb_default;
    }
}



void qos_task_entry_default(void){
    qos_verify( false );
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




void qos_task_stop( int16u_t task_id ){
    qos_verify( OS_TASK_NUM > task_id );

    qos_enter_critical();
    os_tcb_table[task_id].state = TASK_STATE_STOP;
    qos_exit_critical();
}



void qos_task_ready( int16u_t task_id ){
    qos_verify( OS_TASK_NUM > task_id );

    qos_enter_critical();
    os_tcb_table[task_id].state = TASK_STATE_READY;
    qos_exit_critical();
}




void qos_task_set_entry( task_entry_t entry ){
    qos_verify( NULL != entry );

    qos_enter_critical();
    qos_verify( NULL != os_tcb_run );
    os_tcb_run->entry = entry;
    qos_exit_critical();
}




void qos_task_sleep_tick( int32u_t tick ){
    qos_enter_critical();
    qos_verify( NULL != os_tcb_run );
    os_tcb_run->delay_tick = tick;
    os_tcb_run->state = TASK_STATE_WAIT;
    qos_exit_critical();
}





// note: it's in critical range when entry below function
// and it's also in critical range when exit.
static void run_task( os_tcb_t * const os_tcb ){
    qos_verify( NULL != os_tcb );

    //qos_enter_critical();
    os_tcb->state = TASK_STATE_RUN;
    qos_verify( NULL == os_tcb_run );
    os_tcb_run = os_tcb; 
    qos_exit_critical();

    os_tcb->entry();

    qos_enter_critical();
    qos_verify( NULL != os_tcb_run );
    os_tcb_run = NULL;

    // task didn't change state
    if( os_tcb->state == TASK_STATE_RUN ){
        os_tcb->state = TASK_STATE_READY;
    }
    
    //qos_exit_critical();
}





static void task_delay_tick_dec( os_tcb_t * const os_tcb, int32u_t tick ){
    qos_verify( NULL != os_tcb );

    if( os_tcb->delay_tick <= tick ){
        os_tcb->delay_tick = 0;
    }
    else{
        os_tcb->delay_tick -= tick;
    }

    if( 0 == os_tcb->delay_tick ){
        if( TASK_STATE_WAIT == os_tcb->state ){
            os_tcb->state = TASK_STATE_READY;
        }
    }
}




static void qos_task_tick( os_tcb_t * const os_tcb, int32u_t tick ){
    qos_verify( NULL != os_tcb );

    qos_enter_critical();
    if( TASK_STATE_STOP == os_tcb->state ){
        qos_exit_critical();
        return;
    }

    // check if os_tcb initialed?
    if( TASK_INVALID_TASK_ID == os_tcb->task_id ){
        qos_exit_critical();
        return;
    }

    // dec task's delay_tick 
    task_delay_tick_dec( os_tcb, tick );

    // if task ready, run it
    if( os_tcb->state == TASK_STATE_READY ){
        // call run_task() in critical range. So interrupt can post message or semphore to the task.
        run_task( os_tcb );
    }
    qos_exit_critical();

    return;
}




// all task pass n ticks
void qos_tasks_tick( int32u_t tick ){
    int16u_t i = 0;

    for( i=0; i<OS_TASK_NUM; i++ ){
        qos_task_tick( os_tcb_table + i, tick );
    }
}




// no more------------------------------

