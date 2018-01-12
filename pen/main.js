// create an instance of the rpio-gpio-buttons object with pins 11 and 13 
var buttons = require('rpi-gpio-buttons')([11, 13]);
 
// bind to the clicked event and check for the assigned pins when clicked 
buttons.on('clicked', function (pin) {
  switch(pin) {
    // Up button on pin 11 was clicked 
    case 11:
    userClickedUp();
    break
 
    // Down button on pin 13 was clicked 
    case 13:
    userClickedDown();
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
