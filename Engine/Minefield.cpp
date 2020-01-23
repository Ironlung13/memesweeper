#include "Minefield.h"


void Minefield::Draw(Graphics& gfx)
{
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height*SpriteCodex::tileSize, { 192,192,192 });	//Draws background for tiles


	Vei2 field{ 0,0 };
	for (field.x = 0; field.x < width * SpriteCodex::tileSize; field.x += SpriteCodex::tileSize)
	{
		for (field.y = 0; field.y < height * SpriteCodex::tileSize; field.y += SpriteCodex::tileSize)
		{
			SpriteCodex::DrawTileButton(field, gfx);
		}
	}
}
