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
#ifndef ArduinoSerialRpc_h
#define ArduinoSerialRpc_h

#include "Arduino.h"

class ArduinoSerialRpc
{

public:
     //
     // Constructor.
	 // The given cardName is the logical name assigned to this Arduino card,
	 // this name could be readed from a remote application by the way of getCardName() method.
     ArduinoSerialRpc(String cardName);

     //
	 // External To Internal Setup Interface
	 // The given referenceName is a logical name used by the external application to indicate which function must be executed.
	 //
	 void registerArduinoAction(String referenceName, void (*functionToCall)(void));
     void registerArduinoAction(String referenceName, int (*functionToCall)(int, int));
     void registerArduinoAction(String referenceName, float (*functionToCall)(float));     
     void registerArduinoAction(String referenceName, String (*functionToCall)(String));
     
     //
	 // Handling of Remote Calling
	 // The given methodToCall is the name of a method inside the remote application
	 //
     void executeRemoteAction(String methodToCall);
     void executeRemoteAction(String methodToCall, int arg1, int arg2);
     void executeRemoteAction(String methodToCall, float arg);
	 void executeRemoteAction(String methodToCall, String arg);
	 
	 //
	 // Utility message sender
	 // The given string will be sent to the remote application as a log text
	 //
	 void print(String txt);

	 //
	 // Internal Structure
	 // The sketch must include it in the serialEvent listener
	 //
     void serialEventHandler();
	 
private:
	struct void_void_function_item {
	   String functionName;
	   void (*functionToCall)(void);
	   void_void_function_item* nextItem;
	};
	struct int_int_int_function_item {
	   String functionName;
	   int (*functionToCall)(int, int);
	   int_int_int_function_item* nextItem;
	};
	struct float_float_function_item {
	   String functionName;
	   float (*functionToCall)(float);
	   float_float_function_item* nextItem;
	};
	struct string_string_function_item {
	   String functionName;
	   String (*functionToCall)(String);
	   string_string_function_item* nextItem;
	};

	struct void_void_function_item     *void_void_functions_list = NULL;
    struct int_int_int_function_item   *int_int_int_functions_list = NULL;
    struct float_float_function_item   *float_float_functions_list = NULL;
	struct string_string_function_item *string_string_functions_list = NULL;


    // Internal executors
    void doAction(String requiredFunction);
	void doAction(String requiredFunction, int arg1);
    void doAction(String requiredFunction, int arg1, int arg2);
    void doAction(String requiredFunction, float arg1);
	void doAction(String requiredFunction, String arg1);

    // Internal Services
    void sendErrorOfUnknowCommand(String calledSignature, String unknowMethod);
};

#endif
