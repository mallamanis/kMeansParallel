#include "ClusterRepository.h"

//Definition of static variable
ClusterRepository *ClusterRepository::instance;

/**
 *  A static function allowing to set the static property of number of clusters
 */

void ClusterRepository::setNumberOfClusters(unsigned int nClusters) {

	instance = new ClusterRepository;
	instance->repository = new Cluster[nClusters];
	instance->numberOfClusters = nClusters;
}

/** 
 *  Returns the singleton instance of the ClusterRepository
 */
ClusterRepository *ClusterRepository::getInstance() {
	if (instance == NULL) {
		cout << "Cluster number not set!";
		exit(3);
	}
	return instance;
}

