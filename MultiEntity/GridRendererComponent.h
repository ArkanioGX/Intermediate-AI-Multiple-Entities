#pragma once
#include "RenderComponent.h"
class GridRendererComponent : public RenderComponent
{
private:
	class GridComponent* gc;
public:
	GridRendererComponent(class Actor* owner, class GridComponent* gcT);
	~GridRendererComponent();

	void draw() override;
};

