// gcc dht11_interr.c -o dht11_interr -lwiringPi -Wall

#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAXTIMINGS 85
#define DHTPIN 7

int data[MAXTIMINGS];
int data_counter = 0;
uint32_t lastTime = 0;
int bitidx = 0;

void sensorReadISR(void)
{
    uint32_t currentTime = micros();
    int duration = currentTime - lastTime;
    lastTime = currentTime;

    if (data_counter < MAXTIMINGS)
    {
        data[data_counter++] = duration;
    }
}

void initiateRead()
{
    // Reset variables
    data_counter = 0;
    lastTime = micros();

    // Send start signal
    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18); // 18 ms

    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);

    // Prepare to read the pin
    pinMode(DHTPIN, INPUT);
}

int main(void)
{
    if (wiringPiSetup() == -1)
        exit(1);

    // Set up ISR
    if (wiringPiISR(DHTPIN, INT_EDGE_BOTH, &sensorReadISR) < 0)
    {
        printf("Unable to setup ISR\n");
        exit(1);
    }

    while (1)
    {
        initiateRead();
        delay(5); // Wait for data transmission to complete

        // Process data
        int bits[40];
        int i, j = 0;

        // Skip first 3 transitions (DHT11 response signal)
        for (i = 3; i < data_counter; i += 2)
        {
            int duration = data[i];
            if (duration > 40)
            {
                bits[j++] = 1;
            }
            else
            {
                bits[j++] = 0;
            }
        }

        if (j >= 40)
        {
            // Parse the 40 bits into 5 bytes
            uint8_t dht11_dat[5] = {0, 0, 0, 0, 0};
            for (i = 0; i < 40; i++)
            {
                dht11_dat[i / 8] <<= 1;
                dht11_dat[i / 8] |= bits[i];
            }

            // Verify checksum
            if (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))
            {
                float h = dht11_dat[0];
                float c = dht11_dat[2];
                float f = c * 9. / 5. + 32;
                printf( "Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", h, c, f );

                // Print time
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                printf("Current time: %d-%02d-%02d %02d:%02d:%02d\n",
                       tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                       tm.tm_hour, tm.tm_min, tm.tm_sec);

                // Write humidity and temperature into file
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
                printf("Checksum error\n");
            }
        }
        else
        {
            printf("Not enough bits received\n");
        }

        // Wait 1 second before next read
        delay(2000);
    }

    return 0;
}
