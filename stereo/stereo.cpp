//#include "Segment.h"

#include "TrivialTests.h"
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
		TestVectorOperations();
		TestSegment();
		TestMatrix();
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