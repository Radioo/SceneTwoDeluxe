#pragma once
#include "pch.h"

void RetryConection();

void SendSwitchScene(std::string sceneName, std::string delayMs);

void SendSwitchScene(std::string sceneName);

void PerformSend(std::string sceneName);

void PerformSendWithDelay(std::string sceneName, std::string delayMs);

void RunServer(std::string uri);
