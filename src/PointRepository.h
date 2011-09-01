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
