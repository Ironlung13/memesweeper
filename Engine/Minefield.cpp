#include "Minefield.h"

void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height*SpriteCodex::tileSize, { 192,192,192 });	//Draws background for tiles


	Vei2 fieldpos{ 0,0 };
	for (fieldpos.x = 0; fieldpos.x < width * SpriteCodex::tileSize; fieldpos.x += SpriteCodex::tileSize)
	{
		for (fieldpos.y = 0; fieldpos.y < height * SpriteCodex::tileSize; fieldpos.y += SpriteCodex::tileSize)
		{
			switch (TileAt(fieldpos).GetState())
			{
				case State::Hidden:
					SpriteCodex::DrawTileButton(fieldpos, gfx);
					break;

				case State::Revealed:
					SpriteCodex::DrawTile0(fieldpos, gfx);
					break;

				case State::Flagged:
					SpriteCodex::DrawTileFlag(fieldpos, gfx);
					break;

				case State::Mine:
					SpriteCodex::DrawTileBomb(fieldpos, gfx);
					break;

				default:
					SpriteCodex::DrawTile0(fieldpos, gfx);
					break;
			}
		}
	}
}

const Minefield::Tile& Minefield::TileAt(const Vei2& fieldpos) const
{
	return tiles[fieldpos.x + fieldpos.y * width];
}

Minefield::Tile::Tile(std::mt19937& rng)
{
	std::uniform_int_distribution<int> chance{ 0, 5 };
	switch (chance(rng))
	{
	case 0:
		hasMine = true;
		break;
	default:
		hasMine = false;
		break;
	}
}

bool Minefield::Tile::HasMine() const
{
	return hasMine;
}

const State& Minefield::Tile::GetState() const
{
	return state;
}
