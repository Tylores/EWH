// =====================================================================================
// Copyright (c) 2016, Electric Power Research Institute (EPRI)
// All rights reserved.
//
// libcea2045 ("this software") is licensed under BSD 3-Clause license.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// *  Neither the name of EPRI nor the names of its contributors may
//    be used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
//
// This EPRI software incorporates work covered by the following copyright and permission
// notices. You may not use these works except in compliance with their respective
// licenses, which are provided below.
//
// These works are provided by the copyright holders and contributors "as is" and any express or
// implied warranties, including, but not limited to, the implied warranties of merchantability
// and fitness for a particular purpose are disclaimed.
//
// This software relies on the following libraries and licenses:
//
// #########################################################################################
// Boost Software License, Version 1.0
// #########################################################################################
//
// * catch++ v1.2.1 (https://github.com/philsquared/Catch)
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// #########################################################################################
// MIT Licence
// #########################################################################################
//
// * easylogging++ Copyright (c) 2017 muflihun.com
//   https://github.com/easylogging/easyloggingpp
//   https://easylogging.muflihun.com
//   https://muflihun.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
/*
 * main.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: dupes
 */

// INCLUDE
#include <iostream>     // cout, cin
#include <thread>       // thread, join
#include <chrono>       // now, duration
#include <map>
#include <string>
#include <vector>

#include "include/electric_water_heater.h"
#include "include/easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

bool done = false;  //signals program to stop

// Help
// - CLI interface description
void Help () {
    cout << "\n\t[Help Menu]\n\n";
    cout << "> q		quit\n";
    cout << "> h		display help menu\n";
    cout << "> i 		import power\n";
    cout << "> s 		shed\n";
    cout << "> p        	print properties\n" << endl;
} // end Help

// Command Line Interface
// - method to allow user controls during program run-time
static bool CommandLineInterface (const string& input, 
                                  ElectricWaterHeater *EWH) {
    // check for program argument
    if (input == "") {
        return false;
    }
    char cmd = input[0];

    // deliminate input string to argument parameters
    vector <string> tokens;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }

    switch (cmd) {
        case 'q':
           return true;

        case 'i': {
            try {
                EWH->SetImportWatts(4500);
            } catch(...) {
                cout << "[ERROR]: Invalid Argument." << endl;
            }
            return false;
        }

        case 's': {
            try {
                EWH->SetImportWatts(0);
            } catch(...) {
                cout << "[ERROR]: Invalid Argument." << endl;
            }
            return false;
        }

        case 'p': {
            EWH->Print ();
            return false;
        }

        default: {
            Help();
            return false;
        }
    }
}  // end Command Line Interface

void ControlLoop (ElectricWaterHeater *EWH) {
	unsigned int time_remaining, time_past;
    unsigned int time_wait = 1000;
    auto time_start = chrono::high_resolution_clock::now();
    auto time_end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_elapsed;

    while (!done) {
        time_start = chrono::high_resolution_clock::now();
            // time since last control call;
            time_elapsed = time_start - time_end;
            time_past = time_elapsed.count();
            EWH->Loop();
        time_end = chrono::high_resolution_clock::now();
        time_elapsed = time_end - time_start;

        // determine sleep duration after deducting process time
        time_remaining = (time_wait - time_elapsed.count());
        time_remaining = (time_remaining > 0) ? time_remaining : 0;
        this_thread::sleep_for (chrono::milliseconds (time_remaining));
    }
}  // end Control Loop

int main() {
	el::Configurations conf("../data/easy_logger.conf");
	// Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(conf);

	ElectricWaterHeater *ewh_ptr = new ElectricWaterHeater ();
	thread EWH (ControlLoop, ewh_ptr);

    Help ();
    string input;
    while (!done) {
        getline(cin, input);
        done = CommandLineInterface(input, ewh_ptr);
    }

	delete ewh_ptr;
	return 0;
}
