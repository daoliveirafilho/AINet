#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_http_client.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048
static const char *TAG = "HTTP_CLIENT";

extern const char cert_pem_start[] asm("_binary_cert_pem_start");
extern const char cert_pem_end[]   asm("_binary_cert_pem_end");

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
static char *output_buffer;
static int output_len;
switch(evt->event_id) {
case HTTP_EVENT_ERROR:
ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
break;
case HTTP_EVENT_ON_CONNECTED:
ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
break;
case HTTP_EVENT_HEADER_SENT:
ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
break;
case HTTP_EVENT_ON_HEADER:
ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
break;
case HTTP_EVENT_ON_DATA:
ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
if (output_len == 0 && evt->user_data)
{
memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
}
if (!esp_http_client_is_chunked_response(evt->client))
{
int copy_len = 0;
if (evt->user_data)
{
copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
if (copy_len)
{
memcpy(evt->user_data + output_len, evt->data, copy_len);
} } else
{
int content_len = esp_http_client_get_content_length(evt->client);
if (output_buffer == NULL)
{
output_buffer = (char *) calloc(content_len + 1, sizeof(char));
output_len = 0;
if (output_buffer == NULL)
{
ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
return ESP_FAIL;
} }
copy_len = MIN(evt->data_len, (content_len - output_len));
if (copy_len)
{
memcpy(output_buffer + output_len, evt->data, copy_len);
} }
output_len += copy_len;
}

break;
case HTTP_EVENT_ON_FINISH:
ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
if (output_buffer != NULL)
{
#if CONFIG_EXAMPLE_ENABLE_RESPONSE_BUFFER_DUMP
ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
#endif
free(output_buffer);
output_buffer = NULL;
}
output_len = 0;
break;
case HTTP_EVENT_DISCONNECTED:
ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
int mbedtls_err = 0;
esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
if (err != 0)
{
ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
}
if (output_buffer != NULL)
{
free(output_buffer);
output_buffer = NULL;
}
output_len = 0;
break;
case HTTP_EVENT_REDIRECT:
ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
esp_http_client_set_header(evt->client, "From", "user@example.com");
esp_http_client_set_header(evt->client, "Accept", "text/html");
esp_http_client_set_redirection(evt->client);
break;
}
return ESP_OK;
}

void https_request(void)
{
char bufferA[256];
const char *apikey = "sb_secret_???????????????????????????????";
snprintf(bufferA, sizeof(bufferA), "Bearer %s", apikey);
printf("\n%s\n", bufferA);
const char *bearer = bufferA;
char bufferB[256];
char mac_string[18];
float humidity = 0.034;
float temperature = 34;
uint8_t mac_bytes[6];
esp_read_mac(mac_bytes, ESP_MAC_WIFI_STA);
snprintf(mac_string, sizeof(mac_string), "%02X:%02X:%02X:%02X:%02X:%02X", mac_bytes[0], mac_bytes[1], mac_bytes[2], mac_bytes[3], mac_bytes[4], mac_bytes[5]);
snprintf(bufferB, sizeof(bufferB), "{\"humidity\":\"%f\",\"temperature\":\"%f\",\"macaddress\":\"%s\"}", humidity, temperature, mac_string);
printf("\n%s\n", bufferB);
const char *post_data = bufferB;
esp_http_client_config_t config =
{
.url = "https://????????????????????.supabase.co/rest/v1/nome-da-tabela",
.event_handler = _http_event_handler,
.crt_bundle_attach = esp_crt_bundle_attach,
.buffer_size_tx = 1024,
};
ESP_LOGI(TAG, "HTTPS request start =>");
esp_http_client_handle_t client = esp_http_client_init(&config);
esp_http_client_set_header(client, "apikey", apikey);
esp_http_client_set_header(client, "Authorization", bearer);
esp_http_client_set_header(client, "Content-Type", "application/json");
esp_http_client_set_header(client, "User-Agent", "esp-idf/1.0 esp32");
esp_http_client_set_method(client, HTTP_METHOD_POST);
esp_http_client_set_post_field(client, post_data, strlen(post_data));
esp_err_t err = esp_http_client_perform(client);
if (err == ESP_OK)
{
ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRId64,
esp_http_client_get_status_code(client),
esp_http_client_get_content_length(client));
} else
{
ESP_LOGE(TAG, "HTTP POST Status = %d, content_length = %lld");
ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
}
esp_http_client_cleanup(client);
}

void request_task(void *pvParameters)
{
https_request();
ESP_LOGI(TAG, "Finish request");
#if !CONFIG_IDF_TARGET_LINUX
vTaskDelete(NULL);
#endif
}

void app_main(void)
{
esp_err_t ret = nvs_flash_init();
if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
{
ESP_ERROR_CHECK(nvs_flash_erase());
ret = nvs_flash_init();
}
ESP_ERROR_CHECK(ret);
ESP_ERROR_CHECK(esp_netif_init());
ESP_ERROR_CHECK(esp_event_loop_create_default());
ESP_ERROR_CHECK(example_connect());
ESP_LOGI(TAG, "Connected to access point");
#if CONFIG_IDF_TARGET_LINUX
http_request_task(NULL);
#else
while (1)
{
xTaskCreate(&request_task, "request_task", 8192, NULL, 5, NULL);
vTaskDelay(600000 / portTICK_PERIOD_MS);
fflush(stdout);
}
#endif
}
