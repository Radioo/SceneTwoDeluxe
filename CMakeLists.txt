cmake_minimum_required(VERSION 3.23)
project(SceneTwoDeluxe)

set(CMAKE_CXX_STANDARD 14)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

include_directories(
        SceneTwoDeluxe,
        "C:\\Users\\Radio\\Repos\\websocketpp"
        "C:\\Users\\Radio\\boost\\boost_1_80_0"
        "C:\\Users\\Radio\\Repos\\minhook\\include"
        "C:\\Users\\Radio\\Repos\\json\\include"
)

link_directories(
        "C:\\Users\\Radio\\boost\\boost_1_80_0\\stage\\lib"
        "C:\\Users\\Radio\\Repos\\minhook\\build\\VC17\\lib\\Release"
)


add_library(SceneTwoDeluxe SHARED
        SceneTwoDeluxe/dllmain.cpp
        SceneTwoDeluxe/DiamondCrossing.cpp
        SceneTwoDeluxe/DiamondCrossing.h
        SceneTwoDeluxe/framework.h
        SceneTwoDeluxe/GetGameModuleInfo.cpp
        SceneTwoDeluxe/GetGameModuleInfo.h
        SceneTwoDeluxe/ParseINI.cpp
        SceneTwoDeluxe/ParseINI.h
        SceneTwoDeluxe/pch.cpp
        SceneTwoDeluxe/pch.h
        SceneTwoDeluxe/WSClient.cpp
        SceneTwoDeluxe/WSClient.h)
