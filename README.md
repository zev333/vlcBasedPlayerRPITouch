#VLC based player for Raspberry Pi with touch screen

This player is done to use rapsbeery as a music player. It has simple UI, that can be used on touch display. This player will automcatically add all media files form "/home/zev/Music". Media file location can be changed in player.cpp file: "#define MUSIC_DIR ...".

Tested on Raspberry configuration: Raspberry Pi 3 Model B+ 3.5 inch Touchscreen LCD

Player hax next view:
|  | 0 | 1 | 2 | 3 |  
| --- | --- | --- | --- | --- |
| 0 | Previous song | Normalize + show curr. song place and full duration | Next song | Louder |
| 1 | Jump back by 10% | Stop/Play | Jump forward by 10% | Quiet |
| 2 | Make song slower by rate 0.3 | To start | Make song faster by rate 0.3 | Cycle type |
