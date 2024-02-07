#ifndef CARWINKNANO_PUSHBUTTON_H
#define CARWINKNANO_PUSHBUTTON_H


#include <Arduino.h>

class PushButton
{
public:
    static constexpr auto s_debounceDelay { 20UL };

    PushButton() = default;

    explicit PushButton(uint8_t pinNumber)
            : m_pinNumber { pinNumber }
    {}

    [[nodiscard]] uint8_t getPinNumber() const { return m_pinNumber; }
    void resetDebounceTimer() { m_lastUpdate = millis(); }
    bool hasInput() const { return m_lastUpdate; }
    bool verifyExistingInput();

private:
    uint8_t m_pinNumber {};
    volatile unsigned long m_lastUpdate {};

};


#endif