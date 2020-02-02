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
	void OnMouseClick(Mouse& mouse);		//Reveal, flag, unflag etc.

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
		void Init();
		bool HasMine() const;
		void SpawnMine();
		void Reveal();
		void Flag();
		const State& GetState() const;
		void SetNeighborMines(int MineCount);
		int GetNeighborMines() const;
	private:
		State state = State::Hidden;
		bool hasMine = false;
		int neighborMines = -1;
	};

private:
	const Tile& TileAt(const Vei2& fieldpos) const;
	Tile& TileAt(const Vei2& fieldpos);
	Vei2 ScreenToField(const Vei2 screenpos);
	int CountMines(const Vei2& fieldpos);

	static constexpr int height = 16;
	static constexpr int width = 20;
	static constexpr int nMines = 10;
	Tile tiles[height*width];
	bool gameover = false;

};


