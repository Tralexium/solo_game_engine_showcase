#include "BackgroundObject.h"
#include "ColourComponent.h"

BackgroundObject::BackgroundObject()
{
	addComponent(new RedComponent);
	addComponent(new GreenComponent);
	addComponent(new BlueComponent);
}

BackgroundObject::~BackgroundObject()
{
}

void BackgroundObject::OnUpdate(float dt)
{
}

void BackgroundObject::OnMessage(const std::string msg)
{
}
