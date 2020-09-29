#include <iostream>
#include <math.h>
#include <chrono>

#define PI 3.141592653589793f
#define TABLE_SIZE 360

using namespace std;

int main()
{
	/*
	Como tg(x) é um função eu preciso de uma tabela dupla, uma para a imagem e outra para o domínio (uma para x e outra para tg(x)).
	Já que a precisão é de 0.25°, 360°/0.25° = 1440 valores. Porém a função tg é periódica de período 180°, então assim eu precisaria
	de só metade dos 1440. Além de ser periódica a função tg também é ímpar (tg(-x) = -tg(x)), sendo assim pode ser usado o intervalo
	-90° até 90° e só teria que usar os valores para os ângulos positivos o que reduz pela metade denovo a quantidade de valores e assim
	teria que guardar 360 valores. Como é uma tabela dupla 360 x 2 = 720 valores.
	*/

	//parte de criação da tabela
	float tgTable[TABLE_SIZE][2];								//array de array onde o array de 2 posições é: [0] = x; [1] = tg(x)
	float x = 0.0f;												//variável auxiliar

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
			tgTable[i][1] = tan(x * (PI / 180.0));				//valor de tg(x), onde (PI / 180.0) é para transformar de grau para rad
			x = x + 0.25f;										//passo
		}
	}	
	
	auto inicio = chrono::high_resolution_clock::now();
	//parte que substituiria a função atan2
	float value = 2.456f;										//variával de teste, no programa principal seria o (y/x)
	bool isNegative = value < 0;								//preciso dela já que reduzi o número de valores pelo fato de tg(x) ser ímpar
		
	if (isNegative)
	{
		value = value * (-1);									//preciso dos valores positivos para os cálculos
	}

	if (value > tgTable[TABLE_SIZE - 1][1])						//aqui é onde o valor fica próxima da assíntota e assume o valor máximo da tabela, 89.75°
	{
		value = tgTable[TABLE_SIZE - 1][0];						//valor em grau, depois será convertido para rad
	}
	else
	{
		for (short i = 0; i < TABLE_SIZE - 1; i++)
		{
			/*
			Aqui é o seguinte: o valor de tg(x) é comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do próximo
			então ele está neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a variável supDif (diferença superior) que é a diferença entre
			o valor de teste e o do proximo ciclo da tabela, e a variável infDiff (diferença inferior) que é a diferença entre o valor da tabela do ciclo
			atual e o valor de teste. Se infDiff < supDiff então o mais preciso é usar o valor da tabela do ciclo atual, se falso então usa-se o valor da
			tabela do próximo ciclo. Ex.: se value fosse 8, ciclo atual 5 e próximo fosse 10; então supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o 
			valor aproximado mais correto seria o 10 por estar mais próximo de 8 do que o 5.
			*/
			if (value > tgTable[i][1] && value < tgTable[i + 1][1])
			{
				float supDif = tgTable[i + 1][1] - value;
				float infDif = value - tgTable[i][1];

				if (supDif > infDif)
				{
					value = tgTable[i][0];						//valor em grau, depois será convertido para rad
				}
				else
				{
					value = tgTable[i + 1][0];					//valor em grau, depois será convertido para rad
				}
				break;
			}
		}
		

		if (isNegative)
		{
			//Peculiaridade da função tg(x) por ser ímpar
			value = 90 + (90 - value);
		}

		value = value * (PI / 180.0);							//convertendo o valor para rad
	}
	auto resultado = chrono::high_resolution_clock::now() - inicio;
	long long microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(resultado).count();

	cout << value << endl << microseconds << endl;

	
}