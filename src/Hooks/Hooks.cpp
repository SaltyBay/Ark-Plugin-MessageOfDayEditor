/*****************************************************************************************************************
* Copyright (C) 2019 by Matthias Birnthaler                                                                      *
*                                                                                                                *
* This file is part of the MessageOfDayEditor Plugin for Ark Server API                                           *
*                                                                                                                *
*    This program is free software : you can redistribute it and/or modify                                       *
*    it under the terms of the GNU General Public License as published by                                        *
*    the Free Software Foundation, either version 3 of the License, or                                           *
*    (at your option) any later version.                                                                         *
*                                                                                                                *
*    This program is distributed in the hope that it will be useful,                                             *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of                                              *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                                                 *
*    GNU General Public License for more details.                                                                *
*                                                                                                                *
*    You should have received a copy of the GNU General Public License                                           *
*    along with this program.If not, see <https://www.gnu.org/licenses/>.                                        *
*                                                                                                                *
*****************************************************************************************************************/

/**
* \file Hooks.cpp
* \author Matthias Birnthaler Matthias-Birnthaler@outlook.com
* \date 15 May 2019
* \brief File containing the implementation for all needed Hooks
*
*/


/* ================================================[includes]================================================ */
#include "Hooks.h"
#include "MessageOfDayEditor.h"
#include "Util.h"


/* ========================================== [local defines] =============================================== */



/* ===================================== [prototype of local functions] ======================================= */


/* ===================================== [definition of local functions] ====================================== */


/* ===================================== [definition of global functions] ===================================== */


void  Hook_AShooterGameMode_ShowMessageOfTheDay(AShooterGameMode* _this)
{
	AShooterGameMode* game_mode = ArkApi::GetApiUtils().GetShooterGameMode();

	if (nullptr != game_mode)
	{
		auto messageOfDay = Util::GetMessageOfDay();
		auto timeDiffCloudOpen = Util::GetTimeDiffClusterOpen();
		auto timeReWipe = Util::GetTimeDiffClusterOpen();

		FString  displayString = (FString::Format(*messageOfDay, (int)(timeDiffCloudOpen/24), (int)(timeDiffCloudOpen % 24), (int)(timeReWipe % 24), (int)(timeReWipe % 24)));

		game_mode->SetMessageOfTheDay(&displayString);
	}

	AShooterGameMode_ShowMessageOfTheDay_original(_this);
}

/**
* \brief Initialisation of needed Hooks
*
* This function initialise all needed Hooks
*
* \return void
*/
void InitHooks(void)
{
	ArkApi::GetHooks().SetHook("AShooterGameMode.ShowMessageOfTheDay", 
		&Hook_AShooterGameMode_ShowMessageOfTheDay, 
		&AShooterGameMode_ShowMessageOfTheDay_original);
}


/**
* \brief Cancellation of needed Hooks
*
* This function removes all needed Hooks.
*
* \return void
*/
void RemoveHooks(void)
{
	ArkApi::GetHooks().DisableHook("AShooterGameMode.ShowMessageOfTheDay", 
		&Hook_AShooterGameMode_ShowMessageOfTheDay);
}

/* =================================================[end of file]================================================= */
