// Author: Tylor Slay
// Project: ECE510 - EV project
// Description: These are functions I have build using lots of other resources
// to help with importing, export, and manipulating data. I have tried to follow
// Google's coding standard to make things easier to read and understand.
//
// Google C++ coding standard
// --------------------------
// https://google.github.io/styleguide/cppguide.html
//
// General rules:
// - Indents are two spaces. No tabs should be used anywhere.
// - Each line must be at most 80 characters long.
// - Comments can be // or /* but // is most commonly used.
// - Avoid Abbreviations
// - Type/Function Names: CamelCase
// - Variable Names: lowercase_underscores
// - Class Data Members: trailing_underscores_
// - Constant Names: kName
// - Enumerator/Macro Names: ALL_CAPS
//
// Note: Please reference http://en.cppreference.com/w/cpp for additional help

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

// INCLUDE
#include <iostream>     // cin, cout
#include <fstream>
#include <string>       // getline, stoi, stod
#include <chrono>	//
#include <ctime>        // strftime, time_t, tm

// MACROS
#define DEBUG(x) std::cout << x << std::endl

// tylor slay utilities (tsu)
namespace tsu {


class Logger {
public:
	// mamber variables
	std::string msg_;

public:
	Logger (std::string context) {
		std::string date_time = " 15:45:30";
		msg_ = date_time + '\t' + context + '\t';
	}
	~Logger () {
		ToFile ();	
	}
	void ToFile  () {
    		std::ofstream output_file("ewh-tester.log", std::ios_base::app);
	   	if (output_file.is_open()) {
			output_file << msg_ << '\n';
		}
		output_file.close();
	}
	template <typename T>
	std::string ToString (T t_value) {
	    std::ostringstream ss;
	    ss << t_value;
	    return ss.str();
	} // end ToString

	template <typename T>
	Logger& operator << (T rhs) {
		msg_ += ToString( rhs);
		return *this;
	}
};

// Get Time
// * convert UTC time in seconds to date/time readable
static std::string GetTime (const unsigned int kEpoch) {
    time_t time_utc = kEpoch;
    struct tm ts;
    char buf[80];
    ts = *localtime(&time_utc);
    strftime(buf, sizeof(buf), "%D %T", &ts);
    std::string date_time = buf;
    return date_time;
} // end EpochToReadable

// Get Seconds
unsigned int GetSeconds () {
    time_t now = time(0); 	// store current time
    struct tm time = *localtime(&now);
    return time.tm_sec;
}  // end Get Seconds

// Get Minutes
unsigned int GetMinutes () {
    time_t now = time(0); 	// store current time
    struct tm time = *localtime(&now);
    return time.tm_min;
}  // end Get Minutes

// Get Hours
unsigned int GetHours () {
    time_t now = time(0); 	// store current time
    struct tm time = *localtime(&now);
    return time.tm_hour;
}  // end Get Hours

} // end namespace tsu

#endif // UTILITY_H_INCLUDED