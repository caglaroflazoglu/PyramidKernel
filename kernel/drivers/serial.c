#include "io.h"
#include "serial.h"

void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00ff);
    outb(SERIAL_DATA_PORT(com), divisor & 0x00ff);
}

void serial_configure_line(unsigned short com){
      /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
       * Content: | d | b | prty  | s | dl  |
       * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
       */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com) {
    //empty if bit 5 of line status IO port is equal to 1
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write(char *buf, unsigned int len) {
    serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
    serial_configure_line(SERIAL_COM1_BASE);

    unsigned int i; 
    for (i=0; i<len; i++) {
        while (!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE));
        outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), buf[i]);
    }
}

