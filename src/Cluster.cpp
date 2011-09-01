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
