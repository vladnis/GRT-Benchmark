/*
 * MemoryUsage.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: Nistorica Vlad
 */

#include "MemoryUsage.h"

MemoryUsage::MemoryUsage() {
	totalPhysMem = 0;
	totalVirtualMem = 0;
	virtualMemUsed = 0;
	physMemUsed = 0;
	virtualMemUsedByProccess = 0;
	physMemUsedByProccess = 0;
	ru_idrss = 0;
	ru_isrss = 0;
}

int parseLine(char* line){
	int i = strlen(line);
	while (*line < '0' || *line > '9') line++;
	line[i-3] = '\0';
	i = atoi(line);
	return i;
}

/* Value is in KB */
int getvirtualMemUsedByProccess(){
	FILE* file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];

	while (fgets(line, 128, file) != NULL){
		if (strncmp(line, "VmSize:", 7) == 0){
			result = parseLine(line);
			break;
		}
	}
	fclose(file);
	return result;
}

int getphysMemUsedByProccess(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];


    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

void MemoryUsage::takeSample() {
	struct sysinfo memInfo;
	sysinfo (&memInfo);

	/* Total virtual memory */
	totalVirtualMem = memInfo.totalram;
	totalVirtualMem += memInfo.totalswap;
	totalVirtualMem *= memInfo.mem_unit;

	/* Virtual memory used */
	virtualMemUsed = memInfo.totalram - memInfo.freeram;
	virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
	virtualMemUsed *= memInfo.mem_unit;

	/* Virtual memory used by current proccess */
	virtualMemUsedByProccess = getvirtualMemUsedByProccess();

	/* Total physical memory */
	totalPhysMem = memInfo.totalram;
	totalPhysMem *= memInfo.mem_unit;

	/* Physical Memory currently used */
	physMemUsed = memInfo.totalram - memInfo.freeram;
	physMemUsed *= memInfo.mem_unit;

	/* Physical memory used by current process */
	physMemUsedByProccess = getphysMemUsedByProccess();

	/* Take samples with getrusage */
	struct rusage rUsage;
	getrusage(RUSAGE_SELF, &rUsage);
	ru_idrss = rUsage.ru_idrss;
	ru_isrss = rUsage.ru_isrss;
}

MemoryUsage::~MemoryUsage() {
}

