#include <M5StickCPlus.h>

#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include <ESP_Color.h>

#include "sprite_to_ws2812b.hpp"
#include "image_scaling.hpp"
#include "constants.h"

#include "mario-bross.h"

TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);
TFT_eSprite ledSprite = TFT_eSprite(&M5.Lcd);

void setup()
{
	Serial.begin(115200);
	M5.begin();
	M5.Lcd.setRotation(0);

	tftSprite.setColorDepth(16);
	tftSprite.createSprite(m5.Lcd.width(), m5.Lcd.height());

	ledSprite.setColorDepth(16);
	ledSprite.createSprite(16, 16);

	ledSprite.pushImage(0, 0, 16, 16, image_data_mario_bross);
	ledSprite.pushImage(0, 0, 16, 16, image_data_mario_bross);

	InitLed();
}

void Render()
{
	tftSprite.pushSprite(0, 0);
	SendToLed(ledSprite);
}

void loop()
{
	M5.update();
	Render();
	delay(1000);
}