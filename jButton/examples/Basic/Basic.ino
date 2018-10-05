// You have to include Button.h to use this library.
#include "Button.h"

// Constant containing digital pin number which iButton library should listen to
const byte MYPIN = 9;

// This creates an instance of Button which listens on pin 9.
// Resolution is automatically set to 250ms. This means click
// could be registered every 250ms.
Button btn( MYPIN );

// Standard Arduino setup function
void setup() {

    pinMode( 13, OUTPUT );
    
    // Register a function which will be called after each press of the button
    btn.onPressed( [&](Button *which){
        
        digitalWrite( 13, HIGH );
        
    });
    
    btn.onReleased([&](Button *which){
        
        digitalWrite( 13, LOW );
        
    });
}

// Standard Arduino loop function
void loop() {
    
    // Listen for button updates
    btn.update();
    
}