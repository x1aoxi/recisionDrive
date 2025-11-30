#ifndef __KEY_DRIVER_H__
#define __KEY_DRIVER_H__

#include "main.h"
#include "ebtn.h"

unsigned char Key_Read(void);
int Ebtn_Init(void);
void my_handle_key_event(struct ebtn_btn *btn, ebtn_evt_t evt);

#endif
