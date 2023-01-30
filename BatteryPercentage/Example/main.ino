#define voltagePin A0
#define peakVoltage 15

#include "../Headers/BPercentageIndicator.h";

ShahJugalR::BatteryUtilities::BPerc PercentageDevice(peakVoltage);

void setup(){
    Serial.begin(9600);
}

void loop(){
    Serial.println(PercentageDevice.QueryLevel(analogRead(voltagePin)));
}
