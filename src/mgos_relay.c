/*
 * 2020 Aleksey Kotelnikov <kotelnikov.www@gmail.com>
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

#include "mgos.h"
#include "mgos_timers.h"
#include "mgos_gpio.h"
#include "mgos_relay.h"

struct mgos_relay {
    uint8_t gpio;
    enum    mgos_relay_type type;
    uint8_t state;
};

static void mgos_relay_callback(void *arg);

static void mgos_relay_callback(void *arg) {
    struct mgos_relay *r = (struct mgos_relay*)arg;
    mgos_relay_toggle(r);
    (void) arg;
};

struct mgos_relay *mgos_relay_create(uint8_t gpio, enum mgos_relay_type type) {
    struct mgos_relay *r = calloc(1, sizeof(*r));
    if (r == NULL) return NULL;
    memset(r, 0, sizeof(struct mgos_relay));

    r->gpio = gpio;
    r->type = type;
    r->state = 0;

    bool res = mgos_gpio_set_mode(gpio, MGOS_GPIO_MODE_OUTPUT);
    if (res) {
        mgos_gpio_write(gpio, type == NORMALLY_OPEN ? 0 : 1);
        return r;
    }
    else {
        mgos_relay_clear(r);
        return NULL;
    } 
};

void mgos_relay_clear(struct mgos_relay *r) {
  free(r);
  r = NULL;
};

uint8_t mgos_relay_get_state(struct mgos_relay *r) {
    uint8_t res = r->state;
    return res;
};

uint8_t mgos_relay_on(struct mgos_relay *r) {
    mgos_gpio_write(r->gpio, r->type == NORMALLY_OPEN ? 1 : 0);
    r->state = 1;
    return mgos_relay_get_state(r);
};

uint8_t mgos_relay_off(struct mgos_relay *r) {
    mgos_gpio_write(r->gpio, r->type == NORMALLY_OPEN ? 0 : 1);
    r->state = 0;
    return mgos_relay_get_state(r);
};

uint8_t mgos_relay_toggle(struct mgos_relay *r) {
    return r->state == 0 ? mgos_relay_on(r) : mgos_relay_off(r);
};

uint8_t mgos_relay_touch(uint16_t ms, struct mgos_relay *r) {
    int res = mgos_relay_toggle(r);
    mgos_set_timer(ms, false, mgos_relay_callback, r);
    return res;
};

uint8_t mgos_relay_init(void) {
    return true;
};