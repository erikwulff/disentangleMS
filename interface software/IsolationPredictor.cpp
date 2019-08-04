
#include "StdAfx.h"
#include "Spectrum.h"
#include "PeakSet.h"
#include "Peak.h"
#include "IsolationPredictor.h"
using namespace std;

Spectrum* IsolationPredictor::predict(Peak* peak){
	Spectrum* isolationResult = new Spectrum();
	Spectrum* spectrum = peak->getSpectrum();
	double isolatedMZ = peak->getSpectrum()->parentMass;
	double isolationWidth = peak->getSpectrum()->isolationWidth;
	double lowerBound = isolatedMZ - isolationWidth * 3;
	double upperBound = isolatedMZ + isolationWidth * 3;
	vector<Variants*> oldVariants;
	vector<Variants*> newVariants;

	int index = spectrum->getIndexRightOf( lowerBound );
	while( index < spectrum->size() && spectrum->getPeak(index)->getMZ() < upperBound ){
		Peak* p = spectrum->getPeak(index);
		double retainment = isolationEfficiency(p->getMZ(), isolatedMZ, isolationWidth);
		if( retainment > 0 ){
			isolationResult->addPeak( p->getMZ(), p->getIntensity() * retainment );
			Peak* newPeak = isolationResult->getLastPeak();
			newPeak->setCharge(p->getCharge());
			newPeak->setIsotopeNumber(p->getIsotopeNumber());
			if( p->getVariants() != NULL ){
				int i = 0;
				while( i < oldVariants.size()){
					if( p->getVariants() == oldVariants.at(i) )
						break;
					i++;
				}
				if(i != p->getVariants()->size())
					newVariants.at(i)->add(newPeak);
				else{
					Variants* v = isolationResult->newVariants();
					v->add(newPeak);
					newVariants.push_back(v);
					oldVariants.push_back(p->getVariants());
				}	
			}
		}
		index++;
	}
	return isolationResult;
}

Spectrum* IsolationPredictor::predictSingleVariant(Peak* peak){
	if(peak->getVariants() == NULL){
		Spectrum* s = new Spectrum();
		s->addPeak(peak->getMZ(), peak->getIntensity());
		s->getLastPeak()->setCharge(peak->getCharge());
		s->getLastPeak()->setIsotopeNumber(peak->getIsotopeNumber());
		return s;
	}
	Spectrum* isolationResult = new Spectrum();
	Variants* spectrum = peak->getVariants();
	double isolatedMZ = peak->getSpectrum()->parentMass;
	double isolationWidth = peak->getSpectrum()->isolationWidth;
	double lowerBound = isolatedMZ - isolationWidth * 3;
	double upperBound = isolatedMZ + isolationWidth * 3;
	vector<Variants*> oldVariants;
	vector<Variants*> newVariants;

	int index = spectrum->getIndexRightOf( lowerBound );
	while( index < spectrum->size() && spectrum->getPeak(index)->getMZ() < upperBound ){
		cout << "a" << endl;
		Peak* p = spectrum->getPeak(index);
		double retainment = isolationEfficiency(p->getMZ(), isolatedMZ, isolationWidth);
		if( retainment > 0 ){
			isolationResult->addPeak( p->getMZ(), p->getIntensity() * retainment );
			Peak* newPeak = isolationResult->getLastPeak();
			newPeak->setCharge(p->getCharge());
			newPeak->setIsotopeNumber(p->getIsotopeNumber());
		}
		index++;
	}
	Variants* v = isolationResult->newVariants();
	for(int i = 0; i < isolationResult->size(); i++)
		v->add(isolationResult->getPeak(i));
	return isolationResult;
}

Spectrum* IsolationPredictor::predict(PeakSet* set, double isolatedMZ, double isolationWidth){
	Spectrum* isolationResult = new Spectrum();
	double lowerBound = isolatedMZ - isolationWidth * 3;
	double upperBound = isolatedMZ + isolationWidth * 3;
	vector<Variants*> oldVariants;
	vector<Variants*> newVariants;

	int index = set->getIndexRightOf( lowerBound );
	while( index < set->size() && set->getPeak(index)->getMZ() < upperBound ){
		Peak* p = set->getPeak(index);
		double retainment = isolationEfficiency(p->getMZ(), isolatedMZ, isolationWidth);
		if( retainment > 0 ){
			isolationResult->addPeak( p->getMZ(), p->getIntensity() * retainment );
			Peak* newPeak = isolationResult->getLastPeak();
			newPeak->setCharge(p->getCharge());
			newPeak->setIsotopeNumber(p->getIsotopeNumber());
			if( p->getVariants() != NULL ){
				bool flag = true;
				for( int i = 0; i < oldVariants.size(); i++ ){
					if( oldVariants.at(i) == p->getVariants() ){
						newVariants.at(i)->add(newPeak);
						flag = false;
						break;
					}
				}
				if(flag){
					Variants* v = isolationResult->newVariants();
					v->add(newPeak);
					newVariants.push_back( v );
					oldVariants.push_back( p->getVariants() );
				}
			}
		}
		index++;
	}
	return isolationResult;
}





double IsolationPredictor::isolationEfficiency(double mz, double isolatedMZ, double isolationWidth){
	if( abs(mz - isolatedMZ) <= isolationWidth / 0.7 )
		return 1;
	else
		return 0;
}