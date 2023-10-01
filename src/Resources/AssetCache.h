#include <Model.h>
#include <string>

class AssetCache
{
public:

	static Model* LoadModel(std::string path);

private:

	static std::unordered_map<std::string, Model*> ModelCache;
};