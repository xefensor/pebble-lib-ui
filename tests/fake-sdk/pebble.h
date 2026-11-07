#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef struct GContext GContext;
typedef struct { int16_t x, y; } GPoint;
typedef struct { int16_t w, h; } GSize;
typedef struct { GPoint origin; GSize size; } GRect;
typedef struct { uint8_t argb; } GColor;
typedef enum { GTextAlignmentLeft, GTextAlignmentCenter, GTextAlignmentRight } GTextAlignment;
#define GPoint(px,py) ((GPoint){(px),(py)})
#define GRect(px,py,pw,ph) ((GRect){{(px),(py)},{(pw),(ph)}})
