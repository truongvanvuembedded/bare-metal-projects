//==================================================================================================
//
// 	File Name		button.h
//	Customer
//	Coding			V.Vu
//	History			Ver.0.01	2026.03.28	V.Vu New
//	Outline			button header file
//
//==================================================================================================
//==================================================================================================
//	Local Compile Option
//==================================================================================================
#ifndef BUTTON_H
#    define BUTTON_H

#    include "Define.h"
//==================================================================================================
//	define
//==================================================================================================
/* Cycle time */
#    define U1_CYCLE_TIME ((U1)10) /* */

/* Button timing parameters */
#    define U4_BUTTON_SHORT_PRESS_MIN_TIME ((U4)(50 / U1_CYCLE_TIME))   /* 50ms */
#    define U4_BUTTON_SHORT_PRESS_MAX_TIME ((U4)(1000 / U1_CYCLE_TIME)) /* 1s */
#    define U4_BUTTON_LONG_PRESS_TIME      ((U4)(2000 / U1_CYCLE_TIME)) /* 2s */
#    define U4_BUTTON_DOUBLE_PRESS_TIMEOUT ((U4)(300 / U1_CYCLE_TIME))  /* 300ms */

/* Button software states */
#    define U1_BUTTON_SW_STATE_RELEASE             ((U1)(0x00)) /* Button is released, no press detected */
#    define U1_BUTTON_SW_STATE_SHORT_HOLD_PRESS    ((U1)(0x01)) /* Button is being held in a short press state */
#    define U1_BUTTON_SW_STATE_SHORT_RELEASE_PRESS ((U1)(0x02)) /* Button was short pressed and released */
#    define U1_BUTTON_SW_STATE_LONG_PRESS          ((U1)(0x03)) /* Button is held for long press duration */
#    define U1_BUTTON_SW_STATE_DOUBLE_PRESS        ((U1)(0x04)) /* Button is double pressed */

//==================================================================================================
//	structure
//==================================================================================================
/* Function pointer of button control */
typedef void (*pf_button_init)();
typedef void (*pf_button_callback)(void*);
typedef U1 (*pu1_button_read)();

/* Data structure representing a button object */
typedef struct
{
    U1 u1_Enable;        /* Enable/disable the button */
    U1 u1_CounterEnable; /* Enable/disable counting for long press */
    U1 u1_Status;        /* Current button state (U1PRESS/U1RELEASE) */
    U1 u1_OnLevel;       /* Hardware level indicating button press (e.g., active low/high) */
    U1 u1_Double_F;      /* Double flag */
    U4 u4_Double_Cnt;    /* Double timeout */
    U4 u4_Counter;       /* Counter to track button press duration */

    /* Function pointers */
    pf_button_init pf_Init;         /* Pointer to hardware initialization function */
    pf_button_callback pf_Callback; /* Pointer to callback when button is pressed */
    pu1_button_read pu1_Read;       /* Pointer to function to read button level/state */
} ST_BUTTON;
//==================================================================================================
//	Function Prototype
//==================================================================================================
U1 u1_Button_Init(ST_BUTTON* st_Button, U1 u1_OnLevel, pf_button_init pf_Init, pu1_button_read pu1_Read, pf_button_callback pf_Callback);
void Button_Enable(ST_BUTTON* st_Button);
void Button_Disable(ST_BUTTON* st_Button);
void Button_Timer_Polling(ST_BUTTON* st_Button);

#endif /* BUTTON_H */
       /* ************************************* End of File ******************************************** */