#include "displayd_i2c.h"

#include "globals.h" 

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>

#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

#include "messages.h"

// SDA - PIN
#define PIN_SDA GPIO_NUM_21

// SCL - PIN
#define PIN_SCL GPIO_NUM_22

// I2C - Adresse
#define I2C_ADRESS 0x78

static const char *TAG = "DISPLAYD_SSD1306_I2C";

void displayd_i2c(void *pvParameters) {
   // init i2c
   u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
   u8g2_esp32_hal.sda  = PIN_SDA;
   u8g2_esp32_hal.scl  = PIN_SCL;
   u8g2_esp32_hal_init(u8g2_esp32_hal);

   u8g2_t u8g2; // a structure which will contain all the data for one display
   // u8g2_Setup_ssd1306_i2c_128x32_univision_f  --> see u8g2/csrc/u8g2.h for other displays
   u8g2_Setup_ssd1306_i2c_128x64_noname_f(
	&u8g2,
	U8G2_R0,
	//u8x8_byte_sw_i2c,
	u8g2_esp32_i2c_byte_cb,
	u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
   u8x8_SetI2CAddress(&u8g2.u8x8,I2C_ADRESS);

   ESP_LOGI(TAG, "u8g2_InitDisplay");
   u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

   ESP_LOGI(TAG, "u8g2_SetPowerSave");
   u8g2_SetPowerSave(&u8g2, 0); // wake up display

   char buffer[10];
 
   // Daemonbetrieb
   while(1)
    {

	//ESP_LOGI(TAG, "u8g2_ClearBuffer");
	  u8g2_ClearBuffer(&u8g2);
          u8g2_SetFont(&u8g2, u8g2_font_8x13_tr); // more fonts in /u8g2/tools/font/build/single_font_files
   
          if (global_agc_mode == AGC_ON) {
              u8g2_DrawStr(&u8g2, 1,13,"[AGC ON]\0");
	      sprintf (buffer, "%f", global_agc_gain);	
              u8g2_DrawStr(&u8g2, 1,26,buffer);
	    }
          else
	    u8g2_DrawStr(&u8g2, 1,13,"[AGC OFF]\0");

          switch(global_current_demodulator) {
             case MODE_AM:  u8g2_DrawStr(&u8g2, 90,13,"[AM]\0"); 	break;
             case MODE_USB: u8g2_DrawStr(&u8g2, 90,13,"[USB]\0");	break;
             case MODE_LSB: u8g2_DrawStr(&u8g2, 90,13,"[LSB]\0");	break;
             case MODE_CW:  u8g2_DrawStr(&u8g2, 90,13,"[CW]\0");	break;
	  }  

         switch(global_current_filter_bw) {
             case LPF_500:  u8g2_DrawStr(&u8g2, 70,26,"600 Hz\0"); 	break;
             case LPF_3200: u8g2_DrawStr(&u8g2, 70,26,"3200 Hz\0");	break;
             case LPF_4500: u8g2_DrawStr(&u8g2, 70,26,"4500 Hz\0");	break;
             case LPF_6000: u8g2_DrawStr(&u8g2, 70,26,"6000 Hz\0");	break;
	  } 
      
          for (int i=0; i<128; i+=1){
            if (global_cb_fft_spectrum[i] < 32)
               u8g2_DrawLine(&u8g2,i,64,i,64-global_cb_fft_spectrum[i]);
            else 
	        u8g2_DrawLine(&u8g2,i,64,i,64-32);
	  }


	  u8g2_SendBuffer(&u8g2);
  
       // ESP_LOGI(TAG, "Heartbeat");
       // vTaskDelay(1/portTICK_PERIOD_MS ); 
    }

	
	vTaskDelete(NULL);
}

