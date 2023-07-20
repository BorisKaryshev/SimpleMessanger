# SiM 
**(SimpleMessenger)**

Простой мессенджер с асинхронной клиент-серверной архитектурой.

Более подробная [документация](https://flat-bottle-4d0.notion.site/SiM-Simple-Messanger-59bc7913f6df40d39e96d371060d6874?pvs=4) по проекту



# Клиент
## Сборка

```
cmake -DBUILD_CLIENT=ON -S . -B build
cmake --build build -t CLIENT

./build/SiM 192.168.1.1 8080    # 192.168.1.1 is an example address and 8080 is an example port
```

В данном примере необходимо подставить ip адрес сервера и свой логин/пароль

## Список поддерживаемых команд
- `send` [receiver login] [text]
  - Sends message to receiver.
  - [receiver login] - one word.
  - [text] - message to be send. Can contain any ASCII characters.
- `stop`
  - Stops client.

# Север
## Сборка

```
cmake -DBUILD_SERVER=ON -S . -B build
cmake --build build -t SERVER

./build/SiM 8080       # 8080 is an example port
```

## Список поддерживаемых команд
- `stop`
  - Stops server.
