<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de (Juan Ernesto Juarez Lezama) - (110418) - (jjuarez@gmail.com)

- Para compilar:

```bash
gcc -g tp_hash.c src/hash.c src/csv.c src/split.c -o tp_hash
```

- Para ejecutar:

```bash
./tp_hash pokedex.csv
```

- Para ejecutar con valgrind:
```bash
valgrind ./tp_hash pokedex.csv
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluir **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).

## Qué es un diccionario
Así como en un diccionario tiene palabras y sus definiciones, en el ámbito de la informática, se entiende como diccionario a aquello que existe 1 única clave (palabra) y esa clave tiene asociado un valor (definición) que es el contenido que guarda. Entones se dice que un diccionario es una colección de claves y cada clave única tiene asociado valores. Podemos acceder a los valores únicamente conociendo las claves.

## Qué es una función de hash y qué características debe tener para nuestro problema en particular
Una función de hash se basa es la aplicación de diversos métodos matemáticos (sumar, restar, multiplicar, etc) para poder transformar una entrada (las claves), en alguna salida alfanumerica. Para nuestro caso en particular, solo será una salida numerica. Entonces la función de hash depende tanto del algoritmos matematico, como de la entrada.

## Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)


## Explicá 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1)
Como sabemos que un Diccionario tiene la idea de guardar información que está asociada a una clave, se pueden implementar de varias maneras:
1) Tabla Hash
Esta forma se basa en transformar las claves en información numerica, trans alguna formula matematica, y dicho numero, es la posición donde ingresaremos nuestra clave y valor.
Lo bueno: Tanto las formulas y saber en qué posición debe ir cada par `clave-valor` son instantaneas.
Lo malo: Las claves no van a estar ordenadas, el único 'orden' que seguirán, depende de la función de hashing que le demos.
3) Lista Simplemente Enlazada
A diferencia de la tabla hash, aquí debemos iterar todas las claves para verificar si ya existe o si no, y poder agregar o actualizar.
Lo bueno: Podemos hacer que sea ordenado o no (Para nuestro ejemplo diré que será ordenado).
Lo malo: Al ser nodos enlazados, para buscar una clave, vamos a tener que iterar, en el peor caso, todos los elementos, entonces esta forma lo convierte en la más lenta de nuestras 3 opciones.
2) Árbol Binario de Busqueda
Como ya sabemos que un ABB es un metodo de ordenamiento comparativo, podriamos guardar claves y valores.
Lo bueno: Al ser comparativo, tenemos la ventaja de que las claves van a estar ordenadas
Lo malo: Al ser de busqueda, no va a ser rápido como con una tabla hash, pero es más rapido que usando una lista.
En lo personal, si quiero que este ordenado y sea rápido, obtaría por esta opción.

## Explique por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)
Para un Hash Cerrado, sabiendo que la tabla de hash es donde voy a almacenar mis claves y valores, es esencial aumentar el tamaño, porque si no aumento el tamaño de mi tabla, no voy a poder seguir ingresando nuevos pares, entonces podemos concluir que el tamaño sí importa.

Para un Hash Abierto, no es tan fácil verlo a simlpe vista. Sabemos que podemos encadenar colisiones sin importa el tamaño de la tabla, ya que, si tenemos una tabla de 5 bloques, y tenemos una función de hashing muy buena, donde la probabilidad de que la mayoría de los pares estén en un solo bloque, cada bloque podría tranquilamente tener todos las claves 'casi distribuidos', el problema es que si solo dependemos de esa cantidad de bloque, la busqueda de alguna clave se hace más compleja, porque vamos a tardanos más en encontrarla. Por esa razón, y respondiendo a la pregunta de `¿Realmente importa el tamaño`, pues la verdad es que sí, porque mientras más vayamos agrandando la tabla, las claves que teniamos, van a volver a volver a distribuir con mucha menos probabilidad de que algún bloque tenga la mayoria de claves de la cantidad total de clave.

