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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum mgos_relay_type {
    NORMALLY_OPEN,
    NORMALLY_CLOSE
};

struct mgos_dht;

struct mgos_relay* mgos_relay_create(uint8_t gpio, enum mgos_relay_type type);
void mgos_relay_clear(struct mgos_relay *r);
uint8_t mgos_relay_on(struct mgos_relay *r);
uint8_t mgos_relay_off(struct mgos_relay *r);
uint8_t mgos_relay_toggle(struct mgos_relay *r);
uint8_t mgos_relay_touch(uint16_t ms, struct mgos_relay *r);
uint8_t mgos_relay_get_state(struct mgos_relay *r);
uint8_t mgos_relay_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */