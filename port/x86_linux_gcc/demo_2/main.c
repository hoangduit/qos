#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os_cfg.h"
#include "../../port_src/os_cpu.h"
#include "../../../src/qos.h"
#include "../../../src/task.h"



static int32u_t cnt = 0;


static void print_tcb( os_tcb_t const * const tcb ){
    qos_verify( NULL != tcb );
    
    printf( "   task_id=%u, state=%u, delay_tick=%u, entry=0x%x\n"
            , (unsigned int)tcb->task_id
            , (unsigned int)tcb->state
            , (unsigned int)tcb->delay_tick
            , (unsigned int)tcb->entry );
}



void user_tick_hook( int32u_t tick ){
    static int32u_t tick_cnt = 0;
    os_tcb_t tcb;

    tick_cnt += tick;

    printf( "pass %u tick\n", (unsigned int)tick_cnt );
    qos_task_tcb( &tcb, 0 );
    print_tcb( &tcb );
    qos_task_tcb( &tcb, 1 );
    print_tcb( &tcb );
}





static void task_2(void){
    ++cnt;
    printf( "task_2: %u\n", (unsigned int)cnt );

    qos_task_sleep_tick( 2 );
}





static void task_1(void){
    ++cnt;
    printf( "task_1: %u\n", (unsigned int)cnt );

    qos_task_sleep_tick( 4 );
} 




int main(void){
    qos_init();

    qos_task_create( task_1, 0 );
    qos_task_create( task_2, 1 );
    qos_start();

    return 0;
}


