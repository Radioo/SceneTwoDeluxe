#include "pch.h"
#include "ParseINI.h"


// Create a map of scene enum to scene name
std::map<Scene, std::string> CreateSceneMap(boost::property_tree::ptree& pt)
{
	std::map<Scene, std::string> sceneMap;
	sceneMap[Scene::IIDX_SP_STAGE] = pt.get<std::string>("IIDX.sp-stage");
	sceneMap[Scene::IIDX_DP_STAGE] = pt.get<std::string>("IIDX.dp-stage");
	sceneMap[Scene::IIDX_RESULT] = pt.get<std::string>("IIDX.result");
	sceneMap[Scene::IIDX_MUSIC_SELECT] = pt.get<std::string>("IIDX.music-select");
	sceneMap[Scene::IIDX_MONITOR_CHECK] = pt.get<std::string>("IIDX.monitor-check");
	sceneMap[Scene::IIDX_ATTRACT] = pt.get<std::string>("IIDX.attract-screen");
	sceneMap[Scene::IIDX_TITLE] = pt.get<std::string>("IIDX.title-screen");
	sceneMap[Scene::IIDX_CARD_IN] = pt.get<std::string>("IIDX.card-in");
	sceneMap[Scene::IIDX_MODE_SELECT] = pt.get<std::string>("IIDX.mode-select");
	sceneMap[Scene::IIDX_TEST_MENU] = pt.get<std::string>("IIDX.test-menu");
	sceneMap[Scene::IIDX_DAN_SELECT] = pt.get<std::string>("IIDX.dan-select");
	sceneMap[Scene::IIDX_DAN_RESULT] = pt.get<std::string>("IIDX.dan-result");
	sceneMap[Scene::IIDX_CARD_OUT] = pt.get<std::string>("IIDX.card-out");
	sceneMap[Scene::IIDX_ARENA_LOBBY] = pt.get<std::string>("IIDX.arena-lobby");
	sceneMap[Scene::IIDX_ARENA_BEFORE_SONG] = pt.get<std::string>("IIDX.arena-before-song");
	sceneMap[Scene::IIDX_ARENA_PODIUM] = pt.get<std::string>("IIDX.arena-podium");
	sceneMap[Scene::IIDX_BPL_LOBBY] = pt.get<std::string>("IIDX.bpl-lobby");
	sceneMap[Scene::IIDX_BPL_BEFORE_SONG] = pt.get<std::string>("IIDX.bpl-before-song");
	sceneMap[Scene::IIDX_BPL_PODIUM] = pt.get<std::string>("IIDX.bpl-podium");
	sceneMap[Scene::IIDX_DJVIP_PASS_SELECT] = pt.get<std::string>("IIDX.djvip-pass-select");
	sceneMap[Scene::IIDX_ATTRACT_TUTORIAL] = pt.get<std::string>("IIDX.attract-tutorial");
	sceneMap[Scene::IIDX_DEMO_PLAY] = pt.get<std::string>("IIDX.demo-play");
	sceneMap[Scene::IIDX_EXPERT_SELECT] = pt.get<std::string>("IIDX.expert-select");
	sceneMap[Scene::IIDX_EXPERT_RESULT] = pt.get<std::string>("IIDX.expert-result");
	sceneMap[Scene::SDVX_MUSIC_SELECT] = pt.get<std::string>("SDVX.music-select");
	sceneMap[Scene::SDVX_MUSIC_SELECT_DELAY] = pt.get<std::string>("SDVX.music-select-delay");
	sceneMap[Scene::SDVX_STAGE] = pt.get<std::string>("SDVX.stage");
	sceneMap[Scene::SDVX_STAGE_DELAY] = pt.get<std::string>("SDVX.stage-delay");
	sceneMap[Scene::SDVX_RESULT] = pt.get<std::string>("SDVX.result-screen");
	sceneMap[Scene::SDVX_RESULT_DELAY] = pt.get<std::string>("SDVX.result-screen-delay");
	sceneMap[Scene::SDVX_ATTRACT] = pt.get<std::string>("SDVX.attract-screen");
	sceneMap[Scene::SDVX_TITLE] = pt.get<std::string>("SDVX.title-screen");
	sceneMap[Scene::SDVX_TEST_MENU] = pt.get<std::string>("SDVX.test-menu");
	sceneMap[Scene::SDVX_COURSE_SELECT] = pt.get<std::string>("SDVX.course-select");
	sceneMap[Scene::SDVX_COURSE_RESULT] = pt.get<std::string>("SDVX.course-result");
	return sceneMap;
}

// Checks whether the .ini contains all the required keys
void AssertINI(boost::property_tree::ptree& pt)
{
	if (pt.get<std::string>("Main.obs-address") == "")
	{
		throw std::runtime_error("Main.obs-address is not set.");
	}
	
	if (pt.get<std::string>("Main.enable-debug") != "true" && pt.get<std::string>("Main.enable-debug") != "false")
	{
		throw std::runtime_error("Main.enable-debug is not set, must be true or false.");
	}	
	
	std::stoi(pt.get<std::string>("SDVX.music-select-delay"));
	std::stoi(pt.get<std::string>("SDVX.stage-delay"));
	std::stoi(pt.get<std::string>("SDVX.result-screen-delay"));
}
