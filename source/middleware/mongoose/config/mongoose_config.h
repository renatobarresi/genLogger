#pragma once

#ifdef TARGET_MICRO
// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_NEWLIB
#define MG_TLS MG_TLS_BUILTIN

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 0
#define MG_ENABLE_DRIVER_STM32F 1
#define MG_OTA MG_OTA_STM32F
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 0
// use HAL-defined execute-in-ram section

// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 4
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)
#endif