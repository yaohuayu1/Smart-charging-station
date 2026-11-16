#include "framework.h"

extern drvp_uart_t* drvp_uart;



/*

需要分配环形缓冲区
需要实现串口接收的回调函数，并且安装到drvp_uart
*/
/*-------------------------------------------------------------------------*/
//环形缓冲区
static loopbuf_t* lb_uart_rx[_e_max_uart];
static void rev_callback(int index,uint8_t dat)
{
    loopbuf_write( lb_uart_rx[index], &dat,1 );
}
static void uart_init(int index,uint32_t baudrate)
{
    int err = 0;        //记录错误值
    loopbuf_t* tlb = NULL;
    if(index >= _e_max_uart)
    {
        err = 1;        //说明是index不合法
        goto __deal_err_;
    }
    //初始化缓冲区
    tlb = loopbuf_init(1024);
    if(!tlb)
    {
        err = 2;        //说明是缓冲区出错
        goto __deal_err_;
    }
    lb_uart_rx[index] = tlb;
    //安装中断回调函数
    drvp_uart->set_int_rxfunc(index,rev_callback);
    
    //初始化外设
    drvp_uart->uart_init(index,baudrate);
    
    return;
    __deal_err_:
    printf("err[%d]:%s(%d)\r\n",err,__func__,index );
    while(1);
}
/*-------------------------------------------------------------------------*/
static void uart_open(int index)
{
    drvp_uart->uart_open(index);
}
/*-------------------------------------------------------------------------*/
static void uart_close(int index)
{
    drvp_uart->uart_close(index);
}
/*-------------------------------------------------------------------------*/
static void uart_write(int index,uint8_t dat)
{
    drvp_uart->uart_write(index,dat);
}
/*-------------------------------------------------------------------------*/
//index 索引号
//buf 用于保存数据的缓冲区
//len 本次想要读取多少个数据
//返回值 返回真实读取到的数据长度
static int uart_read( int index,void* buf,int len )
{

    return loopbuf_read( lb_uart_rx[index],buf,len );

}
static void clr_rxbuf( int index )
{
    loopbuf_reset( lb_uart_rx[index] );
}

static void irq_enable(int index,int enable)
{
    drvp_uart->irq_enable(index,enable);
}


static drv_uart_t do_drv_uart = 
{
    .uart_init = uart_init,
    .uart_open = uart_open,
    .uart_close = uart_close,
    .uart_write = uart_write,
    .uart_read = uart_read,
    .clr_rxbuf = clr_rxbuf,
    .irq_enable = irq_enable
};

drv_uart_t* uart = &do_drv_uart;


