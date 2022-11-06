/* 
 * Debug on and off enables code optimization
 * DEBUG 0 will ignore all Serial.print functions
 * DEBUG 1 will will add back Serial.print functions back to the code
 */

#if (ARDUINO >= 100) 
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define DEBUG 0
#define MOTORDEBUG 0

#if MOTORDEBUG == 1
DEBUG == 0
#endif

#if DEBUG == 1
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x) 
#endif
