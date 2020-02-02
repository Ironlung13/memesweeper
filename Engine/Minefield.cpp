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

	for (fieldpos.x = 0; fieldpos.x <= width; fieldpos.x++)
		for (fieldpos.y = 0; fieldpos.y <= height; fieldpos.y++)
		{
			TileAt(fieldpos).SetNeighborMines(CountMines(fieldpos));
		}

	for (Tile b : tiles)
	{
		b.Init();
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
				if (!gameover)
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
				if (!gameover)
				{
					SpriteCodex::DrawTileNumber(screenpos, TileAt(ScreenToField(screenpos)).GetNeighborMines(), gfx);
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
						SpriteCodex::DrawTileNumber(screenpos, TileAt(ScreenToField(screenpos)).GetNeighborMines(), gfx);
						break;
					}
				}

			case Minefield::Tile::State::Flagged:
				if (!gameover)
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
	int Mines = 0;
	for (int x = fieldpos.x - 1; x <= fieldpos.x + 1; x++)
		for (int y = fieldpos.y - 1; y <= fieldpos.y + 1; y++)
		{
			if (x >= 0 && x <= width && y >= 0 && y <= height)
			{
				if (TileAt({ x,y }).HasMine())
					Mines++;
			}
		}
	return Mines;
}


void Minefield::OnMouseClick(Mouse& mouse)
{
	if (!gameover)
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
						gameover = true;
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
				}
			}
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
	assert(state == State::Hidden);
	if (!hasMine)
	{
		state = State::Revealed;
	}
	else
	{
		state = State::Mine;
	}
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
