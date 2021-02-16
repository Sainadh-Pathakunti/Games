#ifndef ASSETMANAGER_H_INCLUDED
#define ASSETMANAGER_H_INCLUDED

#include <string>
//#include "AssetManager.hpp"
template <typename ASSET>
class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	ASSET *Create(const std::string &assetId,
		const typename ASSET::CreateParams *params);
	ASSET *Get(const std::string &assetId) const;
	void Destroy(const std::string &assetId);

private:
	//AssetManager(const AssetManager & Asset);
	void operator=(const AssetManager &);

	typedef std::map<std::string, ASSET*> AssetMap;
	AssetMap assets_;
};



#endif // ASSETMANAGER_H_INCLUDED
