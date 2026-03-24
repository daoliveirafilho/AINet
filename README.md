<!-- ABOUT THE PROJECT -->
# AINet

## Aquisição de dados com um modelo de implantação flexível e um conjunto de políticas e procedimentos que atuam na continuidade, recuperação de ambientes e replicação de dados armazenados, suportando aplicações de TI de modo que permaneçam estáveis.

Certifique-se de que já tenha cumprido os pré-requisitos de configuração do ambiente.

```
#mkdir -p ~/esp
#cd ~/esp
#git clone -b v5.5.3 --recursive https://github.com/espressif/esp-idf.git
#cd ~/esp/esp-idf
#./install.sh esp32
#. ./export.sh
```

Clone o repositório de modelo, em https://github.com/daoliveirafilho/AINet, ou crie seu próprio repositório inicial de firmware.

```
#cd ~/esp/esp-idf/
#mkdir ~/esp/esp-idf/programa
#cd ~/esp/esp-idf/programa/
#cp -R ~/Downloads/AINet/projeto/* .
#idf.py set-target esp32
#idf.py build
```

Adicione ou modifique seus códigos para executar funções e atender aos requisitos. Compile o aplicativo e todos os componentes, gerando em seguida o carregador de inicialização, a tabela de partições e os binários do aplicativo. Para gravar os arquivos binários que você acabou de compilar para o ESP32 na etapa anterior, você precisa executar o seguinte comando.

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
