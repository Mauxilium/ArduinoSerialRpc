# ArduinoSerialRpc

ArduinoSerialRpc is the Arduino side part of a serial communication library with Arduino Card.

There are some other projects to interact with this one:
* Java side: ArduinoJavaSerialRpc
* Python side: ArduinoPythonSerialRpc

The communication model is implemented in form of:
* Remote Method Invocation. Where Arduino calls a method declared into a Java class.
* Remote Procedure Call. Where a Java process call a function defined into the Arduino sketch.

The communication is a point to point model, performed through the serial (USB) port.
 
## Architecture

Tbdf

## Features

* Bidirectional communication
* Low Arduino resources required
* Easy to learn interface
* Flexible naming convention

# Getting Started

## Arduino Side Installation
It is required to:
 * download this Arduino library
 * expand it into your "library" path of Arduino Ide,
  or follows the manual installation section of https://www.arduino.cc/en/guide/libraries
  
## Other Language Side Installation
Please refer to others repositories:
* Java side: ArduinoJavaSerialRpc
* Python side: ArduinoPythonSerialRpc

## Using it in Arduino Sketch
```c++
#include <ArduinoSerialRpc.h>

// Creating the rpc agent you are totally free to assign
// your preferred name to the card
ArduinoSerialRpc rpc("MyTestCard");

void setup() {
  Serial.begin(9600); // or any other supported value

  // Other languages can use "callIt" string to force the execution of myMethod function
  rpc.registerArduinoAction("callIt", myMethod);
}

void myMethod() {
    // This method is called when the remote program
    // executes an executeRemoteMethod("callIt"); 
}

void loop() {
    // Here Arduino calls a method of the remote program program.
    // No registration is required
    rpc.executeRemoteAction("pingFromArduino");
}
```

### Next steps
* ArduinoJavaSerialRpc tutorial - A tutorial to discover a more complex use of library
* ArduinoSpring - The Spring library developed on top of ArduinoJavaSeriaRpc
* www.mauxilium.it - The reference site for my other projects
