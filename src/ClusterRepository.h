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
