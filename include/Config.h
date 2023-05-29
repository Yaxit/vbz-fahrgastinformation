#ifndef CONFIG_H
#define CONFIG_H

#include "Secret.h"

#define TIME_SERVER "0.ch.pool.ntp.org"
#define AP_NAME SECRET_AP_NAME
#define AP_PASSWORD SECRET_AP_PASSWORD
#define ID_ALTSTETTEN_BANHOF_BUS ("8591056")
#define ID_ALTSTETTEN_BANHOF ("8503001")
#define ID_GRIMSELSTRASSE ("8591165")
#define ID_BELLVUE ("8576193")
#define POLLING_FREQ_SECONDS (30)

// Api
#define OPEN_DATA_API_KEY SECRET_API_KEY
#define OPEN_DATA_URL "https://api.opentransportdata.swiss/trias2020"
#define OPEN_DATA_STATION_1 ID_GRIMSELSTRASSE
#define OPEN_DATA_STATION_2 ID_ALTSTETTEN_BANHOF
#define OPEN_DATA_RESULTS "5"
#define OPEN_DATA_DIRECTION "H" // H = Outward, R = Return, A = All

// Display
// @note R1/G1 and R2/G2 are swapped on the display
#define R1_PIN 26 // TCK
#define G1_PIN 25
#define B1_PIN 27
#define R2_PIN 12
#define G2_PIN 14
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 19
#define C_PIN 5
#define D_PIN 17
#define E_PIN 18 // required for 1/32 scan panels, like 64x64. Any available pin would do, i.e. IO32
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 16
#define PANEL_RES_X 128 // Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 64  // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1   // Total number of panels chained one to another

#endif
