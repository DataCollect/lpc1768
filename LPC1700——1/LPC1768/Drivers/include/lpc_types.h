#ifndef __LPC_TYPES_H__
#define __LPC_TYPES_H__

#include<stdint.h>

/**
 * @brief Boolean Type definition
 */
typedef enum
{
    FALSE = 0,
	TRUE  = !FALSE
}Bool;

/**
 * @Status Type definition
 */
typedef enum
{
	ERROR = 0,
	SUCCESS = !ERROR
}Status;

/**
 * @brief Flag Status and Interrupt Flag Status type definition
 */
typedef enum 
{
    RESET = 0, 
    SET = !RESET
}FlagStatus, IntStatus, SetState;

/**
 * @brief Functionalstate Type definition
 */
typedef enum
{
    DISABLE = 0,
    ENABLE  = !DISABLE
}FunctionalState;

/** Pointer to Function returning Void (any number of parameters) */
typedef void (*PFV)();

/**	Pointer to Function returning int32_t (any number of parameters) */
typedef int32_t (*PFI)();


/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions
 */
#undef _BIT
#define _BIT(n) (1<<n)

/* _SBF(f,v) sets the bit field starting at position "f" to value "v"
 */
#undef _SBF
#define _SBF(f, v) (v<<f)

/** _BITMASK constructs a symbol with 'feild_width' least significant bits set */
#undef _BITMASK
#define _BITMASK(feildwidth) (_BIT(feildwidth) - 1)

/* NULL	 ponter */
#ifndef NULL
#define NULL ((void *) 0)
#endif

/* Number of elements in an array */
#define NELEMENTS(array) (sizeof(array) / sizeof(array[0]))

/* Static data or function define */
#define STATIC static

/* External data or function define*/
#define EXTERN extern

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* old type redefinition */

typedef char     CHAR;
typedef uint8_t  UNS_8;
typedef int8_t   INT_8;
typedef uint16_t UNS_16;
typedef int16_t  INT_16;
typedef uint32_t UNS_32;
typedef int32_t  INT_32;

#endif



 
