#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <GL/freeglut.h>
#include <SOIL.h>

struct textura {
    int largura;
    int altura;
    int canais;
    unsigned char* pixels;
};

struct vetor {
    float x, y, z;
};

bool isFilledPolygon = false;
struct textura texturaTerreno;
float alturaMaxima = 5;
struct vetor posicaoCamera;
struct vetor alvoCamera;
struct vetor direcaoCamera;
struct vetor up;

using namespace std;



float normaVetor(struct vetor* v)
{
    return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

struct vetor vetorNormalizado(struct vetor* v)
{
    struct vetor u;
    float norma = normaVetor(v);
    u.x = v->x/norma;
    u.y = v->y/norma;
    u.z = v->z/norma;
    return u;
}

struct vetor somaVetor(struct vetor* u, struct vetor* v)
{
    struct vetor s;
    s.x = u->x + v->x;
    s.y = u->y + v->y;
    s.z = u->z + v->z;
    return s;
};

struct vetor multEscalarVetor(struct vetor* v, float e)
{
    struct vetor s;
    s.x = v->x*e;
    s.y = v->y*e;
    s.z = v->z*e;
    return s;
};

struct vetor diferencaVetor(struct vetor* u, struct vetor* v)
{
    struct vetor s;
    s.x = u->x - v->x;
    s.y = u->y - v->y;
    s.z = u->z - v->z;
    return s;
};

struct vetor produtoVetorial(struct vetor* u, struct vetor* v)
{
    struct vetor s;
    s.x = u->y*v->z - u->z*v->y;
    s.y = u->z*v->x - u->x*v->z;
    s.z = u->x*v->y - u->y*v->x;
    return s;
};

float* vetorParaArray(struct vetor *u) {
    float arr[3];
    arr[0] = u->x;
    arr[1] = u->y;
    arr[2] = u->z;
    return arr;
}

void logVetor(struct vetor* u)
{
    printf("vetor(%f, %f, %f)\n", u->x, u->y, u->z);
}

void atualizaDirecaoCamera()
{
    direcaoCamera = diferencaVetor(&alvoCamera, &posicaoCamera);
}

void atualizaAlvoCamera()
{
    alvoCamera = somaVetor(&posicaoCamera, &direcaoCamera);
}

void carregaTexturaTerreno()
{
    texturaTerreno.pixels = SOIL_load_image(
                                "mapa-altura.bmp",
                                &texturaTerreno.largura, &texturaTerreno.altura, &texturaTerreno.canais,
                                SOIL_LOAD_L
                            );
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Turn on OpenGL lighting.
    //glEnable(GL_LIGHTING);

    // Automatically resize all normals to unit length.
    glEnable(GL_NORMALIZE);

    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 0.0 };
    float lightPos[] = {-1.0, -1.0, 0.0, 0.0 };
    float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0); // Enable particular light source.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

    // Material property vectors.
    float matAmbAndDif[] = {0.0, 0.5, 0.5, 1.0};
    float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float matShine[] = { 50.0 };

    // Material properties.
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

    // carrega o mapa de textura
    carregaTexturaTerreno();
    // configura a posição e alvo inicial da câmera
    posicaoCamera.x = 0;
    posicaoCamera.y = 5;
    posicaoCamera.z = 30;
    alvoCamera.x = 0;
    alvoCamera.y = 0;
    alvoCamera.z = 0;
    atualizaDirecaoCamera();

    up.x = 0;
    up.y = 1;
    up.z = 0;
}

struct vetor normalFace(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    struct vetor normal;
    struct vetor p1; struct vetor p2; struct vetor p3;
    struct vetor v1; struct vetor v2;
    p1.x = x1; p1.y = y1; p1.z = z1;
    p2.x = x2; p2.y = y2; p2.z = z2;
    p3.x = x3; p3.y = y3; p3.z = z3;
    v1 = diferencaVetor(&p2, &p1);
    v2 = diferencaVetor(&p3, &p1);

    normal = produtoVetorial(&v1, &v2);
    return normal;
};

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(posicaoCamera.x, posicaoCamera.y, posicaoCamera.z,
              alvoCamera.x, alvoCamera.y, alvoCamera.z,
              0.0, 1.0, 0.0);
    if (isFilledPolygon) {
        glPolygonMode(GL_FRONT, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    glColor3f(0.8f,0.8f,0.8f);
    // Draw a plane triangular grid.
    for(float v = -100.0; v < 100.0 - 1; v += 1.0) {
        glBegin(GL_TRIANGLE_STRIP);
        for(float u = -100.0; u < 100.0; u += 1.0) {
            int s = (int)(((u+100)/200.0f)*(texturaTerreno.largura-1));
            int t = (int)(((v+100)/200.0f)*(texturaTerreno.altura-1));
            unsigned char tomDeCinza = texturaTerreno.pixels[(t-1)*texturaTerreno.largura+s];
            float y = (tomDeCinza/255.0f) * alturaMaxima;
            struct vetor normal = normalFace(u, y, v-1, u, y, v, u+1, y, v-1);
            glNormal3fv(vetorParaArray(&normal));
            glVertex3f(u, y, v - 1);

            s = (int)(((u+100)/200.0f)*(texturaTerreno.largura-1));
            t = (int)(((v+100)/200.0f)*(texturaTerreno.altura-1));
            tomDeCinza = texturaTerreno.pixels[(t+1)*texturaTerreno.largura+s];
            y = (tomDeCinza/255.0f) * alturaMaxima;
            normal = normalFace(u, y, v, u+1, y, v-1, u+1, y, v);
            glNormal3fv(vetorParaArray(&normal));
            glVertex3f(u, y, v);
        }
        glEnd();
    }

    glutSwapBuffers();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void keyInput(unsigned char key, int x, int y)
{
    struct vetor deslocamento;
    struct vetor temp1, temp2;
    switch(key) {
    case 27:
        exit(0);
        break;
    case 'f':
        isFilledPolygon = !isFilledPolygon;
    case '+':
        alturaMaxima += 0.5f;
        break;
    case '-':
        alturaMaxima -= 0.5f;
        break;
    case 'w':
        temp1 = vetorNormalizado(&direcaoCamera);
        posicaoCamera = somaVetor(&posicaoCamera, &temp1);
        atualizaAlvoCamera();
        break;
    case 's':
        temp1 = vetorNormalizado(&direcaoCamera);
        posicaoCamera = diferencaVetor(&posicaoCamera, &temp1);
        atualizaAlvoCamera();
        break;
    case 'd':
        temp1 = produtoVetorial(&direcaoCamera, &up);
        deslocamento = vetorNormalizado(&temp1);
        temp2 = multEscalarVetor(&deslocamento, 1);
        direcaoCamera = somaVetor(&direcaoCamera, &temp2);
        atualizaAlvoCamera();
        break;
    case 'a':
        temp1 = produtoVetorial(&direcaoCamera, &up);
        deslocamento = vetorNormalizado(&temp1);
        temp2 = multEscalarVetor(&deslocamento, 1);
        direcaoCamera = diferencaVetor(&direcaoCamera, &temp2);
        atualizaAlvoCamera();
        break;
    case 'c':
        posicaoCamera.x = 0;
        posicaoCamera.y = 30;
        posicaoCamera.z = 0;
        alvoCamera.x = 0;
        alvoCamera.y = 0;
        alvoCamera.z = 0;
        direcaoCamera.x = 0;
        direcaoCamera.y = -1;
        direcaoCamera.z = 0.5f;
        atualizaAlvoCamera();
    default:
        break;
    }
    glutPostRedisplay();
}

void imprimeAjuda(void)
{
    cout << "Interacao:" << endl;
    cout << "  'f': ativa/destiva o preenchimento dos triangulos (glPolygonMode)." << endl;
    cout << "  'w,a,s,d': navega na cena." << endl;
    cout << "  'c': coloca a camera no topo, olhando para o terreno." << endl;
}

int main(int argc, char **argv)
{
    imprimeAjuda();
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mapa de Alturas");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    setup();

    glutMainLoop();
}

