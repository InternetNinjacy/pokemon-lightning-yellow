#ifndef GUARD_DYNAMIC_WEATHER_H
#define GUARD_DYNAMIC_WEATHER_H
#include "global.h"
u8 DynamicWeatherForMap(u8 originalWeather);
void DynamicWeatherUpdate(void);
void DynamicWeatherAfterBattle(void);
u8 DynamicWeatherForBattle(void);
#endif
