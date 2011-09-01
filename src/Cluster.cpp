#include "Cluster.h"

/** 
 *  The cluster constructor, initializing the local variables
 */

Cluster::Cluster() {
	mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, NULL);
	ptReposit = PointRepository::getInstance();
	hasChanged = false;
}

/** 
 *  Clears all points in cluster in order to re-classify the points. Used when a new iteration is started
 */

void Cluster::clear() {
	pthread_mutex_lock(mutex);
	pointsInClusterId.clear();
	hasChanged = false;
	pthread_mutex_unlock(mutex);
}

/** 
 *  This function calculates the new centroid of the cluster using the points in the cluster
 */

void Cluster::calculateNewCenter() {
	pthread_mutex_lock(mutex);

	//clear newCenter //TODO: use memset to avoid for
	for (unsigned int i = 0; i < Point::dimension; i++) {
		newCenter.coordinates[i] = 0;
	}

	for (unsigned int j = 0; j < pointsInClusterId.size(); j++) {
		Point *adding = &ptReposit->repository[pointsInClusterId[j]];
		for (unsigned int i = 0; i < Point::dimension; i++) {
			newCenter.coordinates[i] += adding->coordinates[i];
		}
	}
	int size = pointsInClusterId.size();
	if (size > 0)
		for (unsigned int i = 0; i < Point::dimension; i++)
			newCenter.coordinates[i] /= size;

	if (!(newCenter == clusterCenter)) {
		//std::cout<<"OLD:"<<clusterCenter<<" new:"<<newCenter<<"\n"; //DEBUG
		hasChanged = true;
		clusterCenter = newCenter;
	}
	pthread_mutex_unlock(mutex);

}
