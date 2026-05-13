/**
 * @file menu.cpp
 * @brief Menu principal que lanza las practicas 1-12 de la asignatura SSI.
 *        Compila y ejecuta el binario asociado a cada practica.
 */

#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

struct Entrada {
  const char* nombre;
  const char* dir;
  const char* fuente;
  const char* binario;
  bool python;
};

static const std::array<Entrada, 12> kPracticas = {{
  {"Vernam",         "../practica1",  "vernam.cpp",         "vernam",         false},
  {"Vigenere",       "../practica2",  "vigenere.cpp",       "vigenere",       false},
  {"ChaCha20",       "../practica3",  "chacha20.cpp",       "chacha20",       false},
  {"CA Satelite",    "../practica4",  "practica.cpp",       "ca_gps",         false},
  {"Snow3G",         "../practica5",  "practica.cpp",       "snow3g",         false},
  {"AES",            "../practica6",  "aes.cpp",            "aes",            false},
  {"CBC",            "../practica7",  "aesCBC.cpp",         "aesCBC",         false},
  {"Diffie-Hellman", "../practica8",  "diffie_hellman.cpp", "diffie_hellman", false},
  {"BB84",           "../practica9",  "bb84_sin_atacante.py", "",             true},
  {"RSA",            "../practica10", "rsa.cpp",            "rsa",            false},
  {"Eliptica",       "../practica11", "eliptica.cpp",       "eliptica",       false},
  {"Firma RSA",      "../practica12", "firma_rsa.cpp",      "firma_rsa",      false},
}};

void ImprimirMenu() {
  std::printf("--- MENU ---\n");
  for (size_t i = 0; i < kPracticas.size(); i++) {
    std::printf("%zu. %s\n", i + 1, kPracticas[i].nombre);
  }
  std::printf("0. Salir del menu\n");
  std::printf("Elige una opcion: ");
}

int Ejecutar(const Entrada& e) {
  std::string cmd;
  if (e.python) {
    std::printf("  1. Sin atacante\n  2. Con atacante\n  3. Modificacion\n");
    std::printf("Elige variante: ");
    std::fflush(stdout);
    int v;
    if (std::scanf("%d", &v) != 1) return 1;
    int c;
    while ((c = std::getchar()) != '\n' && c != EOF) {}
    const char* script = "bb84_sin_atacante.py";
    if (v == 2) script = "bb84_con_atacante.py";
    else if (v == 3) script = "bb84_modificacion.py";
    cmd = "cd " + std::string(e.dir) + " && python3 " + script;
  } else {
    cmd = "cd " + std::string(e.dir) +
          " && (test -x " + e.binario + " || clang++ -std=c++17 -O2 " +
          e.fuente + " -o " + e.binario + ") && ./" + e.binario;
  }
  return std::system(cmd.c_str());
}

int main() {
  std::setvbuf(stdin, nullptr, _IONBF, 0);
  while (true) {
    ImprimirMenu();
    std::fflush(stdout);
    int opt;
    if (std::scanf("%d", &opt) != 1) return 0;
    int c;
    while ((c = std::getchar()) != '\n' && c != EOF) {}
    if (opt == 0) { std::printf("Adios.\n"); return 0; }
    if (opt < 1 || opt > (int)kPracticas.size()) {
      std::printf("Opcion no valida.\n\n");
      continue;
    }
    std::printf("\n=== %s ===\n", kPracticas[opt - 1].nombre);
    std::fflush(stdout);
    Ejecutar(kPracticas[opt - 1]);
    std::printf("\n");
  }
}
