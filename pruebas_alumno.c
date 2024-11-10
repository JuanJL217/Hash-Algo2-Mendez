#include "pa2m.h"
#include "src/hash.h"
#include <stdlib.h>

void crear_hash()
{
	hash_t *hash1 = hash_crear(0);
	if (!hash1) {
		printf("Error al crear el primer Hash");
		return;
	}
	pa2m_afirmar(hash1 != NULL, "Hash con capacidad menor a 3");
	hash_t *hash2 = hash_crear(3);
	if (!hash2) {
		printf("Error al crear el segundo Hash");
		hash_destruir_todo(hash1, NULL);
		return;
	}
	pa2m_afirmar(hash2 != NULL, "Hash con capacidad igual a 3");
	hash_t *hash3 = hash_crear(10);
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

void insertar_elemento()
{
	hash_t *hash = hash_crear(15);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char *clave1 = "Juan";
	size_t edad = 23;
	if (!hash_insertar(hash, clave1, (void *)&edad, NULL)) {
		printf("No se puedo insertar el par clave-valor: Juan-23");
		hash_destruir(hash);
		return;
	}
	char *direccion_diferente = "Juan";
	char *otro_nombre = "Ernesto";
	pa2m_afirmar(hash_cantidad(hash) == 1, "Se insertó el par: '%s'-'%li'",
		     clave1, edad);
	pa2m_afirmar(hash_contiene(hash, direccion_diferente) == true,
		     "La clave '%s' existe en el hash", clave1);
	size_t *obtener = (size_t *)hash_buscar(hash, direccion_diferente);
	size_t *obtener_otra_clave = (size_t *)hash_buscar(hash, otro_nombre);
	pa2m_afirmar(
		obtener == &edad,
		"Su valor es '%li' Direccion obtenida: '%p' -> direccion esperada: '%p'",
		*obtener, obtener, &edad);
	pa2m_afirmar(obtener_otra_clave == NULL,
		     "No hay ninguna clave 'Ernesto' en el hash");
	hash_destruir_todo(hash, NULL);
}

void insertar_varios_elementos()
{
	hash_t *hash = hash_crear(20);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char *listado[6] = { "Juan",   "Ernesto", "Juarez",
			     "Lezama", "Pepito",  "Leonardo" };
	size_t numeros[6] = { 4, 7, 6, 6, 1, 8 };
	for (size_t i = 0; i < 6; i++) {
		if (!hash_insertar(hash, listado[i], (void *)&numeros[i],
				   NULL)) {
			printf("Error al insertar el par clave-valor: %s-%li",
			       listado[i], numeros[i]);
			hash_destruir(hash);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 1,
			     "Se insertó el par: '%s'-'%li'", listado[i],
			     numeros[i]);
	}

	for (size_t j = 0; j < 6; j++) {
		pa2m_afirmar(hash_contiene(hash, listado[j]) == true,
			     "La clave '%s' existe en el hash", listado[j]);
		size_t *obtener_valor = hash_buscar(hash, listado[j]);
		pa2m_afirmar(
			obtener_valor == &numeros[j],
			"Su valor es '%li'. Dirección obtenida: '%p' -> dirección esperada: '%p'",
			*obtener_valor, obtener_valor, &numeros[j]);
	}

	hash_destruir_todo(hash, NULL);
}

void insertar_elementos_parte_2()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char *animales[10] = { "Perro", "Gato", "Elefante" };
	double peso[10] = { 10.25, 3.1, 2700.5 };

	for (size_t i = 0; i < 3; i++) {
		if (!hash_insertar(hash, animales[i], (void *)&peso[i], NULL)) {
			printf("Error al insertar el par clave-valor: %s-%f",
			       animales[i], peso[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 1,
			     "Se insertó el par: '%s'-'%.2f'", animales[i],
			     peso[i]);
	}

	for (size_t j = 0; j < 3; j++) {
		pa2m_afirmar(hash_contiene(hash, animales[j]) == true,
			     "La clave '%s' existe en el hash", animales[j]);
		double *obtener_valor = hash_buscar(hash, animales[j]);
		pa2m_afirmar(
			obtener_valor == &peso[j],
			"Su valor es '%.2f'. Dirección obtenida: '%p' -> dirección esperada: '%p'",
			*obtener_valor, obtener_valor, &peso[j]);
	}

	hash_destruir_todo(hash, NULL);
}

void insertar_varios_elementos_parte_2()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char *animales[10] = { "Perro",	  "Gato",     "Elefante", "Mono",
			       "Caracol", "Leon",     "Tigre",	  "Koala",
			       "Paloma",  "Serpiente" };
	double peso[10] = { 10.25, 3.1,	  2700.5, 2.80, 0.05,
			    150.5, 120.2, 5.0,	  0.2,	0.15 };

	for (size_t i = 0; i < 10; i++) {
		if (!hash_insertar(hash, animales[i], (void *)&peso[i], NULL)) {
			printf("Error al insertar el par clave-valor: %s-%f",
			       animales[i], peso[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 1,
			     "Se insertó el par: '%s'-'%.2f'", animales[i],
			     peso[i]);
	}

	for (size_t j = 0; j < 10; j++) {
		pa2m_afirmar(hash_contiene(hash, animales[j]) == true,
			     "La clave '%s' existe en el hash", animales[j]);
		double *obtener_valor = hash_buscar(hash, animales[j]);
		pa2m_afirmar(
			obtener_valor == &peso[j],
			"Su valor es '%.2f'. Dirección obtenida: '%p' -> dirección esperada: '%p'",
			*obtener_valor, obtener_valor, &peso[j]);
	}

	hash_destruir_todo(hash, NULL);
}

void insertar_repetido()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char *nombre1 = "Juan";
	char *apellido1 = "Juarez";

	if (!hash_insertar(hash, nombre1, (void *)&apellido1, NULL)) {
		printf("Error al insertar el par clave-valor: %s-%s", nombre1,
		       apellido1);
		hash_destruir_todo(hash, NULL);
		return;
	}
	pa2m_afirmar(hash_cantidad(hash) == 1, "Se insertó el par: '%s'-'%s'",
		     nombre1, apellido1);

	char *nombre2 = "Juan";
	char *apellido2 = "Lezama";
	char **valor_anterior;
	if (!hash_insertar(hash, nombre2, (void *)&apellido2,
			   (void **)&valor_anterior)) {
		printf("Error al insertar el par clave-valor: %s-%s", nombre1,
		       apellido2);
		hash_destruir_todo(hash, NULL);
		return;
	}
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Se actualizó el valor de la clave: '%s'", nombre2);
	pa2m_afirmar(
		*valor_anterior == apellido1,
		"Antiguo valor: '%s'. Dirección obtenida: '%p' -> dirección esperada '%p'",
		*valor_anterior, &*valor_anterior, &apellido1);
	char **nuevo_valor = (char **)hash_buscar(hash, nombre1);
	pa2m_afirmar(
		*nuevo_valor == apellido2,
		"Nuevo valor: '%s'. Direccion obtenida: '%p' -> dirección esperada: '%p'",
		*nuevo_valor, &*nuevo_valor, &apellido2);
	hash_destruir_todo(hash, NULL);
}

void insertar_varios_repetidos()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");
	char *nombres[5] = { "Juan", "Leonardo", "Jahzeel", "Brenda",
			     "Celeste" };
	char *apellidos[5] = { "Juarez", "Casas", "Yaro", "Gallo", "Quichiz" };
	char *nuevos_apellidos[3] = { "Lezama", "Rodriguez", "Alarcon" };

	for (size_t i = 0; i < 5; i++) {
		if (!hash_insertar(hash, nombres[i], (void *)&apellidos[i],
				   NULL)) {
			printf("Error al insertar el par clave-valor: %s-%s",
			       nombres[i], apellidos[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 1,
			     "Se agregó el par: '%s'-'%s'", nombres[i],
			     apellidos[i]);
	}

	char **valor_anterior;
	for (size_t j = 0; j < 3; j++) {
		if (!hash_insertar(hash, nombres[j],
				   (void *)&nuevos_apellidos[j],
				   (void **)&valor_anterior)) {
			printf("Error en insertar con clave repetida: %s",
			       nombres[j]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == 5,
			     "Se actualizó el valor de la clave: '%s'",
			     nombres[j]);
		pa2m_afirmar(
			*valor_anterior == apellidos[j],
			"Antiguo valor: '%s'. Dirección obtenida: '%p' -> dirección esperada '%p'",
			*valor_anterior, &*valor_anterior, &apellidos[j]);
	}
	char **lista_apellidos;
	for (size_t k = 0; k < 5; k++) {
		if (k < 3) {
			lista_apellidos = &nuevos_apellidos[k];
		} else {
			lista_apellidos = &apellidos[k];
		}
		pa2m_afirmar(hash_contiene(hash, nombres[k]) == true,
			     "La clave '%s' existe en el hash", nombres[k]);
		char **obtener_apellido =
			(char **)hash_buscar(hash, nombres[k]);
		pa2m_afirmar(
			*obtener_apellido == *lista_apellidos,
			"Su valor es '%s'. Dirección obtenida: '%p' -> dirección esperada: '%p'",
			*obtener_apellido, &*obtener_apellido,
			&*lista_apellidos);
	}

	hash_destruir_todo(hash, NULL);
}

void eliminar_elemento()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");

	char *carrera = "Informatica";
	size_t alumnos = 20;

	if (!hash_insertar(hash, carrera, (void *)&alumnos, NULL)) {
		printf("Error al insertar el par clave-valor: %s-%li", carrera,
		       alumnos);
		hash_destruir_todo(hash, NULL);
		return;
	}
	pa2m_afirmar(hash_cantidad(hash) == 1, "Se insertó el par: '%s'-'%li'",
		     carrera, alumnos);

	char *otra_direccion_de_la_misma_clave = "Informatica";
	size_t *valor_quitado =
		(size_t *)hash_quitar(hash, otra_direccion_de_la_misma_clave);
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Se quitó la clave '%s'. Hay 0 pares en el hash",
		     otra_direccion_de_la_misma_clave);
	pa2m_afirmar(
		*valor_quitado == alumnos,
		"Su valor es '%li'. Direccion obtenida: '%p' -> direccion esperada: '%p'",
		*valor_quitado, &*valor_quitado, &alumnos);
	pa2m_afirmar(hash_cantidad(hash) == 0, "Hash vacio");
	hash_destruir_todo(hash, NULL);
}

void eliminar_varios_elemetos()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");

	char *carreras1[3] = { "Industrial", "Mecanica", "Informatica" };
	size_t alumnos1[3] = { 80, 20, 100 };

	char *carreras2[4] = { "Agrimensura", "Naval", "Electronica",
			       "Petroleo" };
	size_t alumnos2[4] = { 10, 0, 12, 2 };

	for (size_t i = 0; i < 3; i++) {
		if (!hash_insertar(hash, carreras1[i], (void *)&alumnos1[i],
				   NULL)) {
			printf("Error al insertar el par clave-valor: %s-%li",
			       carreras1[i], alumnos1[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 1,
			     "Se insertó el par: '%s'-'%li'", carreras1[i],
			     alumnos1[i]);
	}

	pa2m_afirmar(hash_cantidad(hash) == 3, "Hay 3 pares en el hash");

	for (size_t j = 2; j > 0; j--) {
		size_t *valor_quitado =
			(size_t *)hash_quitar(hash, carreras1[j]);
		pa2m_afirmar(
			hash_cantidad(hash) == j,
			"Se quitó la clave '%s'. Queda %li pares en el hash",
			carreras1[j], j);
		pa2m_afirmar(
			*valor_quitado == alumnos1[j],
			"Su valor es '%li'. Direccion obtenida: '%p' -> direccion esperada: '%p'",
			*valor_quitado, &*valor_quitado, &alumnos1[j]);
	}

	for (size_t i = 0; i < 4; i++) {
		if (!hash_insertar(hash, carreras2[i], (void *)&alumnos2[i],
				   NULL)) {
			printf("Error al insertar el par clave-valor: %s-%li",
			       carreras2[i], alumnos2[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 2,
			     "Se insertó el par: '%s'-'%li'", carreras2[i],
			     alumnos2[i]);
	}

	pa2m_afirmar(hash_cantidad(hash) == 5, "Hay 5 pares en el hash");

	for (size_t j = 0; j < 4; j++) {
		if (j == 0) {
			size_t *valor_quitado =
				(size_t *)hash_quitar(hash, carreras1[0]);
			pa2m_afirmar(
				hash_cantidad(hash) == 4,
				"Se quitó la clave '%s'. Queda %li pares en el hash",
				carreras1[0], 4);
			pa2m_afirmar(
				*valor_quitado == alumnos1[0],
				"Su valor es '%li'. Direccion obtenida: '%p' -> direccion esperada: '%p'",
				*valor_quitado, &*valor_quitado, &alumnos1[0]);
		}
		size_t *valor_quitado =
			(size_t *)hash_quitar(hash, carreras2[j]);
		pa2m_afirmar(
			hash_cantidad(hash) == 3 - j,
			"Se quitó la clave '%s'. Queda %li pares en el hash",
			carreras2[j], 3 - j);
		pa2m_afirmar(
			*valor_quitado == alumnos2[j],
			"Su valor es '%li'. Direccion obtenida: '%p' -> direccion esperada: '%p'",
			*valor_quitado, &*valor_quitado, &alumnos2[j]);
	}
	pa2m_afirmar(hash_cantidad(hash) == 0, "Hash vacio");
	hash_destruir(hash);
}

void test_de_volumen()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");

	char *claves[10000];
	size_t valores[10000];

	for (size_t i = 0; i < 5000; i++) {
		valores[i] = i;
		size_t len = (size_t)snprintf(NULL, 0, "%li", i);
		claves[i] = (char *)malloc(len + 1);
		if (!claves[i]) {
			printf("No se pudo tranformar el valor '%li' a string",
			       i);
			hash_destruir_todo(hash, NULL);
			return;
		}
		snprintf(claves[i], len + 1, "%li", i);
	}

	for (size_t i = 0; i < 3000; i++) { //Para actualizar 3000 elementos
		valores[i] = valores[i] * 7;
		hash_insertar(hash, claves[i], (void *)&valores[i], NULL);
	}

	for (size_t i = 5000; i < 10000; i++) {
		valores[i] = i;
		// Todo eso es para convertir de valor numerico a string, para que sea una clave.
		size_t len = (size_t)snprintf(NULL, 0, "%li", i);
		claves[i] = (char *)malloc(len + 1);
		if (!claves[i]) {
			printf("No se pudo tranformar el valor '%li' a string",
			       i);
			hash_destruir_todo(hash, NULL);
			return;
		}
		snprintf(claves[i], len + 1, "%li", i);
	}

	for (size_t j = 0; j < 10000; j++) {
		if (!hash_insertar(hash, claves[j], (void *)&valores[j],
				   NULL)) {
			printf("Error al insertar el par clave-valor: %s-%li",
			       claves[j], valores[j]);
			hash_destruir_todo(hash, NULL);
			return;
		}
	}

	pa2m_afirmar(hash_cantidad(hash) == 10000,
		     "Hay 10mil pares en el hash");

	for (size_t k = 0; k < 10000; k++) {
		hash_quitar(hash, claves[k]);
		free(claves[k]);
	}

	hash_destruir_todo(hash, NULL);
}

bool contar_todos_valores(char *clave, void *valor, void *contador)
{
	*(size_t *)contador += *(size_t *)valor;
	return true;
}

bool contar_valores_hasta_quiebre(char *clave, void *valor, void *contador)
{
	if (*(size_t *)valor == 10)
		return false;
	*(size_t *)contador += *(size_t *)valor;
	return true;
}

void iterar_todos_los_pares()
{
	hash_t *hash = hash_crear(1);
	if (!hash) {
		printf("Error al crear el diccionario");
		return;
	}
	pa2m_afirmar(hash != NULL, "Nuevo Hash creado");

	char *claves[9] = { "1", "2", "3", "4", "5", "10", "20", "30", "50" };
	size_t valores[9] = { 1, 2, 3, 4, 5, 10, 20, 30, 50 };

	for (size_t i = 0; i < 9; i++) {
		if (!hash_insertar(hash, claves[i], (void *)&valores[i],
				   NULL)) {
			printf("Error al insertar el par clave-valor: %s-%li",
			       claves[i], valores[i]);
			hash_destruir_todo(hash, NULL);
			return;
		}
		pa2m_afirmar(hash_cantidad(hash) == i + 1,
			     "Se insertó el par: '%s'-'%li'", claves[i],
			     valores[i]);
	}
	pa2m_afirmar(hash_cantidad(hash) == 9, "Hay 9 pares en el hash");

	size_t contador = 0;
	size_t valor_esperado_sin_quiebre = 125;

	size_t cantidad_iterados =
		hash_iterar(hash, contar_todos_valores, &contador);
	pa2m_afirmar(
		cantidad_iterados == hash_cantidad(hash) &&
			contador == valor_esperado_sin_quiebre,
		"Se iteró todos los elementos. La suma dio '%li', esperado: '%li'",
		contador, valor_esperado_sin_quiebre);

	contador = 0;
	cantidad_iterados =
		hash_iterar(hash, contar_valores_hasta_quiebre, &contador);
	pa2m_afirmar(
		hash_cantidad(hash) == 9,
		"Se iteró %li veces hasta alcanzar el valor '10'. La suma dio '%li'",
		cantidad_iterados, contador);
	hash_destruir_todo(hash, NULL);
}

void hash_null()
{
	hash_t *hash = NULL;
	char *clave = "Juan";
	size_t valor = 23;
	size_t contador = 0;
	pa2m_afirmar(hash_insertar(hash, clave, (void *)&valor, NULL) == false,
		     "Si el hash es nulo, insertar da False");
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Si el Hash es nulo, cantidad retorna 0");
	pa2m_afirmar(hash_contiene(hash, clave) == false,
		     "Si el hash es nulo, contener retorna False");
	pa2m_afirmar(hash_buscar(hash, clave) == NULL,
		     "Si el hash es nulo, buscar retorna NULL");
	pa2m_afirmar(hash_quitar(hash, clave) == NULL,
		     "Si el Hash es nulo, quitar retorna NULl");
	pa2m_afirmar(hash_iterar(hash, contar_todos_valores, &contador) == 0,
		     "Si el hash es nulo, iterar retorna 0");
	hash_destruir_todo(hash, NULL);
}

int main()
{
	pa2m_nuevo_grupo("PRUEBA CREAR HASH");
	crear_hash();

	pa2m_nuevo_grupo("PRUEBA INSERTAR SIN REDIMENSION");
	insertar_elemento();
	insertar_varios_elementos();

	pa2m_nuevo_grupo("PRUEBA INSERTAR CON REDIMENSION");
	insertar_elementos_parte_2();
	insertar_varios_elementos_parte_2();

	pa2m_nuevo_grupo("PRUEBA INSERTAR CLAVES REPETIDAS");
	insertar_repetido();
	insertar_varios_repetidos();

	pa2m_nuevo_grupo("PRUEBA ELIMINAR");
	eliminar_elemento();
	eliminar_varios_elemetos();

	pa2m_nuevo_grupo("PRUEBA DE VOLUMEN");
	test_de_volumen();

	pa2m_nuevo_grupo("PRUEBA DE ITERACION");
	iterar_todos_los_pares();

	pa2m_nuevo_grupo("PRUEBA HASH NULL");
	hash_null();

	return pa2m_mostrar_reporte();
}