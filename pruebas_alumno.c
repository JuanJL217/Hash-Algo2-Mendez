#include "pa2m.h"
#include "src/hash.h"

void prueba_simple()
{
	hash_t* hash1 = hash_crear(0);
	if (!hash1) {
		printf("Error al crear el primer Hash");
		return;
	}
	pa2m_afirmar(hash1 != NULL, "Hash con capacidad menor a 3");
	hash_t* hash2 = hash_crear(3);
	if (!hash2) {
		printf("Error al crear el segundo Hash");
		hash_destruir_todo(hash1, NULL);
		return;
	}
	pa2m_afirmar(hash2 != NULL, "Hash con capacidad igual a 3");
	hash_t* hash3 = hash_crear(10);
	if (!hash3) {
		printf("Error al crear el tercer Hash");
		hash_destruir_todo(hash1, NULL);
		hash_destruir(hash2);
		return;
	}
	pa2m_afirmar(hash3 != NULL, "Hash con capacidad mayor a 3");

	hash_destruir(hash1);
	hash_destruir(hash2);
	hash_destruir(hash3);
}

void prueba_insertar_elemento()
{
	hash_t* hash = hash_crear(15);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char* clave1 = "Juan";
	size_t edad  = 23;
	if (!hash_insertar(hash, clave1, (void*)&edad, NULL)) {
		printf("No se puedo insertar el par clave-valor: Juan-23");
		hash_destruir(hash);
		return;
	}
	char* direccion_diferente = "Juan";
	char* otro_nombre = "Ernesto";
	pa2m_afirmar(hash_cantidad(hash) == 1, "Hay un par en el Hash");
	pa2m_afirmar(hash_contiene(hash, direccion_diferente) == true, "La clave guardada es Juan");
	size_t* obtener = (size_t*)hash_buscar(hash, direccion_diferente);
	size_t* obtener_otra_clave = (size_t*)hash_buscar(hash, otro_nombre);
	pa2m_afirmar(obtener == &edad, "Su valor es '%li' Direccion obtenida: '%p' -> direccion esperada: '%p'", *obtener, obtener, &edad);
	pa2m_afirmar(obtener_otra_clave == NULL, "No hay ninguna clave 'Ernesto' en el hash");
	hash_destruir_todo(hash, NULL);
}

void prueba_insertar_varios_elementos()
{
	hash_t* hash = hash_crear(20);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char* listado[6] = {"Juan", "Ernesto", "Juarez", "Lezama", "Pepito", "Leonardo"};
	size_t numeros[6] = {4, 7, 6, 6, 1, 8};
	for (size_t i = 0; i < 6; i ++) {
		if(!hash_insertar(hash, listado[i], (void*)&numeros[i], NULL)) {
        	printf("Error al insertar el par clave-valor: %s-%li", listado[i], numeros[i]);
			hash_destruir(hash);
			return;
		}
	}
	pa2m_afirmar(hash_cantidad(hash) == 6, "Hay 4 pares en el hash");

	for (size_t j = 0; j < 6; j ++) {
		pa2m_afirmar(hash_contiene(hash, listado[j]) == true, "Clave '%s' encontrada", listado[j]);
		size_t* obtener_valor = hash_buscar(hash, listado[j]);
		pa2m_afirmar(obtener_valor == &numeros[j], "Su valor es  '%li'. Dirección obtenida: '%p' -> dirección esperada: '%p'", *obtener_valor, obtener_valor, &numeros[j]);
	}
	hash_destruir_todo(hash, NULL);
}

void prueba_insertar_elementos_parte_2()
{
	hash_t* hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char* animales[10] = {"Perro", "Gato", "Elefante"};
	double peso[10] = {10.25, 3.1, 2700.5};
	
	for (size_t i = 0; i < 3; i++) {
		if(!hash_insertar(hash, animales[i], (void*)&peso[i], NULL)) {
			printf("Error al insertar el par clave-valor: %s-%f", animales[i], peso[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
	}
	pa2m_afirmar(hash_cantidad(hash) == 3, "Hay 3 pares en el hash");
	for (size_t j = 0; j < 3; j++) {
		pa2m_afirmar(hash_contiene(hash, animales[j]) == true, "Clave '%s' encontrada", animales[j]);
		double* obtener_valor = hash_buscar(hash, animales[j]);
		pa2m_afirmar(obtener_valor == &peso[j], "Su valor es  '%.2f'. Dirección obtenida: '%p' -> dirección esperada: '%p'", *obtener_valor, obtener_valor, &peso[j]);
	}

	hash_destruir_todo(hash, NULL);
}

void prueba_insertar_varios_elementos_parte_2()
{
	hash_t* hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char* animales[10] = {"Perro", "Gato", "Elefante", "Mono", "Caracol", "Leon", "Tigre", "Koala", "Paloma", "Serpiente"};
	double peso[10] = {10.25, 3.1, 2700.5, 2.80, 0.05, 150.5, 120.2, 5.0, 0.2, 0.15};
	
	for (size_t i = 0; i < 10; i++) {
		if(!hash_insertar(hash, animales[i], (void*)&peso[i], NULL)) {
			printf("Error al insertar el par clave-valor: %s-%f", animales[i], peso[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
	}
	pa2m_afirmar(hash_cantidad(hash) == 10, "Hay 10 pares en el hash");
	for (size_t j = 0; j < 10; j++) {
		pa2m_afirmar(hash_contiene(hash, animales[j]) == true, "Clave '%s' encontrada", animales[j]);
		double* obtener_valor = hash_buscar(hash, animales[j]);
		pa2m_afirmar(obtener_valor == &peso[j], "Su valor es  '%.2f'. Dirección obtenida: '%p' -> dirección esperada: '%p'", *obtener_valor, obtener_valor, &peso[j]);
	}

	hash_destruir_todo(hash, NULL);
}

int main()
{
	pa2m_nuevo_grupo("PRUEBA CREAR HASH");
	prueba_simple();

	pa2m_nuevo_grupo("PRUEBA INSERTAR EN EL HASH SIN REDIMENSION");
	prueba_insertar_elemento();
	prueba_insertar_varios_elementos();

	pa2m_nuevo_grupo("PRUEBA INSERTAR EN EL HASH CON REDIMENSION");
	prueba_insertar_elementos_parte_2();
	prueba_insertar_varios_elementos_parte_2();

	return pa2m_mostrar_reporte();
}
