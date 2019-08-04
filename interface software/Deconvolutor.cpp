
#include "StdAfx.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Deconvolutor.h"
#include "Parameters.h"
#include "SeriePredictor.h"
#include "Timer.h"
#include "Spectrum.h"
#include "PeakSet.h"
#include "Variants.h"
#include "IsotopeVariants.h"
#include "Peak.h"
using namespace std;

Deconvolutor::Deconvolutor(){
	seriePredictor = new SeriePredictor();
	deconvolutionLimit = Parameters::getInt("deconvolution limit", 50);
}

void Deconvolutor::deconvolute(Spectrum* spectrum){
	if(spectrum == NULL)
		return;
	Timer time;
	deisotope(spectrum);
	dechargeDeisotopedSeries(spectrum);
	decharge(spectrum);
	cout << " Spectrum is MS" << (spectrum->scanMode + 1) << ". Deconvoluted in " << time.ms() << " ms." << endl;
}

void Deconvolutor::deisotope(Spectrum* spectrum){
	Timer time;
	PeakSet s;
	for(int i = 0; i < spectrum->size(); i++)
		if(spectrum->getPeak(i)->getIntensity() > 10)
			s.add(spectrum->getPeak(i));
	int initialSize = s.size();
	while( s.isNotEmpty() && time.ms() < deconvolutionLimit ){
		Peak* peak = s.getFirstPeak(BY_INTENSITY);
		deisotope(peak, &s, spectrum);

		if( peak->getIsotopeSerie() != NULL )
			for( int i = 0; i < peak->getIsotopeSerie()->size(); i++ )
				s.remove( peak->getIsotopeSerie()->getPeak(i) );
	}
	cout << "Deconvoluted " << (initialSize - s.size()) << "peaks.\t" << (spectrum->size() - initialSize) << " left out of Deconvolution.\t" << s.size() << " peaks left undeconvoluted.\t";
}

// Determines set of peaks in <peak>s parent spectrum that forms the best isotope serie containing <peak>.
// Sets the isotopologue for each of these peaks to that set.
void Deconvolutor::deisotope(Peak* peak, PeakSet* s, Spectrum* spectrum){
	double bestScore = 10000000000000000;
	PeakSet* bestTheoreticalIsotopeSerie = NULL;
	PeakSet* bestMatch = NULL;

	int maxCharge = 6;
	if( spectrum->getParentPeak() != NULL )
		maxCharge = spectrum->getParentPeak()->getCharge();

	for( int q = 1; q <= maxCharge; q++ )
		for( int i = 0; i <= 4; i++ ){
			Spectrum* theoreticalIsotopeSerie = seriePredictor->predict(peak, q, i);
			if( theoreticalIsotopeSerie == NULL || theoreticalIsotopeSerie->size() == 0 )
				continue;
			PeakSet* currentMatch = s->findPeaksMatching(theoreticalIsotopeSerie);

			double currentScore = 0;
			for( int n = 0; n < theoreticalIsotopeSerie->size(); n++ ){
				Peak* theoreticalPeak = theoreticalIsotopeSerie->getPeak(n);
				Peak* matchingPeak = currentMatch->findPeakMatching(theoreticalPeak);
				currentScore += theoreticalPeak->compareTo(matchingPeak);
			}
			currentScore /= sqrt( (double)theoreticalIsotopeSerie->size() );

			if( currentScore < bestScore ){
				bestScore					= currentScore;
				delete bestTheoreticalIsotopeSerie;
				bestTheoreticalIsotopeSerie = theoreticalIsotopeSerie;
				delete bestMatch;
				bestMatch					= currentMatch;
			} else {
				delete theoreticalIsotopeSerie;
				delete currentMatch;
			}
		}

	// join everything in <bestMatch> into one isotope serie
	if( bestMatch != NULL && bestMatch->size() > 1 ){
		Variants* v = spectrum->newVariants();
		for(int n = 0; n < bestMatch->size(); n++){
			Peak* currentPeak = bestMatch->getPeak(n);
			Peak* correspondingTheoreticalPeak = bestTheoreticalIsotopeSerie->findPeakMatching(currentPeak);
			if(correspondingTheoreticalPeak != NULL){
				currentPeak->setCharge( correspondingTheoreticalPeak->getCharge() );
				currentPeak->setIsotopeNumber( correspondingTheoreticalPeak->getIsotopeNumber() );
			}
		}
		v->add( bestMatch );
		delete bestMatch;
	}
}

void Deconvolutor::dechargeDeisotopedSeries(Spectrum* spectrum){
	for(int i = 0; i < spectrum->getNumberOfVariants() - 1; i++){
		Variants* variants1 = spectrum->getVariants(i);
		double mass1 = variants1->getDeisotopedDechargedMass();
		for(int j = i+1; j < spectrum->getNumberOfVariants(); j++){
			Variants* variants2 = spectrum->getVariants(j);
			double mass2 = variants2->getDeisotopedDechargedMass();
			if( abs(mass1 - mass2) < 0.07 ){
				variants1->add(variants2);
				j--;
			}
		}
	}
}



void Deconvolutor::decharge(Spectrum* spectrum){
	int maxCharge = 5;
	if(spectrum->scanMode != 0)
		maxCharge = spectrum->getParentPeak()->getCharge();

	if(maxCharge == 1)
		return;

	PeakSet s;
	for(int i = 0; i < spectrum->size(); i++)
		if(spectrum->getPeak(i)->getVariants() == NULL && spectrum->getPeak(i)->getIntensity() > 10)
			s.add(spectrum->getPeak(i));
	Timer time;
	while( s.isNotEmpty() && time.ms() < deconvolutionLimit  ){
		Peak* peak1 = s.getFirstPeak(BY_INTENSITY);
		if(peak1->getVariants() == NULL){
			Peak* bestHits[5] = { NULL, NULL, NULL, NULL, NULL };
			double bestScore = 0;
			int bestCount = 0;

			for(int q1 = 1; q1 <= maxCharge; q1++){
				double mass1 = peak1->getMZ() * q1 - q1;
				Peak* hits[5] = { NULL, NULL, NULL, NULL, NULL };
				double score = 0;
				int count = 0;

				for(int q = 1; q <= maxCharge; q++){
					hits[q-1] = s.getLargestPeakInRadius((mass1 + q) / q, Parameters::accuracy);
					if( hits[q-1] != NULL ){
						score += log( max(hits[q-1]->getIntensity(), 1) ) / log(10.0);
						if( hits[q-1]->getIntensity() > 500 )
							count++;
					}
				}

				if(score > bestScore){
					bestHits[0] = hits[0];
					bestHits[1] = hits[1];
					bestHits[2] = hits[2];
					bestHits[3] = hits[3];
					bestHits[4] = hits[4];
					bestScore = score;
					bestCount = count;
				}
			}
			if( bestCount >= 2 && bestScore >= 6.0 ){
				Variants* v = spectrum->newVariants();
				for(int n = 0; n < 5; n++){
					Peak* currentPeak = bestHits[n];
					if(currentPeak != NULL){
						currentPeak->setCharge( n+1 );
						currentPeak->setIsotopeNumber( 0 );
						v->add(currentPeak);
						s.remove(currentPeak);
					}
				}
			} else
				s.remove(peak1);
		}
	}
}