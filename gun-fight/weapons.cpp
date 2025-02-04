#include "entities.h"

// ----------------------- WEAPON LOADED STATE ------------------
bool entities::weapon::loaded_state::fire(entities::weapon* w) {
	if (w->cooldown_ == 0) {
		w->state_.reset(nullptr);
		w->state_ = std::make_unique<unloaded_state>(unloaded_state());
		w->animation_.play_animation(); // in draw check if you are the end of the animation
		w->reset_cooldown();

		return true;
	}
	return false;

}
bool entities::weapon::loaded_state::reload(entities::weapon* w) {
	return true;
}
std::unique_ptr<entities::weapon::weapon_state> entities::weapon::loaded_state::clone() {
	return std::make_unique<entities::weapon::loaded_state>(*this);
}

// ----------------------- WEAPON UNLOADED STATE ------------------
bool entities::weapon::unloaded_state::fire(entities::weapon* w) {
	return false;
}
bool entities::weapon::unloaded_state::reload(entities::weapon* w) {
	if (w->ammo_ == 0) { return false; }
	w->ammo_ -= 1;
	w->state_.reset(nullptr);
	w->state_ = std::make_unique<entities::weapon::loaded_state>(entities::weapon::loaded_state());
	w->animation_.play_animation();
	return true;
}

std::unique_ptr<entities::weapon::weapon_state> entities::weapon::unloaded_state::clone() {
	return std::make_unique<entities::weapon::unloaded_state>(*this);

}

// ----------------------- WEAPON ACCESSORS -------------------------
int entities::weapon::get_ammo() {
	return ammo_;
}
bool entities::weapon::is_loaded() {
	auto state = state_.get();
	return dynamic_cast<entities::weapon::loaded_state*>(state) != nullptr;
}
int  entities::weapon::get_fire_rate() {
	return fire_rate_;
}
int entities::weapon::get_cooldown() {
	return cooldown_;
}

void entities::weapon::decrement_cooldown() {
	--cooldown_;
}
// ------------------- WEAPON OPERATOR OVERLOADS ------------------------------
entities::weapon& entities::weapon::operator=(const entities::weapon& other) {
	//TODO call the entity version of the operator=
	//entities::entity::operator=(other);
	ammo_ = other.ammo_;
	state_ = other.state_->clone();
	fire_rate_ = other.fire_rate_;
	cooldown_ = other.cooldown_;
	return *this;
}

//--------------------- WEAPON OTHER BEHAVIOUR ----------------------------------

//------------------- REVOLVER OVERLOADS ----------------------------------
std::shared_ptr<entities::projectile> entities::revolver::create_bullet(float x, float y, int direction) {
	if (direction == 1) {
		return std::make_shared<entities::bullet>(entities::bullet(x, y, config::BULLET_LEFT, direction));;
	}
	else {
		return std::make_shared<entities::bullet>(entities::bullet(x, y, config::BULLET_RIGHT, direction));;
	}
}
bool entities::revolver::fire() {
	return state_->fire(this);
}
bool entities::revolver::reload() {
	return state_->reload(this);
}
void entities::revolver::replenish() {
	ammo_ = config::REVOLVER_AMMO;
	state_ = std::make_unique<loaded_state>(loaded_state());
	animation_.default_frame();
	cooldown_ = 0;

}
void entities::revolver::draw(int x, int y) {
	Vector2 pos = { x,y };
	animation_.draw_frame(pos);
	if (animation_.get_play()) {
		if (not animation_.get_frame_num() < config::REVOLVER_ANIMATION_LENGTH) {
			animation_.pause_animation();
			animation_.next_animation();
		}
		else {
			animation_.next_frame();
		}
	}
}
void entities::revolver::reset_cooldown() {
	cooldown_ = fire_rate_;
}
bool entities::revolver::update(std::vector<std::shared_ptr<entity>>& entities) {
	return true;
}
bool entities::revolver::collide(entity& other) {
	return false;
}