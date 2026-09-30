#include <bus.h>

int print_debug_register(char data)
{
    static int char_offset = 0;
    static char debug_string[128] = {0};

    if (data == '\0') {
        // Print the debug string
        debug_string[char_offset] = '\0'; // Null-terminate the string
        printf("Debug Register: %s\n", debug_string);
        // Reset the debug string and offset
        char_offset = 0;
        for (int i = 0; i < 128; i++) {
            debug_string[i] = 0;
        }
    } else {
        // Store the character in the debug string
        if (char_offset < 127) { // Ensure we don't overflow the buffer
            debug_string[char_offset++] = data;
        }
    }

    return 0;
}