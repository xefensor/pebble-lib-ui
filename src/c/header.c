#include "pebble-xoce-ui.h"

void xoce_ui_draw_header(GContext *ctx, int width, const char *title,
                         const char *clock, GColor ink, GColor divider,
                         XoceUiTextDraw text, void *context) {
  if (!ctx || !text || width <= XOCE_UI_INSET * 2) return;
  const int clock_width = 64;
  const int clock_x = width - XOCE_UI_INSET - clock_width;
  text(ctx, title ? title : "", XOCE_UI_HEADER_POINT,
       GRect(XOCE_UI_INSET, 0, clock_x - XOCE_UI_INSET - 4, XOCE_UI_HEADER_HEIGHT),
       ink, GTextAlignmentLeft, context);
  text(ctx, clock ? clock : "", XOCE_UI_HEADER_POINT,
       GRect(clock_x, 0, clock_width, XOCE_UI_HEADER_HEIGHT), ink,
       GTextAlignmentRight, context);
  graphics_context_set_stroke_color(ctx, divider);
  graphics_context_set_stroke_width(ctx, 1);
  graphics_draw_line(ctx, GPoint(XOCE_UI_INSET, XOCE_UI_HEADER_HEIGHT - 1),
                     GPoint(width - XOCE_UI_INSET, XOCE_UI_HEADER_HEIGHT - 1));
}

void xoce_ui_draw_action_bar(GContext *ctx, GRect bar,
                             const XoceUiIcon icons[3],
                             const int8_t offsets[3], GColor background,
                             GColor ink) {
  if (!ctx || !icons) return;
  graphics_context_set_fill_color(ctx, background);
  graphics_fill_rect(ctx, bar, 0, GCornerNone);
  for (unsigned i = 0; i < 3; ++i) {
    GPoint center = xoce_ui_action_anchor(bar, i);
    if (offsets) center.x += offsets[i];
    xoce_ui_draw_icon(ctx, icons[i], center, XOCE_UI_RAIL_ICON_SIZE, ink);
  }
}
