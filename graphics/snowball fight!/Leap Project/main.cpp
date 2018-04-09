#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#define OVR_OS_MAC
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#define OVR_OS_LINUX
#endif

#include "stdafx.h"
#include "main.h"
#include "./LibOVR/Include/OVR_CAPI.h" //include oculus sdk
#include "./OVRAvatarSDK/Include/OVR_Avatar.h" //include oculus avatar sdk
#include "LeapSDK/include/Leap.h"
#include "ClientGame.h"

GLFWwindow* window;

ClientGame * client;


void clientLoop(void);


void error_callback(int error, const char* description)
{
	// Print error
	fputs(description, stderr);
}

void setup_callbacks()
{
	// Set the error callback
	glfwSetErrorCallback(error_callback);
	// Set the key callback
	glfwSetKeyCallback(window, Window::key_callback);
	// Set the window resize callback
	glfwSetFramebufferSizeCallback(window, Window::resize_callback);
}

void setup_glew()
{
	// Initialize GLEW. Not needed on OSX systems.
#ifndef __APPLE__
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Current GLEW version: %s\n", glewGetString(GLEW_VERSION));
#endif
}

void setup_opengl_settings()
{
#ifndef __APPLE__
	// Setup GLEW. Don't do this on OSX systems.
	setup_glew();
#endif
	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer
	glDepthFunc(GL_ALWAYS);
	// Set polygon drawing mode to fill front and back of each polygon
	// You can also use the paramter of GL_LINE instead of GL_FILL to see wireframes
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable backface culling to render both sides of polygons
	glDisable(GL_CULL_FACE);
	// Set clear color
	glClearColor(0.05f, 0.8f, 0.85f, 1.0f);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

void clientLoop()
{
	while (true)
	{
		//do game stuff
		//client->update();
	}
}

int main(void)
{
	/*ovrResult result = ovr_Initialize(nullptr);
	if (OVR_FAILURE(result))
		return 0;

	ovrSession session;
	ovrGraphicsLuid luid;
	result = ovr_Create(&session, &luid);
	if (OVR_FAILURE(result))
	{
		ovr_Shutdown();
		return 0;
	}

	ovrHmdDesc desc = ovr_GetHmdDesc(session);
	ovrSizei resolution = desc.Resolution;*/

	// Create the GLFW window
	window = Window::create_window(1000, 800);
	//window = Window::create_window(1920, 1080);
	// Print OpenGL and GLSL versions
	print_versions();
	// Setup callbacks
	setup_callbacks();
	// Setup OpenGL settings, including lighting, materials, etc.
	setup_opengl_settings();
	// Initialize objects/pointers for rendering
	Window::initialize_objects();


	// initialize the client 
	client = new ClientGame();

	//clientLoop();


	glm::vec3 playerPos = glm::vec3(0);	
	glm::vec3 playerSnowballPos = glm::vec3(0);
	//bool snowballInHand = false;

	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(window))
	{
		glm::vec3 playerSnowballPos = glm::vec3(0);
		// Main render display callback. Rendering of objects is done here.
		Window::display_callback(window);
		playerPos = Window::getPlayerPos();
		client->playerX = playerPos.x;
		client->playerY = playerPos.y;
		client->playerZ = playerPos.z;
		client->playerSnowballInHand = Window::getPlayerSnowballInHand();
		//std::cout << client->enemyPlayerX << ", y, " << -client->enemyPlayerZ << std::endl;
		Window::setEnemyPlayerPos(-client->enemyPlayerX, client->enemyPlayerY, -client->enemyPlayerZ);
		if(client->playerSnowballInHand) playerSnowballPos = Window::getPlayerSnowballPos();
		client->snowballX = playerSnowballPos.x;
		client->snowballY = playerSnowballPos.y;
		client->snowballZ = playerSnowballPos.z;
		//client->playerSnowballInHand = Window::getPlayerSnowballInHand();
		Window::setEnemySnowballinHand(client->enemySnowballInHand);
		if(client->enemySnowballInHand) Window::setEnemySnowballPosition(glm::vec3(-client->enemySnowballX, client->enemySnowballY, -client->enemySnowballZ));
		client->hitByEnemy = Window::getHitByEnemy();
		Window::setEnemyHit(client->enemyHit);

		client->update();
	}

	Window::clean_up();
	// Destroy the window
	glfwDestroyWindow(window);
	// Terminate GLFW
	glfwTerminate();

	//ovr_Destroy(session);
	//ovr_Shutdown();

	exit(EXIT_SUCCESS);
}

/*// Initialize LibOVR...
ovr_Initialize();

ovrHmd l_Hmd = ovrHmd_Create(0);
if (!l_Hmd) l_Hmd = ovrHmd_CreateDebug(ovrHmd_DK1);

ovrHmdDesc l_HmdDesc;
ovrHmd_GetDesc(l_Hmd, &l_HmdDesc);

ovrHmd_StartSensor(l_Hmd, ovrHmdCap_Orientation, 0);

GLFWwindow* l_Window;

glfwSetErrorCallback(ErrorCallback);

if (!glfwInit()) exit(EXIT_FAILURE);

ovrSizei l_ClientSize;
l_ClientSize.w = l_HmdDesc.Resolution.w; // 1280 for DK1...
l_ClientSize.h = l_HmdDesc.Resolution.h; // 800 for DK1...
										 // Create a fullscreen window with the Oculus Rift resolution...
l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", glfwGetPrimaryMonitor(), NULL);
if (!l_Window)
{
	glfwTerminate();
	exit(EXIT_FAILURE);
}

// Print the OpenGL version we are using...
int l_Major = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MAJOR);
int l_Minor = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MINOR);
printf("OpenGL: %d.%d\n", l_Major, l_Minor);

// Make the context current for this window...
glfwMakeContextCurrent(l_Window);

// Create some lights, materials, etc...
SetOpenGLState();

// Don't forget to initialize Glew...
glewInit();

// We will do some offscreen rendering, setup FBO...
ovrSizei l_TextureSizeLeft = ovrHmd_GetFovTextureSize(l_Hmd, ovrEye_Left, l_HmdDesc.DefaultEyeFov&#91;0 & #93;, 1.0f);
ovrSizei l_TextureSizeRight = ovrHmd_GetFovTextureSize(l_Hmd, ovrEye_Right, l_HmdDesc.DefaultEyeFov&#91;1 & #93;, 1.0f);
ovrSizei l_TextureSize;
l_TextureSize.w = l_TextureSizeLeft.w + l_TextureSizeRight.w;
l_TextureSize.h = (l_TextureSizeLeft.h&gt;l_TextureSizeRight.h ? l_TextureSizeLeft.h : l_TextureSizeRight.h);

// Create FBO...
GLuint l_FBOId;
glGenFramebuffers(1, &l_FBOId);
glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

// The texture we're going to render to...
GLuint l_TextureId;
glGenTextures(1, &l_TextureId);
// "Bind" the newly created texture : all future texture functions will modify this texture...
glBindTexture(GL_TEXTURE_2D, l_TextureId);
// Give an empty image to OpenGL (the last "0")
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l_TextureSize.w, l_TextureSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
// Linear filtering...
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

// Create Depth Buffer...
GLuint l_DepthBufferId;
glGenRenderbuffers(1, &l_DepthBufferId);
glBindRenderbuffer(GL_RENDERBUFFER, l_DepthBufferId);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, l_TextureSize.w, l_TextureSize.h);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_DepthBufferId);

// Set the texture as our colour attachment #0...
glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_TextureId, 0);

// Set the list of draw buffers...
GLenum l_GLDrawBuffers&#91;1 & #93; = { GL_COLOR_ATTACHMENT0 };
glDrawBuffers(1, l_GLDrawBuffers); // "1" is the size of DrawBuffers

								   // Unbind...
glBindRenderbuffer(GL_RENDERBUFFER, 0);
glBindTexture(GL_TEXTURE_2D, 0);
glBindFramebuffer(GL_FRAMEBUFFER, 0);

// Oculus Rift eye configurations...
ovrEyeDesc l_Eyes&#91;2 & #93;;
l_Eyes&#91;0 & #93;.Eye = ovrEye_Left;
l_Eyes&#91;1 & #93;.Eye = ovrEye_Right;
l_Eyes&#91;0 & #93;.Fov = l_HmdDesc.DefaultEyeFov&#91;0 & #93;;
l_Eyes&#91;1 & #93;.Fov = l_HmdDesc.DefaultEyeFov&#91;1 & #93;;
l_Eyes&#91;0 & #93;.TextureSize.w = l_TextureSize.w;
l_Eyes&#91;0 & #93;.TextureSize.h = l_TextureSize.h;
l_Eyes&#91;1 & #93;.TextureSize.w = l_TextureSize.w;
l_Eyes&#91;1 & #93;.TextureSize.h = l_TextureSize.h;
l_Eyes&#91;0 & #93;.RenderViewport.Pos.x = 0;
l_Eyes&#91;0 & #93;.RenderViewport.Pos.y = 0;
l_Eyes&#91;1 & #93;.RenderViewport.Pos.x = (l_TextureSize.w + 1) / 2;
l_Eyes&#91;1 & #93;.RenderViewport.Pos.y = 0;
l_Eyes&#91;0 & #93;.RenderViewport.Size.w = l_TextureSize.w / 2;
l_Eyes&#91;0 & #93;.RenderViewport.Size.h = l_TextureSize.h;
l_Eyes&#91;1 & #93;.RenderViewport.Size.w = l_Eyes&#91;0 & #93;.RenderViewport.Size.w;
l_Eyes&#91;1 & #93;.RenderViewport.Size.h = l_Eyes&#91;0 & #93;.RenderViewport.Size.h;

ovrGLConfig l_Cfg;
l_Cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
l_Cfg.OGL.Header.Multisample = 0;
l_Cfg.OGL.Header.RTSize.w = l_ClientSize.w;
l_Cfg.OGL.Header.RTSize.h = l_ClientSize.h;
l_Cfg.OGL.WglContext = glfwGetWGLContext(l_Window);
l_Cfg.OGL.Window = glfwGetWin32Window(l_Window);
l_Cfg.OGL.GdiDc = GetDC(l_Cfg.OGL.Window);

int l_RenderCaps = 0;
int l_DistortionCaps = ovrDistortion_Chromatic | ovrDistortion_TimeWarp;
ovrEyeRenderDesc l_EyeRenderDesc&#91;2 & #93;;
ovrHmd_ConfigureRendering(l_Hmd, &l_Cfg.Config, l_RenderCaps, l_DistortionCaps, l_Eyes, l_EyeRenderDesc);

ovrGLTexture l_EyeTexture&#91;2 & #93;;
l_EyeTexture&#91;0 & #93;.OGL.Header.API = ovrRenderAPI_OpenGL;
l_EyeTexture&#91;0 & #93;.OGL.Header.TextureSize.w = l_TextureSize.w;
l_EyeTexture&#91;0 & #93;.OGL.Header.TextureSize.h = l_TextureSize.h;
l_EyeTexture&#91;0 & #93;.OGL.Header.RenderViewport = l_Eyes&#91;0 & #93;.RenderViewport;
l_EyeTexture&#91;0 & #93;.OGL.TexId = l_TextureId;

// Right eye uses the same texture, but a different rendering viewport...
l_EyeTexture&#91;1 & #93; = l_EyeTexture&#91;0 & #93;;
l_EyeTexture&#91;1 & #93;.OGL.Header.RenderViewport = l_Eyes&#91;1 & #93;.RenderViewport;

glfwSetKeyCallback(l_Window, KeyCallback);

GLfloat l_SpinX;
GLfloat l_SpinY;

while (!glfwWindowShouldClose(l_Window))
{
	l_SpinX = (GLfloat)fmod(glfwGetTime()*17.0, 360.0);
	l_SpinY = (GLfloat)fmod(glfwGetTime()*23.0, 360.0);

	ovrFrameTiming m_HmdFrameTiming = ovrHmd_BeginFrame(l_Hmd, 0);

	// Bind the FBO...
	glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);
	// Clear...
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int l_EyeIndex = 0; l_EyeIndex&lt;ovrEye_Count; l_EyeIndex++)
	{
		ovrEyeType l_Eye = l_HmdDesc.EyeRenderOrder&#91;l_EyeIndex&#93;;
		ovrPosef l_EyePose = ovrHmd_BeginEyeRender(l_Hmd, l_Eye);

		glViewport(l_EyeRenderDesc&#91;l_Eye&#93;.Desc.RenderViewport.Pos.x,	// StartX
			l_EyeRenderDesc&#91;l_Eye&#93;.Desc.RenderViewport.Pos.y,	// StartY
			l_EyeRenderDesc&#91;l_Eye&#93;.Desc.RenderViewport.Size.w,	// Width
			l_EyeRenderDesc&#91;l_Eye&#93;.Desc.RenderViewport.Size.h	// Height
		);

		// Get Projection and ModelView matrici from the device...
		OVR::Matrix4f l_ProjectionMatrix = ovrMatrix4f_Projection(
			l_EyeRenderDesc&#91;l_Eye&#93;.Desc.Fov, 0.3f, 100.0f, true);
		OVR::Quatf l_Orientation = OVR::Quatf(l_EyePose.Orientation);
		OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());

		// Pass matrici on to OpenGL...
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(&(l_ProjectionMatrix.Transposed().M&#91;0 & #93;&#91;0 & #93;));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// Translate for specific eye based on IPD...
		glTranslatef(l_EyeRenderDesc&#91;l_Eye&#93;.ViewAdjust.x,
			l_EyeRenderDesc&#91;l_Eye&#93;.ViewAdjust.y,
			l_EyeRenderDesc&#91;l_Eye&#93;.ViewAdjust.z);
		// Multiply with orientation retrieved from sensor...
		glMultMatrixf(&(l_ModelViewMatrix.Transposed().M&#91;0 & #93;&#91;0 & #93;));
		// Move back a bit to show scene in front of us...
		glTranslatef(0.0f, 0.0f, -3.0f);
		// Make the cube spin...
		glRotatef(l_SpinX, 1.0f, 0.0f, 0.0f);
		glRotatef(l_SpinY, 0.0f, 1.0f, 0.0f);

		// Render...
		RenderCube(1.0f);

		ovrHmd_EndEyeRender(l_Hmd, l_Eye, l_EyePose, &l_EyeTexture&#91;l_Eye&#93;.Texture);
	}

	// Unbind the FBO, back to normal drawing...
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Oculus wants CW orientations, avoid the problem by turning of culling...
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	ovrHmd_EndFrame(l_Hmd);
	// Restore state after Oculus did it's work (-sigh-)...
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);
	glUseProgramObjectARB(0);

	glfwPollEvents();
}

glfwDestroyWindow(l_Window);

glfwTerminate();

ovrHmd_Destroy(l_Hmd);
ovr_Shutdown();

exit(EXIT_SUCCESS);*/