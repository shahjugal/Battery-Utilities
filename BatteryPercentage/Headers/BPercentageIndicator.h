namespace ShahJugalR{
    namespace BatteryUtilities{
        class BPerc
        {
        private:
            int baseVoltage;
            int offset;
        public:
            BPerc() = delete;

            /* 
                @brief Initialize a instance of BPerc Class with base voltage.
                @param baseVoltage is the max expected voltage of battery.
                @param offset is the after calculation offset needed to be applied.
            */
            BPerc(int baseVoltage, int offset = 0){
                this->baseVoltage = baseVoltage;
                this->offset = offset;
            }

            ~BPerc();

            /*
                @brief Returns the battery levels out of 4.
                @param reading is the analog reading from the voltage sensor.
                @returns integer clamped between 0 to 100 i.e. percentage.
                @note Please pass the analogReading without any manipulation.
            */
            int QueryLevel(int reading){
                double voltage = reading * (baseVoltage/1023.0) + offset;
              	voltage *= 100;
                voltage /= baseVoltage;
                if(voltage > 100)
                    voltage = 100;
                else if(voltage < 0)
                    voltage = 0;
                return voltage;
            }
        };
    }

}
