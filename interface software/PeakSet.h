#pragma once

#include <vector>
#include "Peak.h"

using namespace std;

// Set of peaks. Guaranteed to not contain duplicates, NULLs or multiple peaks with identical mz values.
class PeakSet{
protected:
	double totalIntensity;
	vector<Peak*> peaks[NUMBER_OF_ORDERING_SCHEMES];

public:
	// Constructs an empty set.
	PeakSet();

	// Constructs a set containing the same peaks as <templateSet>.
	PeakSet( PeakSet* templateSet );

	// Adds <peak> to the set. If peak is NULL or has an mz already in the set, does nothing.
	bool add( Peak* peak );

	// Returns true if <peak> is in the set and false otherwise.
	// If  <peak> is NULL, returns false.
	bool contains( Peak* peak );

	// Adds all peaks in <set> to the set. If <set> is NULL, does nothing.
	void add( PeakSet* set );

	// Removes <peak> from the set. If <peak> is not a peak in the set, does nothing.
	void remove( Peak* peak );

	// Returns the number of peaks in the set.
	int size();

	// Returns whether the set is empty.
	bool isEmpty();

	// Returns whether the set contains any peak.
	bool isNotEmpty();

	// Returns the index of <peak> according to <order>.
	// E.g., if peak is the set's third most intense peak and order is <BY_INTENSITY>, returns 2.
	// E.g., if peak has the fifth lowest mz in the set and order is <BY_MZ> or unspecified, returns 4.
	// If <peak> is not a peak in the spectrum, returns -1.
	int getIndexOf( Peak* peak, orders order = BY_MZ );

	// If <order> is ORDER_BY_INTENSITY, returns ptr to <n>+1'th most intense peak.
	// Otherwise, returns ptr to the <n>+1'th peak by mz.
	// If <n> is out of range, returns NULL.
	Peak* getPeak( int peakNumber, orders order = BY_MZ );

	// If <order> is ORDER_BY_INTENSITY, returns ptr to most intense peak.
	// Otherwise, returns ptr to peak with lowest mz.
	// If spectrum is empty, returns NULL.
	Peak* getFirstPeak( orders order = BY_MZ );

	// If <order> is ORDER_BY_INTENSITY, returns ptr to least intense peak.
	// Otherwise, returns ptr to peak with highest mz.
	// If spectrum is empty, returns NULL.
	Peak* getLastPeak( orders order = BY_MZ );

	// If <peak> is NULL, sets it to the first peak by <order>.
	// If <peak> is to the last peak by <order>, returns false.
	// If <peak> is to n'th peak by <order>, sets it to n+1'th peak and returns true.
	// For ptrs not to NULL or peaks in the set, behavior is undefined.
	bool getNext( Peak* &peak, orders order = BY_MZ );

	// If <peak> is NULL, sets it to the last peak by <order>.
	// If <peak> is to the first peak by <order>, returns false.
	// If <peak> is to n'th peak by <order>, sets it to n-1'th peak and returns true.
	// For ptrs not to NULL or peaks in the set, behavior is undefined.
	bool getPrevious( Peak* &peak, orders order = BY_MZ );

	// Returns index by mz of the peak with the highest mz that is below <mz>.
	// If there is no such peak, returns -1.
	int getIndexLeftOf( double mz );

	// Returns index by mz of the peak with the lowest mz that is above <mz>.
	// If there is no such peak, returns -1.
	int getIndexRightOf( double mz );

	// Returns index by intensity of peak with the lowest intensity that is above <intensity>.
	// If there is no such peak, returns -1.
	int getIndexLargerThan( double intensity );

	// Returns index by intensity of peak with the highest intensity that is below <intensity>.
	// If there is no such peak, returns -1.
	int getIndexSmallerThan( double intensity );

	// Returns index by mz of peak with mz nearest <mz>.
	// If there are no peaks, returns -1.
	int getIndexNearest( double mz );

	// Returns index by mz of most intense peak with mz in range (<lowerMZBound>, <upperMZBound>).
	// If there are no peaks in the range, returns -1.
	int PeakSet::getLargestIndexInRange( double lowerMZBound, double upperMZBound );

	// Returns index by mz of most intense peak with mz less than <radius> from <centerMZ>.
	// If there are no such peaks, returns -1.
	int getLargestIndexInRadius( double centerMZ, double radius );

	// Returns ptr to the peak with the highest mz that is below <mz>.
	// If there is no such peak, returns NULL.
	Peak* getPeakLeftOf( double mz );

	// Returns ptr to the peak with the lowest mz that is above <mz>.
	// If there is no such peak, returns NULL.
	Peak* getPeakRightOf( double mz );
	
	// Returns ptr to the peak with the lowest intensity that is above <intensity>.
	// If there is no such peak, returns NULL.
	Peak* getPeakLargerThan( double intensity );

	// Returns ptr to the peak with the highest intensity that is below <intensity>.
	// If there is no such peak, returns NULL.
	Peak* getPeakSmallerThan( double intensity );

	// Returns ptr to the peak with mz nearest <mz>.
	// If there are no peaks, returns NULL.
	Peak* getPeakNearest( double mz );

	// Returns ptr to most intense peak with mz in range (<lowerMZBound>, <upperMZBound>).
	// If there are no peaks in the range, returns NULL.
	Peak* getLargestPeakInRange( double lowerMZBound, double upperMZBound );

	// Returns ptr to most intense peak with mz less than <radius> from <centerMZ>.
	// If there are no such peaks, returns NULL.
	Peak* getLargestPeakInRadius( double centerMZ, double radius );

	// Returns set of all peaks with mz's in (<lowerMZBound>, <upperMZBound>).
	// Caller is responsible for deleting set.
	PeakSet* getPeaksInRange( double lowerMZBound, double upperMZBound );

	// Returns set of all peaks with mz's less than <radius> from <centerMZ>.
	// Caller is responsible for deleting set.
	PeakSet* getPeaksInRadius( double centerMZ, double radius );

	// Returns the sum of intensities for all peaks in the set.
	double getTotalIntensity();

	// Returns number indicating how well the two spectra match each other in both mzs and intensities.
	// Exact match will return 0.
	double compareTo( PeakSet* peakSet );

	// Returns ptr to peak best matching the mz and intensity of <peak>.
	// If set is empty, returns NULL.
	Peak* findPeakMatching( Peak* peak );

	// Returns set of peaks that can be matched to those in <peakSet> based on mz and intensity.
	PeakSet* findPeaksMatching( PeakSet* otherSet );

	// Returns the set that best matches this one based on mz and intensity.
	PeakSet* findMatchingPeakSet( vector<PeakSet*>* peakSets );
};