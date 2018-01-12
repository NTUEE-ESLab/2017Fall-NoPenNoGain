// create an instance of the rpio-gpio-buttons object with pins 11 and 13 
var buttons = require('rpi-gpio-buttons')([12, 32]);

var bleno = require('../..');

var BlenoPrimaryService = bleno.PrimaryService;

//var EchoCharacteristic = require('./characteristic');

var left = '0';

var right = '0';

var util = require('util');

var BlenoCharacteristic = bleno.Characteristic;

var EchoCharacteristic = function() {
  EchoCharacteristic.super_.call(this, {
    uuid: 'ec0e',
    properties: ['read', 'write', 'notify'],
    value: null
  });

  this._value = new Buffer(0);
  this._updateValueCallback = null;
};

util.inherits(EchoCharacteristic, BlenoCharacteristic);

EchoCharacteristic.prototype.onReadRequest = function(offset, callback) {
  console.log('EchoCharacteristic - onReadRequest: value = ' + this._value.toString('hex'));
  this._value = new Buffer(left + right);
  callback(this.RESULT_SUCCESS,this._value );
};

EchoCharacteristic.prototype.onWriteRequest = function(data, offset, withoutResponse, callback) {
  this._value = data;

  console.log('EchoCharacteristic - onWriteRequest: value = ' + this._value.toString('hex'));

  if (this._updateValueCallback) {
    console.log('EchoCharacteristic - onWriteRequest: notifying');

    this._updateValueCallback(this._value);
  }

  callback(this.RESULT_SUCCESS);
};

EchoCharacteristic.prototype.onSubscribe = function(maxValueSize, updateValueCallback) {
  console.log('EchoCharacteristic - onSubscribe');

  this._updateValueCallback = updateValueCallback;
};

EchoCharacteristic.prototype.onUnsubscribe = function() {
  console.log('EchoCharacteristic - onUnsubscribe');

  this._updateValueCallback = null;
};



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
