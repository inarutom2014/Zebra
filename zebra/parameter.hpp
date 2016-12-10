/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-06 10:07:11
**/

#ifndef _PARAMETER_HPP_
#define _PARAMETER_HPP_

#include <iostream>
#include <sstream>
#include <cassert>

namespace Zebra {

class Parameter
{
	public:
		Parameter() { }

		Parameter(const std::string &str, int line):str_(std::istringstream(str)), line_(line) { }

		bool eof() const {
			return str_.eof();
		}

		int FindInteger() {
			int res;
			str_ >> res;
			check();
			return res;
		}

		double FindDouble() {
			double res;
			str_ >> res;
			check();
			return res;
		}

		std::string FindString() {
			std::string s;
			str_ >> s;
			check();
			return std::move(s);
		}

		Point FindPosition() {
			std::string s;
			str_ >> s;
			assert(s == "Position");
			Point res;
			str_ >> res.x_ >> res.y_ >> res.z_;
			check();
			return res;
		}

		Vector FindVector() {
			std::string s;
			str_ >> s;
			assert(s == "Spectrum" || s == "Direction" || s == "Intensity" || s == "Normal");
			Vector res;
			str_ >> res.x_ >> res.y_ >> res.z_;
			check();
			return res;
		}

	private:

		void check() const {
			if (str_.fail()) {
				std::cerr << "line " << line_ << ": syntax error :(\n";
				exit(-1);
			}
		}

		std::istringstream str_;
		int                line_;
};

} // namespace Zebra

#endif /* _PARAMETER_HPP_ */