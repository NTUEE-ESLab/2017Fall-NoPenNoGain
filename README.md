# NoPenNoGain
This is a project demonstrates how to use a laser pointer to control the cursor on the projection screen.

## Introduction
![](https://user-images.githubusercontent.com/30167968/34890525-3d99920c-f80d-11e7-813d-e287ed6db3d7.png)
![](https://user-images.githubusercontent.com/30167968/34890783-33fae5b0-f80e-11e7-9c35-7223a3d31df5.png)
![](https://user-images.githubusercontent.com/30167968/34890783-33fae5b0-f80e-11e7-9c35-7223a3d31df5.png | width = 450)
### Scenario
Most of the time, like in a class or meeting, the most comment and convenient way to present is to project the screen of the speecher's computer(or some other devices like iPad) to a projection screen and using a mouse or touching on the host computer(or a pad) to control the cursor. Often, there's a need for writing on the screen. An obvious limitation for presenting while writing on the screen is that you need to stay in front of the computer's screen. Now, imagine that you can just use a laser pointer, walk around the stage, present with gestures, and control the cursor with the pointer right in your hand! And that what NoPenNoGain want to do for you.
### Description
The main part of this project is an Raspberry Pi with a camera, detecting the light point in the screen, and returning the position to the host computer. There's another Raspberry Pi with two button binded with the laser pointer. The second Raspberry Pi return the right or left click of a mouse to the fisrt Pi for it to pack the position and click information into a package to transfer to the host computer. All the communications mentioned aboved are Bluetooth Low Energy(ble). 
## Requirements
Requires a [***Raspberry Pi computer***](https://www.raspberrypi.org/documentation/setup/) and a 
[***RPI camera module installed***](https://www.raspberrypi.org/documentation/usage/camera/).
 
**IMPORTANT** - It is suggested you do a Raspbian ***sudo apt-get update*** and ***sudo apt-get upgrade***
before curl install, since it is **No longer** performed by the pi-timolo-install.sh script

***Step 1*** With mouse left button highlight curl command in code box below. Right click mouse in **highlighted** area and Copy.     
***Step 2*** On RPI putty SSH or terminal session right click, select paste then Enter to download and run script.     

    curl -L https://raw.github.com/pageauc/pi-timolo/master/source/pi-timolo-install.sh | bash

The command above will download and Run the GitHub ***pi-timolo-install.sh*** script. 
An upgrade will not overwrite configuration files.   

* ***NOTICE*** gdrive is no longer installed with pi-timolo-install.sh, I have been testing
rclone and it is Now the Default. Some ***rclone-*** samples are included. Make a copy of one, rename and edit for
your own needs.  See [Wiki - How to Setup Rclone](https://github.com/pageauc/pi-timolo/wiki/How-to-Setup-rclone).
Note: If a ***/usr/local/bin/gdrive*** File Exists, It Will Remain. Older files are still available on this GitHub Repo.   



![menubox main menu](menubox.png)
 


![webserver browser screen shot](webserver.png)
 
## Reference Links  
Detailed pi-timolo Wiki https://github.com/pageauc/pi-timolo/wiki  
YouTube Videos https://www.youtube.com/playlist?list=PLLXJw_uJtQLa11A4qjVpn2D2T0pgfaSG0
 
