# ApolloBVM #
### Rice University Oshman Engineering Design Kitchen ###
###### Author: Thomas Herring ######

![Build Status](https://github.com/apollobvm/apollobvm/workflows/build/badge.svg)
<!--[![Build Status](https://github.com/apollobvm/apollobvm/workflows/test/badge.svg)](https://github.com/arduino/arduino-cli-example/actions)-->

This repository contains all the working code for both the master and slave Arduino to build the ApolloBVM. Feel free to fork it for your own development, we encourage collaboration! All mechanical parts and assembly instructions can be found [here](http://oedk.rice.edu/apollobvm).

For those looking to just download and run the code, please refer to the [releases](https://github.com/apollobvm/apollobvm/releases) section and downloaded the latest firmware. Otherwise, please follow the instructions below on install

For any issues with the code, electrical setup, or mechanical components, please file a bug report [here](https://github.com/apollobvm/apollobvm/issues).

## Installation Instructions ##
1. Download the latest Arduino IDE from their website [here](https://www.arduino.cc/en/Main/Software).
2. Download the latest posted release of the device firmware [here](https://github.com/apollobvm/apollobvm/releases), it should look something like "apollobvm_X.X.X.zip".
3. Unzip the downloaded release.
3.  Add the ApolloBVM library to the Arduino IDE:
    1. In the Arduino IDE, go to Sketch->Include Library->Add .ZIP Library...
    2. Navigate to and click on the library zip or library directory inside the extracted release. This will be a file that looks like "ApolloBVM.zip" or a directory with the name "ApolloBVM".
4. Add ApolloBVM library dependencies.
    1. In the Arduino IDE, go to Tools->Manage Libraries...
    2. In the Library Manager, type "Encoder" into the search box.
    3. Scroll down and locate the "Encoder by Paul Stoffregen" library and then click install.
5. Open the slave and master code.
    1. In the Arduino IDE, go to File->Open...
    2. Navigate to and click on the slave.ino file inside the extracted release.
    3. Repeat this process for the master.ino file.
6. Flash the Arduino boards with the firmware.
    1. Plug the first Arduino board into your computer.
    2. In the Arduino IDE, go to Tools->Board and select the Arduino board you are using.
    3. Next, in the Arduino IDE, go to Tools->Port and select the port for your Arduino board.
    4. Push the arrow button to upload code on the Arduino IDE window with the slave code.
    5. Unplug the first Arduino, plug in the second, and repeat the above step for the master code.
7. Follow the wiring diagram on the [ApolloBVM website](http://oedk.rice.edu/apollobvm) to complete assembly.
    

