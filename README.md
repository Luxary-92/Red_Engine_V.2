## Red_Engine

Red Engine is a 3D videogame engine [(Forked from the Sheeesh Engine V.2.0)](https://github.com/Urii98/SheeeshEngine)

## Team Members

Nixon Correa Albarracín -> Github account: [Nixonbit3](https://github.com/Nixonbit3)

Enric Arxer Cortés -> Github account: [Luxary-92](https://github.com/Luxary-92)

## Install
Download the most recent release
Extract the .zip
Click on the Emmerald Engine folder and Execute Red_Engine.exe

## Engine Description

## High level sistem

You can put a game object with skeleton

### Controls

#### Camera
- While Right clicking:
	- WASD: move forward, left, backward and right
	- QE: move up and down
	- Look around
- F: look at target
- Hold Alt:
	- Mouse Hold Left to orbit the game object selected.
- Mouse wheel to zoom in and zoom out.
- Holding SHIFT duplicates movement speed.

### Features

#### Inspector
The user should can use the inspector to modify a GameObject:
- Hierarchy: delete, reparent, create empty and create children
- Transform: translate, rotate and scale Game Objects.
- Mesh: select or drop any imported mesh
- Texture: select or drop any imported texture
- Camera is a component with settings that can be modified.

#### Game Objects
- Drag and drop game objects and textures in the Hierarchy.
- Create game objects with Game Objects Menu
- Delete game objects by pressing Delete at Inspector
- Delete selected game object at Hierarchy by pressing Delete.
- Move Game Objects in Hierarchy by drag and drop them.
- Can be picked from the world using the mouse

#### Frustrum Culling
All meshes use a bounding volume (AABB) and can be discarded using Frustum Culling. User can visualize this in the editor (debug raycast and boxes).

#### Serialization
We have 2 different json files:
1.Configuration Engine Json
Save/Load:
- FPS
- Window configuration: width, height, brightness, fullscreen, fullscreen desktop, borderless and resizable.
- Camera3D: Position of the camera
- Renderer3D: vsync, wireframe, depthTest, cullFace, Lighting, colorMaterial, Blend, AlphaTest, LineSmooth, PointSmooth, PolygonSmooth.
  
2.Scene Module Serialization
Save:
- Hierarchy Game Objects and Components:
  	- Name
  	- Active
  	- Is Time to Delete
  	- Type of component
  	- Material/Texture Path
  	- Local Transform position

#### Guizmos
Implementation of Guizmo enabling users to manipulate the position, rotation, and scale of an object.

#### Windows
The editor features two windows: one displaying the scene (editor view), and the other showcasing a view captured from a game object with a camera component (main game camera). Each window operates with its own framebuffer, allowing users to simultaneously view both perspectives.

The user can Start / Pause / Stop the simulation and recover its original state.

#### Mouse Picking
GameObjects can be picked from the world using the mouse.

#### Components
- Component Transform.
- Component Mesh.
- Component Material.
- Component Camera.

#### Additional funcionality
An image of the texture is displayed in the Inspector Window.
Serialization of different Modules such as ModuleWindow and ModuleRenderer.

#### Additional Comments
We have managed to carry out all the assignments, except Custom file format and Resource Management.
