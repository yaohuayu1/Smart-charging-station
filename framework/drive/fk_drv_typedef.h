


typedef struct
{
    //设置引脚模式
    void (*set_mode)(int pin,int mode);
    //写引脚状态
    void (*write_pin)(int pin,int value);
    //读引脚状态
    int (*read_pin)(int pin);
    //给引脚附加外部中断
    int (*attach_irq)(int pin,int mode,pin_callback_t cb,void *args);
    //去除外部中断
    int (*detach_irq)(int pin);
    //中断使能
    int (*irq_enable)(int pin,int enabled);
}drv_gpio_t;

extern drv_gpio_t* gpio;

typedef struct
{
    void (*uart_init)(int index,uint32_t baudrate);
    void (*uart_open)(int index);
    void (*uart_close)(int index);
    void (*uart_write)(int index,uint8_t dat);
    int  (*uart_read)( int index,void* buf,int len );
    void (*clr_rxbuf)( int index );
    void (*irq_enable)(int index,int enable);
    
}drv_uart_t;

extern drv_uart_t* uart;
/*
typedef struct {
	uint16_t Year;
	uint8_t  Month;
	uint8_t  Date;
	uint8_t  Day;			//RTC_SUN、RTC_MON、RTC_TUE、RTC_WED、RTC_THU、RTC_FRI、RTC_SAT
	uint8_t  Hour;
	uint8_t  Minute;
	uint8_t  Second;
} RTC_DateTime;

*/
//借助官方sdk的数据类型


typedef struct 
{
    void (*rtc_init)(rtc_dt_t* dt);
    void (*rtc_start)(void);
    void (*rtc_stop)(void);
    rtc_dt_t* (*get_dt)(void);
    void (*rtc_read)(rtc_dt_t* dt);
    
}drv_rtc_t;

extern drv_rtc_t* rtc;
