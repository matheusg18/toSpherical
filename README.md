# toSpherical
### Código para substituir a função atan().
* Estou fazendo as comparações com int porque com float os resultados não eram satisfatórios.
* A tabela está como variável global porque no código original ela vai ser um atributo da classe SphericalParameter.
* 3 algoritmos de busca diferentes:
  - Linear search (Pior caso: 359 ciclos)
  - Binary search (Pior caso: 9 ciclos)
  - Jump search (Pior caso: 37 ciclos)
