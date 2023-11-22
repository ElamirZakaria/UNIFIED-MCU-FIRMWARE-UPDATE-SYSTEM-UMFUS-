
#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef enum
{
	E_OK,
	E_NOK,
	E_NULL_POINTER
}STD_error_t;

#define NULL_POINTER ((void *)0)


/* Boolean Data Type */
typedef unsigned char bool;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif

#ifndef TRUE
#define TRUE        (1u)
#endif



typedef unsigned char         u8;          /*           0 .. 255             */
typedef signed char           s8;          /*        -128 .. +127            */
typedef unsigned short        u16;         /*           0 .. 65535           */
typedef signed short          s16;         /*      -32768 .. +32767          */
typedef unsigned long         u32;         /*           0 .. 4294967295      */
typedef signed long           s32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    u64;         /*       0..18446744073709551615  */
typedef signed long long      s64;
typedef float                 f32;
typedef double                f64;




/* Boolean Data Type */
typedef unsigned char  		Bool;

/* Boolean Values */
#define FALSE       (0u)
#define TRUE        (1u)

#define HIGH        (1u)
#define LOW         (0u)

typedef unsigned char  			uint8 ;
typedef unsigned short  		uint16;
typedef unsigned long   		uint32;
typedef unsigned long long  uint64;

typedef signed char       	sint8 ;
typedef signed short 	  		sint16;
typedef signed long 	   		sint32;
typedef signed long long  	sint64;

typedef float 							float32;
typedef double							float64;

/**********************************************/

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

/* for compiler use weak in can driver*/

#ifndef   __weak 
  #define __weak                                   __attribute__((weak))
#endif


#endif  // STD_TYPES_H
