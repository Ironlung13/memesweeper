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
	Minefield(std::mt19937& rng);
	void Draw(Graphics& gfx);
private:
	class Tile
	{
	public:
		Tile(std::mt19937& rng);

		void OnMouseClick(Mouse& mouse);		//Reveal, flag, unflag etc.
		bool HasMine() const;
		const State& GetState() const;
	private:
		State state = State::Uninitialized;
		bool hasMine = false;
	};

private:
	const Tile& TileAt(const Vei2& fieldpos) const;
	static constexpr int height = 16;
	static constexpr int width = 20;
	Tile tiles[height*width];

};


