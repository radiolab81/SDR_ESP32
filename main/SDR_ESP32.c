#include "globals.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "board.h"

#include "modules/messages.h"

#include "modules/gpiod.h"
#include "modules/gpiod_rotary.h"

#include "modules/display/ssd1306OLED/displayd_i2c.h"

#include "modules/iqreader.h"

#if __has_include("esp_idf_version.h")
#include "esp_idf_version.h"
#else
#define ESP_IDF_VERSION_VAL(major, minor, patch) 1
#endif

#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 1, 0))
#include "esp_netif.h"
#else
#include "tcpip_adapter.h"
#endif

static const char *TAG = "SDR_ESP32";


void app_main(void)
{
   esp_log_level_set(TAG, ESP_LOG_INFO);
 
   // BOARDINITIALISIERUNGEN
   ESP_LOGI(TAG, "call startup procedures (aka rc.local)");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
#if (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 1, 0))
    ESP_ERROR_CHECK(esp_netif_init());
#else
    tcpip_adapter_init();
#endif
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);

    // Queues für Nachrichtenaustausch zwischen den Prozessen
    ESP_LOGI(TAG, "start IPC");
   
    // - displayd queue 
    /*xDisplaydQueue = xQueueCreate( 10, sizeof( struct ADisplaydMessage * ) );
    if (!xDisplaydQueue)
      ESP_LOGE(TAG, "IPC:Displayd Message Queue failed");*/
      
    // Prozess gpiod starten
    TaskHandle_t xgpiodHandle = NULL;
    xTaskCreate( gpiod, "gpiod", 2048, NULL , tskIDLE_PRIORITY, &xgpiodHandle );
    configASSERT(xgpiodHandle);

    // Prozess gpiod_rotary starten
    // TaskHandle_t xgpiodHandle = NULL;
    // xTaskCreate( gpiod_rotary, "gpiod", 2048, NULL , tskIDLE_PRIORITY, &xgpiodHandle );
    // configASSERT(xgpiodHandle);
 
    // Prozess displayd starten
    TaskHandle_t xdisplaydHandle = NULL;
    xTaskCreate( displayd_i2c, "displayd", 4096, NULL , tskIDLE_PRIORITY, &xdisplaydHandle );
    configASSERT(xdisplaydHandle);

    // Prozess iqreader starten
    TaskHandle_t xiqreaderHandle = NULL;
    xTaskCreate( iqreader, "iqreader", 4096, NULL , tskIDLE_PRIORITY, &xiqreaderHandle );
    configASSERT(xiqreaderHandle);

    // Daemonbetrieb - Hauptprozess
    while(1)
    {

      ESP_LOGI(TAG, "Heartbeat");
      vTaskDelay(1000/portTICK_PERIOD_MS );
         
    }

   
    ESP_LOGI(TAG, "cleanup");

  //  if(xPlayerControlTaskHandle)
    //  vTaskDelete(xPlayerControlTaskHandle);
  
    if(xgpiodHandle)
      vTaskDelete(xgpiodHandle);

    if(xdisplaydHandle)
      vTaskDelete(xdisplaydHandle);


    if(xiqreaderHandle)
      vTaskDelete(xiqreaderHandle);

    esp_periph_set_destroy(set);
}
