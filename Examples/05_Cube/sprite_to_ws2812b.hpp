#include "./constants.h"

CRGB leds[LED_MATRIX_NUM_LEDS];

void InitLed()
{
	auto correction = CRGB(0xFF, 0xE0, 0x1C);
	LEDS.addLeds<WS2812B, DATA_PIN, GRB>(leds, LED_MATRIX_NUM_LEDS).setCorrection(correction);
	LEDS.setBrightness(BRIGHTNESS);
}

void SendToLed(TFT_eSprite& sprite)
{
	for (auto y = 0; y < LED_MATRIX_WIDTH; y++)
	{
		for (auto x = 0; x < LED_MATRIX_WIDTH; x++)
		{
			uint16_t rgb565 = sprite.readPixel(x, y);

			ESP_Color::Color color(rgb565);
			auto rgb = CRGB(color.R_Byte(), color.G_Byte(), color.B_Byte());

			auto ix = y;
			auto iy = x;
			leds[(iy % 2 ? LED_MATRIX_WIDTH - ix - 1 : ix) + iy * LED_MATRIX_WIDTH] = rgb;
		}
	}

	FastLED.show();
}
