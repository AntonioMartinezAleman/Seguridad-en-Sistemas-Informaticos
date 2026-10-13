# 🔐 Seguridad en Sistemas Informáticos

![C++](https://img.shields.io/badge/C++17-00599C?logo=cplusplus&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=white)
![Universidad de La Laguna](https://img.shields.io/badge/ULL-Ingeniería%20Informática-5C2D91)

Implementación desde cero de algoritmos criptográficos clásicos y modernos: cifrados de flujo y de bloque, generadores pseudoaleatorios, intercambio de claves, criptografía de clave pública y distribución cuántica de claves.

## Prácticas

| # | Tema | Algoritmo / concepto | Código |
|---|------|----------------------|--------|
| 1 | Cifrado clásico | Cifrado de **Vernam** (one-time pad) | [`practica1`](practicas/practica1) |
| 2 | Cifrado clásico | Cifrado de **Vigenère** | [`practica2`](practicas/practica2) |
| 3 | Cifrado de flujo | **ChaCha20** | [`practica3`](practicas/practica3) |
| 4 | Generadores pseudoaleatorios | Código **C/A del GPS** (registros LFSR) | [`practica4`](practicas/practica4) |
| 5 | Cifrado de flujo | Multiplicación en GF(2⁸) de **SNOW 3G** y **AES** | [`practica5`](practicas/practica5) |
| 6 | Cifrado de bloque | **AES / Rijndael** (AES-128) | [`practica6`](practicas/practica6) |
| 7 | Modos de operación | AES en modo **CBC** | [`practica7`](practicas/practica7) |
| 8 | Intercambio de claves | **Diffie-Hellman** | [`practica8`](practicas/practica8) |
| 9 | Criptografía cuántica | Protocolo **BB84**, con y sin atacante (Python) | [`practica9`](practicas/practica9) |
| 10 | Clave pública | **RSA**: generación de claves, cifrado y descifrado | [`practica10`](practicas/practica10) |
| 11 | Clave pública | **Criptografía de curva elíptica** | [`practica11`](practicas/practica11) |
| 12 | Firma digital | **Firma RSA** | [`practica12`](practicas/practica12) |
| 13 | Integración | Menú que compila y ejecuta todas las prácticas | [`practica13`](practicas/practica13) |

En muchas prácticas hay un fichero `*_modificacion.cpp`: es la variante pedida en la sesión de evaluación en directo.

## Compilación y ejecución

Requisitos: `g++` con soporte de C++17 y Python 3 (para la práctica 9).

```bash
# Una práctica concreta
g++ -std=c++17 -Wall practicas/practica6/aes.cpp -o aes && ./aes

# O todas desde el menú interactivo
cd practicas/practica13
g++ -std=c++17 menu.cpp -o menu && ./menu

# BB84
python3 practicas/practica9/bb84_con_atacante.py
```

---

Grado en Ingeniería Informática · Universidad de La Laguna · Antonio Martínez Alemán
