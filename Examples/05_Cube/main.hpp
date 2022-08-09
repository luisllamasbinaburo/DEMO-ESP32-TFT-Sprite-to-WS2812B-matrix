#include <M5StickCPlus.h>

#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include <ESP_Color.h>

#include "CubeRenderer.hpp"
#include "image_scaling.hpp"
#include "sprite_to_ws2812b.hpp"

const int WIDTH = 80;
const int HEIGHT = 160;
const int ROTATION = 0;

const uint8_t origSize = 64;
const uint8_t destSize = 16;

TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);
TFT_eSprite tftSpriteScaled = TFT_eSprite(&M5.Lcd);
TFT_eSprite tftSpriteFinal = TFT_eSprite(&M5.Lcd);

void setup() {
	Serial.begin(115200);
	M5.begin();
	M5.Lcd.setRotation(ROTATION);

	tftSprite.setColorDepth(16);
	tftSprite.createSprite(origSize, origSize);
	tftSprite.fillScreen(TFT_BLACK);

	tftSpriteScaled.setColorDepth(16);
	tftSpriteScaled.createSprite(destSize, destSize);
	tftSpriteScaled.fillScreen(TFT_BLACK);

	tftSpriteFinal.setColorDepth(16);
	tftSpriteFinal.createSprite(origSize, origSize);
	tftSpriteFinal.fillScreen(TFT_BLACK);

	InitCube();
	InitLed();
}

void loop() {
	M5.update();

	UpdateCube();
	RenderCube(tftSprite);
	
	ImageScaling::Average(tftSprite, tftSpriteScaled);
	SendToLed(tftSpriteScaled);
	ImageScaling::Bilineal(tftSpriteScaled, tftSpriteFinal);

	tftSprite.pushSprite(0, 0);
	tftSpriteScaled.pushSprite(30, 64 + 10);
	tftSpriteFinal.pushSprite(0, 160 - 64);

}