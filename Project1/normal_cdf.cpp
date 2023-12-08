#include <corecrt_math_defines.h>
#include <cmath>

double normal_cdf(double value)
/*
Фунция возвращает значение функции нормального распределения в заданной точке

Параметры:
    1. double value - значение варианты

Возвращаемые значение:
    1. double - значение функции плотности нормального распределения
*/

{
    return 0.5 * erfc(-value * M_SQRT1_2);
}