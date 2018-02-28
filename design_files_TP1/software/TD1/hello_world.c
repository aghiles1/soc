/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */


#include <stdio.h>
#include "altera_avalon_pio_regs.h"
#include <unistd.h>
#include "system.h"
#include "sys/alt_irq.h"
volatile int edge_capture;
static void handle_button_interrupts(void* context, alt_u32 id);
static void init_button_pio ();
int recupererBit(unsigned long buttons);


int main()
{
 int LUT[16] = {0xc0, 0xf9, 0x24, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0x46, 0xa1, 0x86, 0x8e};
 int afficheur[6] = {0x8005080, 0x8005070, 0x8005020, 0x8005040, 0x8005050, 0x8005060};
 int button = 0x8005030;
 int switche = 0x8005090;
 int led = 0x80050a0;
 printf("Hello from Nios II!\n");
 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[4], LUT[0]);
 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[5], LUT[0]);
 init_button_pio();
 for(int i=0; i < 10000;i++)
 {
	 //affichage du compteur de 0 à 9999 sur les 4 premier segments /*
	 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[0], LUT[i%10]);
	 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[1], LUT[(i/10)%10]);
	 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[2], LUT[(i/100)%10]);
	 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[3], LUT[(i/1000)]);

	 IOWR_ALTERA_AVALON_PIO_DATA(afficheur[4],LUT[ recupererBit(edge_capture)]);

	 int valueOfTheSwitches = IORD_ALTERA_AVALON_PIO_DATA(switche);
	 IOWR_ALTERA_AVALON_PIO_DATA(led, valueOfTheSwitches);
	 //tester les valeur des switchs
     //int data = IORD_ALTERA_AVALON_PIO_DATA(switche);
     //int data1 = IORD_ALTERA_AVALON_PIO_DATA(button);
	 //allumer la led R0, (0xff pour tout allumer)
	 //IOWR_ALTERA_AVALON_PIO_DATA(led, 0x0);

	 usleep(100000);
 }
 //IOWR_ALTERA_AVALON_PIO_DATA(0x8005080, 0xc0);
 //int data = IORD_ALTERA_AVALON_PIO_DATA(0x8005090);  return 0;
}
static void handle_button_interrupts(void* context, alt_u32 id)
{
	/* Cast context to edge_capture's type. It is important that this be
      * declared volatile to avoid unwanted compiler optimization.
      */
     volatile int* edge_capture_ptr = (volatile int*) context;
     /* Store the value in the Button's edge capture register in *context. */
     *edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(0x8005030);

     //printf("%d\n",IORD_ALTERA_AVALON_PIO_EDGE_CAP(0x8005030));
     /* Reset the Button's edge capture register. */
     IOWR_ALTERA_AVALON_PIO_EDGE_CAP(0x8005030, 0);
}

static void init_button_pio (){
	/* Recast the edge_capture pointer to match alt_irq_register() function prototype. */
	void * edge_capture_ptr = ( void *) &edge_capture;
	/* Enable all 4 button interrupts. */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK (0x8005030, 0xf);
	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP (0x8005030, 0x0);
	/* Register the ISR. */
	alt_irq_register( KEY_IRQ,edge_capture_ptr,handle_button_interrupts );
}

int recupererBit(unsigned long button)
{
    unsigned bit = 0;
    int j = 1;
    if(button == 1)
    	return 0;

    while( (button = (button >> 1)) != 1 && (j < 8) ){
		j++;
    }
    return j;
}
