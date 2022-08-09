/***************************************************
Copyright (c) 2021 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _ImageScaling_h
#define _ImageScaling_h

#include <ESP_Color.h>

class ImageScaling
{
public:

	// Scale up. Faster
	static void Scalar(TFT_eSprite& orig, TFT_eSprite& dest)
	{
		const uint16_t origWidth = orig.width();
		const uint16_t origHeight = orig.height();
		const uint16_t destWidth = dest.width();
		const uint16_t destHeight = dest.height();

		const uint16_t ratioX = destWidth / origWidth;
		const uint16_t ratioY = destHeight / origHeight;


		for (auto x = 0; x < origWidth; x++)
		{
			for (auto y = 0; y < origHeight; y++)
			{
				auto color = orig.readPixel(x, y);
				dest.fillRect(x * ratioX, y * ratioY, ratioX, ratioY, color);
			}
		}
	}

	// Scale Up. Fast
	static void NearestNeighbor(TFT_eSprite& orig, TFT_eSprite& dest)
	{
		const float origWidth = orig.width();
		const float origHeight = orig.height();
		const float destWidth = dest.width();
		const float destHeight = dest.height();

		for (auto x = 0; x < destWidth; x++)
		{
			for (auto y = 0; y < destHeight; y++)
			{
				auto color = orig.readPixel(origWidth / destWidth * x, origHeight / destHeight * y);
				dest.drawPixel(x, y, color);
			}
		}
	}

	// Scale down. Slow
	static void Average(TFT_eSprite& orig, TFT_eSprite& dest) {
		const float origWidth = orig.width();
		const float origHeight = orig.height();

		const float destWidth = dest.width();
		const float destHeight = dest.height();

		float ratioX = origWidth / destWidth;
		float ratioY = origHeight / destHeight;

		float yend = 0.0;
		for (int f = 0; f < destHeight; f++)
		{
			float ystart = yend;
			yend = (f + 1) * ratioY;
			if (yend >= origHeight) yend = origHeight - 0.000001;

			float xend = 0.0;
			for (int g = 0; g < destWidth; g++)
			{
				float xstart = xend;
				xend = (g + 1) * ratioX;
				if (xend >= origWidth) xend = origWidth - 0.000001;

				float R = 0.0;
				float G = 0.0;
				float B = 0.0;
				for (int y = (int)ystart; y <= (int)yend; y++)
				{
					float yportion = 1.0;
					if (y == (int)ystart) yportion -= ystart - y;
					if (y == (int)yend) yportion -= y + 1 - yend;

					for (int x = (int)xstart; x <= (int)xend; x++)
					{
						float xportion = 1.0;
						if (x == (int)xstart) xportion -= xstart - x;
						if (x == (int)xend) xportion -= x + 1 - xend;

						auto pixel = getpixel(orig, x, y);
						R += pixel.R * yportion * xportion / ratioX / ratioY;
						G += pixel.G * yportion * xportion / ratioX / ratioY;
						B += pixel.B * yportion * xportion / ratioX / ratioY;
					}
				}

				ESP_Color::Color result(R, G, B);
				putpixel(dest, g, f, result);
			}
		}
	}

	// Scale up. Slow
	static void Bilineal(TFT_eSprite& orig, TFT_eSprite& dest)
	{
		const float origWidth = orig.width();
		const float origHeight = orig.height();

		const float destWidth = dest.width();
		const float destHeight = dest.height();

		int x, y;
		for (x = 0, y = 0; y < destHeight; x++)
		{
			if (x > destWidth)
			{
				x = 0; y++;
			}
			float gx = x / (float)(destWidth) * (origWidth - 1);
			float gy = y / (float)(destHeight) * (origHeight - 1);

			int gxi = (int)gx;
			int gyi = (int)gy;


			auto c00 = getpixel(orig, gxi, gyi);
			auto c10 = getpixel(orig, gxi + 1, gyi);
			auto c01 = getpixel(orig, gxi, gyi + 1);
			auto c11 = getpixel(orig, gxi + 1, gyi + 1);

			auto R = lerp2D(c00.R, c10.R, c01.R, c11.R, gx - gxi, gy - gyi);
			auto G = lerp2D(c00.G, c10.G, c01.G, c11.G, gx - gxi, gy - gyi);
			auto B = lerp2D(c00.B, c10.B, c01.B, c11.B, gx - gxi, gy - gyi);
			
			ESP_Color::Color result(R, G, B);
			putpixel(dest, x, y, result);
		}
	}

private:
	inline static ESP_Color::Color getpixel(TFT_eSprite& image, uint16_t x, uint16_t y)
	{
		auto rgb = image.readPixel(x, y);
		return ESP_Color::Color(rgb);
	}

	inline static void putpixel(TFT_eSprite& image, uint16_t x, uint16_t y, ESP_Color::Color& color)
	{
		image.drawPixel(x, y, color.To<uint16_t>());
	}

	inline static float lerp1D(float s, float e, float t)
	{
		return s + (e - s) * t;
	}

	inline static float lerp2D(float c00, float c10, float c01, float c11, float tx, float ty)
	{
		return lerp1D(lerp1D(c00, c10, tx), lerp1D(c01, c11, tx), ty);
	}
};
#endif
