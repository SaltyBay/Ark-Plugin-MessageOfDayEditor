/****************************************************************************
* Copyright (C) 2019 by Matthias Birnthaler                                 *
*                                                                           *
* This file is part of the OverwriteCuddleWalking Plugin for Ark Server API *
*                                                                           *
*   The OverwriteCuddleWalking Plugin is free software: you can             *
*	redistribute it and/or modify it under the terms of the GNU Lesser      *
*	General Public License as published by the Free Software Foundation,    *
*	either version 3 of the License, or (at your option) any later version. *
*                                                                           *
*   The OverwriteCuddleWalking Plugin is distributed in the hope that it    *
*   will be useful, but WITHOUT ANY WARRANTY; without even the implied      *
* 	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.        *
* 	See the GNU Lesser General Public License for more details.             *
*                                                                           *
****************************************************************************/

/**
* \file Util.cpp
* \author Matthias Birnthaler
* \date 19 March 2019
* \brief Implementation of utility functions
*
*/


#include "Util.h"
#include "json.hpp"
#include <fstream>
#include <locale>
#include <ctime>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Util
{


	static nlohmann::json config;


	static FString MessageOfDay;
	tm DateClusterOpen;
	tm DateRewipe;

	static void ReadConfig(void);
	static void GetDates(void);


	/**
	* \brief ReadConfig
	*
	* This function reads out the json config file
	*
	* \return  nlohmann::json
	*/
	void ReadConfig(void)
	{


		const std::string config_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/MessageOfDayEditor/config.json";
		std::ifstream file{ config_path };
		if (!file.is_open())
			throw std::runtime_error("Can't open config.json");

		file >> config;

		file.close();
	}


	void GetDates(void)
	{
		std::tm t = {};
		std::istringstream temp;

		
		temp = (std::istringstream)(FString(ArkApi::Tools::Utf8Decode(config["Messages"]["SuppressPlayerJoinTribeMessage"]).c_str()).ToString());

		temp.imbue(std::locale("de_DE.utf-8"));
		temp >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
		if (temp.fail()) {
			Log::GetLog()->error("Date parse cluster open failed");
		}
		else {
			DateClusterOpen = t;
		}

		temp = (std::istringstream)(FString(ArkApi::Tools::Utf8Decode(config["General"]["DateRewipe"]).c_str()).ToString());


		temp.imbue(std::locale("de_DE.utf-8"));
		temp >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
		if (temp.fail()) {
			Log::GetLog()->error("Date parse cluster open failed");
		}
		else {
			DateRewipe = t;
		}
	}


	FString GetMessageOfDay()
	{

		return MessageOfDay;
	}

	int GetTimeRewipe()
	{

		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);

		int result = 0;

		auto tms = system_clock::from_time_t(std::mktime(&tm));

		auto tme = system_clock::from_time_t(std::mktime(&DateRewipe));

		if (tms < tme)
		{
			result = (std::chrono::duration_cast<std::chrono::hours>(tms - tme)).count();
		}


		return result;
	}

	int GetTimeDiffClusterOpen()
	{

		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);

		int result = 0;

		auto tms = system_clock::from_time_t(std::mktime(&tm));

		auto tme = system_clock::from_time_t(std::mktime(&DateClusterOpen));

		auto diff_in_hours = std::chrono::duration_cast<std::chrono::hours>(tms - tme);

		if (tms < tme)
		{
			result = (std::chrono::duration_cast<std::chrono::hours>(tms - tme)).count();
		}


		return result;
	}


	void UtilInit(void)
	{
		ReadConfig();
		MessageOfDay = FString(ArkApi::Tools::Utf8Decode(config["General"]["MessageOfDay"]).c_str());
		GetDates();
	}

}
