// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include "minhook.h"
#include <iostream>
#include <vector>
#include <Psapi.h>
#include <winver.h>
#include <strsafe.h>
#include <atlstr.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include <fstream>
#include <regex>
#include <ctime>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#endif //PCH_H
