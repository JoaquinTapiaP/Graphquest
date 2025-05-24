#include "extra.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    int square[4][4]; 
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
    List* actions; // Secuencia de movimientos para llegar al estado
} State;

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

    int es_final;
} Escenario;

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
        Escenario* esc = (Escenario *) malloc(sizeof(Escenario));
        esc->items = list_create();

        esc->id = atoi(campos[0]);
        esc->nombre = strdup(campos[1]);
        esc->descripcion = strdup(campos[2]);

        List* TempItems = split_string(campos[3], ";");

        for (char *item = list_first(TempItems); item != NULL; item = list_next(TempItems)) {

            List* values = split_string(item, ",");
            char* item_name = list_first(values);
            int item_value = atoi(list_next(values));
            int item_weight = atoi(list_next(values));
            
            Item* nuevoItem = (Item*) malloc(sizeof(Item));
            strncpy(nuevoItem->nombre, item_name, sizeof(nuevoItem->nombre));
            nuevoItem->puntos = item_value;
            nuevoItem->peso = item_weight;
            list_pushBack(esc->items, nuevoItem);
            
            list_clean(values);
            free(values);
        }

        esc->id_arriba = atoi(campos[4]);
        esc->id_abajo = atoi(campos[5]);
        esc->id_izquierda = atoi(campos[6]);
        esc->id_derecha = atoi(campos[7]);
    
        esc->es_final = atoi(campos[8]);
        list_pushBack(escenarios, esc);
    }
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
    return escenarios;
}

Escenario* buscarEscenarioPorId(List* escenarios, int id) {
    for (Escenario* esc = list_first(escenarios); esc != NULL; esc = list_next(escenarios)) {
        if (esc->id == id)
            return esc;
    }
    return NULL;
}


void vincularEscenarios(List* escenarios) {
    for (Escenario* esc = list_first(escenarios); esc != NULL; esc = list_next(escenarios)) {
        // Solo conectamos si el valor no es -1
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

void mostrarEscenarioActual(Escenario* escenarioActual, int AcID, int tiempo){
    
    
    puts("========================================");
    puts("              GraphQuest");
    puts("========================================");

    printf("Habitación actual: %s\n", escenarioActual->nombre);
    printf("Descripción de la habitación: '%s'\n", escenarioActual->descripcion);
    printf("Items en la habitacion: EN PROCESO");
    printf("\nTiempo restante: %d turnos\n", tiempo);

}

void mostrarMenuPrincipal() {
    //Se le pedira al usuario que ingrese un numero relacionado a una opcion por lo que se crea este dato
    /*int option;
    Escenario* actual = (Escenario*) malloc(sizeof(Escenario));
    actual = list_first(escenarios);*/
    puts("========================================");

    puts("1.- Recoger Ítem");
    puts("2.- Descartar Ítem");
    puts("3.- Avanzar en una Dirección");
    puts("4.- Reinicio Partida");
    puts("5.- Salir del Juego");
    
}

int main() {

    limpiarPantalla();
    system("chcp 65001 > NUL");
    List* escenarios = leer_escenarios();
    int tiempo = 10;

    vincularEscenarios(escenarios);
    
    int option; // se le pedira al usuario que ingrese un numero relacionado a una opcion por lo que se crea este dato
    do { // haz lo siguiente mientras que se cumpla la condicion de que no ingresen 5
        int IDActual = 1;
        Escenario* EscenarioActual = buscarEscenarioPorId(escenarios, IDActual);
        mostrarEscenarioActual(EscenarioActual, IDActual, tiempo);
        List* Inventario = list_create();



        mostrarMenuPrincipal(); // Se muestra el menu principal
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1: // Recoger Ítem
                limpiarPantalla();
                RecogerItem();
                printf("Recojo Item");
                break;
            case 2: //Descartar Ítem
                limpiarPantalla();
                DescartarItem();
                printf("DescartoItem");
                break;
            case 3: //Avanzar en una Dirección
                limpiarPantalla();
                AvanzarEnDireccion();
                printf("Avanzo en una direccion");
                break;
            case 4: //Reiniciar Partida
                limpiarPantalla();
                ReiniciarPartida();
                printf("Reinicio partida...");
                break;
            case 5: //Salir
                printf("Saliendo...\n");
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
