
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>



#include "os_cfg.h"
#include "os_cpu.h"
#include "qos.h"

#define  OS_SLEEP()		{ struct timespec req, rem; req.tv_sec=0; req.tv_nsec=10000000; nanosleep(&req, &rem);}

#define NANO_PER_SECOND     (1000 * 1000 * 1000)

// simulate timer interrupt
static pthread_t thread_timer;
// simulate interrupt switcher
static pthread_mutex_t mutex;


void qos_enter_critical(void){
    pthread_mutex_lock( &mutex );
}



void qos_exit_critical(void){
    pthread_mutex_unlock( &mutex );
}




static void ctrl_break_handler( int ctrl ){
    int res;

    // let timer thread exit
    res = pthread_cancel( thread_timer );
    if( res != 0 ){
        printf( "pthread_cancel failed\n" );
        exit( EXIT_FAILURE );
    }
    res = pthread_join( thread_timer, NULL );
    if( res != 0 ){
        printf( "pthread_join failed\n" );
        exit( EXIT_FAILURE );
    }
    pthread_mutex_destroy( &mutex );
    
    if( ctrl == SIGTSTP ){				// Handler if CTRL-D is pressed
 	    exit( EXIT_SUCCESS );
    } 
    else if (ctrl==SIGINT){			    //Handler if CTRL-C is pressed
 	    exit( EXIT_SUCCESS );
    }

    return;
}




// function for time tick 'interrupt'
static void *thread_timer_callback( void *parg ){
    int res;
    
    // let pthread_cancel() work
    res = pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL );
    if( res != 0 ){
        printf( "Thread pthread_setcancelstate failed\n" );
        exit( EXIT_FAILURE );
    }
    //res = pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
    res = pthread_setcanceltype( PTHREAD_CANCEL_DEFERRED, NULL );
    if( res != 0 ){
        printf( "Thread pthread_setcanceltype failed\n" );
        exit( EXIT_FAILURE );
    }

    for(;;){
        struct timespec timeout = { 0, 0 };

        timeout.tv_sec =  ( NANO_PER_SECOND / OS_TICKS_PER_SEC ) / NANO_PER_SECOND;
        timeout.tv_nsec = ( NANO_PER_SECOND / OS_TICKS_PER_SEC ) % NANO_PER_SECOND;
    
        qos_tick();

        nanosleep( &timeout, NULL );
    }
    
    pthread_exit( 0 );
    return NULL;
}




bool qos_port_init(void){
    int res = 0;

    signal( SIGINT,  ctrl_break_handler );
    signal( SIGTSTP, ctrl_break_handler);

    printf( "usuage: ctrl-c exit\n" );

    res = pthread_mutex_init( &mutex, NULL );
    if( res != 0 ){
        printf( "mutex initialization failed\n" );
        exit( EXIT_FAILURE );
        return false;
    }

    // create timer thread, to simulate timer interrupt
    res = pthread_create( &thread_timer, NULL, (void *)thread_timer_callback, NULL );
    if( res != 0 ){
        printf( "create thread error\n" );
        exit( EXIT_FAILURE );
        return false;
    }

    return true;
}



void qos_verify_process( char const * const express, char const * const file_name, int line ){
    printf( "verify FAIL:%s file:%s @%d\n", express, file_name, line );
}


// no more-----------------------
