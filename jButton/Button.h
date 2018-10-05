#include "Arduino.h"

/******************************************************************************\
| iButton Library for Arduino IDE
| ===============================
| Author:       malja (looorin at gmail dot com)
| Version:      0.1
| Date:         27. 05. 2016
| 
| Description:
| ------------
| iButton is a small library created to simplyfy using buttons with Arduino.
| Everything you have to do is to define a new Button instance for each
| button you are using. Every time button is pushed, listener (custom
| funcion of yours) is called to produce an action. 
|
| Example:
| ---------

    // You have to include Button.h to use this library.
    #include "Button.h"

    // Constant containing digital pin number which iButton library should listen to
    const byte MYPIN = 9;

    // This creates an instance of Button which listens on pin 9
    Button btn( MYPIN );

    // Standard Arduino setup function
    void setup() {
 
        // Sets time in ms (milliseconds) which defines the smallest time diversion
        // between two clicks. Even with continuous press, next click will be
        // registered after defined time.
        // Click -- 250ms delay -- Yet another click -- 250ms delay -- Third click!
        btn.setResolution( 250 );
        
        // Register a function which will be called after each press of the button
        btn.onPressed( [&](Button *which){
            // do something really cool here ;)
        });
    }
    
    // Standard Arduino loop function
    void loop() {
        
        // Listen for button updates
        btn.update();
        
    }

| Licence:
| --------
|
| Use this as you want. :)
\******************************************************************************/

/******************************************************************************\
| Configuration
| =============
|
| In this section, you can adjust some setings of this library.
|
| __BUTTON_INFO_MESSAGE_PRINTED__
|---------------------------------
|
| Definition which changes function Button::update() to Button::update( delta ).
| This could be useful when using more than one Button or if you use some sort
| of timing in other parts of your sketch.
| Parameter for update method should contain the time in ms passed from the last 
| loop.
|
\******************************************************************************/
//#define __BUTTON_SHARED_DELTA_TIME__

class Button {
    
    public:
    
        // Definition of a callback function used in on* methods
        //
        // example:
        // void OnMyButtonPressed( Button *which ) { /* do something */ }
        typedef void (*btn_callback_t)(Button*);
        
        // Constructor.
        //
        // pin - Digital pin number on which instance listens  
        // resolution - Minimal time in ms which separates two clicks 
        //              from each other. See example setResolution.
        Button( const uint8_t pin, const uint16_t resolution = 250 );
        
        // Returns number of ms button stayed in pressed state
        inline uint32_t pressedForMs() const {
            return _counter * _resolution;    
        }
        
        // Sets minimal time between two clicks. Even continuous press
        // will be divided into separate clicks which will be delayed
        // exactly ’resolution’ ms from each other.
        //
        // resolution - Time in ms.
        // 
        // example:
        // click registered -- ’resolution’ ms delay -- click2 registered
        //
        inline void setResolution( const uint16_t resolution = 250 ) {
            _resolution = resolution;  
        }
        
        // Sets listener function which will be called each time button
        // is pressed. See btn_callback_t.
        //
        // pressed - Function pointer. 
        void onPressed( btn_callback_t pressed ) {
            _on_pressed = pressed;
        }      
        
        // Sets listener function which will be called each time button
        // is released. See btn_callback_t.
        //
        // released - Function pointer. 
        void onReleased( btn_callback_t released) {
            _on_released = released;   
        }
        
        #ifdef __BUTTON_SHARED_DELTA_TIME__
            
            // Update function with external timing. To use this, visit 
            // the Configuration section and define __BUTTON_SHARED_DELTA_TIME__.
            // This may be useful if you use more than one button.
            // See update, __BUTTON_INFO_MESSAGE_PRINTED__
            void update( uint32_t delta );
            
        #else
                        
            // Update function with internal timing.
            // See update(delta)
            void update();
            
        #endif
                 
    private:
    
        // .. which pin do we listen to?
        uint8_t _pin;
        
        // .. number of ’resolution’ ms during which button is in
        // the same state 
        uint8_t _counter;
        
        // .. number of ms between clicks
        uint16_t _resolution;
        
        // .. is button pushed
        bool _is_pushed;
        
        // .. content depends on __BUTTON_SHARED_DELTA_TIME__
        // defined -> adds delta time to its content
        // undefined -> last time button was pressed (millis)
        uint32_t _last_time;
                
        // .. which function should be called when button pressed/released
        btn_callback_t _on_pressed;
        btn_callback_t _on_released;
        
};