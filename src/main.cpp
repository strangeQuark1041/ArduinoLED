#include <FastLED.h>
#include <Arduino.h>
#define LED_PIN 7
#define NUM_LEDS 300

CRGB leds[NUM_LEDS];

struct Rgb
{
    double r;
    double g;
    double b;

    Rgb(double red = 0.0, double green = 0.0, double blue = 0.0)
    {
        this->r = red;   // [0, 255]
        this->g = green; // [0, 255]
        this->b = blue;  // [0, 255]
    }
};

struct Hsv
{
    Hsv(double hue = 0.0, double sat = 0.0, double val = 0.0)
    {
        this->h = hue;
        this->s = sat;
        this->v = val;
    }
    double h; // [0, 360]
    double s; // [0, 100]
    double v; // [0, 100]
};

Rgb to_rgb(Hsv color)
{
    double s = color.s / 100;
    double v = color.v / 100;
    double C = s * v;
    double X = C * (1 - abs(fmod(color.h / 60.0, 2) - 1));
    double m = v - C;
    double r, g, b;
    if (color.h >= 0 && color.h < 60) r = C, g = X, b = 0;
    else if (color.h >= 60 && color.h < 120)
        r = X, g = C, b = 0;
    else if (color.h >= 120 && color.h < 180)
        r = 0, g = C, b = X;
    else if (color.h >= 180 && color.h < 240)
        r = 0, g = X, b = C;
    else if (color.h >= 240 && color.h < 300)
        r = X, g = 0, b = C;
    else
        r = C, g = 0, b = X;

    return Rgb((r + m) * 255, (g + m) * 255, (b + m) * 255);
}

void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.show();
}

template <typename T>
T maprange(T value, T from_min, T from_max, T to_min, T to_max)
{
    return from_min +
           (value - from_min) * (to_max - to_min) / (from_max - from_min);
}







double offset       = 0;
unsigned int period = 30;

void loop()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        Rgb col_rgb = to_rgb(Hsv(maprange(fmod((i + offset), period), 0.0,
                                          double(period - 1), 0.0, 360.0),
                                 100.0, 100.0));
        leds[i]     = CRGB(static_cast<uint8_t>(col_rgb.g),
                       static_cast<uint8_t>(col_rgb.r),
                       static_cast<uint8_t>(col_rgb.b));
    }
    FastLED.show();
    offset += 4;
}
