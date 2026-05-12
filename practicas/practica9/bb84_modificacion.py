import numpy as np
from qiskit import QuantumCircuit, transpile
from qiskit_aer import AerSimulator


def bb84(eve, n=10 ** 3):
    qubit_index = 0
    backend = AerSimulator()
    message = [QuantumCircuit(1, 2) for _ in range(n)]

    alice_bits = np.random.randint(2, size=n)
    alice_bases = np.random.randint(2, size=n)

    for i, qc in enumerate(message):
        if alice_bits[i] == 1:
            qc.x(0)
        if alice_bases[i] == 1:
            qc.h(0)

    # modificacion
    if eve:
        eve_bases = np.random.randint(2, size=n)
        for i, qc in enumerate(message):
            if eve_bases[i] == 1:
                qc.h(0)
            qc.measure(qubit_index, 1)
            if eve_bases[i] == 1:
                qc.h(0)

    bob_bases = np.random.randint(2, size=n)
    for i, qc in enumerate(message):
        if bob_bases[i] == 1:
            qc.h(0)

    classical_bit_index_for_bob_measurement = 0
    for qc in message:
        qc.measure(qubit_index, classical_bit_index_for_bob_measurement)

    cbit = classical_bit_index_for_bob_measurement
    bob_measured_bits = []
    for qc in message:
        transpiled_qc = transpile(qc, backend)
        result = backend.run(transpiled_qc, shots=1).result()
        counts = result.get_counts()
        measured_bit = list(counts.keys())[0][::-1][cbit]
        bob_measured_bits.append(measured_bit)

    alice_key, bob_key = [], []
    for i in range(n):
        if alice_bases[i] == bob_bases[i]:
            alice_key.append(int(alice_bits[i]))
            bob_key.append(int(bob_measured_bits[i]))

    errores = sum(a != b for a, b in zip(alice_key, bob_key))
    qber = errores / len(alice_key) if len(alice_key) > 0 else 0
    return qber, alice_key, bob_key


eve = input("Eve? s o n: ").strip().lower() in ("s", "si", "sí", "y", "yes")


if eve:
    qber_con, _, _ = bb84(eve=True)
    print(f"QBER con atacante: {qber_con:.4f}")
if eve == False:
   qber_con, _, _ = bb84(eve=False)
   print(f"QBER con atacante: {qber_con:.4f}")