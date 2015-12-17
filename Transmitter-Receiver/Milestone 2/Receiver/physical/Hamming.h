#ifndef TRANSMITTER_HAMMING_H
#define TRANSMITTER_HAMMING_H

// Takes any char with a value between '0'-'9' and returns the integer value
int get_int_value_of_char(char value);

void reverse_hamming(char *message, int length, char *returned_data);

int check_hamming(char *message, int start_index);

char parse_byte(char *bit_string);
#endif //TRANSMITTER_HAMMING_H
