void mpisend_vectordouble(std::vector<double> const& vector_to_send, int dest, int tag, MPI_Comm comm);
void mpisend_vectorint(std::vector<int> const& vector_to_send, int dest, int tag, MPI_Comm comm);
void mpirecv_vectordouble(std::vector<double>& vector_to_recive, int source, int tag, MPI_Comm comm);
void mpirecv_vectorint(std::vector<int>& vector_to_recive, int source, int tag, MPI_Comm comm);
