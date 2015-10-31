#ifndef __WZTAB_GLOBAL_H__
#define __WZTAB_GLOBAL_H__

#define DEFAULT_ITEM_HEIGHT    24
#define DEFAULT_ROW_HEIGHT     24
#define BORDER_MARGIN          1
#define ICON_AREA_SIZE         18
#define ICON_SIZE              16
#define TEXT_AREA_PAD          8
#define SUB_TEXT_AREA_PAD      6
#define SUB_ICON_AREA_PAD      2

#define HIT_TEST_NONE             0
#define HIT_TEST_CLOSEBTN         1
#define HIT_TEST_TEXTAREA         2
#define HIT_TEST_SUBITEMSTART     3

#define STATE_MOUSE_L_DOWN        0x00000001
#define STATE_MOUSE_L_UP          0x00000002


typedef enum
{
    NORMAL_STATE,
    ACTIVE_STATE,
    HOVER_STATE,
    LAST_ACTIVE_STATE
}TAB_ITEM_STATE;

typedef enum
{
    SUB_NORMAL_STATE,
    SUB_ACTIVE_STATE,
    SUB_HOVER_STATE,
    SUB_LAST_ACTIVE_STATE
}TAB_SUB_ITEM_STATE;

#endif /*__WZTAB_GLOBAL_H__*/
