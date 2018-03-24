# video-server

## Установка

    git clone https://github.com/roboflot-ru/video-server.git video-server
    cd video-server
    chmod +x install.sh
    ./install.sh
    cd Server
    make all



## Настройки сервера

    nano VideoService.ini

http_server_port - порт http сервера для управления трансляциями через API



## Запуск видеосервера

    ./VideoService



## Автозапуск и управление через systemctl

Создать файл

    sudo nano /etc/systemd/system/video.service

с таким содержимым (поменять USERNAME и каталог установки при необходимости)

    [Unit]
    Description=VideoService

    [Service]
    Type=simple
    User=USERNAME
    WorkingDirectory=/home/USERNAME/video-server/Server
    ExecStart=/home/USERNAME/video-server/Server/VideoService
    Restart=on-abort

    [Install]
    WantedBy=default.target

Ctrl+X сохраняем
Обновляем сервисы запуска

    sudo systemctl daemon-reload

Включаем автозапуск при загрузке

    sudo systemctl enable video

Запустить вручную

    sudo systemctl start video

Состояние

    sudo systemctl status video

Остановить

    sudo systemctl stop video




## Управление трансляциями

UID - идентификатор
PORT_IN - входящий порт, куда транслирует камера
PORT_OUT - исходящий порт, откуда смотрит пользователь

Включаем канал:

    curl -v 'http://localhost:9009/register?uid=UID&port_in=PORT_IN&port_out=PORT_OUT'

Отключаем канал:

    curl -v 'http://localhost:9009/unregister?uid=UID'


## Трансляция

Запускаем трасляцию с борта

    curl -v 'http://localhost:9009/start_live?width=640&height=480&framerate=25&bitrate=1000000&server=SERVER_HOST&port=PORT_IN'

Адрес для плеера

    rtsp://SERVER_HOST:PORT_OUT/UID




