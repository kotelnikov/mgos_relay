let Relay = {
    NORMALLY_OPEN: 0,
    NORMALLY_CLOSE: 1,

    _crt: ffi('void *mgos_relay_create(int, int)'),
    _clr: ffi('void mgos_relay_clear(void *)'),
    _on: ffi('int mgos_relay_on(void *)'),
    _off: ffi('int mgos_relay_off(void *)'),
    _tgl: ffi('int mgos_relay_toggle(void *)'),
    _tch: ffi('int mgos_relay_touch(int, void *)'),
    _st: ffi('int mgos_relay_get_state(void *)'),

    clear: function() {
        Relay._clr(this._r);
    },
    on: function() {
        Relay._on(this._r);
    },
    off: function() {
        return Relay._off(this._r);
    },
    toggle: function() {
        return Relay._tgl(this._r);            
    },
    touch: function(ms) {
        return Relay._tch(ms, this._r);            
    },
    state: function() {
        return Relay._st(this._r);                
    },
    create: function(pin, type) {
        return Object.create({
            _r: Relay._crt(pin, type),
            close: Relay.close,
            state: Relay.state,
            on: Relay.on,
            off: Relay.off,
            toggle: Relay.toggle,
            touch: Relay.touch
        });
    }
};