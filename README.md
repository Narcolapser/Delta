Project:	Delta
Programmer:	Toben "Littlefoot" "Narcolapser" Archer
Purpose:
	Delta is a simple game engine that's sole purpose is to give me an educational project to learn how to create a game engine. It will never be an extrodinary system, but my goal is that I'll have a much greater understanding of these systems when I'm done. 

Features:
	All physics and graphics will take place on the GPU. User interaction data gets sent to the gpu during an openCL call which it uses to make the appropriate changes. Since it handles all the physics it would be impossible to have it anywhere but the GPU. openCl will also handle the creation of transform matrices for openGL through uniform buffer objects. OpenGl maintains one instance of every model in use, but need not have more than one as individual transforms unique to each instance will handle the transposition from the uniform state to the unique state. Textures, materials, and simple lighting will be supported (shadows may not.)
	Every frame location information will be sent back to the CPU to deal with as it needs. This will allow for the use of openAL for audio. AI may be supported, but no plans as of yet. The UI is point and click with key board support. because of the usage of GLUT as the windowing system, mouse looking will not be supported. The engine will be complined to a single standalone file. when you want to play a game you send commandline args that point to the game's config file which lead the engine through loading resources and initial states. 

Road map:
Arch 1: getting off the ground.
Alpha 1: Initial work with openGL. Get objects to display.
Alpha 2: Create classes for programs, cameras, and attributes. This did not succeed entirely.
Alpha 3: Get the classes from Alpha 2 working.
Alpha 4: Create a mesh class for dynamic loading of meshes.
Alpha 5: Clean: fix mesh rendering and loading bugs. Comment all the uncommented code files. finish removal of scafolding code. create readme file. Diagram current architecture. update make file.

Arch 2: Scenes and Resources
Alpha 6: Move all interaction over to buffer objects except uniforms. add the ability to translate quads to triangles in mesh's load function. Create master ancestor class "Object"
Alpha 7: Create construct of "Scenes" as well as 3D objects which allow for objects to parent other objects. And lastly, get objects moving again. 
Alpha 8: Create resource loading system.
Alpha 9: Get follow cams working. Set up XML parsing for scene input. 
Alpha 10: Clean up. Take out scafolding. Comment the uncommented. Update make so that the whole project doesn't have to be built every time. Update the class diagram. split headers over 100 lines into .h and .cpp.

Arch 3: Interactivity Arch
Alpha 11: Get the shaders fixed so something can be blood seen other than blue.
Alpha 12: Figure out how to Events, create a easier debugging system than manual inserting printfs.
Alpha 13: Create system for dispatching events from objects as well as the keyboard.
Alpha 14: Create system for spawning objects.
Alpha 15: Clean up.

I have concluded this Project at Alpha 15. I've hit a lot of dead ends and really want to rework the way I did a lot of things. So I'm starting over later. this project is now left as is. an uncomplete engine. I might re-visit it and complete it for the sack of the challenge years from now. but as of now. This project is orphaned. 
