# Mongoose OS Relay library

## Overview
Mongoose OS Relay library. It is useful for projects where you have to manage number of relays, led and other stuff like power and reset gpio of the different peripherals.

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
let relay_state = 0;

// create(gpio, type) -> create Relay instance
// gpio -> relay gpio
// type -> Relay.NORMALLY_OPEN or Relay.NORMALLY_CLOSED
let relay = Relay.create(relay_pin, Relay.NORMALLY_OPEN);

// on() -> switch on the Relay
relay_state = relay.on();
print('1. Current Relay state is (must be 1):', relay_state);

// off() -> switch off the Relay
relay_state = relay.off();
print('2. Current Relay state is (must be 0):', relay_state);

// toggle() -> change Relay to opposite state
relay_state = relay.toggle();
print('3. Current Relay state is (must be 1):', relay_state);
relay_state = relay.toggle();
print('4. Current Relay state is (must be 0):', relay_state);

// touch(timeout) -> changes Relay to opposite state for timeout period (in milliseconds)
relay_state = relay.touch(5000);
print('5. Current Relay state is (must bee 1, and after 5 sec. 0):', relay_state);

Timer.set(7000, Timer.REPEAT, function() {
    // state() -> return current Relay state
    relay_state = relay.state();
    print('6. Current Relay state is :', relay_state);
    // close() -> close Relay instance, free used memory
    relay.close();
}, null);

```

### C usage
If your prefer to develop in C Add following code to your ```main.c``` file:
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

## Misc
For more details look through the code files, and "examples" folder.