#ifndef _DRINKERBOT_BALANCE_H
#define _DRINKERBOT_BALANCE_H

#define TARE_SAMPLES        (80*5)
#define DERIVATIVE_SAMPLES  8

class HX711
{
    public:
        int sck_pin, dt_pin;
        int zero;

        HX711(int sck_pin_, int dt_pin_);

        // Low-level reading
        bool dataAvailable();
        void init();
        int readBit();
        int read();

        // Tare the balance
        void tare();

        // Sample and weight
        int sample(int samples = 1);

        // Do the computation
        void tick(bool force=false);

        int value;

        float tareSamples[TARE_SAMPLES];
        bool taring;
        bool antiGlitch;
        int tarePos;
};

#endif
