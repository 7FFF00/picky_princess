#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

struct Rectangle {
	//	rectangle width, height, top left x pos, top left y pos
	unsigned int width, height, x, y;
	//	x and y are the absolute x/y relative to the containing map
	//	use rect.x + x to solve for relative within frame of rectangle

	Rectangle() {};
	Rectangle(unsigned int width, unsigned int height, unsigned int x = 0, unsigned int y = 0):
		width(width), height(height), x(x), y(y) {
	}
};

#endif
