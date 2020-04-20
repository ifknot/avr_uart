/*
 * uart_device.cpp
 *
 * Created: 4/18/2020 10:32:00 PM
 *  Author: ifkno
 */ 

#include "uart_device.h"

namespace singleton {
	
	void uart_device::write(char c){
		UDR0 = c;
		loop_until_bit_is_set(UCSR0A, UDRE0);	// spin on data register until completed
	}

	char uart_device::read() {
		loop_until_bit_is_set(UCSR0A, RXC0);	// spin in read register until contains something
		return UDR0;
	}

	void uart_device::endl() {
		write('\r');
		write('\n');
	}

	void uart_device::write_nibble(char n) {
		n &= 0x0F;	// keep bottom 4 bits
		if (n > 9) {
			n += 7;	// convert to ascii 'A' when '0' added
		}
		n += '0';	// convert to decimal
		write(n);
	}

	void uart_device::write_hex(uint8_t n) {
		write_nibble(n >> 4);	// AVR is little-endian
		write_nibble(n);
	}
		
	char uart_device::read(const char* zstring) {
		write(zstring);
		return read();
	}

	void uart_device::write(const char* zstring) {
		uint16_t i{0};
		while(zstring[i]) {
			write(zstring[i++]);
		}
	}
		
	void uart_device::write_bool(bool b) {
		if(b) {
			write("true");
		}
		else {
			write("false");
		}
	}

		void uart_device::write_dec(uint32_t n) {
			uint8_t digit;
			bool lead_digit = false;
			for(uint8_t i{0}; i < 10; ++i) {
			digit = 0;
			if(n >= ranks[i]) {
				while(n >= ranks[i]) {
					n -= ranks[i];
					digit++;
					lead_digit = true;
				}
			}
			if(lead_digit) {
				write('0' + digit);
			}
		}
	}

	void uart_device::write_hex(uint16_t n) {
		write_hex(static_cast<uint8_t>(n >> 8));	// AVR is little-endian
		write_hex(static_cast<uint8_t>(n));
	}

	void uart_device::write_hex(uint32_t n) {
		write_hex(static_cast<uint16_t>(n >> 16));	// AVR is little-endian
		write_hex(static_cast<uint16_t>(n));
	}

	void uart_device::write_hex(uint64_t n) {
		write_hex(static_cast<uint32_t>(n >> 32));
		write_hex(static_cast<uint32_t>(n));
	}

	void uart_device::write_bin(uint8_t n) {
		uint8_t mask = 0x80;
		while(mask) {
			write(n & mask ? '1' : '0');
			mask >>= 1;
			if(mask == 0x08) {
				write(' ');
			}
		}
		write(' ');
	}

	void uart_device::write_bin(uint16_t n) {
		write_bin(static_cast<uint8_t>(n >> 8));	// AVR is little-endian
		write_bin(static_cast<uint8_t>(n));
	}

	void uart_device::write_bin(uint32_t n) {
		write_bin(static_cast<uint16_t>(n >> 16));	// AVR is little-endian
		write_bin(static_cast<uint16_t>(n));
	}

	uart_device::uart_device() {
		UBRR0H = UBRRH_VALUE;				// load baud rate
		UBRR0L = UBRRL_VALUE;				//		"
		// if the desired BAUD rate within the given tolerance could only be achieved by setting the U2X bit
		#if USE_2X							// use _BV convenience macro
		UCSR0A |= _BV(U2X0);				// set the U2X bit in the UART configuration
		#else
		UCSR0A &= ~(_BV(U2X0));				// clear the U2X bit in the UART configuration
		#endif
		UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8-bit data
		UCSR0B = _BV(RXEN0) | _BV(TXEN0);   // Enable RX and TX
	}

}