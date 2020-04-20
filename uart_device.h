/*
 * uart_device.h
 *
 * Created: 4/18/2020 1:37:34 PM
 *  Author: ifkno
 */ 

#ifndef UART_DEVICE_H_
#define UART_DEVICE_H_

#ifndef	F_CPU// appease grumpy compilers
#define F_CPU 16000000UL // 16 MHz
#endif

#ifndef BAUD// appease grumpy compilers
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/setbaud.h>

//TODO: signed values

namespace singleton {
	
	/**
	 * More comprehensive, but larger, uart byte io (Meyer's canonical singleton implementation)
	 * @note Use -fno-threadsafe-statics 
	 * This disables the mutex around initialization of local static variables which is useless
	 * in single threaded and hence `__cxa_guard_acquire'	is undefined and will raise compile time error
	 */
	struct uart_device {	// Truth is, singletons and inheritance do not play well together.

		inline static uart_device& instance() {
			static uart_device u;
			return u;
		}
	
		/**
		 * wait for a character write to serial
		 */
		static void write(char c);
	
		/**
		 * wait for a character read from serial
		 */
		static char read();
		
		/**
		 * write a carriage return and newline
		 */
		static void endl();
	
		/**
		 * write the lower 4 bits as upper case hexadecimal number
		 */
		static void write_nibble(char n);
	
		/**
		 * byte overloaded write hexadecimal
		 */
		static void write_hex(uint8_t n);
	
		/**
		 * display message and wait for a character read from serial
		 */
		char read(const char* zstring);
	
		/**
		 * write a zero terminated char array to serial
		 * maximum size 65535 characters
		 */
		void write(const char* zstring);
	
		/**
		 * write true or false
		 */
		void write_bool(bool b);
	
		/**
		 * write unsigned decimal
		 */
		void write_dec(uint32_t n);
	
		/**
		 * unsigned word overloaded write hexadecimal
		 */
		void write_hex(uint16_t n); // u
	
		/**
		 * unsigned long overloaded write hexadecimal
		 */
		void write_hex(uint32_t n); // UL
	
		/**
		 * unsigned long long overloaded write hexadecimal
		 */
		void write_hex(uint64_t n); // ULL
	
		/**
		 * over loaded write binary
		 */
		void write_bin(uint8_t n);

		void write_bin(uint16_t n); // u

		void write_bin(uint32_t n); // UL

		uart_device(const uart_device &) = delete;
		
		uart_device & operator = (const uart_device &) = delete;
		
		~uart_device() = default;

	private:

		/**
		* default to the most common configuration: 9600, no parity, 8 data bits, 1 stop bit.
		* UCSZ20 UCSZ01 and UCSZ00 control the data size. 
		* Possible sizes are 5-bit (000), 6-bit (001), 7-bit (010), 8-bit (011) and 9-bit (111). 
		* Most common used data size is 8-bit.
		*/
		uart_device();
		
		const uint32_t ranks[10] = { 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };
		
	};
		
}

#endif /* UART_DEVICE_H_ */

/* examples:
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

*/