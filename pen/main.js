// create an instance of the rpio-gpio-buttons object with pins 11 and 13 
var buttons = require('rpi-gpio-buttons')([12, 32]);

var bleno = require('../..');

var BlenoPrimaryService = bleno.PrimaryService;

var EchoCharacteristic = require('./characteristic');

var left = '0';

var right = '0';

console.log('bleno - echo');

bleno.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === 'poweredOn') {
    bleno.startAdvertising('echo', ['ec00']);
  } else {
    bleno.stopAdvertising();
  }
});

bleno.on('advertisingStart', function(error) {
  console.log('on -> advertisingStart: ' + (error ? 'error ' + error : 'success'));

  if (!error) {
    bleno.setServices([
      new BlenoPrimaryService({
        uuid: 'ec00',
        characteristics: [
          new EchoCharacteristic()
        ]
      })
    ]);
  }
});

// bind to the clicked event and check for the assigned pins when clicked 
buttons.on('pressed', function (pin) {
  switch(pin) {
    // Up button on pin 11 was clicked 
    case 12:
    left = '1';
    //EchoCharacteristic._value = new Buffer(left + right);
    console.log("leftpressed");
    break;
 
    // Down button on pin 13 was clicked 
    case 32:
    right = '1';
    //EchoCharacteristic._value = new Buffer(left + right);
    console.log("rightpressed");
    break;
  }
});

buttons.on('released', function (pin) {
  console.log("release");
  switch(pin) {
    // Up button on pin 11 was clicked 
    case 12:
    left = '0';
    EchoCharacteristic._value = new Buffer(left + right);
    console.log("leftreleased");
    break;
 
    // Down button on pin 13 was clicked 
    case 32:
    right = '0';
    EchoCharacteristic._value = new Buffer(left + right);
    console.log("rightreleased");
    break;
  }
});
 
 
function userClickedUp() {
  // do something here for up button 
  console.log('UP');
}
 
 
function userClickedDown() {
  // do something here for down button 
  console.log('DOWN');
}
