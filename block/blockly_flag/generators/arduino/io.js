/**
 * @license Licensed under the Apache License, Version 2.0 (the "License"):
 *          http://www.apache.org/licenses/LICENSE-2.0
 */

/**
 * @fileoverview Code generator for Arduino Digital and Analogue input/output.
 *     Arduino built in function docs: http://arduino.cc/en/Reference/HomePage
 */
'use strict';

goog.provide('Blockly.Arduino.IO');

goog.require('Blockly.Arduino');


/**
 * Function for 'set pin' (X) to a state (Y).
 * Arduino code: setup { pinMode(X, OUTPUT); }
 *               loop  { digitalWrite(X, Y); }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {string} Completed code.
 */
Blockly.Arduino['io_digitalwrite'] = function(block) {
  var pin = block.getFieldValue('PIN');
  var stateOutput = Blockly.Arduino.valueToCode(
      block, 'STATE', Blockly.Arduino.ORDER_ATOMIC) || 'LOW';

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.OUTPUT, 'Digital Write');

  var pinSetupCode = 'pinMode(' + pin + ', OUTPUT);';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = 'digitalWrite(' + pin + ', ' + stateOutput + ');\n';
  return code;
};

Blockly.Arduino['io_digitalwrite_var'] = function(block) {
  var pin = Blockly.Arduino.valueToCode(
      block, 'PIN', Blockly.Arduino.ORDER_ATOMIC) || '0';
  var stateOutput = Blockly.Arduino.valueToCode(
      block, 'STATE', Blockly.Arduino.ORDER_ATOMIC) || 'LOW';

  var code = 'digitalWrite(' + pin + ', ' + stateOutput + ');\n';

  pin = removeHighlightText(pin);
  if (pin.match(pinRegexp)) {
    Blockly.Arduino.reservePin(
        block, pin, Blockly.Arduino.PinTypes.OUTPUT, 'Digital Write');

    var pinSetupCode = 'pinMode(' + pin + ', OUTPUT);';
    Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

    block.prefixCode = '';
  }
  else {
    block.prefixCode = 'pinMode('+pin+', OUTPUT);\n';
  }

  return code;
};

/**
 * Function for reading a digital pin (X).
 * Arduino code: setup { pinMode(X, INPUT); }
 *               loop  { digitalRead(X)     }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {array} Completed code with order of operation.
 */
Blockly.Arduino['io_digitalread'] = function(block) {
  var pin = block.getFieldValue('PIN');

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Digital Read');

  var pinSetupCode = 'pinMode(' + pin + ', INPUT);';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = 'digitalRead(' + pin + ')';
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['io_digitalread_var'] = function(block) {
  var pin = Blockly.Arduino.valueToCode(
      block, 'PIN', Blockly.Arduino.ORDER_ATOMIC) || '0';
  
  var code = 'digitalRead(' + pin + ')';

  pin = removeHighlightText(pin);
  if (pin.match(pinRegexp)) {
    Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Digital Read');

    var pinSetupCode = 'pinMode(' + pin + ', INPUT);';
    Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

    block.prefixCode = '';
  }
  else {
    block.prefixCode = 'pinMode('+pin+', INPUT);\n';
  }

  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

/**
 * Function for setting the state (Y) of a built-in LED (X).
 * Arduino code: setup { pinMode(X, OUTPUT); }
 *               loop  { digitalWrite(X, Y); }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {string} Completed code.
 */
Blockly.Arduino['io_builtin_led'] = function(block) {
  var pin = block.getFieldValue('BUILT_IN_LED');
  var stateOutput = Blockly.Arduino.valueToCode(
      block, 'STATE', Blockly.Arduino.ORDER_ATOMIC) || 'LOW';

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.OUTPUT, 'Set LED');

  var pinSetupCode = 'pinMode(' + pin + ', OUTPUT);';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = 'digitalWrite(' + pin + ', ' + stateOutput + ');\n';
  return code;
};

/**
 * Function for setting the state (Y) of an analogue output (X).
 * Arduino code: setup { pinMode(X, OUTPUT); }
 *               loop  { analogWrite(X, Y);  }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {string} Completed code.
 */
Blockly.Arduino['io_analogwrite'] = function(block) {
  var pin = block.getFieldValue('PIN');
  var stateOutput = Blockly.Arduino.valueToCode(
      block, 'NUM', Blockly.Arduino.ORDER_ATOMIC) || '0';

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.OUTPUT, 'Analogue Write');

  var pinSetupCode = 'pinMode(' + pin + ', OUTPUT);';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  // Warn if the input value is out of range
  if ((stateOutput < 0) || (stateOutput > 255)) {
    block.setWarningText(Blockly.Msg.IO_ANALOGWRITE_WARNING,
                         'pwm_value');
  } else {
    block.setWarningText(null, 'pwm_value');
  }

  var code = 'analogWrite(' + pin + ', ' + stateOutput + ');\n';
  return code;
};

Blockly.Arduino['io_analogwrite_var'] = function(block) {
  var pin = Blockly.Arduino.valueToCode(
      block, 'PIN', Blockly.Arduino.ORDER_ATOMIC) || '0';
  var stateOutput = Blockly.Arduino.valueToCode(
      block, 'NUM', Blockly.Arduino.ORDER_ATOMIC) || '0';

  var code = 'analogWrite(' + pin + ', ' + stateOutput + ');\n';

  pin = removeHighlightText(pin);
  if (pin.match(pinRegexp)) {
    Blockly.Arduino.reservePin(
        block, pin, Blockly.Arduino.PinTypes.OUTPUT, 'Analogue Write');

    var pinSetupCode = 'pinMode(' + pin + ', OUTPUT);';
    // Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

    block.prefixCode = '';
  }
  else {
    // block.prefixCode = 'pinMode('+pin+', OUTPUT);\n';
  }

  // Warn if the input value is out of range
  // if ((stateOutput < 0) || (stateOutput > 255)) {
  //   block.setWarningText(Blockly.Msg.IO_ANALOGWRITE_WARNING,
  //                        'pwm_value');
  // } else {
  //   block.setWarningText(null, 'pwm_value');
  // }

  return code;
};

/**
 * Function for reading an analogue pin value (X).
 * Arduino code: setup { pinMode(X, INPUT); }
 *               loop  { analogRead(X)      }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {array} Completed code with order of operation.
 */
Blockly.Arduino['io_analogread'] = function(block) {
  var pin = block.getFieldValue('PIN');
  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Analogue Read');

  var pinSetupCode = 'pinMode(' + pin + ', INPUT);';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = 'analogRead(' + pin + ')';
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['io_analogread_var'] = function(block) {
  var pin = Blockly.Arduino.valueToCode(
      block, 'PIN', Blockly.Arduino.ORDER_ATOMIC) || '0';
  
  var code = 'analogRead(' + pin + ')';

  pin = removeHighlightText(pin);
  if (pin.match(pinRegexp)) {
    Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Analogue Read');

    var pinSetupCode = 'pinMode(' + pin + ', INPUT);';
    Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

    block.prefixCode = '';
  }
  else {
    block.prefixCode = 'pinMode('+pin+', INPUT);\n';
  }

  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

/**
 * Value for defining a digital pin state.
 * Arduino code: loop { HIGH / LOW }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {array} Completed code with order of operation.
 */
Blockly.Arduino['io_highlow'] = function(block) {
  var code = block.getFieldValue('STATE');
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['io_pulsein'] = function(block) {
  var pin = block.getFieldValue("PULSEPIN");
  var type = Blockly.Arduino.valueToCode(block, "PULSETYPE", Blockly.Arduino.ORDER_ATOMIC);

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Pulse Pin');

  var pinSetupCode = 'pinMode(' + pin + ', INPUT);\n';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = 'pulseIn(' + pin + ', ' + type + ')';

  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['io_pulsetimeout'] = function(block) {
  var pin = block.getFieldValue("PULSEPIN");
  var type = Blockly.Arduino.valueToCode(block, "PULSETYPE", Blockly.Arduino.ORDER_ATOMIC);
  var timeout = Blockly.Arduino.valueToCode(block, "TIMEOUT", Blockly.Arduino.ORDER_ATOMIC);

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Pulse Pin');

  var pinSetupCode = 'pinMode(' + pin + ', INPUT);\n';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = 'pulseIn(' + pin + ', ' + type + ', ' + timeout + ')';

  return [code, Blockly.Arduino.ORDER_ATOMIC];
}; 

Blockly.Arduino['io_pulsetimeout_var'] = function(block) {
  var pin = Blockly.Arduino.valueToCode(
      block, 'PIN', Blockly.Arduino.ORDER_ATOMIC) || '0';
  var type = Blockly.Arduino.valueToCode(block, "PULSETYPE", Blockly.Arduino.ORDER_ATOMIC);
  var timeout = Blockly.Arduino.valueToCode(block, "TIMEOUT", Blockly.Arduino.ORDER_ATOMIC);

  var code = 'pulseIn(' + pin + ', ' + type + ', ' + timeout + ')';

  pin = removeHighlightText(pin);
  if (pin.match(pinRegexp)) {
    Blockly.Arduino.reservePin(
        block, pin, Blockly.Arduino.PinTypes.INPUT, 'Pulse Pin');

    var pinSetupCode = 'pinMode(' + pin + ', INPUT);\n';
    Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

    block.prefixCode = '';
  }
  else {
    block.prefixCode = 'pinMode('+pin+', INPUT);\n';
  }

  return [code, Blockly.Arduino.ORDER_ATOMIC];
}; 

Blockly.Arduino['io_allpins'] = function(block) {
  var code = block.getFieldValue('PIN');
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['io_pwmpins'] = function(block) {
  var code = block.getFieldValue('PIN');
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['io_analogpins'] = function(block) {
  var code = block.getFieldValue('PIN');
  return [code, Blockly.Arduino.ORDER_ATOMIC];
};


//-------------------------------------------------------------
// added by Mee
//-------------------------------------------------------------

Blockly.Arduino['io_input_pullup'] = function(block) {
  var pin = block.getFieldValue('PIN');

  Blockly.Arduino.reservePin(
      block, pin, Blockly.Arduino.PinTypes.INPUT, 'Digital Read');

  var pinSetupCode = 'pinMode(' + pin + ', INPUT_PULLUP);';
  Blockly.Arduino.addSetup('io_' + pin, pinSetupCode, false);

  var code = '';
  return code;
};

Blockly.Arduino['io_pin_name'] = function(block) {
  var pinName = Blockly.Arduino.variableDB_.getName(
    block.getFieldValue('PIN_NAME'), 
    Blockly.Variables.NAME_TYPE/*blocklyArray_NAME_TYPE*/);
  var pin = Blockly.Arduino.valueToCode(block, "PIN", Blockly.Arduino.ORDER_ATOMIC);

  Blockly.Arduino.addVariable(pinName,
    'int ' + pinName + ' = ' + pin + ';' , true);
  //Blockly.Arduino.addDeclaration('PIN_' + pinName, serialDeclareCode);
  var code = '';
  return code;
};

Blockly.Arduino['io_set_name'] = function(block) {
  var pinName = Blockly.Arduino.variableDB_.getName(
    block.getFieldValue('PIN_NAME'), 
    Blockly.Variables.NAME_TYPE/*blocklyArray_NAME_TYPE*/);
  var pin = block.getFieldValue('PIN');

  Blockly.Arduino.addVariable(pinName,
    'int ' + pinName + ' = ' + pin + ';' , true);
  var code = '';
  return code;
};
