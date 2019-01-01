#ifndef  __TYPEDEF_H__
#define  __TYPEDEF_H__

#define __dead2     __attribute__((__noreturn__))
#define __pure2     __attribute__((__const__))
#define __pure
#define __unused    __attribute__((__unused__))
#define __used      __attribute__((__used__))
#define __packed    __attribute__((__packed__))
#define __aligned(x)    __attribute__((__aligned__(x)))
#define __section(x)    __attribute__((__section__(x)))

typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

typedef signed int   int32_t;
typedef signed short int16_t;
typedef signed char  int8_t;

typedef uint32_t bool_t;

typedef uint32_t size_t;

typedef uint32_t uintptr_t;
typedef unsigned long long uint64_t;

#define NULL ((void*)0)

#endif  /*__TYPEDEF_H__*/
