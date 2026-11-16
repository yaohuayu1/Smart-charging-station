//不用头文件卫士,只包含framework.h

//内存管理
#define mem_get_free() 					xPortGetFreeHeapSize()			//例如 int vfree = mem_get_free();
#define mem_alloc(size_bytes) 	pvPortMalloc(size_bytes)		//eg. u8* buf = (u8*)mem_alloc(100);
#define mem_free(pv) 						vPortFree(pv)								//eg. mem_free(buf);


#include "./lib_loopbuf/loopbuf.h"
//设计上，drive是依赖于drive_port的，所以先包含
#include "./drive_port/fk_drvp_typedef.h"

#include "./drive/fk_drv_typedef.h"

