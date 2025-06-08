
/*The two servo motors used for the first joint 
 * doesn't align for some pair of angles. 
 * So we have modified the angle values to 
 * achieve the goal. 
*/
const byte leftAngles[177] = {2 ,    3 ,   4 ,   5 ,   6 ,   7 ,   8 ,   9 ,   10 , 11 ,   12 ,  13 ,  14 , 15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,   22 , 23 ,   24 , 25 ,   26 ,  27 , 28 ,   29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 , 38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,   49 ,  50 ,  51 ,  52 ,  53 ,  54 , 55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,   64 ,  65 , 66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  73 ,  74 ,  75 ,  76 ,  77 ,  78 ,  79 ,  80 ,  81 , 82 , 83 , 84 , 85 , 86 , 87 , 88 , 89 , 90 , 90 , 91 , 92 , 93 , 94 , 95 , 96 , 97 , 98 , 99 , 100 , 101 , 102 , 103 , 104 , 105 ,  106 , 107 ,  107, 108 , 109 , 110 , 111 , 112 , 113 , 114 , 115 , 116 , 117 , 118 , 119 , 120 , 121 , 122 , 123 , 124 , 125 , 125 , 126 , 127 , 128 , 129 , 130 , 131 , 132 , 133 ,133  , 134,  134 , 135 , 136 , 137 , 138 , 139 , 140 , 141 , 142 , 143 , 144 , 145 , 146 , 147 , 148 , 149 , 150 , 151 , 152 , 153 , 154 , 155 , 156 , 157 , 158 , 159 , 160 , 161 , 162 , 163 , 164 , 165 , 166 , 167, 168 , 169  , 170 , 171 , 172 , 173  };
const byte rightAngles[177] ={178 , 177 , 176 , 175 , 174 , 173 , 172 , 171 , 170 , 169 , 168 , 167 , 166 , 165 , 164 , 163 , 162 , 161 , 160 , 159 , 158 , 157 , 156 , 155 , 154 , 153 , 152 , 151 , 150 , 149 , 148 , 147 , 146 , 145 , 144 , 143 , 142 , 141 , 140 , 139 , 138 , 137 , 136 , 135 , 134 , 133 , 132 , 131 , 130 , 129 , 128 , 127 , 126 , 125 , 124 , 123 , 122 , 121 , 120 , 119 , 118 , 117 , 116 , 115 , 114 , 113 , 112 , 111 , 110 , 109 , 108 , 107 , 106 , 105 , 104 , 103 , 102 , 101 , 100 , 99 , 98 , 97 , 96 , 95 , 94 , 93 , 92 , 91 , 90 , 89 , 88 , 87 , 86 , 85 , 84 , 83 , 82 , 81 , 80 ,  79 ,  78 ,   77 , 76 ,  75 ,  74 ,   73 ,  72 ,  71 ,  70 ,  69 ,  68 ,  67 , 66  , 65  , 64  , 63  , 62  , 61  , 60  , 59  , 58  , 57  , 56  , 55  , 54  , 53  , 52  , 51  , 50  , 49  , 48  , 47  , 46  , 45  , 44  , 43  , 42  , 41  , 40  , 39  , 38  , 37  , 36  , 35  , 34  , 33  , 32  , 31  , 30  , 29  , 28  , 27  , 26 ,  25 ,  24  , 23  , 22  , 21  , 20 ,  19 ,  18  , 17 ,  16   , 15 , 14 ,   13 , 12  , 11 ,   10 ,   9 ,  8 ,  7 ,   6  ,   5 ,   4 ,   3 ,   2  };
byte anglePairIndex = 0; //Index of which angle is being written in the motor according 
                         //the above two lists. 
                         
//Pin declaration for the gripper motor. 
#define gripperIn1 14
#define gripperIn2 18

//Pin declaration for the unused 1 motor of the second l293d motor driver. 
#define wristMotorIn3 15
#define wristMotorIn4 21


//Unused servo motors are commented out. 

//PWM pins of arduino Uno -   3, 5, 6, 9, 10, 11
Servo armServo1; //Continuous rotation Servomotor at the base of the robotic arm
Servo armServo2Left; //Continuous rotation Left servo of the second joint from the bottom. 
Servo armServo2Right; //Continuous rotation Right servo for the second joint from the bottom. 
//Servo armServo3; //The fourth servo motor from the base of the robotic arm
Servo armServo4; //The fifth servo motor from the base of the robotic arm
//Servo armServo5; //The sixth servo motor from the base of the robotic arm

 byte servoPositions[6] = {90, leftAngles[anglePairIndex], rightAngles[anglePairIndex], 90, 90, 90}; //Initial positions of the servo motors used in the arm
                                                 //armServo1, armServo3 and armServo4 are continuous rotation, so we set 90 degree (stop). 

#define angleChangeDelay  5
#define CRSruntime   30    //The time period for which we will run (backward/forward) the Continous Rotation Servo (CSR) for 1 press. 
#define CRSspeed 15  //In range 0 to 90


void initializeRoboticArm()
    {
        
  
            //Setting the servo's to their initial positions. 
            unsigned short moveGap = 700; //The gap between writing initial angles of each successive servos
            armServo1.write(servoPositions[0]); 
            //delay(moveGap); 
            armServo2Left.write(servoPositions[1]); 
            armServo2Right.write(servoPositions[2]); 
            delay(moveGap); 
          //  armServo3.write(servoPositions[2]);
            delay(moveGap);  
            armServo4.write(servoPositions[3]); 
            delay(moveGap); 
          //  armServo5.write(servoPositions[4]); 
            delay(moveGap); 
    }

  

void moveWrist(int actionValue)
      {
           if(actionValue == 1)
          {
            DigitalWrite(wristMotorIn3, HIGH); 
            DigitalWrite(wristMotorIn4, LOW); 
            delay(10); 
            DigitalWrite(wristMotorIn3, LOW); 
            DigitalWrite(wristMotorIn4, LOW); 
          }
          else if(actionValue == -1)
          {
            DigitalWrite(wristMotorIn3, LOW); 
            DigitalWrite(wristMotorIn4, HIGH); 
            delay(10); 
            DigitalWrite(wristMotorIn4, LOW);
            DigitalWrite(wristMotorIn3, LOW);  
          }
      }
 

void moveGripper(int actionValue) //actionvalue is the value part of the command recieved from app via Bluetooth. 
{
   if(actionValue == 1)
          {
            DigitalWrite(gripperIn1, HIGH); 
            DigitalWrite(gripperIn2, LOW); 
            delay(10); 
            DigitalWrite(gripperIn1, LOW); 
            DigitalWrite(gripperIn2, LOW); 
          }
          else if(actionValue == -1)
          {
            DigitalWrite(gripperIn1, LOW); 
            DigitalWrite(gripperIn2, HIGH); 
            delay(10); 
            DigitalWrite(gripperIn2, LOW); 
            DigitalWrite(gripperIn1, LOW); 
          }
}
