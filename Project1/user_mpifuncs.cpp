#include <mpi.h>
#include <vector>

void mpisend_vectordouble(std::vector<double> const& vector_to_send, int dest, int tag, MPI_Comm comm)
{
	/*
	* Функция для отправления одномерного вектора чисел с двойной точностью
	*/
	unsigned len = vector_to_send.size();
	// Отправление размера вектора
	MPI_Send(&len, 1, MPI_UNSIGNED, dest, tag, comm);
	if (len != 0)
	{
		MPI_Send(vector_to_send.data(), len, MPI_DOUBLE, dest, tag, comm);
	}
}


void mpisend_vectorint(std::vector<int> const& vector_to_send, int dest, int tag, MPI_Comm comm)
{
	/*
	* Функция для отправления одномерного вектора чисел типа integer
	*/
	unsigned len = vector_to_send.size();
	// Отправление размера вектора
	MPI_Send(&len, 1, MPI_UNSIGNED, dest, tag, comm);
	if (len != 0)
	{
		MPI_Send(vector_to_send.data(), len, MPI_INT, dest, tag, comm);
	}
}


void mpirecv_vectordouble(std::vector<double>& vector_to_recive, int source, int tag, MPI_Comm comm)
{
	/*
	* Функция для приема одномерного вектора чисел с двойной точностью
	*/
	unsigned len;
	MPI_Status status;
	MPI_Recv(&len, 1, MPI_UNSIGNED, source, tag, comm, &status);
	if (len != 0)
	{
		vector_to_recive.resize(len);
		MPI_Recv(vector_to_recive.data(), len, MPI_DOUBLE, source, tag, comm, &status);
	}
	else
	{
		vector_to_recive.clear();
	}
}


void mpirecv_vectorint(std::vector<int>& vector_to_recive, int source, int tag, MPI_Comm comm)
{
	/*
	* Функция для приема одномерного вектора чисел типа integer
	*/
	unsigned len;
	MPI_Status status;
	MPI_Recv(&len, 1, MPI_UNSIGNED, source, tag, comm, &status);
	if (len != 0)
	{
		vector_to_recive.resize(len);
		MPI_Recv(vector_to_recive.data(), len, MPI_INT, source, tag, comm, &status);
	}
	else
	{
		vector_to_recive.clear();
	}
}