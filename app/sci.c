//
// Created by legath on 19.09.18.
//

#include "sci.h"

void sci_init(uint32 baudrate, uint32 workmask) {
    /** - bring SCI out of reset */
    sciREG->GCR0 = 0U;
    sciREG->GCR0 = 1U;

    /** - Disable all interrupts */
    sciREG->CLEARINT = 0xFFFFFFFFU;
    sciREG->CLEARINTLVL = 0xFFFFFFFFU;

    /** - global control 1 */
    sciREG->GCR1 = (uint32) ((uint32) 1U << 25U)  /* enable transmit */
                   | (uint32) ((uint32) 1U << 24U)  /* enable receive */
                   | (uint32) ((uint32) 1U << 5U)   /* internal clock (device has no clock pin) */
                   | (uint32) ((uint32) (2U - 1U) << 4U)  /* number of stop bits */
                   | (uint32) ((uint32) 0U << 3U)  /* even parity, otherwise odd */
                   | (uint32) ((uint32) 0U << 2U)  /* enable parity */
                   | (uint32) ((uint32) 1U << 1U);  /* asynchronous timing mode */

    /** - set baudrate
     * BRS is a prescaller
     * */
    float64 vclk = 110.000 * 1000000.0; //TODO: remove hardcode
    uint32 f = ((sciREG->GCR1 & 2U) == 2U) ? 16U : 1U;
    uint32 temp;
    float64 temp2;
    /*SAFETYMCUSW 96 S MR:6.1 <APPROVED> "Calculations including int and float cannot be avoided" */
    temp = (f * (baudrate));
    temp2 = ((vclk) / ((float64) temp)) - 1U;
    temp2 = floor(temp2 + 0.5);        /* Rounding-off to the closest integer */
    sciREG->BRS = (uint32) ((uint32) temp2 & 0x00FFFFFFU);

    /** - transmission length */
    sciREG->FORMAT = 8U - 1U;  /* length */

    /** - set SCI pins functional mode */
    sciREG->PIO0 = (uint32) ((uint32) 1U << 2U)  /* tx pin */
                   | (uint32) ((uint32) 1U << 1U); /* rx pin */

    /** - set SCI pins default output value */
    sciREG->PIO3 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                   | (uint32) ((uint32) 0U << 1U); /* rx pin */

    /** - set SCI pins output direction */
    sciREG->PIO1 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                   | (uint32) ((uint32) 0U << 1U); /* rx pin */

    /** - set SCI pins open drain enable */
    sciREG->PIO6 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                   | (uint32) ((uint32) 0U << 1U); /* rx pin */

    /** - set SCI pins pullup/pulldown enable */
    sciREG->PIO7 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                   | (uint32) ((uint32) 0U << 1U); /* rx pin */

    /** - set SCI pins pullup/pulldown select */
    sciREG->PIO8 = (uint32) ((uint32) 1U << 2U)  /* tx pin */
                   | (uint32) ((uint32) 1U << 1U);  /* rx pin */

    /** - set interrupt level */
    sciREG->SETINTLVL = (uint32) ((uint32) 0U << 26U)  /* Framing error */
                        | (uint32) ((uint32) 0U << 25U)  /* Overrun error */
                        | (uint32) ((uint32) 0U << 24U)  /* Parity error */
                        | (uint32) ((uint32) 0U << 9U)  /* Receive */
                        | (uint32) ((uint32) 0U << 8U)  /* Transmit */
                        | (uint32) ((uint32) 0U << 1U)  /* Wakeup */
                        | (uint32) ((uint32) 0U << 0U);  /* Break detect */

    /** - set interrupt enable */
    sciREG->SETINT = (uint32) ((uint32) 0U << 26U)  /* Framing error */
                     | (uint32) ((uint32) 0U << 25U)  /* Overrun error */
                     | (uint32) ((uint32) 0U << 24U)  /* Parity error */
                     | (uint32) ((uint32) 0U << 9U)  /* Receive */
                     | (uint32) ((uint32) 0U << 1U)  /* Wakeup */
                     | (uint32) ((uint32) 0U << 0U);  /* Break detect */

    /** - Finaly start SCI */
    sciREG->GCR1 |= 0x80U;

}

void scilin_init(void) {
    scilinREG->GCR0 = 0U;
    scilinREG->GCR0 = 1U;

    /** - Disable all interrupts */
    scilinREG->CLEARINT = 0xFFFFFFFFU;
    scilinREG->CLEARINTLVL = 0xFFFFFFFFU;

    /** - global control 1 */
    scilinREG->GCR1 = (uint32) ((uint32) 1U << 25U)  /* enable transmit */
                      | (uint32) ((uint32) 1U << 24U)  /* enable receive */
                      | (uint32) ((uint32) 1U << 5U)   /* internal clock (device has no clock pin) */
                      | (uint32) ((uint32) (2U - 1U) << 4U)  /* number of stop bits */
                      | (uint32) ((uint32) 0U << 3U)  /* even parity, otherwise odd */
                      | (uint32) ((uint32) 0U << 2U)  /* enable parity */
                      | (uint32) ((uint32) 1U << 1U);  /* asynchronous timing mode */

    /** - set baudrate */
    scilinREG->BRS = 715U;  /* baudrate */

    /** - transmission length */
    scilinREG->FORMAT = 8U - 1U;  /* length */

    /** - set SCI pins functional mode */
    scilinREG->PIO0 = (uint32) ((uint32) 1U << 2U)  /* tx pin */
                      | (uint32) ((uint32) 1U << 1U); /* rx pin */


    /** - set SCI pins default output value */
    scilinREG->PIO3 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                      | (uint32) ((uint32) 0U << 1U); /* rx pin */


    /** - set SCI pins output direction */
    scilinREG->PIO1 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                      | (uint32) ((uint32) 0U << 1U); /* rx pin */


    /** - set SCI pins open drain enable */
    scilinREG->PIO6 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                      | (uint32) ((uint32) 0U << 1U); /* rx pin */


    /** - set SCI pins pullup/pulldown enable */
    scilinREG->PIO7 = (uint32) ((uint32) 0U << 2U)  /* tx pin */
                      | (uint32) ((uint32) 0U << 1U); /* rx pin */


    /** - set SCI pins pullup/pulldown select */
    scilinREG->PIO8 = (uint32) ((uint32) 1U << 2U)  /* tx pin */
                      | (uint32) ((uint32) 1U << 1U); /* rx pin */


    /** - set interrupt level */
    scilinREG->SETINTLVL = (uint32) ((uint32) 0U << 26U)  /* Framing error */
                           | (uint32) ((uint32) 0U << 25U)  /* Overrun error */
                           | (uint32) ((uint32) 0U << 24U)  /* Parity error */
                           | (uint32) ((uint32) 0U << 9U)  /* Receive */
                           | (uint32) ((uint32) 0U << 8U)  /* Transmit */
                           | (uint32) ((uint32) 0U << 1U)  /* Wakeup */
                           | (uint32) ((uint32) 0U);  /* Break detect */

    /** - set interrupt enable */
    scilinREG->SETINT = (uint32) ((uint32) 0U << 26U)  /* Framing error */
                        | (uint32) ((uint32) 0U << 25U)  /* Overrun error */
                        | (uint32) ((uint32) 0U << 24U)  /* Parity error */
                        | (uint32) ((uint32) 0U << 9U)  /* Receive */
                        | (uint32) ((uint32) 0U << 1U)  /* Wakeup */
                        | (uint32) ((uint32) 0U);  /* Break detect */


    /** - Finaly start SCILIN */
    scilinREG->GCR1 |= 0x80U;

}