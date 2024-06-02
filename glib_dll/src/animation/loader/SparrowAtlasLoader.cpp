#include "glib/animation/loader/SparrowAtlasLoader.h"
#include "glib/utils/ImageUtils.h"
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <RapidXMLSTD.hpp>
#include <iostream>
#include <regex>

using namespace glib;


static bool IsFourDigits(const std::string& str, size_t index) {
	if (index + 4 <= str.length()) {
		for (size_t i = 0; i < 4; ++i) {
			if (!isdigit(str[index + i])) {
				return false;
			}
		}
		return true;
	}
	return false;
}

static std::string RemoveFourDigitSequences(const std::string& input) {
	std::string result;
	size_t i = 0;

	while (i < input.length()) {
		if (isdigit(input[i]) && IsFourDigits(input, i)) {
			i += 4;
		}
		else {
			result += input[i];
			++i;
		}
	}

	return result;
}

struct XMLPtrs
{
	XMLFile* f;
	XMLDocument* d;
	XMLElement* e;
};

static XMLPtrs ReadXMLFile(const std::string& path)
{
	std::string error;
	XMLFile* file = OpenXMLFile(path, error);
	if (!file)
	{
		std::cout << error << std::endl;
	}

	XMLDocument* xml = CreateXMLFromFile(file, error);
	if (!xml)
	{
		std::cout << error << std::endl;
		DisposeXMLFile(file);
	}

	XMLElement* root = FirstOrDefaultElement(xml, "TextureAtlas", error);
	if (!root)
	{
		std::cout << error << std::endl;
		DisposeXMLFile(file);
		DisposeXMLObject(xml);
	}

	return { file, xml, root };
}

static float GetFloat(XMLElement* e, const std::string name)
{
	float v = 0.0f;
	XMLAttributte* attr = e->first_attribute(name.c_str());
	if (attr)
	{
		v = std::atof(attr->value());
	}
	return v;
}

static void flip_image_horizontally(unsigned char* data, int width, int height, int channels) {
	int row_size = width * channels;
	std::vector<unsigned char> row(row_size);

	for (int y = 0; y < height; ++y) {
		unsigned char* row_ptr = data + y * row_size;
		std::copy(row_ptr, row_ptr + row_size, row.begin());
		for (int x = 0; x < width; ++x) {
			for (int c = 0; c < channels; ++c) {
				row_ptr[(width - 1 - x) * channels + c] = row[x * channels + c];
			}
		}
	}
}

std::map<std::string, Animation*>glib::SparrowAtlasLoader::LoadFile(const std::string& path, const std::string& imagePath, Window* wnd, bool pixelart, bool xFlipped)
{
	std::map<std::string, Animation*> anims;
	XMLPtrs ptrs = ReadXMLFile(path);
	XMLElement* root = ptrs.e;
	ImageData imgData = wnd->LoadTextureRaw(imagePath);

	std::vector<std::string> names;
	std::vector<AnimationFrame> frames;

	std::string lastName;
	bool first = true;
	for (XMLElement* e = root->first_node("SubTexture"); e; e = e->next_sibling())
	{
		std::string name = RemoveFourDigitSequences((e->first_attribute("name")->value()));
		if (lastName != name)
		{
			names.push_back(name);
			if (!first)
			{
				anims.insert({ lastName, new Animation(frames, 24, false) });
				frames.clear();
			}
			lastName = name;
			first = false;
		}

		AnimationFrame frame{};
		float imgX, imgY, imgW, imgH, frameX, frameY, frameW, frameH;
		imgX = GetFloat(e, "x");
		imgY = GetFloat(e, "y");
		imgW = GetFloat(e, "width");
		imgH = GetFloat(e, "height");
		frameX = GetFloat(e, "frameX");
		frameY = GetFloat(e, "frameY");
		frameW = GetFloat(e, "frameWidth");
		frameH = GetFloat(e, "frameHeight");

		ImageData cutImg = ImageUtils::Cut(imgData, imgX, imgY, imgW, imgH);

		if (xFlipped)
		{
			flip_image_horizontally(cutImg.data, cutImg.width, cutImg.height, cutImg.channels);
		}

		Texture* tex = wnd->LoadTextureFromRawData(cutImg, pixelart);
		frame.tex = tex;
		delete[] cutImg.data;

		frame.x = -frameX;
		frame.y = -frameY;
		//frame.w = frameW;
		//frame.h = frameH;

		frames.push_back(frame);
	}
	anims.insert({ lastName, new Animation(frames, 24, false) });

	free(imgData.data);

	DisposeXMLFile(ptrs.f);
	DisposeXMLObject(ptrs.d);
	return anims;
}
