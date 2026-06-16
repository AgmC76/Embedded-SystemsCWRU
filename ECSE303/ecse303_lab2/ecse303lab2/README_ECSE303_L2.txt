README

Lab 2 ECSE303 contains the pdf for the written lab report
dht11_data.txt contains the outputs for the sensor data
	the data can be read from the command line using the command: cat dht11_data.txt

lab2polling2.c contains the code for reading the sensor data using polling in c
- compile and run with the following commands:
gcc -o lab2polling2 lab2polling2.c -lwiringPi -lwiringPiDev
sudo ./lab2polling2

dht11_interrupt.c contains the code for reading the sensor data using interrupts in c
- run with the following commands:
gcc -o dht11_interrupt dht11_interrupt.c -lwiringPi -lwiringPiDev
sudo ./dht11_interrupt