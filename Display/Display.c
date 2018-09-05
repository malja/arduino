#include "Display.h"

Display::Display( DisplayConfiguration config )
{
    this->config = config;
    this->init();
}

void Display::enable() 
{
    this->display_flags |= LCD_DISPLAY_ON;
    this->send( CMD_DISPLAY_CONTROL | this->display_flags );    
}

void Display::disable()
{
    this->display_flags &= ~LCD_DISPLAY_ON;
    this->send( CMD_DISPLAY_CONTROL | this->display_flags );
}

void Display::clear() 
{
    this->send( LCD_CLEAR_DISPLAY );
    delayMicroseconds( 2000 );
}

void Display::home()
{
    this->send( LCD_RETURN_HOME );
    delayMicroseconds( 2000 );
}

void Display::showCursor()
{
    this->display_flags |= FLAG_CURSOR_ON;
    this->send( CMD_DISPLAY_CONTROL | this->display_flags );
}

void Display::hideCursor()
{
    this->display_flags &= ~FLAG_CURSOR_ON;
    this->send( CMD_DISPLAY_CONTROL | this->display_flags );
}

void Display::setCursorPosition( uint8_t column, uint8_t row )
{
    
    const byte offset = { 0x00, 0x40 }
    
    if column > this->config.colums 
    {
        column = 0;
    }

    if row > this->config.lines
    {
        row = 0;
    }

    send( CMD_SET_DDRAM_ADDR | ( column + offset[row] ) );
}

void Display::setCursorBlink()
{
    this->display_flags |= FLAG_BLINK_ON;
    this->send( CMD_DISPLAY_CONTROL | this->display_flags );
}
        
void Display::setCursorNoBlink()
{
    this->display_flags &= ~FLAG_BLINK_ON;
    this->send( CMD_DISPLAY_CONTROL | this->display_flags );
}

void Display::setLeftToRight()
{
    this->entry_mode_flags |= Display::entry_t::LEFT_ENTRY;
    this->send( CMD_ENTRY_MODE_SET | this->entry_mode_flags );
}

void Display::setRightToLeft()
{
    this->entry_mode_flags &= ~Display::entry_t::LEFT_ENTRY;
    this->send( CMD_ENTRY_MODE_SET | this->entry_mode_flags );
}

void Display::init()
{

    // Nastaví datové piny na output   
    for( uint8_t i = 0; i < 4; i++ )
    {
        pinMode( this->config.data_pins[i], OUTPUT );
    }

    if ( this->config.mode == Display::mode_t::8BIT_MODE )
    {
        for( uint8_t i = 0; i < 4; i++ )
        {
            pinMode( this->config.data_pins[i], OUTPUT );
        }
    }

    // Nastaví ovládací piny na výstup
    pinMode( this->rs_pin, OUTPUT );
    pinMode( this->rw_pin, OUTPUT );
    pinMode( this->enable_pin, OUTPUT );

    // Všechny piny nastaví na LOW, tím se připravíme na zápis
    digitalWrite( this->rs_pin, LOW )
    digitalWrite( this->rw_pin, LOW )
    digitalWrite( this->enable_pin, LOW )

    // Nastaví 4-bitový mód
    if ( this->config.mode == Display::mode_t::4BIT_MODE )
    {
        send( 0x03 );
        delayMicroseconds( 4500 ); // čekání 4.1 ms

        send( 0x03 );
        delayMicroseconds( 150 ); // čekání 100 us

        send( 0x03 );
        delayMicroseconds( 150 )

        send( 0x02 );
        delayMicroseconds( 150 )
    } 
    else
    {
        send( 0x30 );
        delayMicroseconds( 4500 );

        send( 0x30 );
        delayMicroseconds( 150 );

        send( 0x30 );
        delayMicroseconds( 150 );
    }
    

        //                  DB7 DB6 DB5 DB4         DB3         DB2         DB1 DB0
        // Function set:    0   0   1   0           0           0           0   0
        // Display Mode:    0   0   0   1 = 8bit,   0           0           0   0
        //                              0 = 4bit
        // Lines:           0   0   0   0           1 = 2 lines 0           0   0
        //                                          0 = 1 line  0           0   0
        // Character size:  0   0   0   0           0           1 = 5x11    0   0
        //                                                      0 = 5x8     0   0
        // ======================================================================
        //                  0   0   1   ?           ?           ?           0   0
        
        // Nastaví typ připojení, počet řádků a velikost znaků
        send( 0x20 | this->config.mode | this->config.lines | this->config.dot_size )
        delayMicroseconds( 60 ) ;

        // Zapne display a smaže všechny znaky
        display();
        clear();

        //                  DB7 DB6 DB5 DB4 DB3 DB2 DB1         DB0
        // Entry Mode:      0   0   0   0   0   1   0           0
        // Move direction:  0   0   0   0   0   0   1 = left    0
        //                                          0 = right
        // Shift:           0   0   0   0   0   0   0           1 = increment
        //                                                      0 = decrement
        // ======================================================================
        // = 0x6            0   0   0   0   0   1   1           0
        
        // Nastaví psaní zleva doprava
        send( 0x4 | LCD_ENTRY_SHIFT_DECREMENT | LCD_ENTRY_LEFT )

}

void Display::send( uint8_t data ) 
{
    digitalWrite( this->rs_pin, LOW );
    digitalWrite( this->rw_pin, LOW );

    for ( uint8_t i = 0; i < 4 * this->mode; i++ )
    {
        digitalWrite( this->data_pins[i], (data >> i) & 0x01 )
    }

    digitalWrite( this->enable_pin, HIGH );
    delayMicroseconds( 1 );
    digitalWrite( this->enable_pin, LOW );

    delayMicroseconds( 150 );
}