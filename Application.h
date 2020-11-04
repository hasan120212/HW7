/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  0  // 3 seconds

#define NUM_TEST_OPTIONS    3
#define NUM_RANDOM_NUMBERS  5

typedef enum {TITLE_SCREEN,INSTRUCTION_SCREEN, MENU_SCREEN,GAME_SCREEN,BURGER_TIME, HIGH_SCORE, HOW_TO_COOK, RESULT_SCREEN} _GameState;
typedef enum {Moption_1 =0 , Moption_2 = 1, Moption_3 = 2 } _MenuOption;

struct _Application
{
   _GameState GameState; //// Determine which screen been display
   SWTimer timer;    // General-purpose timer for when screens must disappear

   _MenuOption Moption; //// helps to choice which option in the menu
   //Selections
  bool Menu;

   // List of predetermined random numbers
   int randomNumbers[NUM_RANDOM_NUMBERS];
   int randomNumberChoice;

};
typedef struct _Application Application;

///main construct function for application///////////
Application App_Burge_construct(HAL * hal_p);
void App_Burger_loop(Application * app_p, HAL * hal_p);
void App_Burger_handleTitleScreen(Application *app_p, HAL*hal_p);
void App_guessMenu_handleInstructionScreen(Application * app_p, HAL* hal_p);


void App_Burger_showTitleScreen(GFX* gfx_p); ///////////set up the title screen for the burger
void App_Burger_showGamemenuScreen(Application * app_p, GFX* gfx_p);

void App_Burger_handlMainMenuScreen(Application * app_p, HAL * hal_p); //// selections
void App_Burger_updateMenuScreen(Application *app_p, GFX* gfx_p); ////updates arrow in menu screen

void App_Burger_HowToCook(HAL * hal_p, Application *app_p, GFX* gfx_p); //how to cook option screen
void App_Burger_erasemenu(Application *app_p, GFX* gfx_p); //erase the menu screen
void App_Burgermenu_initmenuVariables(Application * app_p, HAL * hal_p); /// helper to reset variables from main menu

void app_helpermenu_return(Application * app_p, HAL* hal); //HELPER TO RETURN MAIN MENU
void app_UpandDown_arrow(Application * app_p, HAL * hal_p); // joystick arrow controller from up and down
void App_HighScore_screen(Application * app_p, GFX* gfx_p); // high scores screen

///////////////////////draw the burger game///////////////////////////
void App_mainDrawBox_forBurger(Application * app_p, HAL *context, GFX *gfx_p);
void App_DrawBox_forBurger(Application * app_p, HAL *context, GFX *gfx_p);
void App_Cheese_ForBurger(Application*app_p, HAL *context, GFX *gfx_p);
void App_Patty_ForBurger(Application*app_p, HAL *context, GFX *gfx_p);
void App_Pickle_forBurger(Application*app_p, HAL *context, GFX *gfx_p);
void App_Tomato_forBurger(Application*app_p, HAL *context, GFX *gfx_p);
void App_Lettuce_forBurger(Application*app_p, HAL *context, GFX *gfx_p);
#endif /* APPLICATION_H_ */
