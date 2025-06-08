Servo sensorServo;  //Servomotor carrying the sonar sensor.
#define SensorServoPin 12 //Pin for sensorServo

#define trigPin 13
#define echoPin A0

unsigned short middleDistance = 0; 
unsigned short rightDistance = 0; 
unsigned short leftDistance = 0; 
unsigned short oaSpeed = 90; //PWM value while driving in obstacle avoidance (OA) mode. 
#define successiveAngleWriteDelay 5 //The delay between changing each degree angle of the ultrasonic sensor. 

//When the car was initially built it was being controlled by 
//another arduino (Nano) via a joystick over bluetooth. 
//These settings helped to finetune different parameters realtime without uploading the code. 
//Though the current version expects a mobile app to send commands; 
//but this setting was pretty meaningful for the Arduino Nano controller. 

#define noSettings 5               //Number of Settings

        //   String oaSettings[noSettings] =  {"OA BF PWM", "OA BF time", "OA LR PWM" , "OA LR time", "Safe Distance"};   //The obstacle avoidance mode settings and values. 
 unsigned short  oaSettingsValue[noSettings] = {200,         80,           230,          400,          12 };     //Time in millisecond, Safe distance is in inch. 
                                      /*Index:  0             1             2             3             4*/
                  


int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); //Extra pin number 14
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 long duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  
  return duration*0.0068; //returns distance in inch. 
}


void avoidObstacles()
{
   middleDistance = calculateDistance(); 
// serialPrint('M', middleDistance); 

      if(middleDistance < oaSettingsValue[4]) //oaSettingsValue[4] = safeDistance
      {
       Stop(); 

       for(int i = 90; i>=0; i--)
       {
       sensorServo.write(i);
       delay(successiveAngleWriteDelay); 
       }   
                                         //Moving US sensor to the right slowly
       delay(1000);                          //Keeping the sensor steady for 1000 ms. 
       rightDistance = calculateDistance();  //returns distance in inch unit
     //  serialPrint('R', rightDistance);      //Sending the reading to the controller.
       delay(500); 
       
       for(int i = 0; i<=180; i++)
       {
       sensorServo.write(i); //Moving ultrasonic sensor to the left
       delay(successiveAngleWriteDelay); 
       }

       delay(1000);       //Keeping the sensor steady on the servo to get a stable reading. 
       leftDistance = calculateDistance();  //Calculating left distance. 
      // serialPrint('L', leftDistance);      //Sending the data to the controller. 
       delay(500); 
       
      for(int i  =180; i>= 90; i--){
       sensorServo.write(i);               //Taking back the sensor on the servo to front direction 
       delay(successiveAngleWriteDelay); 
      }
       delay(1000);                          //Having a short delay before going to newly found direction. 
      
    
        if(rightDistance >= oaSettingsValue[4] && rightDistance >= leftDistance)  //Whether right side has enough safe
       {
        goRight(oaSettingsValue[2]);  
        delay(oaSettingsValue[3]); 
       }
       else if(leftDistance >= oaSettingsValue[4])   //oaSettingsValue[4]  = safe distance. 
       {
          goLeft(oaSettingsValue[2]); 
          delay(oaSettingsValue[3]);
       }
      else{
        goBackward(oaSettingsValue[0]); 
        delay(oaSettingsValue[1]); 
       }
       
      } 
      else 
      {
        goForward(oaSettingsValue[0]); 
      }
}
