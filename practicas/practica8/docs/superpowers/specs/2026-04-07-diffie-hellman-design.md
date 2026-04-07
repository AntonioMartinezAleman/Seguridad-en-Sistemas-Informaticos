# Diseño: Práctica Diffie-Hellman

**Fecha:** 2026-04-07  
**Fichero objetivo:** `diffie_hellman.cpp`

## Objetivo

Implementar el algoritmo de intercambio de claves de Diffie-Hellman en C++, cumpliendo los requisitos del PDF de la práctica.

## Entradas

El programa solicita al usuario:
- `p` — número primo
- `α` (alpha) — número tal que α < p
- `x_A` — secreto de A
- `x_B` — secreto de B

## Algoritmo

1. A calcula `y_A = α^x_A mod p`
2. B calcula `y_B = α^x_B mod p`
3. A genera clave `k = y_B^x_A mod p`
4. B genera clave `k = y_A^x_B mod p` (verificación, mismo resultado)

## Funciones

### `long long modpow(long long base, long long exp, long long mod)`
Exponenciación modular mediante el algoritmo de cuadrados y multiplicaciones (binary exponentiation). Evita overflow para valores grandes.

### `void diffie_hellman(long long p, long long alpha, long long xA, long long xB)`
Ejecuta el protocolo completo e imprime la traza: `y_A`, `y_B` y `k` (desde ambos lados como verificación).

### `int main()`
Pide los valores al usuario, valida que `p > 0` y `α < p`, y llama a `diffie_hellman`.

## Salida esperada

```
--- Parte A ---
y_A = α^x_A mod p = 10
--- Parte B ---
y_B = α^x_B mod p = 3
--- Clave compartida ---
k (calculada por A) = y_B^x_A mod p = 9
k (calculada por B) = y_A^x_B mod p = 9
```

## Tipos

`long long` en todas las operaciones aritméticas para evitar overflow.

## Comentarios

Doxygen en cada función, siguiendo el estilo de las prácticas anteriores del proyecto.

## Ejemplos de verificación

| p   | α  | x_A | x_B | y_A | y_B | k  |
|-----|----|-----|-----|-----|-----|----|
| 13  | 4  | 5   | 2   | 10  | 3   | 9  |
| 43  | 23 | 25  | 33  | 40  | 16  | 4  |
| 113 | 43 | 54  | 71  | 11  | 29  | 61 |
