# ArduinoSerialRpc

ArduinoSerialRpc is the Arduino side part of a serial communication library with Arduino Card.

There are some other projects used to interact with this one:
* [Java side communication library](https://github.com/Mauxilium/ArduinoJavaSerialRpc)
* [Python side communication library](https://github.com/Mauxilium/ArduinoPythonSerialRpc)

The communication model is implemented in form of:
* Remote Method Invocation. Where Arduino calls a method declared into a Java class.
* Remote Procedure Call. Where a Java process call a function defined into the Arduino sketch.

The communication is a point to point model, performed through the serial (USB) port.
 
## Architecture

tbdf

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
  
## Other Languages Installation
Please refer to other repositories:
* [Java](https://github.com/Mauxilium/ArduinoJavaSerialRpc)
* [Python](https://github.com/Mauxilium/ArduinoPythonSerialRpc)

## Using it in Arduino Sketch
```c++
#include <ArduinoSerialRpc.h>

// Creating the rpc agent you are totally free to assign
// your preferred name to the card
ArduinoSerialRpc rpc("MyTestCard");

void setup() {
  Serial.begin(9600); // or any other supported value

  // Other languages can use "callIt" string to force the execution of myMethod function
  rpc.registerArduinoFunction("callIt", myMethod);
}

void myMethod() {
    // This method is called when the remote program
    // executes an executeRemoteMethod("callIt"); 
}

void loop() {
    // Here Arduino calls a method of the remote program program.
    // No registration is required
    rpc.executeRemoteMethod("pingFromArduino");
}
```

### Next steps
* [ArduinoJavaSerialRpcTutorial](https://github.com/Mauxilium/ArduinoJavaSerialRpcTutorial) - A tutorial to discover a more complex use of library
* [ArduinoPythonSerialRpcTutorial](https://github.com/Mauxilium/ArduinoPythonSerialRpcTutorial) - A tutorial to discover a more complex use of library
* [ArduinoSpring](https://github.com/Mauxilium/ArduinoSpring) - A Spring library developed on top of ArduinoJavaSeriaRpc
* [www.mauxilium.it](http://www.mauxilium.it) - The reference site for my open source projects
