load("api_relay.js");
load("api_timer.js");

let led = Relay.create(2, false, true);

led.on();
print('1. Current Relay state is (must bee on):', led.get_state());

led.off();
print('2. Current Relay state is (must bee off):', led.get_state());

led.toggle();
print('3. Current Relay state is (must bee on):', led.get_state());

led.toggle();
print('4. Current Relay state is (must bee off):', led.get_state());

led.touch(5000);
print('5. Current Relay state is (must bee on, but after 5 sec. off):', led.get_state());

Timer.set(10000, Timer.REPEAT, function() {
    let state = led.get_state();
    print('6. Current Relay state is (must bee off) :', led);
    led.delete();
}, null);