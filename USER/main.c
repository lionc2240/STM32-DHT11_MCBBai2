#include "stm32f10x.h"                  
#include "stm32f10x_gpio.h"             
#include "stm32f10x_rcc.h"              
#include "stm32f10x_usart.h"            
#include "stm32f10x_tim.h"              
#include "usart1.h"                    

// KHAI BÁO NGUYÊN MẪU HÀM
void GPIO_Config(void);
void Timer2_Init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void DHT11_Start(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *humidity_int, uint8_t *humidity_dec, uint8_t *temp_int, uint8_t *temp_dec);

int main(void)
{
    // KHAI BÁO BIẾN ĐỂ LƯU GIÁ TRỊ NHIỆT ĐỘ, ĐỘ ẨM VÀ CHECKSUM
    uint8_t hum_int, 
	hum_dec, 
	temp_int, 
	temp_dec, 
	checksum;
    
    // KHỞI TẠO USART1 ĐỂ DEBUG VỚI BAUDRATE 9600
    USART1_DEBUG_Init(9600);
    /*
     * KHỞI TẠO TIMER 2
     * TIMER NÀY SẼ ĐƯỢC SỬ DỤNG ĐỂ TẠO RA CÁC KHOẢNG THỜI GIAN TRỄ CHÍNH XÁC (DELAY)
     * VÀ ĐO LƯỜNG ĐỘ DÀI XUNG TÍN HIỆU TỪ CẢM BIẾN DHT11.
    */
    Timer2_Init();
    // CẤU HÌNH GPIO
    GPIO_Config();
    
    // VÒNG LẶP VÔ TẬN
    while(1)
    {
        // BẮT ĐẦU QUÁ TRÌNH GIAO TIẾP VỚI DHT11
        DHT11_Start();
        // ĐỌC DỮ LIỆU TỪ DHT11 VÀ LẤY GIÁ TRỊ CHECKSUM
        checksum = DHT11_Read_Data(&hum_int, &hum_dec, &temp_int, &temp_dec);
        
        // KIỂM TRA CHECKSUM ĐỂ ĐẢM BẢO DỮ LIỆU ĐÚNG
        if ((hum_int + hum_dec + temp_int + temp_dec) == checksum)
        {
            // GỬI CHUỖI "humidity: " QUA USART
            USART1_DEBUG_SendString("humidity: ");
            // GỬI PHẦN NGUYÊN CỦA ĐỘ ẨM
            USART1_DEBUG_Int_Number(hum_int);
            // GỬI DẤU CHẤM
            USART1_DEBUG_SendChar('.');
            // GỬI PHẦN THẬP PHÂN CỦA ĐỘ ẨM
            if(hum_dec == 0)
                USART1_DEBUG_SendString("0");
            else
                USART1_DEBUG_Int_Number(hum_dec);
            
            // GỬI CHUỖI ", temperature: "
            USART1_DEBUG_SendString(", temperature: ");
            // GỬI PHẦN NGUYÊN CỦA NHIỆT ĐỘ
            USART1_DEBUG_Int_Number(temp_int);
            // GỬI DẤU CHẤM
            USART1_DEBUG_SendChar('.');
            // GỬI PHẦN THẬP PHÂN CỦA NHIỆT ĐỘ
            if(temp_dec == 0)
                USART1_DEBUG_SendString("0");
            else
                USART1_DEBUG_Int_Number(temp_dec);
            // GỬI KÝ TỰ XUỐNG DÒNG
            USART1_DEBUG_SendString("\r\n");
        }
        else
        {
            // NẾU CHECKSUM SAI, GỬI THÔNG BÁO LỖI
            USART1_DEBUG_SendString("checksum Error!\r\n");
        }
        // CHỜ 1 GIÂY TRƯỚC KHI ĐỌC LẠI
        delay_ms(1000);
    }
}

// HÀM KHỞI TẠO TIMER 2
void Timer2_Init(void)
{
    
}

/*
 * HÀM TẠO TRỄ MICRO GIÂY (US)
 * SỬ DỤNG TIMER 2 ĐỂ TẠO RA KHOẢNG TRỄ CHÍNH XÁC.
 * HOẠT ĐỘNG BẰNG CÁCH RESET BỘ ĐẾM CỦA TIMER VỀ 0 VÀ CHỜ CHO ĐẾN KHI
 * BỘ ĐẾM ĐẠT ĐƯỢC GIÁ TRỊ MONG MUỐN (US).
*/
void delay_us(uint16_t us)
{
    // ĐẶT BỘ ĐẾM VỀ 0
    TIM_SetCounter(TIM2, 0);
    // CHỜ CHO ĐẾN KHI BỘ ĐẾM ĐẠT GIÁ TRỊ US
    while(TIM_GetCounter(TIM2) < us);
}

// HÀM TẠO TRỄ MILI GIÂY (MS)
void delay_ms(uint16_t ms)
{
    // LẶP LẠI HÀM DELAY_US 1000 LẦN
    while(ms--)
        delay_us(1000);
}

// CẤU HÌNH CHÂN GPIO CHO DHT11
void GPIO_Config(void)
{
    GPIO_InitTypeDef gpio;
    // CẤP CLOCK CHO GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // CẤU HÌNH CHÂN PB12 LÀ OUTPUT OPEN-DRAIN
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
}

// HÀM BẮT ĐẦU GIAO TIẾP VỚI DHT11
void DHT11_Start(void)
{
    GPIO_InitTypeDef gpio;
    // CẤU HÌNH CHÂN PB12 LÀ OUTPUT OPEN-DRAIN
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Pin = GPIO_Pin_12;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
    // KÉO CHÂN XUỐNG MỨC THẤP TRONG 20MS
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    delay_ms(20);
    // KÉO CHÂN LÊN MỨC CAO TRONG 30US
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    delay_us(30);
    // CHUYỂN CHÂN THÀNH INPUT FLOATING ĐỂ ĐỌC DỮ LIỆU TỪ DHT11
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio);
}

/*
 * HÀM ĐỌC 1 BYTE DỮ LIỆU TỪ DHT11
 * TIMER 2 ĐƯỢC DÙNG Ở ĐÂY ĐỂ ĐO ĐỘ DÀI CỦA MỖI XUNG TÍN HIỆU DATA.
 * NẾU XUNG KÉO DÀI HƠN 45 MICRO GIÂY, NÓ ĐƯỢC HIỂU LÀ BIT 1,
 * NGƯỢC LẠI LÀ BIT 0.
*/
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, byte = 0;
    for(i = 0; i < 8; i++)
    {
        // CHỜ CHO ĐẾN KHI CHÂN DATA XUỐNG MỨC THẤP
        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0);
        // BẮT ĐẦU ĐO THỜI GIAN CHÂN DATA Ở MỨC CAO
        TIM_SetCounter(TIM2, 0);
        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1);
        // NẾU THỜI GIAN LỚN HƠN 45US, BIT LÀ 1, NGƯỢC LẠI LÀ 0
        if(TIM_GetCounter(TIM2) > 45)
            byte = (byte << 1) | 1;
        else
            byte = (byte << 1);
    }
    return byte;
}

// HÀM ĐỌC 5 BYTE DỮ LIỆU TỪ DHT11 (2 BYTE ĐỘ ẨM, 2 BYTE NHIỆT ĐỘ, 1 BYTE CHECKSUM)
uint8_t DHT11_Read_Data(uint8_t *humidity_int, uint8_t *humidity_dec, uint8_t *temp_int, uint8_t *temp_dec)
{
    uint8_t byte[5];
    uint8_t i;
    uint32_t timeout = 0;
    
    // CHỜ DHT11 PHẢN HỒI: BỎ QUA TÍN HIỆU PHẢN HỒI BAN ĐẦU
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1)
    {
        delay_us(1);
        if(++timeout > 100) return 0; // THOÁT NẾU TIMEOUT
    }
    timeout = 0;
    // CHỜ 80US Ở MỨC THẤP
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
    {
        delay_us(1);
        if(++timeout > 100) return 0; // THOÁT NẾU TIMEOUT
    }
    timeout = 0;
    // CHỜ 80US Ở MỨC CAO
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1)
    {
        delay_us(1);
        if(++timeout > 100) return 0; // THOÁT NẾU TIMEOUT
    }
    
    // BẮT ĐẦU ĐỌC 5 BYTE DỮ LIỆU
    for(i = 0; i < 5; i++)
    {
        byte[i] = DHT11_Read_Byte();
    }
    
    // GÁN GIÁ TRỊ ĐỌC ĐƯỢC VÀO CÁC CON TRỎ
    *humidity_int = byte[0];
    *humidity_dec = byte[1];
    *temp_int = byte[2];
    *temp_dec = byte[3];
    
    // TRẢ VỀ GIÁ TRỊ CHECKSUM
    return byte[4]; 
}
