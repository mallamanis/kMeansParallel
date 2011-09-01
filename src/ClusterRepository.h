#ifndef ClusterRepository_h
#define ClusterRepository_h
#include <iostream>
#include <stdlib.h>
#include "Cluster.h"

class ClusterRepository {
public:

	/**
	 *  This variable contains the number of clusters used in the problem
	 */
	unsigned int numberOfClusters;

	/**
	 *  A static function allowing to set the static property of number of clusters
	 */
	static void setNumberOfClusters(unsigned int);

	/**
	 *  Returns the singleton instance of the ClusterRepository
	 */
	static ClusterRepository *getInstance();

	/**
	 *  The "table" containing all clusters
	 */
	Cluster *repository;

private:
	/**
	 *  The single instance of the class
	 */
	static ClusterRepository *instance;

};

#endif // ClusterRepository_h
