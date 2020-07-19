# Nerv engine - physically based rendering

![Imgur](https://i.imgur.com/6vfVbW7.jpeg)

This is a 3D engine for light simulation in mind.<br>
The goal of this engine is to implement a real-time raytraced physically based renderer with the pixar material model.<br>
<br>
The engine use OpenGL, GLFW and GLM, but in the future we plan to remove the need of GLM. <br>
Nerv's core is generalist and can render object in a rastered way but heavly use shaders and a single quad to make "realtime ray based application".
## showcase
#### Depths of fields
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
 - [ ] glTF loader
 - [ ] frame buffer
 - [x] GUI
 - [ ] BVH
 
 ### rendering
 
  - [x] raytracing
  - [x] reflection mat
  - [X] refraction mat
  - [x] lambert mat
  - [x] camera lens (Depths of fields)
  - [ ] shadow
  - [ ] light
  - [ ] Pixar BSDF model
  - [ ] progressive sampleling
