#pragma once

#include "CVector.h"
#include "Constants.h"


/*class LinkedList{
 public:
	LinkedList(){
		next = prev = NULL;
	}
	
	CParticle *next,*prev; 

	void RemoveList{
		if (next)
			next->prev = prev;

		if (prev)
			prev->next = next;
	}
	};*/



class CParticle{
 public:

	CParticle();
	
	//Create a particle from an array on the following form:
	//[HaloID,Mass,Charge,Px,Py,Pz,Vx,Vy,Vz,Ax,Ay,Az]
	CParticle(double* inArray);

	~CParticle();

	//Attatch a particle to the particle list
	void attachParticle(CParticle* p){
		//CParticle* oldNext = nextGrid;
		p->nextGrid = nextGrid;
		if (nextGrid != NULL)
			nextGrid->prevGrid = p;
		nextGrid = p;
		p->prevGrid = this;
	};

	
	//Gives the next and previous particle in a linked list
	CParticle* next,*prev,*nextGrid,*prevGrid;
	//Removes the particle from a linked list
	void RemoveFromList();
	void RemoveFromListGrid();
	
	//LinkedList ListIterator, GridIterator;

	//Set the different values a particle has
	void setPhi(double element);
	double getPhi();

	void setMass(double in_M);
	//void setCharge(double in_C);
	void setPosition(double Px, double Py, double Pz);
	void setVelocity(double Vx, double Vy, double Vz);
	void setAcceleration(double Ax, double Ay, double Az);

	//Set all data, given by a vector
	void setData(vector<double> data);

	void setP(CVector inP);
	void setV(CVector inV);


	//Get the different values a particle has
	//int getID();
	int getHalo();
	double getMass();
	CVector& getP();
	CVector& getV();
	CVector& getA();
	int getParticleSize();


	//Get/set flag value.y
	//Used in FoF methods to tell if this particle has been tested yet or not
	void setFlag(int inFlag);
	int getFlag();


	//Convert a particle to an array on the form:
	//[Phi,Mass,Charge,Px,Py,Pz,Vx,Vy,Vz,Ax,Ay,Az]
	double* Particle2Array();

	//Calculate the kinetic energy of a particle
	double Ekin();
	//Calculate the momentumof a particle
	CVector Momentum();
	//Move the particle one timestep
	void Move(double dt);
	//CalculatePhaseSpaceDistance between 2 particles
	double PhaseSpaceDistance(CParticle* p2, CVector* inSigmaP, CVector* inSigmaV);
	//CalculatePhaseSpaceDistance between a particle and a Halo
	//double PhaseSpaceDistanceHalo(CHalo* inHalo);

	

	//Print out all the information contained in one particle
	void print();

	void calculatePhi(CVector& MeanP);


	//Test if a particle has been assigned a halo
	int gotHalo();
	//Decreases the Phi by one. Used by my inefficient FoF code
	void decreaseHalo();


	//put this private, and get the code to work in some way.
	

 protected:
	//static int ID_Generator;
	CVector P,V,A;
	int ParticleSize,flag;
	double Mass,Phi;

	//void Generate_ID();
};


