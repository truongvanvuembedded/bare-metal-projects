//==================================================================================================
//
//	File Name	:	led.c
//	CPU Type	:
//	IDE			:
//	Customer
//	Version		:	Ver.0.01
//	Coding		:	V.Vu
//	History		:	28/03/2026
//	Outline		:
//
//==================================================================================================
//==================================================================================================
//	#pragma section
//==================================================================================================

//==================================================================================================
//	Local Compile Option
//==================================================================================================

//==================================================================================================
//	Header File
//==================================================================================================
#include "button.h"
#include "Define.h"
//==================================================================================================
//	Local define
//==================================================================================================

//==================================================================================================
//	Local define I/O
//==================================================================================================

//==================================================================================================
//	Local Struct Template
//==================================================================================================

//==================================================================================================
//	Local RAM
//==================================================================================================

//==================================================================================================
//	Local ROM
//==================================================================================================

//==================================================================================================
//	Local Function Prototype
//==================================================================================================

//==================================================================================================
//	Source Code
//==================================================================================================
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	u1_Button_Init
//	Function	:	Initialize pst_Button object and assign platform-specific control functions
//
//	Argument	:	ST_BUTTON *pst_Button - pointer to pst_Button object
//					U1 u1_OnLevel        - hardware level indicating pst_Button press (e.g., active low/high)
//					pf_button_init pf_Init  - hardware initialization function
//					pu1_button_read pu1_Read - function to read pst_Button level/state
//					pf_button_callback pf_Callback - callback function for pst_Button events
//					pf_led_ctrl pf_Off   - function to turn LED off (active level depends on hardware)
//	Return		:	void
//	Created		:	28/03/2026
//	Changed		:	-
//	Remarks		:	User must provide correct control functions according to hardware design
//					(active-high or active-low). This abstraction ensures portability across platforms.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
U1 u1_Button_Init(ST_BUTTON* pst_Button, U1 u1_OnLevel, pf_button_init pf_Init, pu1_button_read pu1_Read, pf_button_callback pf_Callback)
{
    if (!pst_Button || !pf_Init || !pu1_Read || !pf_Callback)
    {
        return U1NG;  // Invalid parameters
    }

    pst_Button->u1_Enable = U1OFF;        // Start with button disabled
    pst_Button->u1_CounterEnable = U1ON;  // Counter disabled by default
    pst_Button->u1_Status = U1RELEASE;    // Initial state is released
    pst_Button->u1_OnLevel = u1_OnLevel;  // Set active level for button press
    pst_Button->u1_Double_F = U1OFF;      // Set active level for button press
    pst_Button->u4_Double_Cnt = U4MIN;    // Reset counter
    pst_Button->u4_Counter = U4MIN;       // Reset counter

    pst_Button->pf_Init = pf_Init;
    pst_Button->pu1_Read = pu1_Read;
    pst_Button->pf_Callback = pf_Callback;

    pf_Init();  // Call hardware initialization function

    return U1OK;  // Initialization successful
}  ////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	Button_Enable
//	Function	:	Enable a button for polling and event handling
//
//	Argument	:	ST_BUTTON *pst_Button - pointer to the button object
//	Return		:	void
//	Created		:	28/03/2026
//	Changed		:	-
//	Remarks		:	After enabling, the button can be polled using Button_Timer_Polling.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void Button_Enable(ST_BUTTON* pst_Button)
{
    if (pst_Button)
    {
        pst_Button->u1_Enable = U1ON;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	Button_Disable
//	Function	:	Disable a button to prevent polling and event handling
//
//	Argument	:	ST_BUTTON *pst_Button - pointer to the button object
//	Return		:	void
//	Created		:	28/03/2026
//	Changed		:	-
//	Remarks		:	After disabling, Button_Timer_Polling will ignore this button.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void Button_Disable(ST_BUTTON* pst_Button)
{
    if (pst_Button)
    {
        pst_Button->u1_Enable = U1OFF;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name		:	Button_Timer_Polling
//	Function	:	Poll the button state periodically and handle short/long press events
//
//	Argument	:	ST_BUTTON *pst_Button - pointer to the button object
//	Return		:	void
//	Created		:	28/03/2026
//	Changed		:	-
//	Remarks		:	This function should be called periodically (e.g., in a timer or main loop).
//					It updates the button counter, detects short, double and long presses,
//					and triggers the assigned callback function.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void Button_Timer_Polling(ST_BUTTON* pst_Button)
{
    U1 u1_Hw_Button_State;
    U1 u1_EventDetected = U1FALSE;

    /* Check if button is enabled */
    if (pst_Button->u1_Enable == U1OFF)
    {
        return;  // Button is disabled, do nothing
    }

    /* Read current hardware button state (pressed/released) */
    u1_Hw_Button_State = pst_Button->pu1_Read();

    /* ----- Button pressed ----- */
    if (u1_Hw_Button_State == pst_Button->u1_OnLevel)
    {
        if (pst_Button->u1_CounterEnable)
        {
            /* Increase press duration counter */
            pst_Button->u4_Counter += U1_CYCLE_TIME;
            /* ----- Double press detection ----- */
            if (pst_Button->u1_Double_F && pst_Button->u4_Double_Cnt)
            {
                pst_Button->u1_Status = U1_BUTTON_SW_STATE_DOUBLE_PRESS;  // Update status
                pst_Button->u1_CounterEnable = U1OFF;                     // Stop counting wait untill user release for detect next state
                pst_Button->u4_Double_Cnt = U4MIN;
                pst_Button->u1_Double_F = U1OFF;
                u1_EventDetected = U1TRUE;
            }
            /* ----- Long press detection ----- */
            else if (pst_Button->u4_Counter >= U4_BUTTON_LONG_PRESS_TIME && pst_Button->u1_Status != U1_BUTTON_SW_STATE_LONG_PRESS)
            {
                pst_Button->u1_Status = U1_BUTTON_SW_STATE_LONG_PRESS;  // Update status
                pst_Button->u1_CounterEnable = U1OFF;                   // Stop counting
                u1_EventDetected = U1TRUE;
            }
            /* ----- Short hold press detection ----- */
            else if (pst_Button->u4_Counter >= U4_BUTTON_SHORT_PRESS_MIN_TIME && pst_Button->u1_Status != U1_BUTTON_SW_STATE_SHORT_HOLD_PRESS)
            {
                /* Short press valid detected */
                pst_Button->u1_Status = U1_BUTTON_SW_STATE_SHORT_HOLD_PRESS;
                u1_EventDetected = U1TRUE;
            }
            /* Trigger callback if event occurs */
            if (u1_EventDetected)
            {
                pst_Button->u1_Enable = U1OFF;        // Temporarily disable for callback
                pst_Button->pf_Callback(pst_Button);  // Trigger callback
                pst_Button->u1_Enable = U1ON;         // Re-enable button
            }
        }
    }
    /* ----- Button released ----- */
    else
    {
        /* Detect valid short press release */
        if (pst_Button->u4_Counter < U4_BUTTON_SHORT_PRESS_MAX_TIME && pst_Button->u4_Counter >= U4_BUTTON_SHORT_PRESS_MIN_TIME &&
            pst_Button->u1_Status != U1_BUTTON_SW_STATE_SHORT_RELEASE_PRESS)
        {
            pst_Button->u1_Status = U1_BUTTON_SW_STATE_SHORT_RELEASE_PRESS;  // Update status to released
            pst_Button->u1_Enable = U1OFF;                                   // Temporarily disable for callback
            pst_Button->pf_Callback(pst_Button);                             // Trigger callback
            pst_Button->u1_Double_F = U1ON;                                  // Save double flag temporary
            pst_Button->u4_Double_Cnt = U4_BUTTON_DOUBLE_PRESS_TIMEOUT;      // Set timeout for detect double status
        }

        /* Reset button state and counter for next press */
        pst_Button->u4_Counter = U4MIN;  // Reset press duration
        pst_Button->u1_Status = U1_BUTTON_SW_STATE_RELEASE;
        pst_Button->u1_CounterEnable = U1ON;  // Re-enable counting
        pst_Button->u1_Enable = U1ON;         // Re-enable button
    }
    /* Double press timeout decrement */
    if (pst_Button->u4_Double_Cnt && --pst_Button->u4_Double_Cnt == 0)
        pst_Button->u1_Double_F = U1OFF;
}
