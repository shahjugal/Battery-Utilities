#define voltagePin A0
#define peakVoltage 15

#include "../Headers/BPercentageIndicator.h";

ShahJugalR::BatteryUtilities::BPerc PercentageDevice(peakVoltage);

void setup(){

}

void loop(){
    print(PercentageDevice.QueryLevel(analogRead(voltagePin)));
}