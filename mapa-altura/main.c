#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include "vetor.h"
#define TRUE 1
#define FALSE 0

struct textura {
    int largura;
    int altura;
    int canais;
    unsigned char* pixels;
};


int modoPreenchido = TRUE;
struct textura texturaTerreno;
float alturaMaxima = 5;
vec3 posicaoCamera;
vec3 alvoCamera;
vec3 direcaoCamera;
vec3 up;
unsigned char teclas[256];


void atualizaDirecaoCamera() {
    direcaoCamera = sub(&alvoCamera, &posicaoCamera);
}

void atualizaAlvoCamera() {
    alvoCamera = add(&posicaoCamera, &direcaoCamera);
}

void carregaTexturaTerreno() {
    const char caminho[] = "mapa-altura.png";
    texturaTerreno.pixels = SOIL_load_image(
                                caminho,
                                &texturaTerreno.largura, &texturaTerreno.altura, &texturaTerreno.canais,
                                SOIL_LOAD_L
                            );
    if (texturaTerreno.pixels == 0) {
        const char* resultado = SOIL_last_result();
        printf("Erro ao carregar a imagem %s: %s\n", caminho, resultado);
    }
}

void inicializa() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);

    vec4 luzAmbiente = {{{ 0.3, 0.3, 0.3, 1.0 }}};
    vec4 luzDifusaEspecular = {{{ 1.0, 1.0, 1.0, 0.0 }}};
    vec4 luzPosicao = {{{-1.0, -1.0, 0.0, 0.0 }}};
    vec4 ambienteGlobal = {{{ 0.3, 0.3, 0.3, 1.0 }}};
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente.v);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusaEspecular.v);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzDifusaEspecular.v);
    glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao.v);

    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambienteGlobal.v);

    vec4 materialAmbienteDifusa = {{{0.42, 0.27, 0.07, 1.0}}};
    vec4 materialEspecular = {{{ 0.1, 0.1, 0.1, 1.0 }}};
    float materialExpoente = 20.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialAmbienteDifusa.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialEspecular.v);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialExpoente);

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

vec3 calculaNormalFace(vec3 p1, vec3 p2, vec3 p3) {
    vec3 v1 = sub(&p1, &p2);
    vec3 v2 = sub(&p3, &p1);

    vec3 normal = cross(&v1, &v2);
    normal = normalize(&normal);

    return normal;
};

float getAlturaEm(float x, float z, struct textura tex, float alturaMaxima) {
    int s = (int)floor((((x+1)/2) * (tex.largura-1)));
    int t = (int)floor((((z+1)/2) * (tex.altura-1)));
    unsigned char tomDeCinza = tex.pixels[t * tex.largura + s];
    float y = (tomDeCinza/255.0f) * alturaMaxima;

    return y;
}

void desenhaCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(posicaoCamera.x, posicaoCamera.y, posicaoCamera.z,
              alvoCamera.x, alvoCamera.y, alvoCamera.z,
              0.0, 1.0, 0.0);
    if (modoPreenchido) {
        glPolygonMode(GL_FRONT, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    glPushMatrix();
        const float resolucao = 100;
        const float incremento = 2/resolucao;
        glScalef(200, 1, 200);
        for(float z = 1.0; z >= -1.0 + incremento; z -= incremento) {
            glBegin(GL_TRIANGLE_STRIP); {
                // longe da câmera
                // v0----v2----v4
                //  |   / |   / |
                //  | /   | /   |
                // v1----v3----v5
                // perto da câmera
                // a cada iteração deste for, cria 2 vértices
                for(float x = -1; x <= 1; x += incremento) {
                    int terceiroVerticeEmDiante = z < 1.0 - incremento ? TRUE : FALSE;

                    //vn: (x,z-1)
                    float yv0 = getAlturaEm(x, z-incremento, texturaTerreno, alturaMaxima);
                    if (terceiroVerticeEmDiante) {
                        vec3 vAtual = {{{x, yv0, z-incremento}}};
                        vec3 vMenos1 = {{{x-incremento, getAlturaEm(x-incremento, z-incremento, texturaTerreno, alturaMaxima), z-incremento}}};
                        vec3 vMenos2 = {{{x-incremento, getAlturaEm(x-incremento, z, texturaTerreno, alturaMaxima), z}}};
                        vec3 normal = calculaNormalFace(vAtual, vMenos1, vMenos2);
                        glNormal3fv(normal.v);
                    }
                    glVertex3f(x, yv0, z-incremento);

                    //vn+1: (x,z)
                    float yv1 = getAlturaEm(x, z, texturaTerreno, alturaMaxima);
                    if (terceiroVerticeEmDiante) {
                        vec3 vAtual = {{{x, yv1, z}}};
                        vec3 vMenos1 = {{{x, getAlturaEm(x, z-incremento, texturaTerreno, alturaMaxima), z-incremento}}};
                        vec3 vMenos2 = {{{x-incremento, getAlturaEm(x-incremento, z, texturaTerreno, alturaMaxima), z}}};
                        vec3 normal = calculaNormalFace(vAtual, vMenos1, vMenos2);
                        glNormal3fv(normal.v);
                    }
                    glVertex3f(x, yv1, z);
                }
            }
            glEnd();
        }
    glPopMatrix();

    glutSwapBuffers();
}


void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ((float)w)/h, 0.1, 4000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void teclado(unsigned char key, int x, int y) {
    teclas[key] = TRUE;
    switch(key) {
    case 27:
        exit(0);
        break;
    case 'f':
    case 'F':
        modoPreenchido = !modoPreenchido;
        break;
    case '+':
        alturaMaxima += 0.5f;
        break;
    case '-':
        alturaMaxima -= 0.5f;
        break;
    case 'c':
    case 'C':
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

void tecladoUp(unsigned char key, int x, int y) {
    teclas[key] = FALSE;
}

void atualiza(int periodo) {
    vec3 deslocamento;
    vec3 temp1, temp2;

    if (teclas['w'] || teclas['W']) {
        temp1 = normalize(&direcaoCamera);
        posicaoCamera = add(&posicaoCamera, &temp1);
        atualizaAlvoCamera();
    } else if (teclas['s'] || teclas['S']) {
        temp1 = normalize(&direcaoCamera);
        posicaoCamera = sub(&posicaoCamera, &temp1);
        atualizaAlvoCamera();
    }

    if (teclas['d'] || teclas['D']) {
        temp1 = cross(&direcaoCamera, &up);
        deslocamento = normalize(&temp1);
        temp2 = mult(&deslocamento, 1);
        direcaoCamera = add(&direcaoCamera, &temp2);
        atualizaAlvoCamera();
    } else if (teclas['a'] || teclas['A']) {
        temp1 = cross(&direcaoCamera, &up);
        deslocamento = normalize(&temp1);
        temp2 = mult(&deslocamento, 1);
        direcaoCamera = sub(&direcaoCamera, &temp2);
        atualizaAlvoCamera();
    }

    glutPostRedisplay();
    glutTimerFunc(periodo, atualiza, periodo);
}

void imprimeAjuda() {
    printf("Interacao:\n");
    printf("\t[f]: ativa/destiva o preenchimento dos triangulos (glPolygonMode).\n");
    printf("\t[w],[a],[s],[d]: navega na cena.\n");
    printf("\t[c]: coloca a camera no topo, olhando para o terreno.\n");
}

int main(int argc, char **argv) {
    imprimeAjuda();
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mapa de Alturas");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutKeyboardUpFunc(tecladoUp);
    glutTimerFunc(33, atualiza, 33);

    inicializa();

    glutMainLoop();
}
