/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include <M5StickCPlus.h>

#include <FastLED.h>
FASTLED_USING_NAMESPACE

#include <ESP_Color.h>

#include "./config.h"
#include "./constants.h"

#include "./sprite_to_ws2812b.hpp"

TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);
TFT_eSprite ledSprite = TFT_eSprite(&M5.Lcd);

void setup()
{
	Serial.begin(115200);
	M5.begin();
	M5.Lcd.setRotation(0);
	M5.IMU.Init();

	tftSprite.setColorDepth(16);
	tftSprite.createSprite(m5.Lcd.width(), m5.Lcd.height());
	tftSprite.fillScreen(TFT_BLUE);

	ledSprite.setColorDepth(16);
	ledSprite.createSprite(LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT);
	ledSprite.fillScreen(TFT_BLUE);	

	InitLed();
}

void loop()
{
	M5.update();
	
	tftSprite.pushSprite(0,0);
    SendToLed(ledSprite);

	delay(1000);
}