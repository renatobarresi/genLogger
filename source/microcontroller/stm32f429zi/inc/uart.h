#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

void uart_init();

void uart_tx(char pBuff, int buffSize);

#ifdef __cplusplus
}
#endif

#endif