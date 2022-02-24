#pragma once
#include <chrono>
#include <thread>
#include <iostream>

std::chrono::high_resolution_clock::time_point timeNow();
double timeSpent(std::chrono::high_resolution_clock::time_point timeStart);

unsigned int getKernelsCount();


