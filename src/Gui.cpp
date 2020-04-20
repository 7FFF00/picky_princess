#include <vector>	// for handling vector list of message log
#include <string>	// for ease of use regarding message/text handling
#include <cstring> // for access to strlen for parsing c style color strings
#include "Gui.hpp"
#include "Ai.hpp"
#include "Engine.hpp"
#include "Entity.hpp"
#include "Stats.hpp"
#include "BearLibTerminal.h"
#include <cmath>	// for round function in bar percentage math
#include <iostream>
#include <sstream> // for taking a string stream to act as a text buffer, easy parsing of var

Gui::Gui() : x_pos(4), y_pos((int)engine.window_height - (int)engine.ui_height) {

}

Gui::~Gui() {

}

void Gui::clear() {
	log.clear();
}

Gui::Message::Message(const char* color, std::string text) : color(color), text(text) {
}

Gui::Message::~Message() {
   	// free(text);
}

void Gui::render() {
	terminal_layer(0); // gui has to be rendered on layer 0 for bg manipulation

	// Background Color for primary UI
	terminal_bkcolor("#ffd1dc");
	int step = 2;
	bool color_swap = false;
	for (int i = 0; i < (int)engine.window_width; i+= (step * 2)) {
		for (int j = (int)engine.window_height - (int)engine.ui_height;
			j < (int)engine.window_height; j+= step) {
			for (int k = i; k < (i + (step * 2)); k++) {
				for (int l = j; l < (j + step); l++) {
					if (color_swap) { terminal_bkcolor("#ffd1dc"); } // lighter
					else { terminal_bkcolor("#ffc0ca"); } // darker
					terminal_put(k, l, ' ');
				}
			}
			color_swap = !color_swap;
		}
		// color_swap = !color_swap;
	}

	// RECIPE LIST (Background Color)
	int note_x = (int)engine.window_width - (int)engine.ui_width;
	int note_x_max = (int)engine.window_width;
	int note_y = 0;
	int note_y_max = (int)engine.window_height - (int)engine.ui_height;
	int note_start_x = note_x + 4;
	int note_center_x = note_start_x + 8;
	int note_start_y = note_y + 5;
	terminal_bkcolor("#fffdd0"); // Cream
	for (int i = note_x; i < note_x_max; i++) {
		for (int j = note_y; j < note_y_max; j++) {
			terminal_layer(0);
			terminal_put(i, j, ' ');
			if (i == note_x + 3) {
				// Vertical Margin
				terminal_color("#ffd1dc");
				terminal_layer(1);
				terminal_put(i, j, '|');
			}
			if (j > 4 && ((j + 1) % 2) == 0 && j != (note_y_max - 1)) {
				// Horizontal Margin
				terminal_layer(2);
				terminal_color("#aec6cf");
				terminal_put(i, j, '_');
			}
		}
	}

	// RECIPE NOTE (Text on line)
	terminal_layer(3); // to be above page line formatting
	terminal_color("#f02400"); // scarlet
	terminal_print(note_center_x, note_start_y, "RECIPE");

	terminal_color("#2400f0"); // not really purple
	std::string prep_timer = "Prep Time: ";
	if (engine.turn_count > 59) {
		int hours = engine.turn_count / 60;
		int minutes = engine.turn_count % 60;
		prep_timer += std::to_string(hours) + "h" + std::to_string(minutes) + "m";
	}
	else {
		prep_timer += std::to_string(engine.turn_count) + "m";
	}
	terminal_print(note_start_x, note_start_y + 2, prep_timer.c_str());

	// RECIPE NOTE (Ingredient List)
	terminal_layer(3);
	terminal_color("#606080");
	Entity* player_item = engine.actors[0]->child;
	for (int i = 0; player_item != NULL; i++) {
		terminal_print(note_start_x, note_start_y + 4 + (2 * i), player_item->name);
		player_item = player_item->child;
	}

	// Player Name/Title Display
	terminal_layer(1);
	std::string player = engine.player->name;
	if (engine.player->title) {
		player += ", ";
		player += engine.player->title;
	}
	terminal_color("#3150d1");
	terminal_print(x_pos, y_pos + 1, player.c_str());

	// BAR Displays
	std::string hp, patience, cute, spacing_justify,
		bump_mode_select, bump_mode_deselect, bump_mode_deselect2;
	int spacing_count;

	// HP Display
	renderBar(x_pos, y_pos + 3, 20, (float)engine.player->stats->hp,
		(float)engine.player->stats->max_hp, "#E05050", "#803030");
	terminal_layer(1); // overlay text placed on layer above background color bars
	hp = "HEART: " + std::to_string(engine.player->stats->hp) +
		"/" + std::to_string(engine.player->stats->max_hp);
	spacing_count = 20 - hp.size();
	spacing_justify = "";
	for (int i = 0; i < spacing_count; i++) { spacing_justify += " ";	}
	hp = "HEART: " + spacing_justify + std::to_string(engine.player->stats->hp) +
		"/" + std::to_string(engine.player->stats->max_hp);
	terminal_color("#303050");
	terminal_print(x_pos, y_pos + 3, hp.c_str());

	// SPICE Display, aka Stamina
	renderBar(x_pos, y_pos + 4, 20, (float)engine.player->stats->patience,
		(float)engine.player->stats->max_patience, "#ffb357", "#df9337");
	terminal_layer(1); // overlay text placed on layer above background color bars
	patience = "SPICE: " + std::to_string(engine.player->stats->patience) +
		"/" + std::to_string(engine.player->stats->max_patience);
	spacing_count = 20 - patience.size();
	spacing_justify = "";
	for (int i = 0; i < spacing_count; i++) { spacing_justify += " ";	}
	patience = "SPICE: " + spacing_justify + std::to_string(engine.player->stats->patience) +
		"/" + std::to_string(engine.player->stats->max_patience);
	terminal_color("#303050");
	terminal_print(x_pos, y_pos + 4, patience.c_str());

	// CUTE Display
	renderBar(x_pos, y_pos + 5, 20, (float)engine.player->stats->cute,
		(float)engine.player->stats->max_cute, "#fdfd96", "#cdcd66");
	terminal_layer(1); // overlay text placed on layer above background color bars
	cute = "CUTE: " + std::to_string(engine.player->stats->cute) +
		"/" + std::to_string(engine.player->stats->max_cute);
	spacing_count = 20 - cute.size();
	spacing_justify = "";
	for (int i = 0; i < spacing_count; i++) { spacing_justify += " ";	}
	cute = "CUTE: " + spacing_justify + std::to_string(engine.player->stats->cute) +
		"/" + std::to_string(engine.player->stats->max_cute);
	terminal_color("#303050");
	terminal_print(x_pos, y_pos + 5, cute.c_str());

	// MODE Display
	// UPDATE THIS TO PROPERLY DISPLAY CURRENT BUMP MODE
	// BUMP modes are CHOP STIR HEAT ICE EAT (TRANSFORM OPTIONS)
	if (engine.player->ai->mode == 0) {
		// GRAB
		terminal_print(x_pos, y_pos + 7,
			"[color=#4160e1]Grab[color=#777696] / C / S / H / E");
	}
	else if (engine.player->ai->mode == 1) {
		// CHOP
		terminal_print(x_pos, y_pos + 7,
			"[color=#777696]G / [color=#4160e1]Chop[color=#777696] / S / H / E");
	}
	else if (engine.player->ai->mode == 2) {
		// STIR
		terminal_print(x_pos, y_pos + 7,
			"[color=#777696]G / C / [color=#4160e1]Stir[color=#777696] / H / E");
	}
	else if (engine.player->ai->mode == 3) {
		// HEAT
		terminal_print(x_pos, y_pos + 7,
			"[color=#777696]G / C / S / [color=#4160e1]Heat[color=#777696] / E");
	}
	else if (engine.player->ai->mode == 4) {
		// EAT
		terminal_print(x_pos, y_pos + 7,
			"[color=#777696]G / C / S / H / [color=#4160e1]Eat");
	}

	// MESSAGE LOG (Background)
	int log_x = x_pos + 50;
	terminal_layer(1);
	terminal_color("#80FFFFFF"); // ARGB
	for (int i = 0; i < (int)engine.ui_height - 2; i++) {
		for (int j = 0; j < (int)engine.log_width + 2; j++) {
			// printing unicode full block
			terminal_print(log_x + j, y_pos + i + 1, "[U+2588]");
		}
	}
	terminal_color("#FFFFFFFF"); // ARGB, reset to normal alpha/color output

	// MESSAGE LOG (Log Text)
	terminal_layer(2);
	if (log.size() > 0) {
		for (int i = 0; i < (int)log.size() && i < (int)engine.ui_height - 2; i++) {
			// print from end, treating vector as queue

			// parse color out of passed line
			std::string color_base = log[(int)log.size() - i - 1]->color;
			std::string color_mod = "#";
			int color_parser_index = (int)strlen(log[(int)log.size() - i - 1]->color) - 6;

			// mod transparency based on line placement
			switch (i) {
				case 0: color_mod += "FF"; break; // line 1, aka most recent line
				case 1: color_mod += "DF"; break; // line 2
				case 2: color_mod += "CF"; break; // line 3
				case 3: color_mod += "BF"; break; // line 4
				case 4: color_mod += "AF"; break; // line 5
				case 5: color_mod += "9F"; break; // line 6
				case 6: color_mod += "8F"; break; // line 7
				default: color_mod += "7F"; break; // default is strongest transparency
			}
			color_mod += color_base.substr(color_parser_index);

			// print based on line placement and using modified string transparency level for order
			terminal_color(color_mod.c_str());
			terminal_print(log_x, y_pos + i + 1, log[(int)log.size() - i - 1]->text.c_str());
		}
	}


	// AVATAR DISPLAY
	int avatar_x = x_pos + 27;
	terminal_color("#FFFFFFFF"); // ARGB, reset to normal alpha/color output
	terminal_print(avatar_x, y_pos, "\uE100");

	// resetting engine colors
	terminal_color(engine.default_color);
	terminal_bkcolor(engine.default_bgcolor);
}

void Gui::message(const char* color, std::string text) {
	if ((int)text.size() <= (int)engine.log_width) {
		// if passed log message is within accepted width of message, send to log
		log.push_back(new Message(color, text));
	}
	else {
		while((int)text.size() > (int)engine.log_width) {
			std::string backend_of_text = " ";
			int backend_remainder = (int)text.size() % engine.log_width;
			if (backend_remainder == 0) { backend_of_text += text.substr(text.size() - engine.log_width); }
			else { backend_of_text += text.substr(text.size() - backend_remainder); }
			text.erase(text.size() - backend_of_text.size() + 1);
			log.push_back(new Message(color, backend_of_text));
		}
		log.push_back(new Message(color, text));
	}
}

void Gui::message(const char* color, std::stringstream& text_buffer) {
	// for c style string concatenation
	message(color, text_buffer.str());
	text_buffer.str(std::string());
}

void Gui::renderBar(int x, int y, int width, float value, float max_value,
	const char* bar_color, const char* back_color) {
		float percent;
		terminal_layer(0); // need to be on layer 0 to manipulate background color
		terminal_bkcolor(bar_color);
		for (int i = 0; i < width; i++) {
			percent = value / max_value;
			if (i < std::round(percent * width)) {
				terminal_bkcolor(bar_color);
			}
			else {
				terminal_bkcolor(back_color);
			}
			terminal_put(x + i, y, ' ');
		}
}
