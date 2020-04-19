# avr_uart
C++ singleton UART comprehensive read and write char decimal hexadecimal binary etc.

Combines Meyer's canonical singleton with, and expands upon, Mika Tuupola's Sat, Nov 19, 2011 blog post Simple Serial Communications With AVR libc.

## exmaples

```cpp

auto& uart = singleton::uart_device::instance();

uart.write("hello world");

for(char i{0}; i < 255; ++i) {
	uart.write_hex((uint8_t)i);
	uart.write(' ');
}

uint16_t item = uart.read("enter a number: ");
uart.write_hex((uint16_t)item); // to correctly choose overloaded function for leading zeros

uart.write("\n\r 16 bit hex ");
uart.write_hex(0xD00Du);
uart.write("\n\r 32 bit hex ");
uart.write_hex(0x1234D00DUL);
uart.write("\n\r 64 bit hex ");
uart.write_hex(0xDEADBEEF1234D00DULL);
uart.write("\n\r 16 bit hex ");
uart.write_hex(0xD00DUL);
uart.write("\n\r 32 bit hex ");
uart.write_hex(0xD00DULL);
uart.write("\n\r decimal ");

for(char i{0}; i < 255; ++i) {
	uart.write_dec(i);
	uart.write(' ');
}

for(uint32_t i{999}; i < 1000000; i += 1000) {
	uart.write_dec(i);
	uart.write(' ');
}

uart.write("\n\r 32 bit decimal ");
uart.write_dec(32768);
uart.write("\n\r 16 bit hex ");
uart.write_hex(32768UL);

uart.write("\n\r  8 bit binary ");
uart.write_bin((uint8_t)0b01010101);
uart.write("\n\r 16 bit binary ");
uart.write_bin(0b0101010111001100u);
uart.write("\n\r 32 bit binary ");
uart.write_bin((uint32_t)0xDEADBEEF);

```

