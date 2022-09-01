#pragma once
#include "pch.h"

void on_open(websocketpp::connection_hdl hdl);

void RetryConection();

void SendSwitchScene(std::string sceneName, std::string delayMs);

void SendSwitchScene(std::string sceneName);

void PerformSend(std::string sceneName);

void Identify();

void PerformSendWithDelay(std::string sceneName, std::string delayMs);

void RunServer(std::string uri);
