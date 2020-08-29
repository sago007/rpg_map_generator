#include "SagoMapGenerator.hpp"



sago::mapgenerator::SagoMapGenerator::SagoMapGenerator() {
	SagoMapGenerator(100, 100);
}

sago::mapgenerator::SagoMapGenerator::SagoMapGenerator(int sizeX, int sizeY) {
	rooms.clear();
	worldSizeX = sizeX;
	worldSizeY = sizeY;
	world = std::vector<char>(sizeX*sizeY);
	FillAll();
}

void sago::mapgenerator::SagoMapGenerator::FillAll(char standardTile) {
	for (size_t i = 0; i < world.size(); ++i) {
		world[i] = standardTile;
	}
}