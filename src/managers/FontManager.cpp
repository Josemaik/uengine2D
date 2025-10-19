
#define STB_TRUETYPE_IMPLEMENTATION
#include "FontManager.hpp"


void FontManager::load(const char* filename, float height, uint8_t id)
{
	FILE* file;
	long filesize;
	int first_char = 32;  //32 a 127 -> carácteres imprimibles
	int num_chars = 96;
	int pw = calculateTextureSize(height, num_chars);
	int ph = pw; /// Usamos una textura cuadrada
	/*int pw = 512;
	int ph = 512;*/

	file = fopen(filename, "r");
	if (file)
	{
		fseek(file, 0, SEEK_END); //nos vamos al final del archivo
		filesize = ftell(file); //sacamos size
		fseek(file, 0, SEEK_SET); // volvemos al inicio

		if (filesize <= 0) {
			printf("Error: El archivo de fuente está vacío o no se pudo leer.\n");
			fclose(file);
			return;
		}

		int fontbuffersize = sizeof(unsigned char) * filesize; //tamaño del buffer
		unsigned char* font = new unsigned char[fontbuffersize]; //creamos buffer

		fread_s(font, fontbuffersize, sizeof(unsigned char), fontbuffersize, file);

		fclose(file);

		unsigned char* pixels_alpha = new unsigned char[pw * ph]; //buffer de pixeles
		memset(pixels_alpha, 0, pw * ph);

		stbtt_bakedchar* chardata = new stbtt_bakedchar[num_chars]; //glifos

		//Renderizo caracteres del buffer( componente alpha de cada píxel)
		int result = stbtt_BakeFontBitmap(font, 0, height, pixels_alpha, pw, ph, first_char, num_chars, chardata);
		if (result < 0) {
			printf("Error: No se pudo renderizar los caracteres. El buffer puede ser insuficiente.\n");
			delete[] pixels_alpha;
			return;
		}

		int sizepixelregba = pw * ph * 4;
		unsigned char* pixels_rgba = new unsigned char[sizepixelregba]; //buffer de pixeles

		//creamos buffer RGBA
		for (int i = 0; i < pw * ph; i++) {
			int idx = i * 4;
			pixels_rgba[idx] = 255;              // R
			pixels_rgba[idx + 1] = 255;          // G
			pixels_rgba[idx + 2] = 255;          // B
			pixels_rgba[idx + 3] = pixels_alpha[i]; // A
		}

		//Crear textura
		ltex_t* data = ltex_alloc(pw, ph, 1);
		ltex_setpixels(data, pixels_rgba);

		//Insertamos nueva fuente
		fontsPool.insert(std::make_pair(id, std::make_pair(data, chardata)));
		//Incrementammos ID
		//m_id++;

		//Liberamos memoria
		delete[] font;
		delete[] pixels_alpha;
		delete[] pixels_rgba;
	}
	else printf("Error opening file: %s\n",filename);
}

void FontManager::draw(const char* text, vec2f& pos, uint8_t type) const
{
	//Recogemos la textura de la entidad
	auto& font = fontsPool.at(type);
	if (font.first == nullptr) {
		printf("Error: El archivo de fuente no pudo ser cargado.\n");
		return;
	}
	int index = 0;

	stbtt_aligned_quad* font_info = new stbtt_aligned_quad; //puntero a info de la fuente
	while (text[index] != '\0') //Dibujamos caracteres
	{
		if (text[index] < 32 || text[index] >= 32 + 96) {
			printf("Carácter '%c' fuera del rango renderizable.\n", text[index]);
		}

		stbtt_GetBakedQuad(font.second, font.first->width, font.first->height, text[index] - 32, &pos.m_x,
			&pos.m_y, font_info, true);

		ltex_drawrotsized(font.first, font_info->x0, font_info->y0, 0.f, 0.5f, 0.5f, font_info->x1 - font_info->x0,
			font_info->y1 - font_info->y0,
			font_info->s0, font_info->t0, font_info->s1, font_info->t1);

		index++;
	}

	delete font_info;
	//printf("He acabado de pintar palabra\n");
}

float FontManager::getWidth(uint8_t type)
{
	auto& font = fontsPool.at(type);
	return static_cast<float>(font.first->width);
}

void FontManager::unloadFonts()
{
	for (auto& font : fontsPool)
	{
		auto& tex = font.second.first; //obtengo textura
		ltex_free(const_cast<ltex_t*>(tex));
		delete[] font.second.second;
	}
	fontsPool.clear();
}

int FontManager::calculateTextureSize(float pixel_height, int num_chars)
{
	// Ancho promedio de cada carácter (ajustado para ser más conservador)
	float avg_char_width = pixel_height * 0.75f;

	// Área total aproximada necesaria (con margen de seguridad)
	float total_area = avg_char_width * pixel_height * num_chars * 1.2f;

	// Dimensiones de la textura (potencia de 2 que cubre el área)
	int texture_size = 32; // Empezamos con 32x32 como mínimo
	while (texture_size * texture_size < total_area) {
		texture_size *= 2;
	}

	// Aseguramos un tamaño mínimo razonable para fuentes pequeñas
	if (pixel_height < 16) {
		texture_size = std::max(texture_size, 256); // Al menos 256x256
	}

	return texture_size; // Retorna ancho/alto (la textura será cuadrada)
}
