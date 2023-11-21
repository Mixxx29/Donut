#include "donut.h"
#include <math.h>
#include "screen.h"
#include <stdio.h>

const float pi = 3.1415f;

const float thetaSpacing = 0.07f;
const float phiSpacing = 0.02f;

float A = 0;
float B = 0;

const float R1 = 1.0f;
const float R2 = 2.0f;
const float K1 = 20.0f;
const float K2 = 5.0f;

const //char* luminance = ".,-~:;=!*#$@";
const WCHAR* luminance = L"░▒▓█";

void DrawDonut(screen_t* screen)
{
	// Rotate
	A += 0.005f;
	B += 0.001f;

	// Precompute sines and cosines of A and B
	float sinA = sinf(A);
	float cosA = cosf(A);

	float sinB = sinf(B);	
	float cosB = cosf(B);

	// For each phi in thorus radius
	for (float phi = 0; phi < 2 * pi; phi += phiSpacing)
	{
		// Precompute sin and cos of phi
		float sinPhi = sinf(phi);
		float cosPhi = cosf(phi);

		// For each theta in thorus croos-section
		for (float theta = 0; theta < 2 * pi; theta += thetaSpacing)
		{
			// Precompute sin and cos of theta
			float sinTheta = sinf(theta);
			float cosTheta= cosf(theta);

			// Get x and y of cross-section before rotation
			float circleX = R2 + R1 * cosTheta;
			float circleY = R1 * sinTheta;

			// Rotate x and y in 3D
			float t = sinPhi * circleX * cosA - sinTheta * sinA;
			float x = cosPhi * circleX * cosB - t * sinB;
			float y = cosPhi * circleX * sinB + t * cosB;

			// Calculate z
			float z = sinPhi * circleX * sinA + sinTheta * cosA + K2;
			float zInverted = 1.0f / z;
			float scale = K1 * zInverted;

			// Project x and y in 3D
			int xProjected = (int)(screen->nWidth * 0.5f + scale * x);
			int yProjected = (int)(screen->nHeight * 0.5f - scale * y);

			// Get pixel index
			int pixelIndex = yProjected * screen->nWidth + xProjected;
			if (pixelIndex < 0 || pixelIndex >= screen->nWidth * screen->nHeight)
				continue;

			// Check z-buffer
			if (zInverted > screen->zBuffer[pixelIndex])
			{
				// Calculate light [-sqrt(2), sqrt(2)] 
				float L = (sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB -
					sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB;

				screen->zBuffer[pixelIndex] = zInverted;
				int luminanceIndex = L > 0 ? L * 2.8 : 0; // Apply lighting
				SetChar(screen, xProjected, yProjected, luminance[luminanceIndex]);
			}
		}
	}
}
