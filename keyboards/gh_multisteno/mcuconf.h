#pragma once

#include_next <mcuconf.h>

// taken from the configuration in STM32CubeIDE
#undef STM32_PLLXTPRE
#define STM32_PLLXTPRE STM32_PLLXTPRE_DIV2

#undef STM32_USBPRE
#define STM32_USBPRE STM32_USBPRE_DIV1P5

#undef STM32_PLLMUL_VALUE
#define STM32_PLLMUL_VALUE 9

#undef STM32_PWM_USE_TIM1
#define STM32_PWM_USE_TIM1 TRUE

#undef STM32_PWM_USE_TIM4
#define STM32_PWM_USE_TIM4 TRUE