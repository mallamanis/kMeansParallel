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
#include "KMeansSolver.h"

//Static variable declaration
int KMeansSolver::numberOfThreads;
JobPool KMeansSolver::classifyJob(0);
JobPool KMeansSolver::calculateCenterJob(0);
ClusterRepository *KMeansSolver::cl_repository;
PointRepository *KMeansSolver::pt_repository;
pthread_cond_t *KMeansSolver::start_working;
bool KMeansSolver::clusters_changed;
pthread_mutex_t *KMeansSolver::mutex_clusters_changed;

/**
 *  Initialization of all mutexes and variables needed in the solver.
 *  The Clusters are also randomly initialized.
 */

void KMeansSolver::initSolver() {

	cl_repository = ClusterRepository::getInstance();
	pt_repository = PointRepository::getInstance();

	//Initialize mutex
	mutex_clusters_changed = new pthread_mutex_t;
	pthread_mutex_init(mutex_clusters_changed, NULL);

	start_working = new pthread_cond_t;
	pthread_cond_init(start_working, NULL);

	clusters_changed = true;

	//Select random points for the clusters.
	srand(time(0)); //random seed
	for (unsigned int i = 0; i < cl_repository->numberOfClusters; i++) { //For every cluster
		int id = rand() % pt_repository->numberOfPoints;
		cl_repository->repository[i].clusterCenter
				= pt_repository->repository[id]; //TODO: Does it matter that this points to a point in the repository?
	}
}

/**
 *  The coordinator of all jobs and workers (threads).
 *  This function starts the threads and starts/restarts the jobs when necessary
 */

void KMeansSolver::solve() {
	unsigned int iteration = 0;//The number of iterations completed;

	//Create threads!
	for (int i = 0; i < numberOfThreads; i++) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_t *a_thread = new pthread_t;
		int rc = pthread_create(a_thread, &attr, KMeansSolver::threadSolve,
				NULL);
		if (rc) {
			std::cout << "ERROR; return code from pthread_create() is " << rc
					<< "\n";
			exit(rc);
		}
	};

	clusters_changed = true;

	while (iteration < MAX_ITERATIONS && clusters_changed) { //perform an iteration
		iteration++;
		std::cout << "Starting iteration " << iteration << "\n";

		//clusters_changed=false; //TODO: Added later. works?
		pthread_mutex_lock(mutex_clusters_changed);
		clusters_changed = false;
		pthread_mutex_unlock(mutex_clusters_changed);

		//Clear Clusters
		for (unsigned int i = 0; i < cl_repository->numberOfClusters; i++)
			cl_repository->repository[i].clear(); //TODO: Anyone setting to false, cluster changed?

		//Load Jobs
		calculateCenterJob.restartJob(cl_repository->numberOfClusters);
		classifyJob.restartJob(pt_repository->numberOfPoints);

		pthread_mutex_lock(calculateCenterJob.mutex_done);
		if (!calculateCenterJob.done)
			pthread_cond_wait(calculateCenterJob.jobDone,
					calculateCenterJob.mutex_done);
		pthread_mutex_unlock(calculateCenterJob.mutex_done);
		//KMeansSolver::printInfo(); //DEBUG
	}
	//std::cout<<"clusters_changed:"<<clusters_changed<<"\n"; //DEBUG
}

/**
 *  The thread that solves the problem. The threads wait for the jobs to start
 *  and finish in order to register and get jobs.
 */

void *KMeansSolver::threadSolve(void * param) {

	while (true) {

		//pthread_cond_wait() for start signal? (need_new_iteration)
		pthread_mutex_lock(classifyJob.mutex_jobsAvailable);
		if (classifyJob.jobsAvailable == 0)
			pthread_cond_wait(classifyJob.jobStart,
					classifyJob.mutex_jobsAvailable);
		if (classifyJob.jobsAvailable == 0) {
			pthread_mutex_unlock(classifyJob.mutex_jobsAvailable);
			continue;
		};
		classifyJob.registerWorker();
		calculateCenterJob.registerWorker();
		pthread_mutex_unlock(classifyJob.mutex_jobsAvailable);
		//First classify all points into some cluster
		int fromPtID, toPtID;
		//register
		//std::cout<<"thread starting classification!"<<pthread_self()<<"\n!"; //DEBUG
		while (classifyJob.getJobs(BATCH_JOB_CLASSIFY_QUANTITY, fromPtID,
				toPtID)) { //while there are jobs to do
			while (fromPtID <= toPtID) { //And we haven't taken them all
				//std::cout<<"thread classifing"<<pthread_self()<<" the "<<toPtID<<"\n!"; //DEBUG
				//Classify point toPtID
				classifyPoint(toPtID - 1);
				toPtID--; //This point is done...
			}
		}
		classifyJob.unregisterWorker();
		//Wait for everyone to finish...

		pthread_mutex_lock(classifyJob.mutex_done);
		if (!classifyJob.done)
			pthread_cond_wait(classifyJob.jobDone, classifyJob.mutex_done);
		pthread_mutex_unlock(classifyJob.mutex_done);

		//Start calculating centers...
		//std::cout<<"thread starting center calculation!"<<pthread_self()<<"\n!"; //DEBUG
		int fromClID, toClID;

		while (calculateCenterJob.getJobs(BATCH_JOB_CENTER_CALCULATE_QUANTITY,
				fromClID, toClID)) {
			while (fromClID <= toClID) {

				//std::cout<<"thread starting center calculation!"<<pthread_self()<<"from ("<<fromClID<<" to "<<toClID<<")\n!"; //DEBUG
				//Calculate New Center
				cl_repository->repository[toClID - 1].calculateNewCenter();

				//Update clusters_changed and cluster's center
				if (cl_repository->repository[toClID - 1].hasChanged) {
					pthread_mutex_lock(mutex_clusters_changed);
					clusters_changed = true;
					pthread_mutex_unlock(mutex_clusters_changed);

				}

				toClID--; //Done with this cluster
			}
		}

		calculateCenterJob.unregisterWorker();
		//std::cout<<"thread out"<<pthread_self()<<"\n!"; //DEBUG
		//std::cout<<"thread out"<<pthread_self()<<"\n!"; //DEBUG

	}
	return NULL;
}

/** 
 *  A function that classifies a point. This is the core function used for classification of
 *  points into clusters
 */

void KMeansSolver::classifyPoint(int pointID) {
	Point *thisPoint = &pt_repository->repository[pointID];
	int best_index = 0;
	float best_distance = thisPoint->squareDistanceFrom(
			cl_repository->repository[0].clusterCenter);
	float distance;

	//find best fit
	for (unsigned int i = 1; i < cl_repository->numberOfClusters; i++) {
		distance = thisPoint->squareDistanceFrom(
				cl_repository->repository[i].clusterCenter);
		if (distance < best_distance) {
			best_distance = distance;
			best_index = i;
		}
	}

	thisPoint->squareDistanceFromCluster = best_distance;

	pthread_mutex_lock(cl_repository->repository[best_index].mutex);
	cl_repository->repository[best_index].pointsInClusterId.push_back(pointID);
	pthread_mutex_unlock(cl_repository->repository[best_index].mutex);

}

/** 
 *  A helper function that prints the summary
 */

void KMeansSolver::printInfo() {
	/*std::cout<<"k-means algorithm produced the following clusters:\n";
	 for (unsigned int i=0;i<cl_repository->numberOfClusters;i++){
	 std::cout<<"Cluster "<<i<<":"<<cl_repository->repository[i].clusterCenter<<"\n";
	 for (unsigned int j=0;j<cl_repository->repository[i].pointsInClusterId.size();j++){
	 std::cout<<"Point "<<j<<":"<<pt_repository->repository[cl_repository->repository[i].pointsInClusterId[j]]<<"\n";
	 }
	 }*/

	float dist = 0;
	std::cout << "Total Square Distance:";
	for (int i = 0; i < pt_repository->numberOfPoints; i++) {
		dist += pt_repository->repository[i].squareDistanceFromCluster;
	}
	std::cout << dist << "\n";
}
