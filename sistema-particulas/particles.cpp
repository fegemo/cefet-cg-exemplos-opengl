#include <SOIL.h>
#include <gl\glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_PARTICLES 100
#define MAX_PARTICLE_AGE 300
#define MAX_BOUNCE_COUNT 5

void redimensiona(int width, int height);
void setup();
void atualiza(int);
void desenhaCena();
void teclado(unsigned char, int, int);
void tecladoSolto(unsigned char, int, int);
void carregaTextura(const char* arquivo, int* idTextura);

void inicializaParticulas();
void atualizaParticulas();
void ativaParticulas();
void ajustaParticulas();
void desenhaParticulas();


bool keys[256];

float Size = 0.2f;

float R = 0.8f;
float G = 0.2f;
float B = 0.0f;
float cR = 0.001f;
float cG = 0.002f;
float cB = 0.003f;

int texturaParticula;
int texturaPlano;


struct particle {

	float X,Y,Z;		// Current position
	float sX,sY,sZ;		// Current Speed/Movement
	float tX,tY,tZ;		// Target Speed/Movement
	float R,B,G;		// Particle Colour
	bool Active;		// Is particle Active
	int Age;			// Age of the particle
	int MaxAge;			// Maximum Age before particle dies
	int BounceCount;

} particles[MAX_PARTICLES];


void teclado(unsigned char key, int x, int y) {
    keys[key] = true;
    switch (key) {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void tecladoSolto(unsigned char key, int x, int y) {
    keys[key] = false;
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sistema de partículas");

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardUpFunc(tecladoSolto);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, atualiza, 0);
    setup();

    glutMainLoop();
    return 0;
}


void redimensiona(int largura, int altura) {
    altura = fmax(1, altura);

	glViewport(0,0, largura, altura);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)largura/(GLfloat)altura,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setup() {
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	inicializaParticulas();
}

void atualiza(int t) {
	atualizaParticulas();
	desenhaCena();
	glutTimerFunc(33, atualiza, 0);
}

void desenhaCena() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, -1.0f, -10.0f);

	glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);
		desenhaParticulas();
	glPopMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

	glColor4f(0.0f, 0.2f, 0.2f, 0.1f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaPlano);

	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);		glVertex3f(-10.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 0.0f);		glVertex3f( 10.0f, 0.0f, 10.0f);
		glTexCoord2f(1.0f, 1.0f);		glVertex3f( 10.0f, 0.0f, -10.0f);
		glTexCoord2f(0.0f, 1.0f);		glVertex3f(-10.0f, 0.0f, -10.0f);
	glEnd();

	desenhaParticulas();

    glutSwapBuffers();
}

void atualizaParticulas(){
	ativaParticulas();
	ajustaParticulas();
}

void inicializaParticulas(){
	int p;

	srand((int)time(NULL));

	for(p=0; p<MAX_PARTICLES; p++){

		particles[p].Active = false;
		particles[p].tX = 0.0f;
		particles[p].tY = -0.1f;
		particles[p].tZ = 0.0f;

	}

    carregaTextura("particula.bmp", &texturaParticula);
    carregaTextura("plano.bmp", &texturaPlano);
}

void carregaTextura(const char* arquivo, int* idTextura) {
    *idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    if (*idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
}

void ativaParticulas(){

	int p;

	for(p=0; p<MAX_PARTICLES; p++){

		if(!particles[p].Active){
			// Start the particle at 0,0,0 origin
			particles[p].X = 0.0f;
			particles[p].Y = 0.0f;
			particles[p].Z = 0.0f;

			// The following lines set a random speed value
			particles[p].sX = (((float)((rand() % 100) + 1)) / 1000.0f) - 0.05f;
			particles[p].sY = (((float)((rand() % 100) + 50)) / 500.0f);
			particles[p].sZ = (((float)((rand() % 100) + 1)) / 1000.0f) - 0.05f;

			// We also activate the particle
			particles[p].Active = true;
			// Set it's Age to zero
			particles[p].Age = 0;
			// We also assign a max age to the particles
			particles[p].MaxAge = MAX_PARTICLE_AGE;
			// We Also reset the bouncecount to zero
			particles[p].BounceCount = 0;

			particles[p].R = R;
			particles[p].G = G;
			particles[p].B = B;

			R+=cR;
			G+=cG;
			B+=cB;

			if(R>1.0f){R=1.0f; cR=-cR;}if(R<0.0f){R=0.0f; cR=-cR;}
			if(G>1.0f){G=1.0f; cG=-cG;}if(G<0.0f){G=0.0f; cG=-cG;}
			if(B>1.0f){B=1.0f; cB=-cB;}if(B<0.0f){B=0.0f; cB=-cB;}

			return;
		}
	}
}

void ajustaParticulas(){

	int p;

	for(p=0; p<MAX_PARTICLES; p++){

		// We move the speed towards the target speed by 1/20 (5%)
		particles[p].sX+= (particles[p].tX - particles[p].sX) / 20.0f;
		particles[p].sY+= (particles[p].tY - particles[p].sY) / 20.0f;
		particles[p].sZ+= (particles[p].tZ - particles[p].sZ) / 20.0f;
		// Then we adjust the position of the particle by the new speed
		particles[p].X+= particles[p].sX;
		particles[p].Y+= particles[p].sY;
		particles[p].Z+= particles[p].sZ;

		// Now for the bounce code.
		if(particles[p].Y < 0.0f){
			particles[p].Y = 0.0f;
			particles[p].sY = -particles[p].sY;
			particles[p].BounceCount++;
			if(particles[p].BounceCount > MAX_BOUNCE_COUNT){
				particles[p].Active = false;
			}

		}

		// And finally the age check
		particles[p].Age++;
		if(particles[p].Age > particles[p].MaxAge){
			particles[p].Active = false;
		}
	}
}


void desenhaParticulas() {

	int p;

	// Enable textures and bind our particle texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaParticula);

	// Disable Depth testing.
	glDisable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR,GL_ONE);

	for(p=0; p<MAX_PARTICLES; p++){

		if(particles[p].Active){
			glColor4f(particles[p].R, particles[p].G, particles[p].B, 1.0f);

			glPushMatrix();

				glTranslatef(particles[p].X, particles[p].Y, particles[p].Z);

				glNormal3f(0.0f, 0.0f, 1.0f);

				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f);		glVertex3f(-Size, -Size, 0.0f);
					glTexCoord2f(1.0f, 0.0f);		glVertex3f(Size, -Size, 0.0f);
					glTexCoord2f(1.0f, 1.0f);		glVertex3f(Size, Size, 0.0f);
					glTexCoord2f(0.0f, 1.0f);		glVertex3f(-Size, Size, 0.0f);
				glEnd();

			glPopMatrix();
		}
	}

	glEnable(GL_DEPTH_TEST);
}
