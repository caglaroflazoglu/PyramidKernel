// http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html/

#ifndef __DESCRIPTOR_TABLES_H__
#define __DESCRIPTOR_TABLES_H__

#include <stdint.h>


//Reserved bits in IDT/GDT entries
#define IDT_FLAG_RESERVED                             0x0E


//GDT FIELDS
#define GDT_BASE                                      0x00000000
#define GDT_LIMIT                                     0xFFFFFFFF

//GDT granularity
  // SEGLEN field (segment length)
#define GDT_SEGMENT_LENGTH                            0xF
  // D field (operand size)
#define GDT_OPERAND_SIZE_16                           0
#define GDT_OPERAND_SIZE_32                           1
  // G field (grandularity)
#define GDT_GRANULARITY_1K                            0
#define GDT_GRANULARITY_4K                            1

//GDT access
  // TYPE field (type)
  // (3.4.5.1 of Intel 64 and IA-32 Architectures Developer's Manual)
#define GDT_DATA_TYPE_READ_ONLY                       0x0
#define GDT_DATA_TYPE_READ_ONLY_ACCESSED              0x1
#define GDT_DATA_TYPE_READ_WRITE                      0x2
#define GDT_DATA_TYPE_READ_WRITE_ACCESSED             0x3
#define GDT_DATA_TYPE_READ_ONLY_EXPAND_DOWN           0x4
#define GDT_DATA_TYPE_READ_ONLY_EXPAND_DOWN_ACCESSED  0x5
#define GDT_DATA_TYPE_READ_WRITE_EXPAND_DOWN          0x6
#define GDT_DATA_TYPE_READ_WRITE_EXPAND_DOWN_ACCESSED 0x7
#define GDT_CODE_TYPE_EXEC_ONLY                       0x8
#define GDT_CODE_TYPE_EXEC_ONLY_ACCESSED              0x9
#define GDT_CODE_TYPE_EXEC_READ                       0xA
#define GDT_CODE_TYPE_EXEC_READ_ACCESSED              0xB
#define GDT_CODE_TYPE_EXEC_CONFORMING                 0xC
#define GDT_CODE_TYPE_EXEC_CONFORMING_ACCESSED        0xD
#define GDT_CODE_TYPE_EXEC_READ_CONFORMING            0xE
#define GDT_CODE_TYPE_EXEC_READ_CONFORMING_ACCESSED   0xF
  // DT field (descriptor type)
#define GDT_SYSTEM_DESCRIPTOR                         0
#define GDT_CODE_AND_DATA_DESCRIPTOR                  1
  // DPL field (which ring)
#define GDT_RING0                                     0
#define GDT_RING1                                     1
#define GDT_RING2                                     2
#define GDT_RING3                                     3
  // P field (present)
#define GDT_SEGMENT_NOT_PRESENT                       0
#define GDT_SEGMENT_PRESENT                           1

// PIC - from http://wiki.osdev.org/8259_PIC
#define PIC1            0x20    /* IO base address for master PIC */
#define PIC2            0xA0    /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
#define PIC1_START_INTERRUPT 0x20 /* Master PIC after remapping*/
#define PIC2_START_INTERRUPT 0x28 /* Slave PIC after remapping*/

#define PIC_EOI         0x20    /* End-of-interrupt command code */

#define ICW1_ICW4       0x01    /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08    /* Level triggered (edge) mode */
#define ICW1_INIT       0x10    /* Initialization - required! */

#define ICW4_8086       0x01    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02    /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08    /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C    /* Buffered mode/master */
#define ICW4_SFNM       0x10    /* Special fully nested (not) */


// ---------------------------
// Global Descriptor Tables
// ---------------------------

/**  Access portion of GDT entry.
 *   | 0...3 | 4 | 5... 6 | 7 |
 *   |  Type | DT|   DPL  | P |
 *       Type: Which type?
 *       DT: descriptor type
 *       DPL: which ring (0 to 3)
 *       P: segment present? (1=yes)
 */
struct gdt_access {
    uint8_t type:4;
    uint8_t dt:1;
    uint8_t dpl:2;
    uint8_t p:1;
} __attribute__((packed));
typedef struct gdt_access gdt_access_t;

/** Granularity portion of GDT entry.
 *   | 0..3    | 4 | 5 | 6 | 7 |
 *   | SEGLEN  |   0   | D | G |
 *       SEGLEN: segment length
 *       0: Always 0
 *       D: Operand Size (0 = 16bit, 1 = 32-bit)
 *       G: granularity (0 = 1byte, 1 = 4kbyte)
*/
struct gdt_granularity {
    uint8_t seglen:4;
    uint8_t zero:2;
    uint8_t d:1;
    uint8_t g:1;
} __attribute__((packed));
typedef struct gdt_granularity gdt_gran_t;

/** This structure contains the value of one GDT entry.
 *  Each entry is 64 bits.
 *  | 0 ... 15 | 16 ... 31 | 32 ... 39 | 40...47 | 48...55 | 56...63 |
 *  |limit_low | base_low  |base_middle| access  | gran    |base_high|
 *  limit_low: the lower 16 bits of the limit
 *  base_low: the lower 16 bits of the base
 *  base_middle: the next 8 bits of the base
 *  access: (see above)
 *  gran:  (see above)
 */
struct gdt_entry_struct {
   uint16_t limit_low;
   uint16_t base_low;
   uint8_t  base_middle;
   gdt_access_t  access;
   gdt_gran_t  granularity;
   uint8_t  base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

/* To tell the processor where to find our GDT */
struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

void init_descriptor_tables();





// ---------------------------
// Interrupt Descriptor Tables
// ---------------------------

/** This structure contains the access flags of an IDT entry.
 *  | 0 ... 4 | 5...6 | 7 |
 *  |RESERVED |  DPL  | P |
 *  P: segment is present (1 = Yes)
 *  DPL: which ring (0 to 3)
 *  RESERVED: always 01110 (14)
 */
struct idt_flags {
  uint8_t reserved:5;
  uint8_t dpl:2;
  uint8_t p:1;
} __attribute__((packed));
typedef struct idt_flags idt_flags_t;

/** This structure contains the value of one IDT entry. (similar to GDT entries)
 *  Each entry is 64 bits.
 *  | 0 ... 15 |  16 ... 31     | 32 ... 39 | 40...47 | 48 ... 63 |
 *  | base_low |segment_selector| reserved  | flags   | base_high |
 *  base_low: the lower 16 bits of the base
 *  segment_selector: Segment Selector for destination code segment
 *  reserved: always 0
 *  flags: (defined above)
 *  base_high: higher 16 bits of the base
 */
struct idt_entry {
  uint16_t base_low;
  uint16_t segment_selector;
  uint8_t reserved;
  idt_flags_t flags;
  uint16_t base_high;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_ptr {
  uint16_t limit;
  idt_entry_t *base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;


extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif
