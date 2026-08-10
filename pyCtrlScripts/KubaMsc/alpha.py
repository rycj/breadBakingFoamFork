T = 303  # K
L = 0.2  # m
Beta = 1 / T  # K^-1
dT = 5  # K
cpVz = 994.75  # J/kg/K
muVz = 18.635e-6  # Pa*s
M = 0.02884  # kg/mol
p = 101325  # Pa
R = 8.314
rhoVz = (M * p) / (R * T)  # kg/m3
nuVz = muVz / rhoVz  # J/kg*s
lambdaVz = 0.0265  # W/m/K
g = 9.81  # m/s^2

Pr = cpVz * muVz / lambdaVz
Gr = (g * Beta * dT * L**3) / (nuVz**2)
Ra = Pr * Gr
Nu = (
    0.825 + ((0.387 * Ra ** (1 / 6)) / ((1 + (0.492 / Pr) ** (9 / 16)) ** (8 / 27)))
) ** 2
print(Nu)
