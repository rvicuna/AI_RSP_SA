Algoritmo: SA
Problema: Radiotherapy Scheduling Problem

Acercamiento: Soluciones infactibles se penalizan en la funcion de evaluación

Representacion: vectores2D o matrices:
				       - cada fila representa un paciente
					   - cada columna representa un bloque de tiempo (sesión)

Existen 2 matrices para la salida con la solución final -repDoc y repMaq - las cuales almacenan el ID del doctor y máquina respectivamente, para el paciente y bloque [i][j], y esto para cada sesión agendada para los pacientes.

Movimiento = randomización de lista de pacientes

Solucion_inicial = aleatoria con priorización de pacientes urgentes y paliativos

Funcion_de_evaluación: tiempoEsperaTotal + (10000 si hay falta o exceso de espera en Pacientes R, P, U según sus restricciones) //si un paciente espera menos o más de lo permitido, se castiga esa solución.


SUPUESTOS:

- Dado que los pacientes no tienen ID individual y sólo se conoce la cantidad según el tipo de prioridad, se asumirá que los pacientes estarán ordenados por ID de la siguente manera: primero urgentes, luego paliativos y por útlimo radicales. (e.g. si hay 3 urgentes serán nº1,nº2,nº3 y el 4º será el primero de los paliativos, así sucesivamente).

- Para todos los efectos de tiempos, ya sea de agendamiento o espera, se cuenta sólo de lunes a viernes (no se consideran los fines de semanas como días de espera).

- Las sesiones de cada paciente se intentan realizar en días corridos desde el inicio del tratamiento, sin embargo si hay un día sin médicos/máquinas disponibles, se continuará el tratamiento al primer día con disponibilidad.

- Se intentará que todos los pacientes urgentes y paliativos agenden sus tratamientos completos.

- El usuario/a ingresará correctamente los datos, incluyendo la instancia a probar. Al ejecutar se le preguntará el número de instancia.

- Los archivos de salida txt son reemplazados cada vez que se ejecuta el programa.


INSTRUCCIONES DE EJECUCIÓN: 

Para Compilar usar:

make clean (borra los archivos compilados)
make (compila y ejecuta)

Para Ejecutar usar:
make


OTROS:

Ambiente de trabajo: Visual Studio Code
Sistema Operativo utilizado: Linux - Ubuntu 18.04




