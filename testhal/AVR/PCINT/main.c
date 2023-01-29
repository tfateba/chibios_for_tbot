/*
    ChibiOS - Copyright (C) 2016 Theodore Ateba

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

/**
 * @brief Global variables.
 */
BaseSequentialStream* chp = (BaseSequentialStream*) &SD1;
static int isrCpt = 0; /**< Interrupt counter. */
static int index = 0;

void pcintInit(void) {

  //cli();

  //PCICR |= 1; // ENABLE PCINT7 to PCINT0
  //PCMSK0 = 0xFF; // PCINT7 to PCINT0 are enabled

  PCICR   |= (1 << PCIE0); // ENABLE PCINT7 to PCINT0
  PCMSK0  |= (1 << PCINT5); // PCINT5

  //sei();
}

/**
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();


  /*
   * External interrupt on the Pin D2 of the Arduino Mega2560.
   */
  palSetPadMode(IOPORT5, PD0, PAL_MODE_INPUT); // INT0

  /*
   * Start the serial driver.
   */
  sdStart(&SD1, NULL);

  palClearPad(IOPORT2, PORTB_LED1);
  chprintf(chp, "\n\r EXT hal test started...");

  pcintInit();

  /*
   * Normal main() thread activity, in this demo it print IRQ counter every
   * seconde.
   */
  while (TRUE) {
    chThdSleepMilliseconds(1000);
    chprintf(chp, "\n\r index = %d, ISR Counter = %d", index, isrCpt);
    index++;
  }
}

ISR(PCINT0_vect) {
  isrCpt++;
}
