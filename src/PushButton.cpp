#include "PushButton.h"

bool PushButton::verifyExistingInput()
{
    // if button has input (as indicated by a non-zero `m_lastUpdate`) from at least 20ms after last input
    if (m_lastUpdate && (millis() - m_lastUpdate >= s_debounceDelay))
    {
        m_lastUpdate = 0;

        if (digitalRead(m_pinNumber))
            return true;
    }

    return false;
}