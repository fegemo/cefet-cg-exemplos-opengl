#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


enum CAMERAS { TERCEIRA_PESSOA = 1, PRIMEIRA_PESSOA, ESTATICA };
int modoCAM = TERCEIRA_PESSOA;            //variável responsável por guardar o modo de câmera que está sendo utilizado

int xMouse = 0, yMouse = 0;     //variáveis globais que serão usadas na função posicionaCamera
int xCursor, yCursor, zCursor;  //guarda o centro do cursor
float phi = 90, teta = 0;       //ângulos das coordenadas esféricas


// estrutura de dados que representará as coordenadas da câmera
struct {
  float x, y, z;
  float targetX, targetY, targetZ;
} camera;


void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27:    //aperte ESC para fechar
            exit(0);
            break;
        case 's':   //andar pelo plano X-Z utilizando W A S D
            xCursor++;
            break;
        case 'w':
            xCursor--;
            break;
        case 'a':
            zCursor++;
            break;
        case 'd':
            zCursor--;
            break;
        case '1':
            modoCAM = TERCEIRA_PESSOA;
            break;
        case '2':
            modoCAM = PRIMEIRA_PESSOA;
            break;
        case '3':
            modoCAM = ESTATICA;
        default:
            break;
    }
}

//capturar posicionamento do mouse
void posicionaCamera(int x, int y){
    // variáveis que guardam o vetor 2D de movimento do mouse na tela
    // xMouse e yMouse são os valores de x e y no frame anterior
    float xChange = x - xMouse;
    float yChange = y - yMouse;

    // este exemplo usa coordenadas esféricas para controlar a câmera...
    // teta e phi guardam a conversão do vetor 2D para um espaço 3D
    // com coordenada esférica
    teta = (teta + xChange/150);
    phi = (phi - yChange/150);

    if(phi >= 180){
      //limite de 180 para o phi
      phi = 180;
    }

    // guarda o x e y do mouse para usar na comparação do próximo frame
    xMouse = x;
    yMouse = y;
}


// callback de atualização
void atualiza(int time) {
    glutPostRedisplay();
    glutTimerFunc(time, atualiza, time);
}

void redimensiona(int w, int h){
    glEnable(GL_DEPTH_TEST);                // Ativa o Z buffer
    glViewport (0, 0, w, h);                //define a proporção da janela de visualização
    glMatrixMode (GL_PROJECTION);           //define o tipo de matriz de transformação que será utilizada
    glLoadIdentity();                       //carrega a matriz identidade do tipo GL_PROJECTION configurado anteriormente
    gluPerspective(60.0, (float)w/(float)h, 0.2, 400.0);    //funciona como se fosse o glOrtho, mas para o espaço 3D
    glMatrixMode(GL_MODELVIEW);                             //ativa o modo de matriz de visualização para utilizar o LookAt
}

void inicializa(){
    glClearColor(1, 1, 1, 1);                          //cor de fundo branca
    glEnable(GL_BLEND);                                //ativa a mesclagem de cores
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //ativando o blend, podemos criar objetos transparentes
    xCursor = 0;                                       //a câmera começa olhando para o ponto 0
    yCursor = 0;
    zCursor = 0;
}

//função que desenhará tudo o que aparece na tela
void desenhaCena() {
    //esfera de raio 100
    camera.x = 100 * sin(phi) * cos(teta);  //coordenada x denotada em coordenadas esféricas
    camera.z = 100 * sin(phi) * sin(teta); //coordenada z denotada em coordenadas esféricas
    camera.y = 100 * cos(phi);          //coordenada y denotada em coordenadas esféricas

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // carrega a matriz identidade do modelo de visualização,
    // sempre utilize antes de usar LookAt
    glLoadIdentity();

    //define um LookAt diferente para cada modo da câmera, veja abaixo o
    // uso de cada um
    switch (modoCAM) {
    case TERCEIRA_PESSOA:
        gluLookAt(xCursor+camera.x, camera.y, zCursor+camera.z,//câmera posicionada na casca da esfera calculada (terceira pessoa)
            xCursor+0, 0, zCursor+0,                          //centro da esfera, o ponto em que estamos olhando
            0, 1, 0);                                        //vetor UP, apontando para o eixo Y (para cima)
        break;

    case PRIMEIRA_PESSOA:
        gluLookAt( xCursor+0, 0, zCursor+0,                    //já aqui, a câmera está posicionada no centro da esfera
            xCursor+camera.x, camera.y, zCursor+camera.z,     //e a câmera estará olhando para a casca da esfera (primeira pessoa)
            0, 1, 0);                                        //vetor UP, apontando para o eixo Y (para cima)
        break;

    case ESTATICA:
    default:
        gluLookAt(0, 0, 200,   // Z=200
                  0, 0, 0,    // (0, 0, 0) origem do mundo
                  0, 1, 0);  //nesse exemplo mais simples, estamos no ponto Z=200 olhando para o ponto 0
        break;
    }


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

    glutSwapBuffers();
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition (0, 0);

    glutCreateWindow("Exemplo LookAt");
    // glutEnterGameMode();                 // fullscreen baby! (retire o comentário para ativar a tela cheia)
    // glutSetCursor(GLUT_CURSOR_NONE);     // esconde o cursor do sistema


    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    // atualização próxima de 60fps (1000/16 = 62.5 fps
    glutTimerFunc(16, atualiza, 16);

    glutKeyboardFunc(teclado);
    // usada para capturar o posicionamento do mouse
    glutPassiveMotionFunc(posicionaCamera);

    inicializa();
    glutMainLoop();

    return 0;
}
