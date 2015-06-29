
#include <Windows.h>
#include <iostream>
#include "Sprite.h"

int main(int argc, char* argv[]) {

	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [input.png] [output.agi]\n";
		return 1;
	}

	Sprite sprite;
	if (!sprite.Load(argv[1])) {
		std::cout << "Failed to load image " << argv[1] << "\n";
		return 1;
	}

	ImageHeader Header;
	Header.Width = sprite.Width;
	Header.Height = sprite.Height;
	Header.HasTransparency = sprite.hasTransparency;
	Header.length = sprite.length;

	int SizeOfFile = sizeof(Header) + sprite.length;

	byte* Buffer = (byte*)VirtualAlloc(0, sizeof(Header) + Header.length, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	memcpy((void*)Buffer, (void*)&Header, sizeof(Header));
	memcpy((void*)(Buffer + sizeof(Header)), (void*)sprite.Data, sprite.length);

	HANDLE ImageFile = CreateFileA(argv[2], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ImageFile == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open " << argv[2] << " for writing\n";
		return 1;
	}

	WriteFile(ImageFile, (void*)Buffer, sizeof(Header) + Header.length, 0, 0);
}