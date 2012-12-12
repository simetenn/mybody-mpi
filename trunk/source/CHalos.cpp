#include "CHalos.h"

using namespace std;

//Sorting routines
//Sorts halo after size with the largest first
bool HaloSortFunc(CHalo* a, CHalo* b){
	return a->getNrParticles() > b->getNrParticles();
}

//Sorts NrInHalo after size with the largest first
bool NrInHaloSortFunc(int a, int b){
	return a > b;
}


//Constructor, creates empty CHalos
CHalos::CHalos(){
	ParticleSize = myConstants::constants.ParticleSize;
	NrParticles = 0;
	NrHalos = 0;
}


//Create CHalos from an in array on the form:
//nr of halos, nr of particles in halo 1, nr of particles in halo 2, ... ,
//nr of particles of in halo N, halo array 1, halo array 2, ... halo array N]
CHalos::CHalos(CArray* inArray){
	Halos.clear();
	NrHalos = inArray->get(0);
	ParticleSize = myConstants::constants.ParticleSize;
	int particle_count = 1 + NrHalos;
	NrParticles = (inArray->len() - 1 - NrHalos - NrHalos*myConstants::constants.HaloSize)/ParticleSize;


	for (int i = 0; i < NrHalos; i++){
		NrInHalo.push_back(inArray->get(1+i));

		double tmpArray[NrInHalo[i]*ParticleSize + myConstants::constants.HaloSize];

		for (int l = 0; l < myConstants::constants.HaloSize; l++){
			tmpArray[l] = inArray->get(particle_count);
			particle_count++;
		}

		for (int j = 0; j < NrInHalo[i];j++){
			for (int k = 0; k < ParticleSize;k++){
				tmpArray[j*ParticleSize+k+myConstants::constants.HaloSize] = inArray->get(particle_count);
				particle_count++;
			}
		}
		CArray* tmpCArray = new CArray(NrInHalo[i]*ParticleSize + myConstants::constants.HaloSize, tmpArray);

		CHalo* tmpHalo = new CHalo(tmpCArray); // <- Memory leak
		Halos.push_back(tmpHalo);
	}
}


CHalos::~CHalos(){

}



//Sort halos by size, not existing yet
void CHalos::HaloSort(){
	sort(Halos.begin(),Halos.end(),&HaloSortFunc);
	sort(NrInHalo.begin(),NrInHalo.end(),&NrInHaloSortFunc);
}




//Convert from all halos to one array. On the form:
//nr of halos, nr of particles in halo 1, nr of particles in halo 2, ... ,
//nr of particles of in halo N, halo array 1, halo array 2, ... halo array N]
CArray*	 CHalos::Halos2Array(){
	double* Array = new double [ParticleSize*NrParticles+NrHalos+1+myConstants::constants.HaloSize]; // Memory leak
	int particle_count = 1+NrHalos;
	double* tmpArray;
	Array[0] = NrHalos;

	for (int i = 0; i<NrHalos;i++){
		Array[i+1] = NrInHalo[i];
		for (int j=0;j<NrInHalo[i];j++){
			tmpArray = Halos[i]->get(j)->Particle2Array();
			for (int k = 0; k < ParticleSize;k++){
				Array[particle_count] = tmpArray[k];
				particle_count++;
			}
		}
	}
	return new CArray(ParticleSize*NrParticles+NrHalos+1,Array); //Memory leak
}





//Add halo to the existing ones
void CHalos::addHalo(CHalo* inHalo){
	Halos.push_back(inHalo);
}


//Add halos to the existing ones
void CHalos::addHalos(CArray* inArray){
	int oldNrHalos = NrHalos;
	int oldNrParticles = NrParticles;

	int newNrHalos = inArray->get(0);
	NrHalos += newNrHalos;
	vector<int> tmpNrInHalo;

	int newNrParticles = 0;
	for (int i = 1; i <= newNrHalos; i++) {
		newNrParticles += inArray->get(i);
	}


	NrParticles += newNrParticles;

	int particle_count = 1+newNrHalos;
	//cout << "in addHalos" << endl;
	for (int i = 0; i < newNrHalos; i++){

		NrInHalo.push_back(inArray->get(1+i));

		tmpNrInHalo.push_back(inArray->get(1+i));

		double tmpArray[tmpNrInHalo[i]*ParticleSize + myConstants::constants.HaloSize];


		for (int l = 0; l < myConstants::constants.HaloSize; l++) {

			tmpArray[l] = inArray->get(particle_count);

			particle_count++;
		}

		for (int j = 0; j < tmpNrInHalo[i];j++){
			for (int k = 0; k < ParticleSize;k++){
				tmpArray[j*ParticleSize + k + myConstants::constants.HaloSize] = inArray->get(particle_count);
				particle_count++;
			}
		}
		CArray* tmpCArray = new CArray(tmpNrInHalo[i]*ParticleSize + myConstants::constants.HaloSize, tmpArray);
		CHalo* tmpHalo = new CHalo(tmpCArray); // <- Memory leak
		Halos.push_back(tmpHalo);
	}
}


//Remove halo nr #element from CHalos
void CHalos::removeHalo(int element){
	NrInHalo.erase(NrInHalo.begin()+element);
	Halos.erase(Halos.begin() + element);
}






//Add two CHalos. Not tested, so unsure if correct
CHalos CHalos::operator+(CHalos* inCHalo){
	int inNrParticles = inCHalo->getNrParticles();
	int inNrHalos = inCHalo->getNrHalos();
	int particle_count = NrHalos+inNrHalos+1;

	CArray* resHalos = new CArray(ParticleSize*(NrParticles+inNrParticles)+NrHalos+inNrHalos+1);
	double tmpArray[ParticleSize];

	resHalos[0]=NrParticles+inNrParticles;

	for (int i = 0; i<NrHalos;i++){
		resHalos[particle_count] = NrInHalo[i];
		particle_count++;
	}

	for (int i = 0; i<inNrHalos;i++){
		resHalos[particle_count] = inCHalo->getNrInHalo(i);
		particle_count++;
	}

	for (int i = 0; i<NrHalos;i++){
		CArray* tmpHaloArray = Halos[i]->Halo2Array();
		for (int j = 0;j < tmpHaloArray->len();j++){
			resHalos[particle_count] = tmpHaloArray->get(j);
			particle_count++;
		}
	}

	for (int i = 0; i<inNrHalos;i++){
		CArray* tmpHaloArray = inCHalo->getHalo(i)->Halo2Array();
		for (int j = 0;j < tmpHaloArray->len();j++){
			resHalos[particle_count] = tmpHaloArray->get(j);
			particle_count++;
		}
	}
	return CHalos(resHalos);
}




//Return total nr of particles
int CHalos::getNrParticles(){
	return NrParticles;
}


//Return nr of halos
int CHalos::getNrHalos(){
	return NrHalos;
}


//return nr of particles in a halo
int CHalos::getNrInHalo(int element){
	return NrInHalo[element];
}


//Return a halo
CHalo* CHalos::getHalo(int element){
	return Halos[element];
}

//Return a halo
CHalo* CHalos::operator[](int element){
	return Halos[element];
}

//Returns CParticles* for a given halo
CParticles* CHalos::getParticles(int element){
	return Halos[element]->getParticles();
}






//Print out all particles one by one
void CHalos::print(){
	for (int i = 0;i<Halos.size();i++){
		Halos[i]->print();
	}
}


//Print out all halos, together with some information
void CHalos::printHalos(){
	cout << "__________________________________" << endl;
	for (int i = 0;i < NrHalos; i++){
		cout << "								   " << endl;
		cout << "Halo nr: " << i << endl;
		cout << "Nr of particles in halo: " << NrInHalo[i] << endl;
		cout << "Position of halo: ";
		Halos[i]->getMeanP()->print();
		cout << "Velocity of halo: ";
		Halos[i]->getMeanV()->print();
		cout << "__________________________________" << endl;
	}
}




//Load a binary file from a N-body simulation into memory
void CHalos::loadBin(string Filename){

	ifstream file((myConstants::constants.data + Filename).c_str(), ios::in | ios::binary);

	cout << "---------------------------------" << endl;
	cout << "Reading file " << Filename << endl;

	unsigned int count = -1;

	//Reading binary file into memory
	file.read((char *)&count, sizeof(unsigned int));
	cout << count << endl;
	particle_save* block = new particle_save[count];
	file.read((char *)block, sizeof(particle_save)*count);


	Halos.clear();
	NrInHalo.clear();
	NrHalos = 1;

	CHalo* tmpHalo = new CHalo();
	Halos.push_back(tmpHalo);

	cout << "Copying data ..." << endl;
	cout << "Nr of particles: "<< count << endl;

	//Saving data into existing structure
	//Saving all Particles into the first halo in Halos, get with Halos[0]
	AllParticles.resize(count);
	for (int i=0;i<count;i++) {
		CParticle* tmpParticle = &AllParticles[i];

		tmpParticle->setPosition(block[i].P.x,block[i].P.y,block[i].P.z);
		tmpParticle->setVelocity(block[i].V.x,block[i].V.y,block[i].V.z);
		tmpParticle->setAcceleration(0,0,0);

		tmpHalo->addParticle(tmpParticle);
	}

	cout << "Done loading!" << endl;
	cout << "---------------------------------" << endl;
	file.close();
	delete[] block;

	NrParticles = count;
	NrInHalo.push_back(NrParticles);
	LinkingLength = pow(1./NrParticles,1./3);

}


//Load a text file, with all information about each particle
void CHalos::loadData(string Filename){
	/*vector<string> strData;

	ifstream file((myConstants::constants.data + Filename).c_str());
	string line;

	Halos.clear();
	NrInHalo.clear();
	NrHalos = 1;

	CHalo* tmpHalo = new CHalo();
	Halos.push_back(tmpHalo);
	double tmpData [ParticleSize];
	NrParticles = 0;

	if (file.is_open()){
		getline(file,line);
		split(strData, line, is_any_of(" "));

		for (int i = 0; i < ParticleSize; i++){
			tmpData[i] = atof(strData[i].c_str());
		}

		tmpHalo->addParticle(new CParticle(tmpData));
		NrParticles++;

		while (!file.eof()){
			split(strData, line, is_any_of(" "));

			for (int i = 0; i < ParticleSize; i++){
				tmpData[i] = atof(strData[i].c_str());
			}

			tmpHalo->addParticle(new CParticle(tmpData));
			NrParticles++;
			getline(file,line);

		}
		file.close();
	}
	else cout << "Unable to open file" << endl;
	NrInHalo.push_back(NrParticles);
	LinkingLength = pow(1./NrParticles,1./3);*/
}





//Save NrParticles2File evenly distributed particles to file
//Mainly used to create smaller datasets
void CHalos::save(string Filename, int NrParticles2File){
	fstream file;
	double* tmpArray;// = double[ParticleSize];
	file.open((myConstants::constants.data + Filename).c_str(), ios::out);

	cout << "---------------------------------" << endl;
	cout << "Saving "<< NrParticles2File <<" particles to file" << endl;
	cout << "." << endl;
	cout << ".." << endl;
	cout << "..." << endl;

	double delta = NrParticles/(double)NrParticles2File;
	//Saves data for each particle to file
	for (double i = 0;i<NrParticles;i+=delta){
		tmpArray = Halos[0]->get((int)i)->Particle2Array();

		for (int j = 0; j < ParticleSize; j++) {
			file << tmpArray[j] << " ";
		}

		if (i != NrParticles-1) file << endl;
	}
	file.close();
	cout << "Finished saving to file" << endl;
}



//Save positions for each particle belonging to a halo to a text file,
//together with which halo it belongs too
void CHalos::saveP(string Filename){
	fstream file;
	CVector tmpP;
	file.open((myConstants::constants.data + Filename).c_str(), ios::out);

	//Saves position data for each particle to file
	for (int i = 0;i<NrHalos;i++){
		for (int j = 0;j<NrInHalo[i];j++){
			tmpP = Halos[i]->get(j)->getP();
			file << tmpP.x() << " " << tmpP.y()<< " " << tmpP.z()<< " " <<	i << endl;
		}
	}
	file.close();
}


//Save the size of all the halos to a txt fil
void CHalos::saveSize(string Filename){
	fstream file;
	CVector tmpP;
	file.open((myConstants::constants.data + Filename).c_str(), ios::out);
	//sort(tmpNrInHalo.begin(),tmpNrInHalo.end());

	//Saves position data for each particle to file
	for (int i = 0;i < NrHalos; i++){
		file << NrInHalo[i] << endl;
	}
	file.close();
}


//Calculate halo statistics for all halos
void CHalos::CalculateAllStatistics(){
	for (int i = 0; i < NrHalos; i++) {
		Halos[i]->CalculateStatistics();
	}
}


//Calculating Friend of Friend using recursion, without a grid.
//Scales as N^2
void CHalos::FriendOfFriendN2(){
	vector<CHalo*> tmpHalos;

	allParticles = *Halos[0]->getParticles();
	searchParticle = allParticles[0];
	CParticle* Particle = searchParticle;

	//Create a linked list of all particles
	Particle->prev=NULL;
	for (int i=1; i < NrParticles;i++){
		Particle->setFlag(0);
		Particle->next = allParticles[i];
		Particle->next->prev = Particle;
		Particle = Particle->next;
	}
	Particle->setFlag(0);
	Particle->next = NULL;


	cout << "---------------------------------" << endl;
	cout << "Calculating Friend of Friend (scaling as N^2)" << endl;
	cout << "." << endl;
	cout << ".." << endl;
	cout << "..." << endl;

	//Using recursion to link all particles belonging to a halo
	while (true){
		Particle = findParticle();

		if (Particle == NULL) break;
		else {
			CHalo* tmpHalo = new CHalo();
			tmpHalos.push_back(tmpHalo);

			//Calls findNeighbors to find the particles within linking distance
			findNeighbors(Particle, tmpHalo);
		}
	}

	//Only saving halos that has more than HaloLimit particles, updating NrInHalos
	Halos.clear();
	NrInHalo.clear();
	for (int i = 0; i < tmpHalos.size(); i++){
		if (tmpHalos[i]->getNrParticles() > myConstants::constants.HaloLimit){
			Halos.push_back(tmpHalos[i]);
			NrInHalo.push_back(tmpHalos[i]->getNrParticles());
		}
		NrHalos = Halos.size();
	}

	cout << "Finished calculating Friend of Friend" << endl;
	cout << "---------------------------------" << endl;
}

//This might be obsolete with removing particles from the list
//Finds the next particle that has no halo assigned and returns it
CParticle* CHalos::findParticle(){
	while (true){
		if (searchParticle->getFlag() == 0)
			return searchParticle;

		searchParticle = searchParticle->next;

		if (searchParticle == NULL)
			return NULL;

	}
}

//Flags the given particle and adds it to the given halo.
//Then finds the neighboring particles, within the linking length.
//Before calling itself for each particle found this way
void CHalos::findNeighbors(CParticle* inParticle, CHalo* inHalo){
	inParticle->setFlag(1);
	inParticle->RemoveFromList();
	inHalo->addParticle(inParticle);
	double distance;
	CHalo FriendList;

	//Loops through all particles and finds the ones
	//within the linking length not assigned to a halo. Then adds them to a temporary halo
	for (int i = 0; i<allParticles.getNrParticles();i++){
		if (allParticles[i]->getFlag()==0){
			distance = (inParticle->getP() - allParticles[i]->getP()).Length();
			if (distance < myConstants::constants.b*LinkingLength){
				allParticles[i]->setFlag(1);
				FriendList.addParticle(allParticles[i]);
			}
		}
	}

	//Finds the neighboring particles for each particle found to be within
	//the linking length and adds them to the given halo
	for (int i = 0; i<FriendList.getNrParticles();i++){
		findNeighbors(FriendList[i],inHalo);
	}
}






//Friend of Friend methode that uses a grid to speed up the calculations
//Scales as something*log(N)
void CHalos::FriendOfFriendGrid(){
	allParticles = *Halos[0]->getParticles();
	searchParticle = allParticles[0];
	CParticle* Particle = searchParticle;

	//Create a linked list of all particles
	Particle->prev=NULL;
	for (int i=1; i < NrParticles;i++){
		Particle->setFlag(0);
		Particle->next = allParticles[i];
		Particle->next->prev = Particle;
		Particle = Particle->next;
	}
	Particle->setFlag(0);
	Particle->next = NULL;


	cout << "---------------------------------" << endl;
	cout << "Initializing grid" << endl;
	cout << "." << endl;
	cout << ".." << endl;
	cout << "..." << endl;

	//Initialize the Grid
	CVector min(-1,-1,-1);
	CVector max(1,1,1);

	//Uncomment to manually set the linking length
	//LinkingLength = myConstants::constants.LinkingLength;

	int Width = (int) 2./LinkingLength;

	Grid.initialize(&min,&max,Width);
	Grid.Populate(Halos[0]->getParticles());

	cout << "Finished initializing grid" << endl;
	cout << "---------------------------------" << endl;
	cout << "Calculating Friend of Friend Grid" << endl;
	cout << "." << endl;
	cout << ".." << endl;
	cout << "..." << endl;

	//Using recursion to link all particles belonging to a halo
	CHalo tmpHalo;

	Halos.clear();
	NrInHalo.clear();

	//int count = 0;

	while (true){
		Particle = findParticle();
		if (Particle == NULL) break;
		else {
			//Calls findNeighbors to find the particles within
			//linkingsort(LinkingLengths.begin(),LinkingLengths.end())
			tmpHalo.clear();
			Particle->RemoveFromListGrid();
			Particle->setFlag(1);
			findNeighborsGrid(Particle, &tmpHalo);

			//Only saving halos that has more than HaloLimit particles, updating NrInHalos
			//count += tmpHalo.getNrParticles();
			if (tmpHalo.getNrParticles() > myConstants::constants.HaloLimit) {
				Halos.push_back(new CHalo(&tmpHalo));
				NrInHalo.push_back(tmpHalo.getNrParticles());
			}
		}
	}
	NrHalos = Halos.size();


	//cout << "Have all particles survived?: "<< count << endl;

	cout << "Finished calculating Friend of Friend" << endl;
	cout << "---------------------------------" << endl;

	cout << "Calculating Halo statistics" << endl;
	cout << "." << endl;
	cout << ".." << endl;
	cout << "..." << endl;

	CalculateAllStatistics();

	cout << "Finished calculating Halo statistics" << endl;
	cout << "---------------------------------" << endl;

	//Sort the halos from biggest to smalest for maximum speed during the parallelisation
	HaloSort();
}


//Flags the given particle and adds it to the given halo.
//Then finds the neighboring particles, within 26 closest cubes in the grid.
//Before calling itself for each particle found this way
void CHalos::findNeighborsGrid(CParticle* inParticle, CHalo* inHalo){
	inHalo->addParticle(inParticle);
	inParticle->RemoveFromList();

	CVector Position = Grid.getPosition(inParticle);
	CHalo FriendList;
	CParticle* tmpParticle;
	double distance;
	//Loops through all particles and finds the ones
	//within the linking length not assigned to a halo. Then adds them to a temporary halo
	double L = myConstants::constants.b*LinkingLength;//pow(myConstants::constants.b*LinkingLength,2.0);

	CParticle* next;
	for (int i=-1;i<=1;i++){
		for (int j=-1;j<=1;j++) {
			for (int k=-1;k<=1;k++) {
				tmpParticle = Grid.getPeriodic(Position.x()+i,Position.y()+j,Position.z()+k);

				while (tmpParticle != NULL) {
					next = tmpParticle->nextGrid;
					if (tmpParticle->getFlag() == 0){
						distance = (inParticle->getP() - tmpParticle->getP()).Length();
						if (distance < L){
							tmpParticle->setFlag(1);
							FriendList.addParticle(tmpParticle);
							tmpParticle->RemoveFromListGrid();
						}
					}
					tmpParticle = next;
				}
			}
		}
	}

	//Finds the neighboring particles for each particle found to be within
	//the linking length and adds them to the given halo
	for (int i = 0; i<FriendList.getNrParticles();i++){
		findNeighborsGrid(FriendList[i], inHalo);
	}
}






//Runs the splitting routine for all halos to split each into several subhalos
void CHalos::SplitHalos(){
	for (int i = 0; i < NrHalos; i++) {
		//cout << "-------------------------------------------------" << endl;
		//cout << "Splitting halo in phase space" << endl;
		//cout << "Splitting halo: " << i << endl;
		//cout << "-------------------------------------------------" << endl;
		Halos[i]->createSubHalos();
	}
}






//A routine to be run in the master process to do calulations on each halo in parallel.
//It sends the data from each halo to all available processors,
//then sends out new data as soon as a processor finishes its task
CHalos* CHalos::master(){
	CMPI MPI;
	int count = 0;
	int processor;
	int size = MPI.getSize();
	CHalos* FinalHalos = new CHalos();
	MPI_Request Req [size-1];
	vector<CArray*> Array (size-1);
	CParticle tmpParticle;

	//Initialize, sending one halo to each processor
	for (int p = 1; p < size; p++){
		cout << "Initializing for halo nr: " << p-1 << endl;

		//Add how many particles in halo to be sent
		//and that it only is one halo to the CArray
		Array[p-1] = Halos[count]->Halo2Array();
		Array[p-1]->front(NrInHalo[count]);
		Array[p-1]->front(1);
		MPI.End(p,0);
		Array[p-1]->send(p);
		Array[p-1]->recieve(p,&Req[p-1]);
		count++;
	}


	cout << "-------------------------------------------------" << endl;
	cout << "Finished distributing halos to each processor" << endl;
	cout << "-------------------------------------------------" << endl;
	//Send halo to processor as soon as a processor finishes
	while (count < NrHalos) {
		cout << "Calculating for halo nr: " << count << endl;
		//Listening for a processor to finish
		processor = MPI.listener(Req);
		FinalHalos->addHalos(Array[processor-1]);
		Array[processor-1] =  Halos[count]->Halo2Array();

		MPI.End(processor,0);

		//Add how many particles in halo to be sent
		//and that it only is one halo to the CArray
		Array[processor-1]->front(NrInHalo[count]);
		Array[processor-1]->front(1);

		//Send the array and start listening for the response
		Array[processor-1]->send(processor);
		Array[processor-1]->recieve(processor,&Req[processor-1]);
		count++;

	}
	//Waiting for all processors to finish their last task
	cout << "Waiting for the rest of the processors to finish" << endl;
	MPI.WaitAll(Req);

	for (int i = 0; i < size-1;i++){
		//processor = MPI.listener(Req);
		//FinalHalos->addHalos(Array[processor]);
		//cout << "finished for" << endl;
		FinalHalos->addHalos(Array[i]);
	}
	cout << "Finished" << endl;

	//Send end signal to all processors
	for (int p = 1;p < size;p++){
		MPI.End(p,1);
	}

	return FinalHalos;
}



//Slave processor, listening for data from the master processor
//Then does something with the data before sending it back to the
//master processor
void CHalos::slave(){

	CArray HalosArray;
	CMPI MPI;
	int flag;
	int rank = MPI.getRank();

	CParticle tmpParticle;

	while (true) {
		if (MPI.ifEnd() == 1) break;
		HalosArray.recieve_slave();
		int tmpLength = HalosArray.len();
		CHalos SlaveHalos (&HalosArray);

		//Do something in each slave processor here
		SlaveHalos.SplitHalos();
		//SlaveHalos.getHalo(0)->printSubHalos();
		SlaveHalos.getHalo(0)->SubHalos2Array()->send_slave_modified(tmpLength);
	}
}


