#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi.h"

#include "debug.h"


#include "wifi_credentials.h"  // lo agregue a .gitignore para no subirlo a internet


#define WIFI_INIT_MESSAGE               "Inicio conexión WiFi\n"
#define WIFI_ID                         WIFI_CREDENTIALS_ID
#define WIFI_PASS                       WIFI_CREDENTIALS_PASS




void wifi_connected(){
    printf("Conectado al WiFi, ahora conectarme a los servicios MQTT\n");
}


void wifi_disconnected(){
    printf("Desconectado del Wifi, liberar recursos y entrar en modo bajo consumo\n");
}
void wifi_failed(){
    printf("La conexión a la red  WiFi fallo\n");
}


void app_main(void)
{
    printf("Inicio\n");
    DEBUG_PRINT(WIFI_INIT_MESSAGE);
    wifi_connect(WIFI_ID,WIFI_PASS,wifi_connected,wifi_disconnected,wifi_failed);
    vTaskDelay(15000/ portTICK_PERIOD_MS);
    while(1){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}