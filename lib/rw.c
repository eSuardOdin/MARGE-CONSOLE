
void store_32(int data, int addr)
{
    *(volatile unsigned char*)(addr)   = data & 0xFF;
    *(volatile unsigned char*)(addr+1) = (data & 0xFF00)        >> 8;
    *(volatile unsigned char*)(addr+2) = (data & 0xFF0000)      >> 16;
    *(volatile unsigned char*)(addr+3) = (data & 0xFF000000)    >> 24;
}

void store_16(int data, int addr)
{
    *(volatile unsigned char*)(addr)   = data & 0xFF;
    *(volatile unsigned char*)(addr+1) = (data & 0xFF00)        >> 8;
}

int load_32(int addr)
{
    return *(volatile unsigned char*)(addr)
        | *(volatile unsigned char*)(addr+1)
        | *(volatile unsigned char*)(addr+2)
        | *(volatile unsigned char*)(addr+3);
}

int load_16(int addr)
{
    return *(volatile unsigned char*)(addr)
        | *(volatile unsigned char*)(addr+1);
}


