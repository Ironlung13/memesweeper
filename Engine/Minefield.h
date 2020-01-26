#pragma once
#include "Graphics.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include "Mouse.h"



class Minefield
{
public:
	Minefield(std::mt19937& rng);
	void Draw(Graphics& gfx);
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Revealed,
			Flagged,
			Mine
		};
	public:
		void OnMouseClick(Mouse& mouse);		//Reveal, flag, unflag etc.
		bool HasMine() const;
		void SpawnMine();
		const State& GetState() const;
	private:
		State state = State::Hidden;
		bool hasMine = false;
	};

private:
	const Tile& TileAt(const Vei2& fieldpos) const;
	Tile& TileAt(const Vei2& fieldpos);
	static constexpr int height = 16;
	static constexpr int width = 20;
	static constexpr int nMines = 10;
	Tile tiles[height*width];

};


