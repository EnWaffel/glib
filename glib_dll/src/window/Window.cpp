#include "glib/window/Window.h"
#include "glib/Instance.h"
#include "glib/graphics/camera/Camera.h"
#include "glib/glibError.h"
#include "glib/graphics/Shader.h"

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <map>

extern int __GLIB_ERROR_CODE;
extern void glib_print_error();

static void _glfw_key_func(GLFWwindow* window, int key, int scancode, int action, int mods);
static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height);

static const wchar_t* __DEFAULT_CHARSET = L"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
										  L"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
										  L"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
										  L"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
										  L"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
										  L"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
										  L"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
										  L"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F";

namespace glib
{
	class WindowImpl
	{
	private:
		Instance* m_Instance;
		Window* m_Wnd;
		RenderPipeline* m_Pipeline;
		std::vector<Camera*> m_Cameras;
		GLFWwindow* m_Handle;
		std::vector<Shader*> m_Shaders;
		std::map<std::string, Texture*> m_Textures;
		bool m_IsOpen;
		int m_Width;
		int m_Height;
		EventManager m_EventManager;
		SoundManager m_SoundManager;
	public:
		Vec2 m_ViewportPos;
		Vec2 m_ViewportSize;
	public:
		WindowImpl(Instance* instance, Window* wnd) : m_Instance(instance), m_Wnd(wnd), m_Width(0), m_Height(0), m_IsOpen(false), m_Handle(nullptr), m_Pipeline(nullptr)
		{
		}

		void Init(const std::string& title, int width, int height)
		{
			m_Width = width;
			m_Height = height;

			glfwDefaultWindowHints();
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_SAMPLES, 4);

			m_Handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			glfwMakeContextCurrent(m_Handle);
			glfwSwapInterval(0);

			glfwSetKeyCallback(m_Handle, _glfw_key_func);
			glfwSetFramebufferSizeCallback(m_Handle, _glfw_framebuffer_resize);

			m_ViewportSize = Vec2(width, height);

			glfwSetWindowUserPointer(m_Handle, this);

			const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowPos(m_Handle, vidMode->width / 2 - width / 2, vidMode->height / 2 - height / 2);

			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_MULTISAMPLE);

			m_Pipeline = m_Instance->CreateDefaultPipeline(m_Wnd);
			m_Cameras.push_back(new Camera(GetInitialSize()));

			m_SoundManager.ChangeOutputDevice("");

			glfwMakeContextCurrent(NULL);
			m_IsOpen = true;

			Draw();
		}

		~WindowImpl()
		{
			for (Camera* cam : m_Cameras)
			{
				delete cam;
			}
			ClearCaches(true);
			
			delete m_Pipeline;

			glfwDestroyWindow(m_Handle);
		}

		const Vec2 GetSize() const
		{
			return Vec2();
		}

		void SetVisible(bool visible)
		{
			if (visible)
			{
				glfwShowWindow(m_Handle);
			}
			else
			{
				glfwHideWindow(m_Handle);
			}
		}

		RenderPipeline* GetRenderPipeline()
		{
			return m_Pipeline;
		}

		void Draw() const
		{
			glfwMakeContextCurrent(m_Handle);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Pipeline->Flush(m_Cameras);

			glfwSwapBuffers(m_Handle);
			glfwMakeContextCurrent(NULL);
		}

		bool IsOpen() const
		{
			return m_IsOpen;
		}
		
		void Update(float delta)
		{
			for (Camera* camera : m_Cameras)
			{
				camera->Update(delta);
			}
			m_SoundManager.Update();
		}

		void UpdateEvents(float delta)
		{
			glfwPollEvents();
			if (glfwWindowShouldClose(m_Handle))
			{
				m_IsOpen = false;
			}
		}

		Shader* LoadShader(const std::string& vertexCode, const std::string& fragmentCode)
		{
			int success;
			char infoLog[1024];
			GLuint id;

			const char* _vertexCode = vertexCode.c_str();
			const char* _fragmentCode = fragmentCode.c_str();

			GLuint vertex, fragment;
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &_vertexCode, NULL);
			glCompileShader(vertex);

			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
				__GLIB_ERROR_CODE = GLIB_FAIL_SHADER_COMPILEv;
				glib_print_error();
				std::cout << infoLog << std::endl;
			}

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &_fragmentCode, NULL);
			glCompileShader(fragment);

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
				__GLIB_ERROR_CODE = GLIB_FAIL_SHADER_COMPILEf;
				glib_print_error();
				std::cout << infoLog << std::endl;
			}

			id = glCreateProgram();
			glAttachShader(id, vertex);
			glAttachShader(id, fragment);
			glLinkProgram(id);

			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 1024, NULL, infoLog);
				__GLIB_ERROR_CODE = GLIB_FAIL_SHADER_LINK;
				glib_print_error();
				std::cout << infoLog << std::endl;
				return nullptr;
			}

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			Shader* shd = new Shader(id);

			m_Shaders.push_back(shd);

			return shd;
		}

		Camera* GetDefaultCamera()
		{
			if (m_Cameras.empty()) return nullptr;
			return m_Cameras[0];
		}

		const Vec2 GetInitialSize() const
		{
			return Vec2(m_Width, m_Height);
		}

		Texture* LoadTexture(const std::string& path, bool pixelart)
		{
			if (m_Textures.count(path) > 0)
			{
				return m_Textures.at(path);
			}

			stbi_set_flip_vertically_on_load(false);

			int width, height, numChannels;

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 4);
			if (stbi_failure_reason())
			{
				std::cout << stbi_failure_reason() << std::endl;
				return nullptr;
			}

			Texture* tex = LoadTextureFromRawData({ numChannels, width, height, data }, pixelart);

			m_Textures.insert({ path, tex });

			stbi_image_free(data);
			glfwMakeContextCurrent(NULL);
			return tex;
		}

		ImageData LoadTextureRaw(const std::string& path)
		{
			stbi_set_flip_vertically_on_load(false);

			int width, height, numChannels;

			stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 4);
			if (stbi_failure_reason())
			{
				std::cout << stbi_failure_reason() << std::endl;
				return {};
			}

			return { numChannels, width, height, data };
		}

		EventManager& GetEventManager()
		{
			return m_EventManager;
		}

		SoundManager& GetSoundManager()
		{
			return m_SoundManager;
		}

		void AddCamera(Camera* camera)
		{
			m_Cameras.push_back(camera);
		}

		void RemoveCamera(Camera* camera)
		{
			m_Cameras.erase(std::find(m_Cameras.begin(), m_Cameras.end(), camera));
		}

		const Vec2& GetViewportPos()
		{
			return m_ViewportPos;
		}

		const Vec2& GetViewportSize()
		{
			return m_ViewportSize;
		}

		Texture* LoadTextureFromRawData(ImageData data, bool pixelart)
		{
			glfwMakeContextCurrent(m_Handle);
			unsigned int id;

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			if (pixelart)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (GL_EXT_texture_filter_anisotropic) {
				GLfloat largest;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
			}

			if (data.channels == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
			}
			else if (data.channels == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

			Texture* tex = new Texture(id, data.width, data.height);
			glfwMakeContextCurrent(NULL);
			return tex;
		}

		void SetToCurrentContext()
		{
			glfwMakeContextCurrent(m_Handle);
		}

		void SetNotToCurrentContext()
		{
			glfwMakeContextCurrent(NULL);
		}

		void HideCursor()
		{
			glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		void ShowCursor()
		{
			glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		Font* LoadFont(const std::string& path)
		{
			return LoadFont(path, __DEFAULT_CHARSET);
		}

		Font* LoadFont(const std::string& path, const wchar_t* charset)
		{

		}

		void ClearCaches(bool del)
		{
			if (del)
			{
				for (Shader* shd : m_Shaders)
				{
					delete shd;
				}
				for (const auto& v : m_Textures)
				{
					delete v.second;
				}
			}

			m_Shaders.clear();
			m_Textures.clear();
		}
	};
}

using namespace glib;

static void _glfw_key_func(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowImpl* wnd = (WindowImpl*) glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS)
	{
		KeyEvent e{};
		e.id = GLIB_EVENT_KEY_PRESS;
		e.key = (KeyCode)key;
		wnd->GetEventManager().CallEvent(e);
	}
	else if (action == GLFW_RELEASE)
	{
		KeyEvent e{};
		e.id = GLIB_EVENT_KEY_RELEASE;
		e.key = (KeyCode)key;
		wnd->GetEventManager().CallEvent(e);
	}
}

static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height)
{
	WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);
	Vec2 size = wnd->GetInitialSize();

	float ratioX = width / (float)size.x;
	float ratioY = height / (float)size.y;
	float ratio = fminf(ratioX, ratioY);

	int viewWidth = (int)(size.x * ratio);
	int viewHeight = (int)(size.y * ratio);

	int viewX = (int)(width - size.x * ratio) / 2;
	int viewY = (int)(height - size.y * ratio) / 2;
	
	wnd->m_ViewportPos = Vec2(viewX, viewY);
	wnd->m_ViewportSize = Vec2(viewWidth, viewHeight);

	WindowResizeEvent event{};
	event.id = GLIB_EVENT_WINDOW_RESIZE;
	event.x = viewX;
	event.y = viewY;
	event.width = viewWidth;
	event.height = viewHeight;
	wnd->GetEventManager().CallEvent(event);
}

glib::Window::Window(Instance* instance, const std::string& title, int width, int height)
{
	impl = new WindowImpl(instance, this);
	impl->Init(title, width, height);
}

glib::Window::~Window()
{
	delete impl;
}

void glib::Window::SetVisible(bool visible)
{
	impl->SetVisible(visible);
}

const Vec2 glib::Window::GetSize() const
{
	return impl->GetSize();
}

const Vec2 glib::Window::GetInitialSize() const
{
	return impl->GetInitialSize();
}

RenderPipeline* glib::Window::GetRenderPipeline()
{
	return impl->GetRenderPipeline();
}

void glib::Window::Draw() const
{
	impl->Draw();
}

bool glib::Window::IsOpen() const
{
	return impl->IsOpen();
}

void glib::Window::Update(float delta)
{
	impl->Update(delta);
}

void glib::Window::UpdateEvents(float delta)
{
	impl->UpdateEvents(delta);
}

Camera* glib::Window::GetDefaultCamera()
{
	return impl->GetDefaultCamera();
}

EventManager& glib::Window::GetEventManager()
{
	return impl->GetEventManager();
}

SoundManager& glib::Window::GetSoundManager()
{
	return impl->GetSoundManager();
}

const Vec2& glib::Window::GetViewportPos()
{
	return impl->GetViewportPos();
}

const Vec2& glib::Window::GetViewportSize()
{
	return impl->GetViewportSize();
}

Shader* glib::Window::LoadShader(const std::string& vertexCode, const std::string& fragmentCode)
{
	return impl->LoadShader(vertexCode, fragmentCode);
}

Shader* glib::Window::LoadShaderFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		return LoadShader(vertexCode, fragmentCode);
	}
	catch (std::ifstream::failure e)
	{
		__GLIB_ERROR_CODE = GLIB_FAIL_FILE_READ;
		glib_print_error();
		return nullptr;
	}
}

Texture* glib::Window::LoadTexture(const std::string& path, bool pixelart)
{
	return impl->LoadTexture(path, pixelart);
}

ImageData glib::Window::LoadTextureRaw(const std::string& path)
{
	return impl->LoadTextureRaw(path);
}

Texture* glib::Window::LoadTextureFromRawData(ImageData data, bool pixelart)
{
	return impl->LoadTextureFromRawData(data, pixelart);
}

Font* glib::Window::LoadFont(const std::string& path)
{
	return nullptr;
}

Font* glib::Window::LoadFont(const std::string& path, const wchar_t* charset)
{
	return nullptr;
}

void glib::Window::SetToCurrentContext()
{
	impl->SetToCurrentContext();
}

void glib::Window::SetNotToCurrentContext()
{
	impl->SetNotToCurrentContext();
}

void glib::Window::HideCursor()
{
	impl->HideCursor();
}

void glib::Window::ShowCursor()
{
	impl->ShowCursor();
}

void glib::Window::AddCamera(Camera* camera)
{
	impl->AddCamera(camera);
}

void glib::Window::RemoveCamera(Camera* camera)
{
	impl->RemoveCamera(camera);
}

void glib::Window::ClearCaches()
{
	impl->ClearCaches(false);
}

void glib::Window::ClearCachesDelete()
{
	impl->ClearCaches(true);
}
