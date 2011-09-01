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
#ifndef Point_h
#define Point_h

#include <iostream>
using namespace std;

class Point {
public:

	/**
	 *  The point constructor, initializing variables and allocating dynamically memory
	 */
	Point();

	/**
	 *  The point copy constructor
	 *  The constructor copies the variables, but creates a new memory allocation to avoid the
	 *  coordinates pointer to point to the same place
	 */
	Point(Point&);

	/**
	 *  The point copy destructor
	 *  It deletes all dynamically allocated memory
	 */
	~Point();

	/**
	 *  Static Variable that contains the dimension of all points, during this run. A static variable is used to save memory.
	 */
	static unsigned int dimension;

	/**
	 *  the "table" containing the coordinates
	 */
	float *coordinates;

	/**
	 *  This float contains the square distance from the cluster it is in.
	 *  This variable is used to avoid the recalculation in the final steps.
	 *  The value of this variable is assigned when the point is classified to a cluster
	 */
	float squareDistanceFromCluster;

	/**
	 *  Calculates the distance of the referenced point to the current point
	 */
	float squareDistanceFrom(Point&);

	/**
	 *  A friendly function overloading operator "<<" used to "pretty" print to an stream the points coordinates
	 */
	friend ostream &operator<<(ostream&, Point &);

	/**
	 *  An overloaded opereator ==
	 *  It is used to simplify comparison of points
	 */
	bool operator==(Point&);

	/**
	 *  Overloaded operator =
	 *  Used when assigning a point to another point. Solves the problem mentioned in the copy constructor
	 */
	Point &operator=(Point &);

};

#endif // Point_h
