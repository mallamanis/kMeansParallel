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
