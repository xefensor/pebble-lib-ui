#include "pebble-xoce-ui.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static const char *last_font;
static int measured_width;
static GSize measured;
GFont fonts_get_system_font(const char *key) {last_font=key;return key;}
GSize graphics_text_layout_get_content_size(const char *text,GFont font,GRect bounds,
    GTextOverflowMode overflow,GTextAlignment alignment) {
  assert(text&&font);assert(overflow==GTextOverflowModeWordWrap);
  assert(alignment==GTextAlignmentLeft);assert(bounds.size.h==2048);
  measured_width=bounds.size.w;return measured;
}
void graphics_context_set_text_color(GContext *ctx,GColor color) {(void)ctx;(void)color;}
void graphics_draw_text(GContext *ctx,const char *text,GFont font,GRect bounds,
    GTextOverflowMode overflow,GTextAlignment alignment,void *attributes) {
  (void)ctx;(void)bounds;(void)overflow;(void)alignment;
  assert(text&&font);assert(attributes==NULL);
}
int main(void) {
  /* The manual override selects exactly the matching OS size, independently
   * of the device's setting. Invalid/absent overrides retain Follow Pebble. */
  for(unsigned os=0;os<5;++os) {
    assert(xoce_ui_effective_text_size(0,os)==(os<=1?0:os>=3?2:1));
    assert(xoce_ui_effective_text_size(9,os)==xoce_ui_effective_text_size(0,os));
    for(unsigned choice=1;choice<=3;++choice)assert(xoce_ui_effective_text_size(choice,os)==choice-1);
  }
  /* Native headers protect the measured complete clock before allocating the
   * label. A taller native font grows the header rather than clipping it. */
  measured=(GSize){61,29};
  XoceUiHeaderLayout header=xoce_ui_native_header_layout("23:58",2,false,166,8,32);
  assert(header.clock.origin.x==97&&header.clock.size.w==61);
  assert(header.label.origin.x==8&&header.label.size.w==81&&header.height==32);
  assert(measured_width==2048);
  measured=(GSize){73,35};header=xoce_ui_native_header_layout("11:58",2,true,166,8,32);
  assert(header.height==37&&header.clock.origin.x==85&&header.label.size.w==69);
  for(unsigned modern=0;modern<2;++modern)for(unsigned size=0;size<3;++size)
    for(unsigned role=0;role<XOCE_UI_TYPE_ROLE_COUNT;++role) {
      xoce_ui_native_measure("gěÁ",size,(XoceUiTypeRole)role,modern,150);
      const char *font=last_font;assert(measured_width==150);
      xoce_ui_native_draw(NULL,"gěÁ",size,(XoceUiTypeRole)role,modern,GRect(0,0,150,50),
          (GColor){0},GTextAlignmentLeft,GTextOverflowModeWordWrap);
      assert(!strcmp(font,last_font));
    }
  assert(!strcmp(xoce_ui_native_font_key(2,XOCE_UI_TYPE_MENU_TITLE,false),"RESOURCE_ID_GOTHIC_28"));
  assert(!strcmp(xoce_ui_native_font_key(2,XOCE_UI_TYPE_MENU_TITLE,true),"RESOURCE_ID_GOTHIC_28_BOLD"));
  puts("Native type profiles, matching overrides and measured header: PASS");
}
