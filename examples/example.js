load("api_relay.js");
load("api_timer.js");


let relay = Relay.create(2, Relay.NORMALLY_OPEN);

let state = relay.on();
print('1. Current Relay state is (must bee on):', state);

state = relay.off();
print('2. Current Relay state is (must bee off):', state);

state = relay.toggle();
print('3. Current Relay state is (must bee on):', state);

state = relay.toggle();
print('4. Current Relay state is (must bee off):', state);

state = relay.touch(5000);
print('5. Current Relay state is (must bee on, but after 5 sec. off):', state);

Timer.set(10000, Timer.REPEAT, function() {
    state = relay.state();
    print('6. Current Relay state is :', state);
    relay.close();
}, null);