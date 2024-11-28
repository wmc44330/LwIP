#include "sys_arch.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <os_cfg_app.h>
#include <string.h>

const void *const NullPointer = (mem_ptr_t *)0xFFFFFFFF;

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	OS_ERR err = OS_ERR_NONE;
	OSSemCreate((OS_SEM      *)sem,
			    (CPU_CHAR    *)"LWIP SEM",
			    (OS_SEM_CTR   )count,
			    (OS_ERR      *)&err);
	if(err != OS_ERR_NONE)
		return ERR_VAL;
	LWIP_ASSERT("OSSemCreate ", sem != NULL);
	return ERR_OK;
}

void sys_sem_signal(sys_sem_t *sem)
{
	OS_ERR err = OS_ERR_NONE;
	OSSemPost(sem, OS_OPT_POST_1, &err); // �����ź���
	LWIP_ASSERT("OSSemPost ", err == OS_ERR_NONE);
}


u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	OS_ERR err = OS_ERR_NONE;
	OS_TICK tick = 0;
	CPU_TS start_ts,end_ts,delta_ts;
	
	if(timeout)
		tick = timeout * OS_CFG_TICK_RATE_HZ /1000;
	
	start_ts = OSTimeGet(&err);
	
	OSSemPend(sem,tick,OS_OPT_PEND_BLOCKING,0,&err);	// �����ȴ�
	
	if(err == OS_ERR_TIMEOUT)
		 return SYS_ARCH_TIMEOUT;
	
	end_ts = OSTimeGet(&err);
	
	delta_ts = (end_ts < start_ts) ? 0XFFFFFFFF - start_ts + end_ts : end_ts - start_ts;
	
	return (delta_ts * 1000 / OS_CFG_TICK_RATE_HZ + 1);
}

void sys_sem_free(sys_sem_t *sem)
{
	OS_ERR err = OS_ERR_NONE;
	OSSemDel(sem, OS_OPT_DEL_ALWAYS, &err);
	LWIP_ASSERT("OSSemDel ", err == OS_ERR_NONE);
	sem = NULL;
}

int sys_sem_valid(sys_sem_t *sem)
{
	if (sem->NamePtr)
		return (strcmp(sem->NamePtr, "?SEM")) ? 1 : 0;
	return 0;
}


void sys_sem_set_invalid(sys_sem_t *sem)
{
	if (sys_sem_valid(sem))
		sys_sem_free(sem);
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	if(size > MAX_QUEUE_ENTRIES)
		size = MAX_QUEUE_ENTRIES;
	OS_ERR err = OS_ERR_NONE;
	OSQCreate((OS_Q        *)mbox,
			  (CPU_CHAR    *)"LWIP QUEUE",
			  (OS_MSG_QTY   )size,
			  (OS_ERR      *)&err);
	if(err != OS_ERR_NONE)
		return ERR_VAL;
	LWIP_ASSERT("OSQCreate ", mbox != NULL);
	return ERR_OK;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	OS_ERR err = OS_ERR_NONE;
	if(msg == NULL)
		msg = (void *)&NullPointer;
	u8_t i;
	for(i = 0;i < 10;i++)
	{
		OSQPost(mbox, msg,strlen(msg),OS_OPT_POST_FIFO, &err); // ������Ϣ����
		if(err == OS_ERR_NONE)
			break;
		OSTimeDly(5u,OS_OPT_TIME_DLY,&err);
	}
	LWIP_ASSERT("sys_mbox_post error!\n", i != 10);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	OS_ERR err = OS_ERR_NONE;
	if (msg == NULL)
		msg = (void *)&NullPointer;
	OSQPost(mbox, msg,sizeof(msg),OS_OPT_POST_FIFO, &err); // ������Ϣ����
	if (err != OS_ERR_NONE)
		return ERR_VAL;
	return ERR_OK;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	return sys_arch_mbox_fetch(mbox, msg, 1); 				// ���Ի�ȡһ����Ϣ
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	OS_ERR err = OS_ERR_NONE;
	OS_TICK tick = 0;
	CPU_TS start_ts,end_ts,delta_ts;
	void *recv = NULL;
	OS_MSG_SIZE size;
	
	if(timeout)
		tick = timeout * OS_CFG_TICK_RATE_HZ /1000;
	
	start_ts = OSTimeGet(&err);
	
	
	recv = OSQPend(mbox,tick,OS_OPT_PEND_BLOCKING,&size,0,&err);	// �����ȴ�
	
	if(msg)
		*msg = (recv == (void *)&NullPointer) ? NULL : recv;
	
	if(err == OS_ERR_TIMEOUT)
		 return SYS_ARCH_TIMEOUT;
	
	end_ts = OSTimeGet(&err);
	
	delta_ts = (end_ts < start_ts) ? 0XFFFFFFFF - start_ts + end_ts : end_ts - start_ts;
	
	return (delta_ts * 1000 / OS_CFG_TICK_RATE_HZ + 1);
}

void sys_mbox_free(sys_mbox_t *mbox)
{
	OS_ERR err = OS_ERR_NONE;
#if OS_CFG_Q_FLUSH_EN > 0u
	OSQFlush(mbox, &err);
#endif
	OSQDel((OS_Q *)mbox,
		   (OS_OPT)OS_OPT_DEL_ALWAYS,
		   (OS_ERR *)&err);
	LWIP_ASSERT("OSQDel ", err == OS_ERR_NONE);
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
	if (mbox->NamePtr)
		return (strcmp(mbox->NamePtr, "?Q")) ? 1 : 0;
	return 0;
}


void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	if (sys_mbox_valid(mbox))
		sys_mbox_free(mbox);
}

CPU_STK AppTaskTcpipStk[TCPIP_THREAD_STACKSIZE]; // TCP IP �ں������ջ,��lwip_comm��������
// LWIP �ں������������ƿ�
OS_TCB AppTaskTcpipTCB;

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	if (strcmp(name, TCPIP_THREAD_NAME) == 0) // ���� TCP IP �ں�����
	{
		OS_CRITICAL_ENTER(); // �����ٽ���
		// ������ʼ����
		OSTaskCreate((OS_TCB *)&AppTaskTcpipTCB, // ������ƿ�
					 (CPU_CHAR *)"TCP/IP Task",	 // ��������
					 (OS_TASK_PTR)thread,		 // ������
					 (void *)0,					 // ���ݸ��������Ĳ���
					 (OS_PRIO)prio,				 // �������ȼ�
					 (CPU_STK *)&AppTaskTcpipStk[0],
					 (CPU_STK_SIZE)stacksize / 10, // �����ջ�����λ
					 (CPU_STK_SIZE)stacksize,	   // �����ջ��С
					 (OS_MSG_QTY)0,
					 (OS_TICK)0,
					 (void *)0,																  // �û�����Ĵ洢��
					 (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP, // ����ѡ��
					 (OS_ERR *)&err);														  // ��Ÿú�������ʱ�ķ���ֵ
		OS_CRITICAL_EXIT();																	  // �˳��ٽ���
	}
	return 0;
}

void sys_init(void)
{
}

u32_t sys_now(void){
	OS_ERR err = OS_ERR_NONE;
	OS_TICK tick = OSTimeGet(&err);							// ��ȡ��ǰϵͳʱ�� �õ����� UCOS �Ľ�����
	return (u32_t)(tick * 1000 / OS_CFG_TICK_RATE_HZ + 1); 	// ��������ת��Ϊ LWIP ��ʱ��	
}

