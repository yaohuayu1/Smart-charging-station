#include "framework.h"

extern drvp_rtc_t* drvp_rtc;

//初始化函数
static void rtc_init( rtc_dt_t* dt )
{
	drvp_rtc->rtc_init( dt );
}

//启动RTC
static void rtc_start(void)
{
	drvp_rtc->rtc_start();
}

//停止RTC
static void rtc_stop(void)
{
	drvp_rtc->rtc_stop();
}									

//获取rtc_dt变量
static rtc_dt_t* get_dt(void)
{
	return drvp_rtc->get_dt();
}	

//从硬件读取rtc_dt
static void rtc_read( rtc_dt_t* dt )
{
	drvp_rtc->rtc_read( dt );
}

static drv_rtc_t do_drv_rtc={
	.rtc_init = rtc_init,
	.rtc_start = rtc_start,
	.rtc_stop = rtc_stop,
	.get_dt = get_dt,
	.rtc_read = rtc_read,
};

drv_rtc_t* rtc = &do_drv_rtc;

