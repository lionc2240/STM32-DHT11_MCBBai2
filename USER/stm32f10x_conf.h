/* stm32f10x_conf.h */

/* Các #include cho các ngoại vi bạn dùng */
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_i2c.h"
#include "misc.h"

/* Bắt buộc phải có nếu bạn không muốn lỗi assert_param */
#ifdef USE_FULL_ASSERT
  #define assert_param(expr) ((void)0) // Bạn có thể để trống hoặc chỉ định hàm kiểm tra
#else
  #define assert_param(expr) ((void)0)
#endif