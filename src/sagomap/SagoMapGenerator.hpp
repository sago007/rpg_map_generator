#include <vector>

namespace sago {
namespace mapgenerator {

struct Point {
	int x = 0;
	int y = 0;
	Point() = default;
	Point(int x, int y) : x(x),y(y) {}
	void set_value(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

struct Room {
	Point upperLeft;
	Point lowerRight;

	Point getMiddle() const {
		Point p;
		p.x = (lowerRight.x + upperLeft.x) / 2;
		p.y = (lowerRight.y + upperLeft.y) / 2;
		return p;
	}

	bool intersects( const Room& room) {
		return (upperLeft.x <= room.lowerRight.x && lowerRight.x >= room.upperLeft.x &&
            upperLeft.y <= room.lowerRight.y && room.lowerRight.y >= room.upperLeft.y);
	}

	bool isTouching(const Room& room) {
		return (upperLeft.x <= room.lowerRight.x + 1 && lowerRight.x + 1 >= room.upperLeft.x &&
            upperLeft.y <= room.lowerRight.y + 1 && lowerRight.y + 1 >= room.upperLeft.y) && !intersects(room);
	}
};


class SagoMapGenerator {
	std::vector<Room> rooms;
	std::vector<char> world = std::vector<char>(100*100);
	public:
	int worldSizeX = 100;
	int worldSizeY = 100;
	int minRoomSize = 10;
	int maxRoomSize = 30;
	int maxNumberOfRooms = 20;
	char getTile(int x, int y) {
		return world.at(x*worldSizeY+y);
	}

	void FillAll(char standardTile = 'X');
	SagoMapGenerator();
	SagoMapGenerator(int sizeX, int sizeY);
	void placeRoom();
	void carveRooms();
	void placeRooms();
	void generatePaths();
	void carvePath(Point from, Point to);
	bool areRoomsConnected(size_t roomIndex1, size_t roomIndex2, const std::vector<std::vector<bool>>& connected) const;
	double getDistanceBetweenRooms(const Room& room1, const Room& room2) const;
};


}  //namespace mapgenerator
}  //namespace sago