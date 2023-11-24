#include "AHS_TIMER.h"

void timer0_init(timer_mode_def_t timer_mode)
{
    cli();

    switch (timer_mode)
    {
    case NORMAL:
        /* Timer 0 Normal mode */
        TCCR0A &= ~(1 << WGM01);
        TCCR0A &= ~(1 << WGM00);
        TCCR0B &= ~(1 << WGM02);

        /* Set prescaler value */
        TCCR0B |= (1 << CS02);
        TCCR0B &= ~(1 << CS01);
        TCCR0B &= ~(1 << CS00);

        /* Set Compare output mode  */
        TCCR0A |= (1 << COM0A0);
        TCCR0A &= ~(1 << COM0A1);

        /* Enable Timer0 Output Compare Interrupt */
        TIMSK0 |= (1 << OCIE0A);

        /* Enable overflow interrupt */
        // TIMSK0 |= (1 << TOIE0);
        break;

    case CTC:
        /* Timer 0 CTC mode */
        TCCR0A |= (1 << WGM01);
        TCCR0A &= ~(1 << WGM00);
        TCCR0B |= ~(1 << WGM02);

        /* Set prescaler value */
        TCCR0B |= (1 << CS02);
        TCCR0B &= ~(1 << CS01);
        TCCR0B &= ~(1 << CS00);

        /* Enable Timer0 Output Compare Interrupt */
        TIMSK0 |= (1 << OCIE0A);

        break;

    case PWM_PHASE_FREQ:
        /* Timer 0 PWM Phase Correct Mode */
        TCCR0A &= ~(1 << WGM01);
        TCCR0A |= (1 << WGM00);
        TCCR0B |= (1 << WGM02);

        /* Set prescaler value */
        TCCR0B &= ~(1 << CS02);
        TCCR0B |= (1 << CS01);
        TCCR0B |= (1 << CS00);

        /* Set Compare output mode  */
        TCCR0A |= (1 << COM0A0);
        TCCR0A &= ~(1 << COM0A1);

        /* Set Compare output mode  */
        TCCR0A &= ~(1 << COM0B0);
        TCCR0A |= (1 << COM0B1);

        // TIMSK0 |= (1 << OCIE0B);

        break;

    default:
        break;
    }
    sei();
}

void timer1_init(timer_mode_def_t timer_mode)
{
    cli();

    switch (timer_mode)
    {
    case NORMAL:
        /* Timer 1 Normal mode */
        TCCR1A &= ~(1 << WGM11);
        TCCR1A &= ~(1 << WGM10);
        TCCR1B &= ~(1 << WGM12);
        TCCR1B &= ~(1 << WGM13);

        /* Set prescaler value --> 2MHz */
        TCCR1B &= ~(1 << CS12);
        TCCR1B &= ~(1 << CS11);
        TCCR1B |= (1 << CS10);

        /* Set Compare output mode  */
        // TCCR1A |= (1 << COM1A0);
        // TCCR1A &= ~(1 << COM1A1);

        /* Set ICP1 capture edge - Rising edge detection */
        TCCR1B |= (1 << ICES1);

        /* Enable input capture pin interrupt */
        TIMSK1 |= (1 << ICIE1);

        /* Enable Timer0 Output Compare Interrupt */
        // TIMSK0 |= (1 << OCIE0A);

        /* Enable overflow interrupt */
        // TIMSK0 |= (1 << TOIE0);
        break;

    case CTC:
        /* Timer 1 CTC mode */
        TCCR1A &= ~(1 << WGM11);
        TCCR1A &= ~(1 << WGM10);
        TCCR1B |= (1 << WGM12);
        TCCR1B &= ~(1 << WGM13);

        /* Set prescaler value --> 2MHz */
        TCCR1B &= ~(1 << CS12);
        TCCR1B |= (1 << CS11);
        TCCR1B &= ~(1 << CS10);

        /* Set Compare output mode  */
        TCCR1A |= (1 << COM1A0);
        TCCR1A &= ~(1 << COM1A1);

        /* Set ICP1 capture edge - Rising edge detection */
        TCCR1B |= (1 << ICES1);

        /* Enable input capture pin interrupt */
        TIMSK1 |= (1 << ICIE1);

        /* Enable Timer0 Output Compare Interrupt */
        TIMSK1 |= (1 << OCIE1A);

        break;

    case PWM_PHASE_FREQ:
        /* Timer 1 PWM Phase Correct Mode */
        TCCR0A &= ~(1 << WGM01);
        TCCR0A |= (1 << WGM00);
        TCCR0B |= (1 << WGM02);

        /* Set prescaler value */
        TCCR0B &= ~(1 << CS02);
        TCCR0B |= (1 << CS01);
        TCCR0B |= (1 << CS00);

        /* Set Compare output mode  */
        TCCR0A |= (1 << COM0A0);
        TCCR0A &= ~(1 << COM0A1);

        /* Set Compare output mode  */
        TCCR0A |= (1 << COM0B0);
        TCCR0A |= (1 << COM0B1);

        break;

    case FAST_PWM:
        /* Timer 1 CTC mode */
        TCCR1A &= ~(1 << WGM11);
        TCCR1A &= ~(1 << WGM10);
        TCCR1B |= (1 << WGM12);
        TCCR1B &= ~(1 << WGM13);

        /* Set prescaler value --> 2MHz */
        TCCR1B &= ~(1 << CS12);
        TCCR1B |= (1 << CS11);
        TCCR1B &= ~(1 << CS10);

        /* Set Compare output mode  */
        TCCR1A |= (1 << COM1A0);
        TCCR1A &= ~(1 << COM1A1);

        /* Set ICP1 capture edge - Rising edge detection */
        TCCR1B |= (1 << ICES1);

        /* Enable input capture pin interrupt */
        TIMSK1 |= (1 << ICIE1);

        /* Enable Timer0 Output Compare Interrupt */
        TIMSK1 |= (1 << OCIE1A);
        break;

    default:
        break;
    }
    sei();
}

void timer2_init(timer_mode_def_t timer_mode)
{
    /* Enable timer 2 in NORMAL mode with channel 2 output */
    TCCR2A &= ~(1 << WGM21);
    TCCR2A &= ~(1 << WGM20);
    TCCR2B &= ~(1 << WGM22);

	/**********************************************************************
	 * Set prescaler value 16Mhz/128 --> 125Khz 
	 *
	 * NOTE: Prescaler values are commented out to disable timer at the 
	 * start of the game. To trigger/toggle the buzzer, set the prescaler
	 * in the corresponding registers and then disable them by clearing 
	 * the register values
	 **********************************************************************/
    // TCCR2B |= (1 << CS22);
    // TCCR2B &= ~(1 << CS21);
    // TCCR2B |= (1 << CS20);

    /* Set Compare output mode - channel B (OC2B)  */
    TCCR2A |= (1 << COM2B0);
    TCCR2A &= ~(1 << COM2B1);
}
