//
//   Version: 1.0
//	    Date: 04 Maj 2020
//    Author: Gabriele Maris
//   Project: www.mauxilium.it
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 
#include "Arduino.h"
#include "ArduinoSerialRpc.h"

const String ERROR_PREAMBLE = "MArC_err";
const String CMD_PREAMBLE = "MArC_cmd";
const String RESULT_PREAMBLE = "MArC_res";
const String MESSAGE_PREAMBLE = "MArC_msg";

const char VOID_ARG_PREAMBLE = 'V';
const char INT_ARG_PREAMBLE = 'I';
const char INT_INT_ARG_PREAMBLE = 'H';
const char STRING_ARG_PREAMBLE = 'S';
const char FLOAT_ARG_PREAMBLE = 'F';

const int SERIAL_TIMEOUT = 3;

// This is the logical (reference) name assigned to this Arduino card
String cardName;

//
// Service method
// It returns the logical name assigned to the Arduino card with the constructor
//
String getCardName(String ignored) {
    return cardName;
}

//
// Constructor implementation
//
ArduinoSerialRpc::ArduinoSerialRpc(String name) {
    cardName = name;
    this->registerArduinoAction("GetCardName", getCardName);
}

//
// External To Internal Setup Interface
//
void ArduinoSerialRpc::registerArduinoAction(String referenceName, void (*functionToCall)(void)) {
  void_void_function_item *newItem = new void_void_function_item();
  newItem->functionName = referenceName;
  newItem->functionToCall = functionToCall;
  newItem->nextItem = NULL;

  if (void_void_functions_list == NULL) {
    void_void_functions_list = newItem;
  } else {
    void_void_function_item *lastItem = void_void_functions_list;
    while (lastItem->nextItem != NULL) {
      lastItem = lastItem->nextItem;
    }
    lastItem->nextItem = newItem;
  }
  Serial.setTimeout(SERIAL_TIMEOUT);
}
void ArduinoSerialRpc::registerArduinoAction(String referenceName, int (*functionToCall)(int, int)) {
  int_int_int_function_item *newItem = new int_int_int_function_item();
  newItem->functionName = referenceName;
  newItem->functionToCall = functionToCall;
  newItem->nextItem = NULL;

  if (int_int_int_functions_list == NULL) {
    int_int_int_functions_list = newItem;
  } else {
    int_int_int_function_item *lastItem = int_int_int_functions_list;
    while (lastItem->nextItem != NULL) {
      lastItem = lastItem->nextItem;
    }
    lastItem->nextItem = newItem;
  }
  Serial.setTimeout(SERIAL_TIMEOUT);
}
void ArduinoSerialRpc::registerArduinoAction(String referenceName, float (*functionToCall)(float)) {
  float_float_function_item *newItem = new float_float_function_item();
  newItem->functionName = referenceName;
  newItem->functionToCall = functionToCall;
  newItem->nextItem = NULL;

  if (float_float_functions_list == NULL) {
    float_float_functions_list = newItem;
  } else {
    float_float_function_item *lastItem = float_float_functions_list;
    while (lastItem->nextItem != NULL) {
      lastItem = lastItem->nextItem;
    }
    lastItem->nextItem = newItem;
  }
  Serial.setTimeout(SERIAL_TIMEOUT);
}
void ArduinoSerialRpc::registerArduinoAction(String referenceName, String (*functionToCall)(String)) {
  string_string_function_item *newItem = new string_string_function_item();
  newItem->functionName = referenceName;
  newItem->functionToCall = functionToCall;
  newItem->nextItem = NULL;

  if (string_string_functions_list == NULL) {
    string_string_functions_list = newItem;
  } else {
    string_string_function_item *lastItem = string_string_functions_list;
    while (lastItem->nextItem != NULL) {
      lastItem = lastItem->nextItem;
    }
    lastItem->nextItem = newItem;
  }
  Serial.setTimeout(SERIAL_TIMEOUT);
}


void ArduinoSerialRpc::print(String txt) {
	Serial.println(MESSAGE_PREAMBLE);
    Serial.println(txt);
	Serial.flush();
}

//
// Handling of External Requests
//
void ArduinoSerialRpc::serialEventHandler() {
	String incomingString = Serial.readString();
	int separatorIndex = incomingString.indexOf(' ');
	String cmdString = incomingString.substring(0, separatorIndex);
	char argType = incomingString.charAt(separatorIndex+1);
	String arg = incomingString.substring(separatorIndex+2);

	switch (argType) {
      case VOID_ARG_PREAMBLE:
        doAction(cmdString);
      break;
      case INT_INT_ARG_PREAMBLE: {
	    String arg1 = arg.substring(0, arg.indexOf(','));
		String arg2 = arg.substring(arg1.length()+1);
        doAction(cmdString, (int)arg1.toInt(), (int)arg2.toInt());
	  }
      break;
      case FLOAT_ARG_PREAMBLE:
        doAction(cmdString, arg.toFloat());
      break;
	  case STRING_ARG_PREAMBLE:
        doAction(cmdString, arg);
      break;
      default:
	    Serial.println(ERROR_PREAMBLE);
        Serial.print("ERROR: Invalid signature type: '");
        Serial.print(argType);
		Serial.print("' (byte: ");
		Serial.print((byte)argType);
		Serial.print(") in command: '");
		Serial.print(cmdString);
		Serial.println("'");
		Serial.flush();
    }
}
void ArduinoSerialRpc::doAction(String requiredFunction) {
  void_void_function_item *theItem = void_void_functions_list;
  while (theItem->functionName != requiredFunction) {
    if (theItem->nextItem == NULL) {
      sendErrorOfUnknowCommand("VoidVoid", requiredFunction);
      return;
    } else {
      theItem = theItem->nextItem;
    }
  }
  (theItem->functionToCall)();
  Serial.println(RESULT_PREAMBLE);
  Serial.println(VOID_ARG_PREAMBLE);
  Serial.flush();
}
void ArduinoSerialRpc::doAction(String requiredFunction, int arg1, int arg2) {
  int_int_int_function_item *theItem = int_int_int_functions_list;
  while (theItem->functionName != requiredFunction) {
    if (theItem->nextItem == NULL) {
      sendErrorOfUnknowCommand("IntIntInt", requiredFunction);
      return;
    } else {
      theItem = theItem->nextItem;
    }
  }
  int result = (theItem->functionToCall)(arg1, arg2);
  Serial.println(RESULT_PREAMBLE);
  Serial.println(INT_ARG_PREAMBLE);
  Serial.println(result);
  Serial.flush();
}
void ArduinoSerialRpc::doAction(String requiredFunction, float arg) {
  float_float_function_item *theItem = float_float_functions_list;
  while (theItem->functionName != requiredFunction) {
    if (theItem->nextItem == NULL) {
      sendErrorOfUnknowCommand("FloatFloat", requiredFunction);
      return;
    } else {
      theItem = theItem->nextItem;
    }
  }
  float result = (theItem->functionToCall)(arg);
  Serial.println(RESULT_PREAMBLE);
  Serial.println(FLOAT_ARG_PREAMBLE);
  Serial.println(result);
  Serial.flush();
}
void ArduinoSerialRpc::doAction(String requiredFunction, String arg) {
  string_string_function_item *theItem = string_string_functions_list;
  while (theItem->functionName != requiredFunction) {
    if (theItem->nextItem == NULL) {
      sendErrorOfUnknowCommand("StringString", requiredFunction);
      return;
    } else {
      theItem = theItem->nextItem;
    }
  }
  String result = (theItem->functionToCall)(arg);
  Serial.println(RESULT_PREAMBLE);
  Serial.println(STRING_ARG_PREAMBLE);
  Serial.println(result);
  Serial.flush();
}


//
// Execution request models directed to the external program (connected to the Arduino by the way of serial cable)
//
void ArduinoSerialRpc::executeRemoteAction(String methodToCall) {
  Serial.println("");
  Serial.println(CMD_PREAMBLE);
  Serial.println(methodToCall);
  Serial.println(VOID_ARG_PREAMBLE);
  Serial.flush();
}
void ArduinoSerialRpc::executeRemoteAction(String methodToCall, int arg1, int arg2) {
  Serial.println("");
  Serial.println(CMD_PREAMBLE);
  Serial.println(methodToCall);
  Serial.println(INT_INT_ARG_PREAMBLE);
  Serial.println(arg1);
  Serial.println(arg2);
  Serial.flush();
}
void ArduinoSerialRpc::executeRemoteAction(String methodToCall, float arg1) {
  Serial.println("");
  Serial.println(CMD_PREAMBLE);
  Serial.println(methodToCall);
  Serial.println(FLOAT_ARG_PREAMBLE);
  Serial.println(arg1);
  Serial.flush();
}
void ArduinoSerialRpc::executeRemoteAction(String methodToCall, String arg1) {
  Serial.println("");
  Serial.println(CMD_PREAMBLE);
  Serial.println(methodToCall);
  Serial.println(STRING_ARG_PREAMBLE);
  Serial.println(arg1);
  Serial.flush();
}

void ArduinoSerialRpc::sendErrorOfUnknowCommand(String calledSignature, String unknowMethod) {
  Serial.println("");
  Serial.println(ERROR_PREAMBLE);
  Serial.print("Unknow command '");
  Serial.print(unknowMethod);
  Serial.print("' calling signature: ");
  Serial.println(calledSignature);
  Serial.flush();
}
