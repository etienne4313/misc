#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/kthread.h>

int ff(void *dat)
{
	while(1){};
}

static int show(struct seq_file *m, void *v)
{
	int cpu = smp_processor_id();
	struct task_struct *t;

	seq_printf(m, "CPU %d\n", cpu);

	t = kthread_run_on_cpu(ff, NULL, 3, "test thread");
	if(!t)
		printk("Err\n");

	return 0;
}

static int __init init(void)
{
	proc_create_single("tt", 0, NULL, show);
	return 0;
}

static void __exit fini(void)
{
	 remove_proc_entry("tt", NULL);
}
module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL v2");

