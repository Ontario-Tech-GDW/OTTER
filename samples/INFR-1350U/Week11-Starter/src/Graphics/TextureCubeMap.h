#pragma once
#include <cstdint>

#include "ITexture.h"
#include "TextureCubeMapData.h"
#include "Graphics/TextureEnums.h"

struct TextureCubeDesc {
	uint32_t       Size;
	InternalFormat Format;
	MinFilter      MinificationFilter;
	MagFilter      MagnificationFilter;
	bool           GenerateMipMaps;

	TextureCubeDesc() :
		Size(0),
		Format(InternalFormat::Unknown),
		MinificationFilter(MinFilter::Linear),
		MagnificationFilter(MagFilter::Linear),
		GenerateMipMaps(false)
	{ }
};

class TextureCubeMap final : public ITexture {
public:
	// We'll disallow moving and copying, since we want to manually control when the destructor is called
	// We'll use these classes via pointers
	TextureCubeMap(const TextureCubeMap& other) = delete;
	TextureCubeMap(TextureCubeMap&& other) = delete;
	TextureCubeMap& operator=(const TextureCubeMap& other) = delete;
	TextureCubeMap& operator=(TextureCubeMap&& other) = delete;

	typedef std::shared_ptr<TextureCubeMap> sptr;
	static inline sptr Create(const TextureCubeDesc& description = TextureCubeDesc()) {
		return std::make_shared<TextureCubeMap>(description);
	}

public:
	/// <summary>
/// Creates a new texture with the given description
/// </summary>
/// <param name="description">The default description for the texture</param>
	TextureCubeMap(const TextureCubeDesc& description);
	// ITexture handles destroying the OpenGL data, so we can use the default destructor
	~TextureCubeMap() = default;

	/// <summary>
	/// Uploads data to this texture
	/// </summary>
	/// <param name="data">The texture data to upload into this texture</param>
	void LoadData(const TextureCubeMapData::sptr& data);

	static TextureCubeMap::sptr LoadFromImages(const std::string& path);

	uint32_t GetSize() const { return _description.Size; }
	InternalFormat GetFormat() const { return _description.Format; }
	MinFilter GetMinFilter() const { return _description.MinificationFilter; }
	MagFilter GetMagFilter() const { return _description.MagnificationFilter; }

	void SetMinFilter(MinFilter filter);
	void SetMagFilter(MagFilter filter);

	const TextureCubeDesc& GetDescription() const { return _description; }

private:
	TextureCubeDesc _description;

	void _RecreateTexture();
};
