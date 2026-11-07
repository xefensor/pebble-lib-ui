#include "pebble-xoce-ui.h"

int xoce_ui_text_origin_y(GRect bounds, int top, int bottom) {
  return bounds.origin.y + (bounds.size.h - (bottom - top + 1)) / 2 - top;
}

GRect xoce_ui_fit_image(GRect box, int width, int height) {
  if (width <= 0 || height <= 0 || box.size.w <= 0 || box.size.h <= 0)
    return GRect(box.origin.x, box.origin.y, 0, 0);
  int w, h;
  if ((int32_t)width * box.size.h >= (int32_t)height * box.size.w) {
    w = box.size.w;
    h = (int32_t)height * w / width;
    if (h < 1) h = 1;
  } else {
    h = box.size.h;
    w = (int32_t)width * h / height;
    if (w < 1) w = 1;
  }
  return GRect(box.origin.x + (box.size.w - w) / 2,
               box.origin.y + (box.size.h - h) / 2, w, h);
}

int xoce_ui_press_offset(uint32_t elapsed) {
  if (elapsed >= XOCE_UI_PRESS_RELEASE_MS) return 0;
  const uint32_t remaining = XOCE_UI_PRESS_RELEASE_MS - elapsed;
  /* Stock ActionBar uses AnimationCurveEaseOut: remaining distance is t².
   * Round only the final pixel offset, without floating-point frame work. */
  const uint32_t denominator = XOCE_UI_PRESS_RELEASE_MS * XOCE_UI_PRESS_RELEASE_MS;
  return (XOCE_UI_PRESS_DISTANCE * remaining * remaining + denominator / 2) / denominator;
}

GPoint xoce_ui_action_anchor(GRect bar, unsigned button) {
  if (button > 2) button = 1;
  /* Native Emery action-bar slots: 54, 114, 174 on a 228px display. */
  const int step = bar.size.h > 108 ? (bar.size.h - 108) / 2 : bar.size.h / 3;
  return GPoint(bar.origin.x + bar.size.w / 2,
                bar.origin.y + bar.size.h / 2 + ((int)button - 1) * step);
}
