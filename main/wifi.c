#include "wifi.h"
#include "esp_wifi.h"

#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <string.h>
#include "debug.h"

#define RCONN_TRY_MAX                   5

uint8_t reconnect_try = 0;



wifi_callback_t __callback_connection = NULL;
wifi_callback_t __callback_disconnection = NULL;



 void callback_connect(){
    DEBUG_PRINT("callback connexion\n");
}

 void callback_disconnect(){   
     DEBUG_PRINT("callback desconexion\n");

}




static void event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    // WiFi esta listo para conectarse a una red en modo STATION.
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        printf("WiFi modo station listo para conectarse a red\n");
        esp_wifi_connect();
    }else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        if(reconnect_try < RCONN_TRY_MAX){
            printf("Wifi desconectado, intentando reconexion:%u / %u\n",reconnect_try,RCONN_TRY_MAX);
            reconnect_try ++;
            esp_wifi_connect();
        }else{
            printf("Fallaron los intentos de reconexion\n");
        }
    }
   
    if (event_id == WIFI_EVENT_STA_CONNECTED){
    printf("WiFi conectado\n");
    reconnect_try = 0;
    __callback_connection();
    }
     if (event_id == WIFI_EVENT_STA_DISCONNECTED){
    printf("WiFi desconectado\n");
    __callback_disconnection();
    }

}



esp_err_t config_nvs_pre_connection(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(err = nvs_flash_erase());
      err = nvs_flash_init();
    }
    return err;
}



 esp_err_t wifi_connect(const char* WIFI_ID, const char * PASS,
                        wifi_callback_t cb_conn , 
                        wifi_callback_t cb_disconn ){
    
    // Asigno los callback recibidos por parámetro o los por defecto
    __callback_connection = (cb_conn == NULL)?callback_connect: cb_conn;
    __callback_disconnection = (cb_disconn == NULL)? callback_disconnect : cb_disconn;

    esp_err_t err = ESP_OK;
    ESP_ERROR_CHECK(err = config_nvs_pre_connection());
    ESP_ERROR_CHECK(err = esp_netif_init());
    ESP_ERROR_CHECK(err = esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
     // Configuración por defecto.
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    // Esta es la primera funcion que debe llamarse para el manejo de WiFi
    ESP_ERROR_CHECK(err = esp_wifi_init(&config));
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(err = esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));

                                                 
    wifi_config_t wifi_config = {0};
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    strncpy((char*)wifi_config.sta.ssid, (char*)WIFI_ID,strlen(WIFI_ID));
    strncpy((char*)wifi_config.sta.password, (char*)PASS,strlen(PASS));

    ESP_ERROR_CHECK(err = esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(err = esp_wifi_start());

    printf( "wifi_init_softap finished. SSID:%s password:%s \n",
             WIFI_ID, PASS);
    
    return err;
}





esp_err_t wifi_desconnect(void){
    esp_err_t err = ESP_OK;
    err = esp_wifi_stop();
    err = esp_wifi_disconnect();
    err = esp_wifi_deinit();
    return err;
}
