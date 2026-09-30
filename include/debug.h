#ifndef DEBUG_H
#define DEBUG_H

#include "bus.h"

/**
 * @brief Prints the value of the debug register in the bus.
 * 
 * @param bus Pointer to the bus structure containing the debug register.
 */
int print_debug_register(char data);

#endif