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

	for (Tile b : tiles)
	{
		b.Init();
	}
}

void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height*SpriteCodex::tileSize, { 192,192,192 });	//Draws background for tiles


	Vei2 screenpos{ 0,0 };
	for (screenpos.x = 0; screenpos.x < width * SpriteCodex::tileSize; screenpos.x += SpriteCodex::tileSize)
	{
		for (screenpos.y = 0; screenpos.y < height * SpriteCodex::tileSize; screenpos.y += SpriteCodex::tileSize)
		{
			switch (TileAt(screenpos/ SpriteCodex::tileSize).GetState())
			{
			case Minefield::Tile::State::Hidden:
				SpriteCodex::DrawTileButton(screenpos, gfx);
				break;

			case Minefield::Tile::State::Revealed:
				switch (CountMines(ScreenToField(screenpos)))
				{
				case 0:
					SpriteCodex::DrawTile0(screenpos, gfx);
					break;
				case 1:
					SpriteCodex::DrawTile1(screenpos, gfx);
					break;
				case 2:
					SpriteCodex::DrawTile2(screenpos, gfx);
					break;
				case 3:
					SpriteCodex::DrawTile3(screenpos, gfx);
					break;
				case 4:
					SpriteCodex::DrawTile4(screenpos, gfx);
					break;
				case 5:
					SpriteCodex::DrawTile5(screenpos, gfx);
					break;
				case 6:
					SpriteCodex::DrawTile6(screenpos, gfx);
					break;
				case 7:
					SpriteCodex::DrawTile7(screenpos, gfx);
					break;
				case 8:
					SpriteCodex::DrawTile8(screenpos, gfx);
					break;
				default:
					SpriteCodex::DrawTile0(screenpos, gfx);
					break;
				}
				break;

			case Minefield::Tile::State::Flagged:
				SpriteCodex::DrawTileButton(screenpos, gfx);
				SpriteCodex::DrawTileFlag(screenpos, gfx);
				break;

			case Minefield::Tile::State::Mine:
				SpriteCodex::DrawTileBomb(screenpos, gfx);
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
	return Vei2{ screenpos.x / SpriteCodex::tileSize,screenpos.y / SpriteCodex::tileSize };
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
