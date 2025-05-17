#include <math.h>
#include <stdlib.h>
#include "model.h"
#include "callbacks.h"
#include "move.h"

int movingWhichTrophy = 0;

void move_trophy(World* world) {
	if(movingWhichTrophy == 0)
	{
		if(abs((int)(camera.position.x - world->trophy1.position.x)) + abs((int)(camera.position.z - world->trophy1.position.z)) <
				abs((int)(camera.position.x - world->trophy2.position.x)) + abs((int)(camera.position.z - world->trophy2.position.z)))
		{
			movingWhichTrophy = 1;
		}
		else {
			movingWhichTrophy = 2;
		}
	}

	if(abs(camera.position.x) < 170 && abs(camera.position.z) < 170)
	{
		switch (movingWhichTrophy) {
			case 1:
				if (abs((int)(camera.position.x - world->trophy1.position.x)) < 50 && abs((int)(camera.position.z - world->trophy1.position.z)) < 50 && camera.position.y < 150)
				{
					world->trophy1.position.x = camera.position.x + 10;
					world->trophy1.position.z = camera.position.z + 10;

					world->trophy1.material_ambient[0] = camera.position.x / 200;
					world->trophy1.material_ambient[1] = camera.position.y / 200;
					world->trophy1.material_ambient[2] = camera.position.z / 200;
				}

				break;
			case 2:
				if (abs((int)(camera.position.x - world->trophy2.position.x)) < 50 && abs((int)(camera.position.z - world->trophy2.position.z)) < 50 && camera.position.y < 150)
				{
					world->trophy2.position.x = camera.position.x + 10;
					world->trophy2.position.z = camera.position.z + 10;

					world->trophy2.material_ambient[0] = camera.position.x / 200;
					world->trophy2.material_ambient[1] = camera.position.y / 200;
					world->trophy2.material_ambient[2] = camera.position.z / 200;
				}

				break;
		}
	}
}
