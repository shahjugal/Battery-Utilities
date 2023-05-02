#include <LiquidCrystal.h>

// ------ Include Statements End here--------//

class Relay {
  private:
    const int pin;
    
  public:
    Relay(int pin) :
      pin(pin) {}

    void setup() {
      pinMode(pin, OUTPUT);
    }

    void turnOn() {
      digitalWrite(pin, HIGH);
    }

    void turnOff() {
      digitalWrite(pin, LOW);
    }
};

class BatteryMonitor {
  private:
    const int voltagePin;
    const float peakVoltage;
    float voltage;
    unsigned long startTime;
    unsigned long elapsedTime;
  	double health = 100;
  	unsigned long startPercentage;
    double perD;
  double inHours;
    
  public:
    BatteryMonitor(int voltagePin, float peakVoltage, double perD = 2, double inHours = 0.5) :
      voltagePin(voltagePin),
      peakVoltage(peakVoltage), perD(perD), inHours(inHours) {}

    void setup() {
      pinMode(voltagePin, INPUT);
      startTime = millis();
      startPercentage = getBatteryPercentage();
    }
  
  void reset(){
    startTime = millis();
      startPercentage = getBatteryPercentage();
  }
		
  void updateData(){
    getVoltage();
    Serial.println(millis()-startTime);
    	
    if(getElapsedTimeHours > inHours){
    	double perDrop = startPercentage - getBatteryPercentage();
    if(perD < perDrop){
      	double refRate = perD/36000;
    	double ourRate = perDrop/36000;
      	double percMore = ourRate/refRate;
      	health = 100/percMore;
    }
      
    	// Reset for next Cycle
    reset();
	}
  }
  	
    float getVoltage() {
      double analogValue = analogRead(voltagePin);
      voltage = (analogValue * peakVoltage/1023.0);
      return voltage;
    }

    double getElapsedTimeHours() {
      unsigned long currentTime = millis();
      elapsedTime = currentTime - startTime;
      unsigned long hours = elapsedTime / 3600000;
      return hours;
    }

    int getBatteryHealth() {
      return health;
    }

    double getBatteryPercentage() {
      double voltage = getVoltage();
      double batteryPercentage = (voltage * 100 / peakVoltage);
      return batteryPercentage;
    }
};


class OutputDevice {
  public:
    virtual void write(const char* text) = 0;
};

class LCD : public OutputDevice {
  public:
    LCD(int rs, int enable, int d4, int d5, int d6, int d7) {
      lcd_ = new LiquidCrystal(rs, enable, d4, d5, d6, d7);
    }
  ~LCD(){
  delete lcd_;
  }

    void init() {
      lcd_->begin(16, 2);
      lcd_->clear();
    }

    void setCursor(int column, int row) {
      lcd_->setCursor(column, row);
    }

    void write(const char* text) override {
      lcd_->print(text);
    }

  private:
    LiquidCrystal* lcd_;
};


class SerialMonitor : public OutputDevice {
  public:
    void write(const char* text) override {
      Serial.println(text);
    }
};

class TextWriter {
  public:
    TextWriter(OutputDevice* output_device) : output_device_(output_device) {}

    void write(const char* text) {
      output_device_->write(text);
    }
  
  
    void write(auto value) {
      output_device_->write(String(value).c_str());
    }

  private:
    OutputDevice* output_device_;
};


//-----Classes end-----------//

const  int voltagePin = A0;
const  float peakVoltage  = 30;

LCD lcd(12, 11, 5, 4, 3, 2);
SerialMonitor serial_monitor;
TextWriter text_writer(&lcd);
BatteryMonitor battery(voltagePin, peakVoltage);
Relay relay(8);

// --------- Varibale Declaration--------//

void setup(){
  Serial.begin(9600);
  battery.setup();
  lcd.init();
}

void loop(){
  if (battery.getBatteryPercentage()<80) 
    relay.turnOn();
   else 
    relay.turnOff();
  
  battery.updateData();
  
  lcd.setCursor(0, 0);
  text_writer.write(String((int)battery.getBatteryPercentage() +  String("%") + String(" Charged")));
  lcd.setCursor(0, 1);
  text_writer.write(String(battery.getBatteryHealth() +  String("%") + String(" Health")));
  delay(100);
}
