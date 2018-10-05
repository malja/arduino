#include "Button.h"

Button::Button( const uint8_t pin, const uint16_t resolution ) {
    
    _pin = pin;
    _resolution = resolution;
    
    _counter = 0;
    _is_pushed = false;
    _last_time = 0;
    _on_pressed = nullptr;
    _on_released = nullptr;
    
    pinMode(pin, INPUT);
}

#ifdef __BUTTON_SHARED_DELTA_TIME__
void Button::update( uint32_t delta ) {
#else
void Button::update() {
#endif

    if ( (bool)digitalRead( _pin ) != _is_pushed ) {
        
        _is_pushed = !_is_pushed;
        
        if ( !_is_pushed && _on_released != nullptr ) {
            _on_released( this );
        }
        
        _counter = 0;
    }
    
    #ifdef __BUTTON_SHARED_DELTA_TIME__
        _last_time += delta;
        if ( _last_time >= _resolution ) {
    #else
        if ( millis() - _last_time >= _resolution ) {
    #endif

            if ( _is_pushed && _on_pressed != nullptr ) {
                
                #ifdef __BUTTON_SHARED_DELTA_TIME__
                    _last_time = 0;
                #else
                    _last_time = millis();
                #endif

                ++_counter;
                
                _on_pressed( this );
            }        
    }
    
    
   
        
    /*if ( (bool)digitalRead( _pin ) != _is_pushed ) {
        _is_pushed = !_is_pushed;
        
        if ( !_is_pushed && _on_released != nullptr) _on_released( this );
        
        // As we have to access this value in listener, we can reset it by now.
        _counter = 0;
        
    } else {
        ++_counter;
    }
        
    #ifdef __BUTTON_SHARED_DELTA_TIME__
        _last_time += delta;
        if ( _last_time >= _resolution ) {
    #else
        if ( millis() - _last_time >= _resolution ) {
    #endif

            if ( _is_pushed && _on_pressed != nullptr ) {
                
                #ifdef __BUTTON_SHARED_DELTA_TIME__
                    _last_time = 0;
                #else
                    _last_time = millis();
                #endif
                
                _on_pressed( this );
            }        
    }*/
}