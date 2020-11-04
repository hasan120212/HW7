/**
 * Starter code for Project 2. Good luck!
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>


#define boarderx_min 0
#define boarderx_max 125
#define boardery_min 10
#define boardery_max 105

extern const Graphics_Image burgersBPP_UNCOMP;
extern const Graphics_Image menuBPP_UNCOMP;
extern const Graphics_Image cheese4BPP_UNCOMP;
extern const Graphics_Image burg4BPP_UNCOMP;
extern const Graphics_Image pickle8BPP_UNCOMP;
extern const Graphics_Image tomato8BPP_UNCOMP;
extern const Graphics_Image lettuce8BPP_UNCOMP;
// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    InitSystemTiming();
    /////////my code part////////////
    HAL hal=HAL_construct();
    Application app = App_Burge_construct(&hal);
    App_Burger_showTitleScreen(&hal.gfx);
    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        PollNonBlockingLED();

        /// my code////
        App_Burger_loop(&app, &hal);
        HAL_refresh(&hal);
    }
}

///////////main construction//////////////////////
Application App_Burge_construct(HAL * hal_p)
{
Application app;
    ///////initialization of FSM variables
    app.GameState = TITLE_SCREEN;
    app.timer = SWTimer_construct(TITLE_SCREEN_WAIT);
    SWTimer_start(&app.timer);

    return app;

}
/////////it should help to move to the next selection after 3 seconds
void App_Burger_handleTitleScreen(Application *app_p, HAL*hal_p)
{
    if(SWTimer_expired(&app_p->timer))
    {
     app_p->GameState = INSTRUCTION_SCREEN;
    // App_Burger_showGamemenuScreen(app_p, &hal_p->gfx);
    }
}

///// CALLBACK function for when the game is in the instruction screen state.

void App_guessMenu_handleInstructionScreen(Application * app_p, HAL* hal_p){


        app_p->GameState = MENU_SCREEN;

      // Increment the random number choice with a mod loopback to 0 when reaching
        // NUM_RANDOM_NUMBERS.
//        app_p->randomNumberChoice = (app_p->randomNumberChoice + 1) % NUM_RANDOM_NUMBERS;
        App_Burger_showGamemenuScreen(app_p, &hal_p->gfx);

}


///setting up the title screen//////////////////////////////////////////////////////////////
void App_Burger_showTitleScreen(GFX* gfx_p)
{
    GFX_clear(gfx_p);
    Graphics_drawImage(&gfx_p->context, &burgersBPP_UNCOMP, 0, 0);
    GFX_print(gfx_p, "ECE 2564 F20P2 ", 10, 0);
    GFX_print(gfx_p, "Hasan Nugmanov ", 11, 0);
    GFX_print(gfx_p, "Khan of the burger ", 12, 0);

}

////The main loop function of the application. Every cycle of this loop, we poll each of the FSMs one time.
void App_Burger_loop(Application * app_p, HAL * hal_p)
{
    switch(app_p->GameState)
    {
    case TITLE_SCREEN:
        App_Burger_handleTitleScreen(app_p,hal_p);
        break;
    case INSTRUCTION_SCREEN:
        App_guessMenu_handleInstructionScreen(app_p, hal_p);
        break;
    case MENU_SCREEN:
        App_Burger_handlMainMenuScreen(app_p, hal_p);
        break;
    case BURGER_TIME: // burger time choice
        app_helpermenu_return(app_p, hal_p);
                break;
    case HIGH_SCORE: // high score choice
        app_helpermenu_return(app_p, hal_p);
                break;
     case HOW_TO_COOK: // how to cook choice
         app_helpermenu_return(app_p, hal_p);
                break;
    case GAME_SCREEN:
        break;
    case RESULT_SCREEN:
        break;
    default:
        break;
    }

}

/////////show the Game Menu screen//////////////////////////////////////////////
void App_Burger_showGamemenuScreen(Application * app_p, GFX* gfx_p)
{
    GFX_clear(gfx_p);
    Graphics_drawImage(&gfx_p->context, &menuBPP_UNCOMP, 0, 0);
    GFX_print(gfx_p,"---- Main Menu ----",10,1);
    GFX_print(gfx_p,"Burger Time", 11, 5);
    GFX_print(gfx_p,"High Scores",12,5);
    GFX_print(gfx_p,"How to Cook",13,5);

    /// draw the cursor
    GFX_print(gfx_p,">", 11 + app_p->Moption, 3);
}

/*Select your menu options function by using switches
 * */
void App_Burger_handlMainMenuScreen(Application *app_p, HAL *hal_p)
{

    /////move the joystick up and down
    app_UpandDown_arrow(app_p, hal_p);
    ////////if BJ is pressed, move to the that screen

    if (Button_isTapped(&hal_p->boosterpackJS))
    {

        switch (app_p->Moption)
        {
        case Moption_1: // burger time choice
            app_p->Menu = true;
            GFX_clear(&hal_p->gfx);
            App_mainDrawBox_forBurger(app_p, hal_p, &hal_p->gfx);
            app_p->GameState = BURGER_TIME;
            break;
        case Moption_2: // high score choice
            GFX_clear(&hal_p->gfx);
            App_HighScore_screen(app_p, &hal_p->gfx);
            app_p->GameState = HIGH_SCORE;
            break;
        case Moption_3: // how to cook choice
            GFX_clear(&hal_p->gfx);
            App_Burger_HowToCook(hal_p, app_p, &hal_p->gfx);
            app_p->GameState = HOW_TO_COOK;
            break;
        default:
            break;
        }

    }

}

////////draw ">" where would updates the main game screen by redrawing only the position////
void App_Burger_updateMenuScreen(Application *app_p, GFX* gfx_p)
{
    ///clear the main menu from any previous game screen
    GFX_print(gfx_p,"-",10,3);
    GFX_print(gfx_p,"  ", 11, 3);
    GFX_print(gfx_p,"  ", 12, 3);
    GFX_print(gfx_p,"  ", 13, 3);

    // draw the cursor
    GFX_print(gfx_p,">", 11 + app_p->Moption, 3);
}

///how to play instruction function
void App_Burger_HowToCook(HAL * hal_p, Application *app_p, GFX* gfx_p)
{
    app_p->Menu = true;

      GFX_print(gfx_p,"How to Play", 0, 0);
      GFX_print(gfx_p,"-----------", 1, 0);
      GFX_print(gfx_p,"Move joystick towards ", 3, 0);
      GFX_print(gfx_p,"an ingredient to ", 4, 0);
      GFX_print(gfx_p,"highlight it.", 5, 0);

      GFX_print(gfx_p,"Press BoostS1 to add ", 7, 0);
      GFX_print(gfx_p,"one serving to dish.", 8, 0);

      GFX_print(gfx_p,"Press BoostS2 to", 9, 0);
      GFX_print(gfx_p,"reset dish.", 10, 0);

      GFX_print(gfx_p,"Press joystick in", 11, 0);
      GFX_print(gfx_p,"to serve dish", 12, 0);

      GFX_print(gfx_p,"Follow the recipe",14, 0);
      GFX_print(gfx_p,"ticket carefully!", 15, 0);


}
///// helper for return to the main menu function
void app_helpermenu_return(Application * app_p, HAL* hal){

    if((app_p->GameState == BURGER_TIME|| app_p->GameState == HIGH_SCORE || app_p->GameState == HOW_TO_COOK) && (Button_isTapped(&hal->boosterpackJS))){

        app_p->Menu = false;
        app_p->GameState = INSTRUCTION_SCREEN;
        app_p->Moption = Moption_1;


    }
}

/////////joystick up and down control for arrow
void app_UpandDown_arrow(Application * app_p, HAL * hal_p){

if(app_p->Menu == false){
    if( (Joystick_isTappedToDown(&hal_p->joystick)) && (app_p->GameState == MENU_SCREEN)){
        app_p->Moption = (_MenuOption)(((int) app_p->Moption+1) % NUM_TEST_OPTIONS);
        App_Burger_updateMenuScreen(app_p, &hal_p->gfx);


    }
    else if((Joystick_isTappedToUp(&hal_p->joystick)) && (app_p->GameState == MENU_SCREEN)){
        app_p->Moption = (_MenuOption)(((int) app_p->Moption-1) % NUM_TEST_OPTIONS);
        App_Burger_updateMenuScreen(app_p, &hal_p->gfx);


    }
}
}

///////////////HIgh Scores screen
void App_HighScore_screen(Application * app_p, GFX* gfx_p){

   app_p->Menu = true;

    GFX_print(gfx_p,"High Scores", 0, 0);
    GFX_print(gfx_p,"-----------", 1, 0);
    GFX_print(gfx_p,"$ 0", 2, 0);
    GFX_print(gfx_p,"$ 0", 3, 0);
    GFX_print(gfx_p,"$ 0", 4, 0);

}

//////////////////DRAW BURGER GAME/////////////////////////////////////////////////////////////////////////////////////////////// ////////////////

void App_mainDrawBox_forBurger(Application * app_p, HAL *context, GFX *gfx_p){

    App_DrawBox_forBurger(app_p, context, gfx_p);
    App_Cheese_ForBurger(app_p, context, gfx_p);
    App_Patty_ForBurger(app_p, context, gfx_p);
    App_Pickle_forBurger(app_p, context, gfx_p);
    App_Tomato_forBurger(app_p, context, gfx_p);
    App_Lettuce_forBurger(app_p, context, gfx_p);
}

////////////big box/////////////////////////////////////////
void App_DrawBox_forBurger(Application * app_p, HAL *context, GFX *gfx_p){

    GFX_print(gfx_p,"$ 0", 0, 0);

    //////draw boarders
        Graphics_Rectangle Rect = {boarderx_min, boardery_min, boarderx_max, boardery_max};
        Graphics_drawRectangle(&context ->gfx.context, &Rect);///drawing rectangle

        Graphics_drawLine( &gfx_p->context, 0, 55, boarderx_max, 55);
        Graphics_drawLine( &gfx_p->context, 0, 45, boarderx_max, 45);
        Graphics_drawLine( &gfx_p->context, 41, 10, 41,boardery_max);
        Graphics_drawLine( &gfx_p->context, 82, 10, 82,boardery_max);
        Graphics_drawLine( &gfx_p->context, 0, 90, boarderx_max, 90);

        Graphics_drawLine( &gfx_p->context, 12, 45, 12 , 55);// 1 column
        Graphics_drawLine( &gfx_p->context, 26, 45, 26 , 55);//
        Graphics_drawLine( &gfx_p->context, 52, 45, 52 , 55); // 2 column
        Graphics_drawLine( &gfx_p->context, 66, 45, 66 , 55);
        Graphics_drawLine( &gfx_p->context, 94, 45, 94 , 55); // 3 column
        Graphics_drawLine( &gfx_p->context, 108, 45, 108 , 55);

        Graphics_drawLine( &gfx_p->context, 12, 90, 12 ,boardery_max); // row 2 column 1
        Graphics_drawLine( &gfx_p->context, 26, 90, 26 ,boardery_max);
        Graphics_drawLine( &gfx_p->context, 52, 90, 52 ,boardery_max); // row 2 column 2
        Graphics_drawLine( &gfx_p->context, 66, 90, 66 ,boardery_max);
        Graphics_drawLine( &gfx_p->context, 94, 90, 94 ,boardery_max);  // row 2 column 3
        Graphics_drawLine( &gfx_p->context, 108, 90, 108 ,boardery_max);
}

///////////////////cheese////////////////////////
void App_Cheese_ForBurger(Application*app_p, HAL *context, GFX *gfx_p){

    GFX_print(gfx_p,"Cheese", 2, 0.9);
    Graphics_drawImage(&gfx_p->context, &cheese4BPP_UNCOMP, 8, 25);


}
///////////patty/////////////////////////////////////////
void App_Patty_ForBurger(Application*app_p, HAL *context, GFX *gfx_p){

    GFX_print(gfx_p,"Patty", 2, 8);
    Graphics_drawImage(&gfx_p->context, &burg4BPP_UNCOMP, 50, 25);

}
///////////pickles///////////////////////////////////
void App_Pickle_forBurger(Application*app_p, HAL *context, GFX *gfx_p){


    Graphics_drawImage(&gfx_p->context, &pickle8BPP_UNCOMP, 90, 25);
    GFX_print(gfx_p,"Pickle", 2, 14);
}
///////////Tomatoes////////////////////////
void App_Tomato_forBurger(Application*app_p, HAL *context, GFX *gfx_p){

    Graphics_drawImage(&gfx_p->context, &tomato8BPP_UNCOMP, 8, 70);
    GFX_print(gfx_p, "Tomato", 7, 0.9);

}

///////////Lettuce//////////////////////////
void App_Lettuce_forBurger(Application*app_p, HAL *context, GFX *gfx_p){


    Graphics_drawImage(&gfx_p->context, &lettuce8BPP_UNCOMP, 90, 70);
    GFX_print(gfx_p, "Lettuce", 7, 14);

}
