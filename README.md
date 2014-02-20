Kookooligit
===========

Kookooligit is a volcano in Alaska.  It has nothing to do with this project.

http://en.wikipedia.org/wiki/Kookooligit_Mountains

Operation "Let's get drunk and build a rougelike"
=================================================

Following http://roguebasin.roguelikedevelopment.org/index.php?title=How_to_Write_a_Roguelike_in_15_Steps
here's the initial master TODO list:

 - [x] 1 - Decide to write a game (Completed 2013-03-15)

 - [x] 2 - Provision an environment and write Hello World (Completed 2013-03-16)

 - [x] 3 - "@" walking around demo, screen layout, keyboard input, message printing (Completed 2013-03-24)

 - [x] 4 - Map display, wall detection, look command
 
 - [x] 4.1 - Basic map generation

 - [ ] 4.2 - Infinite (non-persistent) map

 - [ ] 4.3 - Smooth map scrolling

 - [ ] 5 - Saving & Loading, read map from a file, config files

 - [ ] 6 - Monsters & Time, one monster moving around, dumb AI, turns, time

 - [ ] 7 - Interaction, stats, basic combat, Pac-man level AI (move towards the player)

 - [ ] 8 - Data files for creatures, maps, etc

 - [ ] 9 - Items, item stats, inventory system, stacking, containers

 - [ ] 10 - Magic, item effects, monster specials, etc

 - [ ] 11 - Simple hard coded game, playtest, show it to people

 - [ ] 12 - Levels, map generation, saving inactive levels, monster & item placement

 - [ ] 13 - Experience, character classes, skills, races, advancement

 - [ ] 14 - Citizens, NPCs, shops, non-combat interactions

 - [ ] 15 - Cool and unique features, congratulations you have a game


Toolchain
:::::::::

**language** Classic C, like God intended

**operating system** OS X for now, since that's what I have.  Cross compliers later.

**source control** Git & github

**doucmentation** RST via Sphinx, probably need doxygen & possibly breathe later http://stackoverflow.com/questions/11246637/using-sphinx-apidoc-to-generate-documentation-from-c-code

**other libraries** 

  * SDL, SDL_ttf http://www.libsdl.org/index.php  http://www.sdltutorials.com/sdl-ttf
  * lib_fov
  * mtwist - http://www.cs.hmc.edu/~geoff/mtwist.html

**compiler** clang, make, etc

**task tracking** flat file for now (you're looking at it)
