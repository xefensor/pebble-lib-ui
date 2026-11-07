#include "pebble-xoce-ui.h"

/* One 20-unit drawing grid, shared by action and content roles. No paths are
 * allocated. Optical adjustments live here rather than in every consumer. */
static int scaled(int value, int size) { return value * size / 20; }

void xoce_ui_draw_icon(GContext *ctx, XoceUiIcon icon, GPoint center,
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
    case XOCE_ICON_EDIT: {
      GPoint points[] = {P(-7,8),P(-5,2),P(4,-7),P(8,-3),P(-2,6)};
      GPath path = {.num_points=5,.points=points};gpath_draw_filled(ctx,&path);break;
    }
    case XOCE_ICON_PAUSE: FILL(-7,-8,5,16);FILL(2,-8,5,16);break;
    case XOCE_ICON_PLAY: case XOCE_ICON_PREVIOUS_TRACK: case XOCE_ICON_NEXT_TRACK: {
      const int direction=icon==XOCE_ICON_PREVIOUS_TRACK?-1:1;
      GPoint points[]={P(-5*direction,-8),P(8*direction,0),P(-5*direction,8)};
      GPath path={.num_points=3,.points=points};gpath_draw_filled(ctx,&path);
      if(icon==XOCE_ICON_PREVIOUS_TRACK)FILL(-9,-8,2,16);
      if(icon==XOCE_ICON_NEXT_TRACK)FILL(8,-8,2,16);
      break;
    }
    case XOCE_ICON_MORE:
      for(int offset=-7;offset<=7;offset+=7)graphics_fill_circle(ctx,P(offset,0),size>=24?2:1);
      break;
    case XOCE_ICON_PLUS: LINE(0,-7,0,7); /* fall through */
    case XOCE_ICON_MINUS: LINE(-7,0,7,0);break;
    case XOCE_ICON_NEXT: LINE(-8,0,8,0);LINE(1,-7,8,0);LINE(1,7,8,0);break;
    case XOCE_ICON_CHECK: LINE(-8,0,-3,6);LINE(-3,6,8,-7);break;
    case XOCE_ICON_SNOOZE:
      graphics_draw_circle(ctx,center,scaled(8,size));LINE(-8,-8,-8,-2);LINE(-8,-2,-2,-2);
      LINE(0,-5,0,0);LINE(0,0,4,3);break;
    case XOCE_ICON_UP: LINE(-7,3,0,-4);LINE(0,-4,7,3);break;
    case XOCE_ICON_DOWN: LINE(-7,-3,0,4);LINE(0,4,7,-3);break;
    case XOCE_ICON_TRASH:
      LINE(-7,-5,7,-5);LINE(-5,-4,-5,7);LINE(-5,7,5,7);LINE(5,7,5,-4);
      LINE(-2,-8,2,-8);LINE(-2,-1,-2,4);LINE(2,-1,2,4);break;
    case XOCE_ICON_LAUNCH:
      LINE(-8,-8,-1,-8);LINE(-8,-8,-8,8);LINE(-8,8,-1,8);
      LINE(-2,0,8,0);LINE(3,-5,8,0);LINE(8,0,3,5);break;
    case XOCE_ICON_POWER:
      graphics_draw_circle(ctx,P(0,1),scaled(7,size));
      /* The stem remains distinguishable without painting another surface. */
      LINE(0,-9,0,0);break;
    default: xoce_ui_draw_content_icon(ctx,icon,center,size,ink);break;
  }
  graphics_context_set_stroke_width(ctx,1);
#undef P
#undef LINE
#undef BOX
#undef FILL
}
