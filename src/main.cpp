#include "PushButton.h"
#include "avr/sleep.h"

struct Interface
{
    uint8_t nHeadlightDnPin{};
    uint8_t nHeadlightUpPin{};
    PushButton winkBtn{};

    static constexpr auto s_maxWinkLen{ 1500UL };
    static constexpr auto s_halfWinkLen { s_maxWinkLen / 2 };
};

Interface interface { PD6, PD7, PushButton{PD2} };

void winkBtnHandler();

void setup()
{
    // delay for allowing program startup
    delay(1);

    attachInterrupt(digitalPinToInterrupt(interface.winkBtn.getPinNumber()), winkBtnHandler, RISING);

    pinMode(interface.nHeadlightUpPin, OUTPUT);
    pinMode(interface.nHeadlightDnPin, OUTPUT);
    digitalWrite(interface.nHeadlightUpPin, LOW);
    digitalWrite(interface.nHeadlightDnPin, LOW);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

[[maybe_unused]] void loop()
{
    static auto winkStartTime { 0UL };

    // only accept 'wink' input if there is no 'wink start' timestamp
    if (winkStartTime)
    {
        auto timeNow{ millis() - winkStartTime };

        if (timeNow >= Interface::s_halfWinkLen)
        {
            // begin closing wink
            if (timeNow < Interface::s_maxWinkLen)
            {
                if (digitalRead(interface.nHeadlightDnPin))
                {
                    digitalWrite(interface.nHeadlightDnPin, LOW);
                    digitalWrite(interface.nHeadlightUpPin, HIGH);
                }
            }
                // full wink duration expired, reset timer start timestamp and end wink
            else
            {
                digitalWrite(LED_BUILTIN, LOW);
                digitalWrite(interface.nHeadlightUpPin, LOW);
                winkStartTime = 0;
            }
        }

        // board should not sleep while winking
        return;
    }

    if (interface.winkBtn.verifyExistingInput())
    {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(interface.nHeadlightDnPin, HIGH);
        winkStartTime = millis();

        return;
    }

    // no remaining input, board should sleep
    if (!interface.winkBtn.hasInput())
        sleep_mode();
}

void winkBtnHandler()
{
    interface.winkBtn.resetDebounceTimer();
}