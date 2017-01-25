/**
 *    > Author:            UncP
 *    > Mail:         770778010@qq.com
 *    > Github:    https://www.github.com/UncP/Elephant
 *    > Created Time:  2016-12-06 09:53:07
**/

#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "bsdf.hpp"
#include "object.hpp"
#include "light.hpp"
#include "primitive.hpp"
#include "scene.hpp"
#include "parameter.hpp"

namespace Zebra {

class Parser
{
	public:

		Parser(const char *file):line_(1) {
			std::ifstream in(file);
			if (!in.is_open()) {
				std::cerr << "file " << file << " not existed :(\n";
				exit(-1);
			}
			std::string str;
			for (; !in.eof() || str.size(); ++line_, str.clear(), std::getline(in, str)) {
				if (!str.size() || str.find("#") != std::string::npos)
					continue;
				parameter_ = Parameter(str, line_);
				std::string s(parameter_.FindString());
				if (s == "Object") FindObject();
				else if (s == "Light") FindLight();
				else {
					std::cerr << "line: " << line_ << ": syntax error :(\n";
					exit(-1);
				}
				assert(parameter_.eof());
			}
		}

		void FindObject() {
			std::string o(parameter_.FindString());
			const Object *object = nullptr;
			if (o == "Sphere")
				object = NewSphere(parameter_);
			else if (o == "Plane")
				object = NewPlane(parameter_);
			else {
				std::cerr << "line: " << line_ << ": syntax error :(\n";
				exit(-1);
			}
			std::string s(parameter_.FindString());
			const BSDF *bsdf = nullptr;
			if (s == "Diffuse") bsdf = NewDiffuseBSDF(parameter_);
			else if (s == "Reflect") bsdf = NewReflectBSDF(parameter_);
			else if (s == "Refract") bsdf = NewRefractBSDF(parameter_);
			else {
				std::cerr << "line: " << line_ << ": syntax error :(\n";
				exit(-1);
			}
			primitives_.push_back(std::shared_ptr<Primitive>(new Primitive(object, bsdf)));
		}

		void FindLight() {
			std::string s(parameter_.FindString());
			std::shared_ptr<Light> light = nullptr;
			if (s == "Point") {
				light = std::shared_ptr<Light>(NewPointLight(parameter_));
			} else if (s == "Area") {
				AreaLight *area_light = NewAreaLight(parameter_);
				light = std::shared_ptr<Light>(area_light);
				primitives_.push_back(std::shared_ptr<Primitive>(
					new Primitive(area_light->object_, nullptr, area_light)));
			} else {
				std::cerr << "line: " << line_ << ": syntax error :(\n";
				exit(-1);
			}
			lights_.push_back(light);
		}

		Scene GetScene() const {
			assert(primitives_.size() && lights_.size());
			std::vector<Primitive *> primitives;
			std::vector<Light *> lights;
			for (auto e : primitives_)
				primitives.push_back(e.get());
			for (auto e : lights_)
				lights.push_back(e.get());
			return Scene(primitives, lights);
		}

	private:
		int                                     line_;
		Parameter                               parameter_;
		std::vector<std::shared_ptr<Light>>     lights_;
		std::vector<std::shared_ptr<Primitive>> primitives_;
};

} // namespace Zebra

#endif /* _PARSER_HPP_ */