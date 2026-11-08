#pragma once

#include <stdbool.h>

typedef enum {
  XOCE_UI_TYPE_HEADER, XOCE_UI_TYPE_MENU_TITLE, XOCE_UI_TYPE_MENU_SUBTITLE,
  XOCE_UI_TYPE_FOOTER, XOCE_UI_TYPE_ROLE_COUNT
} XoceUiTypeRole;

/* Emery Small/Medium/Large correspond to SDK Medium/Large/ExtraLarge (1/2/3).
 * App choice 0 follows Pebble; 1/2/3 explicitly choose the same native profiles.
 * Hosts read preferences at launch and own persistence and capability checks. */
static inline unsigned xoce_ui_effective_text_size(unsigned choice,unsigned preferred) {
  if(choice>=1&&choice<=3)return choice-1;
  return preferred<=1?0:preferred>=3?2:1;
}

/* Public system font keys, copied from PebbleOS's semantic style table.
 * modern means firmware >= 4.44; the host supplies that capability decision.
 * Provenance and the two independent expected tables: docs/native-type.json. */
static inline const char *xoce_ui_native_font_key(unsigned size,XoceUiTypeRole role,bool modern) {
  static const char *const keys[3][XOCE_UI_TYPE_ROLE_COUNT]={
    {"RESOURCE_ID_GOTHIC_18_BOLD","RESOURCE_ID_GOTHIC_24_BOLD","RESOURCE_ID_GOTHIC_18","RESOURCE_ID_GOTHIC_18"},
    {"RESOURCE_ID_GOTHIC_24_BOLD","RESOURCE_ID_GOTHIC_24_BOLD","RESOURCE_ID_GOTHIC_24","RESOURCE_ID_GOTHIC_18"},
    {"RESOURCE_ID_GOTHIC_28_BOLD","RESOURCE_ID_GOTHIC_28","RESOURCE_ID_GOTHIC_24_BOLD","RESOURCE_ID_GOTHIC_24"}
  };
  if(size>2)size=2;
  if((unsigned)role>=XOCE_UI_TYPE_ROLE_COUNT)role=XOCE_UI_TYPE_HEADER;
  if(modern&&size==2) {
    if(role==XOCE_UI_TYPE_MENU_TITLE)return "RESOURCE_ID_GOTHIC_28_BOLD";
    if(role==XOCE_UI_TYPE_MENU_SUBTITLE)return "RESOURCE_ID_GOTHIC_28";
  }
  return keys[size][role];
}
