#ifndef RW_H
#define RW_H

/**
 * @brief Stores a 32 bit word in MARGE memory
 * 
 * @param data 
 * @param addr 
 */
void store_32(int data, int addr);

/**
 * @brief Stores a 16 bit word in MARGE memory
 * 
 * @param data 
 * @param addr 
 */
void store_16(int data, int addr);

/**
 * @brief Load a 32 bit word from addr
 * 
 * @param addr 
 * @return int 
 */
int load_32(int addr);

/**
 * @brief Load a 16 bit word from addr
 * 
 * @param addr 
 * @return int 
 */
int load_16(int addr);



#endif