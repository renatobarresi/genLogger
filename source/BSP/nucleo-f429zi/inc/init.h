#ifndef INIT_H
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif

int stm32f429_init();
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif