#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
typedef struct GContext GContext;
typedef struct { int16_t x, y; } GPoint;
typedef struct { int16_t w, h; } GSize;
typedef struct { GPoint origin; GSize size; } GRect;
typedef struct { uint8_t argb; } GColor;
typedef enum { GTextAlignmentLeft, GTextAlignmentCenter, GTextAlignmentRight } GTextAlignment;
typedef enum { GTextOverflowModeWordWrap, GTextOverflowModeTrailingEllipsis } GTextOverflowMode;
typedef const void *GFont;
GFont fonts_get_system_font(const char *key);
GSize graphics_text_layout_get_content_size(const char *text,GFont font,GRect bounds,
                                           GTextOverflowMode overflow,GTextAlignment alignment);
void graphics_context_set_text_color(GContext *context,GColor color);
void graphics_draw_text(GContext *context,const char *text,GFont font,GRect bounds,
                        GTextOverflowMode overflow,GTextAlignment alignment,void *attributes);
#define GPoint(px,py) ((GPoint){(px),(py)})
#define GRect(px,py,pw,ph) ((GRect){{(px),(py)},{(pw),(ph)}})
