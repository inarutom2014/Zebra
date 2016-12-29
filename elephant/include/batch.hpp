/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-28 18:08:25
**/

#ifndef _BATCH_HPP_
#define _BATCH_HPP_

#include <dirent.h>
#include <fstream>
#include <string>

#include "vector.hpp"
#include "matrix.hpp"

namespace Elephant {

class Batch
{
	public:
		Batch(const char *dir_name):dir_name_(std::string(dir_name)) { }

		void Load(size_t total = 0xFFFFFFFF) {
			DIR *dir = opendir(dir_name_.c_str());
			assert(dir);
			struct dirent *dirp = nullptr;
			std::string file(dir_name_);
			file += '/';
			for (size_t c = 0; (dirp = readdir(dir)) && c != total; ++c) {
				if (dirp->d_name[0] == '.') { --c; continue; }
				std::ifstream in(file + dirp->d_name);
				assert(in.is_open());
				y_.Push(dirp->d_name[0] - '0');
				Vector<uint8_t> tmp(1024);
				for (size_t i = 0; !in.eof(); ) {
					std::string str;
					in >> str;
					for (auto e : str)
						tmp[i++] = e - '0';
				}
				x_.Push(tmp);
				in.close();
			}
			closedir(dir);
			assert(x_.Shape().first == y_.size());
		}

		const Matrix<uint8_t>& X() const { return x_; }
		const Vector<uint8_t>& Y() const { return y_; }

	private:
		std::string dir_name_;
		Matrix<uint8_t> x_;
		Vector<uint8_t> y_;
};

} // namespace Elephant

#endif /* _BATCH_HPP_ */