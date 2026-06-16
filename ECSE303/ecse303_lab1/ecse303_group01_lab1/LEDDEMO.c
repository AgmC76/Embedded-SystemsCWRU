#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    // Make these GPIO's available by exporting them.
    system("echo 17 > /sys/class/gpio/export");    // LED1, red
    system("echo 27 > /sys/class/gpio/export");    // LED2, green
    system("echo 22 > /sys/class/gpio/export");	 // LED3, blue

    // Set direction to output for each GPIO.
    system("echo out > /sys/class/gpio/gpio17/direction");
    system("echo out > /sys/class/gpio/gpio27/direction");
    system("echo out > /sys/class/gpio/gpio22/direction");

    // Alternate LED control for 12 cycles (1 minute, every 5 seconds)
    for (int i = 0; i < 12; i++) {
        // Turn on the red LED (GPIO 17), off others
        system("echo 1 > /sys/class/gpio/gpio17/value");
        system("echo 0 > /sys/class/gpio/gpio27/value");
        system("echo 0 > /sys/class/gpio/gpio22/value");
        sleep(5);  // 5 second delay

        // Turn on the green LED (GPIO 27), off others
        system("echo 0 > /sys/class/gpio/gpio17/value");
        system("echo 1 > /sys/class/gpio/gpio27/value");
        system("echo 0 > /sys/class/gpio/gpio22/value");
        sleep(5);  // 5 second delay

        // Turn on the blue LED (GPIO 22), off others
        system("echo 0 > /sys/class/gpio/gpio17/value");
        system("echo 0 > /sys/class/gpio/gpio27/value");
        system("echo 1 > /sys/class/gpio/gpio22/value");
        sleep(5);  // 5 second delay
    }

    // Turn off all LEDs at the end
    system("echo 0 > /sys/class/gpio/gpio17/value");
    system("echo 0 > /sys/class/gpio/gpio27/value");
    system("echo 0 > /sys/class/gpio/gpio22/value");

    // Release GPIO resources by unexporting them.
    system("echo 17 > /sys/class/gpio/unexport");
    system("echo 27 > /sys/class/gpio/unexport");
    system("echo 22 > /sys/class/gpio/unexport");

    return 0;
}

