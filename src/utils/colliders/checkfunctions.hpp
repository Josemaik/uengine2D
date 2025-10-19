#pragma once

#include "../Vec2.hpp"
#include <algorithm>
#include <string_view>
#include <vector>
#include <utility>

namespace checkfunc
{
	//Funciones auxiliares
	inline int GetIndex(int localX, int localY, int SizeX)
	{
		return (localY * SizeX + localX) * 4;
	}

	////////////////////////////////////
	//CIRCLE-CIRCLE
	inline bool checkCircleCircle(const vec2f& pos1, float radius1, const vec2f& pos2, float radius2)
	{
		float distance = std::abs(pos2.distance(pos1));
		if (distance < (radius1 + radius2))
		{
			return true;
		}
		return false;
	}
	////////////////////////////////////
	//CIRCLE-RECT
	inline bool checkCircleRect(const vec2f& circlePos, float circleRadius, const vec2f& rectPos, const vec2f& rectSize)
	{
		float left = rectPos.m_x;
		float right = rectPos.m_x + rectSize.m_x;
		float top = rectPos.m_y;
		float bottom = rectPos.m_y + rectSize.m_y;
		vec2f lefttop{ left ,top };
		vec2f leftbottom{ left,bottom };
		vec2f righttop{ right,top };
		vec2f rightbottom{ right,bottom };

		// Comprobamos si est� dentro del circulo
		if (circlePos.m_x > left && circlePos.m_x < right &&
			circlePos.m_y >= top && circlePos.m_y < bottom)
		{
			return true;
		}
		else {
			// Comprobamos cuadrantes esquinas
			if (circlePos.m_x < left && circlePos.m_y < top)
			{
				// esquina superior izquierda
				if (lefttop.distance(circlePos) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.m_x < left && circlePos.m_y > bottom)
			{
				// esquina inferior izquierda
				if (leftbottom.distance(circlePos) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.m_x > right && circlePos.m_y < top)
			{
				// esquina superior deerecha
				if (righttop.distance(circlePos) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.m_x > right && circlePos.m_y > bottom)
			{
				// esquina inferior deerecha
				if (rightbottom.distance(circlePos) < circleRadius)
				{
					return true;
				}
			}
			// Comprobamos bordes horizontales y verticales
			if (circlePos.m_x >= left && circlePos.m_x <= right)
			{
				// Bordes superior e inferior
				if (std::abs(circlePos.m_y - top) < circleRadius || std::abs(circlePos.m_y - bottom) < circleRadius)
				{
					return true;
				}
			}
			if (circlePos.m_y >= top && circlePos.m_y <= bottom)
			{
				// Bordes izquierdo y derecho
				if (std::abs(circlePos.m_x - left) < circleRadius || std::abs(circlePos.m_x - right) < circleRadius)
				{
					return true;
				}
			}
		}
		return false;
	}
	////////////////////////////////////
	//RECT-RECT
	inline bool checkRectRect(const vec2f& rectPos1, const vec2f& rectSize1, const vec2f& rectPos2, const vec2f& rectSize2)
	{
		// Calculamos los l�mites de ambos rect�ngulos
		//Tile
		float left1 = rectPos1.m_x;
		float right1 = rectPos1.m_x + rectSize1.m_x;
		float top1 = rectPos1.m_y;
		float bottom1 = rectPos1.m_y + rectSize1.m_y;

		//Player
		float left2 = rectPos2.m_x;
		float right2 = rectPos2.m_x + rectSize2.m_x;
		float top2 = rectPos2.m_y;
		float bottom2 = rectPos2.m_y + rectSize2.m_y;
		// Verificamos si hay superposici�n
		//if ((top1 > bottom2) && (top1 < top2))
		//{
		//	//if()
		//	printf("Abajo\n");
		//	return true;
		//}
		bool horizontalOverlap = (left1 < right2) && (right1 > left2);
		bool verticalOverlap_down = (top1 < bottom2) && (bottom1 > top2);

		//if (verticalOverlap_down)
		//{
		//	//Abajo
		//	return true;
		//}
		// Si hay superposici�n en ambas direcciones, hay intersecci�n
		if (horizontalOverlap && verticalOverlap_down)
		{
			return true;
		}
		if (horizontalOverlap)
		{
			if (rectPos2.m_x >= rectPos1.m_x && (rectPos1.m_x - rectPos2.m_x) < 32.f)
			{
				//printf("horizontal por la izquierda\n");
			}
		/*	printf("No hay colision, solo horizontal overlap\n");
			if (rectPos2.m_x > rectPos1.m_x)
			{
				printf("izquierda\n");
			}
			else {
				printf("derecha\n");
			}*/
		}
		/*if (verticalOverlap_down)
		{
			printf("No hay colision, solo vertical overlap\n");
		}*/
		return false;
	}
	////////////////////////////////////
	//CIRCLE-PIXELS
	inline bool checkCirclePixels(const vec2f& circlePos, float circleRadius,
		const vec2f& pixelsPos, const vec2f& pixelsSize, const uint8_t* pixels)
	{
		// Calculamos los l�mites de la caja que envuelve al c�rculo
		float circleLeft = circlePos.m_x - circleRadius;
		float circleRight = circlePos.m_x + circleRadius;
		float circleTop = circlePos.m_y - circleRadius;
		float circleBottom = circlePos.m_y + circleRadius;

		// Calculamos los l�mites del �rea de intersecci�n con los p�xeles
		float interLeft = std::max(circleLeft, pixelsPos.m_x);
		float interRight = std::min(circleRight, pixelsPos.m_x + pixelsSize.m_x);
		float interTop = std::max(circleTop, pixelsPos.m_y);
		float interBottom = std::min(circleBottom, pixelsPos.m_y + pixelsSize.m_y);

		// Si no hay intersecci�n, no hay colisi�n
		if (interLeft >= interRight || interTop >= interBottom) {
			return false;
		}

		// Recorremos el �rea de intersecci�n
		for (int x = static_cast<int>(interLeft); x < static_cast<int>(interRight); ++x) {
			for (int y = static_cast<int>(interTop); y < static_cast<int>(interBottom); ++y) {
				// Convertimos las coordenadas globales a locales dentro del sprite
				int localX = static_cast<int>(x - pixelsPos.m_x);
				int localY = static_cast<int>(y - pixelsPos.m_y);

				// Calculamos el �ndice del p�xel en el arreglo de p�xeles del sprite
				int pixelIndex = GetIndex(localX, localY, static_cast<int>(pixelsSize.m_x));

				// Verificamos si el p�xel no es transparente (canal alfa)
				if (pixels[pixelIndex + 3] != 0) {
					// Calculamos la distancia al centro del c�rculo
					vec2f pixel{ static_cast<float>(x),static_cast<float>(y) };
					float dist = pixel.distance(circlePos);

					// Si la distancia es menor al radio del c�rculo, hay colisi�n
					if (dist <= circleRadius) {
						return true;
					}
				}
			}
		}

		return false; // No se encontr� colisi�n
	}
	////////////////////////////////////
	//PIXELS-PIXELS
	inline bool checkPixelsPixels(const vec2f& pixelsPos1, const vec2f& pixelsSize1, const uint8_t* pixels1,
		const vec2f& pixelsPos2, const vec2f& pixelsSize2, const uint8_t* pixels2)
	{
		// Si no hay intersecci�n, no hay colisi�n
		if (!checkRectRect(pixelsPos1, pixelsSize1, pixelsPos2, pixelsSize2)) {
			return false;
		}
		// Calculamos los l�mites de ambos rect�ngulos
		float left1 = pixelsPos1.m_x;
		float right1 = pixelsPos1.m_x + pixelsSize1.m_x;
		float top1 = pixelsPos1.m_y;
		float bottom1 = pixelsPos1.m_y + pixelsSize1.m_y;

		float left2 = pixelsPos2.m_x;
		float right2 = pixelsPos2.m_x + pixelsSize2.m_x;
		float top2 = pixelsPos2.m_y;
		float bottom2 = pixelsPos2.m_y + pixelsSize2.m_y;

		// Calculamos los l�mites del �rea de intersecci�n
		float interLeft = std::max(left1, left2);
		float interRight = std::min(right1, right2);
		float interTop = std::max(top1, top2);
		float interBottom = std::min(bottom1, bottom2);

		// Iteramos sobre el �rea de intersecci�n
		for (int x = static_cast<int>(interLeft); x < static_cast<int>(interRight); ++x) {
			for (int y = static_cast<int>(interTop); y < static_cast<int>(interBottom); ++y) {
				// Calculamos la posici�n relativa en ambas im�genes(coordenada global a relativa)
				//en pixels las posiciones est�n respecto al origen local
				int localX1 = static_cast<int>(x - left1);
				int localY1 = static_cast<int>(y - top1);

				int localX2 = static_cast<int>(x - left2);
				int localY2 = static_cast<int>(y - top2);

				// Calculamos el �ndice del p�xel
				// fila x ancho -> desplazamiento + columna * 4(cada pixel ocupa 4 bytes)
				int index1 = GetIndex(localX1, localY1, static_cast<int>(pixelsSize1.m_x));
				int index2 = GetIndex(localX2, localY2, static_cast<int>(pixelsSize2.m_x));

				// Comprobamos si ambos p�xeles son no transparentes( + 3 accedemos al byte que indica la transparencia)
				if (pixels1[index1 + 3] != 0 && pixels2[index2 + 3] != 0) {
					return true;
				}
			}
		}
		// No hay colision
		return false;

	}
	////////////////////////////////////
	//PIXELS-RECT
	inline bool checkPixelsRect(const vec2f& pixelsPos, const vec2f& pixelsSize, const uint8_t* pixels,
		const vec2f& rectPos, const vec2f& rectSize)
	{
		// Si no hay intersecci�n, no hay colisi�n
		if (!checkRectRect(pixelsPos, pixelsSize, rectPos, rectSize)) {
			return false;
		}

		// Calculamos los l�mites del �rea de intersecci�n
		float interLeft = std::max(pixelsPos.m_x, rectPos.m_x);
		float interRight = std::min(pixelsPos.m_x + pixelsSize.m_x, rectPos.m_x + rectSize.m_x);
		float interTop = std::max(pixelsPos.m_y, rectPos.m_y);
		float interBottom = std::min(pixelsPos.m_y + pixelsSize.m_y, rectPos.m_y + rectSize.m_y);

		// Recorremos el �rea de intersecci�n y verificamos los p�xeles
		for (int x = static_cast<int>(interLeft); x < static_cast<int>(interRight); ++x) {
			for (int y = static_cast<int>(interTop); y < static_cast<int>(interBottom); ++y) {
				// Convertimos las coordenadas globales a locales dentro de los p�xeles
				int localX = static_cast<int>(x - pixelsPos.m_x);
				int localY = static_cast<int>(y - pixelsPos.m_y);

				// Calculamos el �ndice del p�xel en el arreglo de p�xeles del sprite
				int pixelIndex = checkfunc::GetIndex(localX, localY, static_cast<int>(pixelsSize.m_x));

				// Verificamos si el p�xel del sprite no es transparente
				if (pixels[pixelIndex + 3] != 0) { // Canal alfa
					return true; // Hay colisi�n
				}
			}
		}
		return false; // No se encontr� colisi�n
	}
}
