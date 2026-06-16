README

ECSE 303 Group 01

Lab 4 ECSE303 contains the pdf for the written lab report

LED_BlinkC.c contains the code for utilizing pwm and wiringPi to fade in/out an LED in c
LED_Blink is the executable file generated after compiling the code in C
- compile and run with the following commands:
gcc -o LED_Blink LED_BlinkC.c -lwiringPi -lwiringPiDev
sudo ./LED_Blink

LED_BlinkASM.s contains the code for utilizing pwm and wiringPi to fade in/out an LED in assembly
Lab4_Demo contains the executable file generated after compiling the code in assembly
- compile and run with the following commands:
gcc LED_BlinkASM.s -o Lab4_Demo -lwiringPi -lwiringPiDev
sudo ./Lab4_Demo

L4_bench.jpg is a photo of our raspberry pi and the circuit.

L4_video_demo.mov is video our LED fading in and out in a dark-room.
