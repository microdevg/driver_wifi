# Prueba de conexión WiFi con ESP32

El driver generado para usar las funciones Wifi soporta el uso de callback de conexión , de desconexión y de fallo en la conexión.
 También puede pasar NULL en esos parámetros para activar el comportamiento por defecto.



```

esp_err_t wifi_connect( const char* WIFI_ID,
                        const char * PASS,
                        wifi_callback_t cb_conn,
                        wifi_callback_t cb_desconn,
                        wifi_callback_t cb_failed);

```