let Relay = {
    _crt: ffi('void *mgos_relay_initialize(int, bool, bool)'),
    _clr: ffi('void mgos_relay_clear(void *)'),
    _on: ffi('void mgos_relay_on(void *)'),
    _off: ffi('void mgos_relay_off(void *)'),
    _tgl: ffi('void mgos_relay_toggle(void *)'),
    _tch: ffi('void mgos_relay_touch(int, void *)'),
    _st: ffi('bool mgos_relay_get_state(void *)'),
    create: function(pin, inactive_level, pull) {
        return Object.create({
            _r: Relay._crt(pin, inactive_level, pull),
            close: Relay.close,
            state: Relay.state,
            on: Relay.on,
            off: Relay.off,
            toggle: Relay.toggle,
            touch: Relay.touch
        });
    },
    on: function() {
        Relay._on(this._r);
    },
    off: function() {
        Relay._off(this._r);
    },
    toggle: function() {
        Relay._tgl(this._r);            
    },
    touch: function(ms) {
        Relay._tch(ms, this._r);            
    },
    get_state: function() {
        return Relay._st(this._r);                
    },
    delete: function() {
        Relay._clr(this._r);
    }
};