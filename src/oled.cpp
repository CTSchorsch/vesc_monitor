#include "main.h"
#include "oled.h"
#include "images.h"


SSD1306Wire *display;
OLEDDisplayUi ui(NULL);

/******************** OVERLAYS *************************/
void batOverlay(OLEDDisplay* display, OLEDDisplayUiState* state) {
	float batLevel;

	batLevel = ((vescData.inpVoltage - CONF_OLED_BAT_EMPTY) / (CONF_OLED_BAT_FULL - CONF_OLED_BAT_EMPTY)) * 100;
	if (batLevel < 0) batLevel = 0;

	if (batLevel < 15) {
		display->drawXbm(108, 0, BAT_width, BAT_height, BAT_EMPTY_bits);
	} else if (batLevel < 40) {
		display->drawXbm(108, 0, BAT_width, BAT_height, BAT_1_3_bits);
	} else if (batLevel < 70) {
		display->drawXbm(108, 0, BAT_width, BAT_height, BAT_2_3_bits);
	} else {
		display->drawXbm(108, 0, BAT_width, BAT_height, BAT_bits);
	}
}

void nameOverlay(OLEDDisplay* display, OLEDDisplayUiState* state) 
{
	display->setFont(ArialMT_Plain_10);
	display->setTextAlignment(TEXT_ALIGN_LEFT);
	if (vescData.id == ID_GEORG)
		display->drawString(0, 0, "GScooter - Georg");
	else if (vescData.id == ID_THOMAS) 
		display->drawString(0,0, "GScooter - Thomas");
	else 
		display->drawString(0,0, "GScooter");
}

OverlayCallback overlays[] = { batOverlay, nameOverlay };
int overlayCount = 2;


/************************** FRAMES **************************/
void drawFrame1(OLEDDisplay* display, OLEDDisplayUiState* state, int16_t x,	int16_t y) 
{
	int rpm = vescData.rpm / 14;
	display->flipScreenVertically();
	display->setFont(ArialMT_Plain_24);
	display->setTextAlignment(TEXT_ALIGN_RIGHT);
	if (rpm < 0) rpm = 0;
	display->drawString(x + 80, y + 20, (String)rpm);
	display->setFont(ArialMT_Plain_16);
	display->drawString(x + 125, y + 30, "rpm");
	display->setFont(ArialMT_Plain_16);
    display->drawProgressBar(x + 5, y + 52, 115, 6,  abs(vescData.dutyCycleNow)*100);

}

void drawFrame2(OLEDDisplay* display, OLEDDisplayUiState* state, int16_t x,	int16_t y) 
{
	display->setTextAlignment(TEXT_ALIGN_LEFT);
	display->setFont(ArialMT_Plain_10);
	display->drawString(x, y + 16, "Tpcb");
	display->drawString(x, y + 32, "Tmot");
	display->drawString(x, y + 48, "Tamb");
	display->setFont(ArialMT_Plain_16);
	display->drawString(x + 30, y + 10, (String)vescData.tempMosfet + " °C");
	display->drawString(x + 30, y + 26, (String)vescData.tempMotor + " °C");
	display->drawString(x + 30, y + 42, (String)vescData.tempMisc1 + " °C");
}

void drawFrame3(OLEDDisplay* display, OLEDDisplayUiState* state, int16_t x,
	int16_t y) {
	char buff[10];

	display->setTextAlignment(TEXT_ALIGN_LEFT);
	display->setFont(ArialMT_Plain_10);
	display->drawString(x, y + 16, "Ibat");
	display->drawString(x, y + 32, "Imot");
	display->drawString(x, y + 48, "Pwr");
	sprintf(buff, "%.0f W", vescData.inpVoltage * vescData.avgInputCurrent);
	display->setFont(ArialMT_Plain_16);
	display->drawString(x + 30, y + 10,
		(String)vescData.avgInputCurrent + " A");
	display->drawString(x + 30, y + 26,
		(String)vescData.avgMotorCurrent + " A");
	display->drawString(x + 30, y + 42, (String)buff);
}


void drawFrame4(OLEDDisplay* display, OLEDDisplayUiState* state, int16_t x,	int16_t y) 
{
	char buff[128];
	// TODO Hier nur Licht an aus
	display->setTextAlignment(TEXT_ALIGN_CENTER);
	display->setFont(ArialMT_Plain_16);
	sprintf(buff, "Licht %s",(hw_custom_data.light_on == 0 ? "AN" : "AUS"));
	display->drawString(x+64, y + 20, buff);
	display->drawString(x+64, y + 42, (String)vescData.tempLED + " °C");
}


FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, drawFrame4 };
int frameCount = 4;


static void oled_task(void *arg) 
{ 
    printf("Startinge Display Thread\n");
    for(;;) {
        int remainingTimeBudget = ui.update();
	    if (remainingTimeBudget > 0) {
            //do some stuff here
		}
		delay(remainingTimeBudget);
	}

}


void oled_init( void ) 
{
    display = new SSD1306Wire(0x3c, SDA_OLED, SCL_OLED, RST_OLED, GEOMETRY_128_64);

    ui.setDisplay(display);
    ui.setTargetFPS(10);
	ui.setFrames(frames, frameCount);
	ui.setOverlays(overlays, overlayCount);
	ui.disableIndicator();
	ui.disableAutoTransition();
	ui.disableAllIndicators();
	ui.init();

    xTaskCreatePinnedToCore(oled_task, "oled_task", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}