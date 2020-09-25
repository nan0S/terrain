#include "App.hpp"
#include <filesystem>

namespace fs = std::filesystem;

std::string formatErrorStr()
{
	return "Format error!\n\n"
		  "Valid format example: ./terrain ../data/eurasia/ -lat 50 51 -lon 15 16\n"
		  "Where ../data/eurasia/ is the directory with terrain data,\n"
		  "and -lat 50 51 -lon 15 16 are the ranges of latitude and longitude respectively -\n"
		  "- [50, 51) latitude and [15, 16) longitude.\n\n"
		  "Positive latitude number mean north (N), negative south (S).\n"
		  "Positive longitude number mean east (E), negative west (W).\n";
}

int main(int argc, char* argv[])
{
	if (argc < 8)
		return std::cout << formatErrorStr(), 0;

	std::string dir(argv[1]);
	if (dir.back() != '/')
		dir.push_back('/');
	
	if (!fs::exists(dir))
		return std::cout << "Given directory does not exists!\n", 0;
	if (!fs::is_directory(dir))
		return std::cout << "Given path is not a catalog!\n", 0;

	if (std::string(argv[2]) != "-lat" || std::string(argv[5]) != "-lon")
		return std::cout << formatErrorStr(), 0;

	int sz_min = std::stoi(std::string(argv[3]));
	int sz_max = std::stoi(std::string(argv[4]));

	if (sz_min > sz_max)
		std::swap(sz_min, sz_max);

	int dl_min = std::stoi(std::string(argv[6]));
	int dl_max = std::stoi(std::string(argv[7]));

	if (dl_min > dl_max)
		std::swap(dl_min, dl_max);	

	App app;
	app.start(dir, sz_min, sz_max, dl_min, dl_max);

	return 0;
}
