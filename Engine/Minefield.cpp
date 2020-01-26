#include "Minefield.h"

Minefield::Minefield(std::mt19937& rng)
{
	std::uniform_int_distribution<int> pos_x(0, width);
	std::uniform_int_distribution<int> pos_y(0, height);
	Vei2 fieldpos;
	
	for (int n = 0; n < nMines; n++)
	{
		do {
			fieldpos = { pos_x(rng), pos_y(rng) };
		} while (TileAt(fieldpos).HasMine());

		TileAt(fieldpos).SpawnMine();
	}
}

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
			case Minefield::Tile::State::Hidden:
				SpriteCodex::DrawTileButton(fieldpos, gfx);
				break;

			case Minefield::Tile::State::Revealed:
				SpriteCodex::DrawTile0(fieldpos, gfx);
				break;

			case Minefield::Tile::State::Flagged:
				SpriteCodex::DrawTileButton(fieldpos, gfx);
				SpriteCodex::DrawTileFlag(fieldpos, gfx);
				break;

			case Minefield::Tile::State::Mine:
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

Minefield::Tile& Minefield::TileAt(const Vei2& fieldpos)
{
	return tiles[fieldpos.x + fieldpos.y * width];
}


bool Minefield::Tile::HasMine() const
{
	return hasMine;
}

void Minefield::Tile::SpawnMine()
{
	hasMine = true;
}

const Minefield::Tile::State& Minefield::Tile::GetState() const
{
	return state;
}
