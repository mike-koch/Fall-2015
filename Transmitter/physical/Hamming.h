#ifndef TRANSMITTER_HAMMING_H
#define TRANSMITTER_HAMMING_H

// Takes any char with a value between '0'-'9' and returns the integer value
int get_int_value_of_char(char value);

/* original_message must be the message, already converted to a bit string of 1's and 0's.
 * length_of_message will be overwritten with the new char array's length.
 */
char* apply_hamming(unsigned char *original_message, unsigned int length_of_message, char *new_data);
#endif //TRANSMITTER_HAMMING_H
