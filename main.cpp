#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <gl/freeglut_ext.h>

//--- 아래5개함수는사용자정의함수임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void initBuffer();

//--- 필요한변수선언
GLint width, height;
GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;
GLuint cubeVAO, cubeVBO, cubeEBO;
GLuint axesVAO, axesVBO;

GLvoid Keyboard(unsigned char key, int x, int y);

char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}


void drawScene() {
    glClearColor(1, 1, 1, 1); // 배경 흰색
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 컬러/깊이 버퍼 초기화

    glEnable(GL_DEPTH_TEST); // 겹쳐진 3D 객체 올바르게 보이게

    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    unsigned int modelLocation = glGetUniformLocation(shaderProgram, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));


    glBindVertexArray(axesVAO);
    glDrawArrays(GL_LINES, 0, 6);

    glBindVertexArray(cubeVAO);
    for (int i = 0; i < 6; i++)
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
}

const float cubeVertexData[] = {
    // 앞면 (빨간색) - 면 1
    -0.3f, -0.3f,  0.3f,    1.0f, 0.0f, 0.0f,  // 0
     0.3f, -0.3f,  0.3f,    1.0f, 0.0f, 0.0f,  // 1
     0.3f,  0.3f,  0.3f,    1.0f, 0.0f, 0.0f,  // 2
    -0.3f,  0.3f,  0.3f,    1.0f, 0.0f, 0.0f,  // 3

    // 뒷면 (초록색) - 면 2
    -0.3f, -0.3f, -0.3f,    0.0f, 1.0f, 0.0f,  // 4
     0.3f, -0.3f, -0.3f,    0.0f, 1.0f, 0.0f,  // 5
     0.3f,  0.3f, -0.3f,    0.0f, 1.0f, 0.0f,  // 6
    -0.3f,  0.3f, -0.3f,    0.0f, 1.0f, 0.0f,  // 7

    // 왼쪽면 (파란색) - 면 3
    -0.3f, -0.3f, -0.3f,    0.0f, 0.0f, 1.0f,  // 8
    -0.3f, -0.3f,  0.3f,    0.0f, 0.0f, 1.0f,  // 9
    -0.3f,  0.3f,  0.3f,    0.0f, 0.0f, 1.0f,  // 10
    -0.3f,  0.3f, -0.3f,    0.0f, 0.0f, 1.0f,  // 11

    // 오른쪽면 (노란색) - 면 4
     0.3f, -0.3f, -0.3f,    1.0f, 1.0f, 0.0f,  // 12
     0.3f, -0.3f,  0.3f,    1.0f, 1.0f, 0.0f,  // 13
     0.3f,  0.3f,  0.3f,    1.0f, 1.0f, 0.0f,  // 14
     0.3f,  0.3f, -0.3f,    1.0f, 1.0f, 0.0f,  // 15

     // 위쪽면 (자홍색) - 면 5
     -0.3f,  0.3f, -0.3f,    1.0f, 0.0f, 1.0f,  // 16
      0.3f,  0.3f, -0.3f,    1.0f, 0.0f, 1.0f,  // 17
      0.3f,  0.3f,  0.3f,    1.0f, 0.0f, 1.0f,  // 18
     -0.3f,  0.3f,  0.3f,    1.0f, 0.0f, 1.0f,  // 19

     // 아래쪽면 (청록색) - 면 6
     -0.3f, -0.3f, -0.3f,    0.0f, 1.0f, 1.0f,  // 20
      0.3f, -0.3f, -0.3f,    0.0f, 1.0f, 1.0f,  // 21
      0.3f, -0.3f,  0.3f,    0.0f, 1.0f, 1.0f,  // 22
     -0.3f, -0.3f,  0.3f,    0.0f, 1.0f, 1.0f   // 23
};

// 육면체 인덱스 (각 면별로 분리)
const unsigned int cubeIndices[] = {
    // 앞면 (1번 키)
    0, 1, 2,  2, 3, 0,
    // 뒷면 (2번 키) 
    4, 6, 5,  6, 4, 7,
    // 왼쪽면 (3번 키)
    8, 9, 10,  10, 11, 8,
    // 오른쪽면 (4번 키)
    12, 14, 13,  14, 12, 15,
    // 위쪽면 (5번 키)
    16, 17, 18,  18, 19, 16,
    // 아래쪽면 (6번 키)
    20, 22, 21,  22, 20, 23
};

// 좌표축 vertex 데이터
const float axesVertexData[] = {
    // X축 (빨간색)
    -1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
     // Y축 (초록색)
      0.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
      // Z축 (파란색)
       0.0f, 0.0f, -1.0f,   0.0f, 0.0f, 1.0f,
       0.0f, 0.0f,  1.0f,   0.0f, 0.0f, 1.0f
};

int main(int argc, char** argv)
{
    width = 800;
    height = 800;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutCreateWindow("test");

    glewExperimental = GL_TRUE;
    glewInit();

    make_vertexShaders();
    make_fragmentShaders();
    shaderProgram = make_shaderProgram();
    initBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        glutLeaveMainLoop();
        break;
    }
    glutPostRedisplay();
}

void make_vertexShaders()
{
    GLchar* vertexSource;

    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cout << "Vertex Shader Error: " << errorLog << std::endl;
        return;
    }
}

void make_fragmentShaders()
{
    GLchar* fragmentSource;
    fragmentSource = filetobuf("fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: fragment shader error\n" << errorLog << std::endl;
        return;
    }
}

GLuint make_shaderProgram()
{
    GLint result;
    GLchar* errorLog = NULL;

    GLuint shaderID = glCreateProgram();

    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);

    glLinkProgram(shaderID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
        return false;
    }
    return shaderID;
}

void initBuffer()
{
    // 육면체 VAO, VBO, EBO 설정
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 좌표축 VAO, VBO 설정
    glGenVertexArrays(1, &axesVAO);
    glBindVertexArray(axesVAO);

    glGenBuffers(1, &axesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertexData), axesVertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}