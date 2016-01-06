#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <string>

using namespace std;

/**
  * Este arquivo (.h) descreve 2 classes:
  *	  - Vector3: vetor de tamanho 3 (só este será necessário na primeira parte (15/01/15))
  *   - Vector4: vetor de tamanho 4
  *
  * O que é uma classe?
  *    - Apenas uma struct, mas que também pode conter funções (o nome certo passa a ser "método")
  *    - Para criar um objeto à partir de um struct em C:
  *		    struct Pessoa {
  *           char nome[50];
  *           int idade;
  *         }
  *         Pessoa professor;
  *         professor.nome = "Flavio";
  *         professor.idade = 18;
  *	   - Para criar um objeto à partir de uma classe em C++, há duas formas:
  *         class Pessoa {
  *           char nome[50];
  *           int idade;
  *         public:
  *           Pessoa(char nome[50], int idade) {
  *				this->nome = nome;
  *				this->idade = idade;
  *         }
  *         Pessoa professor = Professor("Flavio", 18);
  *			// ou então, via ponteiro e usando "new"
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

	// operações
	Vector3 add(Vector3&);			// imutável (não altera o Vector3, apenas retorna o resultado)
	Vector3 add(float);				// imutável
	Vector3 diff(Vector3&);			// imutável
	Vector3 diff(float);			// imutável
	Vector3 mult(float);			// imutável
	Vector3 cross(Vector3&);		// imutável
	float dotProduct(Vector3&);		// imutável
	float norm();					// imutável
	void normalize();				// mutável  (altera o Vector3 e não retorna nada)
	Vector3 normalized();			// imutável
	void truncate();				// mutável

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

	// operações
	Vector4 add(Vector4&);			// imutável
	Vector4 add(float);				// imutável
	Vector4 diff(Vector4&);			// imutável
	Vector4 diff(float);			// imutável
	//Vector4 cross(Vector4&);		// imutável
	float dotProduct(Vector4&);		// imutável
	float norm();					// imutável
	void normalize();				// mutável
};


#endif
