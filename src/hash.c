#include "hash.h"
#include <stdlib.h>
#include <string.h>

const size_t CAPACIDAD_MINIMA = 3;
const size_t CANTIDAD_MAXIMA_POR_BLOQUE = 10;
const size_t FACTOR_CRECIMIENTO = 2;
const double FACTOR_PORCENTAJE_CAPACIDAD = 0.70;

typedef struct nodo_par {
	char *clave;
	void *valor;
	struct nodo_par *siguiente;
} nodo_par_t;

typedef struct bloque {
	size_t cantidad_pares;
	nodo_par_t *nodo_inicio;
} bloque_t;

struct hash {
	bloque_t *tabla_hash;
	size_t capacidad_tabla_hash;
	size_t cantidad_pares_totales;
};

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

hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash) {
		return NULL;
	}
	if (capacidad_inicial <= CAPACIDAD_MINIMA) {
		hash->capacidad_tabla_hash = CAPACIDAD_MINIMA;
	} else {
		hash->capacidad_tabla_hash = capacidad_inicial;
	}
	hash->tabla_hash = calloc(hash->capacidad_tabla_hash, sizeof(bloque_t));
	if (!hash->tabla_hash) {
		free(hash);
		return NULL;
	}
	return hash;
}

size_t hash_cantidad(hash_t *hash)
{
	return !hash ? 0 : hash->cantidad_pares_totales;
}

nodo_par_t *crear_nodo_par(char *clave, void *valor)
{
	nodo_par_t *mi_par = calloc(1, sizeof(nodo_par_t));
	if (!mi_par)
		return NULL;
	mi_par->clave = malloc(strlen(clave) + 1);
	if (!mi_par->clave) {
		free(mi_par);
		return NULL;
	}
	strcpy(mi_par->clave, clave);
	mi_par->valor = valor;
	return mi_par;
}

nodo_par_t **buscar_puntero_a_nodo(nodo_par_t **puntero_actual, char *clave)
{
	if (!*puntero_actual || strcmp((*puntero_actual)->clave, clave) == 0)
		return puntero_actual;
	return buscar_puntero_a_nodo(&(*puntero_actual)->siguiente, clave);
}

nodo_par_t **obtener_puntero_a_nodo(bloque_t *tabla_hash, size_t tamaño,
				    char *clave, size_t *posicion)
{
	size_t hasheo = funcion_hash((const char *)clave);
	size_t posicion_en_la_tabla = hasheo % tamaño;
	if (posicion)
		*posicion = posicion_en_la_tabla;
	return buscar_puntero_a_nodo(
		&tabla_hash[posicion_en_la_tabla].nodo_inicio, clave);
}

bool redimensionar_tabla_hash(hash_t *hash,
			      nodo_par_t ***actualizar_doble_puntero,
			      char *clave, size_t *posicion)
{
	size_t nueva_capacidad =
		hash->capacidad_tabla_hash * FACTOR_CRECIMIENTO;
	bloque_t *nueva_tabla_hash = calloc(nueva_capacidad, sizeof(bloque_t));
	if (!nueva_tabla_hash)
		return false;

	for (size_t i = 0; i < hash->capacidad_tabla_hash; i++) {
		nodo_par_t **nodo = &(hash->tabla_hash[i].nodo_inicio);
		while (*nodo) {
			size_t posicion_en_la_tabla;
			nodo_par_t **lugar_encontrado = obtener_puntero_a_nodo(
				nueva_tabla_hash, nueva_capacidad,
				(*nodo)->clave, &posicion_en_la_tabla);
			*lugar_encontrado = *nodo;
			(*nodo) = (*nodo)->siguiente;
			(*lugar_encontrado)->siguiente = NULL;
			nueva_tabla_hash[posicion_en_la_tabla].cantidad_pares++;
		}
	}
	
	free(hash->tabla_hash);
	hash->tabla_hash = nueva_tabla_hash;
	hash->capacidad_tabla_hash = nueva_capacidad;
	*actualizar_doble_puntero = obtener_puntero_a_nodo(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave, posicion);
	return true;
}

bool tope_porcentaje_de_capacidad(size_t cantidad_pares, double tamaño_tabla)
{
	return cantidad_pares >=
	       (size_t)(tamaño_tabla * FACTOR_PORCENTAJE_CAPACIDAD);
}

bool tope_maximo_de_nodos_en_bloque(bloque_t *tabla, size_t posicion)
{
	return tabla[posicion].cantidad_pares >= CANTIDAD_MAXIMA_POR_BLOQUE;
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (!hash || !clave)
		return false;

	size_t posicion_en_la_tabla;
	nodo_par_t **par = obtener_puntero_a_nodo(hash->tabla_hash,
						  hash->capacidad_tabla_hash,
						  clave, &posicion_en_la_tabla);

	if (tope_porcentaje_de_capacidad(hash->cantidad_pares_totales,
					 (double)hash->capacidad_tabla_hash) ||
	    tope_maximo_de_nodos_en_bloque(hash->tabla_hash,
					   posicion_en_la_tabla)) {
		if (!redimensionar_tabla_hash(hash, &par, clave,
					      &posicion_en_la_tabla)) {
			return false;
		}
	}

	void *devolver = NULL;

	if (!*par) {
		nodo_par_t *nuevo_par = crear_nodo_par(clave, valor);
		if (!nuevo_par)
			return false;
		(*par) = nuevo_par;
		hash->tabla_hash[posicion_en_la_tabla].cantidad_pares++;
		hash->cantidad_pares_totales++;
	} else {
		devolver = (*par)->valor;
		(*par)->valor = valor;
	}

	if (encontrado)
		*encontrado = devolver;

	return true;
}

void *hash_buscar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;
	nodo_par_t **par = obtener_puntero_a_nodo(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave, NULL);
	return (*par) ? (*par)->valor : NULL;
}

bool hash_contiene(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return false;
	return *(obtener_puntero_a_nodo(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave, NULL));
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;

	size_t posicion_en_la_tabla;
	nodo_par_t **par = obtener_puntero_a_nodo(hash->tabla_hash,
						  hash->capacidad_tabla_hash,
						  clave, &posicion_en_la_tabla);
	void *valor_guardado = NULL;
	if (*par) {
		valor_guardado = (*par)->valor;
		nodo_par_t *nodo_quitar = *par;
		*par = (*par)->siguiente;
		free(nodo_quitar->clave);
		free(nodo_quitar);
		hash->tabla_hash[posicion_en_la_tabla].cantidad_pares--;
		hash->cantidad_pares_totales--;
	}
	return valor_guardado;
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (!hash || !f)
		return 0;
	size_t contador = 0;
	for (size_t i = 0; i < hash->capacidad_tabla_hash; i++) {
		nodo_par_t *par = hash->tabla_hash[i].nodo_inicio;
		while (par) {
			if (!f(par->clave, par->valor, ctx))
				return contador + 1;
			par = par->siguiente;
			contador++;
		}
	}
	return hash->cantidad_pares_totales;
}

void hash_destruir(hash_t *hash)
{
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;
	for (size_t i = 0; i < hash->capacidad_tabla_hash; i++) {
		nodo_par_t *par = hash->tabla_hash[i].nodo_inicio;
		while (par) {
			nodo_par_t *par_siguiente = par->siguiente;
			if (destructor)
				destructor(par->valor);
			free(par->clave);
			free(par);
			par = par_siguiente;
		}
	}
	free(hash->tabla_hash);
	free(hash);
}