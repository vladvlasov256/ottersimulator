//
//  strings.h
//  CoolOtter
//
//  Created by Vladimir Vlasov on 27.03.16.
//
//

#ifndef strings_h
#define strings_h

#ifdef COOL_OTTER_GAME

#define MAIN_MENU_TITLE "Cool Otter Simulator"
#define MAIN_MENU_GAME "Game"
#define MAIN_MENU_MULTIPLAYER "Mulitplayer"

#define LEVEL_SELECTION_PLAY "Play"

#else //COOL_OTTER_EDITOR

#define EDITOR_START_SCREEN_TITLE "Editor"
#define EDITOR_START_SCREEN_NEW_LEVEL "New Level"
#define EDITOR_START_SCREEN_EDIT_LEVEL "Edit Existing"

#define LEVEL_SELECTION_TITLE "Select level"
#define LEVEL_SELECTION_EDIT "Edit"

#define LEVEL_EDITOR_MENU_TITLE "Editor Menu"
#define LEVEL_EDITOR_MENU_SAVE_AND_CONTINUE_EDITING "Save & Continue"
#define LEVEL_EDITOR_MENU_SAVE_AND_PLAY "Save & Play"
#define LEVEL_EDITOR_MENU_SAVE_AND_EXIT "Save & Exit"
#define LEVEL_EDITOR_MENU_EXIT_WITHOUT_SAVING "Exit without saving"

#endif //COOL_OTTER_EDITOR



#define PREVIOUS_BUTTON_TITLE "<"
#define NEXT_BUTTON_TITLE ">"
#define BACK_BUTTON "Back"
//#define BACK_BUTTON "←"


#endif /* strings_h */
