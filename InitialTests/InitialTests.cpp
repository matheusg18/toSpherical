#include <iostream>
#include <math.h>
#include <chrono>

#define PI 3.141592653589793
#define TABLE_SIZE 360
#define MILLION 1000000

//array de array onde o array de 2 posições é: [0] = x; [1] = tg(x)
unsigned int tgTable[TABLE_SIZE][2];

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

	//passo de construção da tabela onde a precisão é 0.25 mas é multiplicado por 100 para virar int, assim aumenta 25 a cada ciclo
	unsigned int x = 0;

	for (short i = 0; i < TABLE_SIZE; i++)
	{	
		if (i == 0)																//tg(0) = 0
		{
			tgTable[i][0] = 0;													//valor de x
			tgTable[i][1] = 0;													//valor de tg(x)
			x = x + 25;															//passo
		}
		else
		{
			tgTable[i][0] = x;													//valor de x
			tgTable[i][1] = tan((x / 100.0) * (PI / 180.0)) * MILLION;			//valor de tg(x), (PI / 180.0) é para transformar de grau para rad e * MILLION é para transformar de float para int com precisão de 6 casas
			x = x + 25;															//passo
		}
	}	
	
	auto inicio = std::chrono::high_resolution_clock::now();

	float linear = linearSearch(100.0f);

	auto resultado = std::chrono::high_resolution_clock::now() - inicio;
	long long nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(resultado).count();

	std::cout << "x = " <<linear << std::endl << "nanoseconds: " << nanoseconds << std::endl;
}

float linearSearch(float tg)
{
	bool isNegative = tg < 0;													//preciso dela já que reduzi o número de valores pelo fato de tg(x) ser ímpar
	unsigned int tgInt = isNegative ? tg * MILLION * (-1) : tg * MILLION;		//transformando o float tg no int tgInt para melhor precisão na comparação
	unsigned int atgInt = 0;													//arco tangente relativo à tgInt, em graus °

	if (tgInt > tgTable[TABLE_SIZE - 1][1])										//aqui é onde o valor fica próxima da assíntota e assume o valor máximo da tabela, 89.75°
	{
		atgInt = tgTable[TABLE_SIZE - 1][0];
	}
	else
	{
		for (short i = 0; i < TABLE_SIZE - 1; i++)
		{
			/*
			Aqui é o seguinte: o valor de tgInt é comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do próximo então
			ele está neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a variável supDif (diferença superior) que é a diferença
			entre o valor de tgInt e o do proximo ciclo da tabela, e a variável infDiff (diferença inferior) que é a diferença entre o valor da tabela do
			ciclo atual e o valor de tgInt. Se infDiff < supDiff então o mais preciso é usar o valor da tabela do ciclo atual, se falso então usa-se o valor
			da tabela do próximo ciclo. Ex.: se tgInt fosse 8, ciclo atual 5 e próximo fosse 10; então supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o
			valor aproximado mais correto seria o 10 por estar mais próximo de 8 do que o 5.
			*/

			if (tgInt >= tgTable[i][1] && tgInt <= tgTable[i + 1][1])
			{
				unsigned int supDif = tgTable[i + 1][1] - tgInt;
				unsigned int infDif = tgInt - tgTable[i][1];

				atgInt = (supDif > infDif) ? atgInt = tgTable[i][0] : atgInt = tgTable[i + 1][0];
				break;
			}
		}
	}

	if (isNegative)
	{
		//Peculiaridade da função tg(x) por ser ímpar
		//9000 é 90° x 100 porque eu transformei os ângulos de float para int multiplicando por 100
		atgInt = 9000 + (9000 - atgInt);
	}

	//convertendo o valor para rad
	return atgInt / 100.0;
}