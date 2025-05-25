## GraphQuest


## Descripción
Este programa permite a los usuarios jugar un juego de exploracion e items.
El mapa e items se cargan al momento de iniciar el juego.
Esta herramiente esta hecha con el proposito de entretener al jugador, e intentar obtener la mayor cantidad de puntos posibles antes de que se acabe el tiempo.


## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
3. **Compila el código**
    - Abre el archivo principal `main.c`.
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal Git Bash, compila el programa con el siguiente comando: 
    ```
    gcc  main.c list.c extra.c -Wno-unused-result -o Graphquest
    ```
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
    ```
    ./Graphquest
    ```

### Funcionalidades

### Funcionando correctamente:

- Cargar la base de datos del desde un archivo .csv
- Recoger items de la habitacion.
- Descartar objetos del inventario.
- Moverte en direcciones válida.
- Reiniciar la partida desde 0.

### Problemas conocidos:

- No se puede editar el tiempo.

### A mejorar:


- Permitir que el tiempo sea customizable dependiendo de las preferencias del jugador
- Dar la opción de retroceder al momento de no querer escoger un objeto, se puede escoger un numero no valido salir del menu, pero no hay una opción directa.

## Ejemplo de uso

**Paso 1: Recoger objeto**

El usuario abre el programa por primera vez.
El usuario es colocado en la habitacion 1 de entrada.

```
Opción seleccionada: 1) Recoger objeto
Ingresa el número del item que deseas recoger: 1

```

Si en la habitación actual hay objetos, te da la opción de escoger el que tu quieras, incluso sy hay solo uno.
Si en la habitación actual no hay objetos, te informa y te devuelve al menu principal.

**Paso 2: Descartar objeto**

El usuario desea eliminar un objeto de su inventario

```
Opción seleccionada: 2) Descartar objeto
Ingresa el número del item que deseas descartar: 3
```

Si en el inventario hay objetos, entonces el programa entrega una lista enumerada de los objetos a descartar. Al descartar se pierde 1 de tiempo.
Si en el inventario no hay objetos, el programa avisa que no hay y te devuelve al menu. No se pierde tiempo.

**Paso 3: Avanzar en una dirección**

El usuario avanzar en una de las direcciones disponiblesw

```
Opción seleccionada: 3) Avanzar en una dirección
¿En que direccion te dirigiras? derecha
```

El programa le muestra al jugador las posibles direcciones. 
Si la direccion es válida, el jugador se movera a esa habitación. Se quita tiempo en base al la ecuacion: [(Peso Total Inventario + 1) / 10]
Si la direccion no es válida, el jugador no se movera y el programa dira que choco con una pared solida. No se pierde tiempo.

**Paso 4: Reiniciar**

```
Opción seleccionada: 4) Reiniciar
```

El programa se reseta, se devuelven los items a su lugar, el jugador vuelve a la primera habitacion y el inventario se vacia.

**Paso 5: Salir**
El usuario desea salir del programa.
```
Opción Seleccionada: 5) Salir
```
El programa se termina y elimina la informacion.
