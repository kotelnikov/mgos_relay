# Mongoose OS Relay library

## Overview
The Relay library for [Mongoose OS](https://mongoose-os.com/) framework. It is useful for projects where you have to manage number of relays, leds or other stuff like power and reset gpio of the different peripherals connected to the main micro controller. The library successfully tested on ESP8266 and ESP32 from [Espressif Systems](https://www.espressif.com/). If you found this library useful for your Mongoose OS project, please give this repository a star! ⭐

## How to use the library
### Include library to your project
Firstly add following to your ```mos.yml``` file in section "libs":

```
libs:
  - origin: https://github.com/kotelnikov/mgos_relay
    name: relay
```

### MJS (Java Script) usage
If your prefer to develop in JS (using MJS engine) add the following code to your ```init.js``` file:

```js
load("api_relay.js");
load("api_timer.js");

let relay_pin = 2;

// create(gpio, inactive_state, pull) -> create Relay instance
// (int) gpio -> relay GPIO
// (bool) inactive_state -> Inactive relay GPIO level - "true" if HIGH(+) or "false" if LOW(-)
// (bool) pull -> Pull GPIO - "true" if YES or "false" if NO
let relay = Relay.create(relay_pin, false, true);

// on() -> switch on the Relay
relay.on();
print('1. Current Relay state is (must be 1):', relay.state());

// off() -> switch off the Relay
relay.off();
print('2. Current Relay state is (must be 0):', relay.state());

// toggle() -> change Relay to opposite state
relay.toggle();
print('3. Current Relay state is (must be 1):', relay.state());

relay.toggle();
print('4. Current Relay state is (must be 0):', relay.state());

// touch(timeout) -> changes Relay to opposite state for timeout period (in milliseconds)
relay.touch(5000);
print('5. Current Relay state is (must be 1, and after 5 sec. 0):', relay.state());

Timer.set(7000, Timer.REPEAT, function() {
    // state() -> return current Relay state
    let relay_state = relay.state();
    print('6. Current Relay state is :', relay_state);
    // close() -> close Relay instance, free used memory
    relay.delete();
}, null);

```

### C usage
If your prefer to develop in C Add following code to your ```main.c``` file:
```c
#include "mgos.h"
#include "mgos_relay.h"

#define RELAY_PIN 16

struct mgos_relay *relay;

static void relay_callback(void *arg) {
    LOG(LL_INFO, ("6. Current Relay state is (must be 0): %i", mgos_relay_get_state(relay)));
    mgos_relay_clear(relay);
    (void) arg;
}

void test_relay(void) {
    
    // Deprecated but still works, use mgos_relay_initialize() instead
    // relay = mgos_relay_create(RELAY_PIN, NORMALLY_OPEN /* or NORMALLY_CLOSED */);

    // mgos_relay_initialize(gpio, inactive_state, pull) -> create Relay struct
    // (uint8_t) gpio -> relay GPIO
    // (bool) inactive_state -> Inactive relay GPIO level - "true" if HIGH(+) or "false" if LOW(-)
    // (bool) pull -> Internal MCU GPIO pullup/pulldown - "true" if YES or "false" if NO
    relay = mgos_relay_initialize(RELAY_PIN, false , true);
    
    mgos_relay_on(relay);
    LOG(LL_INFO, ("1. Current Relay state is (must be 1): %i", mgos_relay_get_state(relay)));
    
    mgos_relay_off(relay);    
    LOG(LL_INFO, ("2. Current Relay state is (must be 0): %i", mgos_relay_get_state(relay)));
    
    mgos_relay_toggle(relay);    
    LOG(LL_INFO, ("3. Current Relay state is (must be 1): %i", mgos_relay_get_state(relay)));

    mgos_relay_toggle(relay);    
    LOG(LL_INFO, ("4. Current Relay state is (must be 0): %i", mgos_relay_get_state(relay)));
    
    mgos_relay_touch(3000, relay);
    LOG(LL_INFO, ("5. Current Relay state is (must be 1, but after 3 sec. 0): %i", mgos_relay_get_state(relay)));

    mgos_set_timer(5000, false, relay_callback, NULL);
};

enum mgos_app_init_result mgos_app_init(void) {
    test_relay();
    return MGOS_APP_INIT_SUCCESS;
};
```

## Misc
For additional information look through the code from the library's files, and also files in "examples" folder.