
typedef unsigned char   bool;
typedef unsigned char   int8u_t;
typedef char            int8s_t;
typedef unsigned short  int16u_t;
typedef short           int16s_t;
typedef unsigned long   int32u_t;
typedef long            int32s_t;
typedef long long       int64s_t;
typedef float           fp32_t;
typedef double          fp64_t;


void qos_enter_critical(void);
void qos_exit_critical(void);

bool qos_port_init(void);

// for debug
void qos_verify_process( char const * const express, char const * const file_name, int line );

// no more----------------------
