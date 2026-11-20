#include "framework.h"


/*
drvp_rtc实现思路
    静态分布一个变量 m_rtc_dt
    里面存储了RTC的日期时间等信息
    
    在rtc的秒中断里，更新这个变量
    
    drvp_rtc提供该变量的读函数

(1)初始化RTC,开启秒中断
(2)开始函数
(3)停止函数
(4)中断服务函数
每次进入中断，更新变量m_rtc_dt
(5)读函数
*/
static rtc_dt_t m_rtc_dt;
//初始化函数
//输入参数，年月日时分秒
static void rtc_init(rtc_dt_t* dt)
{
    RTC_InitStructure RTC_initStruct;
    
    RTC_initStruct.Year = dt->Year;
    RTC_initStruct.Month = dt->Month;
    RTC_initStruct.Date = dt->Date;
    RTC_initStruct.Hour = dt->Hour;
    RTC_initStruct.Minute = dt->Minute;
    RTC_initStruct.Second = dt->Second;
    RTC_initStruct.SecondIEn = 1;       //秒中断
    RTC_initStruct.MinuteIEn = 0;       //分中断
    RTC_Init(RTC, &RTC_initStruct);
}

static void rtc_start(void)
{
    RTC_Start(RTC);
}
static void rtc_stop(void)
{
    RTC_Stop(RTC);
}

void RTC_Handler(void)
{
    
    if(RTC_INTStat(RTC, RTC_IT_SECOND))
    {//成立，说明触发了秒中断
        RTC_INTClr(RTC, RTC_IT_SECOND);
        //更新m_rtc_dt
        RTC_GetDateTime(RTC, &m_rtc_dt);
    }
    if(RTC_INTStat(RTC, RTC_IT_MINUTE))
    {
        RTC_INTClr(RTC,RTC_IT_MINUTE);
        //执行分中断的代码
    }
}
//获取rtc_dt变量
static rtc_dt_t* get_dt(void)
{
    return &m_rtc_dt;
}
//从硬件读取rtc_dt
static void rtc_read(rtc_dt_t* dt)
{
    RTC_GetDateTime(RTC,dt);
}
    
static drvp_rtc_t do_drvp_rtc = {
    .rtc_init = rtc_init,
    .rtc_start = rtc_start,
    .rtc_stop = rtc_stop,
    .get_dt = get_dt,
    .rtc_read = rtc_read
};

drvp_rtc_t* drvp_rtc = &do_drvp_rtc;


