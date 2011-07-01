

#ifndef false
#define false               0
#endif

#ifndef true
#define true                1
#endif

#ifndef NULL
#define NULL                0
#endif


#if OS_VERIFY_EN > 0
#define qos_verify(a)  do{ if(!(a)) qos_verify_process( #a, __FILE__, __LINE__ ); }while(0)
#else
#define qos_verify(a)  
#endif


#define TASK_INVALID_TASK_ID       0xffff

// task state
// wait     
// ready    
// run      
#define TASK_STATE_STOP             0
#define TASK_STATE_WAIT             1
#define TASK_STATE_READY            2
#define TASK_STATE_RUN              3


typedef void (*task_entry_t)(void);
typedef struct os_tcb{ 
    int16u_t task_id;               // task ID (0..65535)
                                    // task ID equal task priority, 0 is the highest priority
    int16u_t state; 

    int32u_t delay_tick;            //
    task_entry_t entry;
}os_tcb_t;
#define OS_TCB_DEFAULT {                    \
    TASK_INVALID_TASK_ID, TASK_STATE_STOP   \
    , 0                                     \
    , NULL                                  \
}





// task management----------------------
void qos_task_create( task_entry_t entry, int16u_t task_id );

void qos_task_tcb( os_tcb_t * const tcb, int16u_t task_id );
void qos_task_entry( task_entry_t entry );
void qos_task_stop( int16u_t task_id );
void qos_task_ready( int16u_t task_id );

// time management----------------------
void qos_sleep_tick( int32u_t ticks );

// call in port-------------------------
void qos_tick(void);

// miscellaneous------------------------
void qos_start(void);

// compiler specific ISR prototypes-----
//void os_tick_isr(void);



// no more------------------------------

