#include <iostream>
#include <boost/program_options.hpp>
#include "sagomap/SagoMapGenerator.hpp"

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

int main(int argc, const char* argv[]) {
	boost::program_options::options_description desc("Options");
	desc.add_options()
	("version", "Print version information and quit")
	("help,h", "Print basic usage information to stdout and quit")
	//("somestring", boost::program_options::value<std::string>(), "A string to print")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "rpg_map_generator " << VERSIONNUMBER << "\n";
		return 0;
	}
	sago::mapgenerator::SagoMapGenerator smg;
	smg.placeRooms();
	smg.carveRooms();
	//smg.FillAll();
	for (int i = 0; i < smg.worldSizeX; ++i) {
		for (int j = 0; j < smg.worldSizeY; ++j) {
			std::cout << smg.getTile(i, j);
		}
		std::cout << "\n";
	}
	return 0;
}
