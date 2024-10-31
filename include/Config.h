#ifndef NEXUS_CONFIG_H
#define NEXUS_CONFIG_H

//--------------------Type-------------------//

// Type of drone
// Fixed FIXED WING PLANE -> 0
// QUADCOPTER -> 1
// VTOL -> 2
// FW-v_tail -> 3
#define NX_UAV_TYPE 0


//--------------------SETUP---------------------//

// Micro controller board TYPE
// Arduino  -> 0
// Arduino Nano -> 1
// Arduino Nano -> 2
#define CONTROLLER_BOARD 1

#define RUDDER_CONTROL 1            // for nexus plane 

#define PRT1 3
#define PRT2 5
#define PRT3 6
#define PRT4 9
#define PRT5 10
#define PRT6 11

// RECIEVER CHANNEL AND PORT MAP
#define CH1 A0
#define CH2 A1
#define CH3 A2
#define CH4 A3

#define CH1_REV 0
#define CH2_REV 0
#define CH3_REV 0
#define CH4_REV 0
#define CH5_REV 0
#define CH6_REV 0
#define CH7_REV 0
#define CH8_REV 0


#endif



