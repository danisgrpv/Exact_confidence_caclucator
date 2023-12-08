#include <string>
#include <vector>
#include <fstream>
#include <iostream>


std::vector<double> read_vector(std::string s)
{
    double number;
    std::vector<double> read_vector;
    std::ifstream file(s);

    while (!file.eof())
    {
        file >> number;
        read_vector.push_back(number);
        // std::cout << number << std::endl;
    }

    file.close();
    return read_vector;
}


void write_vector(std::vector<double> vector_to_write, std::string s)
{
    std::ofstream file(s);

    for (int i = 0; i < vector_to_write.size(); ++i)
    {
        file << vector_to_write[i] << std::endl;
    }

    file.close();
}


void write_vector2d(std::vector<std::vector<double>> vector_to_write, std::string s)
{
    std::ofstream file(s);
    int const ROW_NUM = vector_to_write.size();
    int const COL_NUM = vector_to_write[0].size();

    for (int r = 0; r < ROW_NUM; ++r)
    {
        for (int c = 0; c < COL_NUM; ++c)
        {
            file << vector_to_write[r][c] << " ";
        }
        file << std::endl;
    }

    file.close();
}
