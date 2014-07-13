/*
 * MemoryUsage.h
 *
 *  Created on: Jun 24, 2014
 *      Author: Nistorica Vlad
 */

#ifndef MEMORYUSAGE_H_
#define MEMORYUSAGE_H_
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <sys/time.h>
#include <sys/resource.h>

class MemoryUsage {

public:
	long long totalPhysMem, totalVirtualMem, virtualMemUsed, physMemUsed;
	long long virtualMemUsedByProccess, physMemUsedByProccess;
	long ru_idrss, ru_isrss;

public:
	MemoryUsage();
	virtual ~MemoryUsage();
	void takeSample();

	inline MemoryUsage operator-(MemoryUsage const sample) const {
		MemoryUsage aux;
		aux.totalPhysMem = llabs(sample.totalPhysMem - this->totalPhysMem);
		aux.totalVirtualMem = llabs(sample.totalVirtualMem - this->totalVirtualMem);
		aux.virtualMemUsed = llabs(sample.virtualMemUsed - this->virtualMemUsed);
		aux.physMemUsed = llabs(sample.physMemUsed - this->physMemUsed);
		aux.virtualMemUsedByProccess = llabs(sample.virtualMemUsedByProccess - this->virtualMemUsedByProccess);
		aux.physMemUsedByProccess = llabs(sample.physMemUsedByProccess - this->physMemUsedByProccess);
		aux.ru_idrss = labs(sample.ru_idrss - this->ru_idrss);
		aux.ru_isrss = labs(sample.ru_isrss - this->ru_isrss);
		return aux;
	}
};



#endif /* MEMORYUSAGE_H_ */
