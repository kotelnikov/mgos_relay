# Библиотека Реле для Mongoose OS

## Общие сведения
Библиотека Relay, разработана для фрейворка [Mongoose OS](https://mongoose-os.com/). Библиотеку удобно использовать, в случае, когда в проекте необходимо управлять некоторым количеством реле, светодиодами, ключами транзисторов, а также в случае, если существует необходимость контролировать пины rst или pwr для каких-либо переферийных устройств соединенных с основным микроконтроллером. Библиотека протестирована с микроконтроллерами ESP8266 и ESP32 от [Espressif Systems](https://www.espressif.com/). Если эта библиотека пригодилась для вашего проекта, пожалуйста поставьте этому репозиторию звезду! ⭐

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

let relay_pin = 16;

// create(gpio, type) -> создание объекта Реле
// (int) gpio -> используемый порт микроконтроллера
// (bool) inactive_state -> Уровень реле в состоянии "выключено", false - Низкий(-) или true - Высокий(+)
// (bool) pull -> Внутренняя подтяжка пина в контроллере - "true" если требуется или "false" если не требуется
let relay = Relay.create(relay_pin, false, true);

// on() -> включает Реле в состояние "включено"
relay.on();
print('1. Текущее состояние Реле (должно быть 1):', relay.get_state());

// off() -> выключает Реле в состояние "выключено"
relay.off();
print('2. Текущее состояние Реле (должно быть 0):', relay.get_state());

// toggle() -> изменяет состояние Реле на противоположное
relay.toggle();
print('3. Текущее состояние Реле (должно быть 1):', relay.get_state());
relay.toggle();
print('4. Текущее состояние Реле (должно быть 0):', relay.get_state());

// touch(timeout) -> изменяет состояние Реле на ВКЛЮЧЕНО на интервал переданный в функцию (в миллисекундах)
// по истечение интервала, Реле возвращается в исходное состояние
elay.touch(5000);
print('5. Текущее состояние Реле (должно быть 1, затем через 5 секунд 0):', relay.get_state());

Timer.set(7000, Timer.REPEAT, function() {
    // state() -> возвращает текущее состояние Реле
    let relay_state = relay.get_state();
    print('6. Текущее состояние Реле (должно быть 0):', relay_state);
    // delete() -> освобождает память выделенную под объект Реле
    relay.delete();
}, null);

```

### Разработка на C
Если  Вы предпочитаете вести разработку на C, то ознакомьтесь с примером использования ниже, просто добавьте код в файл ```main.c``` в своем проекте.

```c
#include "mgos.h"
#include "mgos_relay.h"

#define RELAY_PIN 16

struct mgos_relay *relay;

static void relay_callback(void *arg) {
    LOG(LL_INFO, ("6. Текущее состояние Реле (должно быть 0): %i", mgos_relay_get_state(relay)));
    mgos_relay_clear(relay);
    (void) arg;
}

void test_relay = function(void) {
    
    // Устаревшая функция (но работает для совместимости), используйте mgos_relay_initialize()
    //relay = mgos_relay_create(RELAY_PIN, NORMALLY_OPEN /* or NORMALLY_CLOSED */);

    // mgos_relay_initialize(gpio, inactive_state, pull) -> Инициализирует реле и задает начальное состояние
    // (uint8_t) gpio -> используемый порт микроконтроллера
    // (bool) inactive_state -> Уровень реле в состоянии "выключено", false - Низкий(-) или true - Высокий(+)
    // (bool) pull -> Внутренняя подтяжка пина в контроллере - "true" если требуется или "false" если не требуется
    relay = mgos_relay_initialize(RELAY_PIN, false, true);
    
    mgos_relay_on(relay);
    LOG(LL_INFO, ("1. Текущее состояние Реле (должно быть 1): %i", mgos_relay_get_state(relay)));
    
    mgos_relay_off(relay);
    LOG(LL_INFO, ("2. Текущее состояние Реле (должно быть 0): %i", mgos_relay_get_state(relay)));
    
    mgos_relay_toggle(relay);
    LOG(LL_INFO, ("3. Текущее состояние Реле (должно быть 1): %i", mgos_relay_get_state(relay)));

    mgos_relay_toggle(relay);
    LOG(LL_INFO, ("4. Текущее состояние Реле (должно быть 0): %i", mgos_relay_get_state(relay)));
    
    mgos_relay_touch(3000, relay);
    LOG(LL_INFO, ("5. Текущее состояние Реле (должно быть 1, затем через 5 секунд 0): %i", mgos_relay_get_state(relay)));

    mgos_set_timer(5000, false, relay_callback, NULL);
};

enum mgos_app_init_result mgos_app_init(void) {
    test_relay();
    return MGOS_APP_INIT_SUCCESS;
};
```

## Прочее
Более подробную информацию Вы можете получить изучив код библиотеки, а также примеры, приведенные в папке "examples".