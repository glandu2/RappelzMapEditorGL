**Rappelz 3D Map Viewer**

Here is a a 3D map viewer which allows viewing a map (nfm files).

After having opened a map (button "Open map"), you can move inside with these keys:

*   E/Up arrow : foward
*   D/Down arrow : backwark
*   S/Left arrow : left
*   F/Right arrow : right
*   C : down
*   Spacebar : up
*   Shift : camera speed x10
*   Right mouse click : change camera angle (like official sframe, hold right click and then drag the mouse)

ESDF keys is used instead of WASD / ZQSD as there is no configuration stuff ATM and ESDF keys are always at the same place on the keyboard.

It's also possible to change directly coords to change camera position.

X, Y and Z values are in game units, Pitch and Yaw are in degrees, MovSpd is in game mov.spd. stat unit (as in character window)

1 MovSpd = 0,476 coord unit / sec.

I propose several attachment as maybe not all files are needed, and it might be slow to download (especially maps and textures).

With all files, you need to have that:

- RappelzMapEditorGL.exe
- vertex.glsl
- fragment.glsl
- terraintextureinfo.cfg
- QtCore4.dll
- QtGui4.dll
- QtOpenGL4.dll
- textures\ (folder with all DDS files mentioned by terraintextureinfo.cfg)

Map files can be anywhere.

Using the Ogre library ended with 27 FPS when viewing a map completely on the screen (being above it). Beside, these numbers are for linux, on windows the max FPS were 5 FPS with same hardware. So it was unusable.

Now using directly opengl, which give 3x more FPS on linux, and same FPS on windows than on linux.

The max FPS is set to 60, to avoid using too much GPU usage.

This tool require opengl 3.0 support, which can be mapped to directx 10 compatible hardware.
