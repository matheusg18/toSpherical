#include <iostream>
#include <math.h>
#include <chrono>

#define PI 3.141592653589793
#define TABLE_SIZE 360
#define MILLION 1000000

unsigned int tgTable[TABLE_SIZE][2];										//array de array onde o array de 2 posições é: [0] = x; [1] = tg(x)

float linearSearch(float tg);

int main()
{
	/*
	Como tg(x) é um função eu preciso de uma tabela dupla, uma para a imagem e outra para o domínio (uma para x e outra para tg(x)).
	Já que a precisão é de 0.25°, 360°/0.25° = 1440 valores. Porém a função tg é periódica de período 180°, então assim eu precisaria
	de só metade dos 1440. Além de ser periódica a função tg também é ímpar (tg(-x) = -tg(x)), sendo assim pode ser usado o intervalo
	-90° até 90° e só teria que usar os valores para os ângulos positivos o que reduz pela metade denovo a quantidade de valores e assim
	teria que guardar 360 valores. Como é uma tabela dupla 360 x 2 = 720 valores.
	*/

	unsigned int x = 0;														//variável auxiliar
	for (short i = 0; i < TABLE_SIZE; i++)
	{	
		if (i == 0)															//tg(0) = 0
		{
			tgTable[i][0] = x;												//valor de x
			tgTable[i][1] = 0;												//valor de tg(x)
			x = x + 25;														//passo
		}
		else
		{
			tgTable[i][0] = x;												//valor de x
			tgTable[i][1] = tan((x / 100.0) * (PI / 180.0)) * MILLION;		//valor de tg(x), onde (PI / 180.0) é para transformar de grau para rad
			x = x + 25;														//passo
		}
	}	
	
	auto inicio = std::chrono::high_resolution_clock::now();

	float linear = linearSearch(0.527686f);

	auto resultado = std::chrono::high_resolution_clock::now() - inicio;
	long long nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(resultado).count();

	std::cout << "x = " <<linear << std::endl << "nanoseconds: " << nanoseconds << std::endl;
}

float linearSearch(float tg)
{
	bool isNegative = tg < 0;												//preciso dela já que reduzi o número de valores pelo fato de tg(x) ser ímpar
	std::cout << "isNegative: " << isNegative << std::endl;
	unsigned int tgInt = isNegative ? tg * MILLION * (-1) : tg * MILLION;
	std::cout << "tgInt: " << tgInt << std::endl;
	unsigned int atgInt = 0;

	if (tgInt > tgTable[TABLE_SIZE - 1][1])									//aqui é onde o valor fica próxima da assíntota e assume o valor máximo da tabela, 89.75°
	{
		std::cout << "Maior que 8975!!" << std::endl;
		atgInt = tgTable[TABLE_SIZE - 1][0];								//valor em grau, depois será convertido para rad
	}
	else
	{
		for (short i = 0; i < TABLE_SIZE - 1; i++)
		{
			/*
			Aqui é o seguinte: o valor de tg(x) é comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do próximo então
			ele está neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a variável supDif (diferença superior) que é a diferença
			entre o valor de teste e o do proximo ciclo da tabela, e a variável infDiff (diferença inferior) que é a diferença entre o valor da tabela do
			ciclo atual e o valor de teste. Se infDiff < supDiff então o mais preciso é usar o valor da tabela do ciclo atual, se falso então usa-se o valor
			da tabela do próximo ciclo. Ex.: se value fosse 8, ciclo atual 5 e próximo fosse 10; então supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o
			valor aproximado mais correto seria o 10 por estar mais próximo de 8 do que o 5.
			*/
			
			std::cout << "============== " << i << " ==============" << std::endl;
			std::cout << "Ciclo atual: tg(x) = " << tgTable[i][1] << "\tx = " << tgTable[i][0] << std::endl;
			std::cout << "Proximo ciclo: tg(x) = " << tgTable[i + 1][1] << "\tx = " << tgTable[i + 1][0] << std::endl;

			if (tgInt >= tgTable[i][1] && tgInt <= tgTable[i + 1][1])
			{				
				unsigned int supDif = tgTable[i + 1][1] - tgInt;
				std::cout << "supDif: " << supDif << std::endl;
				unsigned int infDif = tgInt - tgTable[i][1];
				std::cout << "infDif: " << infDif << std::endl;

				if (supDif > infDif)
				{
					atgInt = tgTable[i][0];									//valor em grau, depois será convertido para rad
				}
				else
				{
					atgInt = tgTable[i + 1][0];								//valor em grau, depois será convertido para rad
				}
				std::cout << "atgInt: " << atgInt << std::endl;
				break;
			}
		}
	}

	if (isNegative)
	{
		//Peculiaridade da função tg(x) por ser ímpar
		atgInt = 9000 + (9000 - atgInt);
	}

	//convertendo o valor para rad
	return atgInt;
}