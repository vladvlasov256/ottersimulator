//
//  global.h
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef global_h
#define global_h

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#define MENU_BUTTON_TITLE_FONT_SIZE 60

#else

#define MENU_BUTTON_TITLE_FONT_SIZE 60

#endif

#define MENU_TITLE_FONT_NAME "fonts/Marker Felt.ttf"
#define MENU_TITLE_FONT_SIZE (MENU_BUTTON_TITLE_FONT_SIZE * 1.5f)
#define MENU_TITLE_OUTLINE_SIZE 5

#define MENU_TITLE_POSITION Point(Director::getInstance()->getWinSize().width * .5f, Director::getInstance()->getWinSize().height * .75f)
#define MENU_BUTTONS_ORIGIN Point(Director::getInstance()->getWinSize().width * .5f, Director::getInstance()->getWinSize().height *.5f - MENU_BUTTON_TITLE_FONT_SIZE * .5f)
#define BACK_BUTTON_ORIGIN Point(20, Director::getInstance()->getWinSize().height - 20)
#define MENU_BUTTONS_GAP Point(0, MENU_BUTTON_TITLE_FONT_SIZE * 1.5f)

#define ITEM_SELECTOR_BUTTON_WIDTH (MENU_BUTTON_TITLE_FONT_SIZE * .5f)
#define ITEM_SELECTOR_BUTTON_GAP (MENU_BUTTON_TITLE_FONT_SIZE * .5f)

#define LEVEL_TITLE_WIDTH (Director::getInstance()->getWinSize().width * .25f)

#define USER_FOLDER_NAME "CoolOtter"
#define LEVEL_FOLDER_NAME "Maps"

#define UNITS_IN_SCREEN_HEIGHT 5.f

#endif /* global_h */
