#include <stdio.h>
#include <wiringPi.h>

// set the pin for the LED
int PWMPIN = 1; // wiringPi pin 1

// set the pin to output
void setup() {
   pinMode(PWMPIN, PWM_OUTPUT); 
   pwmSetMode(PWM_MODE_MS);
   pwmSetRange(1024);
   pwmSetClock(32);
}


// blink the LED
void loop() {
    for (int i = 0; i < 1024; i++) { // increases the frequency to the upper limit
        pwmWrite(PWMPIN, i);
        delay(2); 
    }

    for (int i = 1023; i >= 0; i--) { // decreases the frequency to the lower limit
        pwmWrite(PWMPIN, i);
        delay(2); 
    }
}

int main() {
    if (wiringPiSetup() == -1) {
        printf("Setup failed\n");
        return 1;
    }

    setup();

    while ( 1 ) {
        loop();
    }

    return(0);
    
}
