#include "buttons.h"

Button2 ButMenu = Button2(CONF_BUTTON_MENU_PIN, INPUT_PULLUP, false, true);


void btnHandler(Button2& btn) {
	OLEDDisplayUiState* state = ui.getUiState();

	if (state->currentFrame < 4)
		ui.nextFrame();
	else
		ui.transitionToFrame(0);
}

void btnLongHandler(Button2& btn) {
	OLEDDisplayUiState* state = ui.getUiState();
	
	switch (state->currentFrame) {
		case 3:
			if (hw_custom_data.light_on == 1) {
				hw_custom_data.light_on = 0;
			} else {
				hw_custom_data.light_on = 1;
			}
			send_hw_custom_data();
			send_app_custom_data();
			break;
	}
}

static void buttons_task(void *arg) 
{ 
    printf("Startinge Buttons Thread\n");
    for(;;) {
        ButMenu.loop();
        vTaskDelay(50);
	}

}

void buttons_init( void )
{
    ButMenu.setClickHandler(btnHandler);
	ButMenu.setLongClickDetectedHandler(btnLongHandler);
	ButMenu.setLongClickTime(600);
    xTaskCreatePinnedToCore(buttons_task, "buttons_task", 3072, NULL, 8, NULL, tskNO_AFFINITY);
}