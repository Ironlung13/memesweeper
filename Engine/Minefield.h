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
	enum class State
	{
		Game_On,
		Game_Over,
		Win
	};

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
	void RevealNearbyTiles(const Vei2& fieldpos);
	void CheckForWin();
	int CorrectFlags = 0;

	static constexpr int height = 16;
	static constexpr int width = 20;
	static constexpr int nMines = height * width / 10;
	static constexpr int screen_left = (Graphics::ScreenWidth / 2) - SpriteCodex::tileSize * width / 2;
	static constexpr int screen_top = (Graphics::ScreenHeight / 2) - SpriteCodex::tileSize * height / 2;
	Tile tiles[height*width];
	State state = State::Game_On;
	

};


