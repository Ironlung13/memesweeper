#include "Minefield.h"
#include <algorithm>

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

	for (fieldpos.x = 0; fieldpos.x < width; fieldpos.x++)
		for (fieldpos.y = 0; fieldpos.y < height; fieldpos.y++)
		{
			if (!TileAt(fieldpos).HasMine())
			{
				TileAt(fieldpos).SetNeighborMines(CountMines(fieldpos));
			}
		}

}

void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(screen_left, screen_top, screen_left + width * SpriteCodex::tileSize, screen_top + height*SpriteCodex::tileSize, { 192,192,192 });	//Draws background for tiles

	Vei2 screenpos{ screen_left,screen_top };
	for (screenpos.x = screen_left; screenpos.x < screen_left + width * SpriteCodex::tileSize; screenpos.x += SpriteCodex::tileSize)
	{
		for (screenpos.y = screen_top; screenpos.y < screen_top + height * SpriteCodex::tileSize; screenpos.y += SpriteCodex::tileSize)
		{
			switch (TileAt(ScreenToField(screenpos)).GetState())
			{
			case Minefield::Tile::State::Hidden:
				if (!(state == State::Game_Over))
				{
					SpriteCodex::DrawTileButton(screenpos, gfx);
					break;
				}
				else 
				{
					if (TileAt(ScreenToField(screenpos)).HasMine())
					{
						SpriteCodex::DrawTileBomb(screenpos, gfx);
						break;
					}
					else
					{
						SpriteCodex::DrawTileButton(screenpos, gfx);
						break;
					}
				}

			case Minefield::Tile::State::Revealed:
				if (!(state == State::Game_Over))
				{
					if (!TileAt(ScreenToField(screenpos)).HasMine())
					{
						SpriteCodex::DrawTileNumber(screenpos, TileAt(ScreenToField(screenpos)).GetNeighborMines(), gfx);
						break;
					}
				}
				else
				{
					if (TileAt(ScreenToField(screenpos)).HasMine())
					{
						SpriteCodex::DrawTileBomb(screenpos, gfx);
						break;
					}
					else
					{
						SpriteCodex::DrawTileNumber(screenpos, TileAt(ScreenToField(screenpos)).GetNeighborMines(), gfx);
						break;
					}
				}

			case Minefield::Tile::State::Flagged:
				if (state == State::Game_On)
				{
					SpriteCodex::DrawTileButton(screenpos, gfx);
					SpriteCodex::DrawTileFlag(screenpos, gfx);
				}
				else 
				{
					if (TileAt(ScreenToField(screenpos)).HasMine())
					{
						SpriteCodex::DrawTileBomb(screenpos, gfx);
						SpriteCodex::DrawTileFlag(screenpos, gfx);
					}
					else
					{
						SpriteCodex::DrawTileButton(screenpos, gfx);
						SpriteCodex::DrawTileFlag(screenpos, gfx);
						SpriteCodex::DrawTileCross(screenpos, gfx);
					}
				}
				break;

			case Minefield::Tile::State::Mine:
				SpriteCodex::DrawTileBombRed(screenpos, gfx);
				break;

			default:
				SpriteCodex::DrawTile0(screenpos, gfx);
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

Vei2 Minefield::ScreenToField(const Vei2 screenpos)
{
	return Vei2{ (screenpos.x - screen_left) / SpriteCodex::tileSize,(screenpos.y - screen_top) / SpriteCodex::tileSize };
}

int Minefield::CountMines(const Vei2& fieldpos)
{
	const int low_x = std::max(0, fieldpos.x - 1);
	const int high_x = std::min(width - 1, fieldpos.x + 1);
	const int low_y = std::max(0, fieldpos.y - 1);
	const int high_y = std::min(height - 1, fieldpos.y + 1);


	int Mines = 0;
	for (int x = low_x; x <= high_x; ++x)
		for (int y = low_y; y <= high_y; ++y)
		{
			if (TileAt({ x,y }).HasMine())
				Mines++;
		}
	return Mines;
}

void Minefield::RevealNearbyTiles(const Vei2& fieldpos)
{
	const int low_x = std::max(0, fieldpos.x - 1);
	const int high_x = std::min(width - 1, fieldpos.x + 1);
	const int low_y = std::max(0, fieldpos.y - 1);
	const int high_y = std::min(height - 1, fieldpos.y + 1);
	
	Vei2 pos;
	for (pos.x = low_x; pos.x <= high_x; pos.x++)
		for (pos.y = low_y; pos.y <= high_y; pos.y++)
		{
			if (TileAt(pos).GetNeighborMines() == 0 && TileAt(pos).GetState() == Tile::State::Hidden)
			{
				TileAt(pos).Reveal();
				RevealNearbyTiles(pos);
			}
			else TileAt(pos).Reveal();
		}
}

void Minefield::CheckForWin()
{
	if (CorrectFlags == nMines)
	{
		state = State::Win;
	}
}


void Minefield::OnMouseClick(Mouse& mouse)
{
	if (!(state == State::Game_Over))
	{
		Vei2 fieldpos;
		switch (mouse.Read().GetType())
		{
		case Mouse::Event::Type::LPress:
			fieldpos = ScreenToField(mouse.GetPos());
			if (fieldpos.x <= width && fieldpos.y <= height)
			{
				if (TileAt(fieldpos).GetState() == Tile::State::Hidden)
				{
					TileAt(fieldpos).Reveal();
					if (TileAt(fieldpos).HasMine())
					{
						state = State::Game_Over;
					}
					if (TileAt(fieldpos).GetNeighborMines() == 0)
					{
						RevealNearbyTiles(fieldpos);
					}
				}
			}
			break;

		case Mouse::Event::Type::RPress:
			fieldpos = ScreenToField(mouse.GetPos());
			if (fieldpos.x <= width && fieldpos.y <= height)
			{
				if (TileAt(fieldpos).GetState() != Tile::State::Revealed)
				{
					TileAt(fieldpos).Flag();
					if (TileAt(fieldpos).HasMine())
					{
						switch (TileAt(fieldpos).GetState())
						{
						case Tile::State::Flagged:
							CorrectFlags++;
							break;

						case Tile::State::Hidden:
							CorrectFlags--;
							break;

						default:
							break;
						}
					}
				}
			}
			CheckForWin();
			break;
		default:
			break;
		}
	}
}


void Minefield::Tile::Init()
{
	state = State::Hidden;
}

bool Minefield::Tile::HasMine() const
{
	return hasMine;
}

void Minefield::Tile::SpawnMine()
{
	hasMine = true;
}

void Minefield::Tile::Reveal()
{
	state = State::Revealed;
}

void Minefield::Tile::Flag()
{
	assert(state == State::Hidden || state == State::Flagged);
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
	else state = State::Hidden;
}

const Minefield::Tile::State& Minefield::Tile::GetState() const
{
	return state;
}

void Minefield::Tile::SetNeighborMines(int MineCount)
{
	neighborMines = MineCount;
}

int Minefield::Tile::GetNeighborMines() const
{
	return neighborMines;
}
