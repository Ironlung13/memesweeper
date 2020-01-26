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


	Vei2 fieldpos{ 0,0 };
	for (fieldpos.x = 0; fieldpos.x < width * SpriteCodex::tileSize; fieldpos.x += SpriteCodex::tileSize)
	{
		for (fieldpos.y = 0; fieldpos.y < height * SpriteCodex::tileSize; fieldpos.y += SpriteCodex::tileSize)
		{
			switch (TileAt(fieldpos/ SpriteCodex::tileSize).GetState())
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

Vei2 Minefield::ScreenToField(const Vei2 screenpos)
{
	return Vei2{ screenpos.x / SpriteCodex::tileSize,screenpos.y / SpriteCodex::tileSize };
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
			if (TileAt(fieldpos).GetState() == Tile::State::Hidden)
			{
				TileAt(fieldpos).Flag();
			}
		}
		break;
	default:
		break;
	}

	
}

void Minefield::OnLMClick(Mouse& mouse)
{
	Vei2 fieldpos = ScreenToField(mouse.GetPos());
	if (fieldpos.x <= width && fieldpos.y <= height)
	{
		if (TileAt(fieldpos).GetState() == Tile::State::Hidden)
		{
			TileAt(fieldpos).Reveal();
		}
	}
}

void Minefield::OnRMClick(Mouse & mouse)
{
	Vei2 fieldpos = ScreenToField(mouse.GetPos());
	if (fieldpos.x <= width && fieldpos.y <= height)
	{
		if (TileAt(fieldpos).GetState() == Tile::State::Hidden)
		{
			TileAt(fieldpos).Flag();
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
	assert(state == State::Hidden);
	state = State::Flagged;
}

const Minefield::Tile::State& Minefield::Tile::GetState() const
{
	return state;
}
