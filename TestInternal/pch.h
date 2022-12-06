#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include <vector>
#include "reclass.h"
#include <math.h>

Vector3 CalculateDegrees(Vector3 myPosition, Vector3 targetPosition);
int GetClosestAliveEnemy(Vector3 myPosition, int entityCount, entityList* enemy);
bool CheckCrosshair(int entityCount, entityList* enemy, int crosshair, int ptr);
bool Visible(Vector3 from, Vector3 to);
void drawRect(std::vector<Vector3> mouse,
              std::vector<Vector3> foot, std::vector<Vector3> head);

#endif
