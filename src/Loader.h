#include <string>

using namespace std;


class Loader {
public:
	static void Load(string path);

private:
	static float getParam(string line);
};
