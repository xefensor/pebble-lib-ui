#include "pebble-xoce-ui.h"

GSize xoce_ui_native_measure(const char *text,unsigned size,XoceUiTypeRole role,bool modern,int width) {
  return graphics_text_layout_get_content_size(text?text:"",
      fonts_get_system_font(xoce_ui_native_font_key(size,role,modern)),
      GRect(0,0,width,2048),GTextOverflowModeWordWrap,GTextAlignmentLeft);
}
void xoce_ui_native_draw(GContext *ctx,const char *text,unsigned size,XoceUiTypeRole role,
                         bool modern,GRect bounds,GColor color,GTextAlignment align,
                         GTextOverflowMode overflow) {
  graphics_context_set_text_color(ctx,color);
  graphics_draw_text(ctx,text?text:"",fonts_get_system_font(xoce_ui_native_font_key(size,role,modern)),
      bounds,overflow,align,NULL);
}
XoceUiHeaderLayout xoce_ui_native_header_layout(const char *clock,unsigned size,bool modern,
                                               int width,int inset,int minimum_height) {
  const GSize measured=xoce_ui_native_measure(clock,size,XOCE_UI_TYPE_HEADER,modern,2048);
  const int height=measured.h+2>minimum_height?measured.h+2:minimum_height;
  const int clock_width=measured.w<width-2*inset?measured.w:width-2*inset;
  const int clock_x=width-inset-clock_width;
  const int label_width=clock_x-2*inset>0?clock_x-2*inset:0;
  return (XoceUiHeaderLayout){.label=GRect(inset,0,label_width,height),
      .clock=GRect(clock_x,0,clock_width,height),.height=height};
}

XoceUiHeaderLayout xoce_ui_round_header_layout(GRect surface,int content_right,
                                               unsigned size,bool modern) {
  const int line=xoce_ui_native_measure("22:58",size,XOCE_UI_TYPE_HEADER,modern,2048).h;
  const int top=8;
  XoceUiHeaderLayout out={.height=top+2*line+4};
  out.clock=xoce_ui_circle_band(surface,GRect(surface.origin.x,top,surface.size.w,line),5);
  out.label=xoce_ui_circle_band(surface,GRect(surface.origin.x,top+line,surface.size.w,line+2),5);
  if(out.label.origin.x+out.label.size.w>content_right-4)out.label.size.w=content_right-4-out.label.origin.x;
  /* Clock retains the physical center. Context centers in the usable chord. */
  return out;
}
