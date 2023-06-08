# SiM 
**(SimpleMessenger)**

Простой мессенджер с асинхронной клиент-серверной архитектурой.

Более подробная [документация](https://flat-bottle-4d0.notion.site/SiM-Simple-Messanger-59bc7913f6df40d39e96d371060d6874?pvs=4) по проекту

# Требования
- 
- 


# Клиент
## Сборка

```
cmake -DBUILD_CLIENT=ON -S . -B build
cmake --build build -t CLIENT

./build/SiM 192.168.1.1 login password
```

В данном примере необходимо подставить ip адрес сервера и свой логин/пароль

## Список поддерживаемых команд
- 
-
- 

# Север
## Сборка

```
cmake -DBUILD_SERVER=ON -S . -B build
cmake --build build -t SERVER

./build/SiM

```

## Список поддерживаемых команд
- 
-
- 
