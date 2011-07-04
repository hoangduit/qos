

#define TASK_INVALID_TASK_ID       0xffff

// task state
// stop
// wait         task is waiting for semphore, message, or ticks delay.
// ready        task is ready for running.
// run          task is running.
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
    , 0, qos_task_entry_default             \
}



// for all task
void qos_task_init(void);
void qos_tasks_tick( int32u_t tick );
void qos_task_entry_default(void);

// task function for sepcial task_id
void qos_task_create( task_entry_t entry, int16u_t task_id );
void qos_task_tcb( os_tcb_t * const tcb, int16u_t task_id );
void qos_task_stop( int16u_t task_id );
void qos_task_ready( int16u_t task_id );

// for current task
void qos_task_set_entry( task_entry_t entry );
void qos_task_sleep_tick( int32u_t tick );


// no more------------------------------

