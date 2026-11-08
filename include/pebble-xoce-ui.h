#pragma once

#include <pebble.h>
#include "metrics.h"
#include "native-type.h"

/* Measurement and drawing select the same unscaled public system font. */
GSize xoce_ui_native_measure(const char *text,unsigned size,XoceUiTypeRole role,
                            bool modern,int width);
void xoce_ui_native_draw(GContext *ctx,const char *text,unsigned size,XoceUiTypeRole role,
                         bool modern,GRect bounds,GColor color,GTextAlignment align,
                         GTextOverflowMode overflow);
typedef struct { GRect label,clock; int height; } XoceUiHeaderLayout;
XoceUiHeaderLayout xoce_ui_native_header_layout(const char *clock,unsigned size,bool modern,
                                               int width,int inset,int minimum_height);

typedef enum {
  XOCE_ICON_NONE, XOCE_ICON_EDIT, XOCE_ICON_PAUSE, XOCE_ICON_PLAY,
  XOCE_ICON_MORE, XOCE_ICON_PLUS, XOCE_ICON_MINUS, XOCE_ICON_NEXT,
  XOCE_ICON_CHECK, XOCE_ICON_SNOOZE, XOCE_ICON_UP, XOCE_ICON_DOWN,
  XOCE_ICON_TRASH, XOCE_ICON_PREVIOUS_TRACK, XOCE_ICON_NEXT_TRACK,
  XOCE_ICON_LAUNCH, XOCE_ICON_POWER, XOCE_ICON_TIMER, XOCE_ICON_MUSIC,
  XOCE_ICON_PHONE, XOCE_ICON_FIND, XOCE_ICON_NOTIFICATION,
  XOCE_ICON_WEATHER, XOCE_ICON_BATTERY, XOCE_ICON_LIGHT,
  XOCE_ICON_NAVIGATION, XOCE_ICON_CALENDAR, XOCE_ICON_WORKOUT,
  XOCE_ICON_GENERIC, XOCE_ICON_DIRECTION_STRAIGHT, XOCE_ICON_DIRECTION_LEFT,
  XOCE_ICON_DIRECTION_RIGHT, XOCE_ICON_DIRECTION_UTURN
} XoceUiIcon;

/* Hosts fit text and supply their loaded font and measured glyph ink. */
typedef void (*XoceUiTextDraw)(GContext *ctx, const char *text, int point,
                             GRect bounds, GColor color, GTextAlignment align,
                             void *context);

int xoce_ui_text_origin_y(GRect bounds, int ink_top, int ink_bottom);
GRect xoce_ui_fit_image(GRect box, int width, int height);
/* Positive displacement, toward the content; caller chooses its x sign. */
int xoce_ui_press_offset(uint32_t elapsed_ms);
GPoint xoce_ui_action_anchor(GRect bar, unsigned button);
void xoce_ui_draw_header(GContext *ctx, int content_width,
                         const char *title, const char *clock,
                         GColor ink, GColor divider,
                         XoceUiTextDraw draw_text, void *context);
void xoce_ui_draw_icon(GContext *ctx, XoceUiIcon icon, GPoint center,
                       int size, GColor ink);
/* Content-only entrypoint avoids linking action controls into watchfaces. */
void xoce_ui_draw_content_icon(GContext *ctx, XoceUiIcon icon, GPoint center,
                               int size, GColor ink);
void xoce_ui_draw_action_bar(GContext *ctx, GRect bar,
                             const XoceUiIcon icons[3],
                             const int8_t offsets[3], GColor background,
                             GColor ink);
