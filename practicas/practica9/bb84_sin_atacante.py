"""
Practica 9: Protocolo BB84 - Implementacion sin atacante
Criptografia cuantica - Distribucion cuantica de claves
Universidad de La Laguna
"""

import numpy as np
from qiskit import QuantumCircuit, transpile
from qiskit_aer import AerSimulator

# --- 1.1 Librerias y variables iniciales ---
n = 10 ** 3
qubit_index = 0
backend = AerSimulator()
message = [QuantumCircuit(1, 2) for _ in range(n)]

# --- 1.1 Preparacion de estados (Alice) ---
# Alice genera dos secuencias aleatorias: una de bits y otra de bases.
# Cada bit se codifica en un cubit dependiendo de la base seleccionada
# (computacional o Hadamard).
alice_bits = np.random.randint(2, size=n)
alice_bases = np.random.randint(2, size=n)

for i, qc in enumerate(message):
    if alice_bits[i] == 1:
        qc.x(0)
    if alice_bases[i] == 1:
        qc.h(0)

# --- 1.2 Cambio de bases (Bob) ---
# Bob selecciona de forma aleatoria una base de medida para cada cubit recibido.
bob_bases = np.random.randint(2, size=n)

for i, qc in enumerate(message):
    if bob_bases[i] == 1:
        qc.h(0)

# --- 1.3 Medida ---
# Bob realiza la medicion en dicha base.
classical_bit_index_for_bob_measurement = 0

for qc in message:
    qc.measure(qubit_index, classical_bit_index_for_bob_measurement)

# --- 1.4 Comparacion de bases ---
# Alice y Bob comparan publicamente las bases utilizadas y conservan
# unicamente aquellos bits en los que ambas coinciden.
cbit = classical_bit_index_for_bob_measurement
bob_measured_bits = []
for qc in message:
    transpiled_qc = transpile(qc, backend)
    result = backend.run(qc, shots=1).result()
    counts = result.get_counts()
    measured_bit = list(counts.keys())[0][::-1][cbit]
    bob_measured_bits.append(measured_bit)

alice_key = []
bob_key = []
for i in range(n):
    if alice_bases[i] == bob_bases[i]:
        alice_key.append(int(alice_bits[i]))
        bob_key.append(int(bob_measured_bits[i]))

# --- 1.5 Calculo del QBER ---
# Tasa de error cuantico (QBER): proporcion de bits que difieren
# entre Alice y Bob en las posiciones donde las bases coinciden.
errores = sum(a != b for a, b in zip(alice_key, bob_key))
qber = errores / len(alice_key) if len(alice_key) > 0 else 0

# --- Resultados ---
print("=" * 50)
print("PROTOCOLO BB84 - SIN ATACANTE")
print("=" * 50)
print(f"Numero de qubits enviados:       {n}")
print(f"Longitud de la clave compartida: {len(alice_key)}")
print(f"Errores detectados:              {errores}")
print(f"QBER:                            {qber:.4f}")
print(f"Las claves coinciden:            {alice_key == bob_key}")
print()
print(f"Primeros 20 bits de Alice: {alice_key[:20]}")
print(f"Primeros 20 bits de Bob:   {bob_key[:20]}")
