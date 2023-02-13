#include "gpiod_rotary.h"

#include "board.h"

#include "messages.h"
#include "esp_log.h"

#define RO_A_PIN  GPIO_NUM_5
#define RO_B_PIN  GPIO_NUM_18

static const char *TAG = "GPIOD_ROTARY";

unsigned char flag;
unsigned char Last_RoB_Status;
unsigned char Current_RoB_Status;

struct AMessage *pxMessage;

void rotaryDeal(void)
{
	Last_RoB_Status = gpio_get_level(RO_B_PIN);

	while(!gpio_get_level(RO_A_PIN)){
		Current_RoB_Status = gpio_get_level(RO_B_PIN);
		flag = 1;
	}

	if(flag == 1){
		flag = 0;
		if((Last_RoB_Status == 0)&&(Current_RoB_Status == 1)){
			//system("echo \"next\" | nc 127.0.0.1 9294 -N");
 			ESP_LOGI(TAG, "prog+");
			//xMessage.ucMessage = NEXT_PRG;
	                //pxMessage = &xMessage; 
	                //xQueueSend( xPlayerQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
		}
		if((Last_RoB_Status == 1)&&(Current_RoB_Status == 0)){
			//system("echo \"prev\" | nc 127.0.0.1 9294 -N");
			ESP_LOGI(TAG, "prog-");
			//xMessage.ucMessage = PREV_PRG;
	                //pxMessage = &xMessage; 
	                //xQueueSend( xPlayerQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
		}

	}
}


void gpiod_rotary(void *pvParameters) {

  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = ( BIT(RO_A_PIN) | BIT(RO_B_PIN) );
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);


  // Daemonbetrieb
  while (1) {
      rotaryDeal(); 
      vTaskDelay(10/portTICK_PERIOD_MS );
   }


}
