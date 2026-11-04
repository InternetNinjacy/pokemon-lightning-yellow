#include "global.h"
#include "dynamic_weather.h"
#include "field_weather.h"
#include "main.h"
#include "overworld.h"
#include "random.h"
#include "time_of_day.h"
#include "constants/maps.h"
#include "constants/weather.h"

struct WeatherPolicy
{
    u8 group, map;
    u8 weights[5]; // Neutral, Rain, Sun, Sand, Snow; relative integer weights.
};
#include "data/dynamic_weather.h"

// Exactly8 previously unused/checksummed save bytes; leave the other8 intact.
// W, version1, last outdoor group/map/weather, indoor flag, signature, XOR.
static u8 *WeatherContext(void)
{
    return gSaveBlock1Ptr->unused_3D24;
}

static const struct WeatherPolicy *FindWeatherPolicy(u8 group, u8 map)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sWeatherPolicies); i++)
        if (sWeatherPolicies[i].group == group && sWeatherPolicies[i].map == map)
            return &sWeatherPolicies[i];
    return NULL;
}

static bool8 WeatherContextValid(void)
{
    u8 i, sum = 0, *p = WeatherContext();
    for (i = 0; i < 8; i++)
        sum ^= p[i];
    return p[0] == 'W' && p[1] == 1 && p[5] <= 1 && p[6] == 0xA7 && sum == 0
        && FindWeatherPolicy(p[2], p[3]) != NULL
        && (p[4] == WEATHER_NONE || p[4] == WEATHER_SUNNY || p[4] == WEATHER_SHADE
         || p[4] == WEATHER_RAIN || p[4] == WEATHER_SNOW || p[4] == WEATHER_SANDSTORM || p[4] == WEATHER_DROUGHT);
}

static void WriteWeatherContext(u8 group, u8 map, u8 weather, bool8 inside)
{
    u8 i, sum = 0, *p = WeatherContext();
    p[0] = 'W'; p[1] = 1; p[2] = group; p[3] = map;
    p[4] = weather; p[5] = inside; p[6] = 0xA7;
    for (i = 0; i < 7; i++)
        sum ^= p[i];
    p[7] = sum;
}

static u16 WeatherRandomBelow(u16 count)
{
    u16 value;
    u32 limit = 0x10000 - 0x10000 % count;
    do { value = Random(); } while (value >= limit);
    return value % count;
}

static u8 NormalizeSun(u8 weather)
{
    return weather == WEATHER_DROUGHT && GetTimeOfDay() == TIME_OF_DAY_NIGHT ? WEATHER_NONE : weather;
}

static u8 RollWeather(const struct WeatherPolicy *policy)
{
    static const u8 weather[] = {WEATHER_NONE, WEATHER_RAIN, WEATHER_DROUGHT, WEATHER_SANDSTORM, WEATHER_SNOW};
    u8 i, total = 0;
    u16 draw;
    for (i = 0; i < 5; i++)
        total += policy->weights[i];
    draw = WeatherRandomBelow(total);
    for (i = 0; i < 5; i++)
    {
        if (draw < policy->weights[i])
            return NormalizeSun(weather[i]);
        draw -= policy->weights[i];
    }
    return WEATHER_NONE; // Unreachable for the frozen positive-total policies.
}

static bool8 IsFixedWeatherGym(void)
{
    return (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_PEWTER_CITY_GYM)
         && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_PEWTER_CITY_GYM))
        || (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_CELADON_CITY_GYM)
         && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_CELADON_CITY_GYM));
}

u8 DynamicWeatherForMap(u8 originalWeather)
{
    u8 group = gSaveBlock1Ptr->location.mapGroup, map = gSaveBlock1Ptr->location.mapNum;
    const struct WeatherPolicy *policy = FindWeatherPolicy(group, map);
    u8 weather, *p = WeatherContext();
    if (policy == NULL)
    {
        // Unassigned outdoor maps retain native headers, never inferred weights.
        if (IsMapTypeOutdoors(gMapHeader.mapType)
         || (group == MAP_GROUP(MAP_SSANNE_EXTERIOR) && map == MAP_NUM(MAP_SSANNE_EXTERIOR)))
            p[0] = 0;
        else if (WeatherContextValid())
            WriteWeatherContext(p[2], p[3], p[5] ? p[4] : GetSav1Weather(), TRUE);
        return originalWeather;
    }
    if (WeatherContextValid() && !p[5] && p[2] == group && p[3] == map)
        weather = NormalizeSun(GetSav1Weather()); // Same-map reload/warp is not a new outdoor boundary.
    else if (WeatherContextValid() && p[5] && p[2] == group && p[3] == map && (Random() & 1))
        weather = NormalizeSun(p[4]);
    else
        weather = RollWeather(policy);
    // Retain existing forest shade only for neutral; no new lighting presets.
    if (weather == WEATHER_NONE && gMapHeader.weather == WEATHER_SHADE)
        weather = WEATHER_SHADE;
    WriteWeatherContext(group, map, weather, FALSE);
    return weather;
}

static void SynchronizeSun(void)
{
    u8 group = gSaveBlock1Ptr->location.mapGroup, map = gSaveBlock1Ptr->location.mapNum;
    u8 *p = WeatherContext();
    bool8 activeSun = FindWeatherPolicy(group, map) != NULL && GetSav1Weather() == WEATHER_DROUGHT;
    bool8 storedSun = WeatherContextValid() && p[4] == WEATHER_DROUGHT;
    if ((activeSun || storedSun) && NormalizeSun(WEATHER_DROUGHT) == WEATHER_NONE)
    {
        if (storedSun)
            WriteWeatherContext(p[2], p[3], WEATHER_NONE, p[5]);
        if (activeSun)
        {
            SetSavedWeather(WEATHER_NONE);
            SetNextWeather(WEATHER_NONE);
            WriteWeatherContext(group, map, WEATHER_NONE, FALSE);
        }
    }
}

void DynamicWeatherUpdate(void)
{
    // Four RTC polls per second while sunlight is active; no periodic rerolls.
    // Battle entry separately refreshes time before mechanical benefits apply.
    if ((gMain.vblankCounter2 & 15) == 0)
    {
        // Old saves can Continue inside a Gym with the former clear weather.
        // Refresh only this map's field value; keep the outdoor context intact.
        if (IsFixedWeatherGym() && GetSav1Weather() != gMapHeader.weather)
        {
            SetSavedWeather(gMapHeader.weather);
            SetNextWeather(gMapHeader.weather);
        }
        SynchronizeSun();
    }
}

u8 DynamicWeatherForBattle(void)
{
    // Approved fixed indoor Gym weather is independent of outdoor RTC policy.
    // Read the logical header so battle entry never races a field fade.
    if (IsFixedWeatherGym())
        return gMapHeader.weather;

    if (FindWeatherPolicy(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum))
    {
        SynchronizeSun();
        return GetSav1Weather(); // Logical weather, not a lagging fade/transition.
    }
    return GetCurrentWeather();
}

void DynamicWeatherAfterBattle(void)
{
    u8 group = gSaveBlock1Ptr->location.mapGroup, map = gSaveBlock1Ptr->location.mapNum;
    if (group == MAP_GROUP(MAP_ROUTE16)
     && (map == MAP_NUM(MAP_ROUTE16) || map == MAP_NUM(MAP_ROUTE17) || map == MAP_NUM(MAP_ROUTE18)))
    {
        u8 weather = RollWeather(FindWeatherPolicy(group, map));
        SetSavedWeather(weather);
        WriteWeatherContext(group, map, weather, FALSE);
    }
    else
        SynchronizeSun();
}
