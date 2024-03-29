# Wizard Apprentice

This is a clone of Wizard Apprentice - a game released by Fantasia for RISC OS systems in 1997 - using the original assets.

Play through 100 levels spanning 5 worlds as you progress through your quest to become a true wizard. Your goal is simply to pick up all of the collectables in each level by walking sideways into them. Sounds easy? Be warned - the unwise will find themselves trapped in no time...


![Screenshot 0](screenshots/00.png)
![Screenshot 1](screenshots/01.png)
![Screenshot 2](screenshots/02.png)
![Screenshot 3](screenshots/03.png)
![Screenshot 4](screenshots/04.png)

The game was designed primarily to run on Linux, but also runs well on macOS. On both systems, it will create a save file at ~/.wizapp_save. The code abstracts all SDL and platform-specific logic such that in theory, it would require minimal effort to adapt to other platforms.

## Controls
* Move character: Arrow keys, or the original bindings Z/X (left / right) and P/L (up / down)
* Switch character: Space
* Reset level: R
* Quit to Menu: Q
* Shift: Hold to increase game speed (be careful - one false move and you may well be hitting restart!)
* Press Space to exit the intro sequence

## Linux build dependencies
* libsdl2-dev
* libsdl2-image-dev
* libsdl2-mixer-dev
* libsdl2-ttf-dev

## MacOS build dependencies
* Install [Homebrew](https://brew.sh/)
* `brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf`

## Credits
* Music and SFX - Bill Kotsias
* Graphics - Bill Kotsias and Spyros Vergos
* New engine code - Tom Pickering

All content under the 'assets' subdirectory are copyright Bill Kotsias, and are included in this GPLv3-licensed package with his kind permission.

## Differeces from the original (incomplete list)
* Runs in windowed mode, no vertical scrolling!
* Music is not restarted between levels which use the same track
* No in-game menu - replaced with single key presses
* No level complete screen (yet)
* Not all levels in a given world are unlocked at the same time
* No level editor (yet) or other main menu functions, however level files are easy to create
* You can hold Shift to increase level speed

## Level file key
* . - Empty space
* X - Static block
* B - Pushable block
* b - Breakble block
* c - Collectable (non-flying)
* C - Collectable (flying)
* l - Ladder
* w - Wizard start point
* W - Wizard start point with ladder
* o - Blob start point
* O - Blob start point with ladder

Passing a level file as a command-line argument will launch directly into that level. The game will currently only draw levels of size 11x8 correctly, however in theory the game engine will handle any size...
