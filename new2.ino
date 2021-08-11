#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "proj1-6e9d3-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "AL3K6e6ej7rIdrHrfdZraGOzIYvSZ4VoBNDmDEm9"
#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD "zxcvbnmlk"


int gas_sensor = A0; //Sensor pin
float m = -0.318; //Slope
float b = 1.133; //Y-Intercept
float R0 = 5.5;

void setup() {
  Serial.begin(9600);
 
  pinMode(4,INPUT); // set input pin
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


//int mq4_database,mq4_sensor_value;
void loop(){
  
  float sensor_volt=0; //Define variable for sensor voltage
  float RS_gas=0; //Define variable for sensor resistance
  float ratio=0; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0); //Convert analog values to voltage
  RS_gas = ((5.0 * 1.0) / sensor_volt) - 1.0; //Get value of RS in a gas
  ratio = RS_gas / R0;   // Get ratio RS_gas/RS_air

  double ppm_log = (log10(ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale
  double percentage = ppm / 10000; //Convert to percentage
  Serial.println(percentage); //Load screen buffer with percentage value
  delay(500); // 
    Firebase.setFloat("mq4",percentage);
    //Serial.println(mq4_sensor_value);
    
double  mq4_database= Firebase.getFloat("mq4");
 Serial.println(mq4_database);
  if(mq4_database >= 250)
  { 
    Serial.println("Food is about to Spoil");
 
    }
  else {
    Serial.println("Food is hygine");
   
    }
  
  }
