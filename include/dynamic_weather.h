#ifndef GUARD_DYNAMIC_WEATHER_H
#define GUARD_DYNAMIC_WEATHER_H
#include "global.h"
u8 DynamicWeatherForMap(u8 originalWeather);
void DynamicWeatherUpdate(void);
void DynamicWeatherAfterBattle(void);
u8 DynamicWeatherForBattle(void);
// Activate only when the corresponding approved encounter has been installed.
bool8 SpecialWeatherActivate(u8 event);
// Call only AFTER approved aftermath; only WON/CAUGHT can commit resolution.
bool8 SpecialWeatherFinishAftermath(u8 event, u8 outcome);
bool8 SpecialWeatherRestorationReady(void);
// Script callnative:8004=event,8005=outcome; on success wait natively on Ready.
void SpecialWeatherActivateScript(void);
void SpecialWeatherResolveScript(void);
#endif
