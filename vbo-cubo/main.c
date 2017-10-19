#include <GL/glew.h>
#include <GL/freeglut.h>

#define TRUE 1
#define FALSE 0

enum TipoArmazenamento {
  IMMEDIATE_MODE = 0,
  VERTEX_ARRAYS = 1,
  VERTEX_BUFFER_OBJECTS = 2
};

TipoArmazenamento armazenamento = VERTEX_ARRAYS;
static GLuint vboCubo[1];

// cubo ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
// Um cubo tem 6 faces com 2 triângulos cada. Portanto, um cubo possui
// 36 vértices (6 faces * 2 tris * 3 vértices = 36 vértices).
// Cada vértice possui 3 floats (x,y,z), então, precisamos de 108
// floats (36 * 3 = 108).
GLfloat verticesCubo[] = {
    1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (frente)
   -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

    1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (direita)
    1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

    1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (cima)
   -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (esquerda)
   -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

   -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (baixo)
    1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

    1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (trás)
   -1, 1,-1,   1, 1,-1,   1,-1,-1       // v6-v5-v4
};

// array com as normais de cada vértice
GLfloat normaisCubo[]   = {
    0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (frente)
    0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

    1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (direita)
    1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

    0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (cima)
    0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (esquerda)
   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

    0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (baixo)
    0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

    0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (trás)
    0, 0,-1,   0, 0,-1,   0, 0,-1       // v6-v5-v4
};

// array com a cor de cada vértice
GLfloat coresCubo[]    = {
    1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (frente)
    1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

    1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (direita)
    0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

    1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (cima)
    0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

    1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (esquerda)
    0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

    0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (baixo)
    1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

    0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (trás)
    0, 1, 0,   0, 1, 1,   0, 0, 1       // v6-v5-v4
};

void inicializa() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    // create vertex buffer objects, you need to delete them when program exits
    // Try to put both vertex coords array, vertex normal array and vertex color in the same buffer object.
    // glBufferDataARB with NULL pointer reserves only memory space.
    // Copy actual data with 2 calls of glBufferSubDataARB, one for vertex coords and one for normals.
    // target flag is GL_ARRAY_BUFFER_ARB, and usage flag is GL_STATIC_DRAW_ARB
    glGenBuffers(1, vboCubo);
    glBindBuffer(GL_ARRAY_BUFFER, vboCubo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo) + sizeof(normaisCubo) + sizeof(coresCubo), 0, GL_STATIC_DRAW_ARB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesCubo), verticesCubo);                             // copy vertices starting from 0 offest
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticesCubo), sizeof(normaisCubo), normaisCubo);                // copy normals after vertices
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticesCubo) + sizeof(normaisCubo), sizeof(coresCubo), coresCubo);  // copy colours after normals

}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)(w)/h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void desenhaCubo() {
    switch (armazenamento) {
        case VERTEX_ARRAYS:
            // habilita os "vertex arrays" que vamos usar
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);

            // especificamos onde na memória RAM o OpenGL
            // vai encontrar (a) a coordenada, (b) a normal
            // e (c) a cor de cada vértice
            glNormalPointer(GL_FLOAT, 0, normaisCubo);
            glColorPointer(3, GL_FLOAT, 0, coresCubo);
            glVertexPointer(3, GL_FLOAT, 0, verticesCubo);

            // manda desenhar GL_TRIANGLES usando 36 vértices,
            // com 3 vértices por tri (ie, 12 triângulos, 6 faces)
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            break;

        case VERTEX_BUFFER_OBJECTS:
            glBindBuffer(GL_ARRAY_BUFFER, vboCubo[0]);

            // habilita os "vertex arrays" que vamos usar
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            // como os VBOs colocam tudo junto (coords, normais e cores),
            // precisamos definir os offsets de onde começam cada um
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glNormalPointer(GL_FLOAT, 0, (void*)sizeof(verticesCubo));
            glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(verticesCubo) + sizeof(normaisCubo)));

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);

            // pára de usar este VBO (colocando o VBO com id=0 para tanto)
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            break;

        case IMMEDIATE_MODE:
        default:

            glBegin(GL_TRIANGLES);
              int c;
              for (c = 0; c < 108; c += 3) {
                  glNormal3f(normaisCubo[c], normaisCubo[c+1], normaisCubo[c+2]);
                  glColor3f(coresCubo[c], coresCubo[c+1], coresCubo[c+2]);
                  glVertex3f(verticesCubo[c], verticesCubo[c+1], verticesCubo[c+2]);
              }
            glEnd();
            break;
    }
}

void desenhaCena() {
    // apaga a tela para desenhar de novo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glTranslatef(0, 0, -5);
        glRotatef(30, 1, 0, 0);
        desenhaCubo();
    glPopMatrix();

    // troca os buffers e manda desenhar
    glutSwapBuffers();
}

void atualiza(int id) {

    glutPostRedisplay();
    glutTimerFunc(0, atualiza, 17);
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;

        case ' ':
            // armazenamento = ((int)armazenamento + 1) % 3;
            break;
    }
}





int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    // glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(400, 400);

    glutCreateWindow("Cubo usando VBO");
    glutReshapeFunc(redimensiona);
    glutDisplayFunc(desenhaCena);
    glutTimerFunc(0, atualiza, 17);
    glutKeyboardFunc(teclado);

    glewInit();

    inicializa();

    glutMainLoop();
    return 0;
}
