#ifndef MAP_HPP
#define MAP_HPP

class Rectangle;
class Entity;

enum class Terrain {
	// defines traversability/interactability with a given floor tile
	WALL,	// 0, non walkable
	FLOOR,	// 1, walkable
	NATURE, // 2, walkable w/condition
	WATER,	// 3, walkable w/condition
	TREE	// 4, non walkable
};

enum class Direction {
	NONE,	// 0, none
	UP,	// 1, up
	RIGHT, // 2, right
	DOWN,	// 3, down
	LEFT	// 4, left
};

struct Cell {
	// defines an individual cell on the map
	Terrain terrain; // the terrain in a given cell
	bool explored; // has this cell been seen/explored by the player
	Cell(): terrain(Terrain::WALL), explored(false) {}
};

class Map {
public:
	// map variables
	const char* name;
	unsigned int width, height;
	// room list

	// standard map functions
	Map(unsigned int width, unsigned int height);
	// Map(const char* name, int width, int height);
	~Map();
	void init();

	// map query/test/output functions
	int* findWalkable();
	bool isInRange(int x, int y);
	bool canMove(int x, int y);
	bool isWall(int x, int y);
	float getDistance(int x1, int y1, int x2, int y2);

	// fov functions
	bool isInFov(int x, int y); // take actor position and target
	// coordinates and return bool if in fov
	bool isExplored(int x, int y);
	bool isOpaque(int x, int y);
	void computeFov(); // void computeFov(Entity* ray_caster) eventually
	void render();

	// map generation functions
	// generation brush tools
	void paint(int x, int y, Terrain brush = Terrain::FLOOR); // paint xy with passed terrain brush
	void paintPath(int x1, int y1, int x2, int y2, Terrain brush = Terrain::FLOOR);
		// paint L path from p1:p2, with brush
	void paintRect(Rectangle rect, Terrain brush = Terrain::FLOOR, bool fill = true);
		// fill rect area with passed brush
	void paintWalk(int x, int y, Direction dir, Terrain brush = Terrain::FLOOR, int step = -1);

	// generation tools, functions, debug outputs
	void genDungeonBSP(Rectangle rect, int rmin = 2, int rmax = 8, int split_probability = 25);
	void genRandRooms(int attempts = 50);
	void genConnections();

	void flagExplorable(int x, int y, Matrix<bool>& explorable, Terrain bad_tile = Terrain::WALL);
		// modifies passed reference to cell map marking connectivity
	void boolMapRender(Matrix<bool> bool_map);
	int calcDelta(int x1, int y1, int x2, int y2);

	// debug options
	void debugRevealMap();

protected:
	// map variables, private, inaccessible except through map::functions
	Matrix<Cell*> cell_map; // contains actual map of cells/tiles
	Matrix<bool> fov_map; // contains map of tiles in vision, TRUE = in vision
};

#endif
