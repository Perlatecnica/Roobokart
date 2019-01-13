/* Copyright (c) 2019 Perlatecnica no-profit organization
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MODES_H
#define MODES_H

// Reset MODE
#define RESET_MODE 0

// Power On Self Test & Calibration
#define POST_CAL_MODE 1

// Navigation mode: the bot navigates in the streets staying in the way
#define NAV_MODE 2

// Aligning mode: The bot has recognised a road sign
#define ROAD_SIGN_ALIGNING_MODE 3

// It manages the trafficlight
#define TRAFFICLIGHT_MANAGEMENT_MODE 4

// It manages the cross
#define CROSS_MANAGEMENT_MODE 5

// It manages the obstacles
#define OBSTACLES_MANAGEMENT_MODE 6

// NULL MODE
#define NULL_MODE -1

// Navigation mode & stop: the bot navigates in the streets staying in the way and stop on first white line
#define NAV_MODE_STOP 7

#endif
