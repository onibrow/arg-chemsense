/*
   1999250000 = 2
*/

#include <Wire.h>
#include <MCP342x.h>

MCP342x MCPA(0);

float Voltage[4];
String buff = String("deadbuffdead");
int num_samples = 1;

void setup() {
  MCPA.begin(0);
  MCPA.setConfiguration(CH1, RESOLUTION_12_BITS, ONE_SHOT_MODE, PGA_X1);
  Serial.begin(115200); // start serial for output
//  Serial.print("MCP3424 Configuration Complete!\n");
}

void loop() {
  if (Serial.available()) {
    buff = Serial.readStringUntil('\n');
    for (int i = 1; i <= 4; i++) {
      MCPA.setConfiguration((CHANNELS) i, RESOLUTION_12_BITS, ONE_SHOT_MODE, PGA_X1);
      long cumsum_voltage_A = 0;
      for (int j = 0; j < num_samples; j++) {
        MCPA.newConversion();
        cumsum_voltage_A += MCPA.measure();
      }
      Voltage[i - 1] = cumsum_voltage_A / num_samples / 1000000000.0;
    }
    if (buff.equals("req")) {
      Serial.print(Voltage[2],4); Serial.print(",");
      Serial.print(Voltage[1],4); Serial.print(",");
      Serial.print(Voltage[0],4); Serial.print(",");
      Serial.print(Voltage[3],4); Serial.print("\n");
    }
  }
}
