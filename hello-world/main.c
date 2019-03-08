#include <GL/glew.h>
#include <GL/freeglut.h>

void desenhaMinhaCena() {
    // cor para limpar a tela
    glClearColor(1, 1, 1, 1);      // branco

    // limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // começa a usar a cor verde
    glColor3f(0, 1, 0);

    // começa a desenhar um polígono com os vértices especificados
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f( 0.5, -0.5, 0.0);
        glVertex3f( 0.5,  0.5, 0.0);
        glVertex3f(-0.5,  0.5, 0.0);
    glEnd();

    // diz ao OpenGL para colocar o que desenhamos na tela
    glFlush();
}


int main(int argc, char **argv)
{
    // acordando o GLUT
    glutInit(&argc, argv);

    // definindo a versão do OpenGL que vamos usar
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    // configuração inicial da janela do GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    // abre a janela
    glutCreateWindow("Hello World");

    // registra callbacks para alguns eventos
    glutDisplayFunc(desenhaMinhaCena);

    // entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
