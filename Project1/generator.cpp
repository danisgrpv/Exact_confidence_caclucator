#include <ctime>
#include <random>
#include <vector>
#include <algorithm>


std::vector<double> user_generator(int SAMPLE_SIZE, int rank)
{
    /*
    Функция заполняет одномерный вектор заданного размера случайными числами с определенным законом распределения

    Параметры:
        1. int const SAMPLE_SIZE - размер вектора

    Возвращаемые значения:
        1. std::vector<double> - вектор нормально распределенных случайных чисел
    */

    int const MEAN = 0;
    int const STD = 1;

    std::vector<double> sample(SAMPLE_SIZE);
    sample.reserve(SAMPLE_SIZE);

    // Генератор случайных чисел
    std::linear_congruential_engine<std::uint_fast64_t, 48271, 0, 2147483647> user_generator;

    user_generator.seed(rand() ^ std::time(nullptr) + rank);
    std::normal_distribution<double> NormalDistribution{ MEAN, STD };

    auto fill_vector = [&NormalDistribution, &user_generator]()
        {
            return NormalDistribution(user_generator);
        };

    std::generate(begin(sample), end(sample), fill_vector);
    return sample;
}