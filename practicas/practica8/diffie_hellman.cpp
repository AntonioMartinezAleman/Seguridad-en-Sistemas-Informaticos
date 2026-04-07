/**
 * @file diffie_hellman.cpp
 * @brief Implementación del algoritmo de intercambio de claves de Diffie-Hellman.
 *
 * Ambas partes (A y B) acuerdan un primo p y una base α < p.
 * Cada parte elige un secreto privado y calcula su valor público.
 * La clave compartida se obtiene aplicando la exponenciación modular
 * sobre el valor público recibido del otro participante.
 *
 * Práctica 8 — CryptULL, Universidad de La Laguna
 */

#include <iostream>

/**
 * @brief Calcula (base^exp) mod mod mediante exponenciación binaria.
 *
 * Evita overflow trabajando con long long y reduciendo en cada paso.
 *
 * @param base Base de la potencia.
 * @param exp  Exponente.
 * @param mod  Módulo.
 * @return Resultado de (base^exp) mod mod.
 */
long long modpow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

/**
 * @brief Ejecuta el protocolo Diffie-Hellman e imprime la traza completa.
 *
 * Calcula los valores públicos y_A e y_B, y la clave compartida k
 * desde el punto de vista de ambas partes (verificación).
 *
 * @param p     Número primo acordado.
 * @param alpha Generador (α < p).
 * @param xA    Secreto privado de A.
 * @param xB    Secreto privado de B.
 */
void diffie_hellman(long long p, long long alpha, long long xA, long long xB) {
    long long yA = modpow(alpha, xA, p);
    long long yB = modpow(alpha, xB, p);
    long long kA = modpow(yB, xA, p);
    long long kB = modpow(yA, xB, p);

    std::cout << "\n--- Parte A ---\n";
    std::cout << "y_A = " << alpha << "^" << xA << " mod " << p << " = " << yA << "\n";
    std::cout << "\n--- Parte B ---\n";
    std::cout << "y_B = " << alpha << "^" << xB << " mod " << p << " = " << yB << "\n";
    std::cout << "\n--- Clave compartida ---\n";
    std::cout << "k (calculada por A) = " << yB << "^" << xA << " mod " << p << " = " << kA << "\n";
    std::cout << "k (calculada por B) = " << yA << "^" << xB << " mod " << p << " = " << kB << "\n";
}

/**
 * @brief Punto de entrada del programa.
 *
 * Solicita al usuario p, α, x_A y x_B, valida la entrada
 * y ejecuta el protocolo Diffie-Hellman.
 *
 * @return 0 si la ejecución fue correcta, 1 en caso de error de entrada.
 */
int main() {
    long long p, alpha, xA, xB;

    std::cout << "Intercambio de claves Diffie-Hellman\n";
    std::cout << "=====================================\n";
    std::cout << "Numero primo p: ";
    std::cin >> p;
    std::cout << "Generador alpha (< p): ";
    std::cin >> alpha;
    std::cout << "Secreto de A (x_A): ";
    std::cin >> xA;
    std::cout << "Secreto de B (x_B): ";
    std::cin >> xB;

    if (p <= 0 || alpha <= 0 || alpha >= p) {
        std::cerr << "Error: se requiere p > 0 y 0 < alpha < p.\n";
        return 1;
    }

    diffie_hellman(p, alpha, xA, xB);
    return 0;
}
