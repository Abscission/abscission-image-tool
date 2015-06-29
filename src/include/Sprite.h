class ImageHeader {
public:
	unsigned int Width;
	unsigned int Height;
	unsigned int length;
	unsigned int HasTransparency;
};

class Sprite {
public:
	bool Load(const char * filename);

	unsigned long Width;
	unsigned long Height;

	bool hasTransparency;

	unsigned int *Data;
	unsigned int length;
};