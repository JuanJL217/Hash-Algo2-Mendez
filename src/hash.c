#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const size_t CAPACIDAD_MINIMA = 3;
const size_t CANTIDAD_MAXIMA_POR_BLOQUE = 7;
const size_t FACTOR_CRECIMIENTO = 2;
const double FACTOR_PORCENTAJE_CAPACIDAD = 0.70;

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

unsigned int funcion_hash(const char *clave)
{
	unsigned int base = 2166136261U;
	unsigned int dato = 16777619U;
	for (size_t i = 0; clave[i] != '\0'; i++) {
		base ^= (unsigned int)clave[i];
		base *= dato;
	}
	return (unsigned int)base;
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
	hash->tabla_hash =
		calloc(hash->capacidad_tabla_hash, sizeof(nodo_par_t *));
	if (!hash->tabla_hash) {
		free(hash);
		return NULL;
	}
	return hash;
}

size_t hash_cantidad(hash_t *hash)
{
	return hash->cantidad_pares_totales;
}

nodo_par_t *crear_par(char *clave, void *valor)
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

nodo_par_t **buscar_par(nodo_par_t **nodo_actual, char *clave)
{
	if (!*nodo_actual || strcmp((*nodo_actual)->clave, clave) == 0)
		return nodo_actual;
	return buscar_par(&(*nodo_actual)->siguiente, clave);
}

nodo_par_t **obtener_puntero_a_par(nodo_par_t **tabla_hash, size_t tamaño,
				   char *clave)
{
	unsigned int hasheo = funcion_hash((const char *)clave);
	size_t posicion_en_la_tabla = (size_t)((size_t)hasheo % tamaño);
	return buscar_par(&tabla_hash[posicion_en_la_tabla], clave);
}

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

bool tope_porcentaje_capacidad(hash_t *hash)
{
	return hash->cantidad_pares_totales >=
	       (size_t)((double)hash->capacidad_tabla_hash *
			FACTOR_PORCENTAJE_CAPACIDAD);
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (!hash || !clave)
		return false;

	if (tope_porcentaje_capacidad(hash)) {
		if (!redimensionar_tabla_hash(hash)) {
			return false;
		}
	}

	nodo_par_t **par = obtener_puntero_a_par(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave);

	if (!*par) {
		nodo_par_t *nuevo_par = crear_par(clave, valor);
		if (!nuevo_par)
			return false;
		(*par) = nuevo_par;
		hash->cantidad_pares_totales++;
	} else {
		if (encontrado)
			*encontrado = (*par)->valor;
		(*par)->valor = valor;
	}

	return true;
}

// bool hash_insertar(hash_t* hash, char* clave, void* valor, void** encontrado)
// {
//     if (!hash || !clave)
//         return false;

//     if (tope_porcentaje_capacidad(hash)) {
//         if (!redimensionar_tabla_hash(hash)) {
//             return false;
//         }
//     }

//     size_t posicion;
//     nodo_par_t* nodo_anterior = NULL;
//     nodo_par_t* nodo_actual = obtener_par(hash->tabla_hash, hash->capacidad_tabla_hash, clave, nodo_anterior, &posicion);

//     if (!nodo_actual) {
//         nodo_par_t* nuevo_par = crear_par(clave, valor);
//         if (!nodo_anterior) {
//             hash->tabla_hash[posicion] = nuevo_par;
//         } else {
//             nodo_anterior->siguiente = nuevo_par;
//         }
//         hash->cantidad_pares_totales++;
//     } else {
//         if (encontrado)
//             *encontrado = nodo_actual->clave;
//         nodo_actual->clave = clave;
//     }

//     return true;
// }

void *hash_buscar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;
	nodo_par_t **par = obtener_puntero_a_par(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave);
	return (*par) ? (*par)->valor : NULL;
}

bool hash_contiene(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return false;
	return *(obtener_puntero_a_par(hash->tabla_hash,
				       hash->capacidad_tabla_hash, clave));
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;

	nodo_par_t **par = obtener_puntero_a_par(
		hash->tabla_hash, hash->capacidad_tabla_hash, clave);
	void *valor_guardado = NULL;
	if (*par) {
		valor_guardado = (*par)->valor;
		nodo_par_t *nodo_quitar = (*par);
		(*par) = (*par)->siguiente;
		free(nodo_quitar->clave);
		free(nodo_quitar);
	}
	return valor_guardado;
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (!hash || !f)
		return false;
	size_t contador = 0;
	for (size_t i = 0; i < hash->capacidad_tabla_hash; i++) {
		nodo_par_t *par = hash->tabla_hash[i];
		while (par) {
			if (!f(par->clave, par->siguiente, ctx))
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
		nodo_par_t *par = hash->tabla_hash[i];
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