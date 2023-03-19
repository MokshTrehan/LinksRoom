#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <vector>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


struct VertexData
{
  // create a float array which is 3 times the num of veritices
  //  create an int array for the indices of the faces
  float x, y, z;
  float nx, ny, nz;
  float r, g, b;
  float u, v;
};

struct TriData
{
  ivec3 vertex_indices;
};

struct camera
{ 
  //z cord for key presses
  vec3 position = {0.5f, 0.4f, 0.5f};
  //x cord for yaw adjustments and rotation
  vec3 direction = {0.0f, 0.0f, -1.0f};
  vec3 orientation = {0.0f, 1.0f, 0.0f};
  
};

camera room_camera;

void adjustCam(GLFWwindow *window){
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
    room_camera.position += 0.01f * room_camera.direction;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
    room_camera.position -= 0.01f * room_camera.direction;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
    glm::mat4 yaw_rotation = glm::rotate(glm::mat4(1.0f), 0.01f, room_camera.orientation);
    room_camera.direction = glm::vec3(yaw_rotation * glm::vec4(room_camera.direction, 0.0f));
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
    glm::mat4 yaw_rotation = glm::rotate(glm::mat4(1.0f), -0.01f, room_camera.orientation);
    room_camera.direction = glm::vec3(yaw_rotation * glm::vec4(room_camera.direction, 0.0f));
  }

};




void readPLYFile(std::string fname, std::vector<VertexData> &vertices, std::vector<TriData> &faces)
{
  // Open the file and initialize some variables.
  std::ifstream file(fname);
  std::string line;
  int num_vertices = 0;
  int num_faces = 0;
  int cord_index = 0;
  int norm_index = 0;
  int colour_index = 0;
  int u_index = 0;
  bool has_normals = false;
  bool has_colors = false;
  bool has_texcoords = false;
  bool reading_vertices = false;
  bool reading_faces = false;
  std::stringstream ss;

  // Check if the file was successfully opened.
  if (!file.is_open())
  {
    std::cout << "Error: could not open file " << fname << std::endl;
    return;
  }

  int current_index = 0;

  // Read through the file line by line to gather information about the file format.
  while (getline(file, line))
  {
    // Check for the number of vertices in the file.
    if (line.find("element vertex") != std::string::npos)
    {
      ss.clear();
      ss << line;
      ss.ignore(14);
      ss >> num_vertices;
    }
    // Check for the number of faces in the file.
    else if (line.find("element face") != std::string::npos)
    {
      ss.clear();
      ss << line;
      ss.ignore(12);
      ss >> num_faces;
    }
    // Check for the x, y, and z coordinates of each vertex.
    else if (line.find("property float x") != std::string::npos)
    {
      cord_index = current_index;
      current_index++;
      reading_vertices = true;
    }
    // Check for the x, y, and z components of each vertex's normal vector.
    else if (line.find("property float nx") != std::string::npos)
    {
      norm_index = current_index;
      current_index++;
      has_normals = true;
    }
    // Check for the red, green, and blue values of each vertex's color.
    else if (line.find("property uchar red") != std::string::npos)
    {
      colour_index = current_index;
      current_index++;
      has_colors = true;
    }
    // Check for the u and v texture coordinates of each vertex.
    else if (line.find("property float u") != std::string::npos)
    {
      u_index = current_index;
      current_index++;
      has_texcoords = true;
    }
    // Check for the end of the header section of the file.
    else if (line.find("end_header") != std::string::npos)
    {
      break;
    }
  }

  // Loop through all vertices
  for (int i = 0; i < num_vertices; i++){
    VertexData vertex;

    // Read a line from the file
    getline(file, line);

    // Clear the string stream and set it to the new line
    ss.clear();
    ss << line;

    int order = 0;

    // Loop through each attribute of the vertex (up to 5)
    for (int j = 0; j < 5; j++)
    {
      // If we're reading vertices and we've reached the correct index, read in the coordinates
      if (reading_vertices && order == cord_index)
      {
        ss >> vertex.x >> vertex.y >> vertex.z;
        order++;
      }

      // If we have normals and we've reached the correct index, read in the normal coordinates
      if (has_normals && order == norm_index)
      {
        ss >> vertex.nx >> vertex.ny >> vertex.nz;
        order++;
      }

      // If we have colors and we've reached the correct index, read in the color values and convert them to floats between 0 and 1
      if (has_colors && order == colour_index)
      {
        int r, g, b;
        ss >> r >> g >> b;
        vertex.r = r / 255.0f;
        vertex.g = g / 255.0f;
        vertex.b = b / 255.0f;
        order++;
      }

      // If we have texture coordinates and we've reached the correct index, read in the u and v values
      if (has_texcoords && order == u_index)
      {
        ss >> vertex.u >> vertex.v;
        order++;
      }
    }

    // Add the vertex to the list of vertices
    vertices.push_back(vertex);
  }

  // Loop through all faces
  for (int i = 0; i < num_faces; i++)
  {
    TriData face;

    // Read a line from the file
    getline(file, line);

    // Clear the string stream and set it to the new line
    ss.clear();
    ss << line;

    int num_vertices_in_face;

    // Read in the number of vertices in the face
    ss >> num_vertices_in_face;

    // If the face has 3 vertices, read in the vertex indices and add the face to the list of faces
    if (num_vertices_in_face == 3)
    {
      int v1, v2, v3;
      ss >> v1 >> v2 >> v3;
      ivec3 pusher = {v1, v2, v3};
      TriData face;
      face.vertex_indices = pusher;
      faces.push_back(face);
    }
  }

}

void loadARGB_BMP(const char *imagepath, unsigned char **data, unsigned int *width, unsigned int *height)
{

  printf("Reading image %s\n", imagepath);

  // Data read from the header of the BMP file
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int imageSize;
  // Actual RGBA data

  // Open the file
  FILE *file = fopen(imagepath, "rb");
  if (!file)
  {
    printf("%s could not be opened. Are you in the right directory?\n", imagepath);
    getchar();
    return;
  }

  // Read the header, i.e. the 54 first bytes

  // If less than 54 bytes are read, problem
  if (fread(header, 1, 54, file) != 54)
  {
    printf("Not a correct BMP file1\n");
    fclose(file);
    return;
  }

  // Read the information about the image
  dataPos = *(int *)&(header[0x0A]);
  imageSize = *(int *)&(header[0x22]);
  *width = *(int *)&(header[0x12]);
  *height = *(int *)&(header[0x16]);
  // A BMP files always begins with "BM"
  if (header[0] != 'B' || header[1] != 'M')
  {
    printf("Not a correct BMP file2\n");
    fclose(file);
    return;
  }
  // Make sure this is a 32bpp file
  if (*(int *)&(header[0x1E]) != 3)
  {
    printf("Not a correct BMP file3\n");
    fclose(file);
    return;
  }
  // fprintf(stderr, "header[0x1c]: %d\n", *(int*)&(header[0x1c]));
  // if ( *(int*)&(header[0x1C])!=32 ) {
  //     printf("Not a correct BMP file4\n");
  //     fclose(file);
  //     return;
  // }

  // Some BMP files are misformatted, guess missing information
  if (imageSize == 0)
    imageSize = (*width) * (*height) * 4; // 4 : one byte for each Red, Green, Blue, Alpha component
  if (dataPos == 0)
    dataPos = 54; // The BMP header is done that way

  // Create a buffer
  *data = new unsigned char[imageSize];

  if (dataPos != 54)
  {
    fread(header, 1, dataPos - 54, file);
  }

  // Read the actual data from the file into the buffer
  fread(*data, 1, imageSize, file);

  // Everything is in memory now, the file can be closed.
  fclose(file);
}

class TexturedMesh
{

  std::vector<VertexData> vertices;
  std::vector<TriData> faces;
  GLuint textureID;
  GLuint vertexPosVBO;
  GLuint texCoordVBO;
  GLuint faceIndexVBO;
  GLuint VAO;
  GLuint shaderProgram;

public:
  TexturedMesh(const std::string &plyFilePath, const std::string &bmpFilePath)
  {
    // read vertices and faces from PLY file

    readPLYFile(plyFilePath, vertices, faces);
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    std::string VertexShaderCode = "\
            #version 330 core\n\
            // Input vertex data, different for all executions of this shader.\n\
            layout(location = 0) in vec3 vertexPosition;\n\
            layout(location = 1) in vec2 uv;\n\
            // Output data ; will be interpolated for each fragment.\n\
            out vec2 uv_out;\n\
            // Values that stay constant for the whole mesh.\n\
            uniform mat4 MVP;\n\
            void main(){ \n\
                // Output position of the vertex, in clip space : MVP * position\n\
                gl_Position =  MVP * vec4(vertexPosition,1);\n\
                // The color will be interpolated to produce the color of each fragment\n\
                uv_out = uv;\n\
            }\n";

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode = "\
            #version 330 core\n\
            in vec2 uv_out; \n\
            uniform sampler2D tex;\n\
            void main() {\n\
                gl_FragColor = texture(tex, uv_out);\n\
            }\n";
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Compile Fragment Shader
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, VertexShaderID);
    glAttachShader(shaderProgram, FragmentShaderID);

    glLinkProgram(shaderProgram);

    glDetachShader(shaderProgram, VertexShaderID);
    glDetachShader(shaderProgram, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    // load bitmap image
    GLuint width, height;
    unsigned char *image;
    loadARGB_BMP(bmpFilePath.c_str(), &image, &width, &height);

    // create texture object
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexPosVBO);
    glGenBuffers(1, &texCoordVBO);
    glGenBuffers(1, &faceIndexVBO);

    glBindVertexArray(VAO);

    // create vertex buffer object for face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(TriData), faces.data(), GL_STATIC_DRAW);

    // create vertex buffer object for vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vertexPosVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)0);

    // create vertex buffer object for texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(offsetof(VertexData, u)));

    // create vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void draw(const glm::mat4 &MVP) const
  {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(MVP));

    // set MVP matrix uniform
    // activate texture and set sampler uniform

    // draw the mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
};

int main()
{

  // initialize GLFW and GLEW
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(1024, 768, "Textured Mesh Demo", NULL, NULL);
  glfwMakeContextCurrent(window);

  glewInit();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);


  // load PLY and BMP files
  TexturedMesh floor("Floor.ply", "floor.bmp");
  TexturedMesh walls("Walls.ply", "walls.bmp");
  TexturedMesh table("Table.ply", "table.bmp");
  TexturedMesh patio("Patio.ply", "patio.bmp");
  TexturedMesh bottles("Bottles.ply", "bottles.bmp");
  TexturedMesh woodObjects("WoodObjects.ply", "woodobjects.bmp");
  TexturedMesh metalObject("MetalObjects.ply", "metalobjects.bmp");
  TexturedMesh windowbg("WindowBG.ply", "windowbg.bmp");
  TexturedMesh doorbg("DoorBG.ply", "doorbg.bmp");
  TexturedMesh curtains("Curtains.ply", "curtains.bmp");

  // create projection matrix with 45 degree field of view
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  mat4 projectionMatrix = perspective(radians(45.0f), 1024.0f / 768.0f, 0.001f, 1000.0f);

  glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
  // render loop
  while (!glfwWindowShouldClose(window))
  {
    glLoadMatrixf(glm::value_ptr(projectionMatrix));
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    adjustCam(window);
    mat4 viewModel = lookAt(room_camera.position, room_camera.position + room_camera.direction, room_camera.orientation);
    mat4 M = glm::mat4(1.0f);
    mat4 MV = viewModel * M;
    mat4 MVP = projectionMatrix * viewModel * M;
    glLoadMatrixf(value_ptr(viewModel));

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render the textured mesh
    floor.draw(MVP);
    walls.draw(MVP);
    table.draw(MVP);
    patio.draw(MVP);
    bottles.draw(MVP);
    woodObjects.draw(MVP);
    metalObject.draw(MVP);
    windowbg.draw(MVP);
    doorbg.draw(MVP);
    curtains.draw(MVP);
    // swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glfwTerminate();

  return 0;
}