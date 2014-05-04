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

   - [x] 4.2 - Infinite (non-persistent) map

   - [x] 4.3 - Smooth map scrolling

 - [ ] 5 - Saving & Loading, read map from a file, config files

   - [x] 5.1 - Convert to SDL 2

   - [x] 5.2 - Sprite system

   - [ ] 5.3 - Read key mappings from a file

   - [ ] 5.4 - Show commands for current mode

   - [ ] 5.5 - Clean up message list stuff (cap size, use utlist)

   - [ ] 5.6 - Basic player object - Turn counter & distance counter

   - [ ] 5.7 - Show something in the status window in normal mode

   - [ ] 5.8 - Isolate SDL logic - basically, there should be no .h files that import it

   - [ ] 5.9 - Save file.  For now, just save the player object (turns & distance)

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

Game concept
------------

Kookooligit is a sci-fi (or perhaps more accurately space opera - there's
technology is the point) rougelike.  The idea is very roughly inspired by
Greg Bear's novel Eon, which (**Spoiler Alert from 1985**) involves a
space-time anomaly in the form of an infinitely long corridor some kilometers
wide extending off beyond the end of the universe in some as yet unnamed
direction.  I'm doing a bad job of describing it; if catstrophic cold war
politcs aren't a trigger point for you, go read the novel, it's good.

That said, in Kookooligit, the player has entered such a corridor at a random
place.  It twists and winds away in either direction in some non-euclidian
way.  For our purposes, that means the player can't backtrack very far, which
we're modeling by capping how many "frames" of map are saved at a given time.
As of this point, there's no reason for why the player is in this situation
or what their objective is.  I have a few directions I might go in with that,
but I'm waiting till I get further in before making a decision.  For now,
it's a pure exploration motive.

I want to emphisize ranged combat, and in particular not limit the player to
eight directional fire for weapons.  I'm imagining something where the player
picks a specific enemy to fire at, or potentially just a random point on the
map.  When I get to step 6 on the above list, I'll prototype that and
hopefully it'll be clearer.

The other centeral concept is the skill system.  Rather than being attribute
focused like, e.g., nethack, I want to focus on skills the PC learns during
the game.  These should impact lots of things - e.g. instead of having an
identify spell, you id things based on how skilled you are with the relevant
technology.  There will need to be a good system for defaulting or failing
over a skill you don't have to one you do.  I'm also thinking auto-leveling
through use.  I might include some kind of "perks system" for players to have
something to pick as they level, to give a bit more direction.

Toolchain
---------

**language** Classic C, like God intended

**operating system** OS X for now, since that's what I have.  Cross compliers later.

**source control** Git & github

**doucmentation** Currently, basically just inline comments.  Eventually - RST via Sphinx, probably need doxygen & possibly breathe later http://stackoverflow.com/questions/11246637/using-sphinx-apidoc-to-generate-documentation-from-c-code

**other libraries** 

  * SDL, SDL_ttf http://www.libsdl.org/index.php  http://www.sdltutorials.com/sdl-ttf
  * lib_fov
  * mtwist - http://www.cs.hmc.edu/~geoff/mtwist.html

**compiler** clang, make, etc

**task tracking** flat file for now (you're looking at it)


Design Choices
==============

Randomized Map
--------------

I've decided to embrace the random map design.  This is in part a technical
choice, so I don't need to deal with saving maps (they can get pretty big!)
and partly a design choice.  It fits with the feel of the setting, such as it
is.
