#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static unsigned int compileShader(unsigned int shaderType, std::string &shaderSourceCode)
{
    unsigned int shaderID = glCreateShader(shaderType);
    const char *sourceCode = shaderSourceCode.c_str();
    glShaderSource(shaderID, 1, &sourceCode, nullptr);
    glCompileShader(shaderID);

    return shaderID;
}

std::string fileToString(std::string path)
{
    std::ifstream stream(path);
    std::string line;
    std::stringstream out;

    while (getline(stream, line))
    {
        out << line << '\n';   
    }
    
    return out.str();
}

static unsigned int createShader()
{
    unsigned int programID = glCreateProgram();
    std::string vertexShaderSourceCode = fileToString("C:/Dev/Rayan/C++/Firestorm/Firestorm/vertexShader.glsl");
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSourceCode);
    std::string fragmentShaderSourceCode = fileToString("C:/Dev/Rayan/C++/Firestorm/Firestorm/fragmentShader.glsl");
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCode);

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    glValidateProgram(programID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return programID;
}

int main()
{
    float red = 0.2f;
    float green = 0.2f;
    float blue = 0.2f;
    float alpha = 0.0f;
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800, 600, "Firestorm", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] =
        {
            -0.5f, 0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f,
            -0.5f, -0.5f};


    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void *)0);


    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int shader = createShader();
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        //  check for input
        processInput(window);
        //  rendering
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        //  check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();

    return 0;
}