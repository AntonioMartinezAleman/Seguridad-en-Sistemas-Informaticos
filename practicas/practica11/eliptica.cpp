/**
 * @file eliptica.cpp
 * @brief Diffie-Hellman y ElGamal sobre curva eliptica y^2 = x^3 + ax + b (mod p).
 *        Practica de criptografia eliptica de CryptULL (ULL).
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

using i64 = long long;

struct Punto {
  i64 x, y;
  bool infinito;
};

const Punto kInfinito{0, 0, true};

bool operator==(const Punto& a, const Punto& b) {
  if (a.infinito && b.infinito) return true;
  if (a.infinito != b.infinito) return false;
  return a.x == b.x && a.y == b.y;
}

i64 Mod(i64 a, i64 p) {
  i64 r = a % p;
  if (r < 0) r += p;
  return r;
}

i64 EuclidesExt(i64 a, i64 b, i64& x, i64& y) {
  if (b == 0) { x = 1; y = 0; return a; }
  i64 x1, y1;
  i64 g = EuclidesExt(b, a % b, x1, y1);
  x = y1;
  y = x1 - (a / b) * y1;
  return g;
}

i64 InversoMod(i64 a, i64 p) {
  i64 x, y;
  EuclidesExt(Mod(a, p), p, x, y);
  return Mod(x, p);
}

/** Suma de puntos P+Q sobre la curva y^2 = x^3 + ax + b mod p. */
Punto Suma(const Punto& P, const Punto& Q, i64 a, i64 p) {
  if (P.infinito) return Q;
  if (Q.infinito) return P;
  if (P.x == Q.x && Mod(P.y + Q.y, p) == 0) return kInfinito;

  i64 lambda;
  if (P == Q) {
    i64 num = Mod(3 * P.x * P.x + a, p);
    i64 den = InversoMod(2 * P.y, p);
    lambda = Mod(num * den, p);
  } else {
    i64 num = Mod(Q.y - P.y, p);
    i64 den = InversoMod(Q.x - P.x, p);
    lambda = Mod(num * den, p);
  }
  i64 x3 = Mod(lambda * lambda - P.x - Q.x, p);
  i64 y3 = Mod(lambda * (P.x - x3) - P.y, p);
  return {x3, y3, false};
}

/** Multiplicacion escalar k*P por suma repetida (doble-y-suma). */
Punto Mult(i64 k, const Punto& P, i64 a, i64 p) {
  Punto R = kInfinito;
  Punto B = P;
  while (k > 0) {
    if (k & 1) R = Suma(R, B, a, p);
    B = Suma(B, B, a, p);
    k >>= 1;
  }
  return R;
}

/** Genera todos los puntos (x,y) de E con x,y en [0, p-1]. */
std::vector<Punto> PuntosCurva(i64 a, i64 b, i64 p) {
  std::vector<bool> es_cuadrado(p, false);
  std::vector<std::vector<i64>> raices(p);
  for (i64 y = 0; y < p; y++) {
    i64 y2 = Mod(y * y, p);
    es_cuadrado[y2] = true;
    raices[y2].push_back(y);
  }
  std::vector<Punto> pts;
  for (i64 x = 0; x < p; x++) {
    i64 rhs = Mod(x * x * x + a * x + b, p);
    if (es_cuadrado[rhs]) {
      for (i64 y : raices[rhs]) pts.push_back({x, y, false});
    }
  }
  return pts;
}

/** Devuelve el menor M = 2^k tal que m < M. */
i64 SiguientePotencia2(i64 m) {
  i64 M = 1;
  while (M <= m) M <<= 1;
  return M;
}

/** Codifica m como punto buscando j en [0,h-1] tal que x = m*h + j sea coordenada
 *  x de algun punto de la curva. Devuelve el primer punto con esa x (y minima). */
std::optional<Punto> CodificarMensaje(i64 m, i64 h, i64 a, i64 b, i64 p) {
  for (i64 j = 0; j < h; j++) {
    i64 x = Mod(m * h + j, p);
    i64 rhs = Mod(x * x * x + a * x + b, p);
    for (i64 y = 0; y < p; y++) {
      if (Mod(y * y, p) == rhs) return Punto{x, y, false};
    }
  }
  return std::nullopt;
}

void ImprimePunto(const Punto& P) {
  if (P.infinito) std::cout << "O";
  else std::cout << "(" << P.x << "," << P.y << ")";
}

int main() {
  i64 p, a, b, gx, gy, dA, dB, m;
  std::cout << "p: "; std::cin >> p;
  std::cout << "a: "; std::cin >> a;
  std::cout << "b: "; std::cin >> b;
  std::cout << "Punto base G (x y): "; std::cin >> gx >> gy;
  std::cout << "dA: "; std::cin >> dA;
  std::cout << "dB: "; std::cin >> dB;
  std::cout << "Mensaje (entero): "; std::cin >> m;

  Punto G{gx, gy, false};

  auto pts = PuntosCurva(a, b, p);
  std::cout << "Puntos de la curva: ";
  for (size_t i = 0; i < pts.size(); i++) {
    ImprimePunto(pts[i]);
    std::cout << (i + 1 < pts.size() ? "," : "\n");
  }

  Punto dBG = Mult(dB, G, a, p);
  std::cout << "Clave publica de B: " << dB << "*"; ImprimePunto(G);
  std::cout << "="; ImprimePunto(dBG); std::cout << "\n";

  Punto dAG = Mult(dA, G, a, p);
  std::cout << "Clave publica de A: " << dA << "*"; ImprimePunto(G);
  std::cout << "="; ImprimePunto(dAG); std::cout << "\n";

  Punto K_A = Mult(dA, dBG, a, p);
  Punto K_B = Mult(dB, dAG, a, p);
  std::cout << "Clave secreta compartida calculada por A: " << dA << "*";
  ImprimePunto(dBG); std::cout << "="; ImprimePunto(K_A); std::cout << "\n";
  std::cout << "Clave secreta compartida calculada por B: " << dB << "*";
  ImprimePunto(dAG); std::cout << "="; ImprimePunto(K_B); std::cout << "\n";

  i64 M = SiguientePotencia2(m);
  i64 h = p / M;
  std::cout << "M=" << M << "\n";
  std::cout << "h=" << h << "<" << p << "/" << M << "\n";

  auto Qm_opt = CodificarMensaje(m, h, a, b, p);
  if (!Qm_opt) {
    std::cout << "Error: no se pudo codificar el mensaje como punto.\n";
    return 1;
  }
  Punto Qm = *Qm_opt;
  std::cout << "Mensaje original codificado como punto Qm="; ImprimePunto(Qm);
  std::cout << "\n";

  Punto C1 = Suma(Qm, K_A, a, p);
  std::cout << "Mensaje cifrado y clave publica enviados de A a B: {";
  ImprimePunto(Qm); std::cout << "+"; ImprimePunto(K_A); std::cout << ",";
  ImprimePunto(dAG); std::cout << "}={";
  ImprimePunto(C1); std::cout << ","; ImprimePunto(dAG); std::cout << "}\n";

  return 0;
}
