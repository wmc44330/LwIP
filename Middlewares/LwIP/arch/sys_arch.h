#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#include <os.h>
#include <arch/cc.h>


#if (OS_VERSION >= 30000u)
#define SYS_ARCH_DECL_PROTECT(lev) u32_t lev
#define SYS_ARCH_PROTECT(lev) lev = CPU_SR_Save()
#define SYS_ARCH_UNPROTECT(lev) CPU_SR_Restore(lev)
#endif

#ifdef SYS_ARCH_GLOBALS
#define SYS_ARCH_EXT
#else
#define SYS_ARCH_EXT extern
#endif

#define MAX_QUEUES OS_CFG_MSG_POOL_SIZE // ��Ϣ���������
#define MAX_QUEUE_ENTRIES 20			// ÿ����Ϣ����Ĵ�С

typedef int sys_prot_t;
typedef OS_SEM sys_sem_t;		 // LWIP ʹ�õ��ź���
typedef OS_MUTEX sys_mutex_t;	 // LWIP ʹ�õĻ����ź���
typedef OS_Q sys_mbox_t;		 // LWIP ʹ�õ���Ϣ����,��ʵ���� UCOS �е���Ϣ����
typedef CPU_INT08U sys_thread_t; // �߳� ID,Ҳ�����������ȼ�

#endif
