# Nerv engine - physically based rendering

![Github](https://github.com/Cewein/nerv_pathtracer/blob/master/Render/glass%20bunny%20light.PNG)

This is a 3D engine for light simulation in mind.<br>
The goal of this engine is to implement a real-time raytraced physically based renderer with the pixar material model.<br>
<br>
The engine use OpenGL, GLFW and GLM, but in the future we plan to remove the need of GLM. <br>
Nerv's core is generalist and can render object in a rastered way but heavly use shaders and a single quad to make "realtime ray based application".
## showcase
#### Depths of fields
![Github](https://github.com/Cewein/nerv_pathtracer/blob/master/Render/rabbit%20sphere.png)
![Imgur](https://i.imgur.com/ZvpZmb0.png)

## Nerv engine status
### core
 - [x] config file
 - [x] I/O for mouse and keyboard
 - [x] file reader
 - [x] windows manager

### engine

 - [x] shader support
 - [x] transform controle
 - [x] FPS camera
 - [x] basic objects
 - [x] textures (kinda)
 - [ ] USD loader
 - [x] GUI
 - [x] BVH
 - [ ] material model
 - [ ] scene descriptor
 
 ### rendering
 
  - [x] raytracing
  - [x] reflection mat
  - [X] refraction mat
  - [x] lambert mat
  - [x] camera lens (Depths of fields)
  - [x] shadow
  - [x] light
  - [ ] Pixar BSDF model
  - [x] progressive sampleling
