# SceneTwoDeluxe
Automatic OBS scene switcher for IIDX.

# Usage
Install https://github.com/obsproject/obs-websocket (version 4.x.x).  
Disable obs websocket authentication: Tools -> WebSockets Server Settings (I can implement it if people really need it, but just couldn't be bothered this time).  
Extract `SceneTwoDeluxe.dll` and `SceneTwoDeluxe.json` to your game's directory.  
Edit the `.json` to configure behavior:  
  Change `"obs-address"` if you are running the game on a separate PC.
  Then input your desired scenes to the fileds below. The first four are the most common ones you'll be using, but I've included the rest that I've discovered while reversing. Leaving them empty will not trigger any scene change.  
Inject the `.dll` using your preferred toolset.  
The client will attempt to reconnect if a connection is lost.
