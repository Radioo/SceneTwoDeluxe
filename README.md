# SceneTwoDeluxe
Automatic OBS scene switcher for IIDX and SDVX.

## Usage
Install https://github.com/obsproject/obs-websocket (version 4.x.x).  
Disable obs websocket authentication: Tools -> WebSockets Server Settings (I can implement it if people really need it, but just couldn't be bothered this time).  
Extract `SceneTwoDeluxe.dll` and `SceneTwoDeluxe.json` to your game's directory.  
Edit the `.json` to configure behavior:  
  Change `"obs-address"` if you are running the game on a separate PC.
  Then input your desired scenes to the fileds below. The first four are the most common ones you'll be using, but I've included the rest that I've discovered while reversing. Leaving them empty will not trigger any scene change.  
Inject the `.dll` using your preferred toolset, for automatic injection to INFINITAS use: [infinitas_chainload](https://github.com/emskye96/infinitas_chainload), for SDVX6 コナステ use: [konasute_chainload](https://github.com/Radioo/konasute_chainload).  
You can see various debug messages in the cmd log if somehing goes wrong (will probably add an option to disable them).  
The client will attempt to reconnect if a connection is lost.

## Supported games
`IIDX25 (2018091900)`  
`IIDX28 (2021091500)` 
  
`SDVX6 (2021121400)`  
`SDVX6 (2022042500)`  
`SDVX6 (2022052400)`  

`SDVX6 コナステ (2022042600)`  
`SDVX6 コナステ (2022053103)`  
