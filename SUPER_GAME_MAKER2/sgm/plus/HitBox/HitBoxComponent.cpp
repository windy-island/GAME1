#include "HitBoxComponent.h"
#include "CollisionManager.h"

using namespace SGM2::plus::hitbox;

uint32_t HitBoxComponent::nextId = 1;

void HitBoxComponent::calc_collision() {
	cm.recalc_collision(*this);
}
