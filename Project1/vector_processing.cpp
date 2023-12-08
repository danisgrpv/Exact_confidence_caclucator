#include <ctime>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>


double get_vector_mean(std::vector<double> input_vector)
{
    double vector_sum;
    double vector_mean;

    vector_sum = std::accumulate(input_vector.begin(), input_vector.end(), 0.0);
    vector_mean = vector_sum / input_vector.size();

    return vector_mean;
}


double get_vector_std(std::vector<double> input_vector, int degree_of_freedom)
{
    /* Функция вычисляет стандартное отклонение элементов вектора

    Параметры:
        1. std::vector<double> input_vector - входной вектор
        2. int degree_of_freedom - число степеней свободы

    Возвращаемые значения:
        1. double std - стандартное отклонение элементов вектора
    */

    double vector_mean;
    double vector_std;
    double total_square_deviation;
    int const vector_size = input_vector.size();
    std::vector<double> deviations_vector(vector_size);

    vector_mean = get_vector_mean(input_vector);
    // применение операции (определенной лямбда-функцией) к диапазону от input_vector.begin() до input_vector.end(), запись в диапазон начиная с difference_vector.begin()
    std::transform(input_vector.begin(), input_vector.end(), deviations_vector.begin(), [vector_mean](double x) {return x - vector_mean; });

    total_square_deviation = std::inner_product(deviations_vector.begin(), deviations_vector.end(), deviations_vector.begin(), 0.0);
    vector_std = std::sqrt(total_square_deviation / (input_vector.size() - degree_of_freedom));

    return vector_std;
}