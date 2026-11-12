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
#ifdef PBL_ROUND
  /* Native negative-x press animation retains the final pixel until release.
   * Match its integer quantization as well as its quadratic timing. */
  return (XOCE_UI_PRESS_DISTANCE * remaining * remaining + denominator - 1) / denominator;
#else
  return (XOCE_UI_PRESS_DISTANCE * remaining * remaining + denominator / 2) / denominator;
#endif
}

GPoint xoce_ui_action_anchor(GRect bar, unsigned button) {
  if (button > 2) button = 1;
#ifdef PBL_ROUND
  const int y=button==0?89:button==2?bar.size.h-89:bar.size.h/2;
  return GPoint(bar.origin.x+bar.size.w/2-1,bar.origin.y+y);
#else
  /* Native Emery action-bar slots: 54, 114, 174 on a 228px display. */
  const int step = bar.size.h > 108 ? (bar.size.h - 108) / 2 : bar.size.h / 3;
  return GPoint(bar.origin.x + bar.size.w / 2,
                bar.origin.y + bar.size.h / 2 + ((int)button - 1) * step);
#endif
}

GRect xoce_ui_circle_band(GRect surface,GRect box,int inset) {
  const int diameter=surface.size.w<surface.size.h?surface.size.w:surface.size.h;
  const int radius=diameter/2-inset;
  const int cx=surface.origin.x+surface.size.w/2,cy=surface.origin.y+surface.size.h/2;
  int dy=box.origin.y-cy,other=box.origin.y+box.size.h-cy;
  if(dy<0)dy=-dy;
  if(other<0)other=-other;
  if(other>dy)dy=other;
  if(radius<=0||dy>=radius)return GRect(cx,box.origin.y,0,box.size.h);
  /* Floor sqrt without libm; all rectangle corners stay inside the circle. */
  const int square=radius*radius-dy*dy;
  int lo=0,hi=radius;
  while(lo<hi){const int mid=(lo+hi+1)/2;if(mid*mid<=square)lo=mid;else hi=mid-1;}
  int left=box.origin.x>cx-lo?box.origin.x:cx-lo;
  int right=box.origin.x+box.size.w<cx+lo?box.origin.x+box.size.w:cx+lo;
  return GRect(left,box.origin.y,right>left?right-left:0,box.size.h);
}

GRect xoce_ui_safe_viewport(GRect bounds, bool round) {
  if (!round) return bounds;
  const int diameter = bounds.size.w < bounds.size.h ? bounds.size.w : bounds.size.h;
  /* floor(diameter / sqrt(2)); round down so the corners remain inside the
   * circle. Integer arithmetic keeps this usable in small drawing packages. */
  const int side = (diameter * 181 / 256) & ~1;
  return GRect(bounds.origin.x + (bounds.size.w - side) / 2,
               bounds.origin.y + (bounds.size.h - side) / 2, side, side);
}
