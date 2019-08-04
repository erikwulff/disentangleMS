#pragma once


#include "TheoreticalIsotopeVariants.h"
#include "SeriePredictor.h"

class SpectrumGenerator{
	SeriePredictor seriePredictor;
public:
	Spectrum* generate(Spectrum* settings);
	Spectrum* noise(Spectrum* original);
	Spectrum* MS1();
	Spectrum* MSn(Spectrum* settings);
	Spectrum* scaleRandomly(Spectrum* beforeScaling);

};