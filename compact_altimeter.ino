#include <Adafruit_BMP085_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

//#include <EEPROMex.h>

//#define DEBUG

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

float altitude;
float highestAltitude;
float seaLevelPressure = 1032;
float lastAltitude;
const float baseAltitude = 241;  //Fairlie Moor = 240m
const int beeper =	4;
const int triggerDistance = 10;

/**************************************************************************/
/*
Displays some basic information on this sensor from the unified
sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
	sensor_t sensor;
	bmp.getSensor(&sensor);
	Serial.println("------------------------------------");
	Serial.print  ("Sensor:       "); Serial.println(sensor.name);
	Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
	Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
	Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
	Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
	Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
	Serial.println("------------------------------------");
	Serial.println("");

	//lastAltitude = EEPROM.readFloat(1);

	Serial.print("Last Flight: ");
	Serial.print (lastAltitude);
	Serial.print ("m / ");
	Serial.print (lastAltitude* 3.2808399);
	Serial.println ("ft");
	//delay(5000);
	sensors_event_t event;
	bmp.getEvent(&event);
	seaLevelPressure = bmp.seaLevelForAltitude(baseAltitude, event.pressure);
	Serial.print("Sea Level Pressure: ");
	Serial.println (seaLevelPressure);
}


void beep (){
#ifdef DEBUG
	Serial.println("Beep");
#endif
	digitalWrite (beeper, HIGH);
	delay(500);
	digitalWrite (beeper, LOW);
	delay (1000);
}

void setup(void) 
{
	Serial.begin(115200);
	Serial.println("Pressure Sensor Test"); Serial.println("");

	pinMode(beeper, OUTPUT);

	/* Initialise the sensor */
	if(!bmp.begin())
	{
		/* There was a problem detecting the BMP085 ... check your connections */
		Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
		while(1);
	}

	/* Display some basic information on this sensor */
	displaySensorDetails();
	beep();
}

void loop(void) 
{
	/* Get a new sensor event */ 
	sensors_event_t event;
	bmp.getEvent(&event);

	/* Display the results (barometric pressure is measure in hPa) */
	if (event.pressure)
	{

		altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
#ifdef DEBUG
		Serial.print(altitude); 
		Serial.println(" m");
#endif
		if (altitude > highestAltitude){
			highestAltitude = altitude;
#ifdef DEBUG
			Serial.print("Highest Altitude:    "); 
			Serial.print(altitude); 
			Serial.println(" m");
#endif

		} else if (highestAltitude - altitude > triggerDistance){
#ifdef DEBUG
			Serial.print("Altitude:    "); 
			Serial.println(altitude); 
#endif

			//EEPROM.writeFloat (1, highestAltitude-baseAltitude);
			beep();

		}
	}
	else
	{
		Serial.println("Sensor error");
	}
}


