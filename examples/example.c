#include "mgos.h"
#include "mgos_relay.h"

#define RELAY_PIN 2

struct mgos_relay *relay;

static void relay_callback(void *arg) {
    LOG(LL_INFO, ("6. Current Relay state is (must bee 0): %i", mgos_relay_get_state(relay)));
    mgos_relay_clear(relay);
    (void) arg;
}

void test_relay = function(void) {
    relay = mgos_relay_create(RELAY_PIN, NORMALLY_OPEN);
    
    uint8_t state = mgos_relay_on(relay);
    LOG(LL_INFO, ("1. Current Relay state is (must bee 1): %i", state));
    
    state = mgos_relay_off(relay);    
    LOG(LL_INFO, ("2. Current Relay state is (must bee 0): %i", state));
    
    state = mgos_relay_toggle(relay);    
    LOG(LL_INFO, ("3. Current Relay state is (must bee 1): %i", state));

    state = mgos_relay_toggle(relay);    
    LOG(LL_INFO, ("4. Current Relay state is (must bee 0): %i", state));
    
    state = mgos_relay_touch(3000, relay);  
    LOG(LL_INFO, ("5. Current Relay state is (must bee 1, but after 5 sec. 0): %i", state));

    mgos_set_timer(10000, false, relay_callback, NULL);
};

enum mgos_app_init_result mgos_app_init(void) {
    test_relay();
    return MGOS_APP_INIT_SUCCESS;
};