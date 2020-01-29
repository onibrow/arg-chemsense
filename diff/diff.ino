/* MCP 3424 version 1.2 example sketch OneShotConversion
  Written by B@tto
  Contact : batto@hotmail.fr

  In this example, one conversion per second is performed on channel 1 and 16 bits resolution.
  A new conversion has to be initiated by the user
*/


#include <Wire.h>
#include <MCP342x.h>

MCP342x MCP(0);

long Voltage[4];
char incomingByte = 0;
int num_samples = 1;

void setup() {
  MCP.begin(0);
  MCP.setConfiguration(CH1, RESOLUTION_16_BITS, CONTINUOUS_MODE, PGA_X1); // Channel 1, 16 bits resolution, one-shot mode, amplifier gain = 1
  Serial.begin(115200); // start serial for output
  Serial.print("MCP3424 Configuration Complete!\n");
}

void loop() {
  for (int i = 1; i <= 4; i++) {
    MCP.setConfiguration(i, RESOLUTION_16_BITS, CONTINUOUS_MODE, PGA_X1);
    long cumsum_voltage = 0;
    for (int j = 0; j < num_samples; j++) {
      cumsum_voltage += MCP.measure();
    }
    Voltage[i - 1] = -1 * cumsum_voltage / num_samples;
  }
  if (Serial.available()) {
    incomingByte = Serial.read();
    if (incomingByte == 0x64) {
      Serial.print(Voltage[0]); Serial.print(",");
      Serial.print(Voltage[1]); Serial.print(",");
      Serial.print(Voltage[2]); Serial.print(",");
      Serial.print(Voltage[3]); Serial.print(",\n");
    }
  }
}
