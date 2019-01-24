// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#ifdef _WIN32
#include <Ws2tcpip.h>
#include <winsock2.h>
#include <tchar.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <assert.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <cstdint>

