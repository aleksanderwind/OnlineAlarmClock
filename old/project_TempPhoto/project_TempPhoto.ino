// pins
const int photosens = A0;
const int tempsens = A1;

//Parameters for moving average temperature

const int numReadingsTemp  = 20;
float readingsTemp [numReadingsTemp];
int readIndexTemp  = 0;
float totalTemp = 0;

// Parameters for moving average lumen
const int numReadingsLumen  = 20;
float readingsLumen [numReadingsLumen];
int readIndexLumen  = 0;
float totalLumen = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(photosens,INPUT);
  pinMode(tempsens,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(smoothTemp(A1),1);Serial.println("°C"); // print smoothed temperature
  Serial.print(smoothLumen(A0,220),1);Serial.println(" lm");
  delay(1000);
}

float smoothTemp(const int pinTemp) {
  /*Description
  Performs average of last N temperature readings (numReadingsTemp, defined in parameters at the beginning of doc).
  Temperature sensor should be connected to an analog pin and supplied with 5V. 
  Input [int]: Pin for temperature sensor
  Output [float]: Average temperature in degrees Celsius of last N readings
  
  Example: 
  >>> smoothTemp(A1)
    22.4
  */ 
  
  float averageTemp;
  // subtract the last reading:
  totalTemp = totalTemp - readingsTemp[readIndexTemp];
  // read the sensor:
  int tempsensorVal = analogRead(pinTemp);
  float voltageTempSens = tempsensorVal * (5.0/1024.0); // conversion from sensor reading in bits to voltage in mV
  // float temp = ((voltageTempSens*1000)-500)/10; //For TMP36GZ, conversion from mV to deg C, TMP36GZ has offset of 500mV
  float temp = voltageTempSens*100; // For LM35DZ, conversion from V to deg C
  readingsTemp[readIndexTemp] = temp;
  // add value to totalTemp:
  totalTemp = totalTemp + readingsTemp[readIndexTemp];
  // handle index
  readIndexTemp = readIndexTemp + 1;
  if (readIndexTemp >= numReadingsTemp) {
    readIndexTemp = 0;
  }
  // calculate the average:
  averageTemp = totalTemp / numReadingsTemp;

  return averageTemp;
}

int smoothLumen(const int ldrPin,const int R){
  /* Description
  Performs average of last N lumen readings (numReadingsLumen, defined in parameters at the beginning of doc).
  Light dependent resistor (LDR) should be connected to an analog pin and supplied with 5V. 
  Input [int]: Pin for LDR (ldrPin), Value of resistor connected to the LDR (R)
  Output [float]: Average luminosity in lumen of last N readings (lumenVal)
  
  Example: 
  >>> smoothLumen(A1)
    94
  */
  // Conversion rule
  float averageLumen;
  int photosensorVal = analogRead(ldrPin);
  float Vout = float(photosensorVal) * (5 / float(1023));// Conversion analog to voltage
  float Rldr = (R * (5 - Vout))/Vout; // Conversion voltage to resistance
  float lumenVal=500/(Rldr/1000); // Conversion resitance to lumen
  // subtract last reading
  totalLumen = totalLumen - readingsLumen[readIndexLumen];
  readingsLumen[readIndexLumen] = lumenVal;
  // add value to totalLumen:
  totalLumen = totalLumen + readingsLumen[readIndexLumen];
  // handle index
  readIndexLumen = readIndexLumen + 1;
  if (readIndexLumen >= numReadingsLumen) {
    readIndexLumen = 0;
  }
  // calculate the average:
  averageLumen = totalLumen / numReadingsLumen;

  return averageLumen;
}
