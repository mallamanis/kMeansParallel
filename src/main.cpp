#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include "KMeansSolver.h"
#include "ClusterRepository.h"

int main(int argc, char** argv) {
	//Check for wrong parameter count
	if (argc < 4 || argc > 4) {
		std::cout << "Error: Wrong number of parameters\n";
		exit(1);
	}

	//Load points serially
	std::cout << "Loading points from " << argv[1] << "...\n";
	PointRepository::loadPoints(argv[1]);

	//Setting number of clusters
	ClusterRepository::setNumberOfClusters(atoi(argv[2]));
	std::cout << "Using " << argv[2] << " clusters.\n";

	//Setting number of threads
	KMeansSolver::numberOfThreads = atoi(argv[3]);
	std::cout << "Maximum number of threads that will be used: "
			<< KMeansSolver::numberOfThreads << "\n";

	std::cout << "Starting Work!\n";
	KMeansSolver::initSolver();
	KMeansSolver::solve();

	KMeansSolver::printInfo();

	return 0;
}
