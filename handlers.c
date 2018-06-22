#include "decls.h"
#include <stdbool.h>

#define LEFT_SHIFT 16

/**
 * Handler para el timer (IRQ0). Escribe un carÃ¡cter cada segundo.
 */
static const uint8_t hz_ratio = 18;  // Default IRQ0 freq (18.222 Hz).

void timer() {
	static char chars[81];
	static unsigned ticks;
	static int8_t line = 20;
	static uint8_t idx = 0;

	if (++ticks % hz_ratio == 0) {
		chars[idx] = '.';
		chars[++idx] = '\0';
		vga_write(chars, line, 0x07);
	}

	if (idx >= sizeof(chars)) {
		line++;
		idx = 0;
	}
}

/**
 * Mapa de "scancodes" a caracteres ASCII en un teclado QWERTY.
 */
/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
unsigned char kbdus[180] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   LEFT_SHIFT,        /* Left shift */ 
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
  'm', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */ // Chars when shift is being held
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'Q', 'W', 'E', 'R',   /* 19 */
  'T','Y','U','I','O','P', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
 'A','S','D','F','G','H','J','K','L', ';', /* 39 */
 '\'', '`',   LEFT_SHIFT,        /* Left shift */ 
 '\\', 'Z','X','C','V','B','N',
 'M', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};      

static const uint8_t KBD_PORT = 0x60;

/**
 * Handler para el teclado (IRQ1).
 *
 * Imprime la letra correspondiente por pantalla.
 */

static bool shift_pressed = false;

void keyboard() {
	uint8_t code;
	static char chars[81];
	static uint8_t idx = 0;

	asm volatile("inb %1,%0" : "=a"(code) : "n"(KBD_PORT));

     /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (code & 0x80) {
        // key released 
        code = code & 0x7F; // turn off bit 7
        if (kbdus[code] == LEFT_SHIFT) { shift_pressed = false; }
    } else {
        // key pressed
        if (idx == 80) {
            while (idx--)
            chars[idx] = ' ';
        }

        if (kbdus[code] == LEFT_SHIFT) { shift_pressed = true; return; }

        if (shift_pressed) {
            code += 90; // go to uppercase, there are 180 chars on kdbus, 90 without shift and 90 with shift held
        }

        chars[idx++] = kbdus[code];
        vga_write(chars, 19, 0x0A);
    }
}
