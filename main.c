#include "extra.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct { 
    int tiempo;
    int Escenario;    // Posición
    List* inventario;    // Lista de objetos
} Jugador;

typedef struct {
    char nombre[64];
    int puntos;
    int peso;
} Item;

typedef struct Escenario {
    int id;
    char* nombre;
    char* descripcion;

    List* items;
    
    struct Escenario* arriba;
    struct Escenario* abajo;
    struct Escenario* izquierda;
    struct Escenario* derecha;

    //Variables para obtener el ID de las posiciones
    int id_arriba;
    int id_abajo;
    int id_izquierda;
    int id_derecha;

    char es_final[3];
} Escenario;

Escenario* buscarEscenarioPorId(List* escenarios, int id) {
    for (Escenario* esc = list_first(escenarios); esc != NULL; esc = list_next(escenarios)) {
        if (esc->id == id)
            return esc;
    }
    return NULL;
}

void vincularEscenarios(List* escenarios) {    
    int total = list_size(escenarios);
    for (int i = 1; i <= total; i++) {
        Escenario* esc = buscarEscenarioPorId(escenarios, i); // Usar list_get si existe
        if (esc == NULL) {
            printf("Elemento %d es NULL\n", i);
            continue;
        }

        //printf("Procesando escenario %d con ID: %d\n", i, esc->id);

        if (esc->id_arriba != -1)
            esc->arriba = buscarEscenarioPorId(escenarios, esc->id_arriba);
        if (esc->id_abajo != -1)
            esc->abajo = buscarEscenarioPorId(escenarios, esc->id_abajo);
        if (esc->id_izquierda != -1)
            esc->izquierda = buscarEscenarioPorId(escenarios, esc->id_izquierda);
        if (esc->id_derecha != -1)
            esc->derecha = buscarEscenarioPorId(escenarios, esc->id_derecha);
    }

}

List* leer_escenarios() {
    // Intenta abrir el archivo CSV que contiene datos de películas
    FILE* archivo = fopen("data/graphquest.csv", "r");
    List* escenarios = list_create();
    char **campos;
    // Leer y parsear una línea del archivo CSV. La función devuelve un array de
    // strings, donde cada elemento representa un campo de la línea CSV procesada.
    campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

    // Lee cada línea del archivo CSV hasta el final
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Escenario* esc = malloc(sizeof(Escenario));
        esc->items = list_create();

        esc->id = atoi(campos[0]);
        esc->nombre = strdup(campos[1]);
        esc->descripcion = strdup(campos[2]);

        List* TempItems = split_string(campos[3], ";");

        for (char *item = list_first(TempItems); item != NULL; item = list_next(TempItems)) {

            List* values = split_string(item, ",");
            char* item_name = list_first(values);
            int item_weight = atoi(list_next(values));
            int item_value = atoi(list_next(values));
            
            Item* nuevoItem = (Item*) malloc(sizeof(Item));
            strncpy(nuevoItem->nombre, item_name, sizeof(nuevoItem->nombre));
            nuevoItem->puntos = item_value;
            nuevoItem->peso = item_weight;
            list_pushBack(esc->items, nuevoItem);
            
            list_clean(values);
            free(values);
        }

        esc->arriba = NULL;
        esc->abajo = NULL;
        esc->izquierda = NULL;
        esc->derecha = NULL;


        esc->id_arriba = atoi(campos[4]);
        esc->id_abajo = atoi(campos[5]);
        esc->id_izquierda = atoi(campos[6]);
        esc->id_derecha = atoi(campos[7]);
        strncpy(esc->es_final, campos[8], 3);
        esc->es_final[3] = '\0';// Asegurar terminación nula

        list_pushBack(escenarios, esc);
    }
    vincularEscenarios(escenarios);
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
    return escenarios;
}

void mostrarEstadoJugador(Escenario* EscenarioActual, Jugador* perfil, int tiempo){
    puts("========================================");
    puts("              GraphQuest");
    puts("========================================");

    printf("Habitación actual: %s\n", EscenarioActual->nombre);
    printf("Descripción de la habitación: '%s'\n\n", EscenarioActual->descripcion);
    printf("Items en la habitacion: \n");
    Item* item = list_first(EscenarioActual->items);
    if (item == NULL) {
        printf("No hay items en esta habitación\n");
    } 
    else {
        // Encabezado tabla
        printf("+---------------------------+-------+-------+\n");
        printf("| %-25s | %-5s | %-5s |\n", "Nombre", "Peso", "Valor");
        printf("+---------------------------+-------+-------+\n");

        while (item != NULL) {
            printf("| %-25s | %-5d | %-5d |\n", item->nombre, item->peso, item->puntos);
            item = list_next(EscenarioActual->items);
        }

        printf("+---------------------------+-------+-------+\n");
    }
    // -------- TIEMPO DEL JUGADOR --------
    printf("\nTiempo restante: %d turnos\n", tiempo);

    



    // -------- INVENTARIO DEL JUGADOR --------
    printf("\nInventario del jugador:\n");
    Item* invItem = list_first(perfil->inventario);
    if (invItem == NULL) {
        printf(" - No hay ítems en tu inventario.\n\n");
    } else {
        int totalPeso = 0;
        int totalPuntos = 0;

        // Encabezado tabla
        printf("+---------------------------+-------+-------+\n");
        printf("| %-25s | %-5s | %-5s |\n", "Nombre", "Peso", "Valor");
        printf("+---------------------------+-------+-------+\n");

        while (invItem != NULL) {
            printf("| %-25s | %-5d | %-5d |\n", invItem->nombre, invItem->peso, invItem->puntos);
            totalPeso += invItem->peso;
            totalPuntos += invItem->puntos;
            invItem = list_next(perfil->inventario);
        }

        printf("+---------------------------+-------+-------+\n");
        printf("| %-25s | %-5d | %-5d |\n", "Total", totalPeso, totalPuntos);
        printf("+---------------------------+-------+-------+\n");
    }
    


    // -------- POSIBLES MOVIMIENTOS -------- 
    printf("Movimientos posibles desde esta habitación:\n");
    if (EscenarioActual == NULL) {
        printf("Error: EscenarioActual es NULL.\n");
        return;
    }
    if (EscenarioActual->id_arriba != -1)
        printf("↑ Arriba - %s\n", EscenarioActual->arriba->nombre);
    if (EscenarioActual->id_abajo != -1)
        printf("↓ Abajo - %s\n", EscenarioActual->abajo->nombre);
    if (EscenarioActual->id_izquierda != -1)
        printf("← Izquierda - %s\n", EscenarioActual->izquierda->nombre);
    if (EscenarioActual->id_derecha != -1)
        printf("→ Derecha - %s\n", EscenarioActual->derecha->nombre);


}

void mostrarMenuPrincipal() {
    puts("========================================");
    puts("1.- Recoger Ítem");
    puts("2.- Descartar Ítem");
    puts("3.- Avanzar en una Dirección");
    puts("4.- Reinicio Partida");
    puts("5.- Salir del Juego");
}

int calcularPesoInventario(Jugador* perfil) {
    int peso_total = 0;
    Item* item = list_first(perfil->inventario);
    while (item != NULL) {
        peso_total += item->peso;
        item = list_next(perfil->inventario);
    }
    return peso_total;
}

void RecogerItem(Jugador* perfil, Escenario* EscenarioActual) {
    if (list_first(EscenarioActual->items) == NULL) {
        printf("No hay items en esta habitación\n");
        return;
    }

    printf("Items disponibles en esta habitación:\n");
    int index = 1;
    Item* item = list_first(EscenarioActual->items);
    while (item != NULL) {
        printf("%d. %s\n", index, item->nombre);
        item = list_next(EscenarioActual->items);
        index++;
    }

    int eleccion;
    printf("Ingresa el número del ítem que deseas recoger: ");
    scanf("%d", &eleccion);

    if (eleccion < 1 || eleccion >= index) {
        printf("Selección inválida\n");
        return;
    }

    // Volver a recorrer para ubicar el ítem
    int contador = 1;
    item = list_first(EscenarioActual->items);
    while (contador < eleccion) {
        item = list_next(EscenarioActual->items);
        contador++;
    }

    // En este punto, item apunta al ítem elegido
    // Y EscenarioActual->items->current apunta al nodo correspondiente

    // Copiar el ítem
    Item* copiaItem = malloc(sizeof(Item));
    memcpy(copiaItem, item, sizeof(Item));
    list_pushBack(perfil->inventario, copiaItem);

    // Eliminar el ítem original
    list_popCurrent(EscenarioActual->items);

    limpiarPantalla();
    printf("Recogiste: %s\n", item->nombre);
}

void DescartarItem(Jugador* perfil, Escenario* EscenarioActual) {
    if (list_size(perfil->inventario) == 0) {
        printf("No tienes ningún ítem para descartar.\n");
        return;
    }

    printf("Ítems en tu inventario:\n");
    printf("%-3s | %-20s | %-5s | %-6s\n", "#", "Nombre", "Peso", "Valor");
    printf("----------------------------------------------\n");

    int index = 1;
    Item* item = list_first(perfil->inventario);
    while (item != NULL) {
        printf("%-3d | %-20s | %-5d | %-6d\n", index, item->nombre, item->peso, item->puntos);
        item = list_next(perfil->inventario);
        index++;
    }

    int opcion;
    printf("Selecciona el número del ítem que deseas descartar: ");
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > list_size(perfil->inventario)) {
        printf("Opción inválida.\n");
        return;
    }

    // Recorremos hasta el ítem elegido (índices desde 1)
    item = list_first(perfil->inventario);
    for (int i = 1; i < opcion; i++) {
        item = list_next(perfil->inventario);
    }

    // Lo eliminamos del inventario
    list_first(perfil->inventario);
    for (int i = 1; i <= opcion; i++) {
        item = list_next(perfil->inventario);
    }

    Item* descartado = list_popCurrent(perfil->inventario);

    // Devolver a la habitación
    list_pushBack(EscenarioActual->items, descartado);

    limpiarPantalla();
    printf("Has descartado '%s'.\n", descartado->nombre);
    perfil->tiempo--;
}

void AvanzarEnDireccion(Jugador* perfil, Escenario* EscenarioActual) {
    printf("Movimientos posibles desde esta habitación:\n");
    if (EscenarioActual == NULL) {
        printf("Error: EscenarioActual es NULL.\n");
        return;
    }

    if (EscenarioActual->id_arriba != -1)
        printf("↑ Arriba - %s\n", EscenarioActual->arriba->nombre);
    if (EscenarioActual->id_abajo != -1)
        printf("↓ Abajo - %s\n", EscenarioActual->abajo->nombre);
    if (EscenarioActual->id_izquierda != -1)
        printf("← Izquierda - %s\n", EscenarioActual->izquierda->nombre);
    if (EscenarioActual->id_derecha != -1)
        printf("→ Derecha - %s\n", EscenarioActual->derecha->nombre);

    printf("¿En que direccion te dirigiras?\n");
    
    char input[20];
    scanf("%19s", input);
    getchar();

    Escenario* destino = NULL;
    if (strcasecmp(input, "arriba") == 0) {
        destino = EscenarioActual->arriba;
    } 
    else if (strcasecmp(input, "abajo") == 0) {
        destino = EscenarioActual->abajo;
    } 
    else if (strcasecmp(input, "izquierda") == 0) {
        destino = EscenarioActual->izquierda;
    } 
    else if (strcasecmp(input, "derecha") == 0) {
        destino = EscenarioActual->derecha;
    }

    limpiarPantalla();
    // Validar y actualizar posición
    if (destino != NULL) {

        // Calcular tiempo a descontar
        int peso_total = calcularPesoInventario(perfil);
        int tiempo_usado = (peso_total + 10) / 10; // Equivalente a ceil((peso_total + 1)/10)
        
        perfil->Escenario = destino->id;
        perfil->tiempo -= tiempo_usado;
        printf("Te has movido a: %s\n", destino->nombre);
    } 
    else {
        printf("Opción inválida. ¡Esa es una pared sólida!\n");
    }
}

void ReiniciarPartida(Jugador* perfil, List** escenarios) {
    // Liberar inventario del jugador
    while (list_size(perfil->inventario) > 0) {
        Item* item = list_popFront(perfil->inventario);
        free(item);
    }

    // Reiniciar atributos del jugador
    perfil->tiempo = 10;
    perfil->Escenario = 1;

    // Liberar memoria de los escenarios existentes
    if (*escenarios != NULL) {
        Escenario* esc;
        while ((esc = list_popFront(*escenarios)) != NULL) {
            // Liberar items del escenario
            while (list_size(esc->items) > 0) {
                Item* item = list_popFront(esc->items);
                free(item);
            }
            free(esc->items);
            free(esc->nombre);
            free(esc->descripcion);
            free(esc);
        }
        free(*escenarios);
    }

    // Cargar nuevos escenarios desde el archivo
    *escenarios = leer_escenarios();
}

int verificarEstadoJuego(Jugador* perfil, List** escenarios, Escenario** EscenarioActual) {
    // Verificar si ya estamos en estado final

    printf("\nDEBUG: Escenario %d - Final: %s\n", (*EscenarioActual)->id, (*EscenarioActual)->es_final);

    if (strcasecmp((*EscenarioActual)->es_final, "si") == 0) {
        limpiarPantalla();
        printf("---------------------------------------\n");
        printf("-          ¡HABITACIÓN FINAL!         -\n");
        printf("---------------------------------------\n");
        
        // Calcular y mostrar puntuación
        int total_puntos = 0;
        printf("Inventario final:\n");
        printf("---------------------------+--------\n");
        printf("%-25s | %-6s\n", "Item", "Puntos");
        printf("---------------------------+--------\n");
        
        Item* item = list_first(perfil->inventario);
        while (item) {
            printf("%-25s | %-6d\n", item->nombre, item->puntos);
            total_puntos += item->puntos;
            item = list_next(perfil->inventario);
        }
        printf("\nPuntaje total: %d\n\n", total_puntos);

        // Opciones post-juego
        printf("1. Reiniciar partida\n");
        printf("2. Salir del juego\n");
        printf("Selección: ");
        
        int opcion;
        scanf("%d", &opcion);
        limpiarPantalla();
        
        if (opcion == 1) {
            ReiniciarPartida(perfil, escenarios);
            return 1;
        }
        return 2;
    }

    // Verificar tiempo agotado
    if (perfil->tiempo <= 0) {
        limpiarPantalla();
        printf("¡Se ha agotado el tiempo!\n\n");
        printf("---------------------------------------\n");
        printf("-              GAME OVER              -\n");
        printf("---------------------------------------\n\n");
        printf("1. Reiniciar partida\n");
        printf("2. Salir del juego\n");
        printf("Selección: ");
        
        int opcion;
        scanf("%d", &opcion);
        limpiarPantalla();
        
        if (opcion == 1) {
            ReiniciarPartida(perfil, escenarios);
            return 1;
        }
        return 2;
    }

    return 0;
}

int main() {
    limpiarPantalla();
    system("chcp 65001 > NUL");
    List* escenarios = leer_escenarios();
    Jugador* perfil = (Jugador*)malloc(sizeof(Jugador));
    perfil->inventario = list_create();
    perfil->tiempo = 10;
    perfil->Escenario = 1;

    int option; // se le pedira al usuario que ingrese un numero relacionado a una opcion por lo que se crea este dato
    do { // haz lo siguiente mientras que se cumpla la condicion de que no ingresen 5
        
        Escenario* EscenarioActual = buscarEscenarioPorId(escenarios, perfil->Escenario);
        if (EscenarioActual == NULL) {
            printf("Error: Escenario no encontrado!\n");
            continue; // o manejar el error adecuadamente
        }

        int estado = verificarEstadoJuego(perfil, &escenarios, &EscenarioActual);
        if (estado == 2) 
            option = 5; // Salir
        if (estado == 1) continue;


        mostrarEstadoJugador(EscenarioActual, perfil, perfil->tiempo);
        mostrarMenuPrincipal(); // Se muestra el menu principal
        if (estado != 2) {
            scanf("%d", &option);
            getchar();
        }

        switch (option) {
            case 1: // Recoger Ítem
                limpiarPantalla();
                RecogerItem(perfil, EscenarioActual);

                //printf("Recojo Item");
                break;
            case 2: //Descartar Ítem
                limpiarPantalla();
                DescartarItem(perfil, EscenarioActual);
                //printf("DescartoItem");
                break;
            case 3: //Avanzar en una Dirección
                limpiarPantalla();
                AvanzarEnDireccion(perfil, EscenarioActual);

                //printf("Avanzo en una direccion");
                break;
            case 4: //Reiniciar Partida
                limpiarPantalla();
                ReiniciarPartida(perfil, &escenarios);
                printf("Partida reiniciada!\n");
                break;
            case 5: //Salir
                limpiarPantalla();
                printf("Saliendo del juego...\n");
                break;
            default:
                limpiarPantalla();
                printf("Opción Invalida, intentelo otra vez\n");
        }

        if (option != 5) {
            printf("\n");
        }

    } while (option != 5);
    
    return 0;
}
