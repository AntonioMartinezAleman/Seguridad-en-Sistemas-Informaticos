/**
 * @file firma_rsa.cpp
 * @brief Firma RSA: modo firmar y modo verificar. Practica 10 (Firma RSA) de
 *        CryptULL (ULL). Usa test Lehman-Peralta, Euclides extendido y
 *        exponenciacion rapida.
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

i64 EuclidesExt(i64 a, i64 b, i64& x, i64& y) {
  if (b == 0) { x = 1; y = 0; return a; }
  i64 x1, y1;
  i64 g = EuclidesExt(b, a % b, x1, y1);
  x = y1;
  y = x1 - (a / b) * y1;
  return g;
}

u64 InversoMod(u64 d, u64 m) {
  i64 x, y;
  i64 g = EuclidesExt((i64)d, (i64)m, x, y);
  if (g != 1) return 0;
  return (u64)((x % (i64)m + (i64)m) % (i64)m);
}

int LetraAValor(char c) {
  c = std::toupper(static_cast<unsigned char>(c));
  if (c < 'A' || c > 'Z') return -1;
  return c - 'A';
}

std::string LimpiarTexto(const std::string& texto) {
  std::string out;
  for (char c : texto) {
    int v = LetraAValor(c);
    if (v >= 0) out.push_back('A' + v);
  }
  return out;
}

int TamanoBloque(u64 n) {
  u64 pot = 1;
  int j = 0;
  while (pot < n) { pot *= 26; j++; }
  return j - 1;
}

u64 BloqueAEntero(const std::string& bloque) {
  u64 r = 0;
  for (char c : bloque) r = r * 26 + (u64)(c - 'A');
  return r;
}

/** Decodifica un entero a un bloque de letras de tamano fijo. */
std::string EnteroABloque(u64 v, int tam) {
  std::string s(tam, 'A');
  for (int i = tam - 1; i >= 0; i--) {
    s[i] = 'A' + (char)(v % 26);
    v /= 26;
  }
  return s;
}

void Firmar() {
  std::string texto;
  u64 pA, qA, eA;
  std::cout << "Texto a firmar: ";
  std::cin.ignore();
  std::getline(std::cin, texto);
  std::cout << "pA: "; std::cin >> pA;
  std::cout << "qA: "; std::cin >> qA;
  std::cout << "eA: "; std::cin >> eA;

  if (!LehmanPeralta(pA)) { std::cout << "Error: pA no es primo.\n"; return; }
  if (!LehmanPeralta(qA)) { std::cout << "Error: qA no es primo.\n"; return; }
  std::cout << "Se comprueba que pA y qA son primos.\n";

  u64 nA = pA * qA;
  u64 phi = (pA - 1) * (qA - 1);

  i64 x, y;
  i64 g = EuclidesExt((i64)eA, (i64)phi, x, y);
  if (g != 1) {
    std::cout << "Error: eA no es primo con phi(nA)=" << phi << ".\n";
    return;
  }
  std::cout << "Se comprueba que eA es primo con phi(nA)=" << phi << ".\n";

  u64 dA = InversoMod(eA, phi);
  std::cout << "Se calcula dA=" << dA << ".\n";

  int bs = TamanoBloque(nA);
  std::cout << "Como nA=" << nA << ", se divide el texto en bloques de "
            << bs << " caracteres.\n";

  std::string limpio = LimpiarTexto(texto);
  while (limpio.size() % bs != 0) limpio.push_back('X');

  std::vector<u64> bloques;
  for (size_t i = 0; i < limpio.size(); i += bs) {
    bloques.push_back(BloqueAEntero(limpio.substr(i, bs)));
  }

  std::cout << "Bloques en decimal: ";
  for (size_t i = 0; i < bloques.size(); i++) {
    std::cout << bloques[i] << (i + 1 < bloques.size() ? ", " : "\n");
  }

  std::cout << "Texto firmado: ";
  for (size_t i = 0; i < bloques.size(); i++) {
    u64 c = PowMod(bloques[i], dA, nA);
    std::cout << c << (i + 1 < bloques.size() ? ", " : "\n");
  }
}

void Verificar() {
  u64 eA, nA;
  int num_bloques;
  std::cout << "eA: "; std::cin >> eA;
  std::cout << "nA: "; std::cin >> nA;
  std::cout << "Numero de bloques firmados: "; std::cin >> num_bloques;

  std::vector<u64> firmados(num_bloques);
  std::cout << "Introduce los " << num_bloques << " bloques firmados:\n";
  for (int i = 0; i < num_bloques; i++) std::cin >> firmados[i];

  int bs = TamanoBloque(nA);
  std::cout << "Tamano de bloque: " << bs << " caracter(es).\n";

  std::cout << "Bloques descifrados (decimal): ";
  std::vector<u64> claros(num_bloques);
  for (int i = 0; i < num_bloques; i++) {
    claros[i] = PowMod(firmados[i], eA, nA);
    std::cout << claros[i] << (i + 1 < num_bloques ? ", " : "\n");
  }

  std::string texto;
  for (u64 v : claros) texto += EnteroABloque(v, bs);
  std::cout << "Texto original: " << texto << "\n";
}

int main() {
  std::cout << "Opcion (1=Firmar, 2=Verificar): ";
  int opt;
  std::cin >> opt;
  if (opt == 1) Firmar();
  else if (opt == 2) Verificar();
  else std::cout << "Opcion no valida.\n";
  return 0;
}
