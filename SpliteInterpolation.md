# Натуральный кубический сплайн

## 1. Вход: точки интерполяции и проверка
Есть набор узлов: ${(x_0, y_0) < (x_1, y_1) < \dots < (x_{n-1}, y_{n-1})}$, где ${x_i \in \mathbb{R}, y_i \in T}$

В функции ```ValidateSplineInput``` проверяется, что число точек совпадает и ${n ≥ 2}$, и узлы строго упорядочены ${x_0 < x_1 < \dots < x_{n-1}, y_{n-1}}$.
## 2. Шаги сетки ${h_i}$
Cчитаем шаги сетки: ${h_i = x_{i+1} - x_i}$, где ${i = 0,\dots, n - 2}$
```text
MutableArraySequence<double> h;
for (int index = 0; index < n - 1; index++) {
    h.Append(xs.Get(index + 1) - xs.Get(index));
}
```
## 3. Правая часть системы: вектор ${α}$:
Определяется вектор ${α = (α_0, \dots, a_{n-1})}$.\
На концах ${α_0 = 0, α_{n-1} = 0}$.\
Для остальных узлов: ${α_i = 3(\frac{y_{i+1}-y_i}{h_i} - \frac{y_i-y_{i-1}}{h_{i-1}})}$, ${i = 1, \dots, n - 2}$.
```text
MutableArraySequence<T> alpha;
alpha.Append(0);
for (int index = 1; index < n - 1; index++) {
    alpha.Append(
        3.0 * (ys.Get(index + 1) - ys.Get(index)) / h.Get(index) -
        3.0 * (ys.Get(index) - ys.Get(index - 1)) / h.Get(index - 1)
    );
}
alpha.Append(0);
```
## 4. Трёхдиагональная система и метод прогонки
Это реализация метода Томаса (прогонки) для решения трёхдиагональной системы вида:\
${a_ib_i}$

