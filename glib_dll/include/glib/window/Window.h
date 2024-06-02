#pragma once

#include "../DLLDefs.h"
#include "../math/Vec2.h"
#include "../graphics/pipeline/RenderPipeline.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../event/EventManager.h"
#include "../sound/SoundManager.h"
#include "../math/Vec2.h"
#include "../graphics/Font.h"

#include <string>

namespace glib
{
	class WindowImpl;
	class Instance;

	class Window
	{
	private:
		WindowImpl* impl;
	public:
		Window(Instance* instance, const std::string& title, int width, int height);
		~Window();

		GLIB_API void SetVisible(bool visible);
		GLIB_API const Vec2 GetSize() const;
		GLIB_API const Vec2 GetInitialSize() const;
		GLIB_API RenderPipeline* GetRenderPipeline();
		GLIB_API void Draw() const;
		GLIB_API bool IsOpen() const;
		GLIB_API void Update(float delta);
		GLIB_API void UpdateEvents(float delta);
		GLIB_API Camera* GetDefaultCamera();
		GLIB_API EventManager& GetEventManager();
		GLIB_API SoundManager& GetSoundManager();
		GLIB_API const Vec2& GetViewportPos();
		GLIB_API const Vec2& GetViewportSize();

		GLIB_API Shader* LoadShader(const std::string& vertexCode, const std::string& fragmentCode);
		GLIB_API Shader* LoadShaderFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
		GLIB_API Texture* LoadTexture(const std::string& path, bool pixelart = false);
		GLIB_API ImageData LoadTextureRaw(const std::string& path);
		GLIB_API Texture* LoadTextureFromRawData(ImageData data, bool pixelart = false);

		GLIB_API Font* LoadFont(const std::string& path);
		GLIB_API Font* LoadFont(const std::string& path, const wchar_t* charset);

		GLIB_API void SetToCurrentContext();
		GLIB_API void SetNotToCurrentContext();
		GLIB_API void HideCursor();
		GLIB_API void ShowCursor();

		/**
		* Added cameras will be automatically deleted when the window closes.
		*/
		GLIB_API void AddCamera(Camera* camera);
		GLIB_API void RemoveCamera(Camera* camera);

		/**
		* Not recommended to use!
		* 
		* This method will remove all objects (e.g. Textures, Fonts) from the internal caches. These objects will not be deleted when the window closes.
		*/
		GLIB_API void ClearCaches();

		/**
		* Not recommended to use!
		* 
		* This method will remvoe and delete all objects (e.g. Textures, Fonts) from the internal caches.
		*/
		GLIB_API void ClearCachesDelete();
	};
}