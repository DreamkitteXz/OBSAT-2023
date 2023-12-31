
#include "http_client.h"

#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

#define TAG "HTTP"

extern float cTemp;

esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}

void http_request()
{
   esp_http_client_config_t config_post = {
        .url = "https://obsat.org.br/teste_post/envio.php",
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = _http_event_handle};
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char  *post_data = "{\"equipe\":76,\"bateria\":82,\"temperatura\":%f,\"pressao\":0.9,\"giroscopio\":[35,78,0],\"acelerometro\":[84,24,65],\"payload\":{\"contador\":4,\"tensao\":65\"temp\":24}}", ler_temp_ds18b20();
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}