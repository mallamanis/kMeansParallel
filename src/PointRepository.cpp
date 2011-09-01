/*
 *	Copyright (C) 2009 by Miltiadis Allamanis
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */
#include "PointRepository.h"
#define BUF_SIZE 1024 //The size of the buffer used while reading the file
//Static variable declaration
PointRepository *PointRepository::instance;

/** 
 *  returns the single instance of the class
 */

PointRepository *PointRepository::getInstance() {
	if (instance == NULL) {
		cout << "Error, no points loaded";
		exit(1);
	}
	return instance;
}

/** 
 *  a static function that loads into the single instance points from a file
 */

void PointRepository::loadPoints(char *filename) {
	if (instance != NULL)
		delete instance;
	instance = new PointRepository(filename);
}

/**
 *  a private contructor, forcing the singleton sterotype
 *  The argument is the filename (sting) of the file containing the points
 */

PointRepository::PointRepository(char* filename) {

	/* Code adapted from Wei-keng Liao */

	unsigned int numAttributes = 0, numObjects = 0;
	;
	char line[BUF_SIZE]; //Temporary buffer
	FILE *infile;

	//Open file if possible
	if ((infile = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error: no such file (%s)\n", filename);
		exit(1);
	}

	while (fgets(line, 1024, infile) != NULL)
		if (strtok(line, " \t\n") != 0)
			numObjects++;
	rewind(infile);
	while (fgets(line, 1024, infile) != NULL) {
		if (strtok(line, " \t\n") != 0) {
			/* ignore the id (first attribute): numAttributes = 1; */
			while (strtok(NULL, " ,\t\n") != NULL)
				numAttributes++;
			break;
		}
	}

	rewind(infile); //Goto the beggining
	Point::dimension = numAttributes; //Set global problem dimension
	numberOfPoints = numObjects;
	repository = new Point[numObjects];

	//Store all the points!

	unsigned int attributeNo = 0;
	int pointNo = -1;
	while (fgets(line, BUF_SIZE, infile) != NULL) {
		if (strtok(line, " \t\n") == NULL)
			continue;
		for (unsigned int j = 0; j < numAttributes; j++) {
			if (attributeNo == 0) {
				pointNo++;
			}
			repository[pointNo].coordinates[attributeNo] = atof(
					strtok(NULL, " ,\t\n"));
			attributeNo = (++attributeNo) % (Point::dimension);

		}
	}

	/* DEBUG: Print data

	 for (int i=0;i<numberOfPoints;i++){
	 std::cout<<"Point "<<i<<":"<<repository[i]<<"\n";
	 } */

	//Close File
	fclose(infile);
}

/**
 *  the destructor of the point repository
 */

PointRepository::~PointRepository() {

	//delete [numberOfPoints] repository;
}
