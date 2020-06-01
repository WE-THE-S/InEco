#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <Wire.h>
#include <cstdint>
#include "../../config.hpp"
#include "../../struct/packet.hpp"
#include "../../struct/handler/gpio_handler.hpp"
#include <NeoPixelBus.h>

using namespace std;

class LedHanlder : public MessageGpioControlHandler {
    protected:
        NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod>* strip = nullptr;
        
    public:
    LedHanlder(gpio_num_t _led) : MessageGpioControlHandler(_led) {

    }

    LedHanlder() : LedHanlder(LED_CONTROL_DEFAULT_PIN) {

    }

    void onMessage(const device_communication_message_t message){
        if(strip == nullptr){
            strip = new NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod>(LED_COUNT, this->pin);
            strip->Begin();
        }
        if(message.type == MESSAGE_TYPE::SET_COLOR){
            led_message_t led;
            led.message = message.message;
            RgbColor color(led.color.r, led.color.g, led.color.b);
            strip->ClearTo(color, 0, LED_COUNT);
            strip->Show();
        }
    }
    ~LedHanlder(){
        delete this->strip;
    }
};

#endif