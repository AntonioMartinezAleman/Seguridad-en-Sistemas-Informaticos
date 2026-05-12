"""
Practica 9: Protocolo BB84 - Implementacion con atacante (Eve)
Criptografia cuantica - Distribucion cuantica de claves
Universidad de La Laguna

Eve intercepta los qubits, los mide en una base aleatoria y los reenvia a Bob.
Esto introduce errores detectables en el QBER (~25%).
"""

import numpy as np
from qiskit import QuantumCircuit, transpile
from qiskit_aer import AerSimulator

# --- Variables iniciales ---
n = 10 ** 3
qubit_index = 0
backend = AerSimulator()
message = [QuantumCircuit(1, 2) for _ in range(n)]

# --- Preparacion de estados (Alice) ---
alice_bits = np.random.randint(2, size=n)
alice_bases = np.random.randint(2, size=n)

for i, qc in enumerate(message):
    if alice_bits[i] == 1:
        qc.x(0)
    if alice_bases[i] == 1:
        qc.h(0)

# --- Intercepcion de Eve ---
# Eve elige bases aleatorias, mide cada qubit y lo reenvia.
# Al medir, colapsa el estado cuantico, introduciendo errores
# cuando su base no coincide con la de Alice.
eve_bases = np.random.randint(2, size=n)

for i, qc in enumerate(message):
    # Eve aplica su base antes de medir
    if eve_bases[i] == 1:
        qc.h(0)
    # Eve mide en el bit clasico 1 (reservamos el 0 para Bob)
    qc.measure(qubit_index, 1)
    # Eve reenvia: prepara el qubit segun lo que midio.
    # Tras la medicion el qubit queda en la base computacional,
    # asi que si Eve uso Hadamard, lo vuelve a aplicar para reenviar
    # en la misma base que ella uso.
    if eve_bases[i] == 1:
        qc.h(0)

# --- Cambio de bases (Bob) ---
bob_bases = np.random.randint(2, size=n)

for i, qc in enumerate(message):
    if bob_bases[i] == 1:
        qc.h(0)

# --- Medida de Bob ---
classical_bit_index_for_bob_measurement = 0

for qc in message:
    qc.measure(qubit_index, classical_bit_index_for_bob_measurement)

# --- Comparacion de bases y extraccion de clave ---
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

# --- Calculo del QBER ---
errores = sum(a != b for a, b in zip(alice_key, bob_key))
qber = errores / len(alice_key) if len(alice_key) > 0 else 0

# --- Resultados ---
print("=" * 50)
print("PROTOCOLO BB84 - CON ATACANTE (EVE)")
print("=" * 50)
print(f"Numero de qubits enviados:       {n}")
print(f"Longitud de la clave compartida: {len(alice_key)}")
print(f"Errores detectados:              {errores}")
print(f"QBER:                            {qber:.4f}")
print(f"Las claves coinciden:            {alice_key == bob_key}")
print()
print(f"Primeros 20 bits de Alice: {alice_key[:20]}")
print(f"Primeros 20 bits de Bob:   {bob_key[:20]}")
print()
if qber > 0.11:
    print(f"ALERTA: QBER ({qber:.4f}) > 0.11 -> Se detecta la presencia de un atacante!")
    print("La clave debe descartarse.")
else:
    print(f"QBER ({qber:.4f}) <= 0.11 -> No se detecta atacante.")
