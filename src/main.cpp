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
