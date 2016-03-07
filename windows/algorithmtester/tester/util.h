#ifndef __UTIL_H__
#define __UTIL_H__

#define MAX(x,y) (((x) > (y))?(x):(y))
#define MIN(x,y) (((x) < (y))?(x):(y))

#define FASTFLOOR(x) (((x) >= 0) ? (int)(x) : (int)(x)-1)

static float Dot3(const char a[], float x, float y, float z);

#endif