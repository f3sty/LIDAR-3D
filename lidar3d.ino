#include <TinyStepper.h>
#include <math.h>

// Define Pin Outputs to to the ULN2003 Darlington Array to drive a 28BYJ-48 Stepper Motor
#define IN1 PB12
#define IN2 PB13
#define IN3 PB14
#define IN4 PB15
#define HALFSTEPS 4096  // Number of half-steps for a full rotation

// Initialize the TinyStepper Class
TinyStepper stepper(HALFSTEPS, IN1, IN2, IN3, IN4);

int16_t i;
String inputString;
float angle = 0;
char buffer[5100];


void setup() {
  Serial.begin(115200);
  Serial2.begin(230400);
  Serial2.setTimeout(3000);
  stepper.Enable();
}

void loop() {
  if (Serial.available() > 1) {
    inputString += Serial.readStringUntil(13);
    if (inRange(inputString.indexOf("stop"), 0, 254)) {
      Serial.println("stopping");
      Serial2.println("e");
    }
    else if (inRange(inputString.indexOf("scan:"), 0, 254)) {
      uint16_t sAngle = inputString.substring(inputString.indexOf("scan:") + 5).toInt();
      float stepSize = inputString.substring(inputString.lastIndexOf(":") + 1).toFloat();
      Serial.println("# starting scan");
      Serial2.print("b");
      delay(3000);

      for (angle = 0; angle <= sAngle; angle += stepSize) {
        stepper.Move(stepSize, 5);
        delay(500);
        Serial.print("#\n#AZ: "); Serial.println(angle);
        if (Serial2.available())
        {
          uint16_t numchars = Serial2.readBytes(buffer, sizeof(buffer));
          char SOR[3] = {0xFA, 0xA0};
          for ( uint16_t i = 0 ; i < sizeof(buffer); i++) {
            if ((buffer[i] == SOR[0]) && (buffer[i + 1] == SOR[1])) {
              for (uint16_t p = 0; p < 2520; p++) {
                if (p % 42 == 0) {
                  uint16_t inc_idx = buffer[i + p + 1] - 160;
                  if (inc_idx > 0) {
                    inc_idx = inc_idx * 6;
                  }
                  uint16_t rpm = (buffer[p + i + 3] << 8) + buffer[p + i + 2];
                  for (uint8_t idx = 0; idx < 6; idx++) {
                    uint16_t dst = (buffer[p + i + (idx * 6) + 7] << 8) + buffer[p + i + (idx * 6) + 6];
                    int16_t inc = (inc_idx + idx);
                    float c_az = angle;

                    Serial.print(c_az); Serial.print(", "); Serial.print(inc); Serial.print(", "); Serial.println(dst);
                  }

                }
              }
              break;
            }
          }
        }
        else {
          Serial.println("#No data received");
          delay(2000);
        }
        inputString = "";
      }
      stepper.Move(angle * -1, 5);
      Serial2.println("e");
    }

    else {
      float angle_ = inputString.toFloat();
      //angle = inputString.toInt();
      inputString = "";
      stepper.Move(angle_, 5);
      delay(500);
      Serial.print("#Angle: "); Serial.println(angle_);
    }
  }
}


// function to check if a given int is within a range
bool inRange(int val, int minimum, int maximum)
{
  return ((minimum <= val) && (val <= maximum));
}
