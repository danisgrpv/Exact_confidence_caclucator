#include <mpi.h>
#include <vector>
#include <string>
#include <numeric>
#include <iostream>
#include "generator.h"
#include "normal_cdf.h"
#include "get_success.h"
#include "user_mpifuncs.h"
#include "file_processing.h"
#include "vector_processing.h"


int main(int argc, char** argv)
{
    int stop_flag;
    int srms;
    int tfms;
    int rank;
    int nprocs;
    int task_size;
    int sample_size;
    int packet_size;
    int event_counter;

    double mean;
    double std;
    double lower_bound;
    double upper_bound;
    double coverage;

    std::vector<int> local_successes;
    std::vector<double> survivals;
    std::vector<double> tolerance;
    std::vector<double> random_sample;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // Загрузка исходных данных управляющим процессом
    if (rank == 0)
    {
        // Загрузка параметров задачи
        sample_size = read_vector("..\\DATA\\SAMPLE_SIZE.txt")[0];
        packet_size = read_vector("..\\DATA\\PACKET_SIZE.txt")[0];

        // Загрузка исходных сеток задачи
        survivals = read_vector("..\\DATA\\SURVIVALS.txt");
        tolerance = read_vector("..\\DATA\\TOLERANCE_FACTORS.txt");
       
        // Размеры исходных сеток
        srms = survivals.size();
        tfms = tolerance.size();
        task_size = srms * tfms * tfms;
    }

    // Широковещательная передача исходных данных
    MPI_Bcast(&tfms, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&srms, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sample_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&packet_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&task_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Выделение памяти на всех процессах
    tolerance.resize(tfms);
    survivals.resize(srms);
    local_successes.resize(task_size);
    random_sample.resize(sample_size);
    // Широковещательная передача вычислительных сеток задачи
    MPI_Bcast(tolerance.data(), tolerance.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(survivals.data(), survivals.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);


    stop_flag = 0; // Флаг для остановки задачи
    event_counter = 0; // Счетчик числа статистических событий


    // Начало расчета статистических событий
    do
    {
        // Произведение вычислений на рабочих процессах
        if (rank != 0)
        {
            // Генерация случайной выборки и расчет ее статистик
            random_sample = user_generator(sample_size, rank);
            mean = get_vector_mean(random_sample);
            std = get_vector_std(random_sample, 1);

            /*
            * Для каждой пары коэффициентов расчитываются толерантные границы и 
            * доля накрытия генеральной совокупности (нормально распределенных случайных чисел)
            * данными границами. Для каждой доли накрытия из расчетной сетки survival производится
            * ее сравнение со статистически расчитанной долей накрытия. В случае выполнения необхо-
            * димого условия собития считается успехом, в обратном случае - нет.
            * Уровень доверия к доле накрытия определяется как отношение числа успехов к общему
            * числу статистических испытаний.
            */

            for (int s = 0; s < srms; ++s)
            {
                for (int t1 = 0; t1 < tfms; ++t1)
                {
                    for (int t2 = 0; t2 < tfms; ++t2)
                    {
                        lower_bound = mean - std * tolerance[t1];
                        upper_bound = mean + std * tolerance[t2];
                        coverage = normal_cdf(upper_bound) - normal_cdf(lower_bound);

                        if (coverage >= survivals[s])
                        {
                            local_successes[t1 + t2 * tfms + s * tfms * tfms]++;
                        }
                    }
                }
            }

            event_counter++; // Увеличение счетчика после произведения одного полного статистического расчета

            // Отправка результатов на управляющий процесс (по пакетам заданного размера - packet_size)
            if ((event_counter % packet_size) == 0)
            {
                MPI_Send(local_successes.data(), task_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
                std::cout << event_counter << std::endl;
            }
        }

        // Прием результатов центральным процессом (по пакетам заданного размера)
        if ((event_counter % packet_size) == 0)
        {
            if (rank == 0)
            {
                std::vector<int> total_successes{ 0 }; // Вектор для суммирования результатов всех рабочих процессов
                total_successes.resize(task_size);
                std::vector<std::vector<double>> vector_to_save(tfms, std::vector<double>(tfms));


                for (int pr = 1; pr < nprocs; ++pr)
                {
                    MPI_Recv(local_successes.data(), task_size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    for (int s = 0; s < srms; ++s)
                    {
                        for (int t1 = 0; t1 < tfms; ++t1)
                        {
                            for (int t2 = 0; t2 < tfms; ++t2)
                            {
                                total_successes[t1 + t2 * tfms + s * tfms * tfms] = total_successes[t1 + t2 * tfms + s * tfms * tfms] + local_successes[t1 + t2 * tfms + s * tfms * tfms];
                                vector_to_save[t1][t2] = total_successes[t1 + t2 * tfms + s * tfms * tfms];
                            }
                        }

                        write_vector2d(vector_to_save, "..\\RESULTS\\" + std::to_string(survivals[s]) + "_GAMMA.txt");
                    }
                }

                // Вывод относительного времени выполнения
                std::cout << "time: " << MPI_Wtime() << std::endl;

                // Проверка флага остановки задачи путем считывания файла
                stop_flag = (int)read_vector("..\\DATA\\STOP_FILE.txt")[0];

                // Завершение расчета при соответствующем флаге остановки задачи
                if (stop_flag == 1)
                {
                    std::cout << std::endl;
                    std::cout << "END OF TASK CALCULATION" << std::endl;
                    MPI_Finalize();
                }
            }
        }
    }

    while (stop_flag != 1);

    MPI_Finalize();
    return 0;
}
