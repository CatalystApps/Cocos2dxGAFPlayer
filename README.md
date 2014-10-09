Generic Animation Format
=========

Introduction
-----------------------

GAF stands for Generic Animation Format. GAF is designed to store Flash animations in an open cross platform format for further playback in a multitude of game development frameworks and devices. GAF enables artists and animators to use Flash CS for creating complex animations and seamlessly use them with various game development frameworks.

Supported versions Cocos2d-x
-----------------------
v2.2
v2.2.1
v2.2.2
v2.2.3
v2.2.4

Benefits
-----------------------
GAF Converter analyzes the bytecode of each SWF file and creates a GAF file which provides the following benefits:
### Smaller size.
**The traditional animation conversion process generates an image for each frame of the animation thus greatly increasing overall file size. However the GAF animation file stores only unique parts of the animation in a texture atlas and a highly compressed binary config file describing object positions and transformations.**
###Art requirements.
**GAF supports 99% of what can be created in Flash so your artists and animators don’t need any special planning or preparations of their SWF files before converting.**
###High performance. 
**The GAF conversion process involves numerous optimizations to increase playback performance across all platforms and frameworks. Performance of GAF runtimes is much better than that of the Flash player.**

HOWTO
-----------------------
For this folder: Just copy (or create symlink) to cocos2d-x/external/
Compile
Run