#pragma once

#include "ZStringTools.h"

#include <ObjBase.h>

std::string ZGetCurrentModulePath( std::string strModule = "" );

std::string ZGetCurrentTimeString();

std::string ZGetCurrentTimeString16();

std::string ZGetNewGuidString();

std::string ZGetRandomString8();

std::string ZGetLastErrorMessageString(DWORD dwError = 0);
