# Voxel-Renderer
A Voxel-Renderer used to convert sequence of 2D images into 3D model which can be viewed from any perspective.

This project is developed using OpenGL Fixed Function Pipeline and the 
dependencies are included in Clickme folder. Use the settings as specified.
I have used Rasterisation technique to convert data from images to pixels to display to the screen. 
The output 3D model is rendered in a 3D space. 
After rendering Anti-aliasing is used to make the output image look smooth.

###Description
This project takes 2D sequence of images as input and was designed mainly to produce a 3D model from 2D MRI scans.
Further more this code can be extended for various purposes by changing what to display.  
Input: 2D sequence of processed MRI images.  
Input images format: .png (32 bitdepth)  
Output: A 3D model which can be viewed by rotating it from perspective. 

###Sample output
Input: 2D sequence of heart cross section MRI images.  


![Alt text](https://github.com/BhargavGamit/Voxel-Renderer/blob/master/Output/1.PNG "Output Image")
![Alt text](https://github.com/BhargavGamit/Voxel-Renderer/blob/master/Output/2.PNG "Output Image")
![Alt text](https://github.com/BhargavGamit/Voxel-Renderer/blob/master/Output/3.PNG "Output Image")
![Alt text](https://github.com/BhargavGamit/Voxel-Renderer/blob/master/Output/4.PNG "Output Image")


 
