#ifndef __EC11_H
#define __EC11_H

#include "main.h"

typedef enum
{
	EC11_NONE=0,
	EC11_LEFT,
	EC11_RIGHT,
	EC11_PRESS
}EC11_Event;

void EC11_Init(void);
void EC11_Update(void);
EC11_Event EC11_GetEvent(void);

#endif
