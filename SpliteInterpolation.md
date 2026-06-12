# Натуральный кубический сплайн

## 1. Вход: точки интерполяции и проверка
Есть набор узлов: ${(x_0, y_0) < (x_1, y_1) < \dots < (x_{n-1}, y_{n-1})}$, где ${x_i \in \mathbb{R}, y_i \in T}$

В функции ```ValidateSplineInput``` проверяется, что число точек совпадает и ${n ≥ 2}$, и узлы строго упорядочены ${x_0 < x_1 < \dots < x_{n-1}, y_{n-1}}$.
## 2. Шаги сетки ${h_i}$
Cчитаем шаги сетки: ${h_i = x_{i+1} - x_i}$, при ${i = 0,\dots, n - 2}$
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
Это реализация метода Томаса (прогонки) для решения трёхдиагональной системы вида при ${i = 0,\dots, n - 1}$:\
${a_ic_{i-1} + b_ic_i + d_ic_{i+1} = α_i}$, ${a_i}$ - поддиагональ, ${b_i}$ - главная диагональ, ${d_i}$ - наддиагональ. ${c_i}$ - неизвестные трехдиагольнаьной системы\
${a_i = h_{i-1}}$,\
${b_i = 2(h_{i-1} + h_i)}$,\
${d_i = h_i}$.

Прямой ход:\
Задаём начальные значения: ${μ_0 = 0, z_0 = 0}$;\
Затем вычисляем для ${i = 0,\dots, n - 2}$:\
${L_i = b_i - a_iμ_{i-1}}$,\
${μ_i = \frac{d_i}{L_i}}$,\
${z_i = \frac{α_i - a_iz_{i-1}}{L_i}}$.\
На правом конце: ${z_{n-1} = 0}$.
```text
MutableArraySequence<double> mu;
MutableArraySequence<T> z;

mu.Append(0.0);
z.Append(0.0);

for (int index = 1; index < n - 1; index++) {
    double ai = h.Get(index - 1);
    double bi = 2.0 * (h.Get(index - 1) + h.Get(index));
    double di = h.Get(index);

    double Li = bi - ai * mu.Get(index - 1);i
    mu.Append(di / Li);
    z.Append((alpha.Get(index) - ai * z.Get(index - 1)) / Li);
}

z.Append(0.0);
```
## 5. Нахождение ${a_i, b_i, c_i, d_i}$
### ВАЖНО: Здесь ${a_i, b_i, c_i, d_i}$ это уже коэффициенты кубического сплайна при ${(x-x_i)^0}$, ${(x-x_i)}$, ${(x-x_i)^2}$ и ${(x-x_i)^3}$ соответственно, а не элементы диагоналей из трёхдиагональной системы в пункте 4.
Для каждого отрезка ${[x_i, x_{i+1}]}$ при i = ${i = n - 2,\dots, 0}$:\
${a_i = y_i}$,\
${c_i = z_i - μ_ic_{i+1}}$ - решение трехдиагональной системы, ${c_i}$ - коэффициент при ${(x-x_i)^2}$,\
${b_i = \frac{y_{i+1} - y_i}{h_i} - \frac{h_i}{3}(2c_i + c_{i+1})}$,\
${d_i = \frac{c_{i+1} - c_i}{3h_i}}$.

```text
MutableArraySequence<T> c;
for (int index = 0; index < n; index++) {
    c.Append(0.0);
}

MutableArraySequence<T> b;
MutableArraySequence<T> d;
MutableArraySequence<T> a;

for (int index = 0; index < n - 1; index++) {
    a.Append(ys.Get(index));
    b.Append(0.0);
    d.Append(0.0);
}

for (int index = n - 2; index >= 0; index--) {
    c.Set(index, z.Get(index) - mu.Get(index) * c.Get(index + 1));
    b.Set(index,
          (ys.Get(index + 1) - ys.Get(index)) / h.Get(index) -
          h.Get(index) * (c.Get(index + 1) + 2.0 * c.Get(index)) / 3.0);
    d.Set(index, (c.Get(index + 1) - c.Get(index)) / (3.0 * h.Get(index)));
}
```
## 6. Формирование кусочно полиномиальной функции ${S(x)}$
Локальный кубический сплайн на промежутке ${[x_i, x_{i+1}]}$ при ${i = 1, \dots, n - 2}$.\
${S_i(x) = a_i + b_i(x-x_i) + c_i(x-x_i)^2 + d_i(x-x_i)^3}$.\
Или же:
${S_i(x) = A_{0,i} + A_{1,i}(x-x_i) + A_{2,i}(x-x_i)^2 + A_{3,i}(x-x_i)^3}$.\
Коэффициенты выражаются через ${a_i, b_i, c_i, d_i}$:\
${A_{0,i} = a_i - b_ix_i + c_ix_i^2 - d_ix_i^3}$,\
${A_{1,i} = b_i - 2c_ix_i + 3d_ix_i^2}$,\
${A_{2,i} = c_i - 3d_ix_i}$,\
${A_{3,i} = d_i}$\.

```text
MutableArraySequence<Segment<T>> segments;

for (int index = 0; index < n - 1; index++) {
    double x0 = xs.Get(index);

    T A0 = a.Get(index) - b.Get(index) * x0 + c.Get(index) * x0 * x0 - d.Get(index) * x0 * x0 * x0;
    T A1 = b.Get(index) - 2.0 * c.Get(index) * x0 + 3.0 * d.Get(index) * x0 * x0;
    T A2 = c.Get(index) - 3.0 * d.Get(index) * x0;
    T A3 = d.Get(index);

    MutableArraySequence<T> coeffs;
    coeffs.Append(A0);
    coeffs.Append(A1);
    coeffs.Append(A2);
    coeffs.Append(A3);

    PolynomialFunction<T> function(coeffs);
    segments.Append(Segment<T>(xs.Get(index), xs.Get(index + 1), function));
}
```

## Итоговый кубический спайнлайн:
# ${S(x) = S_i(x)}$ при ${x \in [x_i, x_{i+1}]}$, при ${i = 0,\dots, n - 2}$
