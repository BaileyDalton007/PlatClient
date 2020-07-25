#pragma once
#include <iostream>
#include <vector>
#include "main.h"
#include "client.h"
#include <string>
#include <sstream>
#include <time.h>
#include <chrono>
#include <Windows.h>
#include <thread>


std::vector<const char*> cpsCodes = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9","10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "MAX" };

std::vector<int> clicks;
bool currDown = false;

auto startPoint = std::chrono::high_resolution_clock::now();
const char* cps;

void getCurrClick()
{
	if (currDown == false) {
		if (GetKeyState(MK_LBUTTON) & 0x8000)
		{
			currDown = true;
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> click_ms = t2 - startPoint;
			clicks.push_back(click_ms.count());
		}
	}

	if (currDown == true)
	{
		if (GetKeyState(MK_LBUTTON) == 0)
		{
			currDown = false;
			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> click_ms = t2 - startPoint;
			clicks.push_back(click_ms.count());
		}
	}
}

void updateCPS()
{
	auto curr1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> curr = curr1 - startPoint;
	for (int i = 0; i < clicks.size(); i++)
	{
		if (curr.count() - clicks[i] > 1005)
		{
			clicks.erase(clicks.begin() + (i));
		}
	}
	int size = (int)clicks.size();
	//int size = (int)4;

	//char acps[1000];
	//_itoa_s(size, acps, 10);

	cps = (const char*)cpsCodes[size];
}

const char* getCPS()
{
	return (const char*)cps;
}