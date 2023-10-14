#ifndef OpenTransportDataSwiss_H
#define OpenTransportDataSwiss_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UnixTime.h>
#include <math.h>

class OpenTransportDataSwiss
{

public:
    OpenTransportDataSwiss(
        String stopPointBPUIC,
        String direction,
        String openDataUrl,
        String apiKey,
        String numResults);

    String numResultsString;
    int lateMinCutoff = 3;
    String stopPointBPUIC;
    String direction;
    String openDataUrl;
    String apiKey;
    String httpLastError;

    StaticJsonDocument<1500> doc;

    int getWebData(const String& formattedDate);
    String FormatTimeStamp(const String& formattedDate, const String& format);
    uint32_t GetTimeToDeparture(const String& apiCallTime, const String& departureTime);
    uint32_t GetEpochTime(const String& dateTimeStamp);
    String getXmlValue(const String& xmlStartElement, const String& xmlEndElement, const String& xmlDocument);

private:
    const char *rootCACertificate;
};

#endif
