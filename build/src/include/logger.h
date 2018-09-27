#ifndef _LOGGER_H_INCLUDED_
#define _LOGGER_H_INCLUDED_
#include "tsu.h"
#include <string>

// Logger
// - simple create inline and load with context then use "<<" to add arguments to log.
// - Example:
//	Logger("INFO") << "Data\t" << "More Data";
class Logger {
public:
	// Constructor/Destructor
	Logger (std::string context);
	virtual ~Logger ();

	// Operator Overloads
	template <typename T>
	Logger& operator << (T rhs) {
		msg_ += tsu::ToString(rhs);
		return *this;
	};
private:
	std::string msg_;
	std::string context_;

private:
	void ToFile ();
};

#endif // LOGGER_H_INCLUDED
