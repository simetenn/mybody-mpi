#include "CMPI.h"
#include <iostream>

using namespace std;

//Constructor, initialize MPI
CMPI::CMPI(){
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	currentcount = 1;
}


//Constructor, initialize MPI
CMPI::CMPI(int argc, char **argv){
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	currentcount = 1;
}

//Initialize MPI
void CMPI::initialize_CMPI(int argc, char **argv){
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	currentcount = 1;
}



CMPI::~CMPI(){
}

//send an array from the master processor to a slave processor
void CMPI::send_array_master(double* master_send_array, int processor, int length){
	MPI_Request Req;
	//send the length
	MPI_Send(&length,1,MPI_INT,processor,processor,MPI_COMM_WORLD);
	//send the array
	MPI_Send(master_send_array,length,MPI_DOUBLE,processor,processor+size,MPI_COMM_WORLD);
}


//Recieve an array in the Master processor from the slave node
double* CMPI::receive_array_master(int processor, int& master_length, MPI_Request* Req){
	MPI_Status Stat;
	MPI_Request* tmpReq;

	//recieve length of array
	//MPI_Recv(&master_length,1,MPI_INT,processor,processor+2*size,MPI_COMM_WORLD,&Stat);
	//double* master_receive_array = new double [master_length]; //<- Memory leak

	//Create an array longer than needed to avoid having to send the length back
	//from the master processor
	master_length += (1+myConstants::constants.HaloSize)*myConstants::constants.MaxHalos;
	double* master_receive_array = new double [master_length];

	//Recieve the array, non blocking
	MPI_Irecv(master_receive_array,master_length, MPI_DOUBLE,processor,processor+3*size,MPI_COMM_WORLD, Req);
	return master_receive_array;
}


//Send an array to the Master processor from the slave processor
void CMPI::send_array_slave(double* slave_send_array, int length){
	//MPI_Send(&length,1,MPI_INT,0,rank+2*size,MPI_COMM_WORLD);
	MPI_Send(slave_send_array,length,MPI_DOUBLE,0,rank+3*size,MPI_COMM_WORLD);
}


//Recieve an array in the Slave process from the Master process
double* CMPI::receive_array_slave(int& slave_length){
	MPI_Status Stat;

	MPI_Recv(&slave_length,1,MPI_INT,0,rank,MPI_COMM_WORLD,&Stat);

	double* slave_receive_array = new double [slave_length]; //<- memory leak
	MPI_Recv(slave_receive_array,slave_length,MPI_DOUBLE,0,rank+size,MPI_COMM_WORLD,&Stat);
	
	return slave_receive_array;
}


//Specialized, not used in CArray.
void CMPI::send_array_master_all(double* master_send_array, int length){
	int average_array_length = length/(size-1);
	int reminder = length % (size-1);
	int sub_array_length = average_array_length;
	int array_start_index = 0;

	for (int p=1; p < size; p++){
		if (p < reminder+1){
			sub_array_length = average_array_length+1;
		}
		else {
			sub_array_length = average_array_length;
		}

		CMPI::send_array_master(&master_send_array[array_start_index],p,sub_array_length);
		array_start_index += sub_array_length;
	}

}


//Master process recieve from all slave processes
double** CMPI::receive_array_master_all(int& slave_length){

	MPI_Status Stat[size-1];
	MPI_Status Stat2;
	MPI_Request Req[size-1];
	double** result = new double*[size-1];

	for (int p=1; p < size; p++){
		MPI_Recv(&slave_length,1,MPI_INT,p,p+2*size,MPI_COMM_WORLD, &Stat2);
		result[p-1] = new double [slave_length];
		MPI_Irecv(result[p-1],slave_length,MPI_DOUBLE,p,p+3*size,MPI_COMM_WORLD, &Req[p-1]);
	}

	MPI_Waitall(size-1,Req, Stat);
	return result;

}


//Wait for one processor to finish
int CMPI::WaitOne(MPI_Request* Req){
	int processor;
	MPI_Status Stat [size-1];
	MPI_Waitany(size-1,Req,&processor, Stat);
	return processor;
}


//Wait for all processors to finish
void CMPI::WaitAll(MPI_Request* Req){
	MPI_Status Stat [size-1];
	MPI_Waitall(size-1,Req,Stat);
}


//Return the rank of the current processor
int CMPI::getRank(){
	return rank;
}


//Return total nr of processors
int CMPI::getSize(){
	return size;
}


//Test which slave processor finishes first and return processor nr
int CMPI::listener(MPI_Request* Req){
	MPI_Status Stat;
	int flag, p, test;

	//Eternal loop to keep listening until a processor finishes
	while (true) {
		test = currentcount;
		currentcount++;
		if (currentcount == size) currentcount = 1;
		
		MPI_Test(&Req[test-1],&flag,&Stat);
		if (flag == 1) return test;
	}
}


//Send a signal to a slave processor. 1 tells the prosessor it is finished and everything else to continue.
void CMPI::End(int processor, int sendflag){
	MPI_Request Req;
	MPI_Isend(&sendflag,1,MPI_INT,processor,processor+10*size,MPI_COMM_WORLD,&Req);
}


//Recieve a signal from the master process to check if the slave process should end or continue
int CMPI::ifEnd(){
	MPI_Status Stat;
	MPI_Request Req;
	int flag;
	MPI_Recv(&flag,1,MPI_INT,0,rank+10*size,MPI_COMM_WORLD, &Stat);
	return flag;
}
