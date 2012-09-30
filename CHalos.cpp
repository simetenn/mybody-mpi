#include "CHalos.h"

using namespace std;


CHalos::CHalos(){
	CParticle tmpParticle;
	ParticleSize = tmpParticle.getParticleSize();
	nrParticles = 0;
	nrHalos = 0;
}

/*
  CHalos::CHalos(CArray* inArray){
  nrHalos = inArray->get(0);
  CParticle tmpParticle;
  ParticleSize = tmpParticle.getParticleSize();
  int particle_count = 1+nrHalos;
  nrParticles = (inArray->len()-1-nrHalos)/ParticleSize;

  for (int i = 0; i < nrHalos; i++){
  nrinHalo.push_back(inArray->get(1+i));

  double tmpArray[nrinHalo[i]];
  for (int j = 0; j < nrinHalo[i];j++){
  tmpArray[j] = inArray->get(particle_count);
  parti
*/

CHalos::CHalos(CArray* inArray){
	nrHalos = inArray->get(0);
	CParticle tmpParticle;
	ParticleSize = tmpParticle.getParticleSize();
	int particle_count = 1+nrHalos;
	nrParticles = (inArray->len()-1-nrHalos)/ParticleSize;
	//nrinHalo.push_back(nrParticles);

	for (int i = 0; i < nrHalos; i++){
		nrinHalo.push_back(inArray->get(1+i));

		double tmpArray[nrinHalo[i]*ParticleSize];
		for (int j = 0; j < nrinHalo[i];j++){
			for (int k = 0; k < ParticleSize;k++){
				tmpArray[j*ParticleSize+k] = inArray->get(particle_count);
				particle_count++;
			}
		}
		CArray* tmpCArray = new CArray(nrinHalo[i]*ParticleSize, tmpArray);
		CHalo* tmpHalo = new CHalo(tmpCArray); // <- Memory leak
		Halos.push_back(tmpHalo);
	}

}




CHalos::~CHalos(){

}


//Initialize the halos in my own fake datasett
void CHalos::initialize_Halos(){
	nrHalos = 8;
	//vector<CHalo*> tmpHalos = Halos;
	CHalo* oldHalo = Halos[0];
	Halos.clear();
	nrinHalo.clear();
	//Halos.resize(nrHalos);


	for (int i = 0; i < nrHalos; i++){
		CHalo* tmpHalo = new CHalo();

		Halos.push_back(tmpHalo);
	}


	CParticle tmpParticle;
	ParticleSize = tmpParticle.getParticleSize();

	//oldHalo->getParticle(0);

	for (int i = 0; i < nrParticles;i++){
		Halos[(oldHalo->getParticle(i)->get_P()).Quadrant()]->addParticle(oldHalo->getParticle(i));

		//Halos.push_back(Particles[i]);
		//cout << tmpHalo->getCParticles()->get(i)->get_P() << endl;
		//Halos[tmpHalo->getCParticles()->get(i)->get_P()].push_back(tmpHalo[i]);
		//Halos[(tmpHalos[i]->get_P()).Quadrant()].push_back(tmpHalos[i]);
	}

	//cout << Halos[0]->getnrParticles() << endl;

	//cout << nrHalos << endl;
	for (int i = 0; i < nrHalos;i++){
		nrinHalo.push_back(Halos[i]->getnrParticles());
		//nrinHalo.push_back(;
	}
}


void CHalos::HaloSort(){

}


void CHalos::printHalos(){
	for (int i = 0;i < nrHalos; i++){

		cout << "__________________________________" << endl;
		cout << "								   " << endl;
		cout << "Halo nr: " << i << endl;
		cout << "Nr of particles in halo: " << nrinHalo[i] << endl;
		cout << "__________________________________" << endl;
		cout << endl;
		//for (int j = 0;j < Halos[i].size();j++){
		//Halos[i][j]->print_Particle();
		//}
	}
}


void CHalos::addHalo(CHalo* inHalo){
	Halos.push_back(inHalo);
}






//Add halos to the existing ones
void CHalos::addHalos(CArray* inArray){
	int oldnrHalos = nrHalos;
	int oldnrParticles = nrParticles;

	int newnrHalos = inArray->get(0);
	nrHalos += newnrHalos;

	int newnrParticles = (inArray->len()-1-nrHalos)/ParticleSize;
	nrParticles += newnrParticles;

	int particle_count = 0;
	//vector<CParticle* > tmpParticles;
	//vector<double> tmpArray (ParticleSize);

	//Halos.resize(nrHalos);

	for (int i = 0; i < newnrHalos; i++){
		nrinHalo.push_back(inArray->get(1+i));

		double tmpArray[nrinHalo[i]*ParticleSize];
		for (int j = 0; j < nrinHalo[i];j++){
			for (int k = 0; k < ParticleSize;k++){
				tmpArray[j*ParticleSize+k] = inArray->get(particle_count);
				particle_count++;
			}
		}
		CArray* tmpCArray = new CArray(nrinHalo[i]*ParticleSize, tmpArray);
		CHalo* tmpHalo = new CHalo(tmpCArray); // <- Memory leak
		Halos.push_back(tmpHalo);
	}

	/*for (int i = 0; i<newnrHalos;i++){
	  Halos.push_back(CHalo* tmpHalo (Particles));

	  for (int j = 0;j < inArray->get(i+1);j++){
	  for (int k = 0; k < ParticleSize;k++){
	  tmpArray[k] = inArray->get(particle_count*ParticleSize+1+newnrHalos+k);
	  }


	  //takes up extra


	  Particles.push_back(new CParticle); //Slow way to do this??
	  Particles[particle_count+oldnrParticles]->Set_Data(tmpArray);
	  Halos[i].push_back(Particles[oldnrParticles+particle_count]);
	  //tmpParticles.push_back(new CParticle); //Slow way to do this??
	  //tmpParticles[j]->Set_Data(tmpArray);
	  particle_count++;
	  }

	  //Halos.push_back(tmpParticles);
	  //tmpParticles.clear();
	  }*/
}





//Convert from several halos to one array
CArray*	 CHalos::Halos2Array(){//pointer

	//rewrite this when time, but seems to work atm

	double* Array = new double [ParticleSize*nrParticles+nrHalos+1]; // Memory leak
	int particle_count = 1+nrHalos;
	double* tmpArray;
	Array[0] = nrHalos;

	for (int i = 0; i<nrHalos;i++){
		//cout << "Halo nr:" << i << endl;
		//cout << Halos[i].size() << endl;

		Array[i+1] = nrinHalo[i];
		for (int j=0;j<nrinHalo[i];j++){
			tmpArray = Halos[i]->get(j)->Particle2Array();
			for (int k = 0; k < ParticleSize;k++){
				Array[particle_count] = tmpArray[k];
				particle_count++;
			}
		}
	}

	return new CArray(ParticleSize*nrParticles+nrHalos+1,Array); //Memory leak
}



int CHalos::getnrParticles(){
	return nrParticles;
}

int CHalos::getnrHalos(){
	return nrHalos;
}

int CHalos::getnrinHalo(int element){
	return nrinHalo[element];
}

CHalo* CHalos::getHalo(int element){
	return Halos[element];
}


int CHalos::sizeHalos(){
	return nrHalos;
}



CHalos CHalos::operator+(CHalos* inCHalo){
	int innrParticles = inCHalo->getnrParticles();
	int innrHalos = inCHalo->getnrHalos();
	CArray* resHalos = new CArray(ParticleSize*(nrParticles+innrParticles)+nrHalos+innrHalos+1);

	int particle_count = nrHalos+innrHalos+1;

	double tmpArray[ParticleSize];

	resHalos[0]=nrParticles+innrParticles;



	for (int i = 0; i<nrHalos;i++){
		resHalos[particle_count] = nrinHalo[i];
		particle_count++;
	}

	for (int i = 0; i<innrHalos;i++){
		resHalos[particle_count] = inCHalo->getnrinHalo(i);
		particle_count++;
	}

	for (int i = 0; i<nrHalos;i++){
		CArray* tmpHaloArray = Halos[i]->Halo2Array();
		for (int j = 0;j < tmpHaloArray->len();j++){
			resHalos[particle_count] = tmpHaloArray->get(j);
			particle_count++;
		}
	}


	for (int i = 0; i<innrHalos;i++){
		CArray* tmpHaloArray = inCHalo->getHalo(i)->Halo2Array();
		for (int j = 0;j < tmpHaloArray->len();j++){
			resHalos[particle_count] = tmpHaloArray->get(j);
			particle_count++;
		}
	}




	//for (int i = 0;i<)

	return CHalos(resHalos);
}



CHalos CHalos::operator=(CHalos* inCHalo){

}




//Get data from my own type of input file
void CHalos::get_Data(string filename){
	vector<string> strData;
	//vector<double*> array;
	ifstream file(filename.c_str());
	string line;
	int nr = 1;
	//vector<>

	vector<double> tmpData;

	if (file.is_open()){
		getline(file,line);
		split(strData, line, is_any_of("\t "));

		data_size = strData.size();
		//vector<double> tmpData (data_size);

		for (int i = 0; i < data_size; i++){
			tmpData.push_back(atof(strData[i].c_str()));
		}

		//Particles.push_back(new CParticle);
		//Particles[0]->Set_Data(tmpData);

		while (file.good()){
			getline(file,line);
			split(strData, line, is_any_of(" "));

			for (int i = 0; i < data_size; i++){
				tmpData.push_back(atof(strData[i].c_str()));
			}



			//Particles.push_back(new CParticle); //Memory leak?
			//Particles[nr]->Set_Data(tmpData);
			//Particles[nr]->print_Particle();


			nr++;
		}
		file.close();
	}

	else cout << "Unable to open file" << endl;
	nrinHalo.push_back(nr);
	nrParticles = nr;

	double tmpArray [nr*data_size];

	for (int i = 0;i<nr*data_size;i++){
		tmpArray[i] = tmpData[i];
	}

	//CArray test =	 CArray(nr*data_size,tmpArray);
	//test.print();

	//CHalo test2 = CHalo (&test);
	//test2.print();
	Halos.push_back(new CHalo (new CArray (nr*data_size,tmpArray)));
	//Halos[0]->print();
}


CParticles* CHalos::getCParticles(int element){
	return Halos[element]->getCParticles();
}


void CHalos::print(){
	for (int i = 0;i<Halos.size();i++){
		Halos[i]->print();
	}
}


//Master process
void CHalos::master(){
	CMPI MPI;

	int count = 0;
	int processor;
	int size = MPI.getSize();
	//CParticles finalHalos;
	CHalos FinalHalos;
	MPI_Request Req [size-1];
	vector<CArray*> Array (size-1);
	CParticle tmpParticle;

	
	//Initialize, sending one halo to each processor
	for (int p = 1; p < size; p++){
		cout << "Initializing for processor nr: " << p << endl;

		Array[p-1] = Halos[count]->Halo2Array();//Halo2Array(Halos[count]);
		//Array[p-1]->print();
		MPI.End(p,0);
		Array[p-1]->send(p);
		Array[p-1]->recieve(p,&Req[p-1]);
		count++;
	}


	cout << "-------------------------------------------------" << endl;
	cout << "Finished distributing halos to each processor" << endl;
	cout << "-------------------------------------------------" << endl;

	//Send halo to processor as soon as a processor finishes
	while (count < nrHalos) {
		cout << "Calculating for halo nr: " << count + 1 << endl;
		processor = MPI.listener(Req);
		
		FinalHalos.addHalos(Array[processor-1]);
		
		Array[processor-1] =  Halos[count]->Halo2Array();
		MPI.End(processor,0);
		Array[processor-1]->front(Array[processor-1]->len()/tmpParticle.getParticleSize());
		Array[processor-1]->front(1);
		Array[processor-1]->send(processor);
		Array[processor-1]->recieve(processor,&Req[processor-1]);
		count++;
		
	}

	MPI.WaitAll(Req);

	for (int p = 1;p < size;p++){
		MPI.End(p,1);
	}
}



//slave processor
void CHalos::slave(){

	CArray HalosArray;
	CMPI MPI;
	int flag;
	int rank = MPI.getRank();

	CParticle tmpParticle;


	while (true) {
		if (MPI.ifEnd() == 1) break;
		HalosArray.recieve_slave();
		
		//HalosArray.front(HalosArray.len()/tmpParticle.getParticleSize());
		//HalosArray.front(1);

		//HalosArray.print();

		//cout << "Slave " << rank << " recieved halo" << endl;
		CHalos SlaveHalos (&HalosArray);
		
		//SlaveHalos.print();
		//slaveParticles.print_Particles();
		//slaveParticles.DoSomething();

		SlaveHalos.Halos2Array()->send_slave();
		//cout << "Slave " << rank << " finished with halo" << endl;
	}
}

