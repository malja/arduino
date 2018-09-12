/**
 * Class for reading data from PS2 Joystick.
*/
class PSJoystick {
  private:

    /// Position used for calibration of positin 0 on X-axis
    int pos_x_zero;
    /// Position used for calibration of position 0 on Y-axis
    int pos_y_zero;

    /// Latest value of X position
    int pos_x;
    /// Latest value of Y position
    int pos_y;
    /// Is button pressed?
    bool button;

    /// Store analog pin for reading X axis
    uint8_t pin_x;
    /// Store analog pin for reading Y axis
    uint8_t pin_y;
    /// Store digital pin for reading button press
    uint8_t pin_button;

    /// Interval in ms in which should be values updated
    uint16_t update_interval;
    /// Last time values were updated
    int last_update;

  public:
    /**
     * Call this method in `setup()` function to calibrate default
     * position of X and Y axis.
     * @param pinX Analog pin name for reading X axis
     * @param pinY Analog pin name for reading Y axis
     * @param pinButton Digital pin for reading button press
     * @param update_interval Interval in ms in which should be values updated
    */
    void setup(uint8_t pinX, uint8_t pinY, uint8_t pinButton, int update_interval = 250) {
        this->pin_x = pinX;
        this->pin_y = pinY;
        this->pin_button = pinButton;
        this->update_interval = update_interval;
        
        pinMode(this->pin_button, INPUT_PULLUP);
        
        // Read values for calibration
        this->pos_x_zero = analogRead(this->pin_x);
        this->pos_y_zero = analogRead(this->pin_y);

        // Set some value into last_update
        this->last_update = millis();
    }

    /**
     * Call this method in `loop()` function before using any other
     * methods. It reads data from joystick.
    */
    void update(unsigned long current_ms) {

      this->button = digitalRead(this->pin_button);
      
      // Update values for x and y axis.
      if (current_ms - this->last_update >= this->update_interval) {
        this->pos_x = analogRead(this->pin_x) - this->pos_x_zero;
        this->pos_y = analogRead(this->pin_y) - this->pos_y_zero;

        if (this->pos_x > 0) {
          this->pos_x = map(this->pos_x, 0, 1023-this->pos_x_zero, 0, 100);
        } else {
          this->pos_x = map(this->pos_x, 0, -this->pos_x_zero, 0, -100);
        }
      
        if (this->pos_y > 0) {
          this->pos_y = map(this->pos_y, 0, 1023-this->pos_y_zero, 0, 100);
        } else {
          this->pos_y = map(this->pos_y, 0, -this->pos_y_zero, 0, -100);
        }

        this->last_update = current_ms;
      }
    }

    /**
     * Get position on X axis. Value is in range from -100 to 100.
    */
    int getX() {
      return this->pos_x;
    }
    
    /**
     * Get position on Y axis. Value is in range from -100 to 100.
    */
    int getY() {
      return this->pos_y;
    }
    
    /**
     * Check if button is pressed. 
    */
    bool isPressed() {
      return this->button == LOW;
    }
};