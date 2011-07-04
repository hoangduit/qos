

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


// call in port-------------------------
void qos_tick_isr(void);

// miscellaneous------------------------
void qos_init(void);
void qos_start(void);



// no more------------------------------

