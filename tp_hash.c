#include "src/hash.h"
#include "src/csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TEXTO_MAX 100

typedef struct pokemon {
	char tipo;
	int fuerza;
	int destreza;
	int resistencia;
} info_pokemon;

bool agregar_nombre(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (nuevo == NULL) {
		fprintf(stderr,
			"Problema al asignar memoria para %s tras leer el archivo\n",
			str);
		return false;
	}
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool agregar_tipo(const char *str, void *ctx)
{
	*(char *)ctx = *(char *)str;
	return true;
}

bool agregar_numero(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

bool cargar_pokemones(struct archivo_csv *archivo_pokemones, hash_t *hash)
{
	bool (*funciones[])(const char *,
			    void *) = { agregar_nombre, agregar_tipo,
					agregar_numero, agregar_numero,
					agregar_numero };

	info_pokemon pokemon;
	char *nombre_pokemon = NULL;
	void *punteros[] = { &nombre_pokemon, &pokemon.tipo, &pokemon.fuerza,
			     &pokemon.destreza, &pokemon.resistencia };

	size_t lineas_leidas = 0;

	while (leer_linea_csv(archivo_pokemones, 5, funciones, punteros) == 5) {
		info_pokemon *nueva_ubicacion_pokemon =
			malloc(sizeof(info_pokemon));
		if (!nueva_ubicacion_pokemon) {
			return false;
		}
		*nueva_ubicacion_pokemon = pokemon;
		if (!hash_insertar(hash, nombre_pokemon,
				   (void *)nueva_ubicacion_pokemon, NULL)) {
			fprintf(stderr,
				"Pokemon %s no se puede agregar correctamente al diccionario\n",
				nombre_pokemon);
		}
		free(nombre_pokemon);
		lineas_leidas++;
	}

	if (lineas_leidas == 0) {
		fprintf(stderr, "Archivo vacio\n");
		return false;
	}

	return true;
}

void destruir_informacion_pokemones(void *_informacion)
{
	info_pokemon *informacion = (info_pokemon *)_informacion;
	free(informacion);
}

void mostrar_menu()
{
	printf("Elija una opcion:\n");
	printf("1) Buscar un Pokemon\n");
	printf("2) Mostrar todos los pokemones en el diccionario\n");
	printf("3) Salir\n");
	printf("\n");
	printf("Opcion: ");
}

void mostrar_informacion_del_pokemon(char *nombre_pokemon,
				     info_pokemon *pokemon_encontrado)
{
	printf("\n");
	printf(" - Datos del pokemon %s:\n", nombre_pokemon);
	printf("     Tipo: %c\n", pokemon_encontrado->tipo);
	printf("     Fuerza: %d\n", pokemon_encontrado->fuerza);
	printf("     Destreza: %d\n", pokemon_encontrado->destreza);
	printf("     Resistencia: %d\n", pokemon_encontrado->resistencia);
	printf("\n");
}

bool imprimir_pokemones(char *clave, void *valor, void *ctx)
{
	printf("%li) %s\n", *(size_t *)ctx, clave);
	(*(size_t *)ctx)++;
	return true;
}

void buscar_pokemon_y_mostrar_informacion(hash_t *diccionario_pokemon)
{
	char texto[TEXTO_MAX];
	printf("¿Qué Pokemon buscamos?: ");
	if (fgets(texto, sizeof(texto), stdin) != NULL) {
		texto[strcspn(texto, "\n")] = '\0';
		info_pokemon *pokemon_encontrado =
			(info_pokemon *)hash_buscar(diccionario_pokemon, texto);
		if (pokemon_encontrado) {
			mostrar_informacion_del_pokemon(texto,
							pokemon_encontrado);
		} else {
			printf("Pokemon no encontrado\n");
		}
	}
}

void mostrar_todos_los_pokemones(hash_t *diccionario_pokemon)
{
	size_t cantidad_pokemones = hash_cantidad(diccionario_pokemon);
	if (cantidad_pokemones == 0) {
		printf("No hay pokemones en el diccionario");
	} else {
		size_t enumerador = 1;
		printf("\n");
		hash_iterar(diccionario_pokemon, imprimir_pokemones,
			    &enumerador);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		perror("No hay archivo para ejecutar\n");
		return -1;
	}

	struct archivo_csv *archivo_pokemones = abrir_archivo_csv(argv[1], ';');
	if (!archivo_pokemones) {
		perror("No se pudo inicializar el archivo\n");
		return -2;
	}

	hash_t *diccionario_pokemon = hash_crear(10);
	if (!diccionario_pokemon) {
		cerrar_archivo_csv(archivo_pokemones);
		perror("No se pudo inicializar el hash\n");
		return -3;
	}

	if (!cargar_pokemones(archivo_pokemones, diccionario_pokemon)) {
		cerrar_archivo_csv(archivo_pokemones);
		hash_destruir_todo(diccionario_pokemon,
				   destruir_informacion_pokemones);
		return -4;
	}

	cerrar_archivo_csv(archivo_pokemones);
	bool seguir = true;

	while (seguir) {
		char texto[TEXTO_MAX];
		mostrar_menu();

		if (fgets(texto, sizeof(texto), stdin) != NULL) {
			texto[strcspn(texto, "\n")] = '\0';

			if (strcmp(texto, "1") == 0) {
				buscar_pokemon_y_mostrar_informacion(
					diccionario_pokemon);

			} else if (strcmp(texto, "2") == 0) {
				mostrar_todos_los_pokemones(
					diccionario_pokemon);

			} else if (strcmp(texto, "3") == 0) {
				seguir = false;
			}
			printf("\n");

		} else {
			printf("Error al leer opción\n");
		}
	}

	hash_destruir_todo(diccionario_pokemon, destruir_informacion_pokemones);
	return 0;
}