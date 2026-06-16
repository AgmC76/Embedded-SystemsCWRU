// gcc -Os dht1_polling.c -lwiringPi -o dht1_polling

#include <wiringPi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <wiringPiI2C.h>
#include <signal.h>

#define MAXTIMINGS    85
#define DHTPIN        3
#define LEDPIN	      1
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

// for the lcd
int backLight;
int textDisplay;

void read_dht11_dat()
{
    uint8_t laststate    = HIGH;
    uint8_t counter        = 0;
    uint8_t j        = 0, i;
    float   f;                         // fahrenheit
    float   h;			       // humidity
    float   c; 			       // celsius

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    // pull pin down for 18 milliseconds
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    // pull it up for 40 microseconds
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    // prepare to read the pin
    pinMode( DHTPIN, INPUT );
    // setup for LED pin
    pinMode( LEDPIN, OUTPUT);

    // detect change and read data
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 3 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        // ignore first 3 transitions
        if ( (i >= 4) && (i % 2 == 0) )
        {
            // shove each bit into the storage bytes
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    // verify checksum with reading data.
    if ( (j >= 40) &&
         (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) )
	// the checksum must be wrong, everything below is skipped.    
    {
        h = dht11_dat[0];
        c = dht11_dat[2];
        f = c * (9. / 5.) + 32;
        printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );
	//printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n", dht11_dat[0], dht11_dat[2], dht11_dat[3], f);


        // print time
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("Current time: %d-%02d-%02d %02d:%02d:%02d\n",
               tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
               tm.tm_hour, tm.tm_min, tm.tm_sec);

        // write humidity and temperature into file
        FILE *fp = fopen("dht11_data.txt", "a");
        if (fp != NULL)
        {
            fprintf(fp, "Time: %d-%02d-%02d %02d:%02d:%02d Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n",
                    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                    tm.tm_hour, tm.tm_min, tm.tm_sec,
                    h, c, f);
            fclose(fp);
        }
        else
        {
            printf("Failed to open file for writing\n");
        }

    }
    else
    {
        printf( "Data not good, skip\n" );
	// h = dht11_dat[0];
        // c = dht11_dat[2];
	// f = c * 9. / 5. + 32;
	// printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );
    	

    }
}

void thresholdExceeded() {
	// if the temperature exceeds a threshold, it should turn on an LED
	float threshold = 75.0;
        float c = dht11_dat[2];
        float f = c * (9. / 5.) + 32;
	if (f >= threshold) {
	digitalWrite(LEDPIN, HIGH);
	}
	else {
	digitalWrite(LEDPIN, LOW);
	}
}

// methods for the lcd
// Input: color RGB components. Range 0~255, inclusive.
void setBGColor(unsigned char red, unsigned char green, unsigned blue)
{
	// Initialize.
	wiringPiI2CWriteReg8(backLight, 0x00, 0x00);
	wiringPiI2CWriteReg8(backLight, 0x01, 0x00);
	
	// All LED control by PWM.
	wiringPiI2CWriteReg8(backLight, 0x08, 0xAA);
	
	// Set color component.
	wiringPiI2CWriteReg8(backLight, 0x04, red);
	wiringPiI2CWriteReg8(backLight, 0x03, green);
	wiringPiI2CWriteReg8(backLight, 0x02, blue);
}

// Set text of diaplay.
int setText(const char *string)
{
	// Clear display.
	textCommand(0x01);
	
	delay(50);
	
	// Display on, no cursor.
    textCommand(0x08 | 0x04);
    
    // Display two lines.
    textCommand(0x28);
    
    delay(50);
    
    int count = 0;
    int row = 0;
    int i = 0;
    
    for (; string[i] != '\0'; ++i) {
        if ((string[i] == '\n') || (count == 16)) {
            count = 0;
            ++row;
            if (row == 2) {
				// Reach maximum line number. Truncate any characters behind.
                break;
			}
            textCommand(0xc0);
            if (string[i] == '\n') {
                continue;
			}
		}
        ++count;
        wiringPiI2CWriteReg8(textDisplay, 0x40, string[i]);
	}
}

// Signal handler.
void sigIntHandler(int signal)
{
	// Clear screen.
	textCommand(0x01);
	
	// Disable backlight.
	setBGColor(0, 0, 0);
	
	printf("\nProgram ended with exit value -2.\n");
	exit(-2);
}

int main()
{
    if ( wiringPiSetup() == -1 )
        exit( 1 );

    while ( 1 )
    // while (read_dht11_dat() == false)
    {
        read_dht11_dat();
	thresholdExceeded();
        // wait 3sec to refresh
        delay( 3000 );
    }

    return(0);
}
