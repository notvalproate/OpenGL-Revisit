#include <GL/glew.h>

class Cubemap {
public:
	static Cubemap& getCubemap();

private:
	Cubemap();

	static Cubemap s_Instance;
};	