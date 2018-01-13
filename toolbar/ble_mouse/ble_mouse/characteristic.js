var util = require('util');

var bleno = require('bleno');

var robot = require('robotjs');

// Speed up the mouse.
robot.setMouseDelay(2);
var screenSize = robot.getScreenSize();
var height = screenSize.height;
var width = screenSize.width;

var BlenoCharacteristic = bleno.Characteristic;

var EchoCharacteristic = function () {
    EchoCharacteristic.super_.call(this, {
        uuid: 'ec0e',
        properties: ['read', 'write', 'notify'],
        value: null
    });
    this.x = 0;
    this.y = 0;
    this.left = 0;
    this.right = 0;
    this._value = new Buffer(0);
    this._updateValueCallback = null;
};

util.inherits(EchoCharacteristic, BlenoCharacteristic);

EchoCharacteristic.prototype.onReadRequest = function (offset, callback) {
    console.log('EchoCharacteristic - onReadRequest: value = ' + this._value.toString('hex'));

    callback(this.RESULT_SUCCESS, this._value);
};

EchoCharacteristic.prototype.onWriteRequest = function (data, offset, withoutResponse, callback) {
    this._value = data.toString('ascii');
    this.x = parseInt(this._value.slice(0, 3), 10) / 1000 * width;
    this.y = parseInt(this._value.slice(3, 6), 10) / 1000 * height;
    this.left = parseInt(this._value.slice(6, 8), 10);
    this.right = parseInt(this._value.slice(8, 10), 10);
    console.log('x = ', this.x)
    console.log('y = ', this.y)
    console.log('EchoCharacteristic - onWriteRequest: value = ' + this._value);

    robot.moveMouseSmooth(this.x, this.y);
    if (this.left == 49) {
        robot.mouseClick(button = "left");
    }
    if (this.right == 49) {
        robot.mouseClick(button = "right");
    }

    if (this._updateValueCallback) {
        console.log('EchoCharacteristic - onWriteRequest: notifying');

        this._updateValueCallback(this._value);
    }

    callback(this.RESULT_SUCCESS);
};

EchoCharacteristic.prototype.onSubscribe = function (maxValueSize, updateValueCallback) {
    console.log('EchoCharacteristic - onSubscribe');

    this._updateValueCallback = updateValueCallback;
};

EchoCharacteristic.prototype.onUnsubscribe = function () {
    console.log('EchoCharacteristic - onUnsubscribe');

    this._updateValueCallback = null;
};

module.exports = EchoCharacteristic;