/**
 * @file rsa.cpp
 * @brief Cifrado RSA con test de Lehman-Peralta, Euclides extendido y
 *        exponenciacion rapida. Practica 9 (RSA) de CryptULL (ULL).
 */

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using u64 = unsigned long long;
using i64 = long long;
using u128 = __uint128_t;

/** Exponenciacion rapida modular: (base^exp) mod m. */
u64 PowMod(u64 base, u64 exp, u64 m) {
  u64 result = 1 % m;
  base %= m;
  while (exp > 0) {
    if (exp & 1) result = (u128)result * base % m;
    base = (u128)base * base % m;
    exp >>= 1;
  }
  return result;
}

/** Test de primalidad de Lehman-Peralta. Devuelve true si n es probable primo. */
bool LehmanPeralta(u64 n) {
  if (n < 2) return false;
  if (n == 2) return true;
  if (n % 2 == 0) return false;

  u64 t = std::min<u64>(50, n / 2);
  std::mt19937_64 gen(std::random_device{}());

  std::vector<u64> candidatos;
  if (n - 1 <= 1000000ULL) {
    candidatos.resize(n - 1);
    for (u64 i = 0; i < n - 1; i++) candidatos[i] = i + 1;
    std::shuffle(candidatos.begin(), candidatos.end(), gen);
  } else {
    candidatos.resize(t);
    std::uniform_int_distribution<u64> dist(1, n - 1);
    for (u64 i = 0; i < t; i++) candidatos[i] = dist(gen);
  }

  bool found_minus_one = false;
  u64 exp = (n - 1) / 2;
  for (u64 i = 0; i < t; i++) {
    u64 a = candidatos[i];
    u64 r = PowMod(a, exp, n);
    if (r != 1 && r != n - 1) return false;
    if (r == n - 1) found_minus_one = true;
  }
  return found_minus_one;
}

/** Algoritmo de Euclides extendido: gcd(a, b) = a*x + b*y. */
i64 EuclidesExtendido(i64 a, i64 b, i64& x, i64& y) {
  if (b == 0) { x = 1; y = 0; return a; }
  i64 x1, y1;
  i64 g = EuclidesExtendido(b, a % b, x1, y1);
  x = y1;
  y = x1 - (a / b) * y1;
  return g;
}

/** Calcula el inverso modular de d mod m (con m > 0). */
u64 InversoModular(u64 d, u64 m) {
  i64 x, y;
  i64 g = EuclidesExtendido((i64)d, (i64)m, x, y);
  if (g != 1) return 0;
  return (u64)((x % (i64)m + (i64)m) % (i64)m);
}

/** Convierte una letra A-Z a su valor 0-25. Devuelve -1 si no es letra. */
int LetraAValor(char c) {
  c = std::toupper(static_cast<unsigned char>(c));
  if (c < 'A' || c > 'Z') return -1;
  return c - 'A';
}

/** Limpia el texto: solo A-Z, mayusculas. */
std::string LimpiarTexto(const std::string& texto) {
  std::string out;
  for (char c : texto) {
    int v = LetraAValor(c);
    if (v >= 0) out.push_back('A' + v);
  }
  return out;
}

/** Calcula j tal que 26^(j-1) < n <= 26^j; el tamano de bloque es j-1. */
int TamanoBloque(u64 n) {
  u64 pot = 1;
  int j = 0;
  while (pot < n) { pot *= 26; j++; }
  return j - 1;
}

/** Codifica un bloque de letras a su entero en base 26. */
u64 BloqueAEntero(const std::string& bloque) {
  u64 r = 0;
  for (char c : bloque) r = r * 26 + (u64)(c - 'A');
  return r;
}

int main() {
  std::string texto;
  u64 p, q, d;

  std::cout << "Texto a cifrar: ";
  std::getline(std::cin, texto);
  std::cout << "p: "; std::cin >> p;
  std::cout << "q: "; std::cin >> q;
  std::cout << "d: "; std::cin >> d;

  if (!LehmanPeralta(p)) { std::cout << "Error: p no es primo.\n"; return 1; }
  if (!LehmanPeralta(q)) { std::cout << "Error: q no es primo.\n"; return 1; }
  std::cout << "Se comprueba que p y q son primos.\n";

  u64 n = p * q;
  u64 phi = (p - 1) * (q - 1);

  i64 x, y;
  i64 g = EuclidesExtendido((i64)d, (i64)phi, x, y);
  if (g != 1) { std::cout << "Error: d no es primo con phi(n)=" << phi << ".\n"; return 1; }
  std::cout << "Se comprueba que d es primo con phi(n)=" << phi << ".\n";

  u64 e = InversoModular(d, phi);
  std::cout << "Se calcula e=" << e << ".\n";

  int bloque_size = TamanoBloque(n);
  std::cout << "Como n=" << n << ", se divide el texto en bloques de "
            << bloque_size << " caracteres.\n";

  std::string limpio = LimpiarTexto(texto);
  while (limpio.size() % bloque_size != 0) limpio.push_back('X');

  std::vector<u64> bloques;
  for (size_t i = 0; i < limpio.size(); i += bloque_size) {
    bloques.push_back(BloqueAEntero(limpio.substr(i, bloque_size)));
  }

  std::cout << "Bloques en decimal: ";
  for (size_t i = 0; i < bloques.size(); i++) {
    std::cout << bloques[i] << (i + 1 < bloques.size() ? ", " : "\n");
  }

  std::cout << "Texto cifrado: ";
  for (size_t i = 0; i < bloques.size(); i++) {
    u64 c = PowMod(bloques[i], e, n);
    std::cout << c << (i + 1 < bloques.size() ? ", " : "\n");
  }

  return 0;
}
