#include <stdint.h>

extern int main(void);

/* linker symbols */
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;

void Reset_Handler(void);

__attribute__((section(".isr_vector")))
const void* vector_table[] = {(void*)(0x20000000 + 20 * 1024), Reset_Handler};

void Reset_Handler(void)
{
    // Copy data từ flash -> RAM
    uint32_t* src = &_sidata;
    uint32_t* dst = &_sdata;

    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    // Clear bss
    dst = &_sbss;
    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    main();

    while (1)
    {
    };
}