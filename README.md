Glinde 3D engine
================
This project aims at creating a 3D engine for modern OpenGL. The target genre is classic FPS games, rendered with modern techniques. The current design filosophy is

 * Glinde should be free software. The license is set to GPL version 3+.
 * Glinde should be the appropriate for quick-start indy developent, but should also aim at professional qualities.
 * The focus should be qualtiy over quantity. This means
   - The framerate is capped at a certain rate. The current refresh rate is 30 fps.
   - If you want to do something like `nuts.wad`, choose another engine
 
Target platform and APIs
------------------------
The target platform is currently OpenGL 4.3, and ARB_DirectStateAccess. Later on in the development process, these requirements may be pushed forward. HUDs and menus will be drawn by the Cairo and Pango stack. This gives the opportunity to use TrueTypes directly, without needing a huge charmap. Also, there is full Unicode out the box. The audio system will use JACK as backend. The current source code is GNU/Linux only, but the amount of platform specific code is kept at a minimum, so there should be no trouble in porting to other platforms.

Special features
----------------
This is a non-exhaustive list of special features that I want to put into Glinde.

 * MIDI input and output for other purpose than music. This allows an external MIDI sequencer to record a demo, which can be edited afterwards. In demo mode, the engine can be synchornized with a music track, giving the opportunity for using the engine to create animated music videos.
 * Freewheeling mode. This mode will replace the game timer with a dummy timer that only claims that it waits. This is useful for benchmarks, but needs to be activated when an external sequencer drives the game.
