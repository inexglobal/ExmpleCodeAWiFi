#include <Wire.h>                     // Include Wire.h for I2C functionality
void setup()
{
  Serial.begin(9600);                 // Start Serial 
  Serial.println("Sketch begin");     // Print begin message
  Wire.begin();   // Initialize I2C Bus
}

void loop(){
  
  unsigned int read_value = 0;        // Sets/reset the variable for reading DAC to 0
  unsigned int vref = 4095;           // Sets vref value for calculations (set to 2500 for internal reference)
  float voltage = 0;                  // Sets/reset the variable for voltage caltulations to 0
  
  for(int x = 0; x < 8; x++){                     // Loops from 0 to 7
    read_value = read(0x48,x,1);                 // Read value of ADC channel x (0 to 7) in Single-ended mode
    voltage = read_value * (vref / 4095.0);       // Calculate voltage output value according to the voltage reference and resolution
    Serial.print("Channel ");                 // Prints...
    Serial.print(x);                          // Prints channel number
    Serial.print(": ");                       // Prints...
    Serial.print(read_value);                 // Prints value read from ADC
    Serial.print(" (");                       // Prints...
    Serial.print(voltage,0);                  // Prints value of DAC output voltage on channel 0 to serial monitor
    Serial.println(" mV)");                   // Prints...
  }

  delay(1000);                           // Wait for 1 second
}
int read(unsigned char ads_address,unsigned char channel, int mode){
  // command address for the channels, allows 0-7 channel mapping in the correct order
  unsigned char channels[8] = {0x00, 0x40, 0x10, 0x50, 0x20, 0x60, 0x30, 0x70}; 
  unsigned char command = 0;    // Initialize command variable
  unsigned int reading = 0;     // Initialize reading variable 
  command = channels[channel];      // put required channel address in command variable 
  if (mode){
    command = command ^ 0x80;     // Enable Single-ended mode (toggle MSB, SD bit in datasheet)
  }
  Wire.beginTransmission(ads_address);  // Send a start or repeated start command with a slave address and the R/W bit set to '0' for writing.
  Wire.write(command);            // Then send a command byte for the register to be read.
  Wire.endTransmission();       // Send stop command 
  delay(10);
  Wire.requestFrom(ads_address, 2);   // Request 2 bytes from the ADC
  if(2 <= Wire.available())       // if two bytes were received
  {
    reading = Wire.read();        // receive high byte
    reading = reading << 8;       // shift high byte to be high 8 bits
    reading |= Wire.read();       // receive low byte into lower 8 bits
  }
  return reading;         // return the full 12 bits reading from the ADC channel
}
