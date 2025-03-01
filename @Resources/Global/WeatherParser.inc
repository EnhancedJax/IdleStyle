[Variables]
WeatherUnitsMetric=m
WeatherUnitsImperial=e

Substitute="^null$":"", '^"(.*)"$':"\1", "^\\1$":""
SubstituteIcons="^0$":"storm", "^1$":"storm", "^2$":"storm", "^3$":"storm", "^4$":"storm", "^5$":"sleet", "^6$":"sleet", "^7$":"sleet", "^8$":"sleet", "^9$":"rain", "^10$":"sleet", "^11$":"rain", "^12$":"rain", "^13$":"snow", "^14$":"snow", "^15$":"snow", "^16$":"snow", "^17$":"sleet", "^18$":"sleet", "^19$":"fog", "^20$":"fog", "^21$":"fog", "^22$":"fog", "^23$":"windy", "^24$":"windy", "^25$":"snow", "^26$":"cloudy", "^27$":"cloudy-night", "^28$":"cloudy-day", "^29$":"cloudy-night", "^30$":"cloudy-day", "^31$":"clear-night", "^32$":"clear-day", "^33$":"partly-cloudy-night", "^34$":"partly-cloudy-day", "^35$":"sleet", "^36$":"clear-day", "^37$":"storm-day", "^38$":"storm-day", "^39$":"rain", "^40$":"rain", "^41$":"snow-day", "^42$":"snow", "^43$":"snow", "^44$":"unknown", "^45$":"rain-night", "^46$":"snow-night", "^47$":"storm-night", "^na$":"", "":"unknown"

; ================================================

[mUnitsTemperature]
Measure=String

String=[#WeatherUnits[#WeatherUnits]]

RegExpSubstitute=1
Substitute="^m$":"C", "^e$":"F"

UpdateDivider=-1

[mUnitsSpeed]
Measure=String

String=[#WeatherUnits[#WeatherUnits]]

RegExpSubstitute=1
Substitute="^m$":"km/h", "^e$":"mi/h"

UpdateDivider=-1

[mUnitsDistance]
Measure=String

String=[#WeatherUnits[#WeatherUnits]]

RegExpSubstitute=1
Substitute="^m$":"km", "^e$":"mi"

UpdateDivider=-1

[mUnitsPressure]
Measure=String

String=[#WeatherUnits[#WeatherUnits]]

RegExpSubstitute=1
Substitute="^m$":"mb", "^e$":"in"

UpdateDivider=-1

; ================================================

[mInternetConnectivity]
Measure=Plugin
Plugin=SysInfo

SysInfoType=INTERNET_CONNECTIVITY

OnChangeAction=[!CommandMeasure mLocationParent Update]

; ================================================

[mLocationParent]
Measure=WebParser

URL=https://nominatim.openstreetmap.org/search?q=#WeatherLocation#&format=geocodejson&limit=1&addressdetails=1&accept-language=#Language#
RegExp=(?siU)"features":\[{"type":"Feature","properties":{"geocoding":{.*,"label":"(.*)",.*,"admin":{.*?"level\d+":"(.*)"}}},"geometry":{"type":"Point","coordinates":\[(.*),(.*)\]}}\]
UserAgent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36
UpdateRate=(60*15)

FinishAction=[!UpdateMeasureGroup Location] [!EnableMeasureGroup Parent] [!UpdateMeasureGroup Parent] [!Log "Location found: [mLocation]" Notice]
OnConnectErrorAction=[!UpdateMeasureGroup Location] [!EnableMeasureGroup Parent] [!UpdateMeasureGroup Parent] [!Log "Unable to connect to Nominatim." Error]
OnRegExpErrorAction=[!UpdateMeasureGroup Location] [!EnableMeasureGroup Parent] [!UpdateMeasureGroup Parent] [!Log "No results found for that location." Error]

; ================================================

[mLocation]
Group=Location
Measure=WebParser

URL=[mLocationParent]
StringIndex=1

[mLocationCity]
Group=Location
Measure=WebParser

URL=[mLocationParent]
StringIndex=2

Substitute="":"Earth"

[mLocationLat]
Group=Location
Measure=WebParser

URL=[mLocationParent]
StringIndex=4

Substitute="":"None"

[mLocationLon]
Group=Location
Measure=WebParser

URL=[mLocationParent]
StringIndex=3

Substitute="":"None"

; ================================================

[mCurrentParent]
Group=Parent
Measure=WebParser

URL=https://api.weather.com/v3/wx/observations/current?geocode=[&mLocationLat],[&mLocationLon]&units=[#WeatherUnits[#WeatherUnits]]&language=#Language#&format=json&apiKey=d522aa97197fd864d36b418f39ebb323
RegExp=(?siU){"cloudCeiling":.*,"cloudCoverPhrase":.*,"dayOfWeek":.*,"dayOrNight":.*,"expirationTimeUtc":.*,"iconCode":(.*),"iconCodeExtend":.*,"obsQualifierCode":.*,"obsQualifierSeverity":.*,"precip1Hour":.*,"precip6Hour":.*,"precip24Hour":.*,"pressureAltimeter":(.*),"pressureChange":.*,"pressureMeanSeaLevel":.*,"pressureTendencyCode":.*,"pressureTendencyTrend":.*,"relativeHumidity":(.*),"snow1Hour":.*,"snow6Hour":.*,"snow24Hour":.*,"sunriseTimeLocal":.*,"sunriseTimeUtc":.*,"sunsetTimeLocal":.*,"sunsetTimeUtc":.*,"temperature":(.*),"temperatureChange24Hour":.*,"temperatureDewPoint":.*,"temperatureFeelsLike":.*,"temperatureHeatIndex":.*,"temperatureMax24Hour":.*,"temperatureMaxSince7Am":.*,"temperatureMin24Hour":.*,"temperatureWindChill":.*,"uvDescription":(.*),"uvIndex":(.*),"validTimeLocal":.*,"validTimeUtc":.*,"visibility":(.*),"windDirection":(.*),"windDirectionCardinal":(.*),"windGust":.*,"windSpeed":(.*),"wxPhraseLong":(.*),"wxPhraseMedium":.*,"wxPhraseShort":.*}
UserAgent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36
UpdateRate=1

FinishAction=[!EnableMeasureGroup Current] [!UpdateMeasureGroup Current] [!UpdateMeter *] [!UpdateMeter *] [!Redraw] [!Log "Finished parsing current weather data." Notice]
OnConnectErrorAction=[!EnableMeasureGroup Current] [!UpdateMeasureGroup Current] [!UpdateMeter *] [!UpdateMeter *] [!Redraw] [!Log "Unable to connect to Weather.com." Error]
OnRegExpErrorAction=[!EnableMeasureGroup Current] [!UpdateMeasureGroup Current] [!UpdateMeter *] [!UpdateMeter *] [!Redraw] [!Log "Something went wrong." Error]

DynamicVariables=1
UpdateDivider=-1
Disabled=1

; ================================================

[mCurrentIcon]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=1

RegExpSubstitute=1
Substitute="#Substitute#", "#SubstituteIcons#"

[mCurrentPressure]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=2

RegExpSubstitute=1
Substitute="#Substitute#", "":"--"

[mCurrentHumidity]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=3

RegExpSubstitute=1
Substitute="#Substitute#", "(.+)":"\1%", "":"--"

[mCurrentTemperature]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=4

RegExpSubstitute=1
Substitute="#Substitute#", "(.+)":"\1°", "":"--"

[mCurrentUVDescription]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=5

RegExpSubstitute=1
Substitute="#Substitute#", "":"--"

[mCurrentUVIndex]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=6

RegExpSubstitute=1
Substitute="#Substitute#", "":"--"

[mCurrentVisibility]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=7

RegExpSubstitute=1
Substitute="#Substitute#", "":"--"

[mCurrentWindDirection]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=8

RegExpSubstitute=1
Substitute="#Substitute#", "":"0"

[mCurrentWindDirectionIcon]
Group=Current
Measure=Calc

Formula=(Round([mCurrentWindDirection]/15)*15)%360

Disabled=1
UpdateDivider=-1
DynamicVariables=1

RegExpSubstitute=1
Substitute="(.*)":"from-\1"

[mCurrentWindDirectionCardinal]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=9

RegExpSubstitute=1
Substitute="#Substitute#", "":"--"

[mCurrentWindSpeed]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=10

RegExpSubstitute=1
Substitute="#Substitute#", "":"--"

[mCurrentPhrase]
Group=Current
Measure=WebParser

URL=[mCurrentParent]
StringIndex=11

RegExpSubstitute=1
Substitute="#Substitute#", "":"N/A"
