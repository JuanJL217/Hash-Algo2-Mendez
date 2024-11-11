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

<div align="center">
<img width="70%" src="img/hash_pokemones.svg">
</div>


```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


# IMPLEMENTACIÓN TDA HASH

- Para mi implementación decidí utilizar nodos enlazados y no el TDA Lista, debido a que estaría duplicando la cantidad de memoria que necesitaría. De nada me sirve crear nodos para guardar estructuras `clave - valor`, si mi nodo puede ser esa `clave - valor` y una referencia al siguiente nodo, a parte de tener que eliminar cada lista una vez pasemos toda la información de cada lista, entonces la mejor manera de tener un mejor rendimiento en la memoria es utilizar nodos enlazados, ya que todo mi hash dependerá de como mueva esos nodos cuando quiera redimensionar el vector.

<div align="center">
<img width="70%" src="img/hash_con_listas.svg">
</div>

<div align="center">
<img width="70%" src="img/hash_con_nodos_enlazados.svg">
</div>

Mis estructuras para este TDA son las siguientes:

```c
typedef struct nodo_par {
	char *clave;
	void *valor;
	struct nodo_par *siguiente;
} nodo_par_t;

struct hash {
	nodo_par_t **tabla_hash;
	size_t capacidad_tabla_hash;
	size_t cantidad_pares_totales;
};
```

- Para explicar un poco cómo funciona mi implementación, primero explicar como funciona mi función hash:

```c
size_t funcion_hash(const char *clave)
{
	size_t valor_hash = 2166136261U;
	size_t factor_primo = 16777619U;
	for (size_t i = 0; clave[i] != '\0'; i++) {
        valor_hash ^= (size_t)clave[i];
        valor_hash *= factor_primo;
	}
	return valor_hash;
}
```

La función de has que elgí es: `FNV-1a`  
Enlace: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
Lo que hace este algoritmo es tener 2 variables, un número llamado `valor_hash` que es un número base (valor inicial) donde vamos a contener el hasheo final, y otra variable `factor_primo` que, como su nombre indica, es un número primo, esto es así porque se demuestra que los números primos manejan mejor el factor de mezclar las cosas, mejor dicho, el hasheo tiende a ser diferente clave tras clave y eso ayuda a la dispersión de las claves, lo que significa que no habrá tantas colisiones.  
El operador `^=` es un `XOR`. Ya que cada caracter es un valor numerico en la tabla ASCII, cada valor numerico se puede manejar como binario, entonces se aplica `XOR` entre el binario de `valor_hash` y el binario de del ASCII de cada caracter de la clave y luego el valor en decimal se multiplica con el número primo. Y así hasta iterar todos los caracteres de la clave.

- Como vamos a trabajar con nodos enlazados, vamos a tener una función recursiva de busqueda para obtener la clave, ya sea para elimnar o insertar (todo es buscar).  
Para poder apuntar a algún nodo, tenemos 2 maneras, haciendo simplemente `nodo_par_t* nodo = nodo_par`, aquí estoy apuntando directamente al nodo, por eso es un puntero simple, pero, ¿quién más está apuntando a mi nodo? Claro, el nodo anterior tiene la referencia a mi nodo con `->siguiente`. entonces, yo apuntaré a la dirección de memoria de `->siguiente`. Lo que genero con eso es que tengo una manipulación más directa de los nodos que con un nodo simple y minimizando los if (así como tener menos errores).

```c
nodo_par_t **buscar_puntero_a_par(nodo_par_t **nodo_actual, char *clave)
{
	if (!*nodo_actual || strcmp((*nodo_actual)->clave, clave) == 0)
		return nodo_actual;
	return buscar_puntero_a_par(&(*nodo_actual)->siguiente, clave);
}

nodo_par_t **obtener_puntero_a_par(nodo_par_t **tabla_hash, size_t tamaño,
				   char *clave)
{
	size_t hasheo = funcion_hash((const char *)clave);
	size_t posicion_en_la_tabla = hasheo % tamaño;
	return buscar_puntero_a_par(&tabla_hash[posicion_en_la_tabla], clave);
}
```
1) Le aplico una función de hash a la clave.  
2) El hasheo lo acomodo entre el intervalo de [0, tamaño del vector], obteniendo la posición de la clave en al tabla hash.  
3) Por último llamo a la función `buscar_puntero_a_par` que de devuelve el doble puntero. Con esto puedo tener solo 2 opciones, donde `*par` puede apuntar a NULL (el final de todos los nodos) o a una dirección de memoria válida (algún nodo).

```c
	nodo_par_t **par = obtener_puntero_a_par(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave);

	void *devolver = NULL;

	if (!*par) {
		nodo_par_t *nuevo_par = crear_par(clave, valor);
		if (!nuevo_par)
			return false;
		(*par) = nuevo_par;
		hash->cantidad_pares_totales++;
	} else {
		devolver = (*par)->valor;
		(*par)->valor = valor;
	}
```
En esta parte de la funcióñ `hash_insertar`, una vez que obtenemos dicho puntero doble, podemos apreciar que tenemos solo 1 verificación, si *par apunta a NULL o no. Si apunta a NULL, significa que la clave agregada es nueva, si nos hubiese dado una dirección válida, significa que la clave ya se encuentra insertada. Aquí no verificamos si hay algún elemento en primera posición. Visualmente tenemoes así:

<div align="center">
<img width="70%" src="img/puntero_doble.svg">
</div>

- Una de las partes fundamentales es cuando debemos redimensionar. Mi función es la siguiente:

```c
bool redimensionar_tabla_hash(hash_t *hash)
{
	size_t nueva_capacidad =
		hash->capacidad_tabla_hash * FACTOR_CRECIMIENTO;
	nodo_par_t **nueva_tabla_hash =
		calloc(nueva_capacidad, sizeof(nodo_par_t *));
	if (!nueva_tabla_hash)
		return false;

	for (size_t i = 0; i < hash->capacidad_tabla_hash; i++) {
		nodo_par_t **par = &(hash->tabla_hash[i]);
		while (*par) {
			nodo_par_t **posicion_final_para_el_par =
				obtener_puntero_a_par(nueva_tabla_hash,
						      nueva_capacidad,
						      (*par)->clave);
			nodo_par_t *nodo_guardado = *par;
			(*par) = (*par)->siguiente;
			nodo_guardado->siguiente = NULL;
			*posicion_final_para_el_par = nodo_guardado;
		}
	}

	free(hash->tabla_hash);
	hash->tabla_hash = nueva_tabla_hash;
	hash->capacidad_tabla_hash = nueva_capacidad;
	return true;
}
```

Lo que hace este cóðigo es, en primer lugar, crear un nuevo vector con un tamaño duplicado al que ya tenía `FACTOR_CRECIMIENTO = 2`. Esta función es booleana, porque, en el caso que la creación del vector dé error sin verificación (que la funcion sea un `void`, en vez de un `bool`), significa que no debería ingresar nada al hash. Si yo ignoro esto, y decido insertar pares en mi tabla, aunque haya dado dicho error,, significa que no se va a redimensionar pero seguiré metiendo pares, lo que ocacionaria en un ciclo infinito de nunca redimensionar y mi tabla tendrá siempre dicha cantidad de bloques, y muchos pares, lo que se tardaría en poder buscar algún nodo.

Una vez que creamos la nueva tabla hash, vamos a pasar todos los nodos existentes de mi antigua tabla, a la nueva tabla. Visualmente tenemos esto:  

<div align="center">
<img width="70%" src="img/puntero_doble.svg">
</div>


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

