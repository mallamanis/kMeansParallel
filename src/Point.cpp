#include "Point.h"

/** 
 *  The point constructor, initializing variables and allocating dynamically memory
 */

Point::Point() {
	coordinates = new float[dimension];

}

/** 
 *  The point copy constructor
 *  The constructor copies the variables, but creates a new memory allocation to avoid the
 *  coordinates pointer to point to the same place
 */

Point::Point(Point &rhs) {
	//The default copy constructor...

	rhs.squareDistanceFromCluster = squareDistanceFromCluster;
	for (unsigned int i = 0; i < dimension; i++)
		rhs.coordinates[i] = coordinates[i];
}

/**
 *  Overloaded operator =
 *  Used when assigning a point to another point. Solves the problem mentioned in the copy constructor
 */

Point &Point::operator=(Point & rhs) {
	squareDistanceFromCluster = rhs.squareDistanceFromCluster;
	for (unsigned int i = 0; i < dimension; i++)
		coordinates[i] = rhs.coordinates[i];

	return *this;
}

/** 
 *  The point copy destructor
 *  It deletes all dynamically allocated memory
 */

Point::~Point() {
	delete coordinates;
}

/** 
 *  Calculates the distance of the referenced point to the current point
 */

float Point::squareDistanceFrom(Point& anotherPoint) {
	float distance = 0;
	float difference;
	for (unsigned int i = 0; i < dimension; i++) {
		difference = coordinates[i] - anotherPoint.coordinates[i];
		distance += difference * difference;
	}

	return distance;
}

/** 
 *  A friendly function overloading operator "<<" used to "pretty" print to an stream the points coordinates
 */

ostream &operator<<(ostream &out, Point &point) {
	unsigned int i = 0;
	out << "(";
	for (i = 0; i < Point::dimension - 1; i++) {
		out << point.coordinates[i] << ",";
	}
	out << point.coordinates[i] << ")";
	return out;
}

/** 
 *  An overloaded opereator ==
 *  It is used to simplify comparison of points
 */

bool Point::operator==(Point &Point2) {
	for (unsigned int i = 0; i < Point::dimension; i++)
		if (coordinates[i] != Point2.coordinates[i])
			return false;
	return true;
}

//Static variable declaration
unsigned int Point::dimension = 0;
