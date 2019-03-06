#define TIMER_BASE 0x01c20c00
void printf(const char *,...);

typedef struct st_timer {
	unsigned int tmr_irq_en;        //Timer IRQ Enable Register
	unsigned int tmr_irq_sta;       //Timer Status Register
	unsigned int dumy0;
	unsigned int dumy1;
	unsigned int tmr0_ctrl;        //Timer 0 Control Register
	unsigned int tmr0_intv_value;  //Timer 0 Interval Value Register
	unsigned int tmr0_cur_value;   //Timer 0 Current Value Register
	unsigned int dumy2;
	unsigned int tmr1_ctrl;        //Timer 1 Control Register
	unsigned int tmr1_intv_value;  //Timer 1 Interval Value Register
	unsigned int tmr1_cur_value;   //Timer 1 Current Value Register
} timer_t;

#define h3_timer ((volatile timer_t *)TIMER_BASE)

//tmr_irq_en
//[1]
#define TMR0_IRQ_EN (1 << 0)
//[0]
#define TMR1_IRQ_EN (1 << 1)

//tmr_irq_sta bit
//[1]
#define TMR0_IRQ_PEND (1 << 0)
//[0]
#define TMR1_IRQ_PEND (1 << 1)

//tmrx_ctrl
//TMR_MODE bit [7]
#define MODE_CONTINUOUS (0 << 7)
#define MODE_SINGLE     (1 << 7)
//TMR_CLK_PERS [6:4]
#define DIV1   (0 << 4)
#define DIV2   (1 << 4)
#define DIV4   (2 << 4)
#define DIV8   (3 << 4)
#define DIV16  (4 << 4)
#define DIV32  (5 << 4)
#define DIV64  (6 << 4)
#define DIV128 (7 << 4)
//TMR_CLK_SRC [3:2]
#define OSC    (0 << 2)
#define OSC24M (1 << 2)
//TMR_RELOAD  [1]
#define INTERVAL_RELOAD         (1 << 1)
#define INTERVAL_RELOAD_MASK    (1 << 1)
//TMR_EN      [0]
#define TMR_STOP    (0 << 0)
#define TMR_START   (1 << 0)

//tmr_intv_value
//tmr_cur_value

void timer0_init(void) {
	h3_timer->tmr0_intv_value = 24000000;
	h3_timer->tmr0_ctrl = MODE_CONTINUOUS | DIV1 | OSC24M;
	h3_timer->tmr0_ctrl =  h3_timer->tmr0_ctrl | INTERVAL_RELOAD;
	while (h3_timer->tmr0_ctrl & INTERVAL_RELOAD_MASK)
		;
	h3_timer->tmr0_ctrl = h3_timer->tmr0_ctrl | TMR_START;
	h3_timer->tmr_irq_en |= TMR0_IRQ_EN;
}

unsigned int timer0_get(void) {
	return h3_timer->tmr0_cur_value;
}

void mdelay(unsigned int ms)
{
	unsigned int old = timer0_get();
	unsigned int now;
	unsigned int diff;
	unsigned int time_pass = 0;

	while (1) {
		now = timer0_get();
		if ( now > old) {
			old += 24000000;
		}
		diff = old - now;
		time_pass += diff;
		if (ms * 24000 <= time_pass) {
			break;
		} 
		old = now;
	}
}

void timer0_clear_pending(void) {
	h3_timer->tmr_irq_sta = TMR0_IRQ_PEND; //write 1 to clear irq
}

void timer1_init(void) {
	h3_timer->tmr1_intv_value = 48000000;
	h3_timer->tmr1_ctrl = MODE_CONTINUOUS | DIV1 | OSC24M;
	h3_timer->tmr1_ctrl =  h3_timer->tmr1_ctrl | INTERVAL_RELOAD;
	while (h3_timer->tmr1_ctrl & INTERVAL_RELOAD_MASK)
		;
	h3_timer->tmr1_ctrl = h3_timer->tmr0_ctrl | TMR_START;
	h3_timer->tmr_irq_en |= TMR1_IRQ_EN;
}

unsigned int timer1_get(void) {
	return h3_timer->tmr1_cur_value;
}

void timer1_clear_pending(void) {
	h3_timer->tmr_irq_sta = TMR1_IRQ_PEND; //write 1 to clear irq
}
