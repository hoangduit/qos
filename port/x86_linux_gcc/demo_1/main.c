#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os_cfg.h"
#include "../../port_src/os_cpu.h"
#include "../../../src/qos.h"



#define TASK_ID         0

static int32u_t cnt = 0;

static void task_entry_1( void );
static void task_entry_2( void );

void user_tick_hook( int32u_t tick ){
    os_tcb_t tcb;

    qos_task_tcb( &tcb, TASK_ID );
    printf( "pass %u tick, task_id=%u, state=%u, delay_tick=%u, entry=0x%x\n"
            , (unsigned int)tick
            , (unsigned int)tcb.task_id
            , (unsigned int)tcb.state
            , (unsigned int)tcb.delay_tick
            , (unsigned int)tcb.entry );
}



void task_entry_2( void ){
    printf( "task_entry_2: %u\n", (unsigned int)cnt );

    qos_sleep_tick( 2 );
    qos_task_entry( task_entry_1 );
}





void task_entry_1( void ){
    ++cnt;
    printf( "task_entry_1: %u\n", (unsigned int)cnt );

    qos_sleep_tick( 5 );
    qos_task_entry( task_entry_2 );
    
    return; 
} 



int main( void ){
    qos_init();

    qos_task_create( task_entry_1, TASK_ID );
    qos_start();

    return 0;
}


