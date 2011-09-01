#ifndef KMeansSolver_h
#define KMeansSolver_h

#include "ClusterRepository.h"
#include "JobPool.h"

/** 
 *  Maximum number of iterations
 */
#define MAX_ITERATIONS 30

/** 
 *  The number of jobs that each time a thread that is classifing points receives.
 *  This number is heuristicly set to 80. We should avoid giving small numbers, because
 *  there will be a "congestion" asking for jobs (that require mutex locks). We should
 *  also avoid giving a big number, because for the last jobs a thread may have a bigger
 *  workload than the others.
 */
#define BATCH_JOB_CLASSIFY_QUANTITY 80

/** 
 *  The number of jobs that each time a thread that is calculating centers receives.
 *  This should be small enough to avoid that a thread gets many "overcrowded" clusters
 *  It could result in a thread taking too much time to finish, while others wait.
 */
#define BATCH_JOB_CENTER_CALCULATE_QUANTITY 2

/** 
 *  A static utility containing all functions and threads needed to solve kmeans
 */
class KMeansSolver {
public:

	/**
	 *  The number of theads used in the solver
	 */
	static int numberOfThreads;

	/**
	 *  The thread that solves the problem. The threads wait for the jobs to start
	 *  and finish in order to register and get jobs.
	 */
	static void* threadSolve(void *);

	/**
	 *  A JobPool object representing the classification jobs
	 */
	static JobPool classifyJob;

	/**
	 *  A JobPool object representing the centeroid calculation job
	 */
	static JobPool calculateCenterJob;

	/**
	 *  The coordinator of all jobs and workers (threads).
	 *  This function starts the threads and starts/restarts the jobs when necessary
	 */
	static void solve();

	/**
	 *  Initialization of all mutexes and variables needed in the solver.
	 *  The Clusters are also randomly initialized.
	 */
	static void initSolver();

	/**
	 *  A pointer to the ClusterRepository. Used for quick referencing
	 */
	static ClusterRepository *cl_repository;

	/**
	 *  A pointer to the PointRepository. Used for quick referencing
	 */
	static PointRepository *pt_repository;

	/**
	 *  A function that classifies a point. This is the core function used for classification of
	 *  points into clusters
	 */
	inline static void classifyPoint(int);

	/**
	 *  A helper function that prints the summary
	 */
	static void printInfo();

	/**
	 *  A boolean. It is true if at least one cluster has changed, signaling that a new
	 *  iteration of the kmeans algorithm is needed
	 */
	static bool clusters_changed;

	/**
	 *  A mutex used for writing the clusters_changed variable
	 */
	static pthread_mutex_t *mutex_clusters_changed;

	/**
	 *  A condition variable to signal the start of all work
	 */
	static pthread_cond_t *start_working;

};

#endif // KMeansSolver_h
