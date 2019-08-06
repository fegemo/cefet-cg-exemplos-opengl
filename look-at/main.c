#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "coordenadas.h"
#include "texto.h"
#include "camera.h"

// modos de câmera disponíveis e qual é o modo atual
//enum CAMERAS { TERCEIRA_PESSOA = 0, PRIMEIRA_PESSOA, ESTATICA };
// int modoCamera = TERCEIRA_PESSOA;

// estrutura de dados que representará as coordenadas da câmera
// e uma variável que é um vetor de câmeras
//typedef struct {
//  ponto posicao;
//  ponto alvo;
//  vetor cima;
//} Camera;
//Camera cameras[3];
//Camera *camera = &cameras[ESTATICA];
Camera camera;

//char *textoCameraAtual = "Estatica (3)";


int windowWidth;
int windowHeight;

//int xMouse = 0, yMouse = 0;     //variáveis globais que serão usadas na função posicionaCamera
//int xCursor, yCursor, zCursor;  //guarda o centro do cursor
//float phi = 0, teta = 0;       //ângulos das coordenadas esféricas


//void moverParaFrente(int sentido) {
//    vetor frente = diferencaEntrePontos(cameras[PRIMEIRA_PESSOA].alvo, cameras[PRIMEIRA_PESSOA].posicao);
//    frente = multiplicaPorEscalar(frente, sentido);
//    frente = normalizado(frente);
//
//    // switch (modoCamera) {
//    // case TERCEIRA_PESSOA:
//    //     break;
//    // case PRIMEIRA_PESSOA:
//    cameras[PRIMEIRA_PESSOA].posicao = somaPontoComVetor(cameras[PRIMEIRA_PESSOA].posicao, frente);
//    //     break;
//    // default:
//    //     break;
//    // }
//}

//void moverParaLado(int sentido) {
//    vetor frente = diferencaEntrePontos(cameras[PRIMEIRA_PESSOA].alvo, cameras[PRIMEIRA_PESSOA].posicao);
//    frente = normalizado(frente);
//
//    vetor lado = produtoVetorial(frente, cameras[PRIMEIRA_PESSOA].cima);
//    lado = multiplicaPorEscalar(lado, sentido);
//
//    cameras[PRIMEIRA_PESSOA].posicao = somaPontoComVetor(cameras[PRIMEIRA_PESSOA].posicao, lado);
//    cameras[PRIMEIRA_PESSOA].alvo = somaPontoComVetor(cameras[PRIMEIRA_PESSOA].alvo, lado);
//}

//void teclado(unsigned char key, int x, int y) {
//    switch (key) {
//        case 27:    //aperte ESC para fechar
//            exit(0);
//            break;
//        //andar pelo plano X-Z utilizando W A S D
//        case 's':
//        case 'S':
////             xCursor++;
//            moverParaFrente(-1);
//            break;
//        case 'w':
//        case 'W':
//            // xCursor--;
//            moverParaFrente(+1);
//            break;
//        case 'a':
//        case 'A':
//            moverParaLado(-1);
//            break;
//        case 'd':
//        case 'D':
//            moverParaLado(+1);
//            break;
//            recebeTecladoCamera(camera, key);
//        case '1':
//            camera = &cameras[TERCEIRA_PESSOA];
//            // modoCamera = TERCEIRA_PESSOA;
//            textoCameraAtual = "Terceira pessoa (1)";
//            break;
//        case '2':
//            camera = &cameras[PRIMEIRA_PESSOA];
//            // modoCamera = PRIMEIRA_PESSOA;
//            textoCameraAtual = "Primeira pessoa (2)";
//            break;
//        case '3':
//            camera = &cameras[ESTATICA];
//            // modoCamera = ESTATICA;
//            textoCameraAtual = "Estatica (3)";
//        default:
//            break;
//    }
//}

void teclado2(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case 'w':
    case 'W':
    case 's':
    case 'S':
    case 'd':
    case 'D':
    case 'a':
    case 'A':
        recebeTecladoCamera(&camera, tecla);
        glutPostOverlayRedisplay();
        break;
    case 27:
        exit(0);
        break;
    }
}

// callback de mudança na posição do mouse
//void posicionaCamera(int x, int y) {
//    // variáveis que guardam o vetor 2D de movimento do mouse na tela
//    // xMouse e yMouse são os valores de x e y no frame anterior
//    float xChange = ((float)x - xMouse) / larguraTela;
//    float yChange = ((float)y - yMouse) / alturaTela;
//
//    // vetor desvio = { xChange / larguraTela, -yChange / alturaTela, 0, 0 };
//    teta += xChange;
//    phi += yChange;
//    vetor desvio = { 0, 0, 0, 0 };
//    desvio.x = sin(teta * M_PI / 180);
//    desvio.z = cos(teta* M_PI / 180);
//    desvio.y = sin(phi* M_PI / 180);
//    desvio = multiplicaPorEscalar(desvio, .2);
//
//    vetor frente = normalizado(diferencaEntrePontos(cameras[PRIMEIRA_PESSOA].alvo, cameras[PRIMEIRA_PESSOA].posicao));
//    vetor lado = normalizado(produtoVetorial(frente, cameras[PRIMEIRA_PESSOA].cima));
//    float anguloDaCameraEmXZ = anguloEntreVetores(frente, lado);
//
//    // switch (modoCamera) {
//    // case TERCEIRA_PESSOA:
//    //     break;
//    // case PRIMEIRA_PESSOA:
//    cameras[PRIMEIRA_PESSOA].alvo = somaPontoComVetor(cameras[PRIMEIRA_PESSOA].alvo, desvio);
//    //     break;
//    //
//    // case ESTATICA:
//    // default:
//    //     break;
//    // }
//
//    // guarda o x e y do mouse para usar na comparação do próximo frame
//    xMouse = x;
//    yMouse = y;
//
//    if (x > larguraTela * 0.8 || x < larguraTela * 0.2 || y > alturaTela * 0.8 || y < alturaTela * 0.2) {
//      glutWarpPointer(larguraTela / 2, alturaTela / 2);
//    }
//}

void posicionaCamera2(int x, int y) {

    float xMouseNaCamera = ((float) x) / windowWidth * 2 - 1;
    float yMouseNaCamera = ((float) windowHeight - y) / windowHeight * 2 - 1;

    recebeMouseCamera(&camera, xMouseNaCamera, yMouseNaCamera);

    // mantém o mouse dentro da janela, fazendo uma revolução quando chega
    // em um canto
    if (xMouseNaCamera > 0.98f) {
        glutWarpPointer(0.01f * windowWidth, y);
        camera.cursorMouse.x = -0.98f;
    } else if (xMouseNaCamera < -0.98f) {
        glutWarpPointer(0.99f * windowWidth, y);
        camera.cursorMouse.x = 0.98f;
    }

    glutPostOverlayRedisplay();
}

// callback de atualização
void atualiza(int timer) {
    glutPostRedisplay();
    glutTimerFunc(timer, atualiza, timer);
}

void redimensiona(int w, int h){
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);                //define a proporção da janela de visualização

    glMatrixMode(GL_PROJECTION);           //define o tipo de matriz de transformação que será utilizada
    glLoadIdentity();                       //carrega a matriz identidade do tipo GL_PROJECTION configurado anteriormente
    gluPerspective(60.0, (float)w/(float)h, 0.2, 400.0);    //funciona como se fosse o glOrtho, mas para o espaço 3D

    glMatrixMode(GL_MODELVIEW);                             //ativa o modo de matriz de visualização para utilizar o LookAt
    glLoadIdentity();                       //carrega a matriz identidade do tipo GL_PROJECTION configurado anteriormente
}

void inicializa(){
    glClearColor(1, 1, 1, 1);                           // cor de fundo branca
    glEnable(GL_DEPTH_TEST);                            // ativa o Z buffer
    glEnable(GL_BLEND);                                 // ativa a mesclagem de cores
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // ativando o blend, podemos criar objetos transparentes
    // inicializa as três câmeras
    // cameras[TERCEIRA_PESSOA] = ;
//    cameras[PRIMEIRA_PESSOA] = (Camera){
//      {0, 0, 200, 1},
//      {0, 0,   0, 1},
//      {0, 1,   0, 0}
//    };
//    cameras[ESTATICA] = (Camera){
//      {0, 0, 200, 1},
//      {0, 0,   0, 1},
//      {0, 1,   0, 0}
//    };

    inicializaCamera(&camera);
}

void desenhaCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // carrega a matriz identidade do modelo de visualização,
    // sempre utilize antes de usar LookAt
    glLoadIdentity();

//    gluLookAt(
//      camera->posicao.x, camera->posicao.y, camera->posicao.z,
//      camera->alvo.x, camera->alvo.y, camera->alvo.z,
//      camera->cima.x, camera->cima.y, camera->cima.z
//    );
    posicionaCamera(&camera);

    // desenha uma linda jarra em (0, 0, 0)
    glColor3f(0, 0, 0);
    glutWireTeapot(50);

    glPushMatrix();
        // desenha uma 'mesa' no ponto (0, -100, 0)
        glColor3f(0.5, 0.5, 0.5);
        glTranslatef(0, -100, 0);
        glutSolidCube(100);

        // desenha bordas de um cubo levemente maior
        glColor3f(0, 0, 0);
        glutWireCube(105);
    glPopMatrix();

//    escreveTexto(GLUT_BITMAP_HELVETICA_18, textoCameraAtual, 20, 40, 0);

    glutSwapBuffers();
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Exemplo LookAt");
    // glutEnterGameMode();                 // fullscreen baby! (retire o comentário para ativar a tela cheia)
    // glutSetCursor(GLUT_CURSOR_NONE);     // esconde o cursor do sistema


    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    // atualização próxima de 60fps (1000/16 = 62.5 fps
    glutTimerFunc(16, atualiza, 16);

    glutKeyboardFunc(teclado2);
    // usada para capturar o posicionamento do mouse
    glutPassiveMotionFunc(posicionaCamera2);

    inicializa();
    glutMainLoop();

    return 0;
}
