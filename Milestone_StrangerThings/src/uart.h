// uart.h

// configures UART for baud 9600
void setup_uart();

char* read_data_bytes();

void send_bytes(unsigned char* bytes, unsigned char length);
