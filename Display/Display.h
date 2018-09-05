#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#endif

// COMMANDS 
#define CMD_CLEAR_DISPLAY        0x01
#define CMD_RETURN_HOME          0x02
#define CMD_ENTRY_MODE_SET        0x04
#define CMD_DISPLAY_CONTROL      0x08
#define CMD_CURSOR_SHIFT         0x10
#define CMD_FUNCTION_SET         0x20
#define CMD_SET_CGRAM_ADDR        0x40
#define CMD_SET_DDRAM_ADDR        0x80

// flags for display entry mode
// ---------------------------------------------------------------------------
#define FLG_ENTRY_RIGHT          0x00
#define FLG_ENTRY_LEFT           0x02
#define FLG_ENTRY_SHIFT_INCREMENT 0x01
#define FLG_ENTRY_SHIFT_DECREMENT 0x00

// flags for display on/off and cursor control
// ---------------------------------------------------------------------------

#define FLG_DISPLAY_ON           0x04 //


#define FLAG_DISPLAY_OFF          0x00
#define FLAG_CURSOR_ON            0x02
#define FLAG_CURSOR_OFF           0x00
#define FLAG_BLINK_ON             0x01
#define FLAG_BLINK_OFF            0x00

// flags for display/cursor shift
// ---------------------------------------------------------------------------
#define FLAG_DISPLAY_MOVE         0x08
#define FLAG_CURSOR_MOVE          0x00
#define FLAG_MOVE_RIGHT           0x04
#define FLAG_MOVE_LEFT            0x00

struct DisplayConfiguration 
{
    uint8_t rs_pin;
    uint8_t rw_pin;
    uint8_t enable_pin;
    uint8_t data_pins[8];
    
    Display::mode_t mode;
    
    uint8_t colums;
    Display::line_t lines;
    Display::dotsize_t dot_size;

    Display::entry_t entry;
}

class Display {

    public:

        typedef enum {
            RIGHT_ENTRY = 0x00;
            LEFT_ENTRY = 0x02;
        } entry_t;

        typedef enum {
            4BIT_MODE = 0x00,
            8BIT_MODE = 0x10
        } mode_t;

        typedef enum {
            1LINE = 0x00,
            2LINE = 0x08
        } line_t;

        typedef enum {
            5x8_DOTS = 0x00,
            5x10_DOTS = 0x04
        } dotsize_t;

        Display( const DisplayConfiguration config );

        void enable();
        void disable();

        void clear();
        void home();

        // Cursor Functions
        void showCursor();
        void hideCursor();
        void setCursorPosition( uint8_t x, uint8_t y);
        void setCursorBlink();
        void setCursorNoBlink();

        // void scrollleft, scrollright, autoscroll, noautoscroll
        
        void setLeftToRight(); 
        void setRightToLeft();

        //void createCharacter( uint8_t, uint8_t[]);

    private:

        void init( Display::mode_t mode );
        void send( uint8_t data );

        DisplayConfiguration config;

        uint8_t display_control_flags;
        uint8_t entry_mode_flags;
};