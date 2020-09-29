#include <iostream>
#include <math.h>
#include <chrono>

#define PI 3.141592653589793f
#define TABLE_SIZE 360

using namespace std;

int main()
{
	/*
	Como tg(x) � um fun��o eu preciso de uma tabela dupla, uma para a imagem e outra para o dom�nio (uma para x e outra para tg(x)).
	J� que a precis�o � de 0.25�, 360�/0.25� = 1440 valores. Por�m a fun��o tg � peri�dica de per�odo 180�, ent�o assim eu precisaria
	de s� metade dos 1440. Al�m de ser peri�dica a fun��o tg tamb�m � �mpar (tg(-x) = -tg(x)), sendo assim pode ser usado o intervalo
	-90� at� 90� e s� teria que usar os valores para os �ngulos positivos o que reduz pela metade denovo a quantidade de valores e assim
	teria que guardar 360 valores. Como � uma tabela dupla 360 x 2 = 720 valores.
	*/

	//parte de cria��o da tabela
	float tgTable[TABLE_SIZE][2];								//array de array onde o array de 2 posi��es �: [0] = x; [1] = tg(x)
	float x = 0.0f;												//vari�vel auxiliar

	for (short i = 0; i < TABLE_SIZE; i++)
	{	
		if (i == 0)												//tg(0) = 0
		{
			tgTable[i][0] = x;									//valor de x
			tgTable[i][1] = 0;									//valor de tg(x)
			x = x + 0.25f;										//passo
		}
		else
		{
			tgTable[i][0] = x;									//valor de x
			tgTable[i][1] = tan(x * (PI / 180.0));				//valor de tg(x), onde (PI / 180.0) � para transformar de grau para rad
			x = x + 0.25f;										//passo
		}
	}	
	
	auto inicio = chrono::high_resolution_clock::now();
	//parte que substituiria a fun��o atan2
	float value = 2.456f;										//vari�val de teste, no programa principal seria o (y/x)
	bool isNegative = value < 0;								//preciso dela j� que reduzi o n�mero de valores pelo fato de tg(x) ser �mpar
		
	if (isNegative)
	{
		value = value * (-1);									//preciso dos valores positivos para os c�lculos
	}

	if (value > tgTable[TABLE_SIZE - 1][1])						//aqui � onde o valor fica pr�xima da ass�ntota e assume o valor m�ximo da tabela, 89.75�
	{
		value = tgTable[TABLE_SIZE - 1][0];						//valor em grau, depois ser� convertido para rad
	}
	else
	{
		for (short i = 0; i < TABLE_SIZE - 1; i++)
		{
			/*
			Aqui � o seguinte: o valor de tg(x) � comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do pr�ximo
			ent�o ele est� neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a vari�vel supDif (diferen�a superior) que � a diferen�a entre
			o valor de teste e o do proximo ciclo da tabela, e a vari�vel infDiff (diferen�a inferior) que � a diferen�a entre o valor da tabela do ciclo
			atual e o valor de teste. Se infDiff < supDiff ent�o o mais preciso � usar o valor da tabela do ciclo atual, se falso ent�o usa-se o valor da
			tabela do pr�ximo ciclo. Ex.: se value fosse 8, ciclo atual 5 e pr�ximo fosse 10; ent�o supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o 
			valor aproximado mais correto seria o 10 por estar mais pr�ximo de 8 do que o 5.
			*/
			if (value > tgTable[i][1] && value < tgTable[i + 1][1])
			{
				float supDif = tgTable[i + 1][1] - value;
				float infDif = value - tgTable[i][1];

				if (supDif > infDif)
				{
					value = tgTable[i][0];						//valor em grau, depois ser� convertido para rad
				}
				else
				{
					value = tgTable[i + 1][0];					//valor em grau, depois ser� convertido para rad
				}
				break;
			}
		}
		

		if (isNegative)
		{
			//Peculiaridade da fun��o tg(x) por ser �mpar
			value = 90 + (90 - value);
		}

		value = value * (PI / 180.0);							//convertendo o valor para rad
	}
	auto resultado = chrono::high_resolution_clock::now() - inicio;
	long long microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(resultado).count();

	cout << value << endl << microseconds << endl;

	
}