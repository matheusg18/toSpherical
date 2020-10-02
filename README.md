# toSpherical
### Código para substituir a função atan().
* Estou fazendo as comparações com int porque com float os resultados não eram satisfatórios.
* A tabela está como variável global porque no código original ela vai ser um atributo da classe SphericalParameter.
* 3 algoritmos de busca diferentes:
  - Linear search (Pior caso: 359 ciclos)
  - Binary search (Pior caso: 9 ciclos)
  - Jump search (Pior caso: 37 ciclos)
  
## Testes de performance no arduino UNO

Nos testes eu usei um ciclo que calcula o tempo de execução de uma função, esse ciclo se repete 1000 vezes e retorna o maior valor, o menor e a média. Para isso eu usei a função micros() que retorna o tempo com resolução de 4 microssegundos (múltiplos de 4). O ciclo se repete a cada 1 segundo (delay(1000)).

### atan2

Testei a função atan2 no arduino com o parametro **atan2(15.378126f, 1.12637f)**:

![atan2_statistics](https://github.com/matheusg18/toSpherical/blob/master/atan2_statistics.png)

### linearSearch

A linearSearch foi testada com um dos o piores casos, pouco mais de 350 ciclos, **linearSearch(15.638276f)**:

![linearSearch_statistics](https://github.com/matheusg18/toSpherical/blob/master/Images/linearSearch_statistics.png)

### jumpSearch

jumpSearch também foi testada com um dos piores casos, pouco mais de 35 ciclos, **jumpSearch(15.638276f)**:

![jumpSeacrh_statistics](https://github.com/matheusg18/toSpherical/blob/master/Images/jumpSearch_statistics.png)

### binarySearch

Os resultados do binarySearch não foram bons:
- Na compilação, quando testado com o binarySearch, a memória é 81% ocupada enquanto que nos outros foi de uns 12%;
- Quando testado no pior caso (**binarySearch(0.005585f)**), apenas 9 ciclos, os resultados são piores que os outros.

![binarySearch_statistics](https://github.com/matheusg18/toSpherical/blob/master/Images/binarySearch_statistics.png)
