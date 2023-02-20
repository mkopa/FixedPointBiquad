&#x1F50A; Fast Fixed Point Biquad Filter Implementation
============================

* Original code by Nigel Redmon (http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/)

This is an implementation of a Biquad filter implemented as an Arduino library.

Supported filter types:

* Low Pass
* High Pass
* Band Pass
* Notch
* Peaking
* Low Shelf
* High Shelf

Supported data in:

* 16/32 bit fixed point (fast calculation for MCU without Double-precision FPU e.g. ESP32)

----

Methods:

* Constructors

    FixedPointBiquad();
    FixedPointBiquad(int type, float Fc, float Q, float peakGainDB);

* Destructor

    ~FixedPointBiquad();

* Configure the filter

    void setBiquad(int type, float Fc, float Q, float peakGain);

* Same as above but broken into separate parts

    void setType(int type);
    void setQ(float Q);
    void setFc(float Fc);
    void setPeakGain(float peakGainDB);

* Process a sample and return the filtered result

    float process(float in);
    
