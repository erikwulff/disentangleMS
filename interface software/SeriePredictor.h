#pragma once

#include "TheoreticalIsotopeVariants.h"
#include "IsolationPredictor.h"

#define NUMBER_OF_CHARGES 7
#define NUMBER_OF_ISOTOPES 8

class SeriePredictor{
	IsolationPredictor isolationPredictor;
	// RelativeIntensities[i][x] is relative intensity of isotopologue x of a molecule with
	// mass of about i * 50. Relative intensities accross all [x] add to 1 (within rounding error).
	double relativeIntensities[NUMBER_OF_CHARGES * 2000 / 50][NUMBER_OF_ISOTOPES];

public:
	SeriePredictor();

	// Predicts an isotope envelope of charge <charge> in which <peak> is the <isotopeNumber>'th peak.
	// Returns a spectrum containing these isotopes.
	// If peak is part of an MS1, assumes shape of of isotope envelope is peptide-like.
	// If peak is part of an MSn (n > 1), uses shape of isolated peaks from MS(n-1).
	// This requires that the MS(n-1) has been deconvoluted.
	// If any parameters don't make sense, returns NULL.
	// Caller is responsible for deleting returned spectrum.
	Spectrum* predict(Peak* peak, int charge, int isotopeNumber);

	// Predicts an isotope envelope of charge <charge> and deisotopes mass <baseMass>.
	// Returns a spectrum containing these isotopes.
	// If peak is part of an MS1, assumes shape of of isotope envelope is peptide-like.
	// If peak is part of an MSn (n > 1), uses shape of isolated peaks from MS(n-1).
	// This requires that the MS(n-1) has been deconvoluted.
	// If any parameters don't make sense, returns NULL.
	// Caller is responsible for deleting returned spectrum.
	Spectrum* predict(double baseMass, int charge, Spectrum* spectrum);

private:
	// Returns a spectrum containing the typical isotope enelope for a peptide
	// with deisotoped mass <baseMass> and charge <charge>.
	// If parameters are out of range, returns NULL.
	// Caller is responsible for deleting returned spectrum.
	Spectrum* MS1(double mass, int charge);

	// Returns a spectrum containing an isotope envelope with
	// deisotoped mass <baseMass> and charge <charge>.
	// Shape of envelope is based on isolation of parent peaks to make <spectrum>.
	// Parent spectrum must therefore already be deconvoluted.
	// If parameters are out of range, returns NULL.
	// Caller is responsible for deleting returned spectrum.
	Spectrum* MSn(double mass, int charge, Spectrum* spectrum);

	// Returns a spectrum containing an isotope envelope with
	// deisotoped mass <baseMass> and charge <charge>.
	// Shape of envelope is based on the assumption that the spectrum results
	// from fragmentation of peaks in <parentIsotopologues>.
	// If parameters are out of range, returns NULL.
	// The returned spectrum has the potential to be empty.
	// Caller is responsible for deleting returned spectrum.
	Spectrum* MSn(double mass, int charge, IsotopeVariants* isolatedParents);

	double binomial(double probability, int n, int k);
	double binomial_coefficient(int n, int k);
};