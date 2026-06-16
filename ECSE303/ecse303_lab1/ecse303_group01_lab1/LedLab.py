from gpiozero import LED
import time

red = LED(17)
green = LED(27)
blue = LED(22)

while True:
	red.on()
	time.sleep(0.5)
	red.off()
	time.sleep(0.5)
	# green led
	green.on()
	time.sleep(0.5)
	green.off()
	time.sleep(0.5)
	# blue led
	blue.on()
	time.sleep(0.5)
	blue.off()
	time.sleep(0.5)
