#include <iostream>
#include <math.h>
#include <chrono>

#define PI 3.141592653589793
#define TABLE_SIZE 360
#define MILLION 1000000

//array de array onde o array de 2 posi��es �: [0] = x; [1] = tg(x)
unsigned int tgTable[TABLE_SIZE][2];

float linearSearch(float tg);

int main()
{
	/*
	Como tg(x) � um fun��o eu preciso de uma tabela dupla, uma para a imagem e outra para o dom�nio (uma para x e outra para tg(x)).
	J� que a precis�o � de 0.25�, 360�/0.25� = 1440 valores. Por�m a fun��o tg � peri�dica de per�odo 180�, ent�o assim eu precisaria
	de s� metade dos 1440. Al�m de ser peri�dica a fun��o tg tamb�m � �mpar (tg(-x) = -tg(x)), sendo assim pode ser usado o intervalo
	-90� at� 90� e s� teria que usar os valores para os �ngulos positivos o que reduz pela metade denovo a quantidade de valores e assim
	teria que guardar 360 valores. Como � uma tabela dupla 360 x 2 = 720 valores.
	*/

	//passo de constru��o da tabela onde a precis�o � 0.25 mas � multiplicado por 100 para virar int, assim aumenta 25 a cada ciclo
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
			tgTable[i][1] = tan((x / 100.0) * (PI / 180.0)) * MILLION;			//valor de tg(x), (PI / 180.0) � para transformar de grau para rad e * MILLION � para transformar de float para int com precis�o de 6 casas
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
	bool isNegative = tg < 0;													//preciso dela j� que reduzi o n�mero de valores pelo fato de tg(x) ser �mpar
	unsigned int tgInt = isNegative ? tg * MILLION * (-1) : tg * MILLION;		//transformando o float tg no int tgInt para melhor precis�o na compara��o
	unsigned int atgInt = 0;													//arco tangente relativo � tgInt, em graus �

	if (tgInt > tgTable[TABLE_SIZE - 1][1])										//aqui � onde o valor fica pr�xima da ass�ntota e assume o valor m�ximo da tabela, 89.75�
	{
		atgInt = tgTable[TABLE_SIZE - 1][0];
	}
	else
	{
		for (short i = 0; i < TABLE_SIZE - 1; i++)
		{
			/*
			Aqui � o seguinte: o valor de tgInt � comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do pr�ximo ent�o
			ele est� neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a vari�vel supDif (diferen�a superior) que � a diferen�a
			entre o valor de tgInt e o do proximo ciclo da tabela, e a vari�vel infDiff (diferen�a inferior) que � a diferen�a entre o valor da tabela do
			ciclo atual e o valor de tgInt. Se infDiff < supDiff ent�o o mais preciso � usar o valor da tabela do ciclo atual, se falso ent�o usa-se o valor
			da tabela do pr�ximo ciclo. Ex.: se tgInt fosse 8, ciclo atual 5 e pr�ximo fosse 10; ent�o supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o
			valor aproximado mais correto seria o 10 por estar mais pr�ximo de 8 do que o 5.
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
		//Peculiaridade da fun��o tg(x) por ser �mpar
		//9000 � 90� x 100 porque eu transformei os �ngulos de float para int multiplicando por 100
		atgInt = 9000 + (9000 - atgInt);
	}

	//convertendo o valor para rad
	return atgInt / 100.0;
}