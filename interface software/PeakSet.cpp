
#include "StdAfx.h"
#include <algorithm>
#include "PeakSet.h"
#include "Parameters.h"

struct mzComparator{
	bool operator()( double mz, Peak* peak ) const{ return mz < peak->getMZ(); }
	bool operator()( Peak* peak, double mz ) const{ return peak->getMZ() < mz; }
	bool operator()( Peak* peak1, Peak* peak2 ) const{ return peak1->getMZ() < peak2->getMZ(); }
};
struct reverseMZComparator{
	bool operator()( double mz, Peak* peak ) const{ return mz > peak->getMZ(); }
	bool operator()( Peak* peak, double mz ) const{ return peak->getMZ() > mz; }
	bool operator()( Peak* peak1, Peak* peak2 ) const{ return peak1->getMZ() > peak2->getMZ(); }
};
struct intensityComparator{
	bool operator()( double intensity, Peak* peak ) const{ return intensity > peak->getIntensity(); }
	bool operator()( Peak* peak, double intensity ) const{ return peak->getIntensity() > intensity; }
	bool operator()( Peak* peak1, Peak* peak2 ) const{ return peak1->getIntensity() > peak2->getIntensity(); }
};
struct reverseIntensityComparator{
	bool operator()( double intensity, Peak* peak ) const{ return intensity < peak->getIntensity(); }
	bool operator()( Peak* peak, double intensity ) const{ return peak->getIntensity() < intensity; }
	bool operator()( Peak* peak1, Peak* peak2 ) const{ return peak1->getIntensity() < peak2->getIntensity(); }
};

// Constructs an empty set.
PeakSet::PeakSet(){
	totalIntensity = 0.0;
}

// Constructs a set containing the same peaks as <templateSet>.
PeakSet::PeakSet( PeakSet* templateSet ){
	totalIntensity = templateSet->totalIntensity;
	peaks[BY_MZ] = vector<Peak*>( templateSet->peaks[BY_MZ] );
	peaks[BY_INTENSITY] = vector<Peak*>( templateSet->peaks[BY_INTENSITY] );
}

// Returns true if <peak> is in the set and false otherwise.
// If  <peak> is NULL, returns false.
bool PeakSet::contains( Peak* peak ){
	if( peak != NULL && binary_search( peaks[BY_MZ].begin(), peaks[BY_MZ].end(), peak, mzComparator() ) )
		return true;
	return false;
}

// Adds <peak> to the set. If peak is NULL or has an mz already in the set, does nothing.
bool PeakSet::add( Peak* peak ){
	if( peak == NULL || binary_search( peaks[BY_MZ].begin(), peaks[BY_MZ].end(), peak, mzComparator() ) )
		return false;
	vector<Peak*>::iterator itMZ;
	itMZ = lower_bound( peaks[BY_MZ].begin(), peaks[BY_MZ].end(), peak, mzComparator() );
	peaks[BY_MZ].insert( itMZ, peak );
	vector<Peak*>::iterator itIntensity;
	itIntensity = lower_bound( peaks[BY_INTENSITY].begin(), peaks[BY_INTENSITY].end(), peak, intensityComparator() );
	peaks[BY_INTENSITY].insert( itIntensity, peak );
	totalIntensity += peak->getIntensity();
	return true;
}

// Adds all peaks in <set> to the set. If <set> is NULL, does nothing.
void PeakSet::add( PeakSet* set ){
	if( set == NULL )
		return;
	for( int i = 0; i < set->size(); i++ )
		add( set->getPeak(i) );
}

// Removes <peak> from the set. If <peak> is not a peak in the set, does nothing.
void PeakSet::remove( Peak* peak ){
	if(peak == NULL)
		return;
	vector<Peak*>::iterator it;
	it = lower_bound( peaks[BY_MZ].begin(), peaks[BY_MZ].end(), peak, mzComparator() );
	if( it != peaks[BY_MZ].end() && *it == peak ){
		peaks[BY_MZ].erase(it);
		it = lower_bound( peaks[BY_INTENSITY].begin(), peaks[BY_INTENSITY].end(), peak, intensityComparator() );;
		peaks[BY_INTENSITY].erase(it);
		totalIntensity -= peak->getIntensity();
	}
}

// Returns the number of peaks in the set.
int PeakSet::size(){
	return peaks[BY_MZ].size();
}

// Returns whether the set is empty.
bool PeakSet::isEmpty(){
	return size() == 0;
}

// Returns whether the set contains any peak.
bool PeakSet::isNotEmpty(){
	return size() != 0;
}

// Returns the index of <peak> according to <order>.
// E.g., if peak is the set's third most intense peak and order is <BY_INTENSITY>, returns 2.
// E.g., if peak has the fifth lowest mz in the set and order is <BY_MZ> or unspecified, returns 4.
// If <peak> is not a peak in the spectrum, returns -1.
int PeakSet::getIndexOf( Peak* peak, orders order ){
	if( peak == NULL )
		return -1;
	vector<Peak*>::iterator low;
	if(order == BY_INTENSITY)
		low = lower_bound( peaks[order].begin(), peaks[order].end(), peak, intensityComparator() );
	else
		low = lower_bound( peaks[order].begin(), peaks[order].end(), peak, mzComparator() );
	if( low == peaks[order].end() )
		return -1;
	return low - peaks[order].begin();
}

// If <order> is ORDER_BY_INTENSITY, returns ptr to <n>+1'th most intense peak.
// Otherwise, returns ptr to the <n>+1'th peak by mz.
// If <n> is out of range, returns NULL.
Peak* PeakSet::getPeak( int n, orders order ){
	if( n < 0 || n >= size() )
		return NULL;
	return peaks[order][n];
}

// If <order> is ORDER_BY_INTENSITY, returns ptr to most intense peak.
// Otherwise, returns ptr to peak with lowest mz.
// If spectrum is empty, returns NULL.
Peak* PeakSet::getFirstPeak( orders order ){
	return getPeak( 0, order );
}

// If <order> is ORDER_BY_INTENSITY, returns ptr to least intense peak.
// Otherwise, returns ptr to peak with highest mz.
// If spectrum is empty, returns NULL.
Peak* PeakSet::getLastPeak( orders order ){
	return getPeak( size() - 1, order );
}

// If <peak> is NULL, sets it to the first peak by <order>.
// If <peak> is to the last peak by <order>, returns false.
// If <peak> is to n'th peak by <order>, sets it to n+1'th peak and returns true.
// For ptrs not to NULL or peaks in the spectrum, behavior is undefined.
bool PeakSet::getNext( Peak* &peak, orders order ){
	if( isEmpty() || peak == getLastPeak(order) )
		return false;
	if( peak == NULL ){
		peak = getFirstPeak( order );
		return true;
	}
	Peak* peak2 = NULL;
	if(order == BY_INTENSITY)
		peak2 = getPeakSmallerThan( peak->getIntensity() );
	else
		peak2 = getPeakRightOf( peak->getMZ() );
	if( peak2 == NULL )
		return false;
	peak = peak2;
	return true;
}

// If <peak> is NULL, sets it to the last peak by <order>.
// If <peak> is to the first peak by <order>, returns false.
// If <peak> is to n'th peak by <order>, sets it to n-1'th peak and returns true.
// For ptrs not to NULL or peaks in the spectrum, behavior is undefined.
bool PeakSet::getPrevious( Peak* &peak, orders order ){
	if( isEmpty() )
		return false;
	if( peak == NULL ){
		peak = getLastPeak( order );
		return true;
	}
	Peak* peak2 = NULL;
	if(order == BY_INTENSITY)
		peak2 = getPeakLargerThan( peak->getIntensity() );
	else
		peak2 = getPeakLeftOf( peak->getMZ() );
	if( peak2 == NULL )
		return false;
	peak = peak2;
	return true;
}

// Returns index of the peak with the highest mz that is below <mz>.
// If there is no such peak, returns -1;
int PeakSet::getIndexLeftOf( double mz ){
	if( isEmpty() || getFirstPeak()->getMZ() >= mz )
		return -1;
	vector<Peak*>::reverse_iterator low = upper_bound( peaks[BY_MZ].rbegin(), peaks[BY_MZ].rend(), mz, reverseMZComparator() );
	return (peaks[BY_MZ].rend() - low) - 1;
}

// Returns index by mz of the peak with the lowest mz that is above <mz>.
// If there is no such peak, returns -1.
int PeakSet::getIndexRightOf( double mz ){
	if( isEmpty() || getLastPeak()->getMZ() <= mz )
		return -1;
	vector<Peak*>::iterator low = upper_bound( peaks[BY_MZ].begin(), peaks[BY_MZ].end(), mz, mzComparator() );
	return low - peaks[BY_MZ].begin();
}

// Returns index by intensity of peak with the lowest intensity that is above <intensity>.
// If there is no such peak, returns -1.
int PeakSet::getIndexLargerThan( double intensity ){
	if( isEmpty() || getFirstPeak(BY_INTENSITY)->getIntensity() <= intensity )
		return -1;
	vector<Peak*>::reverse_iterator low = upper_bound( peaks[BY_INTENSITY].rbegin(), peaks[BY_INTENSITY].rend(), intensity, reverseIntensityComparator() );
	return (peaks[BY_INTENSITY].rend() - low) - 1;
}

// Returns index by intensity of peak with the highest intensity that is below <intensity>.
// If there is no such peak, returns -1.
int PeakSet::getIndexSmallerThan( double intensity ){
	if( isEmpty() || getLastPeak(BY_INTENSITY)->getIntensity() >= intensity )
		return -1;
	vector<Peak*>::iterator low = upper_bound( peaks[BY_INTENSITY].begin(), peaks[BY_INTENSITY].end(), intensity, intensityComparator() );
	return low - peaks[BY_INTENSITY].begin();
}

// Returns index by mz of peak with mz nearest <mz>.
// If there are no peaks, returns -1.
int PeakSet::getIndexNearest( double mz ){
	if( isEmpty() )
		return - 1;
	if( mz >= getLastPeak()->getMZ() )
		return size() - 1;
	if( mz <= getFirstPeak()->getMZ() )
		return 0;
	int indexOnRight = getIndexRightOf( mz );
	int indexOnLeftOrAt = indexOnRight - 1;
	if( getPeak(indexOnRight)->getMZ() - mz < mz - getPeak(indexOnLeftOrAt)->getMZ() )
		return indexOnRight;
	else
		return indexOnLeftOrAt;
}

// Returns index by MZ of most intense peak with mz in range (<lowerMZBound>, <upperMZBound>).
// If there are no peaks in the range, returns -1.
int PeakSet::getLargestIndexInRange( double lowerMZBound, double upperMZBound ){
	int greatestIntensityIndex = -1;
	double greatestIntensity = 0;
	int index = getIndexRightOf( lowerMZBound );
	while( getPeak(index) != NULL && getPeak(index)->getMZ() < upperMZBound ){
		double intensity = getPeak(index)->getIntensity();
		if(intensity > greatestIntensity){
			greatestIntensity = intensity;
			greatestIntensityIndex = index;
		}
		index++;
	}
	return greatestIntensityIndex;
}

// Returns index by mz of most intense peak with mz less than <radius> from <centerMZ>.
// If there are no such peaks, returns -1.
int PeakSet::getLargestIndexInRadius( double centerMZ, double radius ){
	return getLargestIndexInRange( centerMZ - radius, centerMZ + radius);
}

// Returns ptr to the peak with the highest mz that is below <mz>.
// If there is no such peak, returns NULL.
Peak* PeakSet::getPeakLeftOf( double mz ){
	return getPeak( getIndexLeftOf(mz) );
}

// Returns ptr to the peak with the lowest mz that is above <mz>.
// If there is no such peak, returns NULL.
Peak* PeakSet::getPeakRightOf( double mz ){
	return getPeak( getIndexRightOf(mz) );
}

// Returns ptr to the peak with the lowest intensity that is above <intensity>.
// If there is no such peak, returns NULL.
Peak* PeakSet::getPeakLargerThan( double intensity ){
	return getPeak( getIndexLargerThan(intensity), BY_INTENSITY);
}

// Returns ptr to the peak with the highest intensity that is below <intensity>.
// If there is no such peak, returns NULL.
Peak* PeakSet::getPeakSmallerThan( double intensity ){
	return getPeak( getIndexSmallerThan(intensity), BY_INTENSITY );
}

// Returns ptr to the peak with mz nearest <mz>.
// If there are no peaks, returns NULL.
Peak* PeakSet::getPeakNearest( double mz ){
	return getPeak( getIndexNearest(mz) );
}

// Returns ptr to most intense peak with mz in range (<lowerMZBound>, <upperMZBound>).
// If there are no peaks in the range, returns NULL.
Peak* PeakSet::getLargestPeakInRange( double lowerMZBound, double upperMZBound ){
	return getPeak( getLargestIndexInRange(lowerMZBound, upperMZBound) );
}

// Returns ptr to most intense peak with mz less than <radius> from <centerMZ>.
// If there are no such peaks, returns NULL.
Peak* PeakSet::getLargestPeakInRadius( double centerMZ, double radius ){
	return getLargestPeakInRange( centerMZ - radius, centerMZ + radius);
}

// Returns set of all peaks with mz's in (<lowerMZBound>, <upperMZBound>).
// Caller is responsible for deleting set.
PeakSet* PeakSet::getPeaksInRange( double lowerMZBound, double upperMZBound ){
	PeakSet* hits = new PeakSet();
	int index = getIndexRightOf( lowerMZBound );
	while( getPeak(index) != NULL && getPeak(index)->getMZ() < upperMZBound ){
		hits->add( getPeak(index) );
		index++;
	}
	return hits;
}

// Returns set of all peaks with mz's less than <radius> from <centerMZ>.
// Caller is responsible for deleting set.
PeakSet* PeakSet::getPeaksInRadius( double centerMZ, double radius ){
	return getPeaksInRange( centerMZ - radius, centerMZ + radius );
}

// Returns the sum of intensities for all peaks in the spectrum.
double PeakSet::getTotalIntensity(){
	return totalIntensity;
}

// Returns number indicating how well the two spectra match each other in both mzs and intensities.
// Exact match will return 0.
double PeakSet::compareTo(PeakSet* otherSet){
	double score = 0;
	for( int i = 0; i < otherSet->size(); i++ ){
		Peak* peakInOtherSet = otherSet->getPeak(i);
		Peak* match = findPeakMatching(peakInOtherSet);
		score += peakInOtherSet->compareTo(match);
	}
	for( int i = 0; i < size(); i++ ){
		Peak* peakInThisSet = getPeak(i);
		Peak* match = otherSet->findPeakMatching(peakInThisSet);
		score += peakInThisSet->compareTo(match);
	}
	return score / 2;
}

// Returns ptr to peak best matching the mz and intensity of <peak>.
// If set is empty, returns NULL.
Peak* PeakSet::findPeakMatching( Peak* peak ){
	if( peak == NULL )
		return NULL;
	Peak* bestMatch = NULL;
	double bestScore = 100000000;
	double lowerBound = peak->getMZ() - 2 * Parameters::accuracy;
	double upperBound = peak->getMZ() + 2 * Parameters::accuracy;
	int index = getIndexLeftOf( upperBound );
	while( index >= 0 && getPeak(index)->getMZ() > lowerBound ){
		Peak* match = getPeak(index);
		double score = match->compareTo(peak);
		if(score < bestScore){
			bestMatch = match;
			bestScore = score;
		}
		index--;
	}
	if(peak->compareTo(NULL) < bestScore)
		bestMatch = NULL;
	return bestMatch;
}

// Returns set of peaks that can be matched to those in <peakSet> based on mz and intensity.
PeakSet* PeakSet::findPeaksMatching( PeakSet* otherSet ){
	PeakSet* matches = new PeakSet();
	for(int n = 0; n < otherSet->size(); n++)
		matches->add( findPeakMatching( otherSet->getPeak(n) ) );
	return matches;
}

// Returns the set that best matches this one based on mz and intensity.
PeakSet* PeakSet::findMatchingPeakSet(vector<PeakSet*>* peakSets){
	if(peakSets == NULL)
		return NULL;

	PeakSet* bestMatch = NULL;
	PeakSet* match;
	double bestScore = 10000000000000000000;
	for( int i = 0; i < peakSets->size(); i++ )
		if( peakSets->at(i) != NULL ){
			match = findPeaksMatching( peakSets->at(i) );
			double score = match->compareTo( peakSets->at(i) );
			if(score < bestScore){
				bestMatch = peakSets->at(i);
				bestScore = score;
			}
		}
	return bestMatch;
}


