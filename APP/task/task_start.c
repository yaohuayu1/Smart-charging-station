#include "framework.h"
extern void create_task_of_host(void);
extern void create_task_of_listen(void);
extern void create_task_of_ctrl(void);
void cslock_init(void);
void myprintf( char* format, ... );

static int flag_init = 0;
static SemaphoreHandle_t cslock; 
static TaskHandle_t TaskHandle_start; 
extern drvp_rtc_t* drvp_rtc;

void task_start_entry(void* arg)
{
    printf("%s\r\n",__FUNCTION__);
    //初始化myprintf
    cslock_init();
    //创建其他任务
    create_task_of_host();
    create_task_of_listen();
    create_task_of_ctrl();
    
    rtc_dt_t dt;
    dt.Year = 2026;
    dt.Month = 10;
    dt.Date = 22;
    dt.Hour = 19;
    dt.Minute = 48;
    dt.Second = 0;
    
    drvp_rtc->rtc_init( &dt );
    drvp_rtc->rtc_start();
    
    
    rtc_dt_t* nowdt = drvp_rtc->get_dt();
    
    drvp_rtc->rtc_read( nowdt );
    
    
    
    while(1)
    {
        myprintf("%04d-%02d-%02d %02d:%02d:%02d \r\n",
			nowdt->Year,nowdt->Month,nowdt->Date,
			nowdt->Hour,nowdt->Minute,nowdt->Second
		);
        vTaskDelay(1000);
    }
   
}



void create_task_of_start(void)
{
    printf("%s\r\n",__FUNCTION__);
    //创建任务
    xTaskCreate(task_start_entry, (const char *)"start", 1024, NULL, 2, &TaskHandle_start);
}


//初始化锁
void cslock_init(void)
{
    if(flag_init == 1) return;
    cslock = xSemaphoreCreateMutex();
    if(cslock == NULL)
    {
        myprintf("err:%s\r\n",__func__);
        while(1);
    }
    flag_init = 1;
}


void cslock_get(void)
{
    if(flag_init == 0) return;
    //获取cslock，最多等待100个滴答，超时则返回错误。成功返回TRUE
    int ret = xSemaphoreTake(cslock,0xFFFFFFFF);
    if(ret == pdTRUE)
    {
        //说明成功了
    }
    else
    {
        //失败
    }
    
}


void cslock_free(void)
{
    if(flag_init == 0) return;
    //释放互斥锁(cslock)
    xSemaphoreGive(cslock);
}


void myprintf( char* format, ... )
{
    if( flag_init == 0 ) return;
    
    char buf[256];		//最大可以打印的字符串长度
    
    va_list v_args;
    va_start( v_args,format );
    (void)vsnprintf( buf,sizeof(buf),format,v_args );
    va_end( v_args );
    
    //获取锁
    cslock_get();
    //使用互斥资源,因为printf最终调用到uart,指向同一个硬件资源，好几个任务同时使用，是不行的。
    printf("%s",buf);
    //释放锁
    cslock_free();
}
