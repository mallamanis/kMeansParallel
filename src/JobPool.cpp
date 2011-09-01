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
#include "JobPool.h"

/**
 *  The cluster constructor, initializing all mutexes and the job
 */

JobPool::JobPool(int noOfJobs) {
	//Initialize mutexes
	mutex_jobsAvailable = new pthread_mutex_t;
	pthread_mutex_init(mutex_jobsAvailable, NULL);

	mutex_workers = new pthread_mutex_t;
	pthread_mutex_init(mutex_workers, NULL);

	mutex_done = new pthread_mutex_t;
	pthread_mutex_init(mutex_done, NULL);

	jobDone = new pthread_cond_t;
	pthread_cond_init(jobDone, NULL);

	jobStart = new pthread_cond_t;
	pthread_cond_init(jobStart, NULL);

	workers = 0;

	restartJob(noOfJobs);
}

/** 
 *  Restarts the job, by reloading the "queue" and signaling any waiting threads
 */

void JobPool::restartJob(int noOfJobs) {
	pthread_mutex_lock(mutex_jobsAvailable);
	pthread_mutex_lock(mutex_done);
	jobsAvailable = noOfJobs;
	done = false;
	//std::cout<<"JobStart!\n"; //DEBUG
	pthread_cond_broadcast(jobStart);
	pthread_mutex_unlock(mutex_done);
	pthread_mutex_unlock(mutex_jobsAvailable);

}

/** 
 *  A member function called when a worker thread needs a new batch job.
 *  The caller of the functions gets the jobs as reference of integers.
 *  In order to call get Jobs, a working thread must have called registerWorker()
 *  @returns true if there are any jobs available.
 */

bool JobPool::getJobs(int numberOfJobsToGet, int &fromId, int &toId) {
	bool hasJobs = true;
	//Lock mutex
	pthread_mutex_lock(mutex_jobsAvailable);

	toId = jobsAvailable;
	fromId = jobsAvailable - numberOfJobsToGet + 1;

	if (fromId < 1) {
		fromId = 1;
		jobsAvailable = 0;
		hasJobs = true;
	} else {
		jobsAvailable = fromId - 1;
	}
	//Unlock mutex
	pthread_mutex_unlock(mutex_jobsAvailable);
	if (toId == 0)
		hasJobs = false;
	return hasJobs;
}

/** 
 *  Each thread "working" on the job registers first, so that the object knows how many threads
 *  are working on the problem.
 */

void JobPool::registerWorker() {
	//Lock mutex
	pthread_mutex_lock(mutex_workers);
	workers++;
	//Unlock mutex
	pthread_mutex_unlock(mutex_workers);
}

/** 
 *  A thread finishing the work, unregisters. When all theads (that have registered) unregister and
 *  there are no jobsAvailable, the job is done
 */

void JobPool::unregisterWorker() {
	//Lock mutex
	pthread_mutex_lock(mutex_workers);
	pthread_mutex_lock(mutex_done);
	pthread_mutex_lock(mutex_jobsAvailable);
	workers--;

	if (workers == 0 && jobsAvailable == 0) {
		//TODO: Maybe call a function to do some wrapping?
		//std::cout<<"JobDone!\n"; //DEBUG
		pthread_cond_broadcast(jobDone);
		done = true;
	}
	//Unlock mutex
	pthread_mutex_unlock(mutex_jobsAvailable);
	pthread_mutex_unlock(mutex_done);
	pthread_mutex_unlock(mutex_workers);

}

