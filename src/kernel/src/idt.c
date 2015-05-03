/*
 * idt.c

 *
 *  Created on: 02.05.2015
 *      Author: leonhardt
 */

#include "stdint.h"
#include "idt.h"
#include "kprintf.h"

#define PIC1							0x20		//IO address master PIC
#define PIC2							0xA0		//IO address slave PIC

#define PIC1_COMMAND					PIC1
#define PIC1_DATA						(PIC1+1)

#define PIC2_COMMAND					PIC2
#define PIC2_DATA						(PIC2+1)

#define ICW1_ICW4						0x01
#define ICW1_SINGLE						0x02
#define ICW1_INTERVAL4					0x04
#define ICW1_LEVEL						0x08
#define ICW1_INIT						0x10

#define ICW4_8086						0x01
#define ICW4_AUTO						0x02
#define ICW4_BUF_SLAVE					0x08
#define ICW4_BUF_MASTER					0x0C
#define ICW4_SFNM						0x10
#define PIC_EOI							0x20

#define IDT_ENTRIES 					256
#define IDT_INTERRUPT_GATE 				0x0E
#define IDT_TRAP_GATE	 				0x0F
#define IDT_RING0						0x00
#define IDT_RING3						0x60
#define IDT_PRESENT_FLAG				0x80

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);

//18 - 31 unused
extern void isr32(void);
extern void isr33(void);
//TODO: Add all 15 ISRs
extern void isr48(void);

typedef struct __attribute__((packed)){
	uint16_t offset_low;
	uint16_t segment_selector;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_high;

} idt_gate_t;

typedef struct __attribute__((packed)){
	uint16_t limit;
	void* pointer;
} idt_pointer_t;

static idt_gate_t idt[IDT_ENTRIES];
static idt_pointer_t idtp = {
	.limit = IDT_ENTRIES * sizeof(idt_gate_t) -1,
	.pointer = idt
};

static inline void outb(uint16_t port, uint8_t value)
{
	asm volatile ("outb %0, %1" : : "a" (value), "Nd" (port));
}


void init_pic(uint8_t offset_pic1, uint8_t offset_pic2)
{
	//Initialize master PIC
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);

	//Initialize slave PIC
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	outb(PIC1_DATA, offset_pic1);
	outb(PIC1_DATA, 0x04);


	outb(PIC2_DATA, offset_pic2);
	outb(PIC2_DATA, 0x02);

	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	//Clear IMR to activate all IRQs
	outb(PIC1_COMMAND, 0x0);
	outb(PIC2_COMMAND, 0x0);
}

void idt_set_gate(int i, void (*handler)(), uint16_t segment, uint8_t flags)
{
	uint32_t offset = (uint32_t)handler;
	idt[i].offset_low = (offset & 0xFFFF);
	idt[i].offset_high = (offset >> 16) & 0xFF;
	idt[i].segment_selector = segment;
	idt[i].flags = flags;
}

void idt_init(void)
{
	init_pic(32, 40);
	// 0x8 code segment
	idt_set_gate(0, isr0, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(1, isr1, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(2, isr2, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(3, isr3, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(4, isr4, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(5, isr5, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(6, isr6, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(7, isr7, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(8, isr8, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(9, isr9, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(10, isr10, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(11, isr11, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(12, isr12, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(13, isr13, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(14, isr14, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(15, isr15, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(16, isr16, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(17, isr17, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(18, isr18, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);

	idt_set_gate(32, isr32, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(33, isr33, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT_FLAG);
	idt_set_gate(48, isr48, 0x8, IDT_INTERRUPT_GATE | IDT_RING3 | IDT_PRESENT_FLAG);

	asm volatile("lidt %0" : : "m"(idtp));
	asm volatile("sti");
}

void tmp_isr(cpu_context_t* cpu)
{
	if (cpu->interrupt_num <= 31) {
		kprintf("Exception occurred; %x", cpu->interrupt_num);
		asm volatile("cli; hlt");
	} else if (cpu->interrupt_num >= 32 && cpu->interrupt_num <= 47) {
		if (cpu->interrupt_num >= 40) {
			outb(PIC2, PIC_EOI);
		}
		outb(PIC1, PIC_EOI);
	} else if (cpu->interrupt_num == 48) {
			kprintf("Syscall: %x\n", cpu->interrupt_num);
	} else {
		kprintf("Unknown interrupt: %x", cpu->interrupt_num);
		asm volatile("cli; hlt");
	}
}
