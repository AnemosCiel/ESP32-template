#ifndef __USYSTEM_H__
#define __USYSTEM_H__


/*-----------------------------Include-----------------------------------*/
#include <stdint.h>
/*------------------------------Define----------------------------------*/
#ifndef MIN
#define MIN(x,y)    (((x)>(y))?(y):(x))
#endif
#ifndef MAX
#define MAX(x,y)    (((x)>(y))?(x):(y))
#endif
#ifndef DIFF
#define DIFF(x,y)   (((x)>(y))?((x)-(y)):((y)-(x)))
#endif
/*------------------------------Exertn----------------------------------*/
int64_t user_get_systemtick( void );
int32_t user_get_runtime( int64_t start );

#endif // !__USYSYTEM_H/