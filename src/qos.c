
#include <stdlib.h>

#include "os_cfg.h"
#include "os_cpu.h"
#include "qos.h"
#include "task.h"

static int32u_t os_tick = 0;


// qos_tick() must be call in tick timer isr
// tick timer isr implement in qos port
void qos_tick(void){
    os_tick++;
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

    qos_tasks_tick( tick );

    return;
}





void qos_init(void){
    qos_task_init();
}




void qos_start(void){
    // qos_port_init() do below:
    // 1. init hardware.
    // 2. enable timer tick isr.
    if( !qos_port_init() ){
        qos_verify( false );
        return;
    }

    for(;;){
        qos_schedule();
    }
}


// no more-------------------------------
