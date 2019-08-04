
#include "StdAfx.h"
#include "SpectrumGenerator.h"
#include "Spectrum.h"
using namespace std;

Spectrum* SpectrumGenerator::generate(Spectrum* settings){
	Spectrum* spectrum = new Spectrum();
	int numberOfIsotopeSeries = 250;
	for( int i = 0; i < numberOfIsotopeSeries; i++ ){
		double mass = rand() % 10000 + (rand() % 1000) / 1000.0;
		for(int q = 1; q <= 5; q++){
			Spectrum* randomIsotopeSerie = noise( scaleRandomly( seriePredictor.predict(mass, q, settings) ) );
			for(int i = 0; i < randomIsotopeSerie->size(); i++)
				spectrum->addPeak( randomIsotopeSerie->getPeak(i)->getMZ(), randomIsotopeSerie->getPeak(i)->getIntensity());
			delete randomIsotopeSerie;
		}
	}
	return spectrum;
}

Spectrum* SpectrumGenerator::noise(Spectrum* beforeNoising){
	double intensityNoise = 1.0 + ( (rand() % 100) / 100.0 - 0.5 );
	double mzNoise = (rand() % 100) / 10000.0 - 0.005;
	Spectrum* afterNoising = new Spectrum();
	if(beforeNoising == NULL )
		return afterNoising;
	for( int i = 0; i < beforeNoising->size(); i++ ){
		Peak* peak = beforeNoising->getPeak(i);
		afterNoising->addPeak( peak->getMZ() + mzNoise, peak->getIntensity() * intensityNoise );
	}
	delete beforeNoising;
	return afterNoising;
}

Spectrum* SpectrumGenerator::scaleRandomly(Spectrum* beforeScaling){
	double scaleFactor = rand() % 100000;
	Spectrum* afterScaling = new Spectrum();
	if(beforeScaling == NULL )
		return afterScaling;
	for( int i = 0; i < beforeScaling->size(); i++ ){
		Peak* peak = beforeScaling->getPeak(i);
		afterScaling->addPeak( peak->getMZ(), peak->getIntensity() * scaleFactor );
	}
	delete beforeScaling;
	return afterScaling;
}


