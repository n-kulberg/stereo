//#include "Segment.h"

#include "TrivialTests.h"
#include "SectionsDistanceTest.h"
#include <iostream>


//------------------------------------------------------------------
//
//	created:	2021/09/16	17:00
//	filename: 	Segment.cpp
//	file path:	q:\Projects\Sandbox\Sandbox\sources
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

using namespace std;

int	main(int, char **)
{
	try
	{
		if(1)
		{
			//principal testing procedure:
			// generate a lot of random segments pairs with predefined distance between them, compute distance, compare result
			//TestSection3DistanceAuto();
			TestSegment3DistanceManual();
		}
		else
		{
			// Auxiliary testing procedures:
			// test basic operations correctness
			TestVectorOperations();
			TestSegment();
			TestMatrix();
		}
		fflush(stdout);

	}
	catch(exception& ex)
	{
		cout << "An error occured:" << endl << ex.what();
	}


	int n;
	std::cin >> n;
	// to avoid console close

	return 0;
}