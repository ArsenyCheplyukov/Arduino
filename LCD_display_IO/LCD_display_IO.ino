#include <I2CIO.h>

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
I2CIO myPort;

void setup()
{
  myPort.begin ( I2C_ADDR );
  
  myPort.portMode ( OUTPUT ); // Define all pins as output on the port
  myPort.write ( 0x0 );       // Set all pins of the board to low
}
