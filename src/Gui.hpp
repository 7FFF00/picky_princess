#ifndef GUI_HPP
#define GUI_HPP

class Gui {
public:
	int x_pos, y_pos;
	Gui();
	~Gui();
	void clear();
	void render();
	void message(const char* color, std::string text);
	void message(const char* color, std::stringstream& text_buffer);

protected:
	struct Message {
		const char* color;
		std::string text;
		Message(const char* color, std::string text);
		~Message();
	};

	std::vector<Message*> log;

	void renderBar(int x, int y, int width, float value, float max_value,
		const char* bar_color, const char* back_color);
};

#endif
