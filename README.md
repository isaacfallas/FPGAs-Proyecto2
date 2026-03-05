# FPGAs-Proyecto2

https://github.com/isaacfallas/FPGAs-Proyecto2

## Descripción del proyecto

Este repositorio contiene la implementación y resultados de **tres diseños** descritos en el enunciado del **Proyecto Corto 1**.  
El objetivo es implementar los diseños en HDL utilizando SystemVerilog/Verilog, sintetizarlos e implementarlos en Vivado, y reportar resultados de **utilización** y **timing**.

## Explicación de los diseños

### Diseño 1 

El diseño 1 implementa un multiplicador de 64x64 bits utilizando una arquitectura de uniciclo. Este presenta cuatro entradas de 32 bits que representan la parte lsb y msb de los buses de datos que se desean operar, además de una entrada de reloj. Para la salida, presenta un resultado descomupesto en cuatro salidas de 32 bits. Dicho diseño se debe al GPIO utilizado que posee entradas y salidas de 32 bits.

Los pasos realizados por el multiplicador son:

1. Recibir las cuatro entradas de 32 bits que representan a(lsb), a(msb), b(lsb) y b(msb), donde a y b son los factores en la multiplicación de 64 bits, así como la entrada del reloj.
2. Concatenar a(lsb), a(msb), b(lsb) y b(msb) para realizar la multiplicación a*b.
3. Ejecución de la multiplicación a*b.
4. Asignar en cada flanco positivo del reloj, las salidas del producto descompuesto en cuatro canales de 32 bits. 

El diseño lógico de la microarquitectura se presenta a continuación en forma de diagrama de bloques:

![single_cycle_block_diagram](./img/single_cycle.png)

### Diseño 2
El diseño 2 implementa el multiplicador de 64x64 bits utilizando una arquitectura segmentada de multiplicadores de 8x8 con suma de productos parciales y registros. El diseño presenta las siguientes etapas:
1. 64 Multiplicadores 8x8 que calculan los productos parciales de cada byte de los operandos de entrada: a y b.
2. Etapa de pipeline antes de desplazamiento.
3. 64 módulos de desplazamiento para extender los productos parciales a 128 bits.
4. Árbol de sumas de productos parciales segmentado:
   
   4.1. Nivel 1: 64 -> 32 sumas + Etapa de pipeline
   
   4.2. Nivel 2: 32 -> 16 sumas + Etapa de pipeline
   
   4.3. Nivel 3: 16 -> 8 sumas + Etapa de pipeline

   4.4. Nivel 4: 8 -> 4 sumas + Etapa de pipeline

   4.5. Nivel 5: 4 -> 2 sumas + Etapa de pipeline

   4.6. Nivel 6: 2 -> suma final + Etapa de pipeline

## Instrucciones de construcción

### Vitis HLS (Línea de Comandos)
1. Los comandos para Vitis HLS se incluyen en el *script* softmax.tcl:

	- Abre el proyecto
		- Determina la función *top*
		- Agrega los archivos fuente
		- Abre la solución
			- Determina el FPGA objetivo
			- Define la frecuencia de reloj
			- Configura la interfaz (axi), 
		- Ejecuta la síntesis
	- Cierra el proyecto.

2. Abrir el Vitis HLS Command Prompt, o alguna otra consola que corra Vitis HLS.
3. Correr Vitis HLS indicando el *script* softmax.tcl
	- `vitis_hls -f softmax.tcl` ó `vitis_hls softmax.tcl`
		-el modificador `-f` no es necesario en algunas versiones de Vitis.
	- La frecuencia máxima estimada entre los últimos 6 mensajes informativos en la consola.
4. Revisar los reportes en **solution/syn/report**

## Árbol de archivos con su descripción
```
.
├─ D1/
│  ├─ softmax.cpp             # Softmax Accelerator - Design 1
│  ├─ softmax.h               # Softmax Accelerator Header - Design 1
│  ├─ softmax.tcl             # TCL Script - Design 1
│  └─ softmax_tb.cc           # Softmax Accelerator Testbench - Design 1
├─ D2/
   ├─ softmax.cpp             # Softmax Accelerator - Design 2
   ├─ softmax.h               # Softmax Accelerator Header - Design 2
   ├─ softmax.tcl             # TCL Script - Design 2
   └─ softmax_tb.cc           # Softmax Accelerator Testbench - Design 2

```

## Tabla de comparación de los diseños D1 y D2

| Métrica | Diseño 1 | Diseño 2 | Cambio |
|----------|----------|----------|----------|
| Delay ruta crítica | 2.92ns | 3.35ns | +15% |
| Fmax | 342.47 MHz | 298.78 MHz | -13% |
| Latencia (ciclos) | 1288 | 1135 | -12% |
| Intervalo | 1289 | 1063 | -18% |
| LUT | 5331 | 4992 | -7% |
| FF | 4515 | 5015 | +11% |
| DSP | 9 | 12 | +33% |
| VRAM | 32 | 33 | +3% |

-------

### MP-6166 Diseño Avanzado con FPGAs
### Maestría en Ingeniería Electrónica
### Instituto Tecnológico de Costa Rica
### Profesor Ph. D. León Vega, Luis Gerardo

### Estudiantes

Aguero Villagra, Leonardo Enrique

Cruz Soto, Federico Alonso

Fallas Mejía, Jorge Isaac

Gutiérrez Quesada, Allan Mauricio
