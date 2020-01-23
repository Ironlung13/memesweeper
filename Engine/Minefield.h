#pragma once
#include "Graphics.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include "Mouse.h"

enum class State
{
	Uninitialized,
	Hidden,
	Revealed,
	Flagged,
	Mine
};

class Minefield
{
public:
	void Draw(Graphics& gfx);
private:
	class Tile
	{
	public:
		void OnMouseClick(Mouse& mouse);		//Reveal, flag, unflag etc.
	private:
		const Vei2 position;
		State state = State::Uninitialized;
		bool hasMine = false;
	};
private:
	static constexpr int height = 16;
	static constexpr int width = 20;
	Tile tiles[height*width];

};


