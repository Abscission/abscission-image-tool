
#include "Sprite.h"
#include "Utility.h"

#include "PicoPNG.h"

#include <vector>


bool Sprite::Load(const char * Filename) {
	Win32FileContents FileContents = ReadEntireFile(Filename);

	if (FileContents.Size == 0) return false;

	if (this->Data) VirtualFree(this->Data, this->Width * this->Height * 4, MEM_RELEASE);

	std::vector<unsigned char> Image;

	decodePNG(Image, this->Width, this->Height, FileContents.Data, (unsigned long)FileContents.Size);

	this->Data = (unsigned int *)VirtualAlloc(0, Image.size() * 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	std::copy(Image.begin(), Image.end(), (unsigned char*)this->Data);

	hasTransparency = false;

	this->length = this->Width * this->Height * 4;

	for (int i = 0; i < this->Width * this->Height * 4; i++) {
		//Transformations to be done on each pixel of the loaded sprite
		unsigned int* P = Data + i;

		//Swap the Red and Blue bytes by masking out the relevant bits, shifting them into place, and using bitwise or to combine the bytes
		//Doing this now is a lot faser than doing it in the rendering code later as I used to
		*P = (*P & 0xff00ff00) | ((*P & 0xff0000) >> 16) | ((*P & 0xff) << 16);

		//Do premultiplied alpha, should make graphics scale and blend better
		float Alpha = (((*P & 0xff000000) >> 24) / 255.f);
		*P = (*P & 0xff000000) | ((int)((*P & 0xff0000) * Alpha) & 0xff0000) | ((int)((*P & 0xff00) * Alpha) & 0xff00) | ((int)((*P & 0xff) * Alpha) & 0xff);

		if (*P & 0xff000000 != 255) {
			hasTransparency = true;
		}
	}

	return true;
}
