// create an instance of the rpio-gpio-buttons object with pins 11 and 13 
var buttons = require('rpi-gpio-buttons')([11, 13]);

var bleno = require('../..');

var BlenoPrimaryService = bleno.PrimaryService;

var EchoCharacteristic = require('./characteristic');

var right = '0'

var left = '0'

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
    case 11:
    left = '1';
    EchoCharacteristic._value = Buffer(left + right);
    break;
 
    // Down button on pin 13 was clicked 
    case 13:
    left = '1';
    EchoCharacteristic._value = Buffer(left + right);
    break;
  }
});

buttons.on('released', function (pin) {
  switch(pin) {
    // Up button on pin 11 was clicked 
    case 11:
    left = '0';
    EchoCharacteristic._value = Buffer(left + right);
    break;
 
    // Down button on pin 13 was clicked 
    case 13:
    right = '0';
    EchoCharacteristic._value = Buffer(left + right);
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
