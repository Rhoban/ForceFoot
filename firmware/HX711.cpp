#include <stdlib.h>
#include <wirish/wirish.h>
#include <terminal.h>
#include "HX711.h"

#define ANTI_GLITCH     50000

#define VALUE_SIGN(value, length) \
    ((value < (1<<(length-1))) ? \
     (value) \
     : (value-(1<<length)))

HX711::HX711(int sck_pin_, int dt_pin_)
    : sck_pin(sck_pin_), dt_pin(dt_pin_)
{
    antiGlitch = false;
    value = 0;
    taring = false;
}

void HX711::init()
{
    zero = 0;

    // Configuring I/Os
    pinMode(sck_pin, OUTPUT);
    digitalWrite(sck_pin, LOW);
    pinMode(dt_pin, INPUT_PULLDOWN);
}

bool HX711::dataAvailable()
{
    return digitalRead(dt_pin)==LOW;
}

int HX711::readBit()
{
    unsigned int result = 0;
    digitalWrite(sck_pin, HIGH);
    delay_us(1);
    result = digitalRead(dt_pin);
    digitalWrite(sck_pin, LOW);
    delay_us(1);

    return result;
}

int HX711::read()
{
    while (!dataAvailable()) {
        delay_us(1);
    }
    unsigned int result = 0;

    for (unsigned int k=0; k<24; k++) {
        result = (result<<1);
        result |= readBit();
    }
    readBit();

    return VALUE_SIGN(result, 24);
}

void HX711::tare()
{
    zero = 0;
    tarePos = 0;
    taring = true;
}

int HX711::sample(int samples)
{
    int result = 0;
    for (int i=0; i<samples; i++) {
        result += read()-zero;
    }
    return result/samples;
}

int compare (const void * a, const void * b)
{
    float fa = *(const float*) a;
    float fb = *(const float*) b;
    return (fa > fb) - (fa < fb);
}

void HX711::tick(bool force)
{
    if (taring) {
        if (dataAvailable()) {
            float value = read();
            tareSamples[tarePos++] = value;
            if (tarePos >= TARE_SAMPLES) {
                qsort(tareSamples, TARE_SAMPLES, sizeof(float), compare);
                zero = tareSamples[TARE_SAMPLES/2];
                taring = false;
            }
        }
    } else {
        if (dataAvailable() || force) {
            int newValue = sample();
            int delta = newValue-value;
            if (delta < 0) delta = -delta;

            if (delta > ANTI_GLITCH) {
                if (antiGlitch) {
                    value = newValue;
                }
                antiGlitch = true;
            } else {
                antiGlitch = false;
                value = newValue;
            }
        }
    }
}
