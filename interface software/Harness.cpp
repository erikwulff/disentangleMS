
#include "StdAfx.h"
#include <stdio.h>
#include "Peak.h"
#include "Spectrum.h"
#include <ctime>

using namespace std;
/*
int main(int argc, char* argv[], char* envp[]){
	Spectrum* s1 = new Spectrum();
	Spectrum* s2 = new Spectrum();
	Spectrum* s3 = new Spectrum();
	double mzs1[16] =			 { 110.3,  122.4,  241.1, 
								   250.0,  250.1,  601.0, 
								   610.7, 1229.9, 1292.9, 
								  1523.0, 1641.2, 1788.9, 
								  1799.0, 1900.0, 1911.2	};
	double intensities1[16] =	 {  1243234, 214124323,  21342, 
								     123432,    123433, 123423, 
								     214323,   4576657,   6575, 
								        123,      1234, 242134, 
								      12799,     23900,    562 };

//	214124323
//	  4576657
//	  1243234
//	   242134
//	   214323
//	   123433
//	   123432
//	   123423
//		23900
//	    21342
//		12799
//		 6575
//		 1234
//		  562
//		  123


	double mzs2[1] =			  {};
	double intensities2[1] =	  {};

	double composite[74] =
	{
		110.3214, 29832471.2,
		122.4327,    31841.1,
		142.3314,    9234.28,
		241.1565,   120914.8,
		532.1247,   321981.0,
		544.5721, 100000,			// 1, q+3
		544.9011,  95900,			// 2, q+3
		545.2399,  48600,			// 3, q+3
		545.5712,  19700,			// 4, q+3
		545.9021,	6700,			// 5, q+3
		546.2371,	5000,			// 6, q+3
		546.5788,	3300,			// 7, q+3
		546.9031,	2700,			// 8, q+3
		547.2399,	2500,			// 9, q+3
		872.2214, 21398123.8,
		952.3167,  93.4,		// 1, q+2
		952.8184, 100.0,		// 2, q+2
		953.3124,  49.3,		// 3, q+2
		953.8410,  23.6,		// 4, q+2
		954.3422, 500.0,
		954.3421,  14.6,		// 5, q+2
		954.8422,   7.3,		// 6, q+2
		955.3498,   5.1,		// 7, q+2
		955.8419,   2.9,		// 8, q+2
	   1032.3457,   2198381.8,
	   1230.3912, 1000,		// 1, q+1
	   1231.3973,  668,		// 2, q+1
	   1232.3946,  182,		// 3, q+1
	   1233.3934,  117,		// 4, q+1
	   1234.3932,   56,		// 4, q+1
	   1235.3931,   41,		// 4, q+1
	   1236.3937,   15,		// 4, q+1
	   1503.2941,    139831.9,
	   1602.9583,    123813.1,
	   1643.2910, 198453971.7,
	   1738.2938,   2439812.7,
	   1903.0283,   8428712.0,
	};
	double mzs3[37];
	double intensities3[37];

	for( int i = 0; i < 37; i++ ){
		mzs3[i] = composite[2*i];
		intensities3[i] = composite[2*i+1];
	}

	s1->setScanData( 10, 15.0, 32.0, 15, mzs1, intensities1 );
	s2->setScanData( 10, 15.0, 32.0, 0, mzs2, intensities2 );
	s3->setScanData( 10, 15.0, 32.0, 36, mzs3, intensities3 );

	// Peak* getPeak( int peakNumber, orders order = BY_MZ );
	if( s1->getPeak( 5 )->getMZ() != 601.0 )
		cout << "getPeak returned unexpected result 1" << endl;
	if( s1->getPeak( 5, BY_MZ )->getMZ() != 601.0 )
		cout << "getPeak returned unexpected result 2" << endl;
	if( s1->getPeak( 5, BY_INTENSITY )->getMZ() != 250.1 )
		cout << "getPeak returned unexpected result 3" << endl;
	if( s1->getPeak( 15 ) != NULL )
		cout << "getPeak returned unexpected result 4" << endl;
	if( s1->getPeak( 15, BY_MZ ) != NULL )
		cout << "getPeak returned unexpected result 5" << endl;
	if( s1->getPeak( 15, BY_INTENSITY ) != NULL )
		cout << "getPeak returned unexpected result 6" << endl;
	if( s1->getPeak( 0 )->getMZ() != 110.3 )
		cout << "getPeak returned unexpected result 7" << endl;
	if( s2->getPeak( 0 ) != NULL )
		cout << "getPeak returned unexpected result 8" << endl;


	// Peak* getFirstPeak( orders order = BY_MZ );
	if( s1->getFirstPeak()->getMZ() != 110.3 )
		cout << "getFirstPeak returned unexpected result 1" << endl;
	if( s1->getFirstPeak( BY_INTENSITY )->getMZ() != 122.4 )
		cout << "getFirstPeak returned unexpected result 2" << endl;
	if( s2->getFirstPeak() != NULL )
		cout << "getFirstPeak returned unexpected result 3" << endl;

	// Peak* getLastPeak( orders order = BY_MZ );
	if( s1->getLastPeak()->getMZ() != 1911.2 )
		cout << "getLastPeak returned unexpected result 1" << endl;
	if( s1->getLastPeak( BY_INTENSITY )->getMZ() != 1523.0 )
		cout << "getLastPeak returned unexpected result 2" << endl;
	if( s2->getLastPeak() != NULL )
		cout << "getLastPeak returned unexpected result 3" << endl;

	// bool getNext( Peak* &peak, orders order = BY_MZ );
	Peak* peak = NULL;
	if( !s1->getNext(peak) || peak->getMZ() != 110.3 )
		cout << "getNext returned unexpected result 1" << endl;
	peak = NULL;
	if( !s1->getNext(peak, BY_INTENSITY) || peak->getMZ() != 122.4 )
		cout << "getNext returned unexpected result 2" << endl;
	peak = s1->getPeak(3);
	if( !s1->getNext(peak) || peak->getMZ() != 250.1 )
		cout << "getNext returned unexpected result 3" << endl;
	peak = s1->getPeak(3);
	if( !s1->getNext(peak, BY_INTENSITY) || peak->getMZ() != 601.0 )
		cout << "getNext returned unexpected result 4" << endl;
	peak = s1->getPeak(14);
	if( s1->getNext(peak) || peak->getMZ() != 1911.2 )
		cout << "getNext returned unexpected result 5" << endl;
	peak = s1->getPeak(9);
	if( s1->getNext(peak, BY_INTENSITY) || peak->getMZ() != 1523.0 )
		cout << "getNext returned unexpected result 6" << endl;
	peak = NULL;
	if( s2->getNext(peak) != false || peak != NULL )
		cout << "getNext returned unexpected result 7" << endl;

	// bool getPrevious( Peak* &peak, orders order = BY_MZ );
	peak = NULL;
	if( !s1->getPrevious(peak) || peak->getMZ() != 1911.2 )
		cout << "getPrevious returned unexpected result 1" << endl;
	peak = NULL;
	if( !s1->getPrevious(peak, BY_INTENSITY) || peak->getMZ() != 1523.0 )
		cout << "getPrevious returned unexpected result 2" << endl;
	peak = s1->getPeak(3);
	if( !s1->getPrevious(peak) || peak->getMZ() != 241.1 )
		cout << "getPrevious returned unexpected result 3" << endl;
	peak = s1->getPeak(3);
	if( !s1->getPrevious(peak, BY_INTENSITY) || peak->getMZ() != 250.1 )
		cout << "getPrevious returned unexpected result 4" << endl;
	peak = s1->getPeak(0);
	if( s1->getPrevious(peak) || peak->getMZ() != 110.3 )
		cout << "getPrevious returned unexpected result 5" << endl;
	peak = s1->getPeak(1);
	if( s1->getPrevious(peak, BY_INTENSITY) || peak->getMZ() != 122.4 )
		cout << "getPrevious returned unexpected result 6" << endl;
	peak = NULL;
	if( s2->getPrevious(peak) != false || peak != NULL )
		cout << "getPrevious returned unexpected result 7" << endl;

	// Peak* getPeakLeftOf( double mz );
	if( s1->getPeakLeftOf( 250.2 )->getMZ() != 250.1 )
		cout << "getPeakLeftOf returned unexpected result 1" << endl;
	if( s1->getPeakLeftOf( 250.0 )->getMZ() != 241.1 )
		cout << "getPeakLeftOf returned unexpected result 2" << endl;
	if( s1->getPeakLeftOf( 50.0 ) != NULL )
		cout << "getPeakLeftOf returned unexpected result 3" << endl;
	if( s2->getPeakLeftOf( 132.2 ) != NULL )
		cout << "getPeakLeftOf returned unexpected result 4" << endl;

	// Peak* getPeakRightOf( double mz );
	if( s1->getPeakRightOf( 250.0 )->getMZ() != 250.1 )
		cout << "getPeakRightOf returned unexpected result 1" << endl;
	if( s1->getPeakRightOf( 250.1 )->getMZ() != 601.0 )
		cout << "getPeakRightOf returned unexpected result 2" << endl;
	if( s1->getPeakRightOf( 1950.0 ) != NULL )
		cout << "getPeakRightOf returned unexpected result 3" << endl;
	if( s2->getPeakRightOf( 132.2 ) != NULL )
		cout << "getPeakRightOf returned unexpected result 4" << endl;

	// Peak* getPeakNearest( double mz );
	if( s1->getPeakNearest( 1.0 )->getMZ() != 110.3 )
		cout << "getPeakNearest returned unexpected result 1" << endl;
	if( s1->getPeakNearest( 2000 )->getMZ() != 1911.2 )
		cout << "getPeakNearest returned unexpected result 2" << endl;
	if( s1->getPeakNearest( 250.0 )->getMZ() != 250.0 )
		cout << "getPeakNearest returned unexpected result 3" << endl;
	if( s1->getPeakNearest( 250.04 )->getMZ() != 250.0 )
		cout << "getPeakNearest returned unexpected result 4" << endl;
	if( s1->getPeakNearest( 250.06 )->getMZ() != 250.1 )
		cout << "getPeakNearest returned unexpected result 5" << endl;
	if( s1->getPeakNearest( 250.1 )->getMZ() != 250.1 )
		cout << "getPeakNearest returned unexpected result 6" << endl;
	if( s1->getPeakNearest( 425.5 )->getMZ() != 250.1 )
		cout << "getPeakNearest returned unexpected result 7" << endl;
	if( s1->getPeakNearest( 425.6 )->getMZ() != 601.0 )
		cout << "getPeakNearest returned unexpected result 8" << endl;
	if( s1->getPeakNearest( 601.5 )->getMZ() != 601.0 )
		cout << "getPeakNearest returned unexpected result 9" << endl;
	if( s2->getPeakNearest( 425.6 ) != NULL )
		cout << "getPeakNearest returned unexpected result 10" << endl;

	// vector<Peak*>* getPeaksInRange( double lowerMZBound, double upperMZBound );
	PeakSet* v = s1->getPeaksInRange(250.1, 250.2);
	if( v->size() != 0 )
		cout << "getPeaksInRange returned unexpected result 1" << endl;
	v = s1->getPeaksInRange(250.0, 601.1);
	if( v->size() != 2 )
		cout << "getPeaksInRange returned unexpected result 2" << endl;
	if( v->getFirstPeak()->getMZ() != 250.1 )
		cout << "getPeaksInRange returned unexpected result 3" << endl;
	if( v->getPeak(1)->getMZ() != 601.0 )
		cout << "getPeaksInRange returned unexpected result 4" << endl;
	v = s1->getPeaksInRange(0.0, 120.0);
	if( v->size() != 1 )
		cout << "getPeaksInRange returned unexpected result 5" << endl;
	if( v->getFirstPeak()->getMZ() != 110.3 )
		cout << "getPeaksInRange returned unexpected result 6" << endl;
	v = s1->getPeaksInRange(1900.0, 2000.0);
	if( v->size() != 1 )
		cout << "getPeaksInRange returned unexpected result 7" << endl;
	if( v->getFirstPeak()->getMZ() != 1911.2 )
		cout << "getPeaksInRange returned unexpected result 8" << endl;
	v = s2->getPeaksInRange(0, 2000.0);
	if( v->size() != 0 )
		cout << "getPeaksInRange returned unexpected result 9" << endl;

	// Peak* getLargestPeakInRange( double lowerMZBound, double upperMZBound );
	if( s1->getLargestPeakInRange( 0, 2000 )->getMZ() != 122.4 )
		cout << "getLargestPeakInRange returned unexpected result 1" << endl;
	if( s1->getLargestPeakInRange( 250.1, 601.0 ) != NULL )
		cout << "getLargestPeakInRange returned unexpected result 2" << endl;
	if( s1->getLargestPeakInRange( 250.0, 601.1 )->getMZ() != 250.1 )
		cout << "getLargestPeakInRange returned unexpected result 3" << endl;
	if( s2->getLargestPeakInRange( 0, 2000 ) != NULL )
		cout << "getLargestPeakInRange returned unexpected result 4" << endl;

	// getMZ(){ return mz; }
	if( s1->getPeak( 2 )->getMZ() != 241.1 )
		cout << "getMZ returned unexpected result 1" << endl;

	// getIntensity(){ return intensity; }
	if( s1->getPeak( 2 )->getIntensity() != 21342 )
		cout << "getIntensity returned unexpected result 1" << endl;

	// compare(double mz, double intensity);
	if( s1->getPeak( 2 )->compareTo( 241.1, 21342 ) != 0 )
		cout << "compare returned unexpected result 1" << endl;
	if( s1->getPeak( 2 )->compareTo( 241.2, 21342 ) <= 0 )
		cout << "compare returned unexpected result 2" << endl;
	if( s1->getPeak( 2 )->compareTo( 241.1, 31342 ) <= 0 )
		cout << "compare returned unexpected result 3" << endl;
	if( s1->getPeak( 2 )->compareTo( 241.3, 21342 ) < s1->getPeak( 2 )->compareTo( 241.2, 21342 ) )
		cout << "compare returned unexpected result 4" << endl;
	if( s1->getPeak( 2 )->compareTo( 241.1, 41342 ) < s1->getPeak( 2 )->compareTo( 241.1, 31342 ) )
		cout << "compare returned unexpected result 5" << endl;


	// compare(Peak* peak);
	if( s1->getPeak( 2 )->compareTo( s1->getPeak( 2 ) ) != 0 )
		cout << "compare2 returned unexpected result 1" << endl;
	if( s1->getPeak( 2 )->compareTo( s1->getPeak( 3 ) ) == 0 )
		cout << "compare2 returned unexpected result 2" << endl;

	// Peak* PeakSet::findPeakMatching(Peak* peak)
	if( s1->findPeakMatching( s1->getPeak( 2 ) ) != s1->getPeak( 2 ) )
		cout << "findPeakMatching returned unexpected result 1" << endl;

	// Peak* PeakSet::findPeakMatching(PeakSet* peakSet)
	PeakSet* matches = s1->findPeaksMatching( s2 );
	if(matches->size() != 0)
		cout << "findPeaksMatching returned unexpected result 1" << endl;
	delete matches;
	matches = s1->findPeaksMatching( s1 );
	if(matches->size() != 15)
		cout << "findPeaksMatching returned unexpected result 2" << endl;
	delete matches;

	// double PeakSet::compareTo(PeakSet* peakSet){
	if( s1->compareTo(s1) != 0 )
		cout << "compareTo returned unexpected result 1" << endl;
	if( ! s1->compareTo(s2) > 0 )
		cout << "compareTo returned unexpected result 2" << endl;


	// int getChargeState();
	// vector<Peak*>* getIsotopeSerie();

//	for(int i = 0; i < s2->size(); i++)
//		cout << s1->getPeak(i)->getMZ() << endl;

	//s2->remove(s3->getPeak(2));

//	cout << endl;
//	for(int i = 0; i < s2->size(); i++)
//		cout << s1->getPeak(i)->getMZ() << endl;

	// void PeakSet::add( Peak* peak )
	Peak* p1 = new Peak(500, 440);
	Peak* p2 = new Peak(501, 440);
	Peak* p3 = new Peak(504, 445);
	Peak* p4 = new Peak(499, 430);
	Peak* p5 = new Peak(499, 431);	//should fail to be added to set
	PeakSet ps;
	ps.add( p1 );
	if( ps.size() != 1 )
		cout << "add returned unexpected result 1" << endl;
	ps.add( p1 );
	if( ps.size() != 1 )
		cout << "add returned unexpected result 2" << endl;
	ps.add( p2 );
	if( ps.size() != 2 )
		cout << "add returned unexpected result 3" << endl;
	ps.add( NULL );
	if( ps.size() != 2 )
		cout << "add returned unexpected result 4" << endl;
	ps.add( p3 );
	ps.add( p4 );
	if( ps.getPeak(0) != p4 || ps.getPeak(1) != p1 || ps.getPeak(2) != p2 || ps.getPeak(3) != p3 )
		cout << "add returned unexpected result 5" << endl;
	if( ps.getPeak(0, BY_INTENSITY) != p3 || ps.getPeak(3, BY_INTENSITY) != p4 )
		cout << "add returned unexpected result 6" << endl;
	if( ps.getPeak(1, BY_INTENSITY) != p1 && ps.getPeak(1, BY_INTENSITY) != p2)
		cout << "add returned unexpected result 7" << endl;
	if( ps.getPeak(2, BY_INTENSITY) != p1 && ps.getPeak(2, BY_INTENSITY) != p2)
		cout << "add returned unexpected result 8" << endl;
	if( ps.getPeak(1, BY_INTENSITY) == ps.getPeak(2, BY_INTENSITY) )
		cout << "add returned unexpected result 9" << endl;
	ps.add( p5 );
	if( ps.size() != 4 )
		cout << "add returned unexpected result 10" << endl;

	//void PeakSet::remove( Peak* peak )
	PeakSet ps2 = PeakSet(&ps);
	ps2.remove(ps2.getPeak(2));
	if(ps2.size() != ps.size() - 1 || ps2.getPeak(1) != ps.getPeak(1) || ps2.getPeak(2) != ps.getPeak(3))
		cout << "remove returned unexpected result 1" << endl;
	ps2.remove(NULL);
	if(ps2.size() != ps.size() - 1)
		cout << "remove returned unexpected result 2" << endl;
	ps2.remove(p5);
	if(ps2.size() != ps.size() - 1)
		cout << "remove returned unexpected result 3" << endl;

	// int PeakSet::getNumberOf( Peak* peak, orders order )
	if( s3->getIndexOf( s3->getPeak(5) ) != 5 )
		cout << "getNumberOf returned unexpected result 1" << endl;
	if( s3->getIndexOf( s3->getPeak(5, BY_INTENSITY), BY_INTENSITY ) != 5 )
		cout << "getNumberOf returned unexpected result 2" << endl;	
	if( s3->getIndexOf( s2->getPeak(5) ) != -1 )
		cout << "getNumberOf returned unexpected result 3" << endl;	
	if( s3->getIndexOf( s2->getPeak(5, BY_INTENSITY), BY_INTENSITY ) != -1 )
		cout << "getNumberOf returned unexpected result 4" << endl;	

	delete s1, s2, s3;
	return 0;
}


/**/