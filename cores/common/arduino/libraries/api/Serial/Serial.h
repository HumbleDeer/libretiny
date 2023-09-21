/* Copyright (c) Kuba Szczodrzyński 2022-06-23. */

#pragma once

#include <Arduino.h>
#include <api/HardwareSerial.h>
#include <api/RingBuffer.h>

typedef struct SerialData SerialData;

class SerialClass : public HardwareSerial {
  private:
	uint32_t port; //!< port number, family-specific
	pin_size_t rx; //!< RX pin number of this instance
	pin_size_t tx; //!< TX pin number of this instance

  private:
	SerialData *data{nullptr}; //!< family-specific, created in begin(), destroyed in end()
	RingBuffer *buf{nullptr};  //!< RX data buffer, assigned in begin(), removed in end()
	uint32_t baudrate{0};	   //!< currently set baudrate
	uint16_t config{0};		   //!< currently set configuration

  public:
	SerialClass(uint32_t port, pin_size_t rx = PIN_INVALID, pin_size_t tx = PIN_INVALID) : port(port), rx(rx), tx(tx) {}

	void begin(unsigned long baudrate, uint16_t config);
	void configure(unsigned long baudrate, uint16_t config);
	void end();
	void flush();
	size_t write(uint8_t c);

	int available();
	int peek();
	int read();

	inline void begin(unsigned long baudrate) {
		begin(baudrate, SERIAL_8N1);
	}

	inline void configure(unsigned long baudrate) {
		configure(baudrate, SERIAL_8N1);
	}

	operator bool() {
		return !!buf;
	}

  public:
#if LT_AUTO_DOWNLOAD_REBOOT
	static void adrParse(uint8_t c);
#endif

	using Print::write;
};

#if LT_HW_UART0
extern SerialClass Serial0;
#endif
#if LT_HW_UART1
extern SerialClass Serial1;
#endif
#if LT_HW_UART2
extern SerialClass Serial2;
#endif

#define SerialN(x) Serial##x
#define SerialM(x) SerialN(x)
#define Serial	   SerialM(LT_UART_DEFAULT_SERIAL)
