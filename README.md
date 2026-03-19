<!-- ABOUT THE PROJECT -->
# AINet

## Aquisição de dados com um modelo de implantação flexível e um conjunto de políticas e procedimentos que atuam na continuidade, recuperação de ambientes e replicação de dados armazenados, suportando aplicações de TI de modo que permaneçam estáveis.

A instalacao do esp-idf ocorre de forma simples, com todos os pré-requisitos instalados você precisa das bibliotecas de software fornecidas pela Espressif no repositório ESP-IDF . Consulte a seção Versões do ESP-IDF para obter informações sobre qual versão usar em uma determinada situação.

```
#mkdir -p ~/esp
#cd ~/esp
#git clone -b v5.5.3 --recursive https://github.com/espressif/esp-idf.git
#cd ~/esp/esp-idf
#./install.sh esp32
#. ./export.sh
```

Efetue o download do projeto em https://github.com/daoliveirafilho/AINet, após descompactá-lo no diretório de sua escolha, inicie um novo projeto.

```
#cd ~/esp/esp-idf/
#mkdir ~/esp/esp-idf/programa
#cd ~/esp/esp-idf/programa/
#cp -R ~/Downloads/AINet/projeto/* .
```

Defina ESP32 como o alvo e execute o compilador do projeto.

```
#idf.py set-target esp32
#idf.py build
```
O aplicativo e todos os componentes serão compilados, gerando em seguida o carregador de inicialização, a tabela de partições e os binários do aplicativo. Para gravar os arquivos binários que você acabou de compilar para o ESP32 na etapa anterior, você precisa executar o seguinte comando.

```
#python -m esptool --chip esp32 -b 115200 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size 4MB 0x1000 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0x10000 build/program.bin
```

Se não houver problemas ao final do processo de gravação da memória flash, a placa será reiniciada e iniciará o aplicativo. O monitor de log pode ser ativado com o comando a seguir.

```
#idf.py -b 115200 -p /dev/ttyUSB0 monitor
```

Para interromper o modo monitor use o atalho de teclado ctrl+] ou ctrl+T seguido de ctrl+X.

O armazenamento dos dados com Supabase é bastante recomendado, por ser extremamente fácil criar uma conta permitindo que desenvolvedores configurem um backend PostgreSQL completo em minutos. Após criar um projeto e efetuar a configuração da chave secreta, pode configurar o arquivo ~/esp/esp-idf/programa/main/programa.c, onde estes valores devem ser setados.

```
...
const char *apikey = "sb_secret_???????????????????????????????";
...
esp_http_client_config_t config =
{
.url = "https://????????????????????.supabase.co/rest/v1/nome-da-tabela",
...
```

A configuração do certificado é necessária para a comunicação do dispositivo com o endpoint. Deve-se extrair o certificado com o comando openssl, editar o arquivo com o editor de textos de sua preferência e salvá-lo no diretório main do projeto.

```
#openssl s_client -showcerts -connect ????????????????????.supabase.co:443 >~/esp/esp-idf/programa/main/cert.pem
```

Apenas o primeiro bloco do certificado deve permanecer no arquivo cert.pem, como no exemplo abaixo.

```
-----BEGIN CERTIFICATE-----
BQUHAQEEUjBQMCcGCCsGAQUFBzABhhtodHRwOi8vby5wa2kuZ29vZy9zL3dlMS83
RWcwJQYIKwYBBQUHMAKGGWh0dHA6Ly9pLnBraS5nb29nL3dlMS5jcnQwGAYDVR0R
BBEwD4INcmVrYWxsLmxvZy5icjATBgNVHSAEDDAKMAgGBmeBDAECATA2BgNVHR8E
LzAtMCugKaAnhiVodHRwOi8vYy5wa2kuZ29vZy93ZTEvSzBVVkFLZTVOOTQuY3Js
MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYADleUvPOuqT4zGyyZB7P3kN+bwj1x
MiXdIaklrGHFTiEAAAGc+QTPHgAABAMARzBFAiBO7b0+okE2KoTQ2M8ibmLHp046
SKiH6Msv0F/w+Fko3gIhAJgWNOYTaBF1QaKznCPYN15Iq96gT6YVexxey0K0dR0Y
AHYAyzj3FYl8hKFEX1vB3fvJbvKaWc1HCmkFhbDLFMMUWOcAAAGc+QTPeQAABAMA
RzBFAiBUQceFC4HxLfNb7ceSZLZ5YiJP4d9EVBwMM2zCirgn+gIhAJLPRKbIW77p
mafdwLZWGdkA4O/xlYohh4r/zcUcqnZFMAoGCCqGSM49BAMCA0cAMEQCIHPkOdeX
VYXUklbYGM/Okl9hEyblDQlPViPmcDL7n+O/199WhaePEcmLHlFm/knCYNhGQi4D
tDg71UXpohTtFGRBhf2jggJGMIICQjAOBgNVHQ8BAf8EBAMCB4AwEwYDVR0lBAww
CgYIKwYBBQUHAwEwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUG9hGwjM1bECgVq7D
nmHcOnyD600wHwYDVR0jBBgwFoAUkHeSNWfE/6jMqeZ72YB5e8yT+TgwXgYIKwYB
BQUHAQEEUjBQMCcGCCsGAQUFBzABhhtodHRwOi8vby5wa2kuZ29vZy9zL3dlMS83
RWcwJQYIKwYBBQUHMAKGGWh0dHA6Ly9pLnBraS5nb29nL3dlMS5jcnQwGAYDVR0R
BBEwD4INcmVrYWxsLmxvZy5icjATBgNVHSAEDDAKMAgGBmeBDAECATA2BgNVHR8E
LzAtMCugKaAnhiVodHRwOi8vYy5wa2kuZ29vZy93ZTEvSzBVVkFLZTVOOTQuY3Js
MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYADleUvPOuqT4zGyyZB7P3kN+bwj1x
MiXdIaklrGHFTiEAAAGc+QTPHgAABAMARzBFAiBO7b0+okE2KoTQ2M8ibmLHp046
SKiH6Msv0F/w+Fko3gIhAJgWNOYTaBF1QaKznCPYN15Iq96gT6YVexxey0K0dR0Y
AHYAyzj3FYl8hKFEX1vB3fvJbvKaWc1HCmkFhbDLFMMUWOcAAAGc+QTPeQAABAMA
RzBFAiBUQceFC4HxLfNb7ceSZLZ5YiJP4d9EVBwMM2zCirgn+gIhAJLPRKbIW77p
mafdwLZWGdkA4O/xlYohh4r/zcUcqnZFMAoGCCqGSM49BAMCA0cAMEQCIHPkOdeX
LDk4yEoq9WyvfeloeNkyT2K7LZyYO0lrQ0AgAiB4DIBAse7Ue0dRVx/XhVcTIhJH
1nR9K/XKbs83q5HI1g==
-----END CERTIFICATE-----
```

---

_Tecn&oacute;logo de redes de computadores dedicado em criar c&oacute;digo de baixo n&iacute;vel, confi&aacute;vel, eficiente e bem projetado. Para mim, C &eacute; a ferramenta perfeita para construir software que n&atilde;o seja apenas perform&aacute;tico, mas tamb&eacute;m seguro e elegante desde a sua concep&ccedil;&atilde;o._

---

![Apache](assets/images/badges/apache.svg)
![FreeBSD](assets/images/badges/freebsd.svg)
![Github](assets/images/badges/github.svg)
![Grafana](assets/images/badges/grafana.svg)
![Huawei](assets/images/badges/huawei.svg)
![Nginx](assets/images/badges/nginx.svg)
![Ollama](assets/images/badges/ollama.svg)
![PHP](assets/images/badges/php.svg)
![Telegram](assets/images/badges/telegram.svg)

![Bash](assets/images/badges/bash.svg)
![Bluetooth](assets/images/badges/ble.svg)
![C](assets/images/badges/C.svg)
![esp-idf](assets/images/badges/esp-idf.svg)
![FreeRTOS](assets/images/badges/freertos.svg)
