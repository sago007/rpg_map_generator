#include <vector>

namespace sago {
namespace mapgenerator {

struct Point {
	int x = 0;
	int y = 0;
};

struct Room {
	Point upperLeft;
	Point lowerRight;

	Point getMiddle() {
		Point p;
		p.x = lowerRight.x - upperLeft.x;
		p.y = lowerRight.y - upperLeft.x;
		return p;
	}

	bool intersects( const Room& room) {
		return (upperLeft.x <= room.lowerRight.x && lowerRight.x >= room.upperLeft.x &&
            upperLeft.y <= room.lowerRight.y && room.lowerRight.y >= room.upperLeft.y);
	}
};


class SagoMapGenerator {
	std::vector<Room> rooms;
	std::vector<char> world = std::vector<char>(100*100);
	public:
	int worldSizeX = 100;
	int worldSizeY = 100;
	char getTile(int x, int y) {
		return world.at(x*worldSizeY+y);
	}

	void FillAll(char standardTile = 'X');
	SagoMapGenerator();
	SagoMapGenerator(int sizeX, int sizeY);
};


}  //namespace mapgenerator
}  //namespace sago