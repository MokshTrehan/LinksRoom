Readme

Description:
This code is written in C++ and is designed to read and render a 3D model in PLY file format. It uses OpenGL and GLM libraries for rendering and manipulation of the model. The code reads the PLY file format and extracts information about the vertices, normals, colors, and texture coordinates. It also reads the face information to build triangles to render the model.

The code defines two structures, VertexData and TriData, to store the information about the vertices and faces. The VertexData structure stores the x, y, and z coordinates of the vertices, the x, y, and z components of the normal vectors, the red, green, and blue color values of the vertices, and the u and v texture coordinates of the vertices. The TriData structure stores the indices of the vertices that form a triangle.

The code defines a camera structure that is used to manipulate the view of the 3D model. The adjustCam function in the code takes the keyboard inputs and adjusts the position and orientation of the camera accordingly.

The readPLYFile function in the code reads the PLY file and extracts the information about the vertices and faces. It checks for the number of vertices and faces in the file, the x, y, and z coordinates of each vertex, the x, y, and z components of each vertex's normal vector, the red, green, and blue values of each vertex's color, and the u and v texture coordinates of each vertex. It then builds the VertexData and TriData structures to store this information.

The code then uses OpenGL to render the 3D model. It initializes the GLFW window, sets up the OpenGL context, loads the PLY file, and renders the model using the information from the VertexData and TriData structures. It also sets up the projection matrix, the model matrix, and the view matrix to adjust the view of the 3D model.

Overall, this code provides a basic framework for reading and rendering a 3D model in PLY file format using OpenGL and GLM libraries.

Requirements:
To compile and run this code, the following libraries are required:

OpenGL Extension Wrangler Library (GLEW): GLEW is used to access OpenGL extensions.
GLFW: GLFW is used to create an OpenGL context and handle user input.
GLM: GLM is used for matrix and vector math operations in OpenGL.
C++ Standard Library: This code also relies on the C++ standard library.
Make sure these libraries are installed and linked correctly before attempting to compile and run the code.

Installation:
Compile the code in Main.cpp using the provided makefile. It will automatically run the program. Ensure the Main.cpp is in the same directory as the various required BMP and PLY files. All are included in the provided folder.

Additional Files (not inclusive of ply and bmp files):
Working program: a video showcasing the operation of the program with keystrokes recorded to demonstrate effectiveness
Working_code_1: a screenshot of the code working
Working_code_2: a second screenshot from another angle of the code working

Usage
The program allows the user to view the inside of links house by reading in various PLY and BMP files. The user can travers the house by using the forward and backward arrow keys to travel, and the left and right keys to rotate the camera.

Functions
adjustCam: A function that adjusts the camera position based on key presses.

readPLYFile: A function that reads in vertex and face data from a PLY file and stores it in vectors.

loadARGB_BMP: A function that reads in texture data and stores it in a bitmap

TexturedMesh: This function loads a 3D mesh from a PLY file and a texture from a BMP file. It compiles and links a shader program, creates a texture object and vertex buffer objects, and sets up the vertex attributes and element array buffer for rendering the mesh with the texture. The mesh is stored as vertex data and face indices, and the texture is applied to the mesh using texture coordinates.

draw: This function draws a textured mesh using the given MVP (Model-View-Projection) matrix. It first gets the uniform location for the MVP matrix and activates the texture. Then it binds the shader program, sets the MVP matrix uniform and sampler uniform, and draws the mesh using GL_TRIANGLES and GL_UNSIGNED_INT. Finally, it disables the texture and unbinds the shader program.

Main: This function is the entry point of the program and it initializes the OpenGL context and the window. It also loads the necessary files and sets up the projection matrix for rendering the textured meshes. Then, it enters a render loop where it clears the screen, renders each textured mesh using the MVP matrix, swaps the buffers, and polls for events.
