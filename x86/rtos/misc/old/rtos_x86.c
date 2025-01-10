#include <linux/init.h>
#include <linux/smp.h>
#include <linux/export.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <asm/switch_to.h>

#include <linux/rtos.h>
void x86_rtos_entry(void);

static struct task_struct *kapmd_tsk;
static struct task_struct *idle_tsk;

static int kapmd(void *arg)
{
	int x, y=0;
	struct task_struct *t;

	while(1){
		y++;
		for(x=0; x<1000; x++)
			udelay(1000);
		printk("QQQALIVE %d:%d\n", smp_processor_id(),y);
		switch_to(kapmd_tsk ,idle_tsk, t); 
	}
	return 0;
}

static int __init init(void)
{
	rtos_entry = x86_rtos_entry;
	kapmd_tsk = kthread_create(kapmd, NULL, "kapmd");
	if (IS_ERR(kapmd_tsk)) {
		printk("EER\n");
		return -1;
	}

//	wake_up_process(kapmd_tsk);
	return 0;
}

static void __exit fini(void)
{
}
module_init(init);
module_exit(fini);

/*
 * echo 0 >/sys/devices/system/cpu/cpu7/online
 * echo 1 >/sys/devices/system/cpu/cpu7/online
 *
 * kthread_park
 * kthread_unpark
 *
 * struct rq *rq = cpu_rq(cpu);
 * ttwu_queue(p, cpu, wake_flags);
 * p->sched_class->enqueue_task(rq, p, flags);
 *
 *
 * __schedule
 * 	rq = context_switch(rq, prev, next, &rf);

 */
void x86_rtos_entry(void)
{
	int x, y = 0;
	struct task_struct *t;

	idle_tsk = current;

	rtos_enabled = 0;

	while(!rtos_enabled){
		y++;
		for(x=0; x<1000; x++)
			udelay(1000);
		printk("ALIVE %d:%d\n", smp_processor_id(),y);
		switch_to(idle_tsk,kapmd_tsk ,t); 
	}
	printk("RTOS dead %d\n", smp_processor_id());

	rtos_start_secondary();

	printk("NEVER REACHED RTOS dead %d\n", smp_processor_id());

	/* The exit path much be with online business */
}

MODULE_LICENSE("GPL v2");

