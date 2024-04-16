/*
 * 2024 Aleksey Kotelnikov <kotelnikov.www@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos_debug.h"
#include "mgos_gpio.h"
#include "mgos_timers.h"
#include "mgos_relay.h"

struct mgos_relay {
    const uint8_t gpio;
    const bool inactive_level;
    bool current_state;
};

static void mgos_relay_callback(void *arg);

static void mgos_relay_callback(void *arg) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_callback()"));
    struct mgos_relay *r = (struct mgos_relay*)arg;
    mgos_relay_off(r);
};

struct mgos_relay *mgos_relay_initialize(uint8_t gpio, bool inactive_level, bool pull) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_initialize()"));
    struct mgos_relay *r = calloc(1, sizeof(*r));
    if (r == NULL) {
        LOG(LL_ERROR, ("RELAY: Error allocating memory"));
        return NULL;
    }

    struct mgos_relay r_init = {gpio, inactive_level, false};
    memcpy(r, &r_init, sizeof(struct mgos_relay));

    bool res = mgos_gpio_setup_output(gpio, inactive_level);
    if (res) {
        res = pull ? 
            mgos_gpio_set_pull(gpio, inactive_level ? MGOS_GPIO_PULL_UP : MGOS_GPIO_PULL_DOWN) : 
            mgos_gpio_set_pull(gpio, MGOS_GPIO_PULL_NONE);
    }
    if (!res) {
        LOG(LL_ERROR, ("RELAY: GPIO setup output error"));
        mgos_relay_clear(r);
        return NULL;
    }
    return r;
};

bool mgos_relay_get_state(struct mgos_relay *r) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_get_state()"));
    return r->current_state;
};

void mgos_relay_on(struct mgos_relay *r) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_on()"));
    mgos_gpio_write(r->gpio, !r->inactive_level ? 1 : 0);
    r->current_state = true;
};

void mgos_relay_off(struct mgos_relay *r) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_off()"));
    mgos_gpio_write(r->gpio, !r->inactive_level ? 0 : 1);
    r->current_state = false;
};

void mgos_relay_toggle(struct mgos_relay *r) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_toggle()"));
    !r->current_state ? mgos_relay_on(r) : mgos_relay_off(r);
};

void mgos_relay_touch(uint16_t ms, struct mgos_relay *r) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_touch()"));
    mgos_relay_on(r);
    mgos_set_timer(ms, false, mgos_relay_callback, r);
};

void mgos_relay_clear(struct mgos_relay *r) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_clear()"));
    free(r);
    r = NULL;
};

uint8_t mgos_relay_init(void) {
    LOG(LL_DEBUG, ("RELAY: mgos_relay_init()"));    
    return true;
};

// !!!DEPRECATED!!! Use mgos_relay_initialize instead
struct mgos_relay *mgos_relay_create(uint8_t gpio, enum mgos_relay_type type) {
    LOG(LL_INFO, ("RELAY: mgos_relay_create() !!!DEPRECATED FN!!! Use mgos_relay_initialize() instead"));
    return mgos_relay_initialize(gpio, type == NORMALLY_OPEN ? false : true, false);
};