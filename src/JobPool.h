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
#ifndef JobPool_h
#define JobPool_h

#include <vector>
#include <pthread.h>
#include <iostream>

class JobPool {
public:

	/**
	 *  The cluster constructor, initializing all mutexes and the job
	 */
	JobPool(int noOfJobs);

	/**
	 *  Restarts the job, by reloading the "queue" and signaling any waiting threads
	 */
	void restartJob(int noOfJobs);

	/**
	 *  A member function called when a worker thread needs a new batch job.
	 *  The caller of the functions gets the jobs as reference of integers.
	 *  In order to call get Jobs, a working thread must have called registerWorker()
	 *  @returns true if there are any jobs available.
	 */
	bool getJobs(int, int&, int&);

	/**
	 *  A condition variable, used by threads to get signalled that the job is done
	 */
	pthread_cond_t *jobDone;

	/**
	 *  A boolean. True if the job is done.
	 *  This variable is used by threads that have not received the signal
	 */
	bool done;

	/**
	 *  The mutex conserning the done variable and the jobDone condition variable
	 */
	pthread_mutex_t *mutex_done;

	/**
	 *  A condition variable, used by threads to get signalled that the job has started,
	 *  prompting them to start getting jobs
	 */
	pthread_cond_t *jobStart;

	/**
	 *  Each thread "working" on the job registers first, so that the object knows how many threads
	 *  are working on the problem.
	 */
	void registerWorker();

	/**
	 *  A thread finishing the work, unregisters. When all theads (that have registered) unregister and
	 *  there are no jobsAvailable, the job is done
	 */
	void unregisterWorker();

	/**
	 *  A mutex used to lock the jobsAvailable variable
	 */
	pthread_mutex_t *mutex_jobsAvailable;

	/**
	 *  This variable contains the number of jobs available. With this variable, the getJobs knows which tasks are
	 *  available for workers
	 */
	int jobsAvailable;

private:
	/**
	 *  A mutex used to lock the workers variable
	 */
	pthread_mutex_t *mutex_workers;

	/**
	 *  This variable contains the number of workers (threads) doing some work on the job
	 */
	int workers;

};

#endif // JobPool_h
