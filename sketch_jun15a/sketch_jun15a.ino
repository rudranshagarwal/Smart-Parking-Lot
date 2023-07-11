#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "Servo.h"
#include "time.h"

#define triggerPin 5
#define echoPin 33
#define soundSpeed 343
Servo servo;

char *ssid = "Redmi 6A";
char *password = "Rudransh123";
String cse_ip = "192.168.43.179"; // YOUR IP from ipconfig/ifconfig
String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
String ae = "ultrasonic";
int parkings1 = 1;
int parkings2 = -1;
String ae2 = "parkings";
String cnt1 = "parking1";
String cnt2 = "parking2";
String cnt3 = "parking3";
String cnt4 = "parking4";
String cnt5 = "parking1";
String cnt6 = "parking2";
String cnt7 = "parking3";
String cnt8 = "parking4";

static const int servoPin = 13;
int a[2][2];
int b[2][2];
int c[2][2];
double occupiedat[2][2];

int x;

Servo servo1;
void setup()
{
    x = 0;
    int i, j;
    b[0][0] = 21;
    b[0][1] = 4;
    b[1][0] = 19;
    b[1][1] = 18;
    c[0][0] = 32;
    c[0][1] = 27;
    c[1][0] = 26;
    c[1][1] = 25;OS
    occupiedat[0][0] = 0;
    occupiedat[0][1] = 0;
    occupiedat[1][0] = 0;
    occupiedat[1][1] = 0;
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());

    servo1.attach(servoPin);
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            a[i][j] = -1;
            pinMode(b[i][j], OUTPUT);
            pinMode(c[i][j], INPUT);
        }
    }
    createCi2(String(parkings2), cnt5);
    createCi2(String(parkings2), cnt6);
    createCi2(String(parkings2), cnt7);
    createCi2(String(parkings2), cnt8);
}

void createCi(String val, String cnt)
{
    HTTPClient http;
    http.begin(server + ae + "/" + cnt + "/");
    http.addHeader("X-M2M-Origin", "admin:admin");
    http.addHeader("Content-Type", "application/json;ty=4");
    int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");
    Serial.println(code);
    if (code == -1)
    {
        Serial.println("UNABLE TO CONNECT TO THE SERVER");
    }
    http.end();
}

void createCi2(String val, String cnt)
{
    HTTPClient http;
    http.begin(server + ae2 + "/" + cnt + "/");
    http.addHeader("X-M2M-Origin", "admin:admin");
    http.addHeader("Content-Type", "application/json;ty=4");
    int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");
    Serial.println(code);
    if (code == -1)
    {
        Serial.println("UNABLE TO CONNECT TO THE SERVER");
    }
    http.end();
}

void loop()
{
start:
    delay(150); // for printing, remove later
    int i, j, count = 0;
    digitalWrite(triggerPin, HIGH);
    delay(10); 
    digitalWrite(triggerPin, LOW);
    int duration = pulseIn(echoPin, HIGH);
    //  digitalWrite(triggerPin, HIGH);
    //  delay(10);
    //  digitalWrite(triggerPin, LOW);
    int distance = (0.034 * duration) / 2;

    Serial.print("dist = ");
    Serial.println(distance);
    Serial.print("Sensor Distances: ");
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            digitalWrite(triggerPin, HIGH);
            delay(10);
            digitalWrite(triggerPin, LOW);
            int durationcurr = pulseIn(c[i][j], HIGH);
            int distance = (0.034 * durationcurr) / 2;
            if (distance <= 10)
            {
                delay(50);
                digitalWrite(triggerPin, HIGH);
                delay(10);
                digitalWrite(triggerPin, LOW);
                int durationcurr = pulseIn(c[i][j], HIGH);
                int distance = (0.034 * durationcurr) / 2;
                Serial.print(distance);
                Serial.print(" ");
                
                if (distance <= 10)
                {
                    a[i][j] = 1;
                    count++;
                }
                else
                {
                    j--;
                    continue;
                }
            }
            else
            {
                delay(50);
                digitalWrite(triggerPin, HIGH);
                delay(10);
                digitalWrite(triggerPin, LOW);
                int durationcurr = pulseIn(c[i][j], HIGH);
                int distance = (0.034 * durationcurr) / 2;
                Serial.print(distance);
                Serial.print(" ");
                if (distance <= 10)
                {
                    j--;
                    continue;
                }
                if (a[i][j] == 1)
                {
                    if (i == 0 && j == 0)
                    {
                        createCi(String(millis() - occupiedat[i][j]), cnt1);
                        createCi2(String(parkings2), cnt5);
                    }

                    else if (i == 0 && j == 1)
                    {
                        createCi(String(millis() - occupiedat[i][j]), cnt2);
                        createCi2(String(parkings2), cnt6);
                    }

                    else if (i == 1 && j == 0)
                    {
                        createCi(String(millis() - occupiedat[i][j]), cnt3);
                        createCi2(String(parkings2), cnt7);
                    }

                    else if (i == 1 && j == 1)
                    {
                        createCi(String(millis() - occupiedat[i][j]), cnt4);
                        createCi2(String(parkings2), cnt8);
                    }
                }
                a[i][j] = -1;
            }

//            Serial.print(distance);
//            Serial.print(" ");
        }

        Serial.println();
        Serial.print("count = ");
        Serial.println(count);
        if (count == 4)
            goto start;
        {
            if (distance < 20)
            {
                digitalWrite(triggerPin, HIGH);
                delay(10);
                digitalWrite(triggerPin, LOW);
                int durationcurr = pulseIn(c[i][j], HIGH);
                int distance = (0.034 * durationcurr) / 2;
                if (distance < 20)
                {
                    for (i = 0; i < 2; i++)
                    {
                        for (j = 0; j < 2; j++)
                        {
                            digitalWrite(triggerPin, HIGH);
                            delay(10);
                            digitalWrite(triggerPin, LOW);
                            int durationcurr = pulseIn(c[i][j], HIGH);
                            int distance = (0.034 * durationcurr) / 2;
                            Serial.print("1 ");
                            Serial.print(distance);
                            Serial.print(" ");
                        }
                    }
                    x++;
                    if (x > 1)
                    {
                        int i, j, flag = 0;
                        for (i = 0; i < 2; i++)
                        {
                            for (j = 0; j < 2; j++)
                            {
                                if (a[i][j] == -1)
                                {
                                    digitalWrite(b[i][j], HIGH);
                                }
                            }
                        }
                        Serial.print("i = ");
                        Serial.print(i);
                        Serial.print(", j = ");
                        Serial.println(j);
                        //           a[i][j] = 1;
                        //           occupiedat[i][j] = millis();
                        digitalWrite(b[i][j], HIGH);
                        delay(1000);
                        for (int posDegrees = 0; posDegrees <= 90; posDegrees++)
                        {
                            servo1.write(posDegrees);

                            delay(10);
                        }
                        delay(3000);

                        for (int posDegrees = 90; posDegrees >= 0; posDegrees--)
                        {
                            servo1.write(posDegrees);

                            delay(10);
                        }
                        for (i = 0; i < 2; i++)
                        {
                            for (j = 0; j < 2; j++)
                            {
                                digitalWrite(triggerPin, HIGH);
                                delay(10);
                                digitalWrite(triggerPin, LOW);
                                int durationcurr = pulseIn(c[i][j], HIGH);
                                int distance = (0.034 * durationcurr) / 2;
                                if (distance < 10 && a[i][j] == -1)
                                {
                                    a[i][j] = 1;
                                    occupiedat[i][j] = millis();
                                    if (i == 0 && j == 0)
                                    {
                                        createCi2(String(parkings1), cnt5);
                                    }

                                    else if (i == 0 && j == 1)
                                    {
                                        createCi2(String(parkings1), cnt6);
                                    }

                                    else if (i == 1 && j == 0)
                                    {
                                        createCi2(String(parkings1), cnt7);
                                    }

                                    else if (i == 1 && j == 1)
                                    {
                                        createCi2(String(parkings1), cnt8);
                                    }
                                }
                                digitalWrite(b[i][j], LOW);
                                Serial.print(a[i][j]);
                                Serial.print(" ");
                            }
                            Serial.println();
                        }
                        // digitalWrite(b[i][j], LOW);
                    }
                }
            }
        }
    }
}
