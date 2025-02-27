#pragma once
#include "entities.h"
#include <tuple>
class player{
public:
	~player() = default;
	//TODO cant clone a nullptr
	player(std::shared_ptr<entities::gunman> gunman, std::shared_ptr<entities::weapon> weapon, std::map<int, Vector2>& movement_keys, std::pair<int, int>& fire_reload_keys, int item_key, int  draw_x, const char* win_path)
		: gunman_(gunman), weapon_(std::move(weapon)),
		item_(std::make_unique<entities::empty_pickup>(entities::empty_pickup(0.0,0.0, config::DEFAULT_PATH))), movement_(movement_keys), fire_reload_(fire_reload_keys), item_use_(item_key), score_(0), draw_x_(draw_x) {
		player_start_pos_ = gunman_->get_position();
		heart_ = animation(config::HEART_PATH, config::HEART_WIDTH, config::HEART_HEIGHT);
		armour_ = animation(config::ARMOUR_PATH, config::HEART_WIDTH, config::HEART_HEIGHT);
		win_ = animation(win_path, config::WIN_WIDTH, config::WIN_HEIGHT);
	};
	player(const player& other)
		: gunman_(other.gunman_), weapon_(other.weapon_),
		item_(other.item_), score_(other.score_), player_start_pos_(other.player_start_pos_),
		movement_(other.movement_), fire_reload_(other.fire_reload_), item_use_(other.item_use_), draw_x_(other.draw_x_), heart_(other.heart_), armour_(other.armour_),win_(other.win_){};

	player& operator=(const player& other);
	// get player gunman
	std::shared_ptr<entities::gunman> get_gunman();
	// get player weapon
	std::shared_ptr<entities::weapon> get_weapon();
	// get player item
	std::shared_ptr<entities::pickup> get_item();
	// update player
	bool update_player(std::vector<std::shared_ptr<entities::entity>>& entities);
	void pickup_item(std::vector<std::shared_ptr<entities::entity>>& entities);
	// draw player
	void draw_player();
	void draw_win();
	// increase_score
	void increase_score();
	// is dead
	bool is_dead();
	// player reset
	void reset_player();

	int get_score();
	void set_score(int score);
	float get_draw_x();
private:
	
	/** entity components */
	std::shared_ptr<entities::gunman> gunman_;
	std::shared_ptr<entities::weapon> weapon_;
	std::shared_ptr<entities::pickup> item_;
	
	/** tracking information for drawing and round management */
	int score_;
	Vector2 player_start_pos_;
	float draw_x_;
	
	/** player controls */
	std::map<int, Vector2>& movement_;
	std::pair<int, int>& fire_reload_;
	int item_use_;

	/** animaations */
	animation heart_;
	animation armour_; 
	animation win_;
};

