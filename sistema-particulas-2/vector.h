#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <string>

using namespace std;

/**
  * Este arquivo (.h) descreve 2 classes:
  *	  - Vector3: vetor de tamanho 3 (s� este ser� necess�rio na primeira parte (15/01/15))
  *   - Vector4: vetor de tamanho 4
  *
  * O que � uma classe?
  *    - Apenas uma struct, mas que tamb�m pode conter fun��es (o nome certo passa a ser "m�todo")
  *    - Para criar um objeto � partir de um struct em C:
  *		    struct Pessoa {
  *           char nome[50];
  *           int idade;
  *         }
  *         Pessoa professor;
  *         professor.nome = "Flavio";
  *         professor.idade = 18;
  *	   - Para criar um objeto � partir de uma classe em C++, h� duas formas:
  *         class Pessoa {
  *           char nome[50];
  *           int idade;
  *         public:
  *           Pessoa(char nome[50], int idade) {
  *				this->nome = nome;
  *				this->idade = idade;
  *         }
  *         Pessoa professor = Professor("Flavio", 18);
  *			// ou ent�o, via ponteiro e usando "new"
  *			Pessoa* professor = new Professor("Flavio", 18);
  */

class Vector3 {

public:
	union {
		float components[3];
		struct {
			float x, y, z;
		};
		struct {
			float r, g, b;
		};
	};


	// construtores
	Vector3(float, float, float);
	Vector3(float[]);
	Vector3(const Vector3&);
	Vector3() {}

	// opera��es
	Vector3 add(Vector3&);			// imut�vel (n�o altera o Vector3, apenas retorna o resultado)
	Vector3 add(float);				// imut�vel
	Vector3 diff(Vector3&);			// imut�vel
	Vector3 diff(float);			// imut�vel
	Vector3 mult(float);			// imut�vel
	Vector3 cross(Vector3&);		// imut�vel
	float dotProduct(Vector3&);		// imut�vel
	float norm();					// imut�vel
	void normalize();				// mut�vel  (altera o Vector3 e n�o retorna nada)
	Vector3 normalized();			// imut�vel
	void truncate();				// mut�vel

	string printDebugInfo();
};




class Vector4 {
public:
	union {
		float components[4];
		struct {
			float x, y, z, w;
		};
		struct {
			float r, g, b, a;
		};
	};

	// construtores
	Vector4(float, float, float, float);
	Vector4(float[]);

	// opera��es
	Vector4 add(Vector4&);			// imut�vel
	Vector4 add(float);				// imut�vel
	Vector4 diff(Vector4&);			// imut�vel
	Vector4 diff(float);			// imut�vel
	//Vector4 cross(Vector4&);		// imut�vel
	float dotProduct(Vector4&);		// imut�vel
	float norm();					// imut�vel
	void normalize();				// mut�vel
};


#endif
