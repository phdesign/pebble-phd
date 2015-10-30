#include "pebble.h"

bool (*mocked_clock_is_24h_style)();

void mock_clock_is_24h_style(bool (*mock)()) {
    mocked_clock_is_24h_style = mock;
}

bool clock_is_24h_style(void) {
    return mocked_clock_is_24h_style();
}
