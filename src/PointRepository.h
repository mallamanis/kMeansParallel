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
#ifndef PointRepository_h
#define PointRepository_h

#include "Point.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** 
 *  A singleton class, representing a repository of all points in the class
 */

class PointRepository {
public:

	/** 
	 *  the table (pointer) containing all the points
	 */
	Point *repository;

	/** 
	 *  returns the single instance of the class
	 */
	static PointRepository *getInstance();

	/** 
	 *  a static function that loads into the single instance points from a file
	 */
	static void loadPoints(char*);

	/** 
	 *  the number of points in the object
	 */
	int numberOfPoints;

private:
	/** 
	 *  the pointer to the instance
	 */
	static PointRepository *instance;

	/** 
	 *  a private contructor, forcing the singleton sterotype
	 *  The argument is the filename (sting) of the file containing the points
	 */
	PointRepository(char *);

	/** 
	 *  the destructor of the point repository
	 */
	~PointRepository();
};

#endif // PointRepository_h
