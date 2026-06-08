#include "object.h"


e_object_resolution get_object_resolution(int flag, int* width, int* height)
{
    
    e_object_resolution res = (e_object_resolution) ((flag & 0x3C) >> 2);
    switch (res)
        {
            case R_8X8:
                *width = 1;
                *height = 1;
                break;
            case R_8X16:
                *width = 1;
                *height = 2;
                break;
            case R_8X32:
                *width = 1;
                *height = 3;
                break;
            case R_16X8:
                *width = 2;
                *height = 1;
                break;
            case R_16X16:
                *width = 2;
                *height = 2;
                break;
                case R_16X32:
                *width = 2;
                *height = 3;
                break;
                case R_32X8:
                *width = 3;
                *height = 1;
                break;
            case R_32X16:
                *width = 3;
                *height = 2;
                break;
            case R_32X32:
                *width = 3;
                *height = 3;
                break;
        }

    return res;
}




char get_object_flip(int flag, char is_x_axis)
{
    return is_x_axis ? (flag & 0x1) : (flag & 0x2);
}



char get_object_frame(int flag)
{
    return ((flag & 0x1C0) >> 6);
}