#include <iostream>
#include <math.h>
#include <chrono>

#define PI 3.141592653589793
#define TABLE_SIZE 360
#define MILLION 1000000

//array de array onde o array de 2 posições é: [0] = x; [1] = tg(x)
unsigned int tgTable[TABLE_SIZE][2];

void createTgTable();
float linearSearch(float tg);
float binarySearch(float tg);
float jumpSearch(float tg);
void testeDeFuncionalidade();
void testeDeTempo(float value, unsigned int ciclos);

int main()
{
	createTgTable();
	//testeDeFuncionalidade();
	testeDeTempo(15.278168f, 5000);
}

//função para criar a tabela
void createTgTable()
{
	/*
	Como tg(x) é um função eu preciso de uma tabela dupla, uma para a imagem e outra para o domínio (uma para x e outra para tg(x)).
	Já que a precisão é de 0.25°, 360°/0.25° = 1440 valores. Porém a função tg é periódica de período 180°, então assim eu precisaria
	de só metade dos 1440. Além de ser periódica a função tg também é ímpar (tg(-x) = -tg(x)), sendo assim pode ser usado o intervalo
	-90° até 90° e só teria que usar os valores para os ângulos positivos o que reduz pela metade denovo a quantidade de valores e assim
	teria que guardar 360 valores. Como é uma tabela dupla 360 x 2 = 720 valores.
	*/

	//passo de construção da tabela onde a precisão é 0.25 mas é multiplicado por 100 para virar int, assim aumenta 25 a cada ciclo
	unsigned int x = 25;

	//tg(0) = 0 
	tgTable[0][0] = 0;														//valor de x
	tgTable[0][1] = 0;														//valor de tg(x)

	for (short i = 1; i < TABLE_SIZE; i++)
	{
		tgTable[i][0] = x;													//valor de x
		tgTable[i][1] = tan((x / 100.0) * (PI / 180.0)) * MILLION;			//valor de tg(x), (PI / 180.0) é para transformar de grau para rad e * MILLION é para transformar de float para int com precisão de 6 casas
		x = x + 25;															//passo
	}
}

//tempo no pior caso: 100ns
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

	//convertendo o valor para grau° (na versão final do código a conversão será para rad)
	return atgInt / 100.0;
}

//tempo no pior caso: 100ns
float binarySearch(float tg)
{
	bool isNegative = tg < 0;													//preciso dela já que reduzi o número de valores pelo fato de tg(x) ser ímpar
	unsigned int tgInt = isNegative ? tg * MILLION * (-1) : tg * MILLION;		//transformando o float tg no int tgInt para melhor precisão na comparação
	unsigned int atgInt = 0;													//arco tangente relativo à tgInt, em graus °

	//variáveis da busca binária
	short start = 0;															//limite da direita
	short end = TABLE_SIZE - 2;													//limite da esquerda
	short mid;																	//média entre os limites, valor do meio

	if (tgInt > tgTable[TABLE_SIZE - 1][1])										//aqui é onde o valor fica próxima da assíntota e assume o valor máximo da tabela, 89.75°
	{
		atgInt = tgTable[TABLE_SIZE - 1][0];
	}
	else
	{
		while (start <= end)
		{
			mid = (start + end) / 2;											//meio da tabela

			/*
			Aqui é o seguinte: o valor de tgInt é comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do próximo então
			ele está neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a variável supDif (diferença superior) que é a diferença
			entre o valor de tgInt e o do proximo ciclo da tabela, e a variável infDiff (diferença inferior) que é a diferença entre o valor da tabela do
			ciclo atual e o valor de tgInt. Se infDiff < supDiff então o mais preciso é usar o valor da tabela do ciclo atual, se falso então usa-se o valor
			da tabela do próximo ciclo. Ex.: se tgInt fosse 8, ciclo atual 5 e próximo fosse 10; então supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o
			valor aproximado mais correto seria o 10 por estar mais próximo de 8 do que o 5.
			*/

			if (tgInt >= tgTable[mid][1] && tgInt <= tgTable[mid + 1][1])
			{
				unsigned int supDif = tgTable[mid + 1][1] - tgInt;
				unsigned int infDif = tgInt - tgTable[mid][1];

				atgInt = (supDif > infDif) ? atgInt = tgTable[mid][0] : atgInt = tgTable[mid + 1][0];
				break;
			}
			else if (tgTable[mid][1] > tgInt)
			{
				end = mid - 1;													//novo valor do limite da direita, velores menores que o do meio
			}
			else
			{
				start = mid + 1;												//novo valor do limite da esquerda, valores maiores que o do meio
			}
		}
	}

	if (isNegative)
	{
		//Peculiaridade da função tg(x) por ser ímpar
		//9000 é 90° x 100 porque eu transformei os ângulos de float para int multiplicando por 100
		atgInt = 9000 + (9000 - atgInt);
	}

	//convertendo o valor para grau ° (na versão final do código a conversão será para rad)
	return atgInt / 100.0;
}

//tempo no pior caso: 100ns
float jumpSearch(float tg)
{
	bool isNegative = tg < 0;													//preciso dela já que reduzi o número de valores pelo fato de tg(x) ser ímpar
	unsigned int tgInt = isNegative ? tg * MILLION * (-1) : tg * MILLION;		//transformando o float tg no int tgInt para melhor precisão na comparação
	unsigned int atgInt = 45000;													//arco tangente relativo à tgInt, em graus °

	//variáveis da jump search
	unsigned short m = 18;														//incremento no índice i do loop, m = sqrt(TABLE_SIZE)
	unsigned short i = 0;														//índice que será usado para linear search no subintervalo onde está o valor de atgInt

	if (tgInt > tgTable[TABLE_SIZE - 1][1])										//aqui é onde o valor fica próxima da assíntota e assume o valor máximo da tabela, 89.75°
	{
		//std::cout << "Maior que 89.75!!" << std::endl;
		atgInt = tgTable[TABLE_SIZE - 1][0];
	}
	else
	{
		//vou dividindo a tabela em subintervalos de m em m (m = 18) e se o valor procurado estiver neste subintervalo é realizado uma linear search nele 
		while (tgTable[m][1] <= tgInt && m < TABLE_SIZE)
		{
			i = m;
			m = m + 18;

			//std::cout << "tgTable[" << m << "][1] = " << tgTable[m][1] << std::endl;
			//std::cout << "tgInt: " << tgInt << std::endl;
		}
		//subintervalo para linear search
		for (; i < m; i++)
		{
			/*
			Aqui é o seguinte: o valor de tgInt é comparado com os valores da tabela, se ele for maior que o do ciclo atual e menor que o do próximo então
			ele está neste intervalo. Se verdadeiro: Para achar o valor aproximado mais preciso tem a variável supDif (diferença superior) que é a diferença
			entre o valor de tgInt e o do proximo ciclo da tabela, e a variável infDiff (diferença inferior) que é a diferença entre o valor da tabela do
			ciclo atual e o valor de tgInt. Se infDiff < supDiff então o mais preciso é usar o valor da tabela do ciclo atual, se falso então usa-se o valor
			da tabela do próximo ciclo. Ex.: se tgInt fosse 8, ciclo atual 5 e próximo fosse 10; então supDiff = 10-8 = 2 e infDiff = 8-5 = 3, sendo assim o
			valor aproximado mais correto seria o 10 por estar mais próximo de 8 do que o 5.
			*/

			//std::cout << "============== " << i << " ==============" << std::endl;
			//std::cout << "Ciclo atual: tg(x) = " << tgTable[i][1] << "\tx = " << tgTable[i][0] << std::endl;
			//std::cout << "Proximo ciclo: tg(x) = " << tgTable[i + 1][1] << "\tx = " << tgTable[i + 1][0] << std::endl;

			if (tgInt >= tgTable[i][1] && tgInt <= tgTable[i + 1][1])
			{
				unsigned int supDif = tgTable[i + 1][1] - tgInt;
				unsigned int infDif = tgInt - tgTable[i][1];

				//std::cout << "supDif: " << supDif << std::endl;
				//std::cout << "infDif: " << infDif << std::endl;

				atgInt = (supDif > infDif) ? atgInt = tgTable[i][0] : atgInt = tgTable[i + 1][0];

				//std::cout << "atgInt: " << atgInt << std::endl;
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

	//convertendo o valor para grau ° (na versão final do código a conversão será para rad)
	return atgInt / 100.0;
}

void testeDeFuncionalidade()
{
	for (short i = 0; i < TABLE_SIZE; i++)
	{
		int j = i * 25;
		float test = jumpSearch(tan((j / 100.0) * (PI / 180.0)) + 0.000456f);			//teste da função
		//std::cout << "x = " << test << "\tj = " << j / 100.0;

		if (test * 100 == j)
		{
			//std::cout << "\t=====================" << std::endl;
		}
		else
		{
			//std::cout << std::endl;
			std::cout << "===== DIFERENTE =====" << std::endl << "x = " << test << "\tj = " << j / 100.0 << std::endl;
		}
	}
}

void testeDeTempo(float value, unsigned int ciclos)
{
	long long maior = 0;
	long long menor = 100000;
	long long media = 0;
	for (short i = 0; i < ciclos; i++)
	{
		//std::cin >> value;

		auto inicio = std::chrono::high_resolution_clock::now();
		float test = jumpSearch(value);
		auto resultado = std::chrono::high_resolution_clock::now() - inicio;

		long long nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(resultado).count();

		maior = (nanoseconds > maior) ? nanoseconds : maior;
		menor = (nanoseconds < menor) ? nanoseconds : menor;
		media += nanoseconds;

		//std::cout << "x = " << test << std::endl << "nanoseconds: " << nanoseconds << std::endl;
	}
	std::cout << "Maior = " << maior << std::endl;
	std::cout << "Menor = " << menor << std::endl;
	std::cout << "Média = " << media / (float)ciclos << std::endl;
}