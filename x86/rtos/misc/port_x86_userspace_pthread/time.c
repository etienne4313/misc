#include <ucos_ii.h>

#define MSEC_PER_SEC 1000u
#define OS_TICKS_PERIOD         (MSEC_PER_SEC/OS_TICKS_PER_SEC)

typedef unsigned long long cycles_t;
typedef uint64_t u64;
	
static cycles_t prev = 0;
static cycles_t calibration = 0;

#define DECLARE_ARGS(val, low, high)    unsigned low, high
#define EAX_EDX_VAL(val, low, high) ((low) | ((u64)(high) << 32))                                                      
#define EAX_EDX_ARGS(val, low, high)    "a" (low), "d" (high)
#define EAX_EDX_RET(val, low, high) "=a" (low), "=d" (high)
#define rdtscll(val) \
	((val) = __native_read_tsc())             
static inline unsigned long long __native_read_tsc(void)                                                      
{
    DECLARE_ARGS(val, low, high);
    asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));                                                               
    return EAX_EDX_VAL(val, low, high);
}

/*
 * IRQ timer tick OR not?
 *
 * We can set a timer IRQ to fire at OS_TICKS_PER_SEC but do we really need
 * to do that?
 *
 * At the end all that does is to force a scheduling decision every so often.
 *
 * For a thread that does while(1) do_stuff; this is usefull bcos otherwise
 * everything would come to a halt except for that one thread.
 *
 * Now that scenario is fairly easy to catch by code inspection and all is
 * needed to avoid starving the other threads is to manually 'tick' the
 * scheduler. Something like: while(1){ do_stuff(); do_tick() }
 *
 * Another more tricky scenarion is where fews tasks are constantly getting
 * re-scheduled amongst themselves. This would prevent the idle task from runnig
 * TODO add hook in scheduler CTX switch to do_tick()
 *
 * do_tick() internally keeps track of monotonic time and upon reaching the
 * OS_TICKS_PERIOD it call OSTimeTick() as if it was the timer IRQ.
 *
 * Most of the architecture support that concept of free running monotonic
 * counter which is function of the CPU frequency. On X86 the TSC counter is
 * such an example.
 *
 * This approach gives a big portability advantage i.e. no need to hook up the
 * timer IRQ. With well behaving task i.e. task calling do_tick() after long
 * execution stretch the 'scheduling outcome' will be identical with OR without
 * Timer IRQ hooked up.
 *
 * On the plus side we can in theory extend the timer wheel to sub 
 * OS_TICKS_PER_SEC precision if need be
 *
 * On the flip side app code needs to track potentially long executing stretch
 * of code. TODO add overflow detection. Also on the flip side, this technique
 * prevent the CPU from going into sleep mode in-between scheduling hops.
 *
 * NOTE that we don't have to worry about concurenty in do_tick() because
 * A) this called from task level only and B) there is no preemption point
 */
void do_tick(void)
{
	static int x = 0;
	cycles_t t;

	rdtscll(t);
	if( (t - prev) > calibration){
		prev = t;
		OSIntEnter();
		OSTimeTick();
		OSIntExit();
		x++;
		if(!(x%100)) /* One second */
			PRINT("TICK %d\n",x);
	}
}

void time_init(void)
{
	int x;
	cycles_t t;

	/* Calibration */
	rdtscll(prev);
	for(x = 0; x < OS_TICKS_PERIOD; x++)
		usleep(1000); /* 1 msec */
	rdtscll(t);
	calibration  = t - prev;

	PRINT("CAL %lld\n", calibration);

	/* Establish base time stamp */
	rdtscll(prev);
}

