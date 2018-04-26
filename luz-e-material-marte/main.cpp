#include <iostream>
#include <fstream>

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>

using namespace std;

static bool light0Ligada = 1;   // Luz branca ligada?
static bool light1Ligada = 1;   // Luz verde ligada?
static float d = 1.0;           // Intensidade da cor difusa da luz branca
static float e = 1.0;           // Intensidade da cor especular da luz branca
static float m = 0.2;           // Intensidade da luz ambiente global
static float p = 1.0;           // A luz branca é posicional?
static float s = 50.0;          // Expoente especular do material (shininess)
float matShine[] = { s };                       // expoente especular (shininess)
static float xAngle = 0.0, yAngle = 0.0;        // Rotação da luz branca
static long font = (long)GLUT_BITMAP_8_BY_13;   // Fonte usada para imprimir na tela
static char theStringBuffer[10];                // String buffer
static float xMouse = 250, yMouse = 250;        // (x,y) do ponteiro do mouse
static float larguraJanela, alturaJanela;       // (w,h) da janela
static bool isLightingOn = false;               // O sistema de iluminação está ligado?
static float anguloEsferaY = 0;                 // Rotação da esfera em torno do eixo y
static int esferaLados = 200;                   // Quantas subdivisões latitudinais/longitudinais da esfera
static bool localViewer = false;
static int marsTexture;
static bool usarTextura = true;

// Escreve uma cadeia de caracteres
void escreveTextoNaTela(void *font, char *string)
{
    char *c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Converte um número decimal em string
void floatParaString(char * destStr, int precision, float val)
{
    sprintf(destStr,"%f",val);
    destStr[precision] = '\0';
}

// Escreve as informações variáveis na tela
void informacoesTela(void)
{
    glDisable(GL_LIGHTING); // Desabilita iluminação
    glColor3f(.85f, .85f, .85f);

    glRasterPos3f(-1.0, 1.10, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Iluminacao (l): ");
    escreveTextoNaTela((void*)font, (char*)(isLightingOn ? "ligada" : "desligada"));

    floatParaString(theStringBuffer, 4, m);
    glRasterPos3f(-1.0, 1.00, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Luz ambiente global: ");
    glRasterPos3f(-1.0, 0.95, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade (m/M): ");
    escreveTextoNaTela((void*)font, theStringBuffer);

    glRasterPos3f(-1.0, 0.85, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Luz branca: ");
    glRasterPos3f(-1.0, 0.80, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade difusa (d/D): ");
    floatParaString(theStringBuffer, 4, d);
    escreveTextoNaTela((void*)font, theStringBuffer);
    glRasterPos3f(-1.0, 0.75, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Intensidade especular (e/E): ");
    floatParaString(theStringBuffer, 4, e);
    escreveTextoNaTela((void*)font, theStringBuffer);


    glRasterPos3f(-1.0, 0.65, -2.0);
    escreveTextoNaTela((void*)font, (char*)"Material: ");
    glRasterPos3f(-1.0, 0.60, -2.0);
    escreveTextoNaTela((void*)font, (char*)"  - Expoente especular (s/S): ");
    floatParaString(theStringBuffer, 5, s);
    escreveTextoNaTela((void*)font, theStringBuffer);

}

// Configuração inicial do OpenGL e GLUT
void setup(void)
{
    //glClearColor(.4,.4,.4, 0.0);                    // fundo cinza
    glClearColor(0,0,0, 0.0);
    glEnable(GL_DEPTH_TEST);                        // Ativa teste Z

    // Propriedades do material da esfera
    float matAmbAndDif[] = {1.0, 1.0, 1.0, 1.0};    // cor ambiente e difusa: branca
    float matSpec[] = { 1.0, 1.0, 1,0, 1.0 };       // cor especular: branca

    // Definindo as propriedades do material
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Carrega a textura
    marsTexture = SOIL_load_OGL_texture
	(
		"mars-small.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if(marsTexture == 0)
    {
        cout << "Problema ao carregar textura: " << SOIL_last_result() << endl;
    }

    // Não mostrar faces do lado de dentro
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Esconder o ponteiro do mouse quando dentro da janela
    glutSetCursor(GLUT_CURSOR_NONE);
}

// Desenha uma esfera na origem, com certo raio e subdivisões
// latitudinais e longitudinais
//
// Não podemos usar glutSolidSphere, porque ela não chama
// glTexCoord para os vértices. Logo, se você quer texturizar
// uma esfera, não pode usar glutSolidSphere
void solidSphere(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
}

// Callback de desenho
void desenhaCena()
{
    // Propriedades das fontes de luz
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDif0[] = { d, d, d, 1.0 };
    float lightSpec0[] = { e, e, e, 1.0 };
    float lightPos0[] = { 0.0, 0.0, 3.0, p };
    float lightDifAndSpec1[] = { 0.0, 1.0, 0.0, 1.0 };
    float lightPos1[] = { 1.0, 2.0, 0.0, 1.0 };
    float globAmb[] = { m, m, m, 1.0 };

    // Propriedades da fonte de luz LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec0);

    // Propriedades da fonte de luz LIGHT1
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);        // Luz ambiente global
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, localViewer);// Enable local viewpoint

    // Ativa as fontes de luz (branca e verde)
    if (light0Ligada) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }
    if (light1Ligada) {
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }

    // Limpa a tela e o z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    informacoesTela();

    // Posiciona a câmera de acordo com posição x,y do mouse na janela
    gluLookAt(1*(xMouse-larguraJanela/2)/(larguraJanela/16), -1*(yMouse-alturaJanela/2)/(alturaJanela/16) + 3, 5,
              0, 0, 0,
              0, 1, 0);

    // Desabilita iluminação para desenhar as esferas que representam as luzes
    glDisable(GL_LIGHTING);

    // Light0 e esfera indicativa (ou seta)
    glPushMatrix();
        glRotatef(xAngle, 1.0, 0.0, 0.0); // Rotação no eixo x
        glRotatef(yAngle, 0.0, 1.0, 0.0); // Rotação no eixo y
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
        glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
        glColor3f(d, d, d);
        if (light0Ligada)
        {
            if (p) glutWireSphere(0.05, 8, 8); // Esfera indicativa
            else // Seta apontando na direção da fonte de luz direcional
            {
                glLineWidth(3.0);
                glBegin(GL_LINES);
                glVertex3f(0.0, 0.0, 0.25);
                glVertex3f(0.0, 0.0, -0.25);
                glVertex3f(0.0, 0.0, -0.25);
                glVertex3f(0.05, 0.0, -0.2);
                glVertex3f(0.0, 0.0, -0.25);
                glVertex3f(-0.05, 0.0, -0.2);
                glEnd();
                glLineWidth(1.0);
            }
        }
    glPopMatrix();

    // Light1 e sua esfera indicativa
    glPushMatrix();
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glTranslatef(lightPos1[0], lightPos1[1], lightPos1[2]);
        glColor3f(0.0, 1.0, 0.0);
        if (light1Ligada) glutWireSphere(0.05, 8, 8);
    glPopMatrix();

    if (isLightingOn) {
        glEnable(GL_LIGHTING);
    }


    // Define (atualiza) o valor do expoente de especularidade
    matShine[0] = s;
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    glColor3f(1, 1, 1);

    // Desenha a esfera grande e bem arredondada
    if (usarTextura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, marsTexture);
    }
    glPushMatrix();
        glRotatef(anguloEsferaY, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        solidSphere(1.5, esferaLados, esferaLados);
        // glutSolidSphere(1.5, esferaLados, esferaLados);
        // glutSolidCube(1.5);
        // glutSolidTeapot(1.5);
    glPopMatrix();
    if (usarTextura) {
        glDisable(GL_TEXTURE_2D);
    }

    glutSwapBuffers();
}

// Callback do evento de teclado
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'w':
    case 'W':
        if (light0Ligada) light0Ligada = false;
        else light0Ligada = true;
        break;
    case 'g':
    case 'G':
        if (light1Ligada) light1Ligada = false;
        else light1Ligada = true;
        break;
    case 'p':
    case 'P':
        if (p) p = 0.0;
        else p = 1.0;
        break;
    case 'd':
        if (d > 0.0) d -= 0.05;
        break;
    case 'D':
        if (d < 1.0) d += 0.05;
        break;
    case 'e':
        if (e > 0.0) e -= 0.05;
        break;
    case 'E':
        if (e < 1.0) e += 0.05;
        break;
    case 's':
        if (s > 5.0) s -= 2.00;
        break;
    case 'S':
        if (s < 100.0) s += 2.00;
        break;
    case 'm':
        if (m > 0.0) m -= 0.05;
        break;
    case 'M':
        if (m < 1.0) m += 0.05;
        break;
    case 'l':
    case 'L':
        isLightingOn = !isLightingOn;
        break;
    case 't':
    case 'T':
        usarTextura = !usarTextura;
        break;
    case 'v':
    case 'V':
        localViewer = !localViewer;
        break;
    case '+':
        esferaLados += 5;
        esferaLados = min(200, esferaLados);
        break;
    case '-':
        esferaLados -= 5;
        esferaLados = max(20, esferaLados);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// Callback para setas do teclado
void specialKeyInput(int key, int x, int y)
{
    if(key == GLUT_KEY_DOWN)
    {
        xAngle++;
        if (xAngle > 360.0) xAngle -= 360.0;
    }
    if(key == GLUT_KEY_UP)
    {
        xAngle--;
        if (xAngle < 0.0) xAngle += 360.0;
    }
    if(key == GLUT_KEY_RIGHT)
    {
        yAngle++;
        if (yAngle > 360.0) yAngle -= 360.0;
    }
    if(key == GLUT_KEY_LEFT)
    {
        yAngle--;
        if (yAngle < 0.0) yAngle += 360.0;
    }
    glutPostRedisplay();
}

// Callback de redimensionamento
void resize(int w, int h)
{
    larguraJanela = w;
    alturaJanela = h;

    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void posicionaCamera(int x, int y) {
    xMouse = x;
    yMouse = y;;
    glutPostRedisplay();
}

void rotacionaEsfera() {
    anguloEsferaY += .1f;
    glutPostRedisplay();
}

// Imprime a ajuda no console
void imprimirAjudaConsole(void)
{
    cout << "Ajuda:" << endl;
    cout << "  Aperte 'l' para ligar/desligar a iluminacao do OpenGL." << endl
         << "  Aperte 'w' para ligar/desligar a fonte de luz branca." << endl
         << "  Aperte 'g' para ligar/desligar a fonte de luz verde." << endl
         << "  Aperte 'd/D' para aumentar/reduzir a intensidade difusa da luz branca." << endl
         << "  Aperte 'e/E' para aumentar/reduzir a intensidade especular da luz branca." << endl
         << "  Aperte 'm/M' para aumentar/reduzir a intensidade da luz ambiente global." << endl
         << "  Aperte 's/S' para aumentar/reduzir o expoente especular do material." << endl
         << "  Aperte 'p' para alternar entre fonte posicional ou direcional." << endl
         << "  Aperte 't' para alternar entre modo com/sem textura." << endl
         << "  Aperte '+/-' para aumentar/diminuir a redondeza da esfera." << endl
         << "  Aperte as setas para rotacionar a fonte de luz branca" << endl;
}

int main(int argc, char *argv[])
{
    imprimirAjudaConsole();
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Luz e Materiais");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutPassiveMotionFunc(posicionaCamera);
    glutSpecialFunc(specialKeyInput);
    glutIdleFunc(rotacionaEsfera);

    setup();

    glutMainLoop();
}
