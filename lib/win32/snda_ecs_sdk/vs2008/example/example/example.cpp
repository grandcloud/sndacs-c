// example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	get_service_example();

	put_bucket();
	get_bucket();
	delete_bucket();
	
	put_bucket_policy();
	get_bucket_policy();
	delete_bucket_policy();

	get_bucket_location();
	
	put_object();
	get_object();
	head_object();
	delete_object() ;
	return 0;
}

