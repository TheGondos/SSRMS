#pragma once
#include <string>
#include "Mission/Mission.h"

bool __cdecl ssuInit();
bool __cdecl ssuExit();
extern "C++" mission::Mission*  __cdecl ssuGetMission(const std::string& filename);