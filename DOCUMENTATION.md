# NEXUS

Nexus is a private UAV flight controller software.

## CONFIGURATION AND SETUP

Nexus is configured using a config.h file located in the ./include folder.

### Parameter

* ***NX_UAV_TYPE*** - Specifies the type of uav. 

Fixed FIXED WING PLANE -> 0
QUADCOPTER -> 1
VTOL -> 2
FW-v_tail -> 3

* ***CONTROLLER_BOARD*** - Micro controller board type
Arduino uno -> 0
Arduino Nano -> 1
Arduino Mega -> 2

* ***RUDDER_CONTROL*** - Determines whether the rudder is controlled or not. 0 means rudder is not controlled, 1 means rudder control is enabled.

