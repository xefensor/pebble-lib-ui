#include "pebble-xoce-ui.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  /* Portrait/landscape inputs preserve shape and stay centered; tiny accepted
   * provider images never disappear through integer rounding. */
  for(int width=1;width<=40;width++)for(int height=1;height<=40;height++) {
    GRect box=GRect(7,11,24,28),result=xoce_ui_fit_image(box,width,height);
    assert(result.size.w>=1 && result.size.h>=1);
    assert(result.size.w<=box.size.w && result.size.h<=box.size.h);
    assert(result.origin.x>=box.origin.x && result.origin.y>=box.origin.y);
    assert(result.origin.x+result.size.w<=box.origin.x+box.size.w);
    assert(result.origin.y+result.size.h<=box.origin.y+box.size.h);
    assert(result.size.w==box.size.w || result.size.h==box.size.h);
    int error=result.size.w*height-result.size.h*width;
    if(error<0)error=-error;
    assert(error<=width+height);
  }
  assert(xoce_ui_fit_image(GRect(0,0,20,20),0,10).size.w==0);
  /* Release is bounded and monotonic, even after a delayed frame or wrap-sized
   * elapsed time. Hosts need no timer once it reaches zero. */
  int previous=XOCE_UI_PRESS_DISTANCE;
  for(unsigned elapsed=0;elapsed<=500;elapsed++) {
    int current=xoce_ui_press_offset(elapsed);
    assert(current>=0 && current<=previous);previous=current;
  }
  assert(xoce_ui_press_offset(0)==5);
  assert(xoce_ui_press_offset(144)==0);
  assert(xoce_ui_press_offset(UINT32_MAX)==0);
  /* Independent stock EaseOut sample points distinguish quadratic release
   * from the former faster cubic curve, including the SDK's30fps cadence. */
  assert(XOCE_UI_PRESS_FRAME_MS==33);
  assert(xoce_ui_press_offset(XOCE_UI_PRESS_FRAME_MS)==3);
  assert(xoce_ui_press_offset(36)==3);
  assert(xoce_ui_press_offset(2*XOCE_UI_PRESS_FRAME_MS)==1);
  assert(xoce_ui_press_offset(72)==1);
  assert(xoce_ui_press_offset(3*XOCE_UI_PRESS_FRAME_MS)==0);
  GRect bar=GRect(166,0,34,228);
  assert(xoce_ui_action_anchor(bar,0).y==54);
  assert(xoce_ui_action_anchor(bar,1).y==114);
  assert(xoce_ui_action_anchor(bar,2).y==174);
  assert(xoce_ui_action_anchor(bar,1).x==183);
  const GRect rectangle=GRect(0,0,144,168);
  const GRect same=xoce_ui_safe_viewport(rectangle,false);
  assert(same.size.w==144 && same.size.h==168);
  const GRect safe=xoce_ui_safe_viewport(GRect(0,0,260,260),true);
  assert(safe.origin.x==39 && safe.origin.y==39);
  assert(safe.size.w==182 && safe.size.h==182);
  /* Check the actual farthest corner against the physical circle, rather
   * than asserting only the implementation's chosen inset. */
  const int dx=130-safe.origin.x,dy=130-safe.origin.y;
  assert(dx*dx+dy*dy<=130*130);
  for(unsigned height=168;height<=260;height+=30) {
    GRect other=GRect(110,0,34,height);
    for(unsigned button=0;button<3;button++) {
      GPoint anchor=xoce_ui_action_anchor(other,button);
      assert(anchor.y>=18 && anchor.y<=(int)height-18);
    }
  }
  /* Accents/descenders with different font offsets share the visible center. */
  GRect row=GRect(0,32,166,36);
  int a=xoce_ui_text_origin_y(row,5,23),b=xoce_ui_text_origin_y(row,9,27);
  assert(a+5==b+9 && a+23==b+27);
  puts("Shared image fitting, input feedback and glyph alignment: PASS");
  return 0;
}
