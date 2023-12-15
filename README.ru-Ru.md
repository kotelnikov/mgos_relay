# Библиотека Реле для Mongoose OS

## Общие сведения
Библиотека Relay, разработана для фрейворка [Mongoose OS](https://mongoose-os.com/). Библиотеку удобно использовать, в случае, когда в проекте необходимо управлять некоторым количеством реле, светодиодами, ключами транзисторов, а также в случае, если существует необходимость контролировать пины rst или pwr для каких-либо переферийных устройств соединенных с основным микроконтроллером. Библиотека протестирована с микроконтроллерами ESP8266 и ESP32 от [Espressif Systems](https://www.espressif.com/).

## Использование библиотеки
### Добавление в проект
Для добавления библиотеки в проект, необходимо включить указанный код в секцию "libs" Вашего файла ```mos.yml```:
```
libs:
  - origin: https://github.com/kotelnikov/mgos_relay
    name: relay
```

### Разработка на Java Script
Если Вы предпочитаете использовать Java Script (движок MJS), в библиотеке реализованы все нобходимые ffi API. Пример использования продемонстрирован в коде ниже. Скопируйте код или его часть в файл ```init.js``` в своем проекте.

```js
load("api_relay.js");
load("api_timer.js");

let relay_pin = 2;
let relay_state = 0;

// create(gpio, type) -> создание объекта Реле
// gpio -> используемый порт микроконтроллера
// type -> тип Реле, может быть "Relay.NORMALLY_OPEN" или "Relay.NORMALLY_CLOSED"
// в зависимости от этого, Реле является "нормально разомкнутым" или "нормально замкнутым"
let relay = Relay.create(relay_pin, Relay.NORMALLY_OPEN);

// on() -> включает Реле в состояние "включено"
relay_state = relay.on();
print('1. Текущее состояние Реле (должно быть 1):', relay_state);

// off() -> выключает Реле в состояние "выключено"
relay_state = relay.off();
print('2. Текущее состояние Реле (должно быть 0):', relay_state);

// toggle() -> изменяет состояние Реле на противоположное
relay_state = relay.toggle();
print('3. Текущее состояние Реле (должно быть 1):', relay_state);
relay_state = relay.toggle();
print('4. Текущее состояние Реле (должно быть 0):', relay_state);

// touch(timeout) -> изменяет состояние Реле на противоположное на интервал переданный в функцию (в миллисекундах)
// по истечение интервала, Реле возвращается в исходное состояние
relay_state = relay.touch(5000);
print('5. Текущее состояние Реле (должно быть 1, затем через 5 секунд 0):', relay_state);

Timer.set(7000, Timer.REPEAT, function() {
    // state() -> возвращает текущее состояние Реле
    relay_state = relay.state();
    print('6. Текущее состояние Реле:', relay_state);
    // close() -> освобождает память выделенную под объект Реле
    relay.close();
}, null);

```

### Разработка на C
Если  Вы предпочитаете вести разработку на C, то ознакомьтесь с примером использования ниже, просто добавьте код в файл ```main.c``` в своем проекте.

```c
#include "mgos.h"
#include "mgos_relay.h"

#define RELAY_PIN 2

struct mgos_relay *relay;

static void relay_callback(void *arg) {
    LOG(LL_INFO, ("6. Current Relay state is (must be 0): %i", mgos_relay_get_state(relay)));
    mgos_relay_clear(relay);
    (void) arg;
}

void test_relay = function(void) {
    relay = mgos_relay_create(RELAY_PIN, NORMALLY_OPEN /* or NORMALLY_CLOSED */);
    
    uint8_t state = mgos_relay_on(relay);
    LOG(LL_INFO, ("1. Current Relay state is (must be 1): %i", state));
    
    state = mgos_relay_off(relay);    
    LOG(LL_INFO, ("2. Current Relay state is (must be 0): %i", state));
    
    state = mgos_relay_toggle(relay);    
    LOG(LL_INFO, ("3. Current Relay state is (must be 1): %i", state));

    state = mgos_relay_toggle(relay);    
    LOG(LL_INFO, ("4. Current Relay state is (must be 0): %i", state));
    
    state = mgos_relay_touch(3000, relay);  
    LOG(LL_INFO, ("5. Current Relay state is (must be 1, but after 3 sec. 0): %i", state));

    mgos_set_timer(5000, false, relay_callback, NULL);
};

enum mgos_app_init_result mgos_app_init(void) {
    test_relay();
    return MGOS_APP_INIT_SUCCESS;
};
```

## Прочее
Более подробную информацию Вы можете получить изучив код библиотеки, а также примеры, приведенные в папке "examples".