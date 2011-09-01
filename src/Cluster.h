#ifndef Cluster_h
#define Cluster_h

#include "PointRepository.h"
#include <vector>
#include <pthread.h>

class Cluster {
public:
	/**
	 *  the current center of the cluster
	 */
	Point clusterCenter;

	/**
	 *  the new (to be calculated) center of the cluster
	 */
	Point newCenter;

	/**
	 *  a vector containing all the ids (index in point repository)
	 *  of points in the cluster
	 */
	vector<int> pointsInClusterId;

	/**
	 *  This function calculates the new centroid of the cluster using the points in the cluster
	 */
	void calculateNewCenter();

	/**
	 *  A boolean. If the cluster center has changed, it is updated to true
	 */
	bool hasChanged;

	/**
	 *  A pointer to the point repository. This pointer is used to avoid frequent time consumning referencing of the getInstance()
	 *  of the singleton PointRepository
	 */
	PointRepository *ptReposit;

	/**
	 *  The cluster constructor, initializing the local variables
	 */
	Cluster();

	/**
	 *  Clears all points in cluster in order to re-classify the points. Used when a new iteration is started
	 */
	void clear();

	/**
	 *  A pthread mutex. It is locked (externally) when a new point is added to the cluster
	 */
	pthread_mutex_t *mutex;
};

#endif // Cluster_h
