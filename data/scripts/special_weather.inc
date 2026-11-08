@ Caller has completed the approved aftermath;8004=event,8005=battle outcome.
@ No new dialogue/placement. Keep field controls until actual weather is ready.
SpecialWeather_EventScript_FinishAftermath::
	callnative SpecialWeatherResolveScript
	goto_if_eq VAR_RESULT, FALSE, SpecialWeather_EventScript_Return
	@ Native wait command; no repeated resolution/RNG while rendering progresses.
	.byte 0x24
	.4byte SpecialWeatherRestorationReady
SpecialWeather_EventScript_Return::
	return
