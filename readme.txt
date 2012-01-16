<ESC>, Q    Exit the program. 
P           Move to predefined location where screen shot was taken
T           Start the tour, ignoring all key presses except
            E, Q and <ESC> 
E           Exit the tour mode otherwise ignored. 
<LEFT>      Turn camera view direction to left.
<RIGHT>     To the right.
<PAGE UP>   Increase the elevation of the camera
<PAGE DOWN> Decrease elevation.
<UP>        Increase the forward speed of the camera.
<DOWN>      Decrease or make the camera move more
            slowly.
R           Reset all animation
H           Display this help message
D           Dump camera postion

./main.cpp	Main code, handle rendering
./200px-Heightmap.png	Wiki's example heightmap
./screenshot.jpg	JPEG format screenshot
./PhongShader.fragmentshader	Shader for doing lighting
./Makefile	To build the project on a Linux lab machine
./images/screenshot.png	PNG format screenshot
./images/thumb.png	
./images/ss001.png
./images/thumb.xcf
./utils.cpp	Utilities such as color
./mesh.hpp	Tools to generate/load meshes (header)
./shader.hpp	Tools to compile shaders (header)
./terrian.bmp	A test terrian
./mountain.png	The terrian that is used in the scene
./libs/SOIL.h	http://www.lonesock.net/soil.html
./libs/libSOIL.a	http://www.lonesock.net/soil.html
./shader.cpp	Tools to compile shaders (header)
./models/gourd.h	A gourd from http://people.sc.fsu.edu/~jburkardt/pl_src/obj2opengl/obj2opengl.html
./models/graphicsDefs.h	required file from http://people.sc.fsu.edu/~jburkardt/pl_src/obj2opengl/obj2opengl.html
./utils.hpp	Utilities such as color (header)
./animation.hpp	Animate the scene (header)
./Normals.geometryshader	A shader used to render normals
./readme.txt	This file
./SimpleVertexShader.vertexshader	Passthrough vertex shader
./SimpleFragmentShader.fragmentshader	Shader without lighting
./animation.cpp	Animate the scene
./mesh.cpp Tools to generate/load meshes
./surface_heightmap_con.png Another terrian
./surface_heightmap.png Another terrian
./renderer.cpp	Used to control values that need to be bound to shaders
./renderer.hpp	Used to control values that need to be bound to shaders (header)
./caves_heightmap.png	A heightmap from minecraft

to build on the Ubuntu lab machines simply rmake the bin directory, then
"make all" to run, run "make run"

