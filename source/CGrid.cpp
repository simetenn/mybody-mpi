#include "CGrid.h"
#include <iostream>

using namespace std;



CGrid::CGrid(){
	Width = 0;
}

CGrid::CGrid(CVector* inMin, CVector* inMax, int inWidth){
	initialize(inMin,inMax, inWidth);
}


CGrid::~CGrid(){
	//clear();
}

void CGrid::clear(){
	Grid.clear();
}

void CGrid::initialize(CVector* inMin, CVector* inMax, int inWidth){
	Width = inWidth;
	Min = *inMin;
	Max = *inMax;

	Delta = (Max-Min)/(double)Width;
	Grid.clear();
	Grid.resize(Width*Width*Width);
	/*for (int i = 0; i < Width*Width*Width; i++) {
		Grid[i] = NULL;
		}*/
}


void CGrid::Populate(CParticles* inParticles){
	for (int i = 0; i < inParticles->getNrParticles(); i++){
		addParticle(inParticles->get(i));
	}
}


CVector CGrid::getPosition(CParticle* inParticle){
	//cout << "in getPosition" << endl;
	//inParticle->getP().print();
	CVector pos = (inParticle->getP() - (Min))/Delta;
	//pos.Add(Width);
	//pos = CVector(pos.x()/Delta.x(),pos.y()/Delta.y(),pos.z()/Delta.z());
	//pos.print();
	//return (inParticle->getP() - (Min))/Delta;
	return CVector((int) (pos.x()) % Width,(int)( pos.y())% Width,(int) (pos.z())% Width);
}

void CGrid::addParticle(CParticle* inParticle){
	CVector tmpPosition = getPosition(inParticle);

	//CVector tmpPosition(1,1,1);
	//CParticles* p = (Grid[tmpPosition.x()][tmpPosition.y()][tmpPosition.z()]);
	//tmpPosition.print();
	//cout << p << endl;
	//p->print();//->addParticle(inParticle);

	int x = (int)tmpPosition.x() % Width;
	int y = (int)tmpPosition.y() % Width;
	int z = (int)tmpPosition.z() % Width;

	inParticle->setFlag(0);
	if (Grid[x + y*Width + z*Width*Width] == NULL){
		Grid[x + y*Width + z*Width*Width] = inParticle;
	}
	else{
		Grid[x + y*Width + z*Width*Width]->attachParticle(inParticle);
	}
	//cout << "there" << endl;
	//tmpPosition.print();
}



CParticle* CGrid::get(int x, int y, int z) {


	x = x % Width;
	y = y % Width;
	z = z % Width;


	return Grid[x + y*Width + z*Width*Width];
}

CParticle* CGrid::getPeriodic(int x, int y, int z) {

	x = x % Width;
	y = y % Width;
	z = z % Width;


	return Grid[x + y*Width + z*Width*Width];
}

/*void CGrid::print(){
	for (int i = 0; i < Width; i++){
		for (int j = 0;j < Width; j++){
			for (int k = 0;k < Width; k++){
				Grid[i][j][k].print();
			}
		}
	}
	}*/
