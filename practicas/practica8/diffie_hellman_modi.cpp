#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

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

    // modifi
    int n;
    std::cout << "\n introduce tamaño \n";
    std::cin >> n;

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<long long> dist(1, p - 1);

    bool posible_generador = true;
    std::vector<long long> resultados;
    std::vector<int> generados;
    bool insertado = true;
    for (int i = 0; i < n; i++) {
      while (insertado == true) {
        long long val = dist(rng);
        bool repetido = false;
        for (int i = 0; i < generados.size(); i++) {
          if (generados[i] == val) repetido = true;
        }
        if (repetido == false) {
          long long res = modpow(alpha, val, p);
          resultados.push_back(res);
          std::cout << "alpha^" << val << " mod " << p << " = " << res << "\n";
          insertado = false;
        }
      }
    }

    std::sort(resultados.begin(), resultados.end());
    for (int i = 1; i < (int)resultados.size(); i++) {
        if (resultados[i] == resultados[i - 1]) {
            posible_generador = false;
            break;
        }
    }
    if (posible_generador) {
        std::cout << "alpha no genera colisiones\n";
    } else {
        std::cout << "alpha si genera colisiones\n";    
    }
    return 0;
}
