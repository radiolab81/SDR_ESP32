#include "gpiod.h"

#include "board.h"
#include "globals.h"

#include "messages.h"
#include "esp_log.h"

#define KEY_DEMODULATOR GPIO_NUM_18
#define KEY_FILTER      GPIO_NUM_5
#define KEY_VOL_UP      GPIO_NUM_23
#define KEY_VOL_DOWN    GPIO_NUM_19
#define KEY_AGC_SWITCH  GPIO_NUM_13

static const char *TAG = "GPIOD";

void gpiod(void *pvParameters) {
  /* ESP_LOGI(TAG, "***** GPIO-DEFS *****");
  ESP_LOGI(TAG, "VOLDOWN-KEY %i",get_input_voldown_id());
  ESP_LOGI(TAG, "VOLUP-KEY %i",get_input_volup_id());
  ESP_LOGI(TAG, "PLAY-KEY %i",get_input_play_id());
  ESP_LOGI(TAG, "SET-KEY %i",get_input_set_id());
  ESP_LOGI(TAG, "MODE-KEY %i",get_input_mode_id());
  ESP_LOGI(TAG, "REC-KEY %i",get_input_rec_id());
  ESP_LOGI(TAG, "HEADPHONE DETECT GPIO %i",get_headphone_detect_gpio());
  ESP_LOGI(TAG, "AUXIN DETECT GPIO %i",get_auxin_detect_gpio());
  ESP_LOGI(TAG, "GREEN LED GPIO %i",get_green_led_gpio());*/

  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = ( BIT(KEY_DEMODULATOR) | BIT(KEY_FILTER) | BIT(KEY_VOL_UP) | BIT(KEY_VOL_DOWN)  | BIT(KEY_AGC_SWITCH) /* | BIT(more_keys) */ );
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);

  // Daemonbetrieb
  while (1) {
    if (!gpio_get_level(KEY_DEMODULATOR)) {
       ESP_LOGI(TAG, "switch demodulator (am/usb/lsb/cw)");
       if (global_current_demodulator < 3)
         global_current_demodulator++;
       else
         global_current_demodulator = 0;
      } // if (!gpio_get_level(KEY_DEMODULATOR)) {

    if (!gpio_get_level(KEY_FILTER)) {
      ESP_LOGI(TAG, "switch next filter");
      if (global_current_filter_bw < 3)
         global_current_filter_bw++;
       else
         global_current_filter_bw = 0;
	    
      } // if (!gpio_get_level(KEY_FILTER)) {

    if (!gpio_get_level(KEY_VOL_UP)) {
      ESP_LOGI(TAG, "volume up");
      if (global_volume_setting <= 98) 
        global_volume_setting=global_volume_setting+2;
   
    } // if (!gpio_get_level(KEY_VOL_UP)) {

    if (!gpio_get_level(KEY_VOL_DOWN)) {
      ESP_LOGI(TAG, "volume down");
      if (global_volume_setting >= 2) 
        global_volume_setting =global_volume_setting-2;
    } // if (!gpio_get_level(KEY_VOL_DOWN)) {
    

    if (!gpio_get_level(KEY_AGC_SWITCH)) {
      ESP_LOGI(TAG, "KEY_AGC_SWITCH pressed");
      if (global_agc_mode==AGC_ON) {
        global_agc_mode = AGC_OFF;
        global_agc_gain = 1.0;
        }
      else
        global_agc_mode=AGC_ON;
    } // if (!gpio_get_level(KEY_2)) {



    vTaskDelay(500/portTICK_PERIOD_MS );
   }

}
