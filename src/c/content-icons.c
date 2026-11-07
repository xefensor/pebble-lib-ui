#include "pebble-xoce-ui.h"

/* One 20-unit drawing grid, shared by action and content roles. No paths are
 * allocated. Optical adjustments live here rather than in every consumer. */
static int scaled(int value, int size) { return value * size / 20; }

void xoce_ui_draw_content_icon(GContext *ctx, XoceUiIcon icon, GPoint center,
                       int size, GColor ink) {
  if (!ctx || size <= 0 || icon == XOCE_ICON_NONE) return;
  graphics_context_set_stroke_color(ctx, ink);
  graphics_context_set_fill_color(ctx, ink);
  graphics_context_set_stroke_width(ctx, size >= 30 ? 3 : 2);
#define P(px,py) GPoint(center.x + scaled((px),size), center.y + scaled((py),size))
#define LINE(x,y,a,b) graphics_draw_line(ctx,P(x,y),P(a,b))
#define BOX(px,py,w,h) GRect(center.x+scaled((px),size),center.y+scaled((py),size),scaled((w),size),scaled((h),size))
#define FILL(x,y,w,h) graphics_fill_rect(ctx,BOX(x,y,w,h),0,GCornerNone)
  switch (icon) {
    case XOCE_ICON_TIMER:
      LINE(-6,-8,6,-8);LINE(-6,8,6,8);LINE(-6,-7,-6,-4);LINE(6,-7,6,-4);
      LINE(-6,-4,6,4);LINE(6,-4,-6,4);LINE(-6,4,-6,7);LINE(6,4,6,7);break;
    case XOCE_ICON_MUSIC:
      LINE(-2,5,-2,-6);LINE(-2,-6,7,-8);LINE(7,-8,7,4);
      graphics_fill_circle(ctx,P(-5,6),scaled(3,size));
      graphics_fill_circle(ctx,P(4,5),scaled(3,size));break;
    case XOCE_ICON_PHONE: case XOCE_ICON_FIND:
      graphics_draw_round_rect(ctx,BOX(-5,-8,10,16),1);LINE(-1,5,1,5);
      if(icon==XOCE_ICON_FIND){LINE(-9,-4,-9,4);LINE(9,-4,9,4);}break;
    case XOCE_ICON_NOTIFICATION:
      graphics_draw_rect(ctx,BOX(-8,-7,16,11));LINE(-6,4,-6,8);LINE(-6,8,0,4);LINE(-5,-3,5,-3);break;
    case XOCE_ICON_WEATHER:
      graphics_draw_circle(ctx,P(-3,-3),scaled(4,size));
      graphics_draw_round_rect(ctx,BOX(-5,0,14,6),scaled(3,size));break;
    case XOCE_ICON_BATTERY:
      graphics_draw_rect(ctx,BOX(-9,-4,16,8));FILL(7,-1,2,3);FILL(-6,-1,7,3);break;
    case XOCE_ICON_LIGHT:
      graphics_draw_circle(ctx,P(0,-3),scaled(6,size));LINE(-3,4,3,4);LINE(-3,7,3,7);break;
    case XOCE_ICON_NAVIGATION:
      LINE(-7,7,-7,0);LINE(-7,0,7,0);LINE(1,-6,7,0);LINE(1,6,7,0);break;
    case XOCE_ICON_CALENDAR:
      graphics_draw_rect(ctx,BOX(-8,-6,16,14));LINE(-8,-2,8,-2);LINE(-4,-8,-4,-5);LINE(4,-8,4,-5);break;
    case XOCE_ICON_WORKOUT:
      graphics_fill_circle(ctx,P(0,-7),scaled(2,size));LINE(0,-4,-3,2);
      LINE(-3,2,7,8);LINE(-3,2,-7,8);LINE(-7,0,7,0);break;
    case XOCE_ICON_DIRECTION_STRAIGHT:
      LINE(0,8,0,-8);LINE(0,-8,-7,-1);LINE(0,-8,7,-1);break;
    case XOCE_ICON_DIRECTION_LEFT: case XOCE_ICON_DIRECTION_RIGHT: {
      int d=icon==XOCE_ICON_DIRECTION_LEFT?-1:1;
      LINE(-7*d,8,-7*d,0);LINE(-7*d,0,7*d,0);LINE(7*d,0,0,-7);LINE(7*d,0,0,7);break;
    }
    case XOCE_ICON_DIRECTION_UTURN:
      graphics_draw_round_rect(ctx,BOX(-6,-7,12,14),scaled(4,size));
      LINE(-6,7,-10,1);LINE(-6,7,0,1);break;
    default:
      graphics_draw_circle(ctx,center,scaled(6,size));graphics_fill_circle(ctx,center,scaled(2,size));break;
  }
  graphics_context_set_stroke_width(ctx,1);
#undef P
#undef LINE
#undef BOX
#undef FILL
}
