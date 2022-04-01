// RUN: %check_clang_tidy %s eastwood-rule-1a %t
/* --------------------- Test Code Style Section I.A 
 * Variable names should be in all lowercase. If the name is composed of more th
 * an one word, then underscores must be used to separate them.
 * ---------------------------------------------------------------------------------------------------
 */

#include "eastwood-rule-1a.h"


// Bad: not all lowercase
int g_roomTemperature = 0;

// Bad: not all lowercase
int g_Room_Temperature = 0;

// Good
int g_room_temperature = 0;

// Bad: not all lowercase
struct RoomTemperature {
  int temperature;
};

// Good
struct room_temperature {
  int temperature;
};

// Bad: not all lowercase

/*
 * getRoomTemperature
 * @returns the temperature of the room
 */

int getRoomTemperature() {
  return g_roomTemperature;
} /* getRoomTemperature() */

// Good

/*
 * get_room_temperature
 * @returns the temperature of the room
 */

int get_room_temperature() {
  return g_room_temperature;
} /* get_room_temperature() */