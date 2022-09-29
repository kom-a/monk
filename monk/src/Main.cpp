#if 1

#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include "Monk.h"

#include <windows.h>

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)


using namespace monk;

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void OnEvent(Event& e);
	bool OnWindowResize(WindowResizeEvent& e);

private:
	Window* m_Window;
	Renderer2D* m_Renderer;
};

Application::Application()
{
	m_Window = new Window(1280, 720, "Monk");
	m_Window->SetEventCallback(BIND_FUNCTION(Application::OnEvent));
	m_Window->SwapInterval(0);

	if (!utils::OpenGLLoader::LoadOpenGL(utils::OpenGLVersion::OPENGL_3_3))
		DIE("Failed to load OpenGL functions");
	else
	{
		LOG_INFO("OpenGL: ");
		LOG_INFO("	vendor: {0}", glGetString(GL_VENDOR));
		LOG_INFO("	renderer: {0}", glGetString(GL_RENDERER));
		LOG_INFO("	version: {0}", glGetString(GL_VERSION));
	}

	m_Renderer = new Renderer2D();

	glClearColor(0.45f, 0.1f, 0.8f, 1.0f);
}

Application::~Application()
{
	delete m_Window;
}

void Application::Run()
{
	Gui::Init();

	auto lastTime = std::chrono::system_clock::now();

	bool open1 = true;
	bool open2 = true;
	bool open3 = true;

	float sdf_size = 50.0;          // the larger this is, the better large font sizes look
	float pixel_dist_scale = 64.0;  // trades off precision w/ ability to handle *smaller* sizes
	int onedge_value = 128;
	int padding = 3; // not used in shader
    

    utils::FileData ttf = utils::FileManager::ReadBytes("C:/Users/koma/Desktop/CascadiaMono.ttf");
    Truetype tt(ttf.Data);
    float scale = tt.ScaleForPixelHeight(sdf_size);
    FontChar K = tt.GetCodepointSDF(scale, 'A', padding, onedge_value, pixel_dist_scale);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture

    uint8_t* data = new uint8_t[K.Width * K.Height * 4];

    /*for (int i = 0; i < K.Height; i++)
    {
        int y = K.Height - i - 1;
        y *= K.Width * 4;
        for (int j = 0; j < K.Width; j++)
        {
			data[y + j * 4 + 0] = K.Data[i * K.Width + j];
			data[y + j * 4 + 1] = K.Data[i * K.Width + j];
			data[y + j * 4 + 2] = K.Data[i * K.Width + j];
			data[y + j * 4 + 3] = K.Data[i * K.Width + j];
        }
    }*/

    for (int i = 0; i < K.Width * K.Height; i++)
    {
        data[i * 4 + 0] = K.Data[i];
        data[i * 4 + 1] = K.Data[i];
        data[i * 4 + 2] = K.Data[i];
        data[i * 4 + 3] = K.Data[i];
    }

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, K.Width, K.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

    float textureData[] = {
        100.0f, 100.0f, 0.0f, 0.0f, 
        500.0f, 100.0f, 1.0f, 0.0f,
        500.0f, 500.0f, 1.0f, 1.0f,
        100.0f, 500.0f, 0.0f, 1.0f
    };

    uint32_t textureIndexData[] = {
        0, 1, 2, 0, 2, 3
    };

	BufferLayout textureLayout = {
		{ 0, BufferLayout::AttribType::Float2 },
		{ 1, BufferLayout::AttribType::Float2 },
	};

    VertexBuffer textureBuffer(textureData, sizeof(textureData), textureLayout);
    IndexBuffer textureIndex(textureIndexData, 6);

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    
    Shader textureShader(utils::FileManager::ReadFile("res/textureVertex.glsl"), utils::FileManager::ReadFile("res/textureFragment.glsl"));

	while (!m_Window->Closed())
	{
		auto currentTime = std::chrono::system_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		float delta = deltaTime / 1000.0f;

		LOG_TRACE("FPS: {0}, deltaTime: {1}ms", 1000.0f / delta, delta);

		glClear(GL_COLOR_BUFFER_BIT);

		m_Window->PollEvents();

		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

		if (Input::IsKeyDown(Key::D1))
			open1 = true;
		if (Input::IsKeyDown(Key::D2))
			open2 = true;
		if (Input::IsKeyDown(Key::D3))
			open3 = true;
		
		m_Renderer->Begin(math::Ortho(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f, -1.0f, 1.0f));

		glBindVertexArray(vao);
		textureBuffer.Bind();
		textureIndex.Bind();
        textureShader.SetMatrix4("u_Projection", math::Ortho(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f, -1.0f, 1.0f));
		textureShader.Bind();

        glDrawElements(GL_TRIANGLES, textureIndex.Count(), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(0);

		m_Renderer->End();

		Gui::NewFrame(math::Ortho(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f, -1.0f, 1.0f));

		if (open1)
		{
			Gui::Begin("Window1", &open1);
			Gui::End();
		}
		
		if (open2)
		{
			Gui::Begin("Window2", &open2);
			Gui::End();
		}
		
		Gui::EndFrame();

		m_Window->Update();
		Input::Update();
	}
}

void Application::OnEvent(Event& e)
{
	e.Dispatch<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	
	return true;
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
#else

#include <string>
#include "core/Log.h"

#define STB_DEFINE
/* stb.h - v2.37 - Sean's Tool Box -- public domain -- http://nothings.org/stb.h
          no warranty is offered or implied; use this code at your own risk

   This is a single header file with a bunch of useful utilities
   for getting stuff done in C/C++.

   Documentation: http://nothings.org/stb/stb_h.html
   Unit tests:    http://nothings.org/stb/stb.c

 ============================================================================
   You MUST

      #define STB_DEFINE

   in EXACTLY _one_ C or C++ file that includes this header, BEFORE the
   include, like this:

      #define STB_DEFINE
      #include "stb.h"

   All other files should just #include "stb.h" without the #define.
 ============================================================================

Version History

   2.36   various fixes
   2.35   fix clang-cl issues with swprintf
   2.34   fix warnings
   2.33   more fixes to random numbers
   2.32   stb_intcmprev, stb_uidict, fix random numbers on Linux
   2.31   stb_ucharcmp
   2.30   MinGW fix
   2.29   attempt to fix use of swprintf()
   2.28   various new functionality
   2.27   test _WIN32 not WIN32 in STB_THREADS
   2.26   various warning & bugfixes
   2.25   various warning & bugfixes
   2.24   various warning & bugfixes
   2.23   fix 2.22
   2.22   64-bit fixes from '!='; fix stb_sdict_copy() to have preferred name
   2.21   utf-8 decoder rejects "overlong" encodings; attempted 64-bit improvements
   2.20   fix to hash "copy" function--reported by someone with handle "!="
   2.19   ???
   2.18   stb_readdir_subdirs_mask
   2.17   stb_cfg_dir
   2.16   fix stb_bgio_, add stb_bgio_stat(); begin a streaming wrapper
   2.15   upgraded hash table template to allow:
            - aggregate keys (explicit comparison func for EMPTY and DEL keys)
            - "static" implementations (so they can be culled if unused)
   2.14   stb_mprintf
   2.13   reduce identifiable strings in STB_NO_STB_STRINGS
   2.12   fix STB_ONLY -- lots of uint32s, TRUE/FALSE things had crept in
   2.11   fix bug in stb_dirtree_get() which caused "c://path" sorts of stuff
   2.10   STB_F(), STB_I() inline constants (also KI,KU,KF,KD)
   2.09   stb_box_face_vertex_axis_side
   2.08   bugfix stb_trimwhite()
   2.07   colored printing in windows (why are we in 1985?)
   2.06   comparison functions are now functions-that-return-functions and
          accept a struct-offset as a parameter (not thread-safe)
   2.05   compile and pass tests under Linux (but no threads); thread cleanup
   2.04   stb_cubic_bezier_1d, smoothstep, avoid dependency on registry
   2.03   ?
   2.02   remove integrated documentation
   2.01   integrate various fixes; stb_force_uniprocessor
   2.00   revised stb_dupe to use multiple hashes
   1.99   stb_charcmp
   1.98   stb_arr_deleten, stb_arr_insertn
   1.97   fix stb_newell_normal()
   1.96   stb_hash_number()
   1.95   hack stb__rec_max; clean up recursion code to use new functions
   1.94   stb_dirtree; rename stb_extra to stb_ptrmap
   1.93   stb_sem_new() API cleanup (no blockflag-starts blocked; use 'extra')
   1.92   stb_threadqueue--multi reader/writer queue, fixed size or resizeable
   1.91   stb_bgio_* for reading disk asynchronously
   1.90   stb_mutex uses CRITICAL_REGION; new stb_sync primitive for thread
          joining; workqueue supports stb_sync instead of stb_semaphore
   1.89   support ';' in constant-string wildcards; stb_mutex wrapper (can
          implement with EnterCriticalRegion eventually)
   1.88   portable threading API (only for win32 so far); worker thread queue
   1.87   fix wildcard handling in stb_readdir_recursive
   1.86   support ';' in wildcards
   1.85   make stb_regex work with non-constant strings;
               beginnings of stb_introspect()
   1.84   (forgot to make notes)
   1.83   whoops, stb_keep_if_different wasn't deleting the temp file
   1.82   bring back stb_compress from stb_file.h for cmirror
   1.81   various bugfixes, STB_FASTMALLOC_INIT inits FASTMALLOC in release
   1.80   stb_readdir returns utf8; write own utf8-utf16 because lib was wrong
   1.79   stb_write
   1.78   calloc() support for malloc wrapper, STB_FASTMALLOC
   1.77   STB_FASTMALLOC
   1.76   STB_STUA - Lua-like language; (stb_image, stb_csample, stb_bilinear)
   1.75   alloc/free array of blocks; stb_hheap bug; a few stb_ps_ funcs;
          hash*getkey, hash*copy; stb_bitset; stb_strnicmp; bugfix stb_bst
   1.74   stb_replaceinplace; use stdlib C function to convert utf8 to UTF-16
   1.73   fix performance bug & leak in stb_ischar (C++ port lost a 'static')
   1.72   remove stb_block, stb_block_manager, stb_decompress (to stb_file.h)
   1.71   stb_trimwhite, stb_tokens_nested, etc.
   1.70   back out 1.69 because it might problemize mixed builds; stb_filec()
   1.69   (stb_file returns 'char *' in C++)
   1.68   add a special 'tree root' data type for stb_bst; stb_arr_end
   1.67   full C++ port. (stb_block_manager)
   1.66   stb_newell_normal
   1.65   stb_lex_item_wild -- allow wildcard items which MUST match entirely
   1.64   stb_data
   1.63   stb_log_name
   1.62   stb_define_sort; C++ cleanup
   1.61   stb_hash_fast -- Paul Hsieh's hash function (beats Bob Jenkins'?)
   1.60   stb_delete_directory_recursive
   1.59   stb_readdir_recursive
   1.58   stb_bst variant with parent pointer for O(1) iteration, not O(log N)
   1.57   replace LCG random with Mersenne Twister (found a public domain one)
   1.56   stb_perfect_hash, stb_ischar, stb_regex
   1.55   new stb_bst API allows multiple BSTs per node (e.g. secondary keys)
   1.54   bugfix: stb_define_hash, stb_wildmatch, regexp
   1.53   stb_define_hash; recoded stb_extra, stb_sdict use it
   1.52   stb_rand_define, stb_bst, stb_reverse
   1.51   fix 'stb_arr_setlen(NULL, 0)'
   1.50   stb_wordwrap
   1.49   minor improvements to enable the scripting language
   1.48   better approach for stb_arr using stb_malloc; more invasive, clearer
   1.47   stb_lex (lexes stb.h at 1.5ML/s on 3Ghz P4; 60/70% of optimal/flex)
   1.46   stb_wrapper_*, STB_MALLOC_WRAPPER
   1.45   lightly tested DFA acceleration of regexp searching
   1.44   wildcard matching & searching; regexp matching & searching
   1.43   stb_temp
   1.42   allow stb_arr to use stb_malloc/realloc; note this is global
   1.41   make it compile in C++; (disable stb_arr in C++)
   1.40   stb_dupe tweak; stb_swap; stb_substr
   1.39   stb_dupe; improve stb_file_max to be less stupid
   1.38   stb_sha1_file: generate sha1 for file, even > 4GB
   1.37   stb_file_max; partial support for utf8 filenames in Windows
   1.36   remove STB__NO_PREFIX - poor interaction with IDE, not worth it
          streamline stb_arr to make it separately publishable
   1.35   bugfixes for stb_sdict, stb_malloc(0), stristr
   1.34   (streaming interfaces for stb_compress)
   1.33   stb_alloc; bug in stb_getopt; remove stb_overflow
   1.32   (stb_compress returns, smaller&faster; encode window & 64-bit len)
   1.31   stb_prefix_count
   1.30   (STB__NO_PREFIX - remove stb_ prefixes for personal projects)
   1.29   stb_fput_varlen64, etc.
   1.28   stb_sha1
   1.27   ?
   1.26   stb_extra
   1.25   ?
   1.24   stb_copyfile
   1.23   stb_readdir
   1.22   ?
   1.21   ?
   1.20   ?
   1.19   ?
   1.18   ?
   1.17   ?
   1.16   ?
   1.15   stb_fixpath, stb_splitpath, stb_strchr2
   1.14   stb_arr
   1.13   ?stb, stb_log, stb_fatal
   1.12   ?stb_hash2
   1.11   miniML
   1.10   stb_crc32, stb_adler32
   1.09   stb_sdict
   1.08   stb_bitreverse, stb_ispow2, stb_big32
          stb_fopen, stb_fput_varlen, stb_fput_ranged
          stb_fcmp, stb_feq
   1.07   (stb_encompress)
   1.06   stb_compress
   1.05   stb_tokens, (stb_hheap)
   1.04   stb_rand
   1.03   ?(s-strings)
   1.02   ?stb_filelen, stb_tokens
   1.01   stb_tolower
   1.00   stb_hash, stb_intcmp
          stb_file, stb_stringfile, stb_fgets
          stb_prefix, stb_strlower, stb_strtok
          stb_image
          (stb_array), (stb_arena)

Parenthesized items have since been removed.

LICENSE

 See end of file for license information.

CREDITS

 Written by Sean Barrett.

 Fixes:
  Philipp Wiesemann
  Robert Nix
  r-lyeh
  blackpawn
  github:Mojofreem
  Ryan Whitworth
  Vincent Isambart
  Mike Sartain
  Eugene Opalev
  Tim Sjostrand
  github:infatum
  Dave Butler (Croepha)
  Ethan Lee (flibitijibibo)
  Brian Collins
  Kyle Langley
*/

#include <stdarg.h>

#ifndef STB__INCLUDE_STB_H
#define STB__INCLUDE_STB_H

#define STB_VERSION  1

#ifdef STB_INTROSPECT
#define STB_DEFINE
#endif

#ifdef STB_DEFINE_THREADS
#ifndef STB_DEFINE
#define STB_DEFINE
#endif
#ifndef STB_THREADS
#define STB_THREADS
#endif
#endif

#if defined(_WIN32) && !defined(__MINGW32__)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif
#if !defined(_MSC_VER) || _MSC_VER > 1700
#include <intrin.h> // _BitScanReverse
#endif
#endif

#include <stdlib.h>     // stdlib could have min/max
#include <stdio.h>      // need FILE
#include <string.h>     // stb_define_hash needs memcpy/memset
#include <time.h>       // stb_dirtree
#ifdef __MINGW32__
#include <fcntl.h>   // O_RDWR
#endif

#ifdef STB_PERSONAL
typedef int Bool;
#define False 0
#define True 1
#endif

#ifdef STB_MALLOC_WRAPPER_PAGED
#define STB_MALLOC_WRAPPER_DEBUG
#endif
#ifdef STB_MALLOC_WRAPPER_DEBUG
#define STB_MALLOC_WRAPPER
#endif
#ifdef STB_MALLOC_WRAPPER_FASTMALLOC
#define STB_FASTMALLOC
#define STB_MALLOC_WRAPPER
#endif

#ifdef STB_FASTMALLOC
#ifndef _WIN32
#undef STB_FASTMALLOC
#endif
#endif

#ifdef STB_DEFINE
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>      // _mktemp
#include <direct.h>  // _rmdir
#endif
#include <sys/types.h> // stat()/_stat()
#include <sys/stat.h>  // stat()/_stat()
#endif

#define stb_min(a,b)   ((a) < (b) ? (a) : (b))
#define stb_max(a,b)   ((a) > (b) ? (a) : (b))

#ifndef STB_ONLY
#if !defined(__cplusplus) && !defined(min) && !defined(max)
#define min(x,y) stb_min(x,y)
#define max(x,y) stb_max(x,y)
#endif

#ifndef M_PI
#define M_PI  3.14159265358979323846f
#endif

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#ifndef deg2rad
#define deg2rad(a)  ((a)*(M_PI/180))
#endif
#ifndef rad2deg
#define rad2deg(a)  ((a)*(180/M_PI))
#endif

#ifndef swap
#ifndef __cplusplus
#define swap(TYPE,a,b)  \
               do { TYPE stb__t; stb__t = (a); (a) = (b); (b) = stb__t; } while (0)
#endif
#endif

typedef unsigned char  uint8;
typedef   signed char   int8;
typedef unsigned short uint16;
typedef   signed short  int16;
#if defined(STB_USE_LONG_FOR_32_BIT_INT) || defined(STB_LONG32)
typedef unsigned long  uint32;
typedef   signed long   int32;
#else
typedef unsigned int   uint32;
typedef   signed int    int32;
#endif

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

// produce compile errors if the sizes aren't right
typedef char stb__testsize16[sizeof(int16) == 2];
typedef char stb__testsize32[sizeof(int32) == 4];
#endif

#ifndef STB_TRUE
#define STB_TRUE 1
#define STB_FALSE 0
#endif

// if we're STB_ONLY, can't rely on uint32 or even uint, so all the
// variables we'll use herein need typenames prefixed with 'stb':
typedef unsigned char stb_uchar;
typedef unsigned char stb_uint8;
typedef unsigned int  stb_uint;
typedef unsigned short stb_uint16;
typedef          short stb_int16;
typedef   signed char  stb_int8;
#if defined(STB_USE_LONG_FOR_32_BIT_INT) || defined(STB_LONG32)
typedef unsigned long  stb_uint32;
typedef          long  stb_int32;
#else
typedef unsigned int   stb_uint32;
typedef          int   stb_int32;
#endif
typedef char stb__testsize2_16[sizeof(stb_uint16) == 2 ? 1 : -1];
typedef char stb__testsize2_32[sizeof(stb_uint32) == 4 ? 1 : -1];

#ifdef _MSC_VER
typedef unsigned __int64 stb_uint64;
typedef          __int64 stb_int64;
#define STB_IMM_UINT64(literalui64) (literalui64##ui64)
#define STB_IMM_INT64(literali64) (literali64##i64)
#else
// ??
typedef unsigned long long stb_uint64;
typedef          long long stb_int64;
#define STB_IMM_UINT64(literalui64) (literalui64##ULL)
#define STB_IMM_INT64(literali64) (literali64##LL)
#endif
typedef char stb__testsize2_64[sizeof(stb_uint64) == 8 ? 1 : -1];

// add platform-specific ways of checking for sizeof(char*) == 8,
// and make those define STB_PTR64
#if defined(_WIN64) || defined(__x86_64__) || defined(__ia64__) || defined(__LP64__)
#define STB_PTR64
#endif

#ifdef STB_PTR64
typedef char stb__testsize2_ptr[sizeof(char*) == 8];
typedef stb_uint64 stb_uinta;
typedef stb_int64  stb_inta;
#else
typedef char stb__testsize2_ptr[sizeof(char*) == 4];
typedef stb_uint32 stb_uinta;
typedef stb_int32  stb_inta;
#endif
typedef char stb__testsize2_uinta[sizeof(stb_uinta) == sizeof(char*) ? 1 : -1];

// if so, we should define an int type that is the pointer size. until then,
// we'll have to make do with this (which is not the same at all!)

typedef union
{
    unsigned int i;
    void* p;
} stb_uintptr;


#ifdef __cplusplus
#define STB_EXTERN   extern "C"
#else
#define STB_EXTERN   extern
#endif

// check for well-known debug defines
#if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#ifndef NDEBUG
#define STB_DEBUG
#endif
#endif

#ifdef STB_DEBUG
#include <assert.h>
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                         C library function platform handling
//

#ifdef STB_DEFINE

#if defined(_WIN32) &&  defined(__STDC_WANT_SECURE_LIB__)
static FILE* stb_p_fopen(const char* filename, const char* mode)
{
    FILE* f;
    if (0 == fopen_s(&f, filename, mode))
        return f;
    else
        return NULL;
}
static FILE* stb_p_wfopen(const wchar_t* filename, const wchar_t* mode)
{
    FILE* f;
    if (0 == _wfopen_s(&f, filename, mode))
        return f;
    else
        return NULL;
}
static char* stb_p_strcpy_s(char* a, size_t size, const char* b)
{
    strcpy_s(a, size, b);
    return a;
}
static char* stb_p_strncpy_s(char* a, size_t size, const char* b, size_t count)
{
    strncpy_s(a, size, b, count);
    return a;
}
#define stb_p_mktemp(s)  (_mktemp_s(s, strlen(s)+1) == 0)
#define stb_p_sprintf    sprintf_s
#define stb_p_size(x)    ,(x)
#else
#define stb_p_fopen      fopen
#define stb_p_wfopen     _wfopen
#define stb_p_strcpy_s(a,s,b)     strcpy(a,b)
#define stb_p_strncpy_s(a,s,b,c)  strncpy(a,b,c)
#define stb_p_mktemp(s)  (mktemp(s) != NULL)

#define stb_p_sprintf    sprintf
#define stb_p_size(x)
#endif

#if defined(_WIN32)
#define stb_p_vsnprintf  _vsnprintf
#else
#define stb_p_vsnprintf  vsnprintf
#endif
#endif // STB_DEFINE

#if defined(_WIN32) && (_MSC_VER >= 1300)
#define stb_p_stricmp    _stricmp
#define stb_p_strnicmp   _strnicmp
#define stb_p_strdup     _strdup
#else
#define stb_p_strdup     strdup
#define stb_p_stricmp    stricmp
#define stb_p_strnicmp   strnicmp
#endif

STB_EXTERN void stb_wrapper_malloc(void* newp, size_t sz, char* file, int line);
STB_EXTERN void stb_wrapper_free(void* oldp, char* file, int line);
STB_EXTERN void stb_wrapper_realloc(void* oldp, void* newp, size_t sz, char* file, int line);
STB_EXTERN void stb_wrapper_calloc(size_t num, size_t sz, char* file, int line);
STB_EXTERN void stb_wrapper_listall(void (*func)(void* ptr, size_t sz, char* file, int line));
STB_EXTERN void stb_wrapper_dump(char* filename);
STB_EXTERN size_t stb_wrapper_allocsize(void* oldp);
STB_EXTERN void stb_wrapper_check(void* oldp);

#ifdef STB_DEFINE
// this is a special function used inside malloc wrapper
// to do allocations that aren't tracked (to avoid
// reentrancy). Of course if someone _else_ wraps realloc,
// this breaks, but if they're doing that AND the malloc
// wrapper they need to explicitly check for reentrancy.
//
// only define realloc_raw() and we do realloc(NULL,sz)
// for malloc() and realloc(p,0) for free().
static void* stb__realloc_raw(void* p, int sz)
{
    if (p == NULL) return malloc(sz);
    if (sz == 0) { free(p); return NULL; }
    return realloc(p, sz);
}
#endif

#ifdef _WIN32
STB_EXTERN void* stb_smalloc(size_t sz);
STB_EXTERN void   stb_sfree(void* p);
STB_EXTERN void* stb_srealloc(void* p, size_t sz);
STB_EXTERN void* stb_scalloc(size_t n, size_t sz);
STB_EXTERN char* stb_sstrdup(char* s);
#endif

#ifdef STB_FASTMALLOC
#define malloc  stb_smalloc
#define free    stb_sfree
#define realloc stb_srealloc
#define strdup  stb_sstrdup
#define calloc  stb_scalloc
#endif

#ifndef STB_MALLOC_ALLCHECK
#define stb__check(p)  1
#else
#ifndef STB_MALLOC_WRAPPER
#error STB_MALLOC_ALLCHECK requires STB_MALLOC_WRAPPER
#else
#define stb__check(p) stb_mcheck(p)
#endif
#endif

#ifdef STB_MALLOC_WRAPPER
STB_EXTERN void* stb__malloc(size_t, char*, int);
STB_EXTERN void* stb__realloc(void*, size_t, char*, int);
STB_EXTERN void* stb__calloc(size_t n, size_t s, char*, int);
STB_EXTERN void   stb__free(void*, char* file, int);
STB_EXTERN char* stb__strdup(char* s, char* file, int);
STB_EXTERN void   stb_malloc_checkall(void);
STB_EXTERN void   stb_malloc_check_counter(int init_delay, int rep_delay);
#ifndef STB_MALLOC_WRAPPER_DEBUG
#define stb_mcheck(p) 1
#else
STB_EXTERN int   stb_mcheck(void*);
#endif


#ifdef STB_DEFINE

#ifdef STB_MALLOC_WRAPPER_DEBUG
#define STB__PAD   32
#define STB__BIAS  16
#define STB__SIG   0x51b01234
#define STB__FIXSIZE(sz)  (((sz+3) & ~3) + STB__PAD)
#define STB__ptr(x,y)   ((char *) (x) + (y))
#else
#define STB__ptr(x,y)   (x)
#define STB__FIXSIZE(sz)  (sz)
#endif

#ifdef STB_MALLOC_WRAPPER_DEBUG
int stb_mcheck(void* p)
{
    unsigned int sz;
    if (p == NULL) return 1;
    p = ((char*)p) - STB__BIAS;
    sz = *(unsigned int*)p;
    assert(*(unsigned int*)STB__ptr(p, 4) == STB__SIG);
    assert(*(unsigned int*)STB__ptr(p, 8) == STB__SIG);
    assert(*(unsigned int*)STB__ptr(p, 12) == STB__SIG);
    assert(*(unsigned int*)STB__ptr(p, sz - 4) == STB__SIG + 1);
    assert(*(unsigned int*)STB__ptr(p, sz - 8) == STB__SIG + 1);
    assert(*(unsigned int*)STB__ptr(p, sz - 12) == STB__SIG + 1);
    assert(*(unsigned int*)STB__ptr(p, sz - 16) == STB__SIG + 1);
    stb_wrapper_check(STB__ptr(p, STB__BIAS));
    return 1;
}

static void stb__check2(void* p, size_t sz, char* file, int line)
{
    stb_mcheck(p);
}

void stb_malloc_checkall(void)
{
    stb_wrapper_listall(stb__check2);
}
#else
void stb_malloc_checkall(void) { }
#endif

static int stb__malloc_wait = (1 << 30), stb__malloc_next_wait = (1 << 30), stb__malloc_iter;
void stb_malloc_check_counter(int init_delay, int rep_delay)
{
    stb__malloc_wait = init_delay;
    stb__malloc_next_wait = rep_delay;
}

void stb_mcheck_all(void)
{
#ifdef STB_MALLOC_WRAPPER_DEBUG
    ++stb__malloc_iter;
    if (--stb__malloc_wait <= 0) {
        stb_malloc_checkall();
        stb__malloc_wait = stb__malloc_next_wait;
    }
#endif
}

#ifdef STB_MALLOC_WRAPPER_PAGED
#define STB__WINDOWS_PAGE (1 << 12)
#ifndef _WINDOWS_
STB_EXTERN __declspec(dllimport) void* __stdcall VirtualAlloc(void* p, unsigned long size, unsigned long type, unsigned long protect);
STB_EXTERN __declspec(dllimport) int   __stdcall VirtualFree(void* p, unsigned long size, unsigned long freetype);
#endif
#endif

static void* stb__malloc_final(size_t sz)
{
#ifdef STB_MALLOC_WRAPPER_PAGED
    size_t aligned = (sz + STB__WINDOWS_PAGE - 1) & ~(STB__WINDOWS_PAGE - 1);
    char* p = VirtualAlloc(NULL, aligned + STB__WINDOWS_PAGE, 0x2000, 0x04); // RESERVE, READWRITE
    if (p == NULL) return p;
    VirtualAlloc(p, aligned, 0x1000, 0x04); // COMMIT, READWRITE
    return p;
#else
    return malloc(sz);
#endif
}

static void stb__free_final(void* p)
{
#ifdef STB_MALLOC_WRAPPER_PAGED
    VirtualFree(p, 0, 0x8000); // RELEASE
#else
    free(p);
#endif
}

int stb__malloc_failure;
#ifdef STB_MALLOC_WRAPPER_PAGED
static void* stb__realloc_final(void* p, size_t sz, size_t old_sz)
{
    void* q = stb__malloc_final(sz);
    if (q == NULL)
        return ++stb__malloc_failure, q;
    // @TODO: deal with p being smaller!
    memcpy(q, p, sz < old_sz ? sz : old_sz);
    stb__free_final(p);
    return q;
}
#endif

void stb__free(void* p, char* file, int line)
{
    stb_mcheck_all();
    if (!p) return;
#ifdef STB_MALLOC_WRAPPER_DEBUG
    stb_mcheck(p);
#endif
    stb_wrapper_free(p, file, line);
#ifdef STB_MALLOC_WRAPPER_DEBUG
    p = STB__ptr(p, -STB__BIAS);
    *(unsigned int*)STB__ptr(p, 0) = 0xdeadbeef;
    *(unsigned int*)STB__ptr(p, 4) = 0xdeadbeef;
    *(unsigned int*)STB__ptr(p, 8) = 0xdeadbeef;
    *(unsigned int*)STB__ptr(p, 12) = 0xdeadbeef;
#endif
    stb__free_final(p);
}

void* stb__malloc(size_t sz, char* file, int line)
{
    void* p;
    stb_mcheck_all();
    if (sz == 0) return NULL;
    p = stb__malloc_final(STB__FIXSIZE(sz));
    if (p == NULL) p = stb__malloc_final(STB__FIXSIZE(sz));
    if (p == NULL) p = stb__malloc_final(STB__FIXSIZE(sz));
    if (p == NULL) {
        ++stb__malloc_failure;
#ifdef STB_MALLOC_WRAPPER_DEBUG
        stb_malloc_checkall();
#endif
        return p;
    }
#ifdef STB_MALLOC_WRAPPER_DEBUG
    * (int*)STB__ptr(p, 0) = STB__FIXSIZE(sz);
    *(unsigned int*)STB__ptr(p, 4) = STB__SIG;
    *(unsigned int*)STB__ptr(p, 8) = STB__SIG;
    *(unsigned int*)STB__ptr(p, 12) = STB__SIG;
    *(unsigned int*)STB__ptr(p, STB__FIXSIZE(sz) - 4) = STB__SIG + 1;
    *(unsigned int*)STB__ptr(p, STB__FIXSIZE(sz) - 8) = STB__SIG + 1;
    *(unsigned int*)STB__ptr(p, STB__FIXSIZE(sz) - 12) = STB__SIG + 1;
    *(unsigned int*)STB__ptr(p, STB__FIXSIZE(sz) - 16) = STB__SIG + 1;
    p = STB__ptr(p, STB__BIAS);
#endif
    stb_wrapper_malloc(p, sz, file, line);
    return p;
}

void* stb__realloc(void* p, size_t sz, char* file, int line)
{
    void* q;

    stb_mcheck_all();
    if (p == NULL) return stb__malloc(sz, file, line);
    if (sz == 0) { stb__free(p, file, line); return NULL; }

#ifdef STB_MALLOC_WRAPPER_DEBUG
    stb_mcheck(p);
    p = STB__ptr(p, -STB__BIAS);
#endif
#ifdef STB_MALLOC_WRAPPER_PAGED
    {
        size_t n = stb_wrapper_allocsize(STB__ptr(p, STB__BIAS));
        if (!n)
            stb_wrapper_check(STB__ptr(p, STB__BIAS));
        q = stb__realloc_final(p, STB__FIXSIZE(sz), STB__FIXSIZE(n));
    }
#else
    q = realloc(p, STB__FIXSIZE(sz));
#endif
    if (q == NULL)
        return ++stb__malloc_failure, q;
#ifdef STB_MALLOC_WRAPPER_DEBUG
    * (int*)STB__ptr(q, 0) = STB__FIXSIZE(sz);
    *(unsigned int*)STB__ptr(q, 4) = STB__SIG;
    *(unsigned int*)STB__ptr(q, 8) = STB__SIG;
    *(unsigned int*)STB__ptr(q, 12) = STB__SIG;
    *(unsigned int*)STB__ptr(q, STB__FIXSIZE(sz) - 4) = STB__SIG + 1;
    *(unsigned int*)STB__ptr(q, STB__FIXSIZE(sz) - 8) = STB__SIG + 1;
    *(unsigned int*)STB__ptr(q, STB__FIXSIZE(sz) - 12) = STB__SIG + 1;
    *(unsigned int*)STB__ptr(q, STB__FIXSIZE(sz) - 16) = STB__SIG + 1;

    q = STB__ptr(q, STB__BIAS);
    p = STB__ptr(p, STB__BIAS);
#endif
    stb_wrapper_realloc(p, q, sz, file, line);
    return q;
}

STB_EXTERN int stb_log2_ceil(size_t);
static void* stb__calloc(size_t n, size_t sz, char* file, int line)
{
    void* q;
    stb_mcheck_all();
    if (n == 0 || sz == 0) return NULL;
    if (stb_log2_ceil(n) + stb_log2_ceil(sz) >= 32) return NULL;
    q = stb__malloc(n * sz, file, line);
    if (q) memset(q, 0, n * sz);
    return q;
}

char* stb__strdup(char* s, char* file, int line)
{
    char* p;
    stb_mcheck_all();
    p = stb__malloc(strlen(s) + 1, file, line);
    if (!p) return p;
    stb_p_strcpy_s(p, strlen(s) + 1, s);
    return p;
}
#endif // STB_DEFINE

#ifdef STB_FASTMALLOC
#undef malloc
#undef realloc
#undef free
#undef strdup
#undef calloc
#endif

// include everything that might define these, BEFORE making macros
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define malloc(s)      stb__malloc (  s, __FILE__, __LINE__)
#define realloc(p,s)   stb__realloc(p,s, __FILE__, __LINE__)
#define calloc(n,s)    stb__calloc (n,s, __FILE__, __LINE__)
#define free(p)        stb__free   (p,   __FILE__, __LINE__)
#define strdup(p)      stb__strdup (p,   __FILE__, __LINE__)
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                         Windows pretty display
//

STB_EXTERN void stbprint(const char* fmt, ...);
STB_EXTERN char* stb_sprintf(const char* fmt, ...);
STB_EXTERN char* stb_mprintf(const char* fmt, ...);
STB_EXTERN int  stb_snprintf(char* s, size_t n, const char* fmt, ...);
STB_EXTERN int  stb_vsnprintf(char* s, size_t n, const char* fmt, va_list v);

#ifdef STB_DEFINE
int stb_vsnprintf(char* s, size_t n, const char* fmt, va_list v)
{
    int res;
#ifdef _WIN32
#ifdef __STDC_WANT_SECURE_LIB__
    res = _vsnprintf_s(s, n, _TRUNCATE, fmt, v);
#else
    res = stb_p_vsnprintf(s, n, fmt, v);
#endif
#else
    res = vsnprintf(s, n, fmt, v);
#endif
    if (n) s[n - 1] = 0;
    // Unix returns length output would require, Windows returns negative when truncated.
    return (res >= (int)n || res < 0) ? -1 : res;
}

int stb_snprintf(char* s, size_t n, const char* fmt, ...)
{
    int res;
    va_list v;
    va_start(v, fmt);
    res = stb_vsnprintf(s, n, fmt, v);
    va_end(v);
    return res;
}

char* stb_sprintf(const char* fmt, ...)
{
    static char buffer[1024];
    va_list v;
    va_start(v, fmt);
    stb_vsnprintf(buffer, 1024, fmt, v);
    va_end(v);
    return buffer;
}

char* stb_mprintf(const char* fmt, ...)
{
    static char buffer[1024];
    va_list v;
    va_start(v, fmt);
    stb_vsnprintf(buffer, 1024, fmt, v);
    va_end(v);
    return stb_p_strdup(buffer);
}

#ifdef _WIN32

#ifndef _WINDOWS_
STB_EXTERN __declspec(dllimport) int __stdcall WriteConsoleA(void*, const void*, unsigned int, unsigned int*, void*);
STB_EXTERN __declspec(dllimport) void* __stdcall GetStdHandle(unsigned int);
STB_EXTERN __declspec(dllimport) int __stdcall SetConsoleTextAttribute(void*, unsigned short);
#endif

static void stb__print_one(void* handle, char* s, ptrdiff_t  len)
{
    if (len)
        if (0 == WriteConsoleA(handle, s, (unsigned)len, NULL, NULL))
            // if it fails, maybe redirected, so output normally...
            // but it's supriously reporting failure now on Win7 and later
        {
        }//fwrite(s, 1, (unsigned) len, stdout);
}

static void stb__print(char* s)
{
    void* handle = GetStdHandle((unsigned int)-11); // STD_OUTPUT_HANDLE
    int pad = 0; // number of padding characters to add

    char* t = s;
    while (*s) {
        int lpad;
        while (*s && *s != '{') {
            if (pad) {
                if (*s == '\r' || *s == '\n')
                    pad = 0;
                else if (s[0] == ' ' && s[1] == ' ') {
                    stb__print_one(handle, t, s - t);
                    t = s;
                    while (pad) {
                        stb__print_one(handle, t, 1);
                        --pad;
                    }
                }
            }
            ++s;
        }
        if (!*s) break;
        stb__print_one(handle, t, s - t);
        if (s[1] == '{') {
            ++s;
            continue;
        }

        if (s[1] == '#') {
            t = s + 3;
            if (isxdigit(s[2]))
                if (isdigit(s[2]))
                    SetConsoleTextAttribute(handle, s[2] - '0');
                else
                    SetConsoleTextAttribute(handle, tolower(s[2]) - 'a' + 10);
            else {
                SetConsoleTextAttribute(handle, 0x0f);
                t = s + 2;
            }
        }
        else if (s[1] == '!') {
            SetConsoleTextAttribute(handle, 0x0c);
            t = s + 2;
        }
        else if (s[1] == '@') {
            SetConsoleTextAttribute(handle, 0x09);
            t = s + 2;
        }
        else if (s[1] == '$') {
            SetConsoleTextAttribute(handle, 0x0a);
            t = s + 2;
        }
        else {
            SetConsoleTextAttribute(handle, 0x08); // 0,7,8,15 => shades of grey
            t = s + 1;
        }

        lpad = (int)(t - s);
        s = t;
        while (*s && *s != '}') ++s;
        if (!*s) break;
        stb__print_one(handle, t, s - t);
        if (s[1] == '}') {
            t = s + 2;
        }
        else {
            pad += 1 + lpad;
            t = s + 1;
        }
        s = t;
        SetConsoleTextAttribute(handle, 0x07);
    }
    stb__print_one(handle, t, s - t);
    SetConsoleTextAttribute(handle, 0x07);
}

void stbprint(const char* fmt, ...)
{
    int res;
    char buffer[1024];
    char* tbuf = buffer;
    va_list v;

    va_start(v, fmt);
    res = stb_vsnprintf(buffer, sizeof(buffer), fmt, v);
    va_end(v);

    if (res < 0) {
        tbuf = (char*)malloc(16384);
        va_start(v, fmt);
        res = stb_vsnprintf(tbuf, 16384, fmt, v);
        va_end(v);
        tbuf[16383] = 0;
    }

    stb__print(tbuf);

    if (tbuf != buffer)
        free(tbuf);
}

#else  // _WIN32
void stbprint(const char* fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    vprintf(fmt, v);
    va_end(v);
}
#endif // _WIN32
#endif // STB_DEFINE



//////////////////////////////////////////////////////////////////////////////
//
//                         Windows UTF8 filename handling
//
// Windows stupidly treats 8-bit filenames as some dopey code page,
// rather than utf-8. If we want to use utf8 filenames, we have to
// convert them to WCHAR explicitly and call WCHAR versions of the
// file functions. So, ok, we do.


#ifdef _WIN32
#define stb__fopen(x,y)    stb_p_wfopen((const wchar_t *)stb__from_utf8(x), (const wchar_t *)stb__from_utf8_alt(y))
#define stb__windows(x,y)  x
#else
#define stb__fopen(x,y)    stb_p_fopen(x,y)
#define stb__windows(x,y)  y
#endif


typedef unsigned short stb__wchar;

STB_EXTERN stb__wchar* stb_from_utf8(stb__wchar* buffer, const char* str, int n);
STB_EXTERN char* stb_to_utf8(char* buffer, const stb__wchar* str, int n);

STB_EXTERN stb__wchar* stb__from_utf8(const char* str);
STB_EXTERN stb__wchar* stb__from_utf8_alt(const char* str);
STB_EXTERN char* stb__to_utf8(const stb__wchar* str);


#ifdef STB_DEFINE
stb__wchar* stb_from_utf8(stb__wchar* buffer, const char* ostr, int n)
{
    unsigned char* str = (unsigned char*)ostr;
    stb_uint32 c;
    int i = 0;
    --n;
    while (*str) {
        if (i >= n)
            return NULL;
        if (!(*str & 0x80))
            buffer[i++] = *str++;
        else if ((*str & 0xe0) == 0xc0) {
            if (*str < 0xc2) return NULL;
            c = (*str++ & 0x1f) << 6;
            if ((*str & 0xc0) != 0x80) return NULL;
            buffer[i++] = c + (*str++ & 0x3f);
        }
        else if ((*str & 0xf0) == 0xe0) {
            if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return NULL;
            if (*str == 0xed && str[1] > 0x9f) return NULL; // str[1] < 0x80 is checked below
            c = (*str++ & 0x0f) << 12;
            if ((*str & 0xc0) != 0x80) return NULL;
            c += (*str++ & 0x3f) << 6;
            if ((*str & 0xc0) != 0x80) return NULL;
            buffer[i++] = c + (*str++ & 0x3f);
        }
        else if ((*str & 0xf8) == 0xf0) {
            if (*str > 0xf4) return NULL;
            if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return NULL;
            if (*str == 0xf4 && str[1] > 0x8f) return NULL; // str[1] < 0x80 is checked below
            c = (*str++ & 0x07) << 18;
            if ((*str & 0xc0) != 0x80) return NULL;
            c += (*str++ & 0x3f) << 12;
            if ((*str & 0xc0) != 0x80) return NULL;
            c += (*str++ & 0x3f) << 6;
            if ((*str & 0xc0) != 0x80) return NULL;
            c += (*str++ & 0x3f);
            // utf-8 encodings of values used in surrogate pairs are invalid
            if ((c & 0xFFFFF800) == 0xD800) return NULL;
            if (c >= 0x10000) {
                c -= 0x10000;
                if (i + 2 > n) return NULL;
                buffer[i++] = 0xD800 | (0x3ff & (c >> 10));
                buffer[i++] = 0xDC00 | (0x3ff & (c));
            }
        }
        else
            return NULL;
    }
    buffer[i] = 0;
    return buffer;
}

char* stb_to_utf8(char* buffer, const stb__wchar* str, int n)
{
    int i = 0;
    --n;
    while (*str) {
        if (*str < 0x80) {
            if (i + 1 > n) return NULL;
            buffer[i++] = (char)*str++;
        }
        else if (*str < 0x800) {
            if (i + 2 > n) return NULL;
            buffer[i++] = 0xc0 + (*str >> 6);
            buffer[i++] = 0x80 + (*str & 0x3f);
            str += 1;
        }
        else if (*str >= 0xd800 && *str < 0xdc00) {
            stb_uint32 c;
            if (i + 4 > n) return NULL;
            c = ((str[0] - 0xd800) << 10) + ((str[1]) - 0xdc00) + 0x10000;
            buffer[i++] = 0xf0 + (c >> 18);
            buffer[i++] = 0x80 + ((c >> 12) & 0x3f);
            buffer[i++] = 0x80 + ((c >> 6) & 0x3f);
            buffer[i++] = 0x80 + ((c) & 0x3f);
            str += 2;
        }
        else if (*str >= 0xdc00 && *str < 0xe000) {
            return NULL;
        }
        else {
            if (i + 3 > n) return NULL;
            buffer[i++] = 0xe0 + (*str >> 12);
            buffer[i++] = 0x80 + ((*str >> 6) & 0x3f);
            buffer[i++] = 0x80 + ((*str) & 0x3f);
            str += 1;
        }
    }
    buffer[i] = 0;
    return buffer;
}

stb__wchar* stb__from_utf8(const char* str)
{
    static stb__wchar buffer[4096];
    return stb_from_utf8(buffer, str, 4096);
}

stb__wchar* stb__from_utf8_alt(const char* str)
{
    static stb__wchar buffer[4096];
    return stb_from_utf8(buffer, str, 4096);
}

char* stb__to_utf8(const stb__wchar* str)
{
    static char buffer[4096];
    return stb_to_utf8(buffer, str, 4096);
}

#endif

//////////////////////////////////////////////////////////////////////////////
//
//                         Miscellany
//

STB_EXTERN void stb_fatal(const char* fmt, ...);
STB_EXTERN void stb_(char* fmt, ...);
STB_EXTERN void stb_append_to_file(char* file, char* fmt, ...);
STB_EXTERN void stb_log(int active);
STB_EXTERN void stb_log_fileline(int active);
STB_EXTERN void stb_log_name(char* filename);

STB_EXTERN void stb_swap(void* p, void* q, size_t sz);
STB_EXTERN void* stb_copy(void* p, size_t sz);
STB_EXTERN void stb_pointer_array_free(void* p, int len);
STB_EXTERN void** stb_array_block_alloc(int count, int blocksize);

#define stb_arrcount(x)   (sizeof(x)/sizeof((x)[0]))


STB_EXTERN int  stb__record_fileline(const char* f, int n);

#ifdef STB_DEFINE

static char* stb__file;
static int   stb__line;

int  stb__record_fileline(const char* f, int n)
{
    stb__file = (char*)f;
    stb__line = n;
    return 0;
}

void stb_fatal(const char* s, ...)
{
    va_list a;
    if (stb__file)
        fprintf(stderr, "[%s:%d] ", stb__file, stb__line);
    va_start(a, s);
    fputs("Fatal error: ", stderr);
    vfprintf(stderr, s, a);
    va_end(a);
    fputs("\n", stderr);
#ifdef STB_DEBUG
#ifdef _MSC_VER
#ifndef STB_PTR64
    __asm int 3;   // trap to debugger!
#else
    __debugbreak();
#endif
#else
    __builtin_trap();
#endif
#endif
    exit(1);
}

static int stb__log_active = 1, stb__log_fileline = 1;

void stb_log(int active)
{
    stb__log_active = active;
}

void stb_log_fileline(int active)
{
    stb__log_fileline = active;
}

#ifdef STB_NO_STB_STRINGS
const char* stb__log_filename = "temp.log";
#else
const char* stb__log_filename = "stb.log";
#endif

void stb_log_name(char* s)
{
    stb__log_filename = s;
}

void stb_(char* s, ...)
{
    if (stb__log_active) {
        FILE* f = stb_p_fopen(stb__log_filename, "a");
        if (f) {
            va_list a;
            if (stb__log_fileline && stb__file)
                fprintf(f, "[%s:%4d] ", stb__file, stb__line);
            va_start(a, s);
            vfprintf(f, s, a);
            va_end(a);
            fputs("\n", f);
            fclose(f);
        }
    }
}

void stb_append_to_file(char* filename, char* s, ...)
{
    FILE* f = stb_p_fopen(filename, "a");
    if (f) {
        va_list a;
        va_start(a, s);
        vfprintf(f, s, a);
        va_end(a);
        fputs("\n", f);
        fclose(f);
    }
}


typedef struct { char d[4]; } stb__4;
typedef struct { char d[8]; } stb__8;

// optimize the small cases, though you shouldn't be calling this for those!
void stb_swap(void* p, void* q, size_t sz)
{
    char buffer[256];
    if (p == q) return;
    if (sz == 4) {
        stb__4 temp = *(stb__4*)p;
        *(stb__4*)p = *(stb__4*)q;
        *(stb__4*)q = temp;
        return;
    }
    else if (sz == 8) {
        stb__8 temp = *(stb__8*)p;
        *(stb__8*)p = *(stb__8*)q;
        *(stb__8*)q = temp;
        return;
    }

    while (sz > sizeof(buffer)) {
        stb_swap(p, q, sizeof(buffer));
        p = (char*)p + sizeof(buffer);
        q = (char*)q + sizeof(buffer);
        sz -= sizeof(buffer);
    }

    memcpy(buffer, p, sz);
    memcpy(p, q, sz);
    memcpy(q, buffer, sz);
}

void* stb_copy(void* p, size_t sz)
{
    void* q = malloc(sz);
    memcpy(q, p, sz);
    return q;
}

void stb_pointer_array_free(void* q, int len)
{
    void** p = (void**)q;
    int i;
    for (i = 0; i < len; ++i)
        free(p[i]);
}

void** stb_array_block_alloc(int count, int blocksize)
{
    int i;
    char* p = (char*)malloc(sizeof(void*) * count + count * blocksize);
    void** q;
    if (p == NULL) return NULL;
    q = (void**)p;
    p += sizeof(void*) * count;
    for (i = 0; i < count; ++i)
        q[i] = p + i * blocksize;
    return q;
}
#endif

#ifdef STB_DEBUG
// tricky hack to allow recording FILE,LINE even in varargs functions
#define STB__RECORD_FILE(x)  (stb__record_fileline(__FILE__, __LINE__),(x))
#define stb_log              STB__RECORD_FILE(stb_log)
#define stb_                 STB__RECORD_FILE(stb_)
#ifndef STB_FATAL_CLEAN
#define stb_fatal            STB__RECORD_FILE(stb_fatal)
#endif
#define STB__DEBUG(x)        x
#else
#define STB__DEBUG(x)
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                         stb_temp
//

#define stb_temp(block, sz)     stb__temp(block, sizeof(block), (sz))

STB_EXTERN void* stb__temp(void* b, int b_sz, int want_sz);
STB_EXTERN void   stb_tempfree(void* block, void* ptr);

#ifdef STB_DEFINE

void* stb__temp(void* b, int b_sz, int want_sz)
{
    if (b_sz >= want_sz)
        return b;
    else
        return malloc(want_sz);
}

void   stb_tempfree(void* b, void* p)
{
    if (p != b)
        free(p);
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
//                      math/sampling operations
//


#define stb_lerp(t,a,b)               ( (a) + (t) * (float) ((b)-(a)) )
#define stb_unlerp(t,a,b)             ( ((t) - (a)) / (float) ((b) - (a)) )

#define stb_clamp(x,xmin,xmax)  ((x) < (xmin) ? (xmin) : (x) > (xmax) ? (xmax) : (x))

STB_EXTERN void stb_newell_normal(float* normal, int num_vert, float** vert, int normalize);
STB_EXTERN int stb_box_face_vertex_axis_side(int face_number, int vertex_number, int axis);
STB_EXTERN void stb_linear_controller(float* curpos, float target_pos, float acc, float deacc, float dt);

STB_EXTERN int stb_float_eq(float x, float y, float delta, int max_ulps);
STB_EXTERN int stb_is_prime(unsigned int m);
STB_EXTERN unsigned int stb_power_of_two_nearest_prime(int n);

STB_EXTERN float stb_smoothstep(float t);
STB_EXTERN float stb_cubic_bezier_1d(float t, float p0, float p1, float p2, float p3);

STB_EXTERN double stb_linear_remap(double x, double a, double b,
    double c, double d);

#ifdef STB_DEFINE
float stb_smoothstep(float t)
{
    return (3 - 2 * t) * (t * t);
}

float stb_cubic_bezier_1d(float t, float p0, float p1, float p2, float p3)
{
    float it = 1 - t;
    return it * it * it * p0 + 3 * it * it * t * p1 + 3 * it * t * t * p2 + t * t * t * p3;
}

void stb_newell_normal(float* normal, int num_vert, float** vert, int normalize)
{
    int i, j;
    float p;
    normal[0] = normal[1] = normal[2] = 0;
    for (i = num_vert - 1, j = 0; j < num_vert; i = j++) {
        float* u = vert[i];
        float* v = vert[j];
        normal[0] += (u[1] - v[1]) * (u[2] + v[2]);
        normal[1] += (u[2] - v[2]) * (u[0] + v[0]);
        normal[2] += (u[0] - v[0]) * (u[1] + v[1]);
    }
    if (normalize) {
        p = normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2];
        p = (float)(1.0 / sqrt(p));
        normal[0] *= p;
        normal[1] *= p;
        normal[2] *= p;
    }
}

int stb_box_face_vertex_axis_side(int face_number, int vertex_number, int axis)
{
    static int box_vertices[6][4][3] =
    {
       { { 1,1,1 }, { 1,0,1 }, { 1,0,0 }, { 1,1,0 } },
       { { 0,0,0 }, { 0,0,1 }, { 0,1,1 }, { 0,1,0 } },
       { { 0,0,0 }, { 0,1,0 }, { 1,1,0 }, { 1,0,0 } },
       { { 0,0,0 }, { 1,0,0 }, { 1,0,1 }, { 0,0,1 } },
       { { 1,1,1 }, { 0,1,1 }, { 0,0,1 }, { 1,0,1 } },
       { { 1,1,1 }, { 1,1,0 }, { 0,1,0 }, { 0,1,1 } },
    };
    assert(face_number >= 0 && face_number < 6);
    assert(vertex_number >= 0 && vertex_number < 4);
    assert(axis >= 0 && axis < 3);
    return box_vertices[face_number][vertex_number][axis];
}

void stb_linear_controller(float* curpos, float target_pos, float acc, float deacc, float dt)
{
    float sign = 1, p, cp = *curpos;
    if (cp == target_pos) return;
    if (target_pos < cp) {
        target_pos = -target_pos;
        cp = -cp;
        sign = -1;
    }
    // first decelerate
    if (cp < 0) {
        p = cp + deacc * dt;
        if (p > 0) {
            p = 0;
            dt = dt - cp / deacc;
            if (dt < 0) dt = 0;
        }
        else {
            dt = 0;
        }
        cp = p;
    }
    // now accelerate
    p = cp + acc * dt;
    if (p > target_pos) p = target_pos;
    *curpos = p * sign;
    // @TODO: testing
}

float stb_quadratic_controller(float target_pos, float curpos, float maxvel, float maxacc, float dt, float* curvel)
{
    return 0; // @TODO
}

int stb_float_eq(float x, float y, float delta, int max_ulps)
{
    if (fabs(x - y) <= delta) return 1;
    if (abs(*(int*)&x - *(int*)&y) <= max_ulps) return 1;
    return 0;
}

int stb_is_prime(unsigned int m)
{
    unsigned int i, j;
    if (m < 2) return 0;
    if (m == 2) return 1;
    if (!(m & 1)) return 0;
    if (m % 3 == 0) return (m == 3);
    for (i = 5; (j = i * i), j <= m && j > i; i += 6) {
        if (m % i == 0) return 0;
        if (m % (i + 2) == 0) return 0;
    }
    return 1;
}

unsigned int stb_power_of_two_nearest_prime(int n)
{
    static signed char tab[32] = { 0,0,0,0,1,0,-1,0,1,-1,-1,3,-1,0,-1,2,1,
                                    0,2,0,-1,-4,-1,5,-1,18,-2,15,2,-1,2,0 };
    if (!tab[0]) {
        int i;
        for (i = 0; i < 32; ++i)
            tab[i] = (1 << i) + 2 * tab[i] - 1;
        tab[1] = 2;
        tab[0] = 1;
    }
    if (n >= 32) return 0xfffffffb;
    return tab[n];
}

double stb_linear_remap(double x, double x_min, double x_max,
    double out_min, double out_max)
{
    return stb_lerp(stb_unlerp(x, x_min, x_max), out_min, out_max);
}
#endif

// create a macro so it's faster, but you can get at the function pointer
#define stb_linear_remap(t,a,b,c,d)   stb_lerp(stb_unlerp(t,a,b),c,d)


//////////////////////////////////////////////////////////////////////////////
//
//                         bit operations
//

#define stb_big32(c)    (((c)[0]<<24) + (c)[1]*65536 + (c)[2]*256 + (c)[3])
#define stb_little32(c) (((c)[3]<<24) + (c)[2]*65536 + (c)[1]*256 + (c)[0])
#define stb_big16(c)    ((c)[0]*256 + (c)[1])
#define stb_little16(c) ((c)[1]*256 + (c)[0])

STB_EXTERN          int stb_bitcount(unsigned int a);
STB_EXTERN unsigned int stb_bitreverse8(unsigned char n);
STB_EXTERN unsigned int stb_bitreverse(unsigned int n);

STB_EXTERN          int stb_is_pow2(size_t);
STB_EXTERN          int stb_log2_ceil(size_t);
STB_EXTERN          int stb_log2_floor(size_t);

STB_EXTERN          int stb_lowbit8(unsigned int n);
STB_EXTERN          int stb_highbit8(unsigned int n);

#ifdef STB_DEFINE
int stb_bitcount(unsigned int a)
{
    a = (a & 0x55555555) + ((a >> 1) & 0x55555555); // max 2
    a = (a & 0x33333333) + ((a >> 2) & 0x33333333); // max 4
    a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
    a = (a + (a >> 8)); // max 16 per 8 bits
    a = (a + (a >> 16)); // max 32 per 8 bits
    return a & 0xff;
}

unsigned int stb_bitreverse8(unsigned char n)
{
    n = ((n & 0xAA) >> 1) + ((n & 0x55) << 1);
    n = ((n & 0xCC) >> 2) + ((n & 0x33) << 2);
    return (unsigned char)((n >> 4) + (n << 4));
}

unsigned int stb_bitreverse(unsigned int n)
{
    n = ((n & 0xAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);
    n = ((n & 0xCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);
    n = ((n & 0xF0F0F0F0) >> 4) | ((n & 0x0F0F0F0F) << 4);
    n = ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);
    return (n >> 16) | (n << 16);
}

int stb_is_pow2(size_t n)
{
    return (n & (n - 1)) == 0;
}

// tricky use of 4-bit table to identify 5 bit positions (note the '-1')
// 3-bit table would require another tree level; 5-bit table wouldn't save one
#if defined(_WIN32) && !defined(__MINGW32__)
#pragma warning(push)
#pragma warning(disable: 4035)  // disable warning about no return value
int stb_log2_floor(size_t n)
{
#if _MSC_VER > 1700
    unsigned long i;
#ifdef STB_PTR64
    _BitScanReverse64(&i, n);
#else
    _BitScanReverse(&i, n);
#endif
    return i != 0 ? i : -1;
#else
    __asm {
        bsr eax, n
        jnz done
        mov eax, -1
    }
done:;
#endif
}
#pragma warning(pop)
#else
int stb_log2_floor(size_t n)
{
    static signed char log2_4[16] = { -1,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3 };

#ifdef STB_PTR64
    if (n >= ((size_t)1u << 32))
        return stb_log2_floor(n >> 32);
#endif

    // 2 compares if n < 16, 3 compares otherwise
    if (n < (1U << 14))
        if (n < (1U << 4))        return     0 + log2_4[n];
        else if (n < (1U << 9))      return  5 + log2_4[n >> 5];
        else                     return 10 + log2_4[n >> 10];
    else if (n < (1U << 24))
        if (n < (1U << 19))      return 15 + log2_4[n >> 15];
        else                     return 20 + log2_4[n >> 20];
    else if (n < (1U << 29))      return 25 + log2_4[n >> 25];
    else                     return 30 + log2_4[n >> 30];
}
#endif

// define ceil from floor
int stb_log2_ceil(size_t n)
{
    if (stb_is_pow2(n))  return     stb_log2_floor(n);
    else                 return 1 + stb_log2_floor(n);
}

int stb_highbit8(unsigned int n)
{
    return stb_log2_ceil(n & 255);
}

int stb_lowbit8(unsigned int n)
{
    static signed char lowbit4[16] = { -1,0,1,0, 2,0,1,0, 3,0,1,0, 2,0,1,0 };
    int k = lowbit4[n & 15];
    if (k >= 0) return k;
    k = lowbit4[(n >> 4) & 15];
    if (k >= 0) return k + 4;
    return k;
}
#endif



//////////////////////////////////////////////////////////////////////////////
//
//                            qsort Compare Routines
//

#ifdef _WIN32
#define stb_stricmp(a,b) stb_p_stricmp(a,b)
#define stb_strnicmp(a,b,n) stb_p_strnicmp(a,b,n)
#else
#define stb_stricmp(a,b) strcasecmp(a,b)
#define stb_strnicmp(a,b,n) strncasecmp(a,b,n)
#endif


STB_EXTERN int (*stb_intcmp(int offset))(const void* a, const void* b);
STB_EXTERN int (*stb_intcmprev(int offset))(const void* a, const void* b);
STB_EXTERN int (*stb_qsort_strcmp(int offset))(const void* a, const void* b);
STB_EXTERN int (*stb_qsort_stricmp(int offset))(const void* a, const void* b);
STB_EXTERN int (*stb_floatcmp(int offset))(const void* a, const void* b);
STB_EXTERN int (*stb_doublecmp(int offset))(const void* a, const void* b);
STB_EXTERN int (*stb_charcmp(int offset))(const void* a, const void* b);

#ifdef STB_DEFINE
static int stb__intcmpoffset, stb__ucharcmpoffset, stb__strcmpoffset;
static int stb__floatcmpoffset, stb__doublecmpoffset;
static int stb__memcmpoffset, stb__memcmpsize;

int stb__intcmp(const void* a, const void* b)
{
    const int p = *(const int*)((const char*)a + stb__intcmpoffset);
    const int q = *(const int*)((const char*)b + stb__intcmpoffset);
    return p < q ? -1 : p > q;
}

int stb__intcmprev(const void* a, const void* b)
{
    const int p = *(const int*)((const char*)a + stb__intcmpoffset);
    const int q = *(const int*)((const char*)b + stb__intcmpoffset);
    return q < p ? -1 : q > p;
}

int stb__ucharcmp(const void* a, const void* b)
{
    const int p = *(const unsigned char*)((const char*)a + stb__ucharcmpoffset);
    const int q = *(const unsigned char*)((const char*)b + stb__ucharcmpoffset);
    return p < q ? -1 : p > q;
}

int stb__floatcmp(const void* a, const void* b)
{
    const float p = *(const float*)((const char*)a + stb__floatcmpoffset);
    const float q = *(const float*)((const char*)b + stb__floatcmpoffset);
    return p < q ? -1 : p > q;
}

int stb__doublecmp(const void* a, const void* b)
{
    const double p = *(const double*)((const char*)a + stb__doublecmpoffset);
    const double q = *(const double*)((const char*)b + stb__doublecmpoffset);
    return p < q ? -1 : p > q;
}

int stb__qsort_strcmp(const void* a, const void* b)
{
    const char* p = *(const char**)((const char*)a + stb__strcmpoffset);
    const char* q = *(const char**)((const char*)b + stb__strcmpoffset);
    return strcmp(p, q);
}

int stb__qsort_stricmp(const void* a, const void* b)
{
    const char* p = *(const char**)((const char*)a + stb__strcmpoffset);
    const char* q = *(const char**)((const char*)b + stb__strcmpoffset);
    return stb_stricmp(p, q);
}

int stb__memcmp(const void* a, const void* b)
{
    return memcmp((char*)a + stb__memcmpoffset, (char*)b + stb__memcmpoffset, stb__memcmpsize);
}

int (*stb_intcmp(int offset))(const void*, const void*)
{
    stb__intcmpoffset = offset;
    return &stb__intcmp;
}

int (*stb_intcmprev(int offset))(const void*, const void*)
{
    stb__intcmpoffset = offset;
    return &stb__intcmprev;
}

int (*stb_ucharcmp(int offset))(const void*, const void*)
{
    stb__ucharcmpoffset = offset;
    return &stb__ucharcmp;
}

int (*stb_qsort_strcmp(int offset))(const void*, const void*)
{
    stb__strcmpoffset = offset;
    return &stb__qsort_strcmp;
}

int (*stb_qsort_stricmp(int offset))(const void*, const void*)
{
    stb__strcmpoffset = offset;
    return &stb__qsort_stricmp;
}

int (*stb_floatcmp(int offset))(const void*, const void*)
{
    stb__floatcmpoffset = offset;
    return &stb__floatcmp;
}

int (*stb_doublecmp(int offset))(const void*, const void*)
{
    stb__doublecmpoffset = offset;
    return &stb__doublecmp;
}

int (*stb_memcmp(int offset, int size))(const void*, const void*)
{
    stb__memcmpoffset = offset;
    stb__memcmpsize = size;
    return &stb__memcmp;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                           Binary Search Toolkit
//

typedef struct
{
    int minval, maxval, guess;
    int mode, step;
} stb_search;

STB_EXTERN int stb_search_binary(stb_search* s, int minv, int maxv, int find_smallest);
STB_EXTERN int stb_search_open(stb_search* s, int minv, int find_smallest);
STB_EXTERN int stb_probe(stb_search* s, int compare, int* result); // return 0 when done

#ifdef STB_DEFINE
enum
{
    STB_probe_binary_smallest,
    STB_probe_binary_largest,
    STB_probe_open_smallest,
    STB_probe_open_largest,
};

static int stb_probe_guess(stb_search* s, int* result)
{
    switch (s->mode) {
    case STB_probe_binary_largest:
        if (s->minval == s->maxval) {
            *result = s->minval;
            return 0;
        }
        assert(s->minval < s->maxval);
        // if a < b, then a < p <= b
        s->guess = s->minval + (((unsigned)s->maxval - s->minval + 1) >> 1);
        break;

    case STB_probe_binary_smallest:
        if (s->minval == s->maxval) {
            *result = s->minval;
            return 0;
        }
        assert(s->minval < s->maxval);
        // if a < b, then a <= p < b
        s->guess = s->minval + (((unsigned)s->maxval - s->minval) >> 1);
        break;
    case STB_probe_open_smallest:
    case STB_probe_open_largest:
        s->guess = s->maxval;  // guess the current maxval
        break;
    }
    *result = s->guess;
    return 1;
}

int stb_probe(stb_search* s, int compare, int* result)
{
    switch (s->mode) {
    case STB_probe_open_smallest:
    case STB_probe_open_largest: {
        if (compare <= 0) {
            // then it lies within minval & maxval
            if (s->mode == STB_probe_open_smallest)
                s->mode = STB_probe_binary_smallest;
            else
                s->mode = STB_probe_binary_largest;
        }
        else {
            // otherwise, we need to probe larger
            s->minval = s->maxval + 1;
            s->maxval = s->minval + s->step;
            s->step += s->step;
        }
        break;
    }
    case STB_probe_binary_smallest: {
        // if compare < 0, then s->minval <= a <  p
        // if compare = 0, then s->minval <= a <= p
        // if compare > 0, then         p <  a <= s->maxval
        if (compare <= 0)
            s->maxval = s->guess;
        else
            s->minval = s->guess + 1;
        break;
    }
    case STB_probe_binary_largest: {
        // if compare < 0, then s->minval <= a < p
        // if compare = 0, then         p <= a <= s->maxval
        // if compare > 0, then         p <  a <= s->maxval
        if (compare < 0)
            s->maxval = s->guess - 1;
        else
            s->minval = s->guess;
        break;
    }
    }
    return stb_probe_guess(s, result);
}

int stb_search_binary(stb_search* s, int minv, int maxv, int find_smallest)
{
    int r;
    if (maxv < minv) return minv - 1;
    s->minval = minv;
    s->maxval = maxv;
    s->mode = find_smallest ? STB_probe_binary_smallest : STB_probe_binary_largest;
    stb_probe_guess(s, &r);
    return r;
}

int stb_search_open(stb_search* s, int minv, int find_smallest)
{
    int r;
    s->step = 4;
    s->minval = minv;
    s->maxval = minv + s->step;
    s->mode = find_smallest ? STB_probe_open_smallest : STB_probe_open_largest;
    stb_probe_guess(s, &r);
    return r;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                           String Processing
//

#define stb_prefixi(s,t)  (0==stb_strnicmp((s),(t),strlen(t)))

enum stb_splitpath_flag
{
    STB_PATH = 1,
    STB_FILE = 2,
    STB_EXT = 4,
    STB_PATH_FILE = STB_PATH + STB_FILE,
    STB_FILE_EXT = STB_FILE + STB_EXT,
    STB_EXT_NO_PERIOD = 8,
};

STB_EXTERN char* stb_skipwhite(char* s);
STB_EXTERN char* stb_trimwhite(char* s);
STB_EXTERN char* stb_skipnewline(char* s);
STB_EXTERN char* stb_strncpy(char* s, char* t, int n);
STB_EXTERN char* stb_substr(char* t, int n);
STB_EXTERN char* stb_duplower(char* s);
STB_EXTERN void   stb_tolower(char* s);
STB_EXTERN char* stb_strchr2(char* s, char p1, char p2);
STB_EXTERN char* stb_strrchr2(char* s, char p1, char p2);
STB_EXTERN char* stb_strtok(char* output, char* src, char* delimit);
STB_EXTERN char* stb_strtok_keep(char* output, char* src, char* delimit);
STB_EXTERN char* stb_strtok_invert(char* output, char* src, char* allowed);
STB_EXTERN char* stb_dupreplace(char* s, char* find, char* replace);
STB_EXTERN void   stb_replaceinplace(char* s, char* find, char* replace);
STB_EXTERN char* stb_splitpath(char* output, char* src, int flag);
STB_EXTERN char* stb_splitpathdup(char* src, int flag);
STB_EXTERN char* stb_replacedir(char* output, char* src, char* dir);
STB_EXTERN char* stb_replaceext(char* output, char* src, char* ext);
STB_EXTERN void   stb_fixpath(char* path);
STB_EXTERN char* stb_shorten_path_readable(char* path, int max_len);
STB_EXTERN int    stb_suffix(char* s, char* t);
STB_EXTERN int    stb_suffixi(char* s, char* t);
STB_EXTERN int    stb_prefix(char* s, char* t);
STB_EXTERN char* stb_strichr(char* s, char t);
STB_EXTERN char* stb_stristr(char* s, char* t);
STB_EXTERN int    stb_prefix_count(char* s, char* t);
STB_EXTERN const char* stb_plural(int n);  // "s" or ""
STB_EXTERN size_t stb_strscpy(char* d, const char* s, size_t n);

STB_EXTERN char** stb_tokens(char* src, char* delimit, int* count);
STB_EXTERN char** stb_tokens_nested(char* src, char* delimit, int* count, char* nest_in, char* nest_out);
STB_EXTERN char** stb_tokens_nested_empty(char* src, char* delimit, int* count, char* nest_in, char* nest_out);
STB_EXTERN char** stb_tokens_allowempty(char* src, char* delimit, int* count);
STB_EXTERN char** stb_tokens_stripwhite(char* src, char* delimit, int* count);
STB_EXTERN char** stb_tokens_withdelim(char* src, char* delimit, int* count);
STB_EXTERN char** stb_tokens_quoted(char* src, char* delimit, int* count);
// with 'quoted', allow delimiters to appear inside quotation marks, and don't
// strip whitespace inside them (and we delete the quotation marks unless they
// appear back to back, in which case they're considered escaped)

#ifdef STB_DEFINE

size_t stb_strscpy(char* d, const char* s, size_t n)
{
    size_t len = strlen(s);
    if (len >= n) {
        if (n) d[0] = 0;
        return 0;
    }
    stb_p_strcpy_s(d, n, s);
    return len;
}

const char* stb_plural(int n)
{
    return n == 1 ? "" : "s";
}

int stb_prefix(char* s, char* t)
{
    while (*t)
        if (*s++ != *t++)
            return STB_FALSE;
    return STB_TRUE;
}

int stb_prefix_count(char* s, char* t)
{
    int c = 0;
    while (*t) {
        if (*s++ != *t++)
            break;
        ++c;
    }
    return c;
}

int stb_suffix(char* s, char* t)
{
    size_t n = strlen(s);
    size_t m = strlen(t);
    if (m <= n)
        return 0 == strcmp(s + n - m, t);
    else
        return 0;
}

int stb_suffixi(char* s, char* t)
{
    size_t n = strlen(s);
    size_t m = strlen(t);
    if (m <= n)
        return 0 == stb_stricmp(s + n - m, t);
    else
        return 0;
}

// originally I was using this table so that I could create known sentinel
// values--e.g. change whitetable[0] to be true if I was scanning for whitespace,
// and false if I was scanning for nonwhite. I don't appear to be using that
// functionality anymore (I do for tokentable, though), so just replace it
// with isspace()
char* stb_skipwhite(char* s)
{
    while (isspace((unsigned char)*s)) ++s;
    return s;
}

char* stb_skipnewline(char* s)
{
    if (s[0] == '\r' || s[0] == '\n') {
        if (s[0] + s[1] == '\r' + '\n') ++s;
        ++s;
    }
    return s;
}

char* stb_trimwhite(char* s)
{
    int i, n;
    s = stb_skipwhite(s);
    n = (int)strlen(s);
    for (i = n - 1; i >= 0; --i)
        if (!isspace(s[i]))
            break;
    s[i + 1] = 0;
    return s;
}

char* stb_strncpy(char* s, char* t, int n)
{
    stb_p_strncpy_s(s, n + 1, t, n);
    s[n] = 0;
    return s;
}

char* stb_substr(char* t, int n)
{
    char* a;
    int z = (int)strlen(t);
    if (z < n) n = z;
    a = (char*)malloc(n + 1);
    stb_p_strncpy_s(a, n + 1, t, n);
    a[n] = 0;
    return a;
}

char* stb_duplower(char* s)
{
    char* p = stb_p_strdup(s), * q = p;
    while (*q) {
        *q = tolower(*q);
        ++q;
    }
    return p;
}

void stb_tolower(char* s)
{
    while (*s) {
        *s = tolower(*s);
        ++s;
    }
}

char* stb_strchr2(char* s, char x, char y)
{
    for (; *s; ++s)
        if (*s == x || *s == y)
            return s;
    return NULL;
}

char* stb_strrchr2(char* s, char x, char y)
{
    char* r = NULL;
    for (; *s; ++s)
        if (*s == x || *s == y)
            r = s;
    return r;
}

char* stb_strichr(char* s, char t)
{
    if (tolower(t) == toupper(t))
        return strchr(s, t);
    return stb_strchr2(s, (char)tolower(t), (char)toupper(t));
}

char* stb_stristr(char* s, char* t)
{
    size_t n = strlen(t);
    char* z;
    if (n == 0) return s;
    while ((z = stb_strichr(s, *t)) != NULL) {
        if (0 == stb_strnicmp(z, t, n))
            return z;
        s = z + 1;
    }
    return NULL;
}

static char* stb_strtok_raw(char* output, char* src, char* delimit, int keep, int invert)
{
    if (invert) {
        while (*src && strchr(delimit, *src) != NULL) {
            *output++ = *src++;
        }
    }
    else {
        while (*src && strchr(delimit, *src) == NULL) {
            *output++ = *src++;
        }
    }
    *output = 0;
    if (keep)
        return src;
    else
        return *src ? src + 1 : src;
}

char* stb_strtok(char* output, char* src, char* delimit)
{
    return stb_strtok_raw(output, src, delimit, 0, 0);
}

char* stb_strtok_keep(char* output, char* src, char* delimit)
{
    return stb_strtok_raw(output, src, delimit, 1, 0);
}

char* stb_strtok_invert(char* output, char* src, char* delimit)
{
    return stb_strtok_raw(output, src, delimit, 1, 1);
}

static char** stb_tokens_raw(char* src_, char* delimit, int* count,
    int stripwhite, int allow_empty, char* start, char* end)
{
    int nested = 0;
    unsigned char* src = (unsigned char*)src_;
    static char stb_tokentable[256]; // rely on static initializion to 0
    static char stable[256], etable[256];
    char* out;
    char** result;
    int num = 0;
    unsigned char* s;

    s = (unsigned char*)delimit; while (*s) stb_tokentable[*s++] = 1;
    if (start) {
        s = (unsigned char*)start;         while (*s) stable[*s++] = 1;
        s = (unsigned char*)end;   if (s)  while (*s) stable[*s++] = 1;
        s = (unsigned char*)end;   if (s)  while (*s) etable[*s++] = 1;
    }
    stable[0] = 1;

    // two passes through: the first time, counting how many
    s = (unsigned char*)src;
    while (*s) {
        // state: just found delimiter
        // skip further delimiters
        if (!allow_empty) {
            stb_tokentable[0] = 0;
            while (stb_tokentable[*s])
                ++s;
            if (!*s) break;
        }
        ++num;
        // skip further non-delimiters
        stb_tokentable[0] = 1;
        if (stripwhite == 2) { // quoted strings
            while (!stb_tokentable[*s]) {
                if (*s != '"')
                    ++s;
                else {
                    ++s;
                    if (*s == '"')
                        ++s;   // "" -> ", not start a string
                    else {
                        // begin a string
                        while (*s) {
                            if (s[0] == '"') {
                                if (s[1] == '"') s += 2; // "" -> "
                                else { ++s; break; } // terminating "
                            }
                            else
                                ++s;
                        }
                    }
                }
            }
        }
        else
            while (nested || !stb_tokentable[*s]) {
                if (stable[*s]) {
                    if (!*s) break;
                    if (end ? etable[*s] : nested)
                        --nested;
                    else
                        ++nested;
                }
                ++s;
            }
        if (allow_empty) {
            if (*s) ++s;
        }
    }
    // now num has the actual count... malloc our output structure
    // need space for all the strings: strings won't be any longer than
    // original input, since for every '\0' there's at least one delimiter
    result = (char**)malloc(sizeof(*result) * (num + 1) + (s - src + 1));
    if (result == NULL) return result;
    out = (char*)(result + (num + 1));
    // second pass: copy out the data
    s = (unsigned char*)src;
    num = 0;
    nested = 0;
    while (*s) {
        char* last_nonwhite;
        // state: just found delimiter
        // skip further delimiters
        if (!allow_empty) {
            stb_tokentable[0] = 0;
            if (stripwhite)
                while (stb_tokentable[*s] || isspace(*s))
                    ++s;
            else
                while (stb_tokentable[*s])
                    ++s;
        }
        else if (stripwhite) {
            while (isspace(*s)) ++s;
        }
        if (!*s) break;
        // we're past any leading delimiters and whitespace
        result[num] = out;
        ++num;
        // copy non-delimiters
        stb_tokentable[0] = 1;
        last_nonwhite = out - 1;
        if (stripwhite == 2) {
            while (!stb_tokentable[*s]) {
                if (*s != '"') {
                    if (!isspace(*s)) last_nonwhite = out;
                    *out++ = *s++;
                }
                else {
                    ++s;
                    if (*s == '"') {
                        if (!isspace(*s)) last_nonwhite = out;
                        *out++ = *s++; // "" -> ", not start string
                    }
                    else {
                        // begin a quoted string
                        while (*s) {
                            if (s[0] == '"') {
                                if (s[1] == '"') { *out++ = *s; s += 2; }
                                else { ++s; break; } // terminating "
                            }
                            else
                                *out++ = *s++;
                        }
                        last_nonwhite = out - 1; // all in quotes counts as non-white
                    }
                }
            }
        }
        else {
            while (nested || !stb_tokentable[*s]) {
                if (!isspace(*s)) last_nonwhite = out;
                if (stable[*s]) {
                    if (!*s) break;
                    if (end ? etable[*s] : nested)
                        --nested;
                    else
                        ++nested;
                }
                *out++ = *s++;
            }
        }

        if (stripwhite) // rewind to last non-whitespace char
            out = last_nonwhite + 1;
        *out++ = '\0';

        if (*s) ++s; // skip delimiter
    }
    s = (unsigned char*)delimit; while (*s) stb_tokentable[*s++] = 0;
    if (start) {
        s = (unsigned char*)start;         while (*s) stable[*s++] = 1;
        s = (unsigned char*)end;   if (s)  while (*s) stable[*s++] = 1;
        s = (unsigned char*)end;   if (s)  while (*s) etable[*s++] = 1;
    }
    if (count != NULL) *count = num;
    result[num] = 0;
    return result;
}

char** stb_tokens(char* src, char* delimit, int* count)
{
    return stb_tokens_raw(src, delimit, count, 0, 0, 0, 0);
}

char** stb_tokens_nested(char* src, char* delimit, int* count, char* nest_in, char* nest_out)
{
    return stb_tokens_raw(src, delimit, count, 0, 0, nest_in, nest_out);
}

char** stb_tokens_nested_empty(char* src, char* delimit, int* count, char* nest_in, char* nest_out)
{
    return stb_tokens_raw(src, delimit, count, 0, 1, nest_in, nest_out);
}

char** stb_tokens_allowempty(char* src, char* delimit, int* count)
{
    return stb_tokens_raw(src, delimit, count, 0, 1, 0, 0);
}

char** stb_tokens_stripwhite(char* src, char* delimit, int* count)
{
    return stb_tokens_raw(src, delimit, count, 1, 1, 0, 0);
}

char** stb_tokens_quoted(char* src, char* delimit, int* count)
{
    return stb_tokens_raw(src, delimit, count, 2, 1, 0, 0);
}

char* stb_dupreplace(char* src, char* find, char* replace)
{
    size_t len_find = strlen(find);
    size_t len_replace = strlen(replace);
    int count = 0;

    char* s, * p, * q;

    s = strstr(src, find);
    if (s == NULL) return stb_p_strdup(src);
    do {
        ++count;
        s = strstr(s + len_find, find);
    } while (s != NULL);

    p = (char*)malloc(strlen(src) + count * (len_replace - len_find) + 1);
    if (p == NULL) return p;
    q = p;
    s = src;
    for (;;) {
        char* t = strstr(s, find);
        if (t == NULL) {
            stb_p_strcpy_s(q, strlen(src) + count * (len_replace - len_find) + 1, s);
            assert(strlen(p) == strlen(src) + count * (len_replace - len_find));
            return p;
        }
        memcpy(q, s, t - s);
        q += t - s;
        memcpy(q, replace, len_replace);
        q += len_replace;
        s = t + len_find;
    }
}

void stb_replaceinplace(char* src, char* find, char* replace)
{
    size_t len_find = strlen(find);
    size_t len_replace = strlen(replace);
    int delta;

    char* s, * p, * q;

    delta = (int)(len_replace - len_find);
    assert(delta <= 0);
    if (delta > 0) return;

    p = strstr(src, find);
    if (p == NULL) return;

    s = q = p;
    while (*s) {
        memcpy(q, replace, len_replace);
        p += len_find;
        q += len_replace;
        s = strstr(p, find);
        if (s == NULL) s = p + strlen(p);
        memmove(q, p, s - p);
        q += s - p;
        p = s;
    }
    *q = 0;
}

void stb_fixpath(char* path)
{
    for (; *path; ++path)
        if (*path == '\\')
            *path = '/';
}

void stb__add_section(char* buffer, char* data, ptrdiff_t curlen, ptrdiff_t newlen)
{
    if (newlen < curlen) {
        ptrdiff_t z1 = newlen >> 1, z2 = newlen - z1;
        memcpy(buffer, data, z1 - 1);
        buffer[z1 - 1] = '.';
        buffer[z1 - 0] = '.';
        memcpy(buffer + z1 + 1, data + curlen - z2 + 1, z2 - 1);
    }
    else
        memcpy(buffer, data, curlen);
}

char* stb_shorten_path_readable(char* path, int len)
{
    static char buffer[1024];
    ptrdiff_t n = strlen(path), n1, n2, r1, r2;
    char* s;
    if (n <= len) return path;
    if (len > 1024) return path;
    s = stb_strrchr2(path, '/', '\\');
    if (s) {
        n1 = s - path + 1;
        n2 = n - n1;
        ++s;
    }
    else {
        n1 = 0;
        n2 = n;
        s = path;
    }
    // now we need to reduce r1 and r2 so that they fit in len
    if (n1 < len >> 1) {
        r1 = n1;
        r2 = len - r1;
    }
    else if (n2 < len >> 1) {
        r2 = n2;
        r1 = len - r2;
    }
    else {
        r1 = n1 * len / n;
        r2 = n2 * len / n;
        if (r1 < len >> 2) r1 = len >> 2, r2 = len - r1;
        if (r2 < len >> 2) r2 = len >> 2, r1 = len - r2;
    }
    assert(r1 <= n1 && r2 <= n2);
    if (n1)
        stb__add_section(buffer, path, n1, r1);
    stb__add_section(buffer + r1, s, n2, r2);
    buffer[len] = 0;
    return buffer;
}

static char* stb__splitpath_raw(char* buffer, char* path, int flag)
{
    ptrdiff_t len = 0, x, y, n = (int)strlen(path), f1, f2;
    char* s = stb_strrchr2(path, '/', '\\');
    char* t = strrchr(path, '.');
    if (s && t && t < s) t = NULL;

    if (!s) {
        // check for drive
        if (isalpha(path[0]) && path[1] == ':')
            s = &path[1];
    }
    if (s) ++s;

    if (flag == STB_EXT_NO_PERIOD)
        flag |= STB_EXT;

    if (!(flag & (STB_PATH | STB_FILE | STB_EXT))) return NULL;

    f1 = s == NULL ? 0 : s - path; // start of filename
    f2 = t == NULL ? n : t - path; // just past end of filename

    if (flag & STB_PATH) {
        x = 0; if (f1 == 0 && flag == STB_PATH) len = 2;
    }
    else if (flag & STB_FILE) {
        x = f1;
    }
    else {
        x = f2;
        if (flag & STB_EXT_NO_PERIOD)
            if (path[x] == '.')
                ++x;
    }

    if (flag & STB_EXT)
        y = n;
    else if (flag & STB_FILE)
        y = f2;
    else
        y = f1;

    if (buffer == NULL) {
        buffer = (char*)malloc(y - x + len + 1);
        if (!buffer) return NULL;
    }

    if (len) { stb_p_strcpy_s(buffer, 3, "./"); return buffer; }
    stb_strncpy(buffer, path + (int)x, (int)(y - x));
    return buffer;
}

char* stb_splitpath(char* output, char* src, int flag)
{
    return stb__splitpath_raw(output, src, flag);
}

char* stb_splitpathdup(char* src, int flag)
{
    return stb__splitpath_raw(NULL, src, flag);
}

char* stb_replacedir(char* output, char* src, char* dir)
{
    char buffer[4096];
    stb_splitpath(buffer, src, STB_FILE | STB_EXT);
    if (dir)
        stb_p_sprintf(output stb_p_size(9999), "%s/%s", dir, buffer);
    else
        stb_p_strcpy_s(output, sizeof(buffer), buffer); // @UNSAFE
    return output;
}

char* stb_replaceext(char* output, char* src, char* ext)
{
    char buffer[4096];
    stb_splitpath(buffer, src, STB_PATH | STB_FILE);
    if (ext)
        stb_p_sprintf(output stb_p_size(9999), "%s.%s", buffer, ext[0] == '.' ? ext + 1 : ext);
    else
        stb_p_strcpy_s(output, sizeof(buffer), buffer); // @UNSAFE
    return output;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                   stb_alloc - hierarchical allocator
//
//                                     inspired by http://swapped.cc/halloc
//
//
// When you alloc a given block through stb_alloc, you have these choices:
//
//       1. does it have a parent?
//       2. can it have children?
//       3. can it be freed directly?
//       4. is it transferrable?
//       5. what is its alignment?
//
// Here are interesting combinations of those:
//
//                              children   free    transfer     alignment
//  arena                          Y         Y         N           n/a
//  no-overhead, chunked           N         N         N         normal
//  string pool alloc              N         N         N            1
//  parent-ptr, chunked            Y         N         N         normal
//  low-overhead, unchunked        N         Y         Y         normal
//  general purpose alloc          Y         Y         Y         normal
//
// Unchunked allocations will probably return 16-aligned pointers. If
// we 16-align the results, we have room for 4 pointers. For smaller
// allocations that allow finer alignment, we can reduce the pointers.
//
// The strategy is that given a pointer, assuming it has a header (only
// the no-overhead allocations have no header), we can determine the
// type of the header fields, and the number of them, by stepping backwards
// through memory and looking at the tags in the bottom bits.
//
// Implementation strategy:
//     chunked allocations come from the middle of chunks, and can't
//     be freed. thefore they do not need to be on a sibling chain.
//     they may need child pointers if they have children.
//
// chunked, with-children
//     void *parent;
//
// unchunked, no-children -- reduced storage
//     void *next_sibling;
//     void *prev_sibling_nextp;
//
// unchunked, general
//     void *first_child;
//     void *next_sibling;
//     void *prev_sibling_nextp;
//     void *chunks;
//
// so, if we code each of these fields with different bit patterns
// (actually same one for next/prev/child), then we can identify which
// each one is from the last field.

STB_EXTERN void  stb_free(void* p);
STB_EXTERN void* stb_malloc_global(size_t size);
STB_EXTERN void* stb_malloc(void* context, size_t size);
STB_EXTERN void* stb_malloc_nofree(void* context, size_t size);
STB_EXTERN void* stb_malloc_leaf(void* context, size_t size);
STB_EXTERN void* stb_malloc_raw(void* context, size_t size);
STB_EXTERN void* stb_realloc(void* ptr, size_t newsize);

STB_EXTERN void stb_reassign(void* new_context, void* ptr);
STB_EXTERN void stb_malloc_validate(void* p, void* parent);

extern int stb_alloc_chunk_size;
extern int stb_alloc_count_free;
extern int stb_alloc_count_alloc;
extern int stb_alloc_alignment;

#ifdef STB_DEFINE

int stb_alloc_chunk_size = 65536;
int stb_alloc_count_free = 0;
int stb_alloc_count_alloc = 0;
int stb_alloc_alignment = -16;

typedef struct stb__chunk
{
    struct stb__chunk* next;
    int                data_left;
    int                alloc;
} stb__chunk;

typedef struct
{
    void* next;
    void** prevn;
} stb__nochildren;

typedef struct
{
    void** prevn;
    void* child;
    void* next;
    stb__chunk* chunks;
} stb__alloc;

typedef struct
{
    stb__alloc* parent;
} stb__chunked;

#define STB__PARENT          1
#define STB__CHUNKS          2

typedef enum
{
    STB__nochildren = 0,
    STB__chunked = STB__PARENT,
    STB__alloc = STB__CHUNKS,

    STB__chunk_raw = 4,
} stb__alloc_type;

// these functions set the bottom bits of a pointer efficiently
#define STB__DECODE(x,v)  ((void *) ((char *) (x) - (v)))
#define STB__ENCODE(x,v)  ((void *) ((char *) (x) + (v)))

#define stb__parent(z)       (stb__alloc *) STB__DECODE((z)->parent, STB__PARENT)
#define stb__chunks(z)       (stb__chunk *) STB__DECODE((z)->chunks, STB__CHUNKS)

#define stb__setparent(z,p)  (z)->parent = (stb__alloc *) STB__ENCODE((p), STB__PARENT)
#define stb__setchunks(z,c)  (z)->chunks = (stb__chunk *) STB__ENCODE((c), STB__CHUNKS)

static stb__alloc stb__alloc_global =
{
   NULL,
   NULL,
   NULL,
   (stb__chunk*)STB__ENCODE(NULL, STB__CHUNKS)
};

static stb__alloc_type stb__identify(void* p)
{
    void** q = (void**)p;
    return (stb__alloc_type)((stb_uinta)q[-1] & 3);
}

static void*** stb__prevn(void* p)
{
    if (stb__identify(p) == STB__alloc) {
        stb__alloc* s = (stb__alloc*)p - 1;
        return &s->prevn;
    }
    else {
        stb__nochildren* s = (stb__nochildren*)p - 1;
        return &s->prevn;
    }
}

void stb_free(void* p)
{
    if (p == NULL) return;

    // count frees so that unit tests can see what's happening
    ++stb_alloc_count_free;

    switch (stb__identify(p)) {
    case STB__chunked:
        // freeing a chunked-block with children does nothing;
        // they only get freed when the parent does
        // surely this is wrong, and it should free them immediately?
        // otherwise how are they getting put on the right chain?
        return;
    case STB__nochildren: {
        stb__nochildren* s = (stb__nochildren*)p - 1;
        // unlink from sibling chain
        *(s->prevn) = s->next;
        if (s->next)
            *stb__prevn(s->next) = s->prevn;
        free(s);
        return;
    }
    case STB__alloc: {
        stb__alloc* s = (stb__alloc*)p - 1;
        stb__chunk* c, * n;
        void* q;

        // unlink from sibling chain, if any
        *(s->prevn) = s->next;
        if (s->next)
            *stb__prevn(s->next) = s->prevn;

        // first free chunks
        c = (stb__chunk*)stb__chunks(s);
        while (c != NULL) {
            n = c->next;
            stb_alloc_count_free += c->alloc;
            free(c);
            c = n;
        }

        // validating
        stb__setchunks(s, NULL);
        s->prevn = NULL;
        s->next = NULL;

        // now free children
        while ((q = s->child) != NULL) {
            stb_free(q);
        }

        // now free self
        free(s);
        return;
    }
    default:
        assert(0); /* NOTREACHED */
    }
}

void stb_malloc_validate(void* p, void* parent)
{
    if (p == NULL) return;

    switch (stb__identify(p)) {
    case STB__chunked:
        return;
    case STB__nochildren: {
        stb__nochildren* n = (stb__nochildren*)p - 1;
        if (n->prevn)
            assert(*n->prevn == p);
        if (n->next) {
            assert(*stb__prevn(n->next) == &n->next);
            stb_malloc_validate(n, parent);
        }
        return;
    }
    case STB__alloc: {
        stb__alloc* s = (stb__alloc*)p - 1;

        if (s->prevn)
            assert(*s->prevn == p);

        if (s->child) {
            assert(*stb__prevn(s->child) == &s->child);
            stb_malloc_validate(s->child, p);
        }

        if (s->next) {
            assert(*stb__prevn(s->next) == &s->next);
            stb_malloc_validate(s->next, parent);
        }
        return;
    }
    default:
        assert(0); /* NOTREACHED */
    }
}

static void* stb__try_chunk(stb__chunk* c, int size, int align, int pre_align)
{
    char* memblock = (char*)(c + 1), * q;
    stb_inta iq;
    int start_offset;

    // we going to allocate at the end of the chunk, not the start. confusing,
    // but it means we don't need both a 'limit' and a 'cur', just a 'cur'.
    // the block ends at: p + c->data_left
    //   then we move back by size
    start_offset = c->data_left - size;

    // now we need to check the alignment of that
    q = memblock + start_offset;
    iq = (stb_inta)q;
    assert(sizeof(q) == sizeof(iq));

    // suppose align = 2
    // then we need to retreat iq far enough that (iq & (2-1)) == 0
    // to get (iq & (align-1)) = 0 requires subtracting (iq & (align-1))

    start_offset -= iq & (align - 1);
    assert(((stb_uinta)(memblock + start_offset) & (align - 1)) == 0);

    // now, if that + pre_align works, go for it!
    start_offset -= pre_align;

    if (start_offset >= 0) {
        c->data_left = start_offset;
        return memblock + start_offset;
    }

    return NULL;
}

static void stb__sort_chunks(stb__alloc* src)
{
    // of the first two chunks, put the chunk with more data left in it first
    stb__chunk* c = stb__chunks(src), * d;
    if (c == NULL) return;
    d = c->next;
    if (d == NULL) return;
    if (c->data_left > d->data_left) return;

    c->next = d->next;
    d->next = c;
    stb__setchunks(src, d);
}

static void* stb__alloc_chunk(stb__alloc* src, int size, int align, int pre_align)
{
    void* p;
    stb__chunk* c = stb__chunks(src);

    if (c && size <= stb_alloc_chunk_size) {

        p = stb__try_chunk(c, size, align, pre_align);
        if (p) { ++c->alloc; return p; }

        // try a second chunk to reduce wastage
        if (c->next) {
            p = stb__try_chunk(c->next, size, align, pre_align);
            if (p) { ++c->alloc; return p; }

            // put the bigger chunk first, since the second will get buried
            // the upshot of this is that, until it gets allocated from, chunk #2
            // is always the largest remaining chunk. (could formalize
            // this with a heap!)
            stb__sort_chunks(src);
            c = stb__chunks(src);
        }
    }

    // allocate a new chunk
    {
        stb__chunk* n;

        int chunk_size = stb_alloc_chunk_size;
        // we're going to allocate a new chunk to put this in
        if (size > chunk_size)
            chunk_size = size;

        assert(sizeof(*n) + pre_align <= 16);

        // loop trying to allocate a large enough chunk
        // the loop is because the alignment may cause problems if it's big...
        // and we don't know what our chunk alignment is going to be
        while (1) {
            n = (stb__chunk*)malloc(16 + chunk_size);
            if (n == NULL) return NULL;

            n->data_left = chunk_size - sizeof(*n);

            p = stb__try_chunk(n, size, align, pre_align);
            if (p != NULL) {
                n->next = c;
                stb__setchunks(src, n);

                // if we just used up the whole block immediately,
                // move the following chunk up
                n->alloc = 1;
                if (size == chunk_size)
                    stb__sort_chunks(src);

                return p;
            }

            free(n);
            chunk_size += 16 + align;
        }
    }
}

static stb__alloc* stb__get_context(void* context)
{
    if (context == NULL) {
        return &stb__alloc_global;
    }
    else {
        int u = stb__identify(context);
        // if context is chunked, grab parent
        if (u == STB__chunked) {
            stb__chunked* s = (stb__chunked*)context - 1;
            return stb__parent(s);
        }
        else {
            return (stb__alloc*)context - 1;
        }
    }
}

static void stb__insert_alloc(stb__alloc* src, stb__alloc* s)
{
    s->prevn = &src->child;
    s->next = src->child;
    src->child = s + 1;
    if (s->next)
        *stb__prevn(s->next) = &s->next;
}

static void stb__insert_nochild(stb__alloc* src, stb__nochildren* s)
{
    s->prevn = &src->child;
    s->next = src->child;
    src->child = s + 1;
    if (s->next)
        *stb__prevn(s->next) = &s->next;
}

static void* malloc_base(void* context, size_t size, stb__alloc_type t, int align)
{
    void* p;

    stb__alloc* src = stb__get_context(context);

    if (align <= 0) {
        // compute worst-case C packed alignment
        // e.g. a 24-byte struct is 8-aligned
        int align_proposed = 1 << stb_lowbit8((unsigned int)size);

        if (align_proposed < 0)
            align_proposed = 4;

        if (align_proposed == 0) {
            if (size == 0)
                align_proposed = 1;
            else
                align_proposed = 256;
        }

        // a negative alignment means 'don't align any larger
        // than this'; so -16 means we align 1,2,4,8, or 16

        if (align < 0) {
            if (align_proposed > -align)
                align_proposed = -align;
        }

        align = align_proposed;
    }

    assert(stb_is_pow2(align));

    // don't cause misalignment when allocating nochildren
    if (t == STB__nochildren && align > 8)
        t = STB__alloc;

    switch (t) {
    case STB__alloc: {
        stb__alloc* s = (stb__alloc*)malloc(size + sizeof(*s));
        if (s == NULL) return NULL;
        p = s + 1;
        s->child = NULL;
        stb__insert_alloc(src, s);

        stb__setchunks(s, NULL);
        break;
    }

    case STB__nochildren: {
        stb__nochildren* s = (stb__nochildren*)malloc(size + sizeof(*s));
        if (s == NULL) return NULL;
        p = s + 1;
        stb__insert_nochild(src, s);
        break;
    }

    case STB__chunk_raw: {
        p = stb__alloc_chunk(src, (int)size, align, 0);
        if (p == NULL) return NULL;
        break;
    }

    case STB__chunked: {
        stb__chunked* s;
        if (align < sizeof(stb_uintptr)) align = sizeof(stb_uintptr);
        s = (stb__chunked*)stb__alloc_chunk(src, (int)size, align, sizeof(*s));
        if (s == NULL) return NULL;
        stb__setparent(s, src);
        p = s + 1;
        break;
    }

    default: p = NULL; assert(0); /* NOTREACHED */
    }

    ++stb_alloc_count_alloc;
    return p;
}

void* stb_malloc_global(size_t size)
{
    return malloc_base(NULL, size, STB__alloc, stb_alloc_alignment);
}

void* stb_malloc(void* context, size_t size)
{
    return malloc_base(context, size, STB__alloc, stb_alloc_alignment);
}

void* stb_malloc_nofree(void* context, size_t size)
{
    return malloc_base(context, size, STB__chunked, stb_alloc_alignment);
}

void* stb_malloc_leaf(void* context, size_t size)
{
    return malloc_base(context, size, STB__nochildren, stb_alloc_alignment);
}

void* stb_malloc_raw(void* context, size_t size)
{
    return malloc_base(context, size, STB__chunk_raw, stb_alloc_alignment);
}

char* stb_malloc_string(void* context, size_t size)
{
    return (char*)malloc_base(context, size, STB__chunk_raw, 1);
}

void* stb_realloc(void* ptr, size_t newsize)
{
    stb__alloc_type t;

    if (ptr == NULL) return stb_malloc(NULL, newsize);
    if (newsize == 0) { stb_free(ptr); return NULL; }

    t = stb__identify(ptr);
    assert(t == STB__alloc || t == STB__nochildren);

    if (t == STB__alloc) {
        stb__alloc* s = (stb__alloc*)ptr - 1;

        s = (stb__alloc*)realloc(s, newsize + sizeof(*s));
        if (s == NULL) return NULL;

        ptr = s + 1;

        // update pointers
        (*s->prevn) = ptr;
        if (s->next)
            *stb__prevn(s->next) = &s->next;

        if (s->child)
            *stb__prevn(s->child) = &s->child;

        return ptr;
    }
    else {
        stb__nochildren* s = (stb__nochildren*)ptr - 1;

        s = (stb__nochildren*)realloc(ptr, newsize + sizeof(s));
        if (s == NULL) return NULL;

        // update pointers
        (*s->prevn) = s + 1;
        if (s->next)
            *stb__prevn(s->next) = &s->next;

        return s + 1;
    }
}

void* stb_realloc_c(void* context, void* ptr, size_t newsize)
{
    if (ptr == NULL) return stb_malloc(context, newsize);
    if (newsize == 0) { stb_free(ptr); return NULL; }
    // @TODO: verify you haven't changed contexts
    return stb_realloc(ptr, newsize);
}

void stb_reassign(void* new_context, void* ptr)
{
    stb__alloc* src = stb__get_context(new_context);

    stb__alloc_type t = stb__identify(ptr);
    assert(t == STB__alloc || t == STB__nochildren);

    if (t == STB__alloc) {
        stb__alloc* s = (stb__alloc*)ptr - 1;

        // unlink from old
        *(s->prevn) = s->next;
        if (s->next)
            *stb__prevn(s->next) = s->prevn;

        stb__insert_alloc(src, s);
    }
    else {
        stb__nochildren* s = (stb__nochildren*)ptr - 1;

        // unlink from old
        *(s->prevn) = s->next;
        if (s->next)
            *stb__prevn(s->next) = s->prevn;

        stb__insert_nochild(src, s);
    }
}

#endif


//////////////////////////////////////////////////////////////////////////////
//
//                                stb_arr
//
//  An stb_arr is directly useable as a pointer (use the actual type in your
//  definition), but when it resizes, it returns a new pointer and you can't
//  use the old one, so you have to be careful to copy-in-out as necessary.
//
//  Use a NULL pointer as a 0-length array.
//
//     float *my_array = NULL, *temp;
//
//     // add elements on the end one at a time
//     stb_arr_push(my_array, 0.0f);
//     stb_arr_push(my_array, 1.0f);
//     stb_arr_push(my_array, 2.0f);
//
//     assert(my_array[1] == 2.0f);
//
//     // add an uninitialized element at the end, then assign it
//     *stb_arr_add(my_array) = 3.0f;
//
//     // add three uninitialized elements at the end
//     temp = stb_arr_addn(my_array,3);
//     temp[0] = 4.0f;
//     temp[1] = 5.0f;
//     temp[2] = 6.0f;
//
//     assert(my_array[5] == 5.0f);
//
//     // remove the last one
//     stb_arr_pop(my_array);
//
//     assert(stb_arr_len(my_array) == 6);


#ifdef STB_MALLOC_WRAPPER
#define STB__PARAMS    , char *file, int line
#define STB__ARGS      ,       file,     line
#else
#define STB__PARAMS
#define STB__ARGS
#endif

// calling this function allocates an empty stb_arr attached to p
// (whereas NULL isn't attached to anything)
STB_EXTERN void stb_arr_malloc(void** target, void* context);

// call this function with a non-NULL value to have all successive
// stbs that are created be attached to the associated parent. Note
// that once a given stb_arr is non-empty, it stays attached to its
// current parent, even if you call this function again.
// it turns the previous value, so you can restore it
STB_EXTERN void* stb_arr_malloc_parent(void* p);

// simple functions written on top of other functions
#define stb_arr_empty(a)       (  stb_arr_len(a) == 0 )
#define stb_arr_add(a)         (  stb_arr_addn((a),1) )
#define stb_arr_push(a,v)      ( *stb_arr_add(a)=(v)  )

typedef struct
{
    int len, limit;
    int stb_malloc;
    unsigned int signature;
} stb__arr;

#define stb_arr_signature      0x51bada7b  // ends with 0123 in decimal

// access the header block stored before the data
#define stb_arrhead(a)         /*lint --e(826)*/ (((stb__arr *) (a)) - 1)
#define stb_arrhead2(a)        /*lint --e(826)*/ (((stb__arr *) (a)) - 1)

#ifdef STB_DEBUG
#define stb_arr_check(a)       assert(!a || stb_arrhead(a)->signature == stb_arr_signature)
#define stb_arr_check2(a)      assert(!a || stb_arrhead2(a)->signature == stb_arr_signature)
#else
#define stb_arr_check(a)       ((void) 0)
#define stb_arr_check2(a)      ((void) 0)
#endif

// ARRAY LENGTH

// get the array length; special case if pointer is NULL
#define stb_arr_len(a)         (a ? stb_arrhead(a)->len : 0)
#define stb_arr_len2(a)        ((stb__arr *) (a) ? stb_arrhead2(a)->len : 0)
#define stb_arr_lastn(a)       (stb_arr_len(a)-1)

// check whether a given index is valid -- tests 0 <= i < stb_arr_len(a)
#define stb_arr_valid(a,i)     (a ? (int) (i) < stb_arrhead(a)->len : 0)

// change the array length so is is exactly N entries long, creating
// uninitialized entries as needed
#define stb_arr_setlen(a,n)  \
            (stb__arr_setlen((void **) &(a), sizeof(a[0]), (n)))

// change the array length so that N is a valid index (that is, so
// it is at least N entries long), creating uninitialized entries as needed
#define stb_arr_makevalid(a,n)  \
            (stb_arr_len(a) < (n)+1 ? stb_arr_setlen(a,(n)+1),(a) : (a))

// remove the last element of the array, returning it
#define stb_arr_pop(a)         ((stb_arr_check(a), (a))[--stb_arrhead(a)->len])

// access the last element in the array
#define stb_arr_last(a)        ((stb_arr_check(a), (a))[stb_arr_len(a)-1])

// is iterator at end of list?
#define stb_arr_end(a,i)       ((i) >= &(a)[stb_arr_len(a)])

// (internal) change the allocated length of the array
#define stb_arr__grow(a,n)     (stb_arr_check(a), stb_arrhead(a)->len += (n))

// add N new uninitialized elements to the end of the array
#define stb_arr__addn(a,n)     /*lint --e(826)*/ \
                               ((stb_arr_len(a)+(n) > stb_arrcurmax(a))      \
                                 ? (stb__arr_addlen((void **) &(a),sizeof(*a),(n)),0) \
                                 : ((stb_arr__grow(a,n), 0)))

// add N new uninitialized elements to the end of the array, and return
// a pointer to the first new one
#define stb_arr_addn(a,n)      (stb_arr__addn((a),n),(a)+stb_arr_len(a)-(n))

// add N new uninitialized elements starting at index 'i'
#define stb_arr_insertn(a,i,n) (stb__arr_insertn((void **) &(a), sizeof(*a), (i), (n)))

// insert an element at i
#define stb_arr_insert(a,i,v)  (stb__arr_insertn((void **) &(a), sizeof(*a), (i), (1)), ((a)[i] = v))

// delete N elements from the middle starting at index 'i'
#define stb_arr_deleten(a,i,n) (stb__arr_deleten((void **) &(a), sizeof(*a), (i), (n)))

// delete the i'th element
#define stb_arr_delete(a,i)   stb_arr_deleten(a,i,1)

// delete the i'th element, swapping down from the end
#define stb_arr_fastdelete(a,i)  \
   (stb_swap(&a[i], &a[stb_arrhead(a)->len-1], sizeof(*a)), stb_arr_pop(a))


// ARRAY STORAGE

// get the array maximum storage; special case if NULL
#define stb_arrcurmax(a)       (a ? stb_arrhead(a)->limit : 0)
#define stb_arrcurmax2(a)      (a ? stb_arrhead2(a)->limit : 0)

// set the maxlength of the array to n in anticipation of further growth
#define stb_arr_setsize(a,n)   (stb_arr_check(a), stb__arr_setsize((void **) &(a),sizeof((a)[0]),n))

// make sure maxlength is large enough for at least N new allocations
#define stb_arr_atleast(a,n)   (stb_arr_len(a)+(n) > stb_arrcurmax(a)      \
                                 ? stb_arr_setsize((a), (n)) : 0)

// make a copy of a given array (copies contents via 'memcpy'!)
#define stb_arr_copy(a)        stb__arr_copy(a, sizeof((a)[0]))

// compute the storage needed to store all the elements of the array
#define stb_arr_storage(a)     (stb_arr_len(a) * sizeof((a)[0]))

#define stb_arr_for(v,arr)     for((v)=(arr); (v) < (arr)+stb_arr_len(arr); ++(v))

// IMPLEMENTATION

STB_EXTERN void stb_arr_free_(void** p);
STB_EXTERN void* stb__arr_copy_(void* p, int elem_size);
STB_EXTERN void stb__arr_setsize_(void** p, int size, int limit  STB__PARAMS);
STB_EXTERN void stb__arr_setlen_(void** p, int size, int newlen  STB__PARAMS);
STB_EXTERN void stb__arr_addlen_(void** p, int size, int addlen  STB__PARAMS);
STB_EXTERN void stb__arr_deleten_(void** p, int size, int loc, int n  STB__PARAMS);
STB_EXTERN void stb__arr_insertn_(void** p, int size, int loc, int n  STB__PARAMS);

#define stb_arr_free(p)            stb_arr_free_((void **) &(p))
#define stb__arr_copy              stb__arr_copy_

#ifndef STB_MALLOC_WRAPPER
#define stb__arr_setsize         stb__arr_setsize_
#define stb__arr_setlen          stb__arr_setlen_
#define stb__arr_addlen          stb__arr_addlen_
#define stb__arr_deleten         stb__arr_deleten_
#define stb__arr_insertn         stb__arr_insertn_
#else
#define stb__arr_addlen(p,s,n)    stb__arr_addlen_(p,s,n,__FILE__,__LINE__)
#define stb__arr_setlen(p,s,n)    stb__arr_setlen_(p,s,n,__FILE__,__LINE__)
#define stb__arr_setsize(p,s,n)   stb__arr_setsize_(p,s,n,__FILE__,__LINE__)
#define stb__arr_deleten(p,s,i,n) stb__arr_deleten_(p,s,i,n,__FILE__,__LINE__)
#define stb__arr_insertn(p,s,i,n) stb__arr_insertn_(p,s,i,n,__FILE__,__LINE__)
#endif

#ifdef STB_DEFINE
static void* stb__arr_context;

void* stb_arr_malloc_parent(void* p)
{
    void* q = stb__arr_context;
    stb__arr_context = p;
    return q;
}

void stb_arr_malloc(void** target, void* context)
{
    stb__arr* q = (stb__arr*)stb_malloc(context, sizeof(*q));
    q->len = q->limit = 0;
    q->stb_malloc = 1;
    q->signature = stb_arr_signature;
    *target = (void*)(q + 1);
}

static void* stb__arr_malloc(int size)
{
    if (stb__arr_context)
        return stb_malloc(stb__arr_context, size);
    return malloc(size);
}

void* stb__arr_copy_(void* p, int elem_size)
{
    stb__arr* q;
    if (p == NULL) return p;
    q = (stb__arr*)stb__arr_malloc(sizeof(*q) + elem_size * stb_arrhead2(p)->limit);
    stb_arr_check2(p);
    memcpy(q, stb_arrhead2(p), sizeof(*q) + elem_size * stb_arrhead2(p)->len);
    q->stb_malloc = !!stb__arr_context;
    return q + 1;
}

void stb_arr_free_(void** pp)
{
    void* p = *pp;
    stb_arr_check2(p);
    if (p) {
        stb__arr* q = stb_arrhead2(p);
        if (q->stb_malloc)
            stb_free(q);
        else
            free(q);
    }
    *pp = NULL;
}

static void stb__arrsize_(void** pp, int size, int limit, int len  STB__PARAMS)
{
    void* p = *pp;
    stb__arr* a;
    stb_arr_check2(p);
    if (p == NULL) {
        if (len == 0 && size == 0) return;
        a = (stb__arr*)stb__arr_malloc(sizeof(*a) + size * limit);
        a->limit = limit;
        a->len = len;
        a->stb_malloc = !!stb__arr_context;
        a->signature = stb_arr_signature;
    }
    else {
        a = stb_arrhead2(p);
        a->len = len;
        if (a->limit < limit) {
            void* p;
            if (a->limit >= 4 && limit < a->limit * 2)
                limit = a->limit * 2;
            if (a->stb_malloc)
                p = stb_realloc(a, sizeof(*a) + limit * size);
            else
#ifdef STB_MALLOC_WRAPPER
                p = stb__realloc(a, sizeof(*a) + limit * size, file, line);
#else
                p = realloc(a, sizeof(*a) + limit * size);
#endif
            if (p) {
                a = (stb__arr*)p;
                a->limit = limit;
            }
            else {
                // throw an error!
            }
        }
    }
    a->len = stb_min(a->len, a->limit);
    *pp = a + 1;
}

void stb__arr_setsize_(void** pp, int size, int limit  STB__PARAMS)
{
    void* p = *pp;
    stb_arr_check2(p);
    stb__arrsize_(pp, size, limit, stb_arr_len2(p)  STB__ARGS);
}

void stb__arr_setlen_(void** pp, int size, int newlen  STB__PARAMS)
{
    void* p = *pp;
    stb_arr_check2(p);
    if (stb_arrcurmax2(p) < newlen || p == NULL) {
        stb__arrsize_(pp, size, newlen, newlen  STB__ARGS);
    }
    else {
        stb_arrhead2(p)->len = newlen;
    }
}

void stb__arr_addlen_(void** p, int size, int addlen  STB__PARAMS)
{
    stb__arr_setlen_(p, size, stb_arr_len2(*p) + addlen  STB__ARGS);
}

void stb__arr_insertn_(void** pp, int size, int i, int n  STB__PARAMS)
{
    void* p = *pp;
    if (n) {
        int z;

        if (p == NULL) {
            stb__arr_addlen_(pp, size, n  STB__ARGS);
            return;
        }

        z = stb_arr_len2(p);
        stb__arr_addlen_(&p, size, n  STB__ARGS);
        memmove((char*)p + (i + n) * size, (char*)p + i * size, size * (z - i));
    }
    *pp = p;
}

void stb__arr_deleten_(void** pp, int size, int i, int n  STB__PARAMS)
{
    void* p = *pp;
    if (n) {
        memmove((char*)p + i * size, (char*)p + (i + n) * size, size * (stb_arr_len2(p) - (i + n)));
        stb_arrhead2(p)->len -= n;
    }
    *pp = p;
}

#endif

//////////////////////////////////////////////////////////////////////////////
//
//                               Hashing
//
//      typical use for this is to make a power-of-two hash table.
//
//      let N = size of table (2^n)
//      let H = stb_hash(str)
//      let S = stb_rehash(H) | 1
//
//      then hash probe sequence P(i) for i=0..N-1
//         P(i) = (H + S*i) & (N-1)
//
//      the idea is that H has 32 bits of hash information, but the
//      table has only, say, 2^20 entries so only uses 20 of the bits.
//      then by rehashing the original H we get 2^12 different probe
//      sequences for a given initial probe location. (So it's optimal
//      for 64K tables and its optimality decreases past that.)
//
//      ok, so I've added something that generates _two separate_
//      32-bit hashes simultaneously which should scale better to
//      very large tables.


STB_EXTERN unsigned int stb_hash(char* str);
STB_EXTERN unsigned int stb_hashptr(void* p);
STB_EXTERN unsigned int stb_hashlen(char* str, int len);
STB_EXTERN unsigned int stb_rehash_improved(unsigned int v);
STB_EXTERN unsigned int stb_hash_fast(void* p, int len);
STB_EXTERN unsigned int stb_hash2(char* str, unsigned int* hash2_ptr);
STB_EXTERN unsigned int stb_hash_number(unsigned int hash);

#define stb_rehash(x)  ((x) + ((x) >> 6) + ((x) >> 19))

#ifdef STB_DEFINE
unsigned int stb_hash(char* str)
{
    unsigned int hash = 0;
    while (*str)
        hash = (hash << 7) + (hash >> 25) + *str++;
    return hash + (hash >> 16);
}

unsigned int stb_hashlen(char* str, int len)
{
    unsigned int hash = 0;
    while (len-- > 0 && *str)
        hash = (hash << 7) + (hash >> 25) + *str++;
    return hash + (hash >> 16);
}

unsigned int stb_hashptr(void* p)
{
    unsigned int x = (unsigned int)(size_t)p;

    // typically lacking in low bits and high bits
    x = stb_rehash(x);
    x += x << 16;

    // pearson's shuffle
    x ^= x << 3;
    x += x >> 5;
    x ^= x << 2;
    x += x >> 15;
    x ^= x << 10;
    return stb_rehash(x);
}

unsigned int stb_rehash_improved(unsigned int v)
{
    return stb_hashptr((void*)(size_t)v);
}

unsigned int stb_hash2(char* str, unsigned int* hash2_ptr)
{
    unsigned int hash1 = 0x3141592c;
    unsigned int hash2 = 0x77f044ed;
    while (*str) {
        hash1 = (hash1 << 7) + (hash1 >> 25) + *str;
        hash2 = (hash2 << 11) + (hash2 >> 21) + *str;
        ++str;
    }
    *hash2_ptr = hash2 + (hash1 >> 16);
    return       hash1 + (hash2 >> 16);
}

// Paul Hsieh hash
#define stb__get16(p) ((p)[0] | ((p)[1] << 8))

unsigned int stb_hash_fast(void* p, int len)
{
    unsigned char* q = (unsigned char*)p;
    unsigned int hash = len;

    if (len <= 0 || q == NULL) return 0;

    /* Main loop */
    for (; len > 3; len -= 4) {
        unsigned int val;
        hash += stb__get16(q);
        val = (stb__get16(q + 2) << 11);
        hash = (hash << 16) ^ hash ^ val;
        q += 4;
        hash += hash >> 11;
    }

    /* Handle end cases */
    switch (len) {
    case 3: hash += stb__get16(q);
        hash ^= hash << 16;
        hash ^= q[2] << 18;
        hash += hash >> 11;
        break;
    case 2: hash += stb__get16(q);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1: hash += q[0];
        hash ^= hash << 10;
        hash += hash >> 1;
        break;
    case 0: break;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

unsigned int stb_hash_number(unsigned int hash)
{
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

#endif

#ifdef STB_PERFECT_HASH
//////////////////////////////////////////////////////////////////////////////
//
//                     Perfect hashing for ints/pointers
//
//   This is mainly useful for making faster pointer-indexed tables
//   that don't change frequently. E.g. for stb_ischar().
//

typedef struct
{
    stb_uint32  addend;
    stb_uint    multiplicand;
    stb_uint    b_mask;
    stb_uint8   small_bmap[16];
    stb_uint16* large_bmap;

    stb_uint table_mask;
    stb_uint32* table;
} stb_perfect;

STB_EXTERN int stb_perfect_create(stb_perfect*, unsigned int*, int n);
STB_EXTERN void stb_perfect_destroy(stb_perfect*);
STB_EXTERN int stb_perfect_hash(stb_perfect*, unsigned int x);
extern int stb_perfect_hash_max_failures;

#ifdef STB_DEFINE

int stb_perfect_hash_max_failures;

int stb_perfect_hash(stb_perfect* p, unsigned int x)
{
    stb_uint m = x * p->multiplicand;
    stb_uint y = x >> 16;
    stb_uint bv = (m >> 24) + y;
    stb_uint av = (m + y) >> 12;
    if (p->table == NULL) return -1;  // uninitialized table fails
    bv &= p->b_mask;
    av &= p->table_mask;
    if (p->large_bmap)
        av ^= p->large_bmap[bv];
    else
        av ^= p->small_bmap[bv];
    return p->table[av] == x ? av : -1;
}

static void stb__perfect_prehash(stb_perfect* p, stb_uint x, stb_uint16* a, stb_uint16* b)
{
    stb_uint m = x * p->multiplicand;
    stb_uint y = x >> 16;
    stb_uint bv = (m >> 24) + y;
    stb_uint av = (m + y) >> 12;
    bv &= p->b_mask;
    av &= p->table_mask;
    *b = bv;
    *a = av;
}

static unsigned long stb__perfect_rand(void)
{
    static unsigned long stb__rand;
    stb__rand = stb__rand * 2147001325 + 715136305;
    return 0x31415926 ^ ((stb__rand >> 16) + (stb__rand << 16));
}

typedef struct {
    unsigned short count;
    unsigned short b;
    unsigned short map;
    unsigned short* entries;
} stb__slot;

static int stb__slot_compare(const void* p, const void* q)
{
    stb__slot* a = (stb__slot*)p;
    stb__slot* b = (stb__slot*)q;
    return a->count > b->count ? -1 : a->count < b->count;  // sort large to small
}

int stb_perfect_create(stb_perfect* p, unsigned int* v, int n)
{
    unsigned int buffer1[64], buffer2[64], buffer3[64], buffer4[64], buffer5[32];
    unsigned short* as = (unsigned short*)stb_temp(buffer1, sizeof(*v) * n);
    unsigned short* bs = (unsigned short*)stb_temp(buffer2, sizeof(*v) * n);
    unsigned short* entries = (unsigned short*)stb_temp(buffer4, sizeof(*entries) * n);
    int size = 1 << stb_log2_ceil(n), bsize = 8;
    int failure = 0, i, j, k;

    assert(n <= 32768);
    p->large_bmap = NULL;

    for (;;) {
        stb__slot* bcount = (stb__slot*)stb_temp(buffer3, sizeof(*bcount) * bsize);
        unsigned short* bloc = (unsigned short*)stb_temp(buffer5, sizeof(*bloc) * bsize);
        unsigned short* e;
        int bad = 0;

        p->addend = stb__perfect_rand();
        p->multiplicand = stb__perfect_rand() | 1;
        p->table_mask = size - 1;
        p->b_mask = bsize - 1;
        p->table = (stb_uint32*)malloc(size * sizeof(*p->table));

        for (i = 0; i < bsize; ++i) {
            bcount[i].b = i;
            bcount[i].count = 0;
            bcount[i].map = 0;
        }
        for (i = 0; i < n; ++i) {
            stb__perfect_prehash(p, v[i], as + i, bs + i);
            ++bcount[bs[i]].count;
        }
        qsort(bcount, bsize, sizeof(*bcount), stb__slot_compare);
        e = entries; // now setup up their entries index
        for (i = 0; i < bsize; ++i) {
            bcount[i].entries = e;
            e += bcount[i].count;
            bcount[i].count = 0;
            bloc[bcount[i].b] = i;
        }
        // now fill them out
        for (i = 0; i < n; ++i) {
            int b = bs[i];
            int w = bloc[b];
            bcount[w].entries[bcount[w].count++] = i;
        }
        stb_tempfree(buffer5, bloc);
        // verify
        for (i = 0; i < bsize; ++i)
            for (j = 0; j < bcount[i].count; ++j)
                assert(bs[bcount[i].entries[j]] == bcount[i].b);
        memset(p->table, 0, size * sizeof(*p->table));

        // check if any b has duplicate a
        for (i = 0; i < bsize; ++i) {
            if (bcount[i].count > 1) {
                for (j = 0; j < bcount[i].count; ++j) {
                    if (p->table[as[bcount[i].entries[j]]])
                        bad = 1;
                    p->table[as[bcount[i].entries[j]]] = 1;
                }
                for (j = 0; j < bcount[i].count; ++j) {
                    p->table[as[bcount[i].entries[j]]] = 0;
                }
                if (bad) break;
            }
        }

        if (!bad) {
            // go through the bs and populate the table, first fit
            for (i = 0; i < bsize; ++i) {
                if (bcount[i].count) {
                    // go through the candidate table[b] values
                    for (j = 0; j < size; ++j) {
                        // go through the a values and see if they fit
                        for (k = 0; k < bcount[i].count; ++k) {
                            int a = as[bcount[i].entries[k]];
                            if (p->table[(a ^ j) & p->table_mask]) {
                                break; // fails
                            }
                        }
                        // if succeeded, accept
                        if (k == bcount[i].count) {
                            bcount[i].map = j;
                            for (k = 0; k < bcount[i].count; ++k) {
                                int a = as[bcount[i].entries[k]];
                                p->table[(a ^ j) & p->table_mask] = 1;
                            }
                            break;
                        }
                    }
                    if (j == size)
                        break; // no match for i'th entry, so break out in failure
                }
            }
            if (i == bsize) {
                // success... fill out map
                if (bsize <= 16 && size <= 256) {
                    p->large_bmap = NULL;
                    for (i = 0; i < bsize; ++i)
                        p->small_bmap[bcount[i].b] = (stb_uint8)bcount[i].map;
                }
                else {
                    p->large_bmap = (unsigned short*)malloc(sizeof(*p->large_bmap) * bsize);
                    for (i = 0; i < bsize; ++i)
                        p->large_bmap[bcount[i].b] = bcount[i].map;
                }

                // initialize table to v[0], so empty slots will fail
                for (i = 0; i < size; ++i)
                    p->table[i] = v[0];

                for (i = 0; i < n; ++i)
                    if (p->large_bmap)
                        p->table[as[i] ^ p->large_bmap[bs[i]]] = v[i];
                    else
                        p->table[as[i] ^ p->small_bmap[bs[i]]] = v[i];

                // and now validate that none of them collided
                for (i = 0; i < n; ++i)
                    assert(stb_perfect_hash(p, v[i]) >= 0);

                stb_tempfree(buffer3, bcount);
                break;
            }
        }
        free(p->table);
        p->table = NULL;
        stb_tempfree(buffer3, bcount);

        ++failure;
        if (failure >= 4 && bsize < size) bsize *= 2;
        if (failure >= 8 && (failure & 3) == 0 && size < 4 * n) {
            size *= 2;
            bsize *= 2;
        }
        if (failure == 6) {
            // make sure the input data is unique, so we don't infinite loop
            unsigned int* data = (unsigned int*)stb_temp(buffer3, n * sizeof(*data));
            memcpy(data, v, sizeof(*data) * n);
            qsort(data, n, sizeof(*data), stb_intcmp(0));
            for (i = 1; i < n; ++i) {
                if (data[i] == data[i - 1])
                    size = 0; // size is return value, so 0 it
            }
            stb_tempfree(buffer3, data);
            if (!size) break;
        }
    }

    if (failure > stb_perfect_hash_max_failures)
        stb_perfect_hash_max_failures = failure;

    stb_tempfree(buffer1, as);
    stb_tempfree(buffer2, bs);
    stb_tempfree(buffer4, entries);

    return size;
}

void stb_perfect_destroy(stb_perfect* p)
{
    if (p->large_bmap) free(p->large_bmap);
    if (p->table) free(p->table);
    p->large_bmap = NULL;
    p->table = NULL;
    p->b_mask = 0;
    p->table_mask = 0;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                     Perfect hash clients

STB_EXTERN int    stb_ischar(char s, char* set);

#ifdef STB_DEFINE

int stb_ischar(char c, char* set)
{
    static unsigned char bit[8] = { 1,2,4,8,16,32,64,128 };
    static stb_perfect p;
    static unsigned char(*tables)[256];
    static char** sets = NULL;

    int z = stb_perfect_hash(&p, (int)(size_t)set);
    if (z < 0) {
        int i, k, n, j, f;
        // special code that means free all existing data
        if (set == NULL) {
            stb_arr_free(sets);
            free(tables);
            tables = NULL;
            stb_perfect_destroy(&p);
            return 0;
        }
        stb_arr_push(sets, set);
        stb_perfect_destroy(&p);
        n = stb_perfect_create(&p, (unsigned int*)(char**)sets, stb_arr_len(sets));
        assert(n != 0);
        k = (n + 7) >> 3;
        tables = (unsigned char(*)[256]) realloc(tables, sizeof(*tables) * k);
        memset(tables, 0, sizeof(*tables) * k);
        for (i = 0; i < stb_arr_len(sets); ++i) {
            k = stb_perfect_hash(&p, (int)(size_t)sets[i]);
            assert(k >= 0);
            n = k >> 3;
            f = bit[k & 7];
            for (j = 0; !j || sets[i][j]; ++j) {
                tables[n][(unsigned char)sets[i][j]] |= f;
            }
        }
        z = stb_perfect_hash(&p, (int)(size_t)set);
    }
    return tables[z >> 3][(unsigned char)c] & bit[z & 7];
}

#endif
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                     Instantiated data structures
//
// This is an attempt to implement a templated data structure.
//
// Hash table: call stb_define_hash(TYPE,N,KEY,K1,K2,HASH,VALUE)
//     TYPE     -- will define a structure type containing the hash table
//     N        -- the name, will prefix functions named:
//                        N create
//                        N destroy
//                        N get
//                        N set, N add, N update,
//                        N remove
//     KEY      -- the type of the key. 'x == y' must be valid
//       K1,K2  -- keys never used by the app, used as flags in the hashtable
//       HASH   -- a piece of code ending with 'return' that hashes key 'k'
//     VALUE    -- the type of the value. 'x = y' must be valid
//
//  Note that stb_define_hash_base can be used to define more sophisticated
//  hash tables, e.g. those that make copies of the key or use special
//  comparisons (e.g. strcmp).

#define STB_(prefix,name)     stb__##prefix##name
#define STB__(prefix,name)    prefix##name
#define STB__use(x)           x
#define STB__skip(x)

#define stb_declare_hash(PREFIX,TYPE,N,KEY,VALUE) \
   typedef struct stb__st_##TYPE TYPE;\
   PREFIX int STB__(N, init)(TYPE *h, int count);\
   PREFIX int STB__(N, memory_usage)(TYPE *h);\
   PREFIX TYPE * STB__(N, create)(void);\
   PREFIX TYPE * STB__(N, copy)(TYPE *h);\
   PREFIX void STB__(N, destroy)(TYPE *h);\
   PREFIX int STB__(N,get_flag)(TYPE *a, KEY k, VALUE *v);\
   PREFIX VALUE STB__(N,get)(TYPE *a, KEY k);\
   PREFIX int STB__(N, set)(TYPE *a, KEY k, VALUE v);\
   PREFIX int STB__(N, add)(TYPE *a, KEY k, VALUE v);\
   PREFIX int STB__(N, update)(TYPE*a,KEY k,VALUE v);\
   PREFIX int STB__(N, remove)(TYPE *a, KEY k, VALUE *v);

#define STB_nocopy(x)        (x)
#define STB_nodelete(x)      0
#define STB_nofields
#define STB_nonullvalue(x)
#define STB_nullvalue(x)     x
#define STB_safecompare(x)   x
#define STB_nosafe(x)
#define STB_noprefix

#ifdef __GNUC__
#define STB__nogcc(x)
#else
#define STB__nogcc(x)  x
#endif

#define stb_define_hash_base(PREFIX,TYPE,FIELDS,N,NC,LOAD_FACTOR,             \
                             KEY,EMPTY,DEL,COPY,DISPOSE,SAFE,                 \
                             VCOMPARE,CCOMPARE,HASH,                          \
                             VALUE,HASVNULL,VNULL)                            \
                                                                              \
typedef struct                                                                \
{                                                                             \
   KEY   k;                                                                   \
   VALUE v;                                                                   \
} STB_(N,_hashpair);                                                          \
                                                                              \
STB__nogcc( typedef struct stb__st_##TYPE TYPE;  )                            \
struct stb__st_##TYPE {                                                       \
   FIELDS                                                                     \
   STB_(N,_hashpair) *table;                                                  \
   unsigned int mask;                                                         \
   int count, limit;                                                          \
   int deleted;                                                               \
                                                                              \
   int delete_threshhold;                                                     \
   int grow_threshhold;                                                       \
   int shrink_threshhold;                                                     \
   unsigned char alloced, has_empty, has_del;                                 \
   VALUE ev; VALUE dv;                                                        \
};                                                                            \
                                                                              \
static unsigned int STB_(N, hash)(KEY k)                                      \
{                                                                             \
   HASH                                                                       \
}                                                                             \
                                                                              \
PREFIX int STB__(N, init)(TYPE *h, int count)                                        \
{                                                                             \
   int i;                                                                     \
   if (count < 4) count = 4;                                                  \
   h->limit = count;                                                          \
   h->count = 0;                                                              \
   h->mask  = count-1;                                                        \
   h->deleted = 0;                                                            \
   h->grow_threshhold = (int) (count * LOAD_FACTOR);                          \
   h->has_empty = h->has_del = 0;                                             \
   h->alloced = 0;                                                            \
   if (count <= 64)                                                           \
      h->shrink_threshhold = 0;                                               \
   else                                                                       \
      h->shrink_threshhold = (int) (count * (LOAD_FACTOR/2.25));              \
   h->delete_threshhold = (int) (count * (1-LOAD_FACTOR)/2);                  \
   h->table = (STB_(N,_hashpair)*) malloc(sizeof(h->table[0]) * count);       \
   if (h->table == NULL) return 0;                                            \
   /* ideally this gets turned into a memset32 automatically */               \
   for (i=0; i < count; ++i)                                                  \
      h->table[i].k = EMPTY;                                                  \
   return 1;                                                                  \
}                                                                             \
                                                                              \
PREFIX int STB__(N, memory_usage)(TYPE *h)                                           \
{                                                                             \
   return sizeof(*h) + h->limit * sizeof(h->table[0]);                        \
}                                                                             \
                                                                              \
PREFIX TYPE * STB__(N, create)(void)                                                 \
{                                                                             \
   TYPE *h = (TYPE *) malloc(sizeof(*h));                                     \
   if (h) {                                                                   \
      if (STB__(N, init)(h, 16))                                              \
         h->alloced = 1;                                                      \
      else { free(h); h=NULL; }                                               \
   }                                                                          \
   return h;                                                                  \
}                                                                             \
                                                                              \
PREFIX void STB__(N, destroy)(TYPE *a)                                               \
{                                                                             \
   int i;                                                                     \
   for (i=0; i < a->limit; ++i)                                               \
      if (!CCOMPARE(a->table[i].k,EMPTY) && !CCOMPARE(a->table[i].k, DEL))    \
         DISPOSE(a->table[i].k);                                              \
   free(a->table);                                                            \
   if (a->alloced)                                                            \
      free(a);                                                                \
}                                                                             \
                                                                              \
static void STB_(N, rehash)(TYPE *a, int count);                              \
                                                                              \
PREFIX int STB__(N,get_flag)(TYPE *a, KEY k, VALUE *v)                               \
{                                                                             \
   unsigned int h = STB_(N, hash)(k);                                         \
   unsigned int n = h & a->mask, s;                                           \
   if (CCOMPARE(k,EMPTY)){ if (a->has_empty) *v = a->ev; return a->has_empty;}\
   if (CCOMPARE(k,DEL)) { if (a->has_del  ) *v = a->dv; return a->has_del;   }\
   if (CCOMPARE(a->table[n].k,EMPTY)) return 0;                               \
   SAFE(if (!CCOMPARE(a->table[n].k,DEL)))                                    \
   if (VCOMPARE(a->table[n].k,k)) { *v = a->table[n].v; return 1; }            \
   s = stb_rehash(h) | 1;                                                     \
   for(;;) {                                                                  \
      n = (n + s) & a->mask;                                                  \
      if (CCOMPARE(a->table[n].k,EMPTY)) return 0;                            \
      SAFE(if (CCOMPARE(a->table[n].k,DEL)) continue;)                        \
      if (VCOMPARE(a->table[n].k,k))                                           \
         { *v = a->table[n].v; return 1; }                                    \
   }                                                                          \
}                                                                             \
                                                                              \
HASVNULL(                                                                     \
   PREFIX VALUE STB__(N,get)(TYPE *a, KEY k)                                         \
   {                                                                          \
      VALUE v;                                                                \
      if (STB__(N,get_flag)(a,k,&v)) return v;                                \
      else                           return VNULL;                            \
   }                                                                          \
)                                                                             \
                                                                              \
PREFIX int STB__(N,getkey)(TYPE *a, KEY k, KEY *kout)                                \
{                                                                             \
   unsigned int h = STB_(N, hash)(k);                                         \
   unsigned int n = h & a->mask, s;                                           \
   if (CCOMPARE(k,EMPTY)||CCOMPARE(k,DEL)) return 0;                          \
   if (CCOMPARE(a->table[n].k,EMPTY)) return 0;                               \
   SAFE(if (!CCOMPARE(a->table[n].k,DEL)))                                    \
   if (VCOMPARE(a->table[n].k,k)) { *kout = a->table[n].k; return 1; }         \
   s = stb_rehash(h) | 1;                                                     \
   for(;;) {                                                                  \
      n = (n + s) & a->mask;                                                  \
      if (CCOMPARE(a->table[n].k,EMPTY)) return 0;                            \
      SAFE(if (CCOMPARE(a->table[n].k,DEL)) continue;)                        \
      if (VCOMPARE(a->table[n].k,k))                                          \
         { *kout = a->table[n].k; return 1; }                                 \
   }                                                                          \
}                                                                             \
                                                                              \
static int STB_(N,addset)(TYPE *a, KEY k, VALUE v,                            \
                             int allow_new, int allow_old, int copy)          \
{                                                                             \
   unsigned int h = STB_(N, hash)(k);                                         \
   unsigned int n = h & a->mask;                                              \
   int b = -1;                                                                \
   if (CCOMPARE(k,EMPTY)) {                                                   \
      if (a->has_empty ? allow_old : allow_new) {                             \
          n=a->has_empty; a->ev = v; a->has_empty = 1; return !n;             \
      } else return 0;                                                        \
   }                                                                          \
   if (CCOMPARE(k,DEL)) {                                                     \
      if (a->has_del ? allow_old : allow_new) {                               \
          n=a->has_del; a->dv = v; a->has_del = 1; return !n;                 \
      } else return 0;                                                        \
   }                                                                          \
   if (!CCOMPARE(a->table[n].k, EMPTY)) {                                     \
      unsigned int s;                                                         \
      if (CCOMPARE(a->table[n].k, DEL))                                       \
         b = n;                                                               \
      else if (VCOMPARE(a->table[n].k,k)) {                                   \
         if (allow_old)                                                       \
            a->table[n].v = v;                                                \
         return !allow_new;                                                   \
      }                                                                       \
      s = stb_rehash(h) | 1;                                                  \
      for(;;) {                                                               \
         n = (n + s) & a->mask;                                               \
         if (CCOMPARE(a->table[n].k, EMPTY)) break;                           \
         if (CCOMPARE(a->table[n].k, DEL)) {                                  \
            if (b < 0) b = n;                                                 \
         } else if (VCOMPARE(a->table[n].k,k)) {                              \
            if (allow_old)                                                    \
               a->table[n].v = v;                                             \
            return !allow_new;                                                \
         }                                                                    \
      }                                                                       \
   }                                                                          \
   if (!allow_new) return 0;                                                  \
   if (b < 0) b = n; else --a->deleted;                                       \
   a->table[b].k = copy ? COPY(k) : k;                                        \
   a->table[b].v = v;                                                         \
   ++a->count;                                                                \
   if (a->count > a->grow_threshhold)                                         \
      STB_(N,rehash)(a, a->limit*2);                                          \
   return 1;                                                                  \
}                                                                             \
                                                                              \
PREFIX int STB__(N, set)(TYPE *a, KEY k, VALUE v){return STB_(N,addset)(a,k,v,1,1,1);}\
PREFIX int STB__(N, add)(TYPE *a, KEY k, VALUE v){return STB_(N,addset)(a,k,v,1,0,1);}\
PREFIX int STB__(N, update)(TYPE*a,KEY k,VALUE v){return STB_(N,addset)(a,k,v,0,1,1);}\
                                                                              \
PREFIX int STB__(N, remove)(TYPE *a, KEY k, VALUE *v)                                \
{                                                                             \
   unsigned int h = STB_(N, hash)(k);                                         \
   unsigned int n = h & a->mask, s;                                           \
   if (CCOMPARE(k,EMPTY)) { if (a->has_empty) { if(v)*v = a->ev; a->has_empty=0; return 1; } return 0; } \
   if (CCOMPARE(k,DEL))   { if (a->has_del  ) { if(v)*v = a->dv; a->has_del  =0; return 1; } return 0; } \
   if (CCOMPARE(a->table[n].k,EMPTY)) return 0;                               \
   if (SAFE(CCOMPARE(a->table[n].k,DEL) || ) !VCOMPARE(a->table[n].k,k)) {     \
      s = stb_rehash(h) | 1;                                                  \
      for(;;) {                                                               \
         n = (n + s) & a->mask;                                               \
         if (CCOMPARE(a->table[n].k,EMPTY)) return 0;                         \
         SAFE(if (CCOMPARE(a->table[n].k, DEL)) continue;)                    \
         if (VCOMPARE(a->table[n].k,k)) break;                                 \
      }                                                                       \
   }                                                                          \
   DISPOSE(a->table[n].k);                                                    \
   a->table[n].k = DEL;                                                       \
   --a->count;                                                                \
   ++a->deleted;                                                              \
   if (v != NULL)                                                             \
      *v = a->table[n].v;                                                     \
   if (a->count < a->shrink_threshhold)                                       \
      STB_(N, rehash)(a, a->limit >> 1);                                      \
   else if (a->deleted > a->delete_threshhold)                                \
      STB_(N, rehash)(a, a->limit);                                           \
   return 1;                                                                  \
}                                                                             \
                                                                              \
PREFIX TYPE * STB__(NC, copy)(TYPE *a)                                        \
{                                                                             \
   int i;                                                                     \
   TYPE *h = (TYPE *) malloc(sizeof(*h));                                     \
   if (!h) return NULL;                                                       \
   if (!STB__(N, init)(h, a->limit)) { free(h); return NULL; }                \
   h->count = a->count;                                                       \
   h->deleted = a->deleted;                                                   \
   h->alloced = 1;                                                            \
   h->ev = a->ev; h->dv = a->dv;                                              \
   h->has_empty = a->has_empty; h->has_del = a->has_del;                      \
   memcpy(h->table, a->table, h->limit * sizeof(h->table[0]));                \
   for (i=0; i < a->limit; ++i)                                               \
      if (!CCOMPARE(h->table[i].k,EMPTY) && !CCOMPARE(h->table[i].k,DEL))     \
         h->table[i].k = COPY(h->table[i].k);                                 \
   return h;                                                                  \
}                                                                             \
                                                                              \
static void STB_(N, rehash)(TYPE *a, int count)                               \
{                                                                             \
   int i;                                                                     \
   TYPE b;                                                                    \
   STB__(N, init)(&b, count);                                                 \
   for (i=0; i < a->limit; ++i)                                               \
      if (!CCOMPARE(a->table[i].k,EMPTY) && !CCOMPARE(a->table[i].k,DEL))     \
         STB_(N,addset)(&b, a->table[i].k, a->table[i].v,1,1,0);              \
   free(a->table);                                                            \
   a->table = b.table;                                                        \
   a->mask = b.mask;                                                          \
   a->count = b.count;                                                        \
   a->limit = b.limit;                                                        \
   a->deleted = b.deleted;                                                    \
   a->delete_threshhold = b.delete_threshhold;                                \
   a->grow_threshhold = b.grow_threshhold;                                    \
   a->shrink_threshhold = b.shrink_threshhold;                                \
}

#define STB_equal(a,b)  ((a) == (b))

#define stb_define_hash(TYPE,N,KEY,EMPTY,DEL,HASH,VALUE)                      \
   stb_define_hash_base(STB_noprefix, TYPE,STB_nofields,N,NC,0.85f,           \
              KEY,EMPTY,DEL,STB_nocopy,STB_nodelete,STB_nosafe,               \
              STB_equal,STB_equal,HASH,                                       \
              VALUE,STB_nonullvalue,0)

#define stb_define_hash_vnull(TYPE,N,KEY,EMPTY,DEL,HASH,VALUE,VNULL)          \
   stb_define_hash_base(STB_noprefix, TYPE,STB_nofields,N,NC,0.85f,           \
              KEY,EMPTY,DEL,STB_nocopy,STB_nodelete,STB_nosafe,               \
              STB_equal,STB_equal,HASH,                                       \
              VALUE,STB_nullvalue,VNULL)

//////////////////////////////////////////////////////////////////////////////
//
//                        stb_ptrmap
//
// An stb_ptrmap data structure is an O(1) hash table between pointers. One
// application is to let you store "extra" data associated with pointers,
// which is why it was originally called stb_extra.

stb_declare_hash(STB_EXTERN, stb_ptrmap, stb_ptrmap_, void*, void*)
stb_declare_hash(STB_EXTERN, stb_idict, stb_idict_, stb_int32, stb_int32)
stb_declare_hash(STB_EXTERN, stb_uidict, stbi_uidict_, stb_uint32, stb_uint32)

STB_EXTERN void        stb_ptrmap_delete(stb_ptrmap* e, void (*free_func)(void*));
STB_EXTERN stb_ptrmap* stb_ptrmap_new(void);

STB_EXTERN stb_idict* stb_idict_new_size(int size);
STB_EXTERN void        stb_idict_remove_all(stb_idict* e);
STB_EXTERN void        stb_uidict_reset(stb_uidict* e);

#ifdef STB_DEFINE

#define STB_EMPTY ((void *) 2)
#define STB_EDEL  ((void *) 6)

stb_define_hash_base(STB_noprefix, stb_ptrmap, STB_nofields, stb_ptrmap_, stb_ptrmap_, 0.85f,
    void*, STB_EMPTY, STB_EDEL, STB_nocopy, STB_nodelete, STB_nosafe,
    STB_equal, STB_equal, return stb_hashptr(k); ,
    void*, STB_nullvalue, NULL)

    stb_ptrmap* stb_ptrmap_new(void)
{
    return stb_ptrmap_create();
}

void stb_ptrmap_delete(stb_ptrmap * e, void (*free_func)(void*))
{
    int i;
    if (free_func)
        for (i = 0; i < e->limit; ++i)
            if (e->table[i].k != STB_EMPTY && e->table[i].k != STB_EDEL) {
                if (free_func == free)
                    free(e->table[i].v); // allow STB_MALLOC_WRAPPER to operate
                else
                    free_func(e->table[i].v);
            }
    stb_ptrmap_destroy(e);
}

// extra fields needed for stua_dict
#define STB_IEMPTY  ((int) 1)
#define STB_IDEL    ((int) 3)
stb_define_hash_base(STB_noprefix, stb_idict, short type; short gc; STB_nofields, stb_idict_, stb_idict_, 0.95f,
    stb_int32, STB_IEMPTY, STB_IDEL, STB_nocopy, STB_nodelete, STB_nosafe,
    STB_equal, STB_equal,
    return stb_rehash_improved(k); , stb_int32, STB_nonullvalue, 0)

    stb_idict* stb_idict_new_size(int size)
{
    stb_idict* e = (stb_idict*)malloc(sizeof(*e));
    if (e) {
        if (!stb_is_pow2(size))
            size = 1 << stb_log2_ceil(size);
        stb_idict_init(e, size);
        e->alloced = 1;
    }
    return e;
}

void stb_idict_remove_all(stb_idict * e)
{
    int n;
    for (n = 0; n < e->limit; ++n)
        e->table[n].k = STB_IEMPTY;
    e->has_empty = e->has_del = 0;
    e->count = 0;
    e->deleted = 0;
}

stb_define_hash_base(STB_noprefix, stb_uidict, STB_nofields, stb_uidict_, stb_uidict_, 0.85f,
    stb_int32, 0xffffffff, 0xfffffffe, STB_nocopy, STB_nodelete, STB_nosafe,
    STB_equal, STB_equal,
    return stb_rehash_improved(k); , stb_uint32, STB_nonullvalue, 0)

    void stb_uidict_reset(stb_uidict * e)
{
    int n;
    for (n = 0; n < e->limit; ++n)
        e->table[n].k = 0xffffffff;
    e->has_empty = e->has_del = 0;
    e->count = 0;
    e->deleted = 0;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                        stb_sparse_ptr_matrix
//
// An stb_ptrmap data structure is an O(1) hash table storing an arbitrary
// block of data for a given pair of pointers.
//
// If create=0, returns

typedef struct stb__st_stb_spmatrix stb_spmatrix;

STB_EXTERN stb_spmatrix* stb_sparse_ptr_matrix_new(int val_size);
STB_EXTERN void           stb_sparse_ptr_matrix_free(stb_spmatrix * z);
STB_EXTERN void* stb_sparse_ptr_matrix_get(stb_spmatrix * z, void* a, void* b, int create);

#ifdef STB_DEFINE
typedef struct
{
    void* a;
    void* b;
} stb__ptrpair;

static stb__ptrpair stb__ptrpair_empty = { (void*)1, (void*)1 };
static stb__ptrpair stb__ptrpair_del = { (void*)2, (void*)2 };

#define STB__equal_ptrpair(x,y) ((x).a == (y).a && (x).b == (y).b)

stb_define_hash_base(STB_noprefix, stb_spmatrix, int val_size; void* arena; , stb__spmatrix_, stb__spmatrix_, 0.85,
    stb__ptrpair, stb__ptrpair_empty, stb__ptrpair_del,
    STB_nocopy, STB_nodelete, STB_nosafe,
    STB__equal_ptrpair, STB__equal_ptrpair, return stb_rehash(stb_hashptr(k.a)) + stb_hashptr(k.b); ,
    void*, STB_nullvalue, 0)

    stb_spmatrix* stb_sparse_ptr_matrix_new(int val_size)
{
    stb_spmatrix* m = stb__spmatrix_create();
    if (m) m->val_size = val_size;
    if (m) m->arena = stb_malloc_global(1);
    return m;
}

void stb_sparse_ptr_matrix_free(stb_spmatrix * z)
{
    if (z->arena) stb_free(z->arena);
    stb__spmatrix_destroy(z);
}

void* stb_sparse_ptr_matrix_get(stb_spmatrix * z, void* a, void* b, int create)
{
    stb__ptrpair t = { a,b };
    void* data = stb__spmatrix_get(z, t);
    if (!data && create) {
        data = stb_malloc_raw(z->arena, z->val_size);
        if (!data) return NULL;
        memset(data, 0, z->val_size);
        stb__spmatrix_add(z, t, data);
    }
    return data;
}
#endif



//////////////////////////////////////////////////////////////////////////////
//
//                  SDICT: Hash Table for Strings (symbol table)
//
//           if "use_arena=1", then strings will be copied
//           into blocks and never freed until the sdict is freed;
//           otherwise they're malloc()ed and free()d on the fly.
//           (specify use_arena=1 if you never stb_sdict_remove)

stb_declare_hash(STB_EXTERN, stb_sdict, stb_sdict_, char*, void*)

STB_EXTERN stb_sdict* stb_sdict_new(int use_arena);
STB_EXTERN stb_sdict* stb_sdict_copy(stb_sdict*);
STB_EXTERN void        stb_sdict_delete(stb_sdict*);
STB_EXTERN void* stb_sdict_change(stb_sdict*, char* str, void* p);
STB_EXTERN int         stb_sdict_count(stb_sdict * d);

STB_EXTERN int         stb_sdict_internal_limit(stb_sdict * d);
STB_EXTERN char* stb_sdict_internal_key(stb_sdict * d, int n);
STB_EXTERN void* stb_sdict_internal_value(stb_sdict * d, int n);

#define stb_sdict_for(d,i,q,z)                                          \
   for(i=0; i < stb_sdict_internal_limit(d) ? (q=stb_sdict_internal_key(d,i),z=stb_sdict_internal_value(d,i),1) : 0; ++i)    \
      if (q==NULL||q==(void *) 1);else   // reversed makes macro friendly

#ifdef STB_DEFINE

// if in same translation unit, for speed, don't call accessors
#undef stb_sdict_for
#define stb_sdict_for(d,i,q,z)                                          \
   for(i=0; i < (d)->limit ? (q=(d)->table[i].k,z=(d)->table[i].v,1) : 0; ++i)    \
      if (q==NULL||q==(void *) 1);else   // reversed makes macro friendly

#define STB_DEL ((void *) 1)
#define STB_SDEL  ((char *) 1)

#define stb_sdict__copy(x)                                             \
   stb_p_strcpy_s(a->arena ? stb_malloc_string(a->arena, strlen(x)+1)    \
                         : (char *) malloc(strlen(x)+1), strlen(x)+1, x)

#define stb_sdict__dispose(x)  if (!a->arena) free(x)

stb_define_hash_base(STB_noprefix, stb_sdict, void* arena; , stb_sdict_, stb_sdictinternal_, 0.85f,
    char*, NULL, STB_SDEL, stb_sdict__copy, stb_sdict__dispose,
    STB_safecompare, !strcmp, STB_equal, return stb_hash(k); ,
    void*, STB_nullvalue, NULL)

    int stb_sdict_count(stb_sdict * a)
{
    return a->count;
}

int stb_sdict_internal_limit(stb_sdict * a)
{
    return a->limit;
}
char* stb_sdict_internal_key(stb_sdict * a, int n)
{
    return a->table[n].k;
}
void* stb_sdict_internal_value(stb_sdict * a, int n)
{
    return a->table[n].v;
}

stb_sdict* stb_sdict_new(int use_arena)
{
    stb_sdict* d = stb_sdict_create();
    if (d == NULL) return NULL;
    d->arena = use_arena ? stb_malloc_global(1) : NULL;
    return d;
}

stb_sdict* stb_sdict_copy(stb_sdict * old)
{
    stb_sdict* n;
    void* old_arena = old->arena;
    void* new_arena = old_arena ? stb_malloc_global(1) : NULL;
    old->arena = new_arena;
    n = stb_sdictinternal_copy(old);
    old->arena = old_arena;
    if (n)
        n->arena = new_arena;
    else if (new_arena)
        stb_free(new_arena);
    return n;
}


void stb_sdict_delete(stb_sdict * d)
{
    if (d->arena)
        stb_free(d->arena);
    stb_sdict_destroy(d);
}

void* stb_sdict_change(stb_sdict * d, char* str, void* p)
{
    void* q = stb_sdict_get(d, str);
    stb_sdict_set(d, str, p);
    return q;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                     Instantiated data structures
//
// This is an attempt to implement a templated data structure.
// What you do is define a struct foo, and then include several
// pointer fields to struct foo in your struct. Then you call
// the instantiator, which creates the functions that implement
// the data structure. This requires massive undebuggable #defines,
// so we limit the cases where we do this.
//
// AA tree is an encoding of a 2-3 tree whereas RB trees encode a 2-3-4 tree;
// much simpler code due to fewer cases.

#define stb__bst_parent(x)    x
#define stb__bst_noparent(x)

#define stb_bst_fields(N)                                   \
    *STB_(N,left), *STB_(N,right);                          \
    unsigned char STB_(N,level)

#define stb_bst_fields_parent(N)                            \
    *STB_(N,left), *STB_(N,right),  *STB_(N,parent);        \
    unsigned char STB_(N,level)

#define STB__level(N,x)         ((x) ? (x)->STB_(N,level) : 0)

#define stb_bst_base(TYPE, N, TREE, M, compare, PAR)                         \
                                                                             \
static int STB_(N,_compare)(TYPE *p, TYPE *q)                                \
{                                                                            \
   compare                                                                   \
}                                                                            \
                                                                             \
static void STB_(N,setleft)(TYPE *q, TYPE *v)                                \
{                                                                            \
   q->STB_(N,left) = v;                                                      \
   PAR(if (v) v->STB_(N,parent) = q;)                                        \
}                                                                            \
                                                                             \
static void STB_(N,setright)(TYPE *q, TYPE *v)                               \
{                                                                            \
   q->STB_(N,right) = v;                                                     \
   PAR(if (v) v->STB_(N,parent) = q;)                                        \
}                                                                            \
                                                                             \
static TYPE *STB_(N,skew)(TYPE *q)                                           \
{                                                                            \
   if (q == NULL) return q;                                                  \
   if (q->STB_(N,left)                                                       \
        && q->STB_(N,left)->STB_(N,level) == q->STB_(N,level)) {             \
      TYPE *p       = q->STB_(N,left);                                       \
      STB_(N,setleft)(q, p->STB_(N,right));                                  \
      STB_(N,setright)(p, q);                                                \
      return p;                                                              \
   }                                                                         \
   return q;                                                                 \
}                                                                            \
                                                                             \
static TYPE *STB_(N,split)(TYPE *p)                                          \
{                                                                            \
   TYPE *q = p->STB_(N,right);                                               \
   if (q && q->STB_(N,right)                                                 \
         && q->STB_(N,right)->STB_(N,level) == p->STB_(N,level)) {           \
      STB_(N,setright)(p, q->STB_(N,left));                                  \
      STB_(N,setleft)(q,p);                                                  \
      ++q->STB_(N,level);                                                    \
      return q;                                                              \
   }                                                                         \
   return p;                                                                 \
}                                                                            \
                                                                             \
TYPE *STB__(N,insert)(TYPE *tree, TYPE *item)                                \
{                                                                            \
   int c;                                                                    \
   if (tree == NULL) {                                                       \
      item->STB_(N,left) = NULL;                                             \
      item->STB_(N,right) = NULL;                                            \
      item->STB_(N,level) = 1;                                               \
      PAR(item->STB_(N,parent) = NULL;)                                      \
      return item;                                                           \
   }                                                                         \
   c = STB_(N,_compare)(item,tree);                                          \
   if (c == 0) {                                                             \
      if (item != tree) {                                                    \
         STB_(N,setleft)(item, tree->STB_(N,left));                          \
         STB_(N,setright)(item, tree->STB_(N,right));                        \
         item->STB_(N,level) = tree->STB_(N,level);                          \
         PAR(item->STB_(N,parent) = NULL;)                                   \
      }                                                                      \
      return item;                                                           \
   }                                                                         \
   if (c < 0)                                                                \
      STB_(N,setleft )(tree, STB__(N,insert)(tree->STB_(N,left), item));     \
   else                                                                      \
      STB_(N,setright)(tree, STB__(N,insert)(tree->STB_(N,right), item));    \
   tree = STB_(N,skew)(tree);                                                \
   tree = STB_(N,split)(tree);                                               \
   PAR(tree->STB_(N,parent) = NULL;)                                         \
   return tree;                                                              \
}                                                                            \
                                                                             \
TYPE *STB__(N,remove)(TYPE *tree, TYPE *item)                                \
{                                                                            \
   static TYPE *delnode, *leaf, *restore;                                    \
   if (tree == NULL) return NULL;                                            \
   leaf = tree;                                                              \
   if (STB_(N,_compare)(item, tree) < 0) {                                   \
      STB_(N,setleft)(tree, STB__(N,remove)(tree->STB_(N,left), item));      \
   } else {                                                                  \
      TYPE *r;                                                               \
      delnode = tree;                                                        \
      r = STB__(N,remove)(tree->STB_(N,right), item);                        \
      /* maybe move 'leaf' up to this location */                            \
      if (restore == tree) { tree = leaf; leaf = restore = NULL;  }          \
      STB_(N,setright)(tree,r);                                              \
      assert(tree->STB_(N,right) != tree);                                   \
   }                                                                         \
   if (tree == leaf) {                                                       \
      if (delnode == item) {                                                 \
         tree = tree->STB_(N,right);                                         \
         assert(leaf->STB_(N,left) == NULL);                                 \
         /* move leaf (the right sibling) up to delnode */                   \
         STB_(N,setleft )(leaf, item->STB_(N,left ));                        \
         STB_(N,setright)(leaf, item->STB_(N,right));                        \
         leaf->STB_(N,level) = item->STB_(N,level);                          \
         if (leaf != item)                                                   \
            restore = delnode;                                               \
      }                                                                      \
      delnode = NULL;                                                        \
   } else {                                                                  \
      if (STB__level(N,tree->STB_(N,left) ) < tree->STB_(N,level)-1 ||       \
          STB__level(N,tree->STB_(N,right)) < tree->STB_(N,level)-1) {       \
         --tree->STB_(N,level);                                              \
         if (STB__level(N,tree->STB_(N,right)) > tree->STB_(N,level))        \
            tree->STB_(N,right)->STB_(N,level) = tree->STB_(N,level);        \
         tree = STB_(N,skew)(tree);                                          \
         STB_(N,setright)(tree, STB_(N,skew)(tree->STB_(N,right)));          \
         if (tree->STB_(N,right))                                            \
            STB_(N,setright)(tree->STB_(N,right),                            \
                  STB_(N,skew)(tree->STB_(N,right)->STB_(N,right)));         \
         tree = STB_(N,split)(tree);                                         \
         if (tree->STB_(N,right))                                            \
            STB_(N,setright)(tree,  STB_(N,split)(tree->STB_(N,right)));     \
      }                                                                      \
   }                                                                         \
   PAR(if (tree) tree->STB_(N,parent) = NULL;)                               \
   return tree;                                                              \
}                                                                            \
                                                                             \
TYPE *STB__(N,last)(TYPE *tree)                                              \
{                                                                            \
   if (tree)                                                                 \
      while (tree->STB_(N,right)) tree = tree->STB_(N,right);                \
   return tree;                                                              \
}                                                                            \
                                                                             \
TYPE *STB__(N,first)(TYPE *tree)                                             \
{                                                                            \
   if (tree)                                                                 \
      while (tree->STB_(N,left)) tree = tree->STB_(N,left);                  \
   return tree;                                                              \
}                                                                            \
                                                                             \
TYPE *STB__(N,next)(TYPE *tree, TYPE *item)                                  \
{                                                                            \
   TYPE *next = NULL;                                                        \
   if (item->STB_(N,right))                                                  \
      return STB__(N,first)(item->STB_(N,right));                            \
   PAR(                                                                      \
      while(item->STB_(N,parent)) {                                          \
         TYPE *up = item->STB_(N,parent);                                    \
         if (up->STB_(N,left) == item) return up;                            \
         item = up;                                                          \
      }                                                                      \
      return NULL;                                                           \
   )                                                                         \
   while (tree != item) {                                                    \
      if (STB_(N,_compare)(item, tree) < 0) {                                \
         next = tree;                                                        \
         tree = tree->STB_(N,left);                                          \
      } else {                                                               \
         tree = tree->STB_(N,right);                                         \
      }                                                                      \
   }                                                                         \
   return next;                                                              \
}                                                                            \
                                                                             \
TYPE *STB__(N,prev)(TYPE *tree, TYPE *item)                                  \
{                                                                            \
   TYPE *next = NULL;                                                        \
   if (item->STB_(N,left))                                                   \
      return STB__(N,last)(item->STB_(N,left));                              \
   PAR(                                                                      \
      while(item->STB_(N,parent)) {                                          \
         TYPE *up = item->STB_(N,parent);                                    \
         if (up->STB_(N,right) == item) return up;                           \
         item = up;                                                          \
      }                                                                      \
      return NULL;                                                           \
   )                                                                         \
   while (tree != item) {                                                    \
      if (STB_(N,_compare)(item, tree) < 0) {                                \
         tree = tree->STB_(N,left);                                          \
      } else {                                                               \
         next = tree;                                                        \
         tree = tree->STB_(N,right);                                         \
      }                                                                      \
   }                                                                         \
   return next;                                                              \
}                                                                            \
                                                                             \
STB__DEBUG(                                                                  \
   void STB__(N,_validate)(TYPE *tree, int root)                             \
   {                                                                         \
      if (tree == NULL) return;                                              \
      PAR(if(root) assert(tree->STB_(N,parent) == NULL);)                    \
      assert(STB__level(N,tree->STB_(N,left) ) == tree->STB_(N,level)-1);    \
      assert(STB__level(N,tree->STB_(N,right)) <= tree->STB_(N,level));      \
      assert(STB__level(N,tree->STB_(N,right)) >= tree->STB_(N,level)-1);    \
      if (tree->STB_(N,right)) {                                             \
        assert(STB__level(N,tree->STB_(N,right)->STB_(N,right))              \
                                               !=    tree->STB_(N,level));   \
        PAR(assert(tree->STB_(N,right)->STB_(N,parent) == tree);)            \
      }                                                                      \
      PAR(if(tree->STB_(N,left)) assert(tree->STB_(N,left)->STB_(N,parent) == tree);) \
      STB__(N,_validate)(tree->STB_(N,left) ,0);                             \
      STB__(N,_validate)(tree->STB_(N,right),0);                             \
   }                                                                         \
)                                                                            \
                                                                             \
typedef struct                                                               \
{                                                                            \
   TYPE *root;                                                               \
} TREE;                                                                      \
                                                                             \
void  STB__(M,Insert)(TREE *tree, TYPE *item)                                \
{ tree->root = STB__(N,insert)(tree->root, item); }                          \
void  STB__(M,Remove)(TREE *tree, TYPE *item)                                \
{ tree->root = STB__(N,remove)(tree->root, item); }                          \
TYPE *STB__(M,Next)(TREE *tree, TYPE *item)                                  \
{ return STB__(N,next)(tree->root, item); }                                  \
TYPE *STB__(M,Prev)(TREE *tree, TYPE *item)                                  \
{ return STB__(N,prev)(tree->root, item); }                                  \
TYPE *STB__(M,First)(TREE *tree) { return STB__(N,first)(tree->root); }      \
TYPE *STB__(M,Last) (TREE *tree) { return STB__(N,last) (tree->root); }      \
void STB__(M,Init)(TREE *tree) { tree->root = NULL; }


#define stb_bst_find(N,tree,fcompare)                                        \
{                                                                            \
   int c;                                                                    \
   while (tree != NULL) {                                                    \
      fcompare                                                               \
      if (c == 0) return tree;                                               \
      if (c < 0)  tree = tree->STB_(N,left);                                 \
      else        tree = tree->STB_(N,right);                                \
   }                                                                         \
   return NULL;                                                              \
}

#define stb_bst_raw(TYPE,N,TREE,M,vfield,VTYPE,compare,PAR)                  \
   stb_bst_base(TYPE,N,TREE,M,                                               \
         VTYPE a = p->vfield; VTYPE b = q->vfield; return (compare);, PAR )  \
                                                                             \
TYPE *STB__(N,find)(TYPE *tree, VTYPE a)                                     \
   stb_bst_find(N,tree,VTYPE b = tree->vfield; c = (compare);)               \
TYPE *STB__(M,Find)(TREE *tree, VTYPE a)                                     \
{ return STB__(N,find)(tree->root, a); }

#define stb_bst(TYPE,N,TREE,M,vfield,VTYPE,compare) \
   stb_bst_raw(TYPE,N,TREE,M,vfield,VTYPE,compare,stb__bst_noparent)
#define stb_bst_parent(TYPE,N,TREE,M,vfield,VTYPE,compare) \
   stb_bst_raw(TYPE,N,TREE,M,vfield,VTYPE,compare,stb__bst_parent)



//////////////////////////////////////////////////////////////////////////////
//
//                             Pointer Nulling
//
//  This lets you automatically NULL dangling pointers to "registered"
//  objects. Note that you have to make sure you call the appropriate
//  functions when you free or realloc blocks of memory that contain
//  pointers or pointer targets. stb.h can automatically do this for
//  stb_arr, or for all frees/reallocs if it's wrapping them.
//

#ifdef STB_NPTR

STB_EXTERN void stb_nptr_set(void* address_of_pointer, void* value_to_write);
STB_EXTERN void stb_nptr_didset(void* address_of_pointer);

STB_EXTERN void stb_nptr_didfree(void* address_being_freed, int len);
STB_EXTERN void stb_nptr_free(void* address_being_freed, int len);

STB_EXTERN void stb_nptr_didrealloc(void* new_address, void* old_address, int len);
STB_EXTERN void stb_nptr_recache(void); // recache all known pointers
                                       // do this after pointer sets outside your control, slow

#ifdef STB_DEFINE
// for fast updating on free/realloc, we need to be able to find
// all the objects (pointers and targets) within a given block;
// this precludes hashing

// we use a three-level hierarchy of memory to minimize storage:
//   level 1: 65536 pointers to stb__memory_node (always uses 256 KB)
//   level 2: each stb__memory_node represents a 64K block of memory
//            with 256 stb__memory_leafs (worst case 64MB)
//   level 3: each stb__memory_leaf represents 256 bytes of memory
//            using a list of target locations and a list of pointers
//            (which are hopefully fairly short normally!)

// this approach won't work in 64-bit, which has a much larger address
// space. need to redesign

#define STB__NPTR_ROOT_LOG2   16
#define STB__NPTR_ROOT_NUM    (1 << STB__NPTR_ROOT_LOG2)
#define STB__NPTR_ROOT_SHIFT  (32 - STB__NPTR_ROOT_LOG2)

#define STB__NPTR_NODE_LOG2   5
#define STB__NPTR_NODE_NUM    (1 << STB__NPTR_NODE_LOG2)
#define STB__NPTR_NODE_MASK   (STB__NPTR_NODE_NUM-1)
#define STB__NPTR_NODE_SHIFT  (STB__NPTR_ROOT_SHIFT - STB__NPTR_NODE_LOG2)
#define STB__NPTR_NODE_OFFSET(x)   (((x) >> STB__NPTR_NODE_SHIFT) & STB__NPTR_NODE_MASK)

typedef struct stb__st_nptr
{
    void* ptr;   // address of actual pointer
    struct stb__st_nptr* next;   // next pointer with same target
    struct stb__st_nptr** prev;  // prev pointer with same target, address of 'next' field (or first)
    struct stb__st_nptr* next_in_block;
} stb__nptr;

typedef struct stb__st_nptr_target
{
    void* ptr;   // address of target
    stb__nptr* first; // address of first nptr pointing to this
    struct stb__st_nptr_target* next_in_block;
} stb__nptr_target;

typedef struct
{
    stb__nptr* pointers;
    stb__nptr_target* targets;
} stb__memory_leaf;

typedef struct
{
    stb__memory_leaf* children[STB__NPTR_NODE_NUM];
} stb__memory_node;

stb__memory_node* stb__memtab_root[STB__NPTR_ROOT_NUM];

static stb__memory_leaf* stb__nptr_find_leaf(void* mem)
{
    stb_uint32 address = (stb_uint32)mem;
    stb__memory_node* z = stb__memtab_root[address >> STB__NPTR_ROOT_SHIFT];
    if (z)
        return z->children[STB__NPTR_NODE_OFFSET(address)];
    else
        return NULL;
}

static void* stb__nptr_alloc(int size)
{
    return stb__realloc_raw(0, size);
}

static void stb__nptr_free(void* p)
{
    stb__realloc_raw(p, 0);
}

static stb__memory_leaf* stb__nptr_make_leaf(void* mem)
{
    stb_uint32 address = (stb_uint32)mem;
    stb__memory_node* z = stb__memtab_root[address >> STB__NPTR_ROOT_SHIFT];
    stb__memory_leaf* f;
    if (!z) {
        int i;
        z = (stb__memory_node*)stb__nptr_alloc(sizeof(*stb__memtab_root[0]));
        stb__memtab_root[address >> STB__NPTR_ROOT_SHIFT] = z;
        for (i = 0; i < 256; ++i)
            z->children[i] = 0;
    }
    f = (stb__memory_leaf*)stb__nptr_alloc(sizeof(*f));
    z->children[STB__NPTR_NODE_OFFSET(address)] = f;
    f->pointers = NULL;
    f->targets = NULL;
    return f;
}

static stb__nptr_target* stb__nptr_find_target(void* target, int force)
{
    stb__memory_leaf* p = stb__nptr_find_leaf(target);
    if (p) {
        stb__nptr_target* t = p->targets;
        while (t) {
            if (t->ptr == target)
                return t;
            t = t->next_in_block;
        }
    }
    if (force) {
        stb__nptr_target* t = (stb__nptr_target*)stb__nptr_alloc(sizeof(*t));
        if (!p) p = stb__nptr_make_leaf(target);
        t->ptr = target;
        t->first = NULL;
        t->next_in_block = p->targets;
        p->targets = t;
        return t;
    }
    else
        return NULL;
}

static stb__nptr* stb__nptr_find_pointer(void* ptr, int force)
{
    stb__memory_leaf* p = stb__nptr_find_leaf(ptr);
    if (p) {
        stb__nptr* t = p->pointers;
        while (t) {
            if (t->ptr == ptr)
                return t;
            t = t->next_in_block;
        }
    }
    if (force) {
        stb__nptr* t = (stb__nptr*)stb__nptr_alloc(sizeof(*t));
        if (!p) p = stb__nptr_make_leaf(ptr);
        t->ptr = ptr;
        t->next = NULL;
        t->prev = NULL;
        t->next_in_block = p->pointers;
        p->pointers = t;
        return t;
    }
    else
        return NULL;
}

void stb_nptr_set(void* address_of_pointer, void* value_to_write)
{
    if (*(void**)address_of_pointer != value_to_write) {
        *(void**)address_of_pointer = value_to_write;
        stb_nptr_didset(address_of_pointer);
    }
}

void stb_nptr_didset(void* address_of_pointer)
{
    // first unlink from old chain
    void* new_address;
    stb__nptr* p = stb__nptr_find_pointer(address_of_pointer, 1); // force building if doesn't exist
    if (p->prev) { // if p->prev is NULL, we just built it, or it was NULL
        *(p->prev) = p->next;
        if (p->next) p->next->prev = p->prev;
    }
    // now add to new chain
    new_address = *(void**)address_of_pointer;
    if (new_address != NULL) {
        stb__nptr_target* t = stb__nptr_find_target(new_address, 1);
        p->next = t->first;
        if (p->next) p->next->prev = &p->next;
        p->prev = &t->first;
        t->first = p;
    }
    else {
        p->prev = NULL;
        p->next = NULL;
    }
}

void stb__nptr_block(void* address, int len, void (*function)(stb__memory_leaf * f, int datum, void* start, void* end), int datum)
{
    void* end_address = (void*)((char*)address + len - 1);
    stb__memory_node* n;
    stb_uint32 start = (stb_uint32)address;
    stb_uint32 end = start + len - 1;

    int b0 = start >> STB__NPTR_ROOT_SHIFT;
    int b1 = end >> STB__NPTR_ROOT_SHIFT;
    int b = b0, i, e0, e1;

    e0 = STB__NPTR_NODE_OFFSET(start);

    if (datum <= 0) {
        // first block
        n = stb__memtab_root[b0];
        if (n) {
            if (b0 != b1)
                e1 = STB__NPTR_NODE_NUM - 1;
            else
                e1 = STB__NPTR_NODE_OFFSET(end);
            for (i = e0; i <= e1; ++i)
                if (n->children[i])
                    function(n->children[i], datum, address, end_address);
        }
        if (b1 > b0) {
            // blocks other than the first and last block
            for (b = b0 + 1; b < b1; ++b) {
                n = stb__memtab_root[b];
                if (n)
                    for (i = 0; i <= STB__NPTR_NODE_NUM - 1; ++i)
                        if (n->children[i])
                            function(n->children[i], datum, address, end_address);
            }
            // last block
            n = stb__memtab_root[b1];
            if (n) {
                e1 = STB__NPTR_NODE_OFFSET(end);
                for (i = 0; i <= e1; ++i)
                    if (n->children[i])
                        function(n->children[i], datum, address, end_address);
            }
        }
    }
    else {
        if (b1 > b0) {
            // last block
            n = stb__memtab_root[b1];
            if (n) {
                e1 = STB__NPTR_NODE_OFFSET(end);
                for (i = e1; i >= 0; --i)
                    if (n->children[i])
                        function(n->children[i], datum, address, end_address);
            }
            // blocks other than the first and last block
            for (b = b1 - 1; b > b0; --b) {
                n = stb__memtab_root[b];
                if (n)
                    for (i = STB__NPTR_NODE_NUM - 1; i >= 0; --i)
                        if (n->children[i])
                            function(n->children[i], datum, address, end_address);
            }
        }
        // first block
        n = stb__memtab_root[b0];
        if (n) {
            if (b0 != b1)
                e1 = STB__NPTR_NODE_NUM - 1;
            else
                e1 = STB__NPTR_NODE_OFFSET(end);
            for (i = e1; i >= e0; --i)
                if (n->children[i])
                    function(n->children[i], datum, address, end_address);
        }
    }
}

static void stb__nptr_delete_pointers(stb__memory_leaf * f, int offset, void* start, void* end)
{
    stb__nptr** p = &f->pointers;
    while (*p) {
        stb__nptr* n = *p;
        if (n->ptr >= start && n->ptr <= end) {
            // unlink
            if (n->prev) {
                *(n->prev) = n->next;
                if (n->next) n->next->prev = n->prev;
            }
            *p = n->next_in_block;
            stb__nptr_free(n);
        }
        else
            p = &(n->next_in_block);
    }
}

static void stb__nptr_delete_targets(stb__memory_leaf * f, int offset, void* start, void* end)
{
    stb__nptr_target** p = &f->targets;
    while (*p) {
        stb__nptr_target* n = *p;
        if (n->ptr >= start && n->ptr <= end) {
            // null pointers
            stb__nptr* z = n->first;
            while (z) {
                stb__nptr* y = z->next;
                z->prev = NULL;
                z->next = NULL;
                *(void**)z->ptr = NULL;
                z = y;
            }
            // unlink this target
            *p = n->next_in_block;
            stb__nptr_free(n);
        }
        else
            p = &(n->next_in_block);
    }
}

void stb_nptr_didfree(void* address_being_freed, int len)
{
    // step one: delete all pointers in this block
    stb__nptr_block(address_being_freed, len, stb__nptr_delete_pointers, 0);
    // step two: NULL all pointers to this block; do this second to avoid NULLing deleted pointers
    stb__nptr_block(address_being_freed, len, stb__nptr_delete_targets, 0);
}

void stb_nptr_free(void* address_being_freed, int len)
{
    free(address_being_freed);
    stb_nptr_didfree(address_being_freed, len);
}

static void stb__nptr_move_targets(stb__memory_leaf * f, int offset, void* start, void* end)
{
    stb__nptr_target** t = &f->targets;
    while (*t) {
        stb__nptr_target* n = *t;
        if (n->ptr >= start && n->ptr <= end) {
            stb__nptr* z;
            stb__memory_leaf* f;
            // unlink n
            *t = n->next_in_block;
            // update n to new address
            n->ptr = (void*)((char*)n->ptr + offset);
            f = stb__nptr_find_leaf(n->ptr);
            if (!f) f = stb__nptr_make_leaf(n->ptr);
            n->next_in_block = f->targets;
            f->targets = n;
            // now go through all pointers and make them point here
            z = n->first;
            while (z) {
                *(void**)z->ptr = n->ptr;
                z = z->next;
            }
        }
        else
            t = &(n->next_in_block);
    }
}

static void stb__nptr_move_pointers(stb__memory_leaf * f, int offset, void* start, void* end)
{
    stb__nptr** p = &f->pointers;
    while (*p) {
        stb__nptr* n = *p;
        if (n->ptr >= start && n->ptr <= end) {
            // unlink
            *p = n->next_in_block;
            n->ptr = (void*)((int)n->ptr + offset);
            // move to new block
            f = stb__nptr_find_leaf(n->ptr);
            if (!f) f = stb__nptr_make_leaf(n->ptr);
            n->next_in_block = f->pointers;
            f->pointers = n;
        }
        else
            p = &(n->next_in_block);
    }
}

void stb_nptr_realloc(void* new_address, void* old_address, int len)
{
    if (new_address == old_address) return;

    // have to move the pointers first, because moving the targets
    //      requires writing to the pointers-to-the-targets, and if some of those moved too,
    //      we need to make sure we don't write to the old memory

    // step one: move all pointers within the block
    stb__nptr_block(old_address, len, stb__nptr_move_pointers, (char*)new_address - (char*)old_address);
    // step two: move all targets within the block
    stb__nptr_block(old_address, len, stb__nptr_move_targets, (char*)new_address - (char*)old_address);
}

void stb_nptr_move(void* new_address, void* old_address)
{
    stb_nptr_realloc(new_address, old_address, 1);
}

void stb_nptr_recache(void)
{
    int i, j;
    for (i = 0; i < STB__NPTR_ROOT_NUM; ++i)
        if (stb__memtab_root[i])
            for (j = 0; j < STB__NPTR_NODE_NUM; ++j)
                if (stb__memtab_root[i]->children[j]) {
                    stb__nptr* p = stb__memtab_root[i]->children[j]->pointers;
                    while (p) {
                        stb_nptr_didset(p->ptr);
                        p = p->next_in_block;
                    }
                }
}

#endif // STB_DEFINE
#endif // STB_NPTR


//////////////////////////////////////////////////////////////////////////////
//
//                             File Processing
//


#ifdef _WIN32
#define stb_rename(x,y)   _wrename((const wchar_t *)stb__from_utf8(x), (const wchar_t *)stb__from_utf8_alt(y))
#else
#define stb_rename   rename
#endif

STB_EXTERN void     stb_fput_varlen64(FILE * f, stb_uint64 v);
STB_EXTERN stb_uint64  stb_fget_varlen64(FILE * f);
STB_EXTERN int      stb_size_varlen64(stb_uint64 v);


#define stb_filec    (char *) stb_file
#define stb_fileu    (unsigned char *) stb_file
STB_EXTERN void* stb_file(char* filename, size_t * length);
STB_EXTERN void* stb_file_max(char* filename, size_t * length);
STB_EXTERN size_t  stb_filelen(FILE * f);
STB_EXTERN int     stb_filewrite(char* filename, void* data, size_t length);
STB_EXTERN int     stb_filewritestr(char* filename, char* data);
STB_EXTERN char** stb_stringfile(char* filename, int* len);
STB_EXTERN char** stb_stringfile_trimmed(char* name, int* len, char comm);
STB_EXTERN char* stb_fgets(char* buffer, int buflen, FILE * f);
STB_EXTERN char* stb_fgets_malloc(FILE * f);
STB_EXTERN int     stb_fexists(char* filename);
STB_EXTERN int     stb_fcmp(char* s1, char* s2);
STB_EXTERN int     stb_feq(char* s1, char* s2);
STB_EXTERN time_t  stb_ftimestamp(char* filename);

STB_EXTERN int     stb_fullpath(char* abs, int abs_size, char* rel);
STB_EXTERN FILE* stb_fopen(char* filename, const char* mode);
STB_EXTERN int     stb_fclose(FILE * f, int keep);

enum
{
    stb_keep_no = 0,
    stb_keep_yes = 1,
    stb_keep_if_different = 2,
};

STB_EXTERN int     stb_copyfile(char* src, char* dest);

STB_EXTERN void     stb_fput_varlen64(FILE * f, stb_uint64 v);
STB_EXTERN stb_uint64  stb_fget_varlen64(FILE * f);
STB_EXTERN int      stb_size_varlen64(stb_uint64 v);

STB_EXTERN void    stb_fwrite32(FILE * f, stb_uint32 datum);
STB_EXTERN void    stb_fput_varlen(FILE * f, int v);
STB_EXTERN void    stb_fput_varlenu(FILE * f, unsigned int v);
STB_EXTERN int     stb_fget_varlen(FILE * f);
STB_EXTERN stb_uint stb_fget_varlenu(FILE * f);
STB_EXTERN void    stb_fput_ranged(FILE * f, int v, int b, stb_uint n);
STB_EXTERN int     stb_fget_ranged(FILE * f, int b, stb_uint n);
STB_EXTERN int     stb_size_varlen(int v);
STB_EXTERN int     stb_size_varlenu(unsigned int v);
STB_EXTERN int     stb_size_ranged(int b, stb_uint n);

STB_EXTERN int     stb_fread(void* data, size_t len, size_t count, void* f);
STB_EXTERN int     stb_fwrite(void* data, size_t len, size_t count, void* f);

#if 0
typedef struct
{
    FILE* base_file;
    char* buffer;
    int    buffer_size;
    int    buffer_off;
    int    buffer_left;
} STBF;

STB_EXTERN STBF* stb_tfopen(char* filename, char* mode);
STB_EXTERN int stb_tfread(void* data, size_t len, size_t count, STBF * f);
STB_EXTERN int stb_tfwrite(void* data, size_t len, size_t count, STBF * f);
#endif

#ifdef STB_DEFINE

#if 0
STBF* stb_tfopen(char* filename, char* mode)
{
    STBF* z;
    FILE* f = stb_p_fopen(filename, mode);
    if (!f) return NULL;
    z = (STBF*)malloc(sizeof(*z));
    if (!z) { fclose(f); return NULL; }
    z->base_file = f;
    if (!strcmp(mode, "rb") || !strcmp(mode, "wb")) {
        z->buffer_size = 4096;
        z->buffer_off = z->buffer_size;
        z->buffer_left = 0;
        z->buffer = malloc(z->buffer_size);
        if (!z->buffer) { free(z); fclose(f); return NULL; }
    }
    else {
        z->buffer = 0;
        z->buffer_size = 0;
        z->buffer_left = 0;
    }
    return z;
}

int stb_tfread(void* data, size_t len, size_t count, STBF * f)
{
    int total = len * count, done = 0;
    if (!total) return 0;
    if (total <= z->buffer_left) {
        memcpy(data, z->buffer + z->buffer_off, total);
        z->buffer_off += total;
        z->buffer_left -= total;
        return count;
    }
    else {
        char* out = (char*)data;

        // consume all buffered data
        memcpy(data, z->buffer + z->buffer_off, z->buffer_left);
        done = z->buffer_left;
        out += z->buffer_left;
        z->buffer_left = 0;

        if (total - done > (z->buffer_size >> 1)) {
            done += fread(out
        }
    }
}
#endif

void stb_fwrite32(FILE * f, stb_uint32 x)
{
    fwrite(&x, 4, 1, f);
}

#if defined(_WIN32)
#define stb__stat   _stat
#else
#define stb__stat   stat
#endif

int stb_fexists(char* filename)
{
    struct stb__stat buf;
    return stb__windows(
        _wstat((const wchar_t*)stb__from_utf8(filename), &buf),
        stat(filename, &buf)
    ) == 0;
}

time_t stb_ftimestamp(char* filename)
{
    struct stb__stat buf;
    if (stb__windows(
        _wstat((const wchar_t*)stb__from_utf8(filename), &buf),
        stat(filename, &buf)
    ) == 0)
    {
        return buf.st_mtime;
    }
    else {
        return 0;
    }
}

size_t  stb_filelen(FILE * f)
{
    long len, pos;
    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, pos, SEEK_SET);
    return (size_t)len;
}

void* stb_file(char* filename, size_t * length)
{
    FILE* f = stb__fopen(filename, "rb");
    char* buffer;
    size_t len, len2;
    if (!f) return NULL;
    len = stb_filelen(f);
    buffer = (char*)malloc(len + 2); // nul + extra
    len2 = fread(buffer, 1, len, f);
    if (len2 == len) {
        if (length) *length = len;
        buffer[len] = 0;
    }
    else {
        free(buffer);
        buffer = NULL;
    }
    fclose(f);
    return buffer;
}

int stb_filewrite(char* filename, void* data, size_t length)
{
    FILE* f = stb_fopen(filename, "wb");
    if (f) {
        unsigned char* data_ptr = (unsigned char*)data;
        size_t remaining = length;
        while (remaining > 0) {
            size_t len2 = remaining > 65536 ? 65536 : remaining;
            size_t len3 = fwrite(data_ptr, 1, len2, f);
            if (len2 != len3) {
                fprintf(stderr, "Failed while writing %s\n", filename);
                break;
            }
            remaining -= len2;
            data_ptr += len2;
        }
        stb_fclose(f, stb_keep_if_different);
    }
    return f != NULL;
}

int stb_filewritestr(char* filename, char* data)
{
    return stb_filewrite(filename, data, strlen(data));
}

void* stb_file_max(char* filename, size_t * length)
{
    FILE* f = stb__fopen(filename, "rb");
    char* buffer;
    size_t len, maxlen;
    if (!f) return NULL;
    maxlen = *length;
    buffer = (char*)malloc(maxlen + 1);
    len = fread(buffer, 1, maxlen, f);
    buffer[len] = 0;
    fclose(f);
    *length = len;
    return buffer;
}

char** stb_stringfile(char* filename, int* plen)
{
    FILE* f = stb__fopen(filename, "rb");
    char* buffer, ** list = NULL, * s;
    size_t len, count, i;

    if (!f) return NULL;
    len = stb_filelen(f);
    buffer = (char*)malloc(len + 1);
    len = fread(buffer, 1, len, f);
    buffer[len] = 0;
    fclose(f);

    // two passes through: first time count lines, second time set them
    for (i = 0; i < 2; ++i) {
        s = buffer;
        if (i == 1)
            list[0] = s;
        count = 1;
        while (*s) {
            if (*s == '\n' || *s == '\r') {
                // detect if both cr & lf are together
                int crlf = (s[0] + s[1]) == ('\n' + '\r');
                if (i == 1) *s = 0;
                if (crlf) ++s;
                if (s[1]) {  // it's not over yet
                    if (i == 1) list[count] = s + 1;
                    ++count;
                }
            }
            ++s;
        }
        if (i == 0) {
            list = (char**)malloc(sizeof(*list) * (count + 1) + len + 1);
            if (!list) return NULL;
            list[count] = 0;
            // recopy the file so there's just a single allocation to free
            memcpy(&list[count + 1], buffer, len + 1);
            free(buffer);
            buffer = (char*)&list[count + 1];
            if (plen) *plen = (int)count;
        }
    }
    return list;
}

char** stb_stringfile_trimmed(char* name, int* len, char comment)
{
    int i, n, o = 0;
    char** s = stb_stringfile(name, &n);
    if (s == NULL) return NULL;
    for (i = 0; i < n; ++i) {
        char* p = stb_skipwhite(s[i]);
        if (*p && *p != comment)
            s[o++] = p;
    }
    s[o] = NULL;
    if (len) *len = o;
    return s;
}

char* stb_fgets(char* buffer, int buflen, FILE * f)
{
    char* p;
    buffer[0] = 0;
    p = fgets(buffer, buflen, f);
    if (p) {
        int n = (int)(strlen(p) - 1);
        if (n >= 0)
            if (p[n] == '\n')
                p[n] = 0;
    }
    return p;
}

char* stb_fgets_malloc(FILE * f)
{
    // avoid reallocing for small strings
    char quick_buffer[800];
    quick_buffer[sizeof(quick_buffer) - 2] = 0;
    if (!fgets(quick_buffer, sizeof(quick_buffer), f))
        return NULL;

    if (quick_buffer[sizeof(quick_buffer) - 2] == 0) {
        size_t n = strlen(quick_buffer);
        if (n > 0 && quick_buffer[n - 1] == '\n')
            quick_buffer[n - 1] = 0;
        return stb_p_strdup(quick_buffer);
    }
    else {
        char* p;
        char* a = stb_p_strdup(quick_buffer);
        size_t len = sizeof(quick_buffer) - 1;

        while (!feof(f)) {
            if (a[len - 1] == '\n') break;
            a = (char*)realloc(a, len * 2);
            p = &a[len];
            p[len - 2] = 0;
            if (!fgets(p, (int)len, f))
                break;
            if (p[len - 2] == 0) {
                len += strlen(p);
                break;
            }
            len = len + (len - 1);
        }
        if (a[len - 1] == '\n')
            a[len - 1] = 0;
        return a;
    }
}

int stb_fullpath(char* abs, int abs_size, char* rel)
{
#ifdef _WIN32
    return _fullpath(abs, rel, abs_size) != NULL;
#else
    if (rel[0] == '/' || rel[0] == '~') {
        if ((int)strlen(rel) >= abs_size)
            return 0;
        stb_p_strcpy_s(abs, 65536, rel);
        return STB_TRUE;
    }
    else {
        int n;
        getcwd(abs, abs_size);
        n = strlen(abs);
        if (n + (int)strlen(rel) + 2 <= abs_size) {
            abs[n] = '/';
            stb_p_strcpy_s(abs + n + 1, 65536, rel);
            return STB_TRUE;
        }
        else {
            return STB_FALSE;
        }
    }
#endif
}

static int stb_fcmp_core(FILE * f, FILE * g)
{
    char buf1[1024], buf2[1024];
    int n1, n2, res = 0;

    while (1) {
        n1 = (int)fread(buf1, 1, sizeof(buf1), f);
        n2 = (int)fread(buf2, 1, sizeof(buf2), g);
        res = memcmp(buf1, buf2, stb_min(n1, n2));
        if (res)
            break;
        if (n1 != n2) {
            res = n1 < n2 ? -1 : 1;
            break;
        }
        if (n1 == 0)
            break;
    }

    fclose(f);
    fclose(g);
    return res;
}

int stb_fcmp(char* s1, char* s2)
{
    FILE* f = stb__fopen(s1, "rb");
    FILE* g = stb__fopen(s2, "rb");

    if (f == NULL || g == NULL) {
        if (f) fclose(f);
        if (g) {
            fclose(g);
            return STB_TRUE;
        }
        return f != NULL;
    }

    return stb_fcmp_core(f, g);
}

int stb_feq(char* s1, char* s2)
{
    FILE* f = stb__fopen(s1, "rb");
    FILE* g = stb__fopen(s2, "rb");

    if (f == NULL || g == NULL) {
        if (f) fclose(f);
        if (g) fclose(g);
        return f == g;
    }

    // feq is faster because it shortcuts if they're different length
    if (stb_filelen(f) != stb_filelen(g)) {
        fclose(f);
        fclose(g);
        return 0;
    }

    return !stb_fcmp_core(f, g);
}

static stb_ptrmap* stb__files;

typedef struct
{
    char* temp_name;
    char* name;
    int   errors;
} stb__file_data;

static FILE* stb__open_temp_file(char* temp_name, char* src_name, const char* mode)
{
    size_t p;
#ifdef _MSC_VER
    int j;
#endif
    FILE* f;
    // try to generate a temporary file in the same directory
    p = strlen(src_name) - 1;
    while (p > 0 && src_name[p] != '/' && src_name[p] != '\\'
        && src_name[p] != ':' && src_name[p] != '~')
        --p;
    ++p;

    memcpy(temp_name, src_name, p);

#ifdef _MSC_VER
    // try multiple times to make a temp file... just in
    // case some other process makes the name first
    for (j = 0; j < 32; ++j) {
        stb_p_strcpy_s(temp_name + p, 65536, "stmpXXXXXX");
        if (!stb_p_mktemp(temp_name))
            return 0;

        f = stb_p_fopen(temp_name, mode);
        if (f != NULL)
            break;
    }
#else
    {
        stb_p_strcpy_s(temp_name + p, 65536, "stmpXXXXXX");
#ifdef __MINGW32__
        int fd = open(stb_p_mktemp(temp_name), O_RDWR);
#else
        int fd = mkstemp(temp_name);
#endif
        if (fd == -1) return NULL;
        f = fdopen(fd, mode);
        if (f == NULL) {
            unlink(temp_name);
            close(fd);
            return NULL;
        }
    }
#endif
    return f;
}


FILE* stb_fopen(char* filename, const char* mode)
{
    FILE* f;
    char name_full[4096];
    char temp_full[sizeof(name_full) + 12];

    // @TODO: if the file doesn't exist, we can also use the fastpath here
    if (mode[0] != 'w' && !strchr(mode, '+'))
        return stb__fopen(filename, mode);

    // save away the full path to the file so if the program
    // changes the cwd everything still works right! unix has
    // better ways to do this, but we have to work in windows
    name_full[0] = '\0'; // stb_fullpath reads name_full[0]
    if (stb_fullpath(name_full, sizeof(name_full), filename) == 0)
        return 0;

    f = stb__open_temp_file(temp_full, name_full, mode);
    if (f != NULL) {
        stb__file_data* d = (stb__file_data*)malloc(sizeof(*d));
        if (!d) { assert(0);  /* NOTREACHED */fclose(f); return NULL; }
        if (stb__files == NULL) stb__files = stb_ptrmap_create();
        d->temp_name = stb_p_strdup(temp_full);
        d->name = stb_p_strdup(name_full);
        d->errors = 0;
        stb_ptrmap_add(stb__files, f, d);
        return f;
    }

    return NULL;
}

int     stb_fclose(FILE * f, int keep)
{
    stb__file_data* d;

    int ok = STB_FALSE;
    if (f == NULL) return 0;

    if (ferror(f))
        keep = stb_keep_no;

    fclose(f);

    if (stb__files && stb_ptrmap_remove(stb__files, f, (void**)&d)) {
        if (stb__files->count == 0) {
            stb_ptrmap_destroy(stb__files);
            stb__files = NULL;
        }
    }
    else
        return STB_TRUE; // not special

    if (keep == stb_keep_if_different) {
        // check if the files are identical
        if (stb_feq(d->name, d->temp_name)) {
            keep = stb_keep_no;
            ok = STB_TRUE;  // report success if no change
        }
    }

    if (keep == stb_keep_no) {
        remove(d->temp_name);
    }
    else {
        if (!stb_fexists(d->name)) {
            // old file doesn't exist, so just move the new file over it
            stb_rename(d->temp_name, d->name);
        }
        else {
            // don't delete the old file yet in case there are troubles! First rename it!
            char preserved_old_file[4096];

            // generate a temp filename in the same directory (also creates it, which we don't need)
            FILE* dummy = stb__open_temp_file(preserved_old_file, d->name, "wb");
            if (dummy != NULL) {
                // we don't actually want the open file
                fclose(dummy);

                // discard what we just created
                remove(preserved_old_file);  // if this fails, there's nothing we can do, and following logic handles it as best as possible anyway

                // move the existing file to the preserved name
                if (0 != stb_rename(d->name, preserved_old_file)) {  // 0 on success
                   // failed, state is:
                   //    filename  -> old file
                   //    tempname  -> new file
                   // keep tempname around so we don't lose data
                }
                else {
                    //  state is:
                    //    preserved -> old file
                    //    tempname  -> new file
                    // move the new file to the old name
                    if (0 == stb_rename(d->temp_name, d->name)) {
                        //  state is:
                        //    preserved -> old file
                        //    filename  -> new file
                        ok = STB_TRUE;

                        // 'filename -> new file' has always been the goal, so clean up
                        remove(preserved_old_file); // nothing to be done if it fails
                    }
                    else {
                        // couldn't rename, so try renaming preserved file back

                        //  state is:
                        //    preserved -> old file
                        //    tempname  -> new file
                        stb_rename(preserved_old_file, d->name);
                        // if the rename failed, there's nothing more we can do
                    }
                }
            }
            else {
                // we couldn't get a temp filename. do this the naive way; the worst case failure here
                // leaves the filename pointing to nothing and the new file as a tempfile
                remove(d->name);
                stb_rename(d->temp_name, d->name);
            }
        }
    }

    free(d->temp_name);
    free(d->name);
    free(d);

    return ok;
}

int stb_copyfile(char* src, char* dest)
{
    char raw_buffer[1024];
    char* buffer;
    int buf_size = 65536;

    FILE* f, * g;

    // if file already exists at destination, do nothing
    if (stb_feq(src, dest)) return STB_TRUE;

    // open file
    f = stb__fopen(src, "rb");
    if (f == NULL) return STB_FALSE;

    // open file for writing
    g = stb__fopen(dest, "wb");
    if (g == NULL) {
        fclose(f);
        return STB_FALSE;
    }

    buffer = (char*)malloc(buf_size);
    if (buffer == NULL) {
        buffer = raw_buffer;
        buf_size = sizeof(raw_buffer);
    }

    while (!feof(f)) {
        size_t n = fread(buffer, 1, buf_size, f);
        if (n != 0)
            fwrite(buffer, 1, n, g);
    }

    fclose(f);
    if (buffer != raw_buffer)
        free(buffer);

    fclose(g);
    return STB_TRUE;
}

// varlen:
//    v' = (v >> 31) + (v < 0 ? ~v : v)<<1;  // small abs(v) => small v'
// output v as big endian v'+k for v' <= k:
//   1 byte :  v' <= 0x00000080          (  -64 <= v <   64)   7 bits
//   2 bytes:  v' <= 0x00004000          (-8192 <= v < 8192)  14 bits
//   3 bytes:  v' <= 0x00200000                               21 bits
//   4 bytes:  v' <= 0x10000000                               28 bits
// the number of most significant 1-bits in the first byte
// equals the number of bytes after the first

#define stb__varlen_xform(v)     (v<0 ? (~v << 1)+1 : (v << 1))

int stb_size_varlen(int v) { return stb_size_varlenu(stb__varlen_xform(v)); }
int stb_size_varlenu(unsigned int v)
{
    if (v < 0x00000080) return 1;
    if (v < 0x00004000) return 2;
    if (v < 0x00200000) return 3;
    if (v < 0x10000000) return 4;
    return 5;
}

void    stb_fput_varlen(FILE * f, int v) { stb_fput_varlenu(f, stb__varlen_xform(v)); }

void    stb_fput_varlenu(FILE * f, unsigned int z)
{
    if (z >= 0x10000000) fputc(0xF0, f);
    if (z >= 0x00200000) fputc((z < 0x10000000 ? 0xE0 : 0) + (z >> 24), f);
    if (z >= 0x00004000) fputc((z < 0x00200000 ? 0xC0 : 0) + (z >> 16), f);
    if (z >= 0x00000080) fputc((z < 0x00004000 ? 0x80 : 0) + (z >> 8), f);
    fputc(z, f);
}

#define stb_fgetc(f)    ((unsigned char) fgetc(f))

int     stb_fget_varlen(FILE * f)
{
    unsigned int z = stb_fget_varlenu(f);
    return (z & 1) ? ~(z >> 1) : (z >> 1);
}

unsigned int stb_fget_varlenu(FILE * f)
{
    unsigned int z;
    unsigned char d;
    d = stb_fgetc(f);

    if (d >= 0x80) {
        if (d >= 0xc0) {
            if (d >= 0xe0) {
                if (d == 0xf0) z = stb_fgetc(f) << 24;
                else           z = (d - 0xe0) << 24;
                z += stb_fgetc(f) << 16;
            }
            else
                z = (d - 0xc0) << 16;
            z += stb_fgetc(f) << 8;
        }
        else
            z = (d - 0x80) << 8;
        z += stb_fgetc(f);
    }
    else
        z = d;
    return z;
}

stb_uint64   stb_fget_varlen64(FILE * f)
{
    stb_uint64 z;
    unsigned char d;
    d = stb_fgetc(f);

    if (d >= 0x80) {
        if (d >= 0xc0) {
            if (d >= 0xe0) {
                if (d >= 0xf0) {
                    if (d >= 0xf8) {
                        if (d >= 0xfc) {
                            if (d >= 0xfe) {
                                if (d >= 0xff)
                                    z = (stb_uint64)stb_fgetc(f) << 56;
                                else
                                    z = (stb_uint64)(d - 0xfe) << 56;
                                z |= (stb_uint64)stb_fgetc(f) << 48;
                            }
                            else z = (stb_uint64)(d - 0xfc) << 48;
                            z |= (stb_uint64)stb_fgetc(f) << 40;
                        }
                        else z = (stb_uint64)(d - 0xf8) << 40;
                        z |= (stb_uint64)stb_fgetc(f) << 32;
                    }
                    else z = (stb_uint64)(d - 0xf0) << 32;
                    z |= (stb_uint)stb_fgetc(f) << 24;
                }
                else z = (stb_uint)(d - 0xe0) << 24;
                z |= (stb_uint)stb_fgetc(f) << 16;
            }
            else z = (stb_uint)(d - 0xc0) << 16;
            z |= (stb_uint)stb_fgetc(f) << 8;
        }
        else z = (stb_uint)(d - 0x80) << 8;
        z |= stb_fgetc(f);
    }
    else
        z = d;

    return (z & 1) ? ~(z >> 1) : (z >> 1);
}

int stb_size_varlen64(stb_uint64 v)
{
    if (v < 0x00000080) return 1;
    if (v < 0x00004000) return 2;
    if (v < 0x00200000) return 3;
    if (v < 0x10000000) return 4;
    if (v < STB_IMM_UINT64(0x0000000800000000)) return 5;
    if (v < STB_IMM_UINT64(0x0000040000000000)) return 6;
    if (v < STB_IMM_UINT64(0x0002000000000000)) return 7;
    if (v < STB_IMM_UINT64(0x0100000000000000)) return 8;
    return 9;
}

void    stb_fput_varlen64(FILE * f, stb_uint64 v)
{
    stb_uint64 z = stb__varlen_xform(v);
    int first = 1;
    if (z >= STB_IMM_UINT64(0x100000000000000)) {
        fputc(0xff, f);
        first = 0;
    }
    if (z >= STB_IMM_UINT64(0x02000000000000)) fputc((first ? 0xFE : 0) + (char)(z >> 56), f), first = 0;
    if (z >= STB_IMM_UINT64(0x00040000000000)) fputc((first ? 0xFC : 0) + (char)(z >> 48), f), first = 0;
    if (z >= STB_IMM_UINT64(0x00000800000000)) fputc((first ? 0xF8 : 0) + (char)(z >> 40), f), first = 0;
    if (z >= STB_IMM_UINT64(0x00000010000000)) fputc((first ? 0xF0 : 0) + (char)(z >> 32), f), first = 0;
    if (z >= STB_IMM_UINT64(0x00000000200000)) fputc((first ? 0xE0 : 0) + (char)(z >> 24), f), first = 0;
    if (z >= STB_IMM_UINT64(0x00000000004000)) fputc((first ? 0xC0 : 0) + (char)(z >> 16), f), first = 0;
    if (z >= STB_IMM_UINT64(0x00000000000080)) fputc((first ? 0x80 : 0) + (char)(z >> 8), f), first = 0;
    fputc((char)z, f);
}

void    stb_fput_ranged(FILE * f, int v, int b, stb_uint n)
{
    v -= b;
    if (n <= (1 << 31))
        assert((stb_uint)v < n);
    if (n > (1 << 24)) fputc(v >> 24, f);
    if (n > (1 << 16)) fputc(v >> 16, f);
    if (n > (1 << 8)) fputc(v >> 8, f);
    fputc(v, f);
}

int     stb_fget_ranged(FILE * f, int b, stb_uint n)
{
    unsigned int v = 0;
    if (n > (1 << 24)) v += stb_fgetc(f) << 24;
    if (n > (1 << 16)) v += stb_fgetc(f) << 16;
    if (n > (1 << 8)) v += stb_fgetc(f) << 8;
    v += stb_fgetc(f);
    return b + v;
}

int     stb_size_ranged(int b, stb_uint n)
{
    if (n > (1 << 24)) return 4;
    if (n > (1 << 16)) return 3;
    if (n > (1 << 8)) return 2;
    return 1;
}

void stb_fput_string(FILE * f, char* s)
{
    size_t len = strlen(s);
    stb_fput_varlenu(f, (unsigned int)len);
    fwrite(s, 1, len, f);
}

// inverse of the above algorithm
char* stb_fget_string(FILE * f, void* p)
{
    char* s;
    int len = stb_fget_varlenu(f);
    if (len > 4096) return NULL;
    s = p ? stb_malloc_string(p, len + 1) : (char*)malloc(len + 1);
    fread(s, 1, len, f);
    s[len] = 0;
    return s;
}

char* stb_strdup(char* str, void* pool)
{
    size_t len = strlen(str);
    char* p = stb_malloc_string(pool, len + 1);
    stb_p_strcpy_s(p, len + 1, str);
    return p;
}

// strip the trailing '/' or '\\' from a directory so we can refer to it
// as a file for _stat()
char* stb_strip_final_slash(char* t)
{
    if (t[0]) {
        char* z = t + strlen(t) - 1;
        // *z is the last character
        if (*z == '\\' || *z == '/')
            if (z != t + 2 || t[1] != ':') // but don't strip it if it's e.g. "c:/"
                *z = 0;
        if (*z == '\\')
            *z = '/'; // canonicalize to make sure it matches db
    }
    return t;
}

char* stb_strip_final_slash_regardless(char* t)
{
    if (t[0]) {
        char* z = t + strlen(t) - 1;
        // *z is the last character
        if (*z == '\\' || *z == '/')
            *z = 0;
        if (*z == '\\')
            *z = '/'; // canonicalize to make sure it matches db
    }
    return t;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                 Options parsing
//

STB_EXTERN char** stb_getopt_param(int* argc, char** argv, char* param);
STB_EXTERN char** stb_getopt(int* argc, char** argv);
STB_EXTERN void   stb_getopt_free(char** opts);

#ifdef STB_DEFINE

void   stb_getopt_free(char** opts)
{
    int i;
    char** o2 = opts;
    for (i = 0; i < stb_arr_len(o2); ++i)
        free(o2[i]);
    stb_arr_free(o2);
}

char** stb_getopt(int* argc, char** argv)
{
    return stb_getopt_param(argc, argv, (char*)"");
}

char** stb_getopt_param(int* argc, char** argv, char* param)
{
    char** opts = NULL;
    int i, j = 1;
    for (i = 1; i < *argc; ++i) {
        if (argv[i][0] != '-') {
            argv[j++] = argv[i];
        }
        else {
            if (argv[i][1] == 0) { // plain - == don't parse further options
                ++i;
                while (i < *argc)
                    argv[j++] = argv[i++];
                break;
            }
            else if (argv[i][1] == '-') {
                // copy argument through including initial '-' for clarity
                stb_arr_push(opts, stb_p_strdup(argv[i]));
            }
            else {
                int k;
                char* q = argv[i];  // traverse options list
                for (k = 1; q[k]; ++k) {
                    char* s;
                    if (strchr(param, q[k])) {  // does it take a parameter?
                        char* t = &q[k + 1], z = q[k];
                        size_t len = 0;
                        if (*t == 0) {
                            if (i == *argc - 1) { // takes a parameter, but none found
                                *argc = 0;
                                stb_getopt_free(opts);
                                return NULL;
                            }
                            t = argv[++i];
                        }
                        else
                            k += (int)strlen(t);
                        len = strlen(t);
                        s = (char*)malloc(len + 2);
                        if (!s) return NULL;
                        s[0] = z;
                        stb_p_strcpy_s(s + 1, len + 2, t);
                    }
                    else {
                        // no parameter
                        s = (char*)malloc(2);
                        if (!s) return NULL;
                        s[0] = q[k];
                        s[1] = 0;
                    }
                    stb_arr_push(opts, s);
                }
            }
        }
    }
    stb_arr_push(opts, NULL);
    *argc = j;
    return opts;
}
#endif


#if 0

//////////////////////////////////////////////////////////////////////////////
//
//                 Portable directory reading
//

STB_EXTERN char** stb_readdir_files(char* dir);
STB_EXTERN char** stb_readdir_files_mask(char* dir, char* wild);
STB_EXTERN char** stb_readdir_subdirs(char* dir);
STB_EXTERN char** stb_readdir_subdirs_mask(char* dir, char* wild);
STB_EXTERN void   stb_readdir_free(char** files);
STB_EXTERN char** stb_readdir_recursive(char* dir, char* filespec);
STB_EXTERN void stb_delete_directory_recursive(char* dir);

#ifdef STB_DEFINE

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

void stb_readdir_free(char** files)
{
    char** f2 = files;
    int i;
    for (i = 0; i < stb_arr_len(f2); ++i)
        free(f2[i]);
    stb_arr_free(f2);
}

static int isdotdirname(char* name)
{
    if (name[0] == '.')
        return (name[1] == '.') ? !name[2] : !name[1];
    return 0;
}


STB_EXTERN int stb_wildmatchi(char* expr, char* candidate);
static char** readdir_raw(char* dir, int return_subdirs, char* mask)
{
    char** results = NULL;
    char buffer[4096], with_slash[4096];
    size_t n;

#ifdef WIN32
    stb__wchar* ws;
    struct _wfinddata_t data;
#ifdef _WIN64
    const intptr_t none = -1;
    intptr_t z;
#else
    const long none = -1;
    long z;
#endif
#else // !WIN32
    const DIR* none = NULL;
    DIR* z;
#endif

    n = stb_strscpy(buffer, dir, sizeof(buffer));
    if (!n || n >= sizeof(buffer))
        return NULL;
    stb_fixpath(buffer);

    if (n > 0 && (buffer[n - 1] != '/')) {
        buffer[n++] = '/';
    }
    buffer[n] = 0;
    if (!stb_strscpy(with_slash, buffer, sizeof(with_slash)))
        return NULL;


#ifdef WIN32
    if (!stb_strscpy(buffer + n, "*.*", sizeof(buffer) - n))
        return NULL;
    ws = stb__from_utf8(buffer);
    z = _wfindfirst((wchar_t*)ws, &data);
#else
    z = opendir(dir);
#endif

    if (z != none) {
        int nonempty = STB_TRUE;
#ifndef WIN32
        struct dirent* data = readdir(z);
        nonempty = (data != NULL);
#endif

        if (nonempty) {

            do {
                int is_subdir;
#ifdef WIN32
                char* name = stb__to_utf8((stb__wchar*)data.name);
                if (name == NULL) {
                    fprintf(stderr, "%s to convert '%S' to %s!\n", "Unable", data.name, "utf8");
                    continue;
                }
                is_subdir = !!(data.attrib & _A_SUBDIR);
#else
                char* name = data->d_name;
                if (!stb_strscpy(buffer + n, name, sizeof(buffer) - n))
                    break;
                // Could follow DT_LNK, but would need to check for recursive links.
                is_subdir = !!(data->d_type & DT_DIR);
#endif

                if (is_subdir == return_subdirs) {
                    if (!is_subdir || !isdotdirname(name)) {
                        if (!mask || stb_wildmatchi(mask, name)) {
                            char buffer[4096], * p = buffer;
                            if (stb_snprintf(buffer, sizeof(buffer), "%s%s", with_slash, name) < 0)
                                break;
                            if (buffer[0] == '.' && buffer[1] == '/')
                                p = buffer + 2;
                            stb_arr_push(results, stb_p_strdup(p));
                        }
                    }
                }
            }
#ifdef WIN32
            while (0 == _wfindnext(z, &data));
#else
            while ((data = readdir(z)) != NULL);
#endif
        }
#ifdef WIN32
        _findclose(z);
#else
        closedir(z);
#endif
    }
    return results;
}


char** stb_readdir_files(char* dir) { return readdir_raw(dir, 0, NULL); }
char** stb_readdir_subdirs(char* dir) { return readdir_raw(dir, 1, NULL); }
char** stb_readdir_files_mask(char* dir, char* wild) { return readdir_raw(dir, 0, wild); }
char** stb_readdir_subdirs_mask(char* dir, char* wild) { return readdir_raw(dir, 1, wild); }

int stb__rec_max = 0x7fffffff;
static char** stb_readdir_rec(char** sofar, char* dir, char* filespec)
{
    char** files;
    char** dirs;
    char** p;

    if (stb_arr_len(sofar) >= stb__rec_max) return sofar;

    files = stb_readdir_files_mask(dir, filespec);
    stb_arr_for(p, files) {
        stb_arr_push(sofar, stb_p_strdup(*p));
        if (stb_arr_len(sofar) >= stb__rec_max) break;
    }
    stb_readdir_free(files);
    if (stb_arr_len(sofar) >= stb__rec_max) return sofar;

    dirs = stb_readdir_subdirs(dir);
    stb_arr_for(p, dirs)
        sofar = stb_readdir_rec(sofar, *p, filespec);
    stb_readdir_free(dirs);
    return sofar;
}

char** stb_readdir_recursive(char* dir, char* filespec)
{
    return stb_readdir_rec(NULL, dir, filespec);
}

void stb_delete_directory_recursive(char* dir)
{
    char** list = stb_readdir_subdirs(dir);
    int i;
    for (i = 0; i < stb_arr_len(list); ++i)
        stb_delete_directory_recursive(list[i]);
    stb_arr_free(list);
    list = stb_readdir_files(dir);
    for (i = 0; i < stb_arr_len(list); ++i)
        if (!remove(list[i])) {
            // on windows, try again after making it writeable; don't ALWAYS
            // do this first since that would be slow in the normal case
#ifdef _MSC_VER
            _chmod(list[i], _S_IWRITE);
            remove(list[i]);
#endif
        }
    stb_arr_free(list);
    stb__windows(_rmdir, rmdir)(dir);
}

#endif

#endif

//////////////////////////////////////////////////////////////////////////////
//
//   construct trees from filenames; useful for cmirror summaries

typedef struct stb_dirtree2 stb_dirtree2;

struct stb_dirtree2
{
    stb_dirtree2** subdirs;

    // make convenient for stb_summarize_tree
    int num_subdir;
    float weight;

    // actual data
    char* fullpath;
    char* relpath;
    char** files;
};

STB_EXTERN stb_dirtree2* stb_dirtree2_from_files_relative(char* src, char** filelist, int count);
STB_EXTERN stb_dirtree2* stb_dirtree2_from_files(char** filelist, int count);
STB_EXTERN int stb_dir_is_prefix(char* dir, int dirlen, char* file);

#ifdef STB_DEFINE

int stb_dir_is_prefix(char* dir, int dirlen, char* file)
{
    if (dirlen == 0) return STB_TRUE;
    if (stb_strnicmp(dir, file, dirlen)) return STB_FALSE;
    if (file[dirlen] == '/' || file[dirlen] == '\\') return STB_TRUE;
    return STB_FALSE;
}

stb_dirtree2* stb_dirtree2_from_files_relative(char* src, char** filelist, int count)
{
    char buffer1[1024];
    int i;
    int dlen = (int)strlen(src), elen;
    stb_dirtree2* d;
    char** descendents = NULL;
    char** files = NULL;
    char* s;
    if (!count) return NULL;
    // first find all the ones that belong here... note this is will take O(NM) with N files and M subdirs
    for (i = 0; i < count; ++i) {
        if (stb_dir_is_prefix(src, dlen, filelist[i])) {
            stb_arr_push(descendents, filelist[i]);
        }
    }
    if (descendents == NULL)
        return NULL;
    elen = dlen;
    // skip a leading slash
    if (elen == 0 && (descendents[0][0] == '/' || descendents[0][0] == '\\'))
        ++elen;
    else if (elen)
        ++elen;
    // now extract all the ones that have their root here
    for (i = 0; i < stb_arr_len(descendents);) {
        if (!stb_strchr2(descendents[i] + elen, '/', '\\')) {
            stb_arr_push(files, descendents[i]);
            descendents[i] = descendents[stb_arr_len(descendents) - 1];
            stb_arr_pop(descendents);
        }
        else
            ++i;
    }
    // now create a record
    d = (stb_dirtree2*)malloc(sizeof(*d));
    d->files = files;
    d->subdirs = NULL;
    d->fullpath = stb_p_strdup(src);
    s = stb_strrchr2(d->fullpath, '/', '\\');
    if (s)
        ++s;
    else
        s = d->fullpath;
    d->relpath = s;
    // now create the children
    qsort(descendents, stb_arr_len(descendents), sizeof(char*), stb_qsort_stricmp(0));
    buffer1[0] = 0;
    for (i = 0; i < stb_arr_len(descendents); ++i) {
        char buffer2[1024];
        char* s = descendents[i] + elen, * t;
        t = stb_strchr2(s, '/', '\\');
        assert(t);
        stb_strncpy(buffer2, descendents[i], (int)(t - descendents[i] + 1));
        if (stb_stricmp(buffer1, buffer2)) {
            stb_dirtree2* t = stb_dirtree2_from_files_relative(buffer2, descendents, stb_arr_len(descendents));
            assert(t != NULL);
            stb_p_strcpy_s(buffer1, sizeof(buffer1), buffer2);
            stb_arr_push(d->subdirs, t);
        }
    }
    d->num_subdir = stb_arr_len(d->subdirs);
    d->weight = 0;
    return d;
}

stb_dirtree2* stb_dirtree2_from_files(char** filelist, int count)
{
    return stb_dirtree2_from_files_relative((char*)"", filelist, count);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                 Checksums: CRC-32, ADLER32, SHA-1
//
//    CRC-32 and ADLER32 allow streaming blocks
//    SHA-1 requires either a complete buffer, max size 2^32 - 73
//          or it can checksum directly from a file, max 2^61

#define STB_ADLER32_SEED   1
#define STB_CRC32_SEED     0    // note that we logical NOT this in the code

STB_EXTERN stb_uint
stb_adler32(stb_uint adler32, stb_uchar * buffer, stb_uint buflen);
STB_EXTERN stb_uint
stb_crc32_block(stb_uint crc32, stb_uchar * buffer, stb_uint len);
STB_EXTERN stb_uint stb_crc32(unsigned char* buffer, stb_uint len);

STB_EXTERN void stb_sha1(
    unsigned char output[20], unsigned char* buffer, unsigned int len);
STB_EXTERN int stb_sha1_file(unsigned char output[20], char* file);

STB_EXTERN void stb_sha1_readable(char display[27], unsigned char sha[20]);

#ifdef STB_DEFINE
stb_uint stb_crc32_block(stb_uint crc, unsigned char* buffer, stb_uint len)
{
    static stb_uint crc_table[256];
    stb_uint i, j, s;
    crc = ~crc;

    if (crc_table[1] == 0)
        for (i = 0; i < 256; i++) {
            for (s = i, j = 0; j < 8; ++j)
                s = (s >> 1) ^ (s & 1 ? 0xedb88320 : 0);
            crc_table[i] = s;
        }
    for (i = 0; i < len; ++i)
        crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
    return ~crc;
}

stb_uint stb_crc32(unsigned char* buffer, stb_uint len)
{
    return stb_crc32_block(0, buffer, len);
}

stb_uint stb_adler32(stb_uint adler32, stb_uchar * buffer, stb_uint buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen, i;

    blocklen = buflen % 5552;
    while (buflen) {
        for (i = 0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (s2 << 16) + s1;
}

static void stb__sha1(stb_uchar * chunk, stb_uint h[5])
{
    int i;
    stb_uint a, b, c, d, e;
    stb_uint w[80];

    for (i = 0; i < 16; ++i)
        w[i] = stb_big32(&chunk[i * 4]);
    for (i = 16; i < 80; ++i) {
        stb_uint t;
        t = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
        w[i] = (t + t) | (t >> 31);
    }

    a = h[0];
    b = h[1];
    c = h[2];
    d = h[3];
    e = h[4];

#define STB__SHA1(k,f)                                            \
   {                                                                 \
      stb_uint temp = (a << 5) + (a >> 27) + (f) + e + (k) + w[i];  \
      e = d;                                                       \
      d = c;                                                     \
      c = (b << 30) + (b >> 2);                               \
      b = a;                                              \
      a = temp;                                    \
   }

    i = 0;
    for (; i < 20; ++i) STB__SHA1(0x5a827999, d ^ (b & (c ^ d)));
    for (; i < 40; ++i) STB__SHA1(0x6ed9eba1, b ^ c ^ d);
    for (; i < 60; ++i) STB__SHA1(0x8f1bbcdc, (b & c) + (d & (b ^ c)));
    for (; i < 80; ++i) STB__SHA1(0xca62c1d6, b ^ c ^ d);

#undef STB__SHA1

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

void stb_sha1(stb_uchar output[20], stb_uchar * buffer, stb_uint len)
{
    unsigned char final_block[128];
    stb_uint end_start, final_len, j;
    int i;

    stb_uint h[5];

    h[0] = 0x67452301;
    h[1] = 0xefcdab89;
    h[2] = 0x98badcfe;
    h[3] = 0x10325476;
    h[4] = 0xc3d2e1f0;

    // we need to write padding to the last one or two
    // blocks, so build those first into 'final_block'

    // we have to write one special byte, plus the 8-byte length

    // compute the block where the data runs out
    end_start = len & ~63;

    // compute the earliest we can encode the length
    if (((len + 9) & ~63) == end_start) {
        // it all fits in one block, so fill a second-to-last block
        end_start -= 64;
    }

    final_len = end_start + 128;

    // now we need to copy the data in
    assert(end_start + 128 >= len + 9);
    assert(end_start < len || len < 64 - 9);

    j = 0;
    if (end_start > len)
        j = (stb_uint)-(int)end_start;

    for (; end_start + j < len; ++j)
        final_block[j] = buffer[end_start + j];
    final_block[j++] = 0x80;
    while (j < 128 - 5) // 5 byte length, so write 4 extra padding bytes
        final_block[j++] = 0;
    // big-endian size
    final_block[j++] = len >> 29;
    final_block[j++] = len >> 21;
    final_block[j++] = len >> 13;
    final_block[j++] = len >> 5;
    final_block[j++] = len << 3;
    assert(j == 128 && end_start + j == final_len);

    for (j = 0; j < final_len; j += 64) { // 512-bit chunks
        if (j + 64 >= end_start + 64)
            stb__sha1(&final_block[j - end_start], h);
        else
            stb__sha1(&buffer[j], h);
    }

    for (i = 0; i < 5; ++i) {
        output[i * 4 + 0] = h[i] >> 24;
        output[i * 4 + 1] = h[i] >> 16;
        output[i * 4 + 2] = h[i] >> 8;
        output[i * 4 + 3] = h[i] >> 0;
    }
}

#ifdef _MSC_VER
int stb_sha1_file(stb_uchar output[20], char* file)
{
    int i;
    stb_uint64 length = 0;
    unsigned char buffer[128];

    FILE* f = stb__fopen(file, "rb");
    stb_uint h[5];

    if (f == NULL) return 0; // file not found

    h[0] = 0x67452301;
    h[1] = 0xefcdab89;
    h[2] = 0x98badcfe;
    h[3] = 0x10325476;
    h[4] = 0xc3d2e1f0;

    for (;;) {
        size_t n = fread(buffer, 1, 64, f);
        if (n == 64) {
            stb__sha1(buffer, h);
            length += n;
        }
        else {
            int block = 64;

            length += n;

            buffer[n++] = 0x80;

            // if there isn't enough room for the length, double the block
            if (n + 8 > 64)
                block = 128;

            // pad to end
            memset(buffer + n, 0, block - 8 - n);

            i = block - 8;
            buffer[i++] = (stb_uchar)(length >> 53);
            buffer[i++] = (stb_uchar)(length >> 45);
            buffer[i++] = (stb_uchar)(length >> 37);
            buffer[i++] = (stb_uchar)(length >> 29);
            buffer[i++] = (stb_uchar)(length >> 21);
            buffer[i++] = (stb_uchar)(length >> 13);
            buffer[i++] = (stb_uchar)(length >> 5);
            buffer[i++] = (stb_uchar)(length << 3);
            assert(i == block);
            stb__sha1(buffer, h);
            if (block == 128)
                stb__sha1(buffer + 64, h);
            else
                assert(block == 64);
            break;
        }
    }
    fclose(f);

    for (i = 0; i < 5; ++i) {
        output[i * 4 + 0] = h[i] >> 24;
        output[i * 4 + 1] = h[i] >> 16;
        output[i * 4 + 2] = h[i] >> 8;
        output[i * 4 + 3] = h[i] >> 0;
    }

    return 1;
}
#endif // _MSC_VER

// client can truncate this wherever they like
void stb_sha1_readable(char display[27], unsigned char sha[20])
{
    char encoding[65] = "0123456789abcdefghijklmnopqrstuv"
        "wxyzABCDEFGHIJKLMNOPQRSTUVWXYZ%$";
    int num_bits = 0, acc = 0;
    int i = 0, o = 0;
    while (o < 26) {
        int v;
        // expand the accumulator
        if (num_bits < 6) {
            assert(i != 20);
            acc += sha[i++] << num_bits;
            num_bits += 8;
        }
        v = acc & ((1 << 6) - 1);
        display[o++] = encoding[v];
        acc >>= 6;
        num_bits -= 6;
    }
    assert(num_bits == 20 * 8 - 26 * 6);
    display[o++] = encoding[acc];
}

#endif // STB_DEFINE

///////////////////////////////////////////////////////////
//
// simplified WINDOWS registry interface... hopefully
// we'll never actually use this?

#if defined(_WIN32)

STB_EXTERN void* stb_reg_open(const char* mode, const char* where); // mode: "rHKLM" or "rHKCU" or "w.."
STB_EXTERN void   stb_reg_close(void* reg);
STB_EXTERN int    stb_reg_read(void* zreg, const char* str, void* data, unsigned long len);
STB_EXTERN int    stb_reg_read_string(void* zreg, const char* str, char* data, int len);
STB_EXTERN void   stb_reg_write(void* zreg, const char* str, const void* data, unsigned long len);
STB_EXTERN void   stb_reg_write_string(void* zreg, const char* str, const char* data);

#if defined(STB_DEFINE) && !defined(STB_NO_REGISTRY)

#define STB_HAS_REGISTRY

#ifndef _WINDOWS_

#define HKEY void *

STB_EXTERN __declspec(dllimport) long __stdcall RegCloseKey(HKEY hKey);
STB_EXTERN __declspec(dllimport) long __stdcall RegCreateKeyExA(HKEY hKey, const char* lpSubKey,
    int  Reserved, char* lpClass, int  dwOptions,
    int samDesired, void* lpSecurityAttributes, HKEY * phkResult, int* lpdwDisposition);
STB_EXTERN __declspec(dllimport) long __stdcall RegDeleteKeyA(HKEY hKey, const char* lpSubKey);
STB_EXTERN __declspec(dllimport) long __stdcall RegQueryValueExA(HKEY hKey, const char* lpValueName,
    int* lpReserved, unsigned long* lpType, unsigned char* lpData, unsigned long* lpcbData);
STB_EXTERN __declspec(dllimport) long __stdcall RegSetValueExA(HKEY hKey, const char* lpValueName,
    int  Reserved, int  dwType, const unsigned char* lpData, int  cbData);
STB_EXTERN __declspec(dllimport) long __stdcall  RegOpenKeyExA(HKEY hKey, const char* lpSubKey,
    int ulOptions, int samDesired, HKEY * phkResult);

#endif // _WINDOWS_

#define STB__REG_OPTION_NON_VOLATILE  0
#define STB__REG_KEY_ALL_ACCESS       0x000f003f
#define STB__REG_KEY_READ             0x00020019

#ifdef _M_AMD64
#define STB__HKEY_CURRENT_USER        0x80000001ull
#define STB__HKEY_LOCAL_MACHINE       0x80000002ull
#else
#define STB__HKEY_CURRENT_USER        0x80000001
#define STB__HKEY_LOCAL_MACHINE       0x80000002
#endif

void* stb_reg_open(const char* mode, const char* where)
{
    long res;
    HKEY base;
    HKEY zreg;
    if (!stb_stricmp(mode + 1, "cu") || !stb_stricmp(mode + 1, "hkcu"))
        base = (HKEY)STB__HKEY_CURRENT_USER;
    else if (!stb_stricmp(mode + 1, "lm") || !stb_stricmp(mode + 1, "hklm"))
        base = (HKEY)STB__HKEY_LOCAL_MACHINE;
    else
        return NULL;

    if (mode[0] == 'r')
        res = RegOpenKeyExA(base, where, 0, STB__REG_KEY_READ, &zreg);
    else if (mode[0] == 'w')
        res = RegCreateKeyExA(base, where, 0, NULL, STB__REG_OPTION_NON_VOLATILE, STB__REG_KEY_ALL_ACCESS, NULL, &zreg, NULL);
    else
        return NULL;

    return res ? NULL : zreg;
}

void stb_reg_close(void* reg)
{
    RegCloseKey((HKEY)reg);
}

#define STB__REG_SZ         1
#define STB__REG_BINARY     3
#define STB__REG_DWORD      4

int stb_reg_read(void* zreg, const char* str, void* data, unsigned long len)
{
    unsigned long type;
    unsigned long alen = len;
    if (0 == RegQueryValueExA((HKEY)zreg, str, 0, &type, (unsigned char*)data, &len))
        if (type == STB__REG_BINARY || type == STB__REG_SZ || type == STB__REG_DWORD) {
            if (len < alen)
                *((char*)data + len) = 0;
            return 1;
        }
    return 0;
}

void stb_reg_write(void* zreg, const char* str, const void* data, unsigned long len)
{
    if (zreg)
        RegSetValueExA((HKEY)zreg, str, 0, STB__REG_BINARY, (const unsigned char*)data, len);
}

int stb_reg_read_string(void* zreg, const char* str, char* data, int len)
{
    if (!stb_reg_read(zreg, str, data, len)) return 0;
    data[len - 1] = 0; // force a 0 at the end of the string no matter what
    return 1;
}

void stb_reg_write_string(void* zreg, const char* str, const char* data)
{
    if (zreg)
        RegSetValueExA((HKEY)zreg, str, 0, STB__REG_SZ, (const unsigned char*)data, (int)strlen(data) + 1);
}
#endif  // STB_DEFINE
#endif  // _WIN32


//////////////////////////////////////////////////////////////////////////////
//
//     stb_cfg - This is like the registry, but the config info
//               is all stored in plain old files where we can
//               backup and restore them easily. The LOCATION of
//               the config files is gotten from... the registry!

#ifndef STB_NO_STB_STRINGS
typedef struct stb_cfg_st stb_cfg;

STB_EXTERN stb_cfg* stb_cfg_open(char* config, const char* mode); // mode = "r", "w"
STB_EXTERN void      stb_cfg_close(stb_cfg * cfg);
STB_EXTERN int       stb_cfg_read(stb_cfg * cfg, char* key, void* value, int len);
STB_EXTERN void      stb_cfg_write(stb_cfg * cfg, char* key, void* value, int len);
STB_EXTERN int       stb_cfg_read_string(stb_cfg * cfg, char* key, char* value, int len);
STB_EXTERN void      stb_cfg_write_string(stb_cfg * cfg, char* key, char* value);
STB_EXTERN int       stb_cfg_delete(stb_cfg * cfg, char* key);
STB_EXTERN void      stb_cfg_set_directory(char* dir);

#ifdef STB_DEFINE

typedef struct
{
    char* key;
    void* value;
    int value_len;
} stb__cfg_item;

struct stb_cfg_st
{
    stb__cfg_item* data;
    char* loaded_file;   // this needs to be freed
    FILE* f; // write the data to this file on close
};

static const char* stb__cfg_sig = "sTbCoNfIg!\0\0";
static char stb__cfg_dir[512];
STB_EXTERN void stb_cfg_set_directory(char* dir)
{
    stb_p_strcpy_s(stb__cfg_dir, sizeof(stb__cfg_dir), dir);
}

STB_EXTERN stb_cfg* stb_cfg_open(char* config, const char* mode)
{
    size_t len;
    stb_cfg* z;
    char file[512];
    if (mode[0] != 'r' && mode[0] != 'w') return NULL;

    if (!stb__cfg_dir[0]) {
#ifdef _WIN32
        stb_p_strcpy_s(stb__cfg_dir, sizeof(stb__cfg_dir), "c:/stb");
#else
        strcpy(stb__cfg_dir, "~/.stbconfig");
#endif

#ifdef STB_HAS_REGISTRY
        {
            void* reg = stb_reg_open("rHKLM", "Software\\SilverSpaceship\\stb");
            if (reg) {
                stb_reg_read_string(reg, "config_dir", stb__cfg_dir, sizeof(stb__cfg_dir));
                stb_reg_close(reg);
            }
        }
#endif
    }

    stb_p_sprintf(file stb_p_size(sizeof(file)), "%s/%s.cfg", stb__cfg_dir, config);

    z = (stb_cfg*)stb_malloc(0, sizeof(*z));
    z->data = NULL;

    z->loaded_file = stb_filec(file, &len);
    if (z->loaded_file) {
        char* s = z->loaded_file;
        if (!memcmp(s, stb__cfg_sig, 12)) {
            char* s = z->loaded_file + 12;
            while (s < z->loaded_file + len) {
                stb__cfg_item a;
                int n = *(stb_int16*)s;
                a.key = s + 2;
                s = s + 2 + n;
                a.value_len = *(int*)s;
                s += 4;
                a.value = s;
                s += a.value_len;
                stb_arr_push(z->data, a);
            }
            assert(s == z->loaded_file + len);
        }
    }

    if (mode[0] == 'w')
        z->f = stb_p_fopen(file, "wb");
    else
        z->f = NULL;

    return z;
}

void stb_cfg_close(stb_cfg * z)
{
    if (z->f) {
        int i;
        // write the file out
        fwrite(stb__cfg_sig, 12, 1, z->f);
        for (i = 0; i < stb_arr_len(z->data); ++i) {
            stb_int16 n = (stb_int16)strlen(z->data[i].key) + 1;
            fwrite(&n, 2, 1, z->f);
            fwrite(z->data[i].key, n, 1, z->f);
            fwrite(&z->data[i].value_len, 4, 1, z->f);
            fwrite(z->data[i].value, z->data[i].value_len, 1, z->f);
        }
        fclose(z->f);
    }
    stb_arr_free(z->data);
    stb_free(z);
}

int stb_cfg_read(stb_cfg * z, char* key, void* value, int len)
{
    int i;
    for (i = 0; i < stb_arr_len(z->data); ++i) {
        if (!stb_stricmp(z->data[i].key, key)) {
            int n = stb_min(len, z->data[i].value_len);
            memcpy(value, z->data[i].value, n);
            if (n < len)
                *((char*)value + n) = 0;
            return 1;
        }
    }
    return 0;
}

void stb_cfg_write(stb_cfg * z, char* key, void* value, int len)
{
    int i;
    for (i = 0; i < stb_arr_len(z->data); ++i)
        if (!stb_stricmp(z->data[i].key, key))
            break;
    if (i == stb_arr_len(z->data)) {
        stb__cfg_item p;
        p.key = stb_strdup(key, z);
        p.value = NULL;
        p.value_len = 0;
        stb_arr_push(z->data, p);
    }
    z->data[i].value = stb_malloc(z, len);
    z->data[i].value_len = len;
    memcpy(z->data[i].value, value, len);
}

int stb_cfg_delete(stb_cfg * z, char* key)
{
    int i;
    for (i = 0; i < stb_arr_len(z->data); ++i)
        if (!stb_stricmp(z->data[i].key, key)) {
            stb_arr_fastdelete(z->data, i);
            return 1;
        }
    return 0;
}

int stb_cfg_read_string(stb_cfg * z, char* key, char* value, int len)
{
    if (!stb_cfg_read(z, key, value, len)) return 0;
    value[len - 1] = 0;
    return 1;
}

void stb_cfg_write_string(stb_cfg * z, char* key, char* value)
{
    stb_cfg_write(z, key, value, (int)strlen(value) + 1);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//     stb_dirtree  - load a description of a directory tree
//                      uses a cache and stat()s the directories for changes
//                      MUCH faster on NTFS, _wrong_ on FAT32, so should
//                      ignore the db on FAT32

#ifdef _WIN32

typedef struct
{
    char* path;           // full path from passed-in root
    time_t   last_modified;
    int      num_files;
    int      flag;
} stb_dirtree_dir;

typedef struct
{
    char* name;              // name relative to path
    int   dir;               // index into dirs[] array
    stb_int64 size;      // size, max 4GB
    time_t   last_modified;
    int      flag;
} stb_dirtree_file;

typedef struct
{
    stb_dirtree_dir* dirs;
    stb_dirtree_file* files;

    // internal use
    void* string_pool;   // used to free data en masse
} stb_dirtree;

extern void         stb_dirtree_free(stb_dirtree * d);
extern stb_dirtree* stb_dirtree_get(char* dir);
extern stb_dirtree* stb_dirtree_get_dir(char* dir, char* cache_dir);
extern stb_dirtree* stb_dirtree_get_with_file(char* dir, char* cache_file);

// get a list of all the files recursively underneath 'dir'
//
// cache_file is used to store a copy of the directory tree to speed up
// later calls. It must be unique to 'dir' and the current working
// directory! Otherwise who knows what will happen (a good solution
// is to put it _in_ dir, but this API doesn't force that).
//
// Also, it might be possible to break this if you have two different processes
// do a call to stb_dirtree_get() with the same cache file at about the same
// time, but I _think_ it might just work.

// i needed to build an identical data structure representing the state of
// a mirrored copy WITHOUT bothering to rescan it (i.e. we're mirroring to
// it WITHOUT scanning it, e.g. it's over the net), so this requires access
// to all of the innards.
extern void stb_dirtree_db_add_dir(stb_dirtree * active, char* path, time_t last);
extern void stb_dirtree_db_add_file(stb_dirtree * active, char* name, int dir, stb_int64 size, time_t last);
extern void stb_dirtree_db_read(stb_dirtree * target, char* filename, char* dir);
extern void stb_dirtree_db_write(stb_dirtree * target, char* filename, char* dir);

#ifdef STB_DEFINE
static void stb__dirtree_add_dir(char* path, time_t last, stb_dirtree * active)
{
    stb_dirtree_dir d;
    d.last_modified = last;
    d.num_files = 0;
    d.path = stb_strdup(path, active->string_pool);
    stb_arr_push(active->dirs, d);
}

static void stb__dirtree_add_file(char* name, int dir, stb_int64 size, time_t last, stb_dirtree * active)
{
    stb_dirtree_file f;
    f.dir = dir;
    f.size = size;
    f.last_modified = last;
    f.name = stb_strdup(name, active->string_pool);
    ++active->dirs[dir].num_files;
    stb_arr_push(active->files, f);
}

// version 02 supports > 4GB files
static char stb__signature[12] = { 's', 'T', 'b', 'D', 'i', 'R', 't', 'R', 'e', 'E', '0', '2' };

static void stb__dirtree_save_db(char* filename, stb_dirtree * data, char* root)
{
    int i, num_dirs_final = 0, num_files_final;
    char* info = root ? root : (char*)"";
    int* remap;
    FILE* f = stb_p_fopen(filename, "wb");
    if (!f) return;

    fwrite(stb__signature, sizeof(stb__signature), 1, f);
    fwrite(info, strlen(info) + 1, 1, f);
    // need to be slightly tricky and not write out NULLed directories, nor the root

    // build remapping table of all dirs we'll be writing out
    remap = (int*)malloc(sizeof(remap[0]) * stb_arr_len(data->dirs));
    for (i = 0; i < stb_arr_len(data->dirs); ++i) {
        if (data->dirs[i].path == NULL || (root && 0 == stb_stricmp(data->dirs[i].path, root))) {
            remap[i] = -1;
        }
        else {
            remap[i] = num_dirs_final++;
        }
    }

    fwrite(&num_dirs_final, 4, 1, f);
    for (i = 0; i < stb_arr_len(data->dirs); ++i) {
        if (remap[i] >= 0) {
            fwrite(&data->dirs[i].last_modified, 4, 1, f);
            stb_fput_string(f, data->dirs[i].path);
        }
    }

    num_files_final = 0;
    for (i = 0; i < stb_arr_len(data->files); ++i)
        if (remap[data->files[i].dir] >= 0 && data->files[i].name)
            ++num_files_final;

    fwrite(&num_files_final, 4, 1, f);
    for (i = 0; i < stb_arr_len(data->files); ++i) {
        if (remap[data->files[i].dir] >= 0 && data->files[i].name) {
            stb_fput_ranged(f, remap[data->files[i].dir], 0, num_dirs_final);
            stb_fput_varlen64(f, data->files[i].size);
            fwrite(&data->files[i].last_modified, 4, 1, f);
            stb_fput_string(f, data->files[i].name);
        }
    }

    fclose(f);
}

// note: stomps any existing data, rather than appending
static void stb__dirtree_load_db(char* filename, stb_dirtree * data, char* dir)
{
    char sig[2048];
    int i, n;
    FILE* f = stb_p_fopen(filename, "rb");

    if (!f) return;

    data->string_pool = stb_malloc(0, 1);

    fread(sig, sizeof(stb__signature), 1, f);
    if (memcmp(stb__signature, sig, sizeof(stb__signature))) { fclose(f); return; }
    if (!fread(sig, strlen(dir) + 1, 1, f)) { fclose(f); return; }
    if (stb_stricmp(sig, dir)) { fclose(f); return; }

    // we can just read them straight in, because they're guaranteed to be valid
    fread(&n, 4, 1, f);
    stb_arr_setlen(data->dirs, n);
    for (i = 0; i < stb_arr_len(data->dirs); ++i) {
        fread(&data->dirs[i].last_modified, 4, 1, f);
        data->dirs[i].path = stb_fget_string(f, data->string_pool);
        if (data->dirs[i].path == NULL) goto bail;
    }
    fread(&n, 4, 1, f);
    stb_arr_setlen(data->files, n);
    for (i = 0; i < stb_arr_len(data->files); ++i) {
        data->files[i].dir = stb_fget_ranged(f, 0, stb_arr_len(data->dirs));
        data->files[i].size = stb_fget_varlen64(f);
        fread(&data->files[i].last_modified, 4, 1, f);
        data->files[i].name = stb_fget_string(f, data->string_pool);
        if (data->files[i].name == NULL) goto bail;
    }

    if (0) {
    bail:
        stb_arr_free(data->dirs);
        stb_arr_free(data->files);
    }
    fclose(f);
}

FILE* hlog;

static int stb__dircount, stb__dircount_mask, stb__showfile;
static void stb__dirtree_scandir(char* path, time_t last_time, stb_dirtree * active)
{
    // this is dumb depth first; theoretically it might be faster
    // to fully traverse each directory before visiting its children,
    // but it's complicated and didn't seem like a gain in the test app

    int n;

    struct _wfinddatai64_t c_file;
    long hFile;
    stb__wchar full_path[1024];
    int has_slash;
    if (stb__showfile) printf("<");

    has_slash = (path[0] && path[strlen(path) - 1] == '/');

    // @TODO: do this concatenation without using swprintf to avoid this mess:
#if (defined(_MSC_VER) && _MSC_VER < 1400) // || (defined(__clang__))
   // confusingly, Windows Kits\10 needs to go down this path?!?
   // except now it doesn't, I don't know what changed
    if (has_slash)
        swprintf(full_path, L"%s*", stb__from_utf8(path));
    else
        swprintf(full_path, L"%s/*", stb__from_utf8(path));
#else
    if (has_slash)
        swprintf((wchar_t*)full_path, (size_t)1024, L"%s*", (wchar_t*)stb__from_utf8(path));
    else
        swprintf((wchar_t*)full_path, (size_t)1024, L"%s/*", (wchar_t*)stb__from_utf8(path));
#endif

    // it's possible this directory is already present: that means it was in the
    // cache, but its parent wasn't... in that case, we're done with it
    if (stb__showfile) printf("C[%d]", stb_arr_len(active->dirs));
    for (n = 0; n < stb_arr_len(active->dirs); ++n)
        if (0 == stb_stricmp(active->dirs[n].path, path)) {
            if (stb__showfile) printf("D");
            return;
        }
    if (stb__showfile) printf("E");

    // otherwise, we need to add it
    stb__dirtree_add_dir(path, last_time, active);
    n = stb_arr_lastn(active->dirs);

    if (stb__showfile) printf("[");
    if ((hFile = (long)_wfindfirsti64((wchar_t*)full_path, &c_file)) != -1L) {
        do {
            if (stb__showfile) printf(")");
            if (c_file.attrib & _A_SUBDIR) {
                // ignore subdirectories starting with '.', e.g. "." and ".."
                if (c_file.name[0] != '.') {
                    char* new_path = (char*)full_path;
                    char* temp = stb__to_utf8((stb__wchar*)c_file.name);

                    if (has_slash)
                        stb_p_sprintf(new_path stb_p_size(sizeof(full_path)), "%s%s", path, temp);
                    else
                        stb_p_sprintf(new_path stb_p_size(sizeof(full_path)), "%s/%s", path, temp);

                    if (stb__dircount_mask) {
                        ++stb__dircount;
                        if (!(stb__dircount & stb__dircount_mask)) {
                            char dummy_path[128], * pad;
                            stb_strncpy(dummy_path, new_path, sizeof(dummy_path) - 1);
                            if (strlen(dummy_path) > 96) {
                                stb_p_strcpy_s(dummy_path + 96 / 2 - 1, 128, "...");
                                stb_p_strcpy_s(dummy_path + 96 / 2 + 2, 128, new_path + strlen(new_path) - 96 / 2 + 2);
                            }
                            pad = dummy_path + strlen(dummy_path);
                            while (pad < dummy_path + 98)
                                *pad++ = ' ';
                            *pad = 0;
                            printf("%s\r", dummy_path);
#if 0
                            if (hlog == 0) {
                                hlog = stb_p_fopen("c:/x/temp.log", "w");
                                fprintf(hlog, "%s\n", dummy_path);
                            }
#endif
                        }
                    }

                    stb__dirtree_scandir(new_path, c_file.time_write, active);
                }
            }
            else {
                char* temp = stb__to_utf8((stb__wchar*)c_file.name);
                stb__dirtree_add_file(temp, n, c_file.size, c_file.time_write, active);
            }
            if (stb__showfile) printf("(");
        } while (_wfindnexti64(hFile, &c_file) == 0);
        if (stb__showfile) printf("]");
        _findclose(hFile);
    }
    if (stb__showfile) printf(">\n");
}

// scan the database and see if it's all valid
static int stb__dirtree_update_db(stb_dirtree * db, stb_dirtree * active)
{
    int changes_detected = STB_FALSE;
    int i;
    int* remap;
    int* rescan = NULL;
    remap = (int*)malloc(sizeof(remap[0]) * stb_arr_len(db->dirs));
    memset(remap, 0, sizeof(remap[0]) * stb_arr_len(db->dirs));
    rescan = NULL;

    for (i = 0; i < stb_arr_len(db->dirs); ++i) {
        struct _stat info;
        if (stb__dircount_mask) {
            ++stb__dircount;
            if (!(stb__dircount & stb__dircount_mask)) {
                printf(".");
            }
        }
        if (0 == _stat(db->dirs[i].path, &info)) {
            if (info.st_mode & _S_IFDIR) {
                // it's still a directory, as expected
                int n = abs((int)(info.st_mtime - db->dirs[i].last_modified));
                if (n > 1 && n != 3600) {  // the 3600 is a hack because sometimes this jumps for no apparent reason, even when no time zone or DST issues are at play
                   // it's changed! force a rescan
                   // we don't want to scan it until we've stat()d its
                   // subdirs, though, so we queue it
                    if (stb__showfile) printf("Changed: %s - %08x:%08x\n", db->dirs[i].path, (unsigned int)db->dirs[i].last_modified, (unsigned int)info.st_mtime);
                    stb_arr_push(rescan, i);
                    // update the last_mod time
                    db->dirs[i].last_modified = info.st_mtime;
                    // ignore existing files in this dir
                    remap[i] = -1;
                    changes_detected = STB_TRUE;
                }
                else {
                    // it hasn't changed, just copy it through unchanged
                    stb__dirtree_add_dir(db->dirs[i].path, db->dirs[i].last_modified, active);
                    remap[i] = stb_arr_lastn(active->dirs);
                }
            }
            else {
                // this path used to refer to a directory, but now it's a file!
                // assume that the parent directory is going to be forced to rescan anyway
                goto delete_entry;
            }
        }
        else {
        delete_entry:
            // directory no longer exists, so don't copy it
            // we don't free it because it's in the string pool now
            db->dirs[i].path = NULL;
            remap[i] = -1;
            changes_detected = STB_TRUE;
        }
    }

    // at this point, we have:
    //
    //   <rescan> holds a list of directory indices that need to be scanned due to being out of date
    //   <remap> holds the directory index in <active> for each dir in <db>, if it exists; -1 if not
    //           directories in <rescan> are not in <active> yet

    // so we can go ahead and remap all the known files right now
    for (i = 0; i < stb_arr_len(db->files); ++i) {
        int dir = db->files[i].dir;
        if (remap[dir] >= 0) {
            stb__dirtree_add_file(db->files[i].name, remap[dir], db->files[i].size, db->files[i].last_modified, active);
        }
    }

    // at this point we're done with db->files, and done with remap
    free(remap);

    // now scan those directories using the standard scan
    for (i = 0; i < stb_arr_len(rescan); ++i) {
        int z = rescan[i];
        stb__dirtree_scandir(db->dirs[z].path, db->dirs[z].last_modified, active);
    }
    stb_arr_free(rescan);

    return changes_detected;
}

static void stb__dirtree_free_raw(stb_dirtree * d)
{
    stb_free(d->string_pool);
    stb_arr_free(d->dirs);
    stb_arr_free(d->files);
}

stb_dirtree* stb_dirtree_get_with_file(char* dir, char* cache_file)
{
    stb_dirtree* output = (stb_dirtree*)malloc(sizeof(*output));
    stb_dirtree db, active;
    int prev_dir_count, cache_mismatch;

    char* stripped_dir; // store the directory name without a trailing '/' or '\\'

    // load the database of last-known state on disk
    db.string_pool = NULL;
    db.files = NULL;
    db.dirs = NULL;

    stripped_dir = stb_strip_final_slash(stb_p_strdup(dir));

    if (cache_file != NULL)
        stb__dirtree_load_db(cache_file, &db, stripped_dir);
    else if (stb__showfile)
        printf("No cache file\n");

    active.files = NULL;
    active.dirs = NULL;
    active.string_pool = stb_malloc(0, 1); // @TODO: share string pools between both?

    // check all the directories in the database; make note if
    // anything we scanned had changed, and rescan those things
    cache_mismatch = stb__dirtree_update_db(&db, &active);

    // check the root tree
    prev_dir_count = stb_arr_len(active.dirs);  // record how many directories we've seen

    stb__dirtree_scandir(stripped_dir, 0, &active);  // no last_modified time available for root

    if (stb__dircount_mask)
        printf("                                                                              \r");

    // done with the DB; write it back out if any changes, i.e. either
    //      1. any inconsistency found between cached information and actual disk
    //   or 2. if scanning the root found any new directories--which we detect because
    //         more than one directory got added to the active db during that scan
    if (cache_mismatch || stb_arr_len(active.dirs) > prev_dir_count + 1)
        stb__dirtree_save_db(cache_file, &active, stripped_dir);

    free(stripped_dir);

    stb__dirtree_free_raw(&db);

    *output = active;
    return output;
}

stb_dirtree* stb_dirtree_get_dir(char* dir, char* cache_dir)
{
    int i;
    stb_uint8 sha[20];
    char dir_lower[1024];
    char cache_file[1024], * s;
    if (cache_dir == NULL)
        return stb_dirtree_get_with_file(dir, NULL);
    stb_p_strcpy_s(dir_lower, sizeof(dir_lower), dir);
    stb_tolower(dir_lower);
    stb_sha1(sha, (unsigned char*)dir_lower, (unsigned int)strlen(dir_lower));
    stb_p_strcpy_s(cache_file, sizeof(cache_file), cache_dir);
    s = cache_file + strlen(cache_file);
    if (s[-1] != '/' && s[-1] != '\\') *s++ = '/';
    stb_p_strcpy_s(s, sizeof(cache_file), "dirtree_");
    s += strlen(s);
    for (i = 0; i < 8; ++i) {
        char* hex = (char*)"0123456789abcdef";
        stb_uint z = sha[i];
        *s++ = hex[z >> 4];
        *s++ = hex[z & 15];
    }
    stb_p_strcpy_s(s, sizeof(cache_file), ".bin");
    return stb_dirtree_get_with_file(dir, cache_file);
}

stb_dirtree* stb_dirtree_get(char* dir)
{
    char cache_dir[256];
    stb_p_strcpy_s(cache_dir, sizeof(cache_dir), "c:/bindata");
#ifdef STB_HAS_REGISTRY
    {
        void* reg = stb_reg_open("rHKLM", "Software\\SilverSpaceship\\stb");
        if (reg) {
            stb_reg_read(reg, "dirtree", cache_dir, sizeof(cache_dir));
            stb_reg_close(reg);
        }
    }
#endif
    return stb_dirtree_get_dir(dir, cache_dir);
}

void stb_dirtree_free(stb_dirtree * d)
{
    stb__dirtree_free_raw(d);
    free(d);
}

void stb_dirtree_db_add_dir(stb_dirtree * active, char* path, time_t last)
{
    stb__dirtree_add_dir(path, last, active);
}

void stb_dirtree_db_add_file(stb_dirtree * active, char* name, int dir, stb_int64 size, time_t last)
{
    stb__dirtree_add_file(name, dir, size, last, active);
}

void stb_dirtree_db_read(stb_dirtree * target, char* filename, char* dir)
{
    char* s = stb_strip_final_slash(stb_p_strdup(dir));
    target->dirs = 0;
    target->files = 0;
    target->string_pool = 0;
    stb__dirtree_load_db(filename, target, s);
    free(s);
}

void stb_dirtree_db_write(stb_dirtree * target, char* filename, char* dir)
{
    stb__dirtree_save_db(filename, target, 0); // don't strip out any directories
}

#endif // STB_DEFINE

#endif // _WIN32
#endif // STB_NO_STB_STRINGS

//////////////////////////////////////////////////////////////////////////////
//
//  STB_MALLOC_WRAPPER
//
//    you can use the wrapper functions with your own malloc wrapper,
//    or define STB_MALLOC_WRAPPER project-wide to have
//    malloc/free/realloc/strdup all get vectored to it

// this has too many very specific error messages you could google for and find in stb.h,
// so don't use it if they don't want any stb.h-identifiable strings
#if defined(STB_DEFINE) && !defined(STB_NO_STB_STRINGS)

typedef struct
{
    void* p;
    char* file;
    int  line;
    size_t size;
} stb_malloc_record;

#ifndef STB_MALLOC_HISTORY_COUNT
#define STB_MALLOC_HISTORY_COUNT 50 // 800 bytes
#endif

stb_malloc_record* stb__allocations;
static int stb__alloc_size, stb__alloc_limit, stb__alloc_mask;
int stb__alloc_count;

stb_malloc_record stb__alloc_history[STB_MALLOC_HISTORY_COUNT];
int stb__history_pos;

static int stb__hashfind(void* p)
{
    stb_uint32 h = stb_hashptr(p);
    int s, n = h & stb__alloc_mask;
    if (stb__allocations[n].p == p)
        return n;
    s = stb_rehash(h) | 1;
    for (;;) {
        if (stb__allocations[n].p == NULL)
            return -1;
        n = (n + s) & stb__alloc_mask;
        if (stb__allocations[n].p == p)
            return n;
    }
}

size_t stb_wrapper_allocsize(void* p)
{
    int n = stb__hashfind(p);
    if (n < 0) return 0;
    return stb__allocations[n].size;
}

static int stb__historyfind(void* p)
{
    int n = stb__history_pos;
    int i;
    for (i = 0; i < STB_MALLOC_HISTORY_COUNT; ++i) {
        if (--n < 0) n = STB_MALLOC_HISTORY_COUNT - 1;
        if (stb__alloc_history[n].p == p)
            return n;
    }
    return -1;
}

static void stb__add_alloc(void* p, size_t sz, char* file, int line);
static void stb__grow_alloc(void)
{
    int i, old_num = stb__alloc_size;
    stb_malloc_record* old = stb__allocations;
    if (stb__alloc_size == 0)
        stb__alloc_size = 64;
    else
        stb__alloc_size *= 2;

    stb__allocations = (stb_malloc_record*)stb__realloc_raw(NULL, stb__alloc_size * sizeof(stb__allocations[0]));
    if (stb__allocations == NULL)
        stb_fatal("Internal error: couldn't grow malloc wrapper table");
    memset(stb__allocations, 0, stb__alloc_size * sizeof(stb__allocations[0]));
    stb__alloc_limit = (stb__alloc_size * 3) >> 2;
    stb__alloc_mask = stb__alloc_size - 1;

    stb__alloc_count = 0;

    for (i = 0; i < old_num; ++i)
        if (old[i].p > STB_DEL) {
            stb__add_alloc(old[i].p, old[i].size, old[i].file, old[i].line);
            assert(stb__hashfind(old[i].p) >= 0);
        }
    for (i = 0; i < old_num; ++i)
        if (old[i].p > STB_DEL)
            assert(stb__hashfind(old[i].p) >= 0);
    stb__realloc_raw(old, 0);
}

static void stb__add_alloc(void* p, size_t sz, char* file, int line)
{
    stb_uint32 h;
    int n;
    if (stb__alloc_count >= stb__alloc_limit)
        stb__grow_alloc();
    h = stb_hashptr(p);
    n = h & stb__alloc_mask;
    if (stb__allocations[n].p > STB_DEL) {
        int s = stb_rehash(h) | 1;
        do {
            n = (n + s) & stb__alloc_mask;
        } while (stb__allocations[n].p > STB_DEL);
    }
    assert(stb__allocations[n].p == NULL || stb__allocations[n].p == STB_DEL);
    stb__allocations[n].p = p;
    stb__allocations[n].size = sz;
    stb__allocations[n].line = line;
    stb__allocations[n].file = file;
    ++stb__alloc_count;
}

static void stb__remove_alloc(int n, char* file, int line)
{
    stb__alloc_history[stb__history_pos] = stb__allocations[n];
    stb__alloc_history[stb__history_pos].file = file;
    stb__alloc_history[stb__history_pos].line = line;
    if (++stb__history_pos == STB_MALLOC_HISTORY_COUNT)
        stb__history_pos = 0;
    stb__allocations[n].p = STB_DEL;
    --stb__alloc_count;
}

void stb_wrapper_malloc(void* p, size_t sz, char* file, int line)
{
    if (!p) return;
    stb__add_alloc(p, sz, file, line);
}

void stb_wrapper_free(void* p, char* file, int line)
{
    int n;

    if (p == NULL) return;

    n = stb__hashfind(p);

    if (n >= 0)
        stb__remove_alloc(n, file, line);
    else {
        // tried to free something we hadn't allocated!
        n = stb__historyfind(p);
        assert(0); /* NOTREACHED */
        if (n >= 0)
            stb_fatal("Attempted to free %d-byte block %p at %s:%d previously freed/realloced at %s:%d",
                stb__alloc_history[n].size, p,
                file, line,
                stb__alloc_history[n].file, stb__alloc_history[n].line);
        else
            stb_fatal("Attempted to free unknown block %p at %s:%d", p, file, line);
    }
}

void stb_wrapper_check(void* p)
{
    int n;

    if (p == NULL) return;

    n = stb__hashfind(p);

    if (n >= 0) return;

    for (n = 0; n < stb__alloc_size; ++n)
        if (stb__allocations[n].p == p)
            stb_fatal("Internal error: pointer %p was allocated, but hash search failed", p);

    // tried to free something that wasn't allocated!
    n = stb__historyfind(p);
    if (n >= 0)
        stb_fatal("Checked %d-byte block %p previously freed/realloced at %s:%d",
            stb__alloc_history[n].size, p,
            stb__alloc_history[n].file, stb__alloc_history[n].line);
    stb_fatal("Checked unknown block %p");
}

void stb_wrapper_realloc(void* p, void* q, size_t sz, char* file, int line)
{
    int n;
    if (p == NULL) { stb_wrapper_malloc(q, sz, file, line); return; }
    if (q == NULL) return; // nothing happened

    n = stb__hashfind(p);
    if (n == -1) {
        // tried to free something we hadn't allocated!
        // this is weird, though, because we got past the realloc!
        n = stb__historyfind(p);
        assert(0); /* NOTREACHED */
        if (n >= 0)
            stb_fatal("Attempted to realloc %d-byte block %p at %s:%d previously freed/realloced at %s:%d",
                stb__alloc_history[n].size, p,
                file, line,
                stb__alloc_history[n].file, stb__alloc_history[n].line);
        else
            stb_fatal("Attempted to realloc unknown block %p at %s:%d", p, file, line);
    }
    else {
        if (q == p) {
            stb__allocations[n].size = sz;
            stb__allocations[n].file = file;
            stb__allocations[n].line = line;
        }
        else {
            stb__remove_alloc(n, file, line);
            stb__add_alloc(q, sz, file, line);
        }
    }
}

void stb_wrapper_listall(void (*func)(void* ptr, size_t sz, char* file, int line))
{
    int i;
    for (i = 0; i < stb__alloc_size; ++i)
        if (stb__allocations[i].p > STB_DEL)
            func(stb__allocations[i].p, stb__allocations[i].size,
                stb__allocations[i].file, stb__allocations[i].line);
}

void stb_wrapper_dump(char* filename)
{
    int i;
    FILE* f = stb_p_fopen(filename, "w");
    if (!f) return;
    for (i = 0; i < stb__alloc_size; ++i)
        if (stb__allocations[i].p > STB_DEL)
            fprintf(f, "%p %7d - %4d %s\n",
                stb__allocations[i].p, (int)stb__allocations[i].size,
                stb__allocations[i].line, stb__allocations[i].file);
}
#endif // STB_DEFINE


//////////////////////////////////////////////////////////////////////////////
//
//                  stb_pointer_set
//
//
// For data structures that support querying by key, data structure
// classes always hand-wave away the issue of what to do if two entries
// have the same key: basically, store a linked list of all the nodes
// which have the same key (a LISP-style list).
//
// The thing is, it's not that trivial. If you have an O(log n)
// lookup data structure, but then n/4 items have the same value,
// you don't want to spend O(n) time scanning that list when
// deleting an item if you already have a pointer to the item.
// (You have to spend O(n) time enumerating all the items with
// a given key, sure, and you can't accelerate deleting a particular
// item if you only have the key, not a pointer to the item.)
//
// I'm going to call this data structure, whatever it turns out to
// be, a "pointer set", because we don't store any associated data for
// items in this data structure, we just answer the question of
// whether an item is in it or not (it's effectively one bit per pointer).
// Technically they don't have to be pointers; you could cast ints
// to (void *) if you want, but you can't store 0 or 1 because of the
// hash table.
//
// Since the fastest data structure we might want to add support for
// identical-keys to is a hash table with O(1)-ish lookup time,
// that means that the conceptual "linked list of all items with
// the same indexed value" that we build needs to have the same
// performance; that way when we index a table we think is arbitrary
// ints, but in fact half of them are 0, we don't get screwed.
//
// Therefore, it needs to be a hash table, at least when it gets
// large. On the other hand, when the data has totally arbitrary ints
// or floats, there won't be many collisions, and we'll have tons of
// 1-item bitmaps. That will be grossly inefficient as hash tables;
// trade-off; the hash table is reasonably efficient per-item when
// it's large, but not when it's small. So we need to do something
// Judy-like and use different strategies depending on the size.
//
// Like Judy, we'll use the bottom bit to encode the strategy:
//
//      bottom bits:
//          00     -   direct pointer
//          01     -   4-item bucket (16 bytes, no length, NULLs)
//          10     -   N-item array
//          11     -   hash table

typedef struct stb_ps stb_ps;

STB_EXTERN int      stb_ps_find(stb_ps * ps, void* value);
STB_EXTERN stb_ps* stb_ps_add(stb_ps * ps, void* value);
STB_EXTERN stb_ps* stb_ps_remove(stb_ps * ps, void* value);
STB_EXTERN stb_ps* stb_ps_remove_any(stb_ps * ps, void** value);
STB_EXTERN void     stb_ps_delete(stb_ps * ps);
STB_EXTERN int      stb_ps_count(stb_ps * ps);

STB_EXTERN stb_ps* stb_ps_copy(stb_ps * ps);
STB_EXTERN int      stb_ps_subset(stb_ps * bigger, stb_ps * smaller);
STB_EXTERN int      stb_ps_eq(stb_ps * p0, stb_ps * p1);

STB_EXTERN void** stb_ps_getlist(stb_ps * ps, int* count);
STB_EXTERN int     stb_ps_writelist(stb_ps * ps, void** list, int size);

// enum and fastlist don't allocate storage, but you must consume the
// list before there's any chance the data structure gets screwed up;
STB_EXTERN int     stb_ps_enum(stb_ps * ps, void* data,
    int (*func)(void* value, void* data));
STB_EXTERN void** stb_ps_fastlist(stb_ps * ps, int* count);
//  result:
//     returns a list, *count is the length of that list,
//     but some entries of the list may be invalid;
//     test with 'stb_ps_fastlist_valid(x)'

#define stb_ps_fastlist_valid(x)   ((stb_uinta) (x) > 1)

#ifdef STB_DEFINE

enum
{
    STB_ps_direct = 0,
    STB_ps_bucket = 1,
    STB_ps_array = 2,
    STB_ps_hash = 3,
};

#define STB_BUCKET_SIZE  4

typedef struct
{
    void* p[STB_BUCKET_SIZE];
} stb_ps_bucket;
#define GetBucket(p)    ((stb_ps_bucket *) ((char *) (p) - STB_ps_bucket))
#define EncodeBucket(p) ((stb_ps *) ((char *) (p) + STB_ps_bucket))

static void stb_bucket_free(stb_ps_bucket * b)
{
    free(b);
}

static stb_ps_bucket* stb_bucket_create2(void* v0, void* v1)
{
    stb_ps_bucket* b = (stb_ps_bucket*)malloc(sizeof(*b));
    b->p[0] = v0;
    b->p[1] = v1;
    b->p[2] = NULL;
    b->p[3] = NULL;
    return b;
}

static stb_ps_bucket* stb_bucket_create3(void** v)
{
    stb_ps_bucket* b = (stb_ps_bucket*)malloc(sizeof(*b));
    b->p[0] = v[0];
    b->p[1] = v[1];
    b->p[2] = v[2];
    b->p[3] = NULL;
    return b;
}


// could use stb_arr, but this will save us memory
typedef struct
{
    int count;
    void* p[1];
} stb_ps_array;
#define GetArray(p)     ((stb_ps_array *) ((char *) (p) - STB_ps_array))
#define EncodeArray(p)  ((stb_ps *) ((char *) (p) + STB_ps_array))

static int stb_ps_array_max = 13;

typedef struct
{
    int size, mask;
    int count, count_deletes;
    int grow_threshhold;
    int shrink_threshhold;
    int rehash_threshhold;
    int any_offset;
    void* table[1];
} stb_ps_hash;
#define GetHash(p)      ((stb_ps_hash *) ((char *) (p) - STB_ps_hash))
#define EncodeHash(p)   ((stb_ps *) ((char *) (p) + STB_ps_hash))

#define stb_ps_empty(v)   (((stb_uint32) v) <= 1)

static stb_ps_hash* stb_ps_makehash(int size, int old_size, void** old_data)
{
    int i;
    stb_ps_hash* h = (stb_ps_hash*)malloc(sizeof(*h) + (size - 1) * sizeof(h->table[0]));
    assert(stb_is_pow2(size));
    h->size = size;
    h->mask = size - 1;
    h->shrink_threshhold = (int)(0.3f * size);
    h->grow_threshhold = (int)(0.8f * size);
    h->rehash_threshhold = (int)(0.9f * size);
    h->count = 0;
    h->count_deletes = 0;
    h->any_offset = 0;
    memset(h->table, 0, size * sizeof(h->table[0]));
    for (i = 0; i < old_size; ++i)
        if (!stb_ps_empty((size_t)old_data[i]))
            stb_ps_add(EncodeHash(h), old_data[i]);
    return h;
}

void stb_ps_delete(stb_ps * ps)
{
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct: break;
    case STB_ps_bucket: stb_bucket_free(GetBucket(ps)); break;
    case STB_ps_array: free(GetArray(ps)); break;
    case STB_ps_hash: free(GetHash(ps)); break;
    }
}

stb_ps* stb_ps_copy(stb_ps * ps)
{
    int i;
    // not a switch: order based on expected performance/power-law distribution
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct: return ps;
    case STB_ps_bucket: {
        stb_ps_bucket* n = (stb_ps_bucket*)malloc(sizeof(*n));
        *n = *GetBucket(ps);
        return EncodeBucket(n);
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        stb_ps_array* n = (stb_ps_array*)malloc(sizeof(*n) + stb_ps_array_max * sizeof(n->p[0]));
        n->count = a->count;
        for (i = 0; i < a->count; ++i)
            n->p[i] = a->p[i];
        return EncodeArray(n);
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        stb_ps_hash* n = stb_ps_makehash(h->size, h->size, h->table);
        return EncodeHash(n);
    }
    }
    assert(0); /* NOTREACHED */
    return NULL;
}

int stb_ps_find(stb_ps * ps, void* value)
{
    int i, code = 3 & (int)(size_t)ps;
    assert((3 & (int)(size_t)value) == STB_ps_direct);
    assert(stb_ps_fastlist_valid(value));
    // not a switch: order based on expected performance/power-law distribution
    if (code == STB_ps_direct)
        return value == ps;
    if (code == STB_ps_bucket) {
        stb_ps_bucket* b = GetBucket(ps);
        assert(STB_BUCKET_SIZE == 4);
        if (b->p[0] == value || b->p[1] == value ||
            b->p[2] == value || b->p[3] == value)
            return STB_TRUE;
        return STB_FALSE;
    }
    if (code == STB_ps_array) {
        stb_ps_array* a = GetArray(ps);
        for (i = 0; i < a->count; ++i)
            if (a->p[i] == value)
                return STB_TRUE;
        return STB_FALSE;
    }
    else {
        stb_ps_hash* h = GetHash(ps);
        stb_uint32 hash = stb_hashptr(value);
        stb_uint32 s, n = hash & h->mask;
        void** t = h->table;
        if (t[n] == value) return STB_TRUE;
        if (t[n] == NULL) return STB_FALSE;
        s = stb_rehash(hash) | 1;
        do {
            n = (n + s) & h->mask;
            if (t[n] == value) return STB_TRUE;
        } while (t[n] != NULL);
        return STB_FALSE;
    }
}

stb_ps* stb_ps_add(stb_ps * ps, void* value)
{
#ifdef STB_DEBUG
    assert(!stb_ps_find(ps, value));
#endif
    if (value == NULL) return ps; // ignore NULL adds to avoid bad breakage
    assert((3 & (int)(size_t)value) == STB_ps_direct);
    assert(stb_ps_fastlist_valid(value));
    assert(value != STB_DEL);     // STB_DEL is less likely

    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        if (ps == NULL) return (stb_ps*)value;
        return EncodeBucket(stb_bucket_create2(ps, value));

    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        stb_ps_array* a;
        assert(STB_BUCKET_SIZE == 4);
        if (b->p[0] == NULL) { b->p[0] = value; return ps; }
        if (b->p[1] == NULL) { b->p[1] = value; return ps; }
        if (b->p[2] == NULL) { b->p[2] = value; return ps; }
        if (b->p[3] == NULL) { b->p[3] = value; return ps; }
        a = (stb_ps_array*)malloc(sizeof(*a) + 7 * sizeof(a->p[0])); // 8 slots, must be 2^k
        memcpy(a->p, b, sizeof(*b));
        a->p[4] = value;
        a->count = 5;
        stb_bucket_free(b);
        return EncodeArray(a);
    }

    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        if (a->count == stb_ps_array_max) {
            // promote from array to hash
            stb_ps_hash* h = stb_ps_makehash(2 << stb_log2_ceil(a->count), a->count, a->p);
            free(a);
            return stb_ps_add(EncodeHash(h), value);
        }
        // do we need to resize the array? the array doubles in size when it
        // crosses a power-of-two
        if ((a->count & (a->count - 1)) == 0) {
            int newsize = a->count * 2;
            // clamp newsize to max if:
            //    1. it's larger than max
            //    2. newsize*1.5 is larger than max (to avoid extra resizing)
            if (newsize + a->count > stb_ps_array_max)
                newsize = stb_ps_array_max;
            a = (stb_ps_array*)realloc(a, sizeof(*a) + (newsize - 1) * sizeof(a->p[0]));
        }
        a->p[a->count++] = value;
        return EncodeArray(a);
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        stb_uint32 hash = stb_hashptr(value);
        stb_uint32 n = hash & h->mask;
        void** t = h->table;
        // find first NULL or STB_DEL entry
        if (!stb_ps_empty((size_t)t[n])) {
            stb_uint32 s = stb_rehash(hash) | 1;
            do {
                n = (n + s) & h->mask;
            } while (!stb_ps_empty((size_t)t[n]));
        }
        if (t[n] == STB_DEL)
            --h->count_deletes;
        t[n] = value;
        ++h->count;
        if (h->count == h->grow_threshhold) {
            stb_ps_hash* h2 = stb_ps_makehash(h->size * 2, h->size, t);
            free(h);
            return EncodeHash(h2);
        }
        if (h->count + h->count_deletes == h->rehash_threshhold) {
            stb_ps_hash* h2 = stb_ps_makehash(h->size, h->size, t);
            free(h);
            return EncodeHash(h2);
        }
        return ps;
    }
    }
    return NULL; /* NOTREACHED */
}

stb_ps* stb_ps_remove(stb_ps * ps, void* value)
{
#ifdef STB_DEBUG
    assert(stb_ps_find(ps, value));
#endif
    assert((3 & (int)(size_t)value) == STB_ps_direct);
    if (value == NULL) return ps; // ignore NULL removes to avoid bad breakage
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        return ps == value ? NULL : ps;
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        int count = 0;
        assert(STB_BUCKET_SIZE == 4);
        if (b->p[0] == value) b->p[0] = NULL; else count += (b->p[0] != NULL);
        if (b->p[1] == value) b->p[1] = NULL; else count += (b->p[1] != NULL);
        if (b->p[2] == value) b->p[2] = NULL; else count += (b->p[2] != NULL);
        if (b->p[3] == value) b->p[3] = NULL; else count += (b->p[3] != NULL);
        if (count == 1) { // shrink bucket at size 1
            value = b->p[0];
            if (value == NULL) value = b->p[1];
            if (value == NULL) value = b->p[2];
            if (value == NULL) value = b->p[3];
            assert(value != NULL);
            stb_bucket_free(b);
            return (stb_ps*)value; // return STB_ps_direct of value
        }
        return ps;
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        int i;
        for (i = 0; i < a->count; ++i) {
            if (a->p[i] == value) {
                a->p[i] = a->p[--a->count];
                if (a->count == 3) { // shrink to bucket!
                    stb_ps_bucket* b = stb_bucket_create3(a->p);
                    free(a);
                    return EncodeBucket(b);
                }
                return ps;
            }
        }
        return ps;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        stb_uint32 hash = stb_hashptr(value);
        stb_uint32 s, n = hash & h->mask;
        void** t = h->table;
        if (t[n] != value) {
            s = stb_rehash(hash) | 1;
            do {
                n = (n + s) & h->mask;
            } while (t[n] != value);
        }
        t[n] = STB_DEL;
        --h->count;
        ++h->count_deletes;
        // should we shrink down to an array?
        if (h->count < stb_ps_array_max) {
            int n = 1 << stb_log2_floor(stb_ps_array_max);
            if (h->count < n) {
                stb_ps_array* a = (stb_ps_array*)malloc(sizeof(*a) + (n - 1) * sizeof(a->p[0]));
                int i, j = 0;
                for (i = 0; i < h->size; ++i)
                    if (!stb_ps_empty((size_t)t[i]))
                        a->p[j++] = t[i];
                assert(j == h->count);
                a->count = j;
                free(h);
                return EncodeArray(a);
            }
        }
        if (h->count == h->shrink_threshhold) {
            stb_ps_hash* h2 = stb_ps_makehash(h->size >> 1, h->size, t);
            free(h);
            return EncodeHash(h2);
        }
        return ps;
    }
    }
    return ps; /* NOTREACHED */
}

stb_ps* stb_ps_remove_any(stb_ps * ps, void** value)
{
    assert(ps != NULL);
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        *value = ps;
        return NULL;
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        int count = 0, slast = 0, last = 0;
        assert(STB_BUCKET_SIZE == 4);
        if (b->p[0]) { ++count;               last = 0; }
        if (b->p[1]) { ++count; slast = last; last = 1; }
        if (b->p[2]) { ++count; slast = last; last = 2; }
        if (b->p[3]) { ++count; slast = last; last = 3; }
        *value = b->p[last];
        b->p[last] = 0;
        if (count == 2) {
            void* leftover = b->p[slast]; // second to last
            stb_bucket_free(b);
            return (stb_ps*)leftover;
        }
        return ps;
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        *value = a->p[a->count - 1];
        if (a->count == 4)
            return stb_ps_remove(ps, *value);
        --a->count;
        return ps;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        void** t = h->table;
        stb_uint32 n = h->any_offset;
        while (stb_ps_empty((size_t)t[n]))
            n = (n + 1) & h->mask;
        *value = t[n];
        h->any_offset = (n + 1) & h->mask;
        // check if we need to skip down to the previous type
        if (h->count - 1 < stb_ps_array_max || h->count - 1 == h->shrink_threshhold)
            return stb_ps_remove(ps, *value);
        t[n] = STB_DEL;
        --h->count;
        ++h->count_deletes;
        return ps;
    }
    }
    return ps; /* NOTREACHED */
}


void** stb_ps_getlist(stb_ps * ps, int* count)
{
    int i, n = 0;
    void** p = NULL;
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        if (ps == NULL) { *count = 0; return NULL; }
        p = (void**)malloc(sizeof(*p) * 1);
        p[0] = ps;
        *count = 1;
        return p;
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        p = (void**)malloc(sizeof(*p) * STB_BUCKET_SIZE);
        for (i = 0; i < STB_BUCKET_SIZE; ++i)
            if (b->p[i] != NULL)
                p[n++] = b->p[i];
        break;
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        p = (void**)malloc(sizeof(*p) * a->count);
        memcpy(p, a->p, sizeof(*p) * a->count);
        *count = a->count;
        return p;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        p = (void**)malloc(sizeof(*p) * h->count);
        for (i = 0; i < h->size; ++i)
            if (!stb_ps_empty((size_t)h->table[i]))
                p[n++] = h->table[i];
        break;
    }
    }
    *count = n;
    return p;
}

int stb_ps_writelist(stb_ps * ps, void** list, int size)
{
    int i, n = 0;
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        if (ps == NULL || size <= 0) return 0;
        list[0] = ps;
        return 1;
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        for (i = 0; i < STB_BUCKET_SIZE; ++i)
            if (b->p[i] != NULL && n < size)
                list[n++] = b->p[i];
        return n;
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        n = stb_min(size, a->count);
        memcpy(list, a->p, sizeof(*list) * n);
        return n;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        if (size <= 0) return 0;
        for (i = 0; i < h->count; ++i) {
            if (!stb_ps_empty((size_t)h->table[i])) {
                list[n++] = h->table[i];
                if (n == size) break;
            }
        }
        return n;
    }
    }
    return 0; /* NOTREACHED */
}

int stb_ps_enum(stb_ps * ps, void* data, int (*func)(void* value, void* data))
{
    int i;
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        if (ps == NULL) return STB_TRUE;
        return func(ps, data);
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        for (i = 0; i < STB_BUCKET_SIZE; ++i)
            if (b->p[i] != NULL)
                if (!func(b->p[i], data))
                    return STB_FALSE;
        return STB_TRUE;
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        for (i = 0; i < a->count; ++i)
            if (!func(a->p[i], data))
                return STB_FALSE;
        return STB_TRUE;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        for (i = 0; i < h->count; ++i)
            if (!stb_ps_empty((size_t)h->table[i]))
                if (!func(h->table[i], data))
                    return STB_FALSE;
        return STB_TRUE;
    }
    }
    return STB_TRUE; /* NOTREACHED */
}

int stb_ps_count(stb_ps * ps)
{
    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        return ps != NULL;
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        return (b->p[0] != NULL) + (b->p[1] != NULL) +
            (b->p[2] != NULL) + (b->p[3] != NULL);
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        return a->count;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        return h->count;
    }
    }
    return 0;
}

void** stb_ps_fastlist(stb_ps * ps, int* count)
{
    static void* storage;

    switch (3 & (int)(size_t)ps) {
    case STB_ps_direct:
        if (ps == NULL) { *count = 0; return NULL; }
        storage = ps;
        *count = 1;
        return &storage;
    case STB_ps_bucket: {
        stb_ps_bucket* b = GetBucket(ps);
        *count = STB_BUCKET_SIZE;
        return b->p;
    }
    case STB_ps_array: {
        stb_ps_array* a = GetArray(ps);
        *count = a->count;
        return a->p;
    }
    case STB_ps_hash: {
        stb_ps_hash* h = GetHash(ps);
        *count = h->size;
        return h->table;
    }
    }
    return NULL; /* NOTREACHED */
}

int stb_ps_subset(stb_ps * bigger, stb_ps * smaller)
{
    int i, listlen;
    void** list = stb_ps_fastlist(smaller, &listlen);
    for (i = 0; i < listlen; ++i)
        if (stb_ps_fastlist_valid(list[i]))
            if (!stb_ps_find(bigger, list[i]))
                return 0;
    return 1;
}

int stb_ps_eq(stb_ps * p0, stb_ps * p1)
{
    if (stb_ps_count(p0) != stb_ps_count(p1))
        return 0;
    return stb_ps_subset(p0, p1);
}

#undef GetBucket
#undef GetArray
#undef GetHash

#undef EncodeBucket
#undef EncodeArray
#undef EncodeHash

#endif


//////////////////////////////////////////////////////////////////////////////
//
//               Random Numbers via Meresenne Twister or LCG
//

STB_EXTERN unsigned int  stb_srandLCG(unsigned int seed);
STB_EXTERN unsigned int  stb_randLCG(void);
STB_EXTERN double        stb_frandLCG(void);

STB_EXTERN void          stb_srand(unsigned int seed);
STB_EXTERN unsigned int  stb_rand(void);
STB_EXTERN double        stb_frand(void);
STB_EXTERN void          stb_shuffle(void* p, size_t n, size_t sz,
    unsigned int seed);
STB_EXTERN void stb_reverse(void* p, size_t n, size_t sz);

STB_EXTERN unsigned int  stb_randLCG_explicit(unsigned int  seed);

#define stb_rand_define(x,y)                                         \
                                                                     \
   unsigned int  x(void)                                             \
   {                                                                 \
      static unsigned int  stb__rand = y;                            \
      stb__rand = stb__rand * 2147001325 + 715136305; /* BCPL */     \
      return 0x31415926 ^ ((stb__rand >> 16) + (stb__rand << 16));   \
   }

#ifdef STB_DEFINE
unsigned int  stb_randLCG_explicit(unsigned int seed)
{
    return seed * 2147001325 + 715136305;
}

static unsigned int  stb__rand_seed = 0;

unsigned int  stb_srandLCG(unsigned int seed)
{
    unsigned int  previous = stb__rand_seed;
    stb__rand_seed = seed;
    return previous;
}

unsigned int  stb_randLCG(void)
{
    stb__rand_seed = stb__rand_seed * 2147001325 + 715136305; // BCPL generator
    // shuffle non-random bits to the middle, and xor to decorrelate with seed
    return 0x31415926 ^ ((stb__rand_seed >> 16) + (stb__rand_seed << 16));
}

double stb_frandLCG(void)
{
    return stb_randLCG() / ((double)(1 << 16) * (1 << 16));
}

void stb_shuffle(void* p, size_t n, size_t sz, unsigned int seed)
{
    char* a;
    unsigned int old_seed;
    int i;
    if (seed)
        old_seed = stb_srandLCG(seed);
    a = (char*)p + (n - 1) * sz;

    for (i = (int)n; i > 1; --i) {
        int j = stb_randLCG() % i;
        stb_swap(a, (char*)p + j * sz, sz);
        a -= sz;
    }
    if (seed)
        stb_srandLCG(old_seed);
}

void stb_reverse(void* p, size_t n, size_t sz)
{
    size_t i, j = n - 1;
    for (i = 0; i < j; ++i, --j) {
        stb_swap((char*)p + i * sz, (char*)p + j * sz, sz);
    }
}

// public domain Mersenne Twister by Michael Brundage
#define STB__MT_LEN       624

int stb__mt_index = STB__MT_LEN * sizeof(int) + 1;
unsigned int stb__mt_buffer[STB__MT_LEN];

void stb_srand(unsigned int seed)
{
    int i;
    stb__mt_buffer[0] = seed & 0xffffffffUL;
    for (i = 1; i < STB__MT_LEN; ++i)
        stb__mt_buffer[i] = (1812433253UL * (stb__mt_buffer[i - 1] ^ (stb__mt_buffer[i - 1] >> 30)) + i);
    stb__mt_index = STB__MT_LEN * sizeof(unsigned int);
}

#define STB__MT_IA           397
#define STB__MT_IB           (STB__MT_LEN - STB__MT_IA)
#define STB__UPPER_MASK      0x80000000
#define STB__LOWER_MASK      0x7FFFFFFF
#define STB__MATRIX_A        0x9908B0DF
#define STB__TWIST(b,i,j)    ((b)[i] & STB__UPPER_MASK) | ((b)[j] & STB__LOWER_MASK)
#define STB__MAGIC(s)        (((s)&1)*STB__MATRIX_A)

unsigned int stb_rand()
{
    unsigned int* b = stb__mt_buffer;
    int idx = stb__mt_index;
    unsigned int  s, r;
    int i;

    if (idx >= STB__MT_LEN * sizeof(unsigned int)) {
        if (idx > STB__MT_LEN * sizeof(unsigned int))
            stb_srand(0);
        idx = 0;
        i = 0;
        for (; i < STB__MT_IB; i++) {
            s = STB__TWIST(b, i, i + 1);
            b[i] = b[i + STB__MT_IA] ^ (s >> 1) ^ STB__MAGIC(s);
        }
        for (; i < STB__MT_LEN - 1; i++) {
            s = STB__TWIST(b, i, i + 1);
            b[i] = b[i - STB__MT_IB] ^ (s >> 1) ^ STB__MAGIC(s);
        }

        s = STB__TWIST(b, STB__MT_LEN - 1, 0);
        b[STB__MT_LEN - 1] = b[STB__MT_IA - 1] ^ (s >> 1) ^ STB__MAGIC(s);
    }
    stb__mt_index = idx + sizeof(unsigned int);

    r = *(unsigned int*)((unsigned char*)b + idx);

    r ^= (r >> 11);
    r ^= (r << 7) & 0x9D2C5680;
    r ^= (r << 15) & 0xEFC60000;
    r ^= (r >> 18);

    return r;
}

double stb_frand(void)
{
    return stb_rand() / ((double)(1 << 16) * (1 << 16));
}

#endif


//////////////////////////////////////////////////////////////////////////////
//
//                        stb_dupe
//
// stb_dupe is a duplicate-finding system for very, very large data
// structures--large enough that sorting is too slow, but not so large
// that we can't keep all the data in memory. using it works as follows:
//
//     1. create an stb_dupe:
//          provide a hash function
//          provide an equality function
//          provide an estimate for the size
//          optionally provide a comparison function
//
//     2. traverse your data, 'adding' pointers to the stb_dupe
//
//     3. finish and ask for duplicates
//
//        the stb_dupe will discard its intermediate data and build
//        a collection of sorted lists of duplicates, with non-duplicate
//        entries omitted entirely
//
//
//  Implementation strategy:
//
//     while collecting the N items, we keep a hash table of approximate
//     size sqrt(N). (if you tell use the N up front, the hash table is
//     just that size exactly)
//
//     each entry in the hash table is just an stb__arr of pointers (no need
//     to use stb_ps, because we don't need to delete from these)
//
//     for step 3, for each entry in the hash table, we apply stb_dupe to it
//     recursively. once the size gets small enough (or doesn't decrease
//     significantly), we switch to either using qsort() on the comparison
//     function, or else we just do the icky N^2 gather


typedef struct stb_dupe stb_dupe;

typedef int (*stb_compare_func)(void* a, void* b);
typedef int (*stb_hash_func)(void* a, unsigned int seed);

STB_EXTERN void stb_dupe_free(stb_dupe * sd);
STB_EXTERN stb_dupe* stb_dupe_create(stb_hash_func hash,
    stb_compare_func eq, int size, stb_compare_func ineq);
STB_EXTERN void stb_dupe_add(stb_dupe * sd, void* item);
STB_EXTERN void stb_dupe_finish(stb_dupe * sd);
STB_EXTERN int stb_dupe_numsets(stb_dupe * sd);
STB_EXTERN void** stb_dupe_set(stb_dupe * sd, int num);
STB_EXTERN int stb_dupe_set_count(stb_dupe * sd, int num);

struct stb_dupe
{
    void*** hash_table;
    int hash_size;
    int size_log2;
    int population;

    int hash_shift;
    stb_hash_func     hash;

    stb_compare_func  eq;
    stb_compare_func  ineq;

    void*** dupes;
};

#ifdef STB_DEFINE

int stb_dupe_numsets(stb_dupe * sd)
{
    assert(sd->hash_table == NULL);
    return stb_arr_len(sd->dupes);
}

void** stb_dupe_set(stb_dupe * sd, int num)
{
    assert(sd->hash_table == NULL);
    return sd->dupes[num];
}

int stb_dupe_set_count(stb_dupe * sd, int num)
{
    assert(sd->hash_table == NULL);
    return stb_arr_len(sd->dupes[num]);
}

stb_dupe* stb_dupe_create(stb_hash_func hash, stb_compare_func eq, int size,
    stb_compare_func ineq)
{
    int i, hsize;
    stb_dupe* sd = (stb_dupe*)malloc(sizeof(*sd));

    sd->size_log2 = 4;
    hsize = 1 << sd->size_log2;
    while (hsize * hsize < size) {
        ++sd->size_log2;
        hsize *= 2;
    }

    sd->hash = hash;
    sd->eq = eq;
    sd->ineq = ineq;
    sd->hash_shift = 0;

    sd->population = 0;
    sd->hash_size = hsize;
    sd->hash_table = (void***)malloc(sizeof(*sd->hash_table) * hsize);
    for (i = 0; i < hsize; ++i)
        sd->hash_table[i] = NULL;

    sd->dupes = NULL;

    return sd;
}

void stb_dupe_add(stb_dupe * sd, void* item)
{
    stb_uint32 hash = sd->hash(item, sd->hash_shift);
    int z = hash & (sd->hash_size - 1);
    stb_arr_push(sd->hash_table[z], item);
    ++sd->population;
}

void stb_dupe_free(stb_dupe * sd)
{
    int i;
    for (i = 0; i < stb_arr_len(sd->dupes); ++i)
        if (sd->dupes[i])
            stb_arr_free(sd->dupes[i]);
    stb_arr_free(sd->dupes);
    free(sd);
}

static stb_compare_func stb__compare;

static int stb__dupe_compare(const void* a, const void* b)
{
    void* p = *(void**)a;
    void* q = *(void**)b;

    return stb__compare(p, q);
}

void stb_dupe_finish(stb_dupe * sd)
{
    int i, j, k;
    assert(sd->dupes == NULL);
    for (i = 0; i < sd->hash_size; ++i) {
        void** list = sd->hash_table[i];
        if (list != NULL) {
            int n = stb_arr_len(list);
            // @TODO: measure to find good numbers instead of just making them up!
            int thresh = (sd->ineq ? 200 : 20);
            // if n is large enough to be worth it, and n is smaller than
            // before (so we can guarantee we'll use a smaller hash table);
            // and there are enough hash bits left, assuming full 32-bit hash
            if (n > thresh && n < (sd->population >> 3) && sd->hash_shift + sd->size_log2 * 2 < 32) {

                // recursively process this row using stb_dupe, O(N log log N)

                stb_dupe* d = stb_dupe_create(sd->hash, sd->eq, n, sd->ineq);
                d->hash_shift = stb_randLCG_explicit(sd->hash_shift);
                for (j = 0; j < n; ++j)
                    stb_dupe_add(d, list[j]);
                stb_arr_free(sd->hash_table[i]);
                stb_dupe_finish(d);
                for (j = 0; j < stb_arr_len(d->dupes); ++j) {
                    stb_arr_push(sd->dupes, d->dupes[j]);
                    d->dupes[j] = NULL; // take over ownership
                }
                stb_dupe_free(d);

            }
            else if (sd->ineq) {

                // process this row using qsort(), O(N log N)
                stb__compare = sd->ineq;
                qsort(list, n, sizeof(list[0]), stb__dupe_compare);

                // find equal subsequences of the list
                for (j = 0; j < n - 1; ) {
                    // find a subsequence from j..k
                    for (k = j; k < n; ++k)
                        // only use ineq so eq can be left undefined
                        if (sd->ineq(list[j], list[k]))
                            break;
                    // k is the first one not in the subsequence
                    if (k - j > 1) {
                        void** mylist = NULL;
                        stb_arr_setlen(mylist, k - j);
                        memcpy(mylist, list + j, sizeof(list[j]) * (k - j));
                        stb_arr_push(sd->dupes, mylist);
                    }
                    j = k;
                }
                stb_arr_free(sd->hash_table[i]);
            }
            else {

                // process this row using eq(), O(N^2)
                for (j = 0; j < n; ++j) {
                    if (list[j] != NULL) {
                        void** output = NULL;
                        for (k = j + 1; k < n; ++k) {
                            if (sd->eq(list[j], list[k])) {
                                if (output == NULL)
                                    stb_arr_push(output, list[j]);
                                stb_arr_push(output, list[k]);
                                list[k] = NULL;
                            }
                        }
                        list[j] = NULL;
                        if (output)
                            stb_arr_push(sd->dupes, output);
                    }
                }
                stb_arr_free(sd->hash_table[i]);
            }
        }
    }
    free(sd->hash_table);
    sd->hash_table = NULL;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                       templatized Sort routine
//
// This is an attempt to implement a templated sorting algorithm.
// To use it, you have to explicitly instantiate it as a _function_,
// then you call that function. This allows the comparison to be inlined,
// giving the sort similar performance to C++ sorts.
//
// It implements quicksort with three-way-median partitioning (generally
// well-behaved), with a final insertion sort pass.
//
// When you define the compare expression, you should assume you have
// elements of your array pointed to by 'a' and 'b', and perform the comparison
// on those. OR you can use one or more statements; first say '0;', then
// write whatever code you want, and compute the result into a variable 'c'.

#define stb_declare_sort(FUNCNAME, TYPE)    \
                       void FUNCNAME(TYPE *p, int n)
#define stb_define_sort(FUNCNAME,TYPE,COMPARE) \
                       stb__define_sort(       void, FUNCNAME,TYPE,COMPARE)
#define stb_define_sort_static(FUNCNAME,TYPE,COMPARE) \
                       stb__define_sort(static void, FUNCNAME,TYPE,COMPARE)

#define stb__define_sort(MODE, FUNCNAME, TYPE, COMPARE)                       \
                                                                              \
static void STB_(FUNCNAME,_ins_sort)(TYPE *p, int n)                          \
{                                                                             \
   int i,j;                                                                   \
   for (i=1; i < n; ++i) {                                                    \
      TYPE t = p[i], *a = &t;                                                 \
      j = i;                                                                  \
      while (j > 0) {                                                         \
         TYPE *b = &p[j-1];                                                   \
         int c = COMPARE;                                                     \
         if (!c) break;                                                       \
         p[j] = p[j-1];                                                       \
         --j;                                                                 \
      }                                                                       \
      if (i != j)                                                             \
         p[j] = t;                                                            \
   }                                                                          \
}                                                                             \
                                                                              \
static void STB_(FUNCNAME,_quicksort)(TYPE *p, int n)                         \
{                                                                             \
   /* threshold for transitioning to insertion sort */                       \
   while (n > 12) {                                                           \
      TYPE *a,*b,t;                                                           \
      int c01,c12,c,m,i,j;                                                    \
                                                                              \
      /* compute median of three */                                           \
      m = n >> 1;                                                             \
      a = &p[0];                                                              \
      b = &p[m];                                                              \
      c = COMPARE;                                                            \
      c01 = c;                                                                \
      a = &p[m];                                                              \
      b = &p[n-1];                                                            \
      c = COMPARE;                                                            \
      c12 = c;                                                                \
      /* if 0 >= mid >= end, or 0 < mid < end, then use mid */                \
      if (c01 != c12) {                                                       \
         /* otherwise, we'll need to swap something else to middle */         \
         int z;                                                               \
         a = &p[0];                                                           \
         b = &p[n-1];                                                         \
         c = COMPARE;                                                         \
         /* 0>mid && mid<n:  0>n => n; 0<n => 0 */                            \
         /* 0<mid && mid>n:  0>n => 0; 0<n => n */                            \
         z = (c == c12) ? 0 : n-1;                                            \
         t = p[z];                                                            \
         p[z] = p[m];                                                         \
         p[m] = t;                                                            \
      }                                                                       \
      /* now p[m] is the median-of-three */                                   \
      /* swap it to the beginning so it won't move around */                  \
      t = p[0];                                                               \
      p[0] = p[m];                                                            \
      p[m] = t;                                                               \
                                                                              \
      /* partition loop */                                                    \
      i=1;                                                                    \
      j=n-1;                                                                  \
      for(;;) {                                                               \
         /* handling of equality is crucial here */                           \
         /* for sentinels & efficiency with duplicates */                     \
         b = &p[0];                                                           \
         for (;;++i) {                                                        \
            a=&p[i];                                                          \
            c = COMPARE;                                                      \
            if (!c) break;                                                    \
         }                                                                    \
         a = &p[0];                                                           \
         for (;;--j) {                                                        \
            b=&p[j];                                                          \
            c = COMPARE;                                                      \
            if (!c) break;                                                    \
         }                                                                    \
         /* make sure we haven't crossed */                                   \
         if (i >= j) break;                                                   \
         t = p[i];                                                            \
         p[i] = p[j];                                                         \
         p[j] = t;                                                            \
                                                                              \
         ++i;                                                                 \
         --j;                                                                 \
      }                                                                       \
      /* recurse on smaller side, iterate on larger */                        \
      if (j < (n-i)) {                                                        \
         STB_(FUNCNAME,_quicksort)(p,j);                                       \
         p = p+i;                                                             \
         n = n-i;                                                             \
      } else {                                                                \
         STB_(FUNCNAME,_quicksort)(p+i, n-i);                                  \
         n = j;                                                               \
      }                                                                       \
   }                                                                          \
}                                                                             \
                                                                              \
MODE FUNCNAME(TYPE *p, int n)                                                 \
{                                                                             \
   STB_(FUNCNAME, _quicksort)(p, n);                                           \
   STB_(FUNCNAME, _ins_sort)(p, n);                                            \
}                                                                             \


//////////////////////////////////////////////////////////////////////////////
//
//      stb_bitset   an array of booleans indexed by integers
//

typedef stb_uint32 stb_bitset;

STB_EXTERN  stb_bitset* stb_bitset_new(int value, int len);

#define stb_bitset_clearall(arr,len)     (memset(arr,   0, 4 * (len)))
#define stb_bitset_setall(arr,len)       (memset(arr, 255, 4 * (len)))

#define stb_bitset_setbit(arr,n)         ((arr)[(n) >> 5] |=  (1 << (n & 31)))
#define stb_bitset_clearbit(arr,n)       ((arr)[(n) >> 5] &= ~(1 << (n & 31)))
#define stb_bitset_testbit(arr,n)        ((arr)[(n) >> 5] &   (1 << (n & 31)))

STB_EXTERN  stb_bitset* stb_bitset_union(stb_bitset * p0, stb_bitset * p1, int len);

STB_EXTERN  int* stb_bitset_getlist(stb_bitset * out, int start, int end);

STB_EXTERN  int  stb_bitset_eq(stb_bitset * p0, stb_bitset * p1, int len);
STB_EXTERN  int  stb_bitset_disjoint(stb_bitset * p0, stb_bitset * p1, int len);
STB_EXTERN  int  stb_bitset_disjoint_0(stb_bitset * p0, stb_bitset * p1, int len);
STB_EXTERN  int  stb_bitset_subset(stb_bitset * bigger, stb_bitset * smaller, int len);
STB_EXTERN  int  stb_bitset_unioneq_changed(stb_bitset * p0, stb_bitset * p1, int len);

#ifdef STB_DEFINE
int stb_bitset_eq(stb_bitset * p0, stb_bitset * p1, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        if (p0[i] != p1[i]) return 0;
    return 1;
}

int stb_bitset_disjoint(stb_bitset * p0, stb_bitset * p1, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        if (p0[i] & p1[i]) return 0;
    return 1;
}

int stb_bitset_disjoint_0(stb_bitset * p0, stb_bitset * p1, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        if ((p0[i] | p1[i]) != 0xffffffff) return 0;
    return 1;
}

int stb_bitset_subset(stb_bitset * bigger, stb_bitset * smaller, int len)
{
    int i;
    for (i = 0; i < len; ++i)
        if ((bigger[i] & smaller[i]) != smaller[i]) return 0;
    return 1;
}

stb_bitset* stb_bitset_union(stb_bitset * p0, stb_bitset * p1, int len)
{
    int i;
    stb_bitset* d = (stb_bitset*)malloc(sizeof(*d) * len);
    for (i = 0; i < len; ++i) d[i] = p0[i] | p1[i];
    return d;
}

int stb_bitset_unioneq_changed(stb_bitset * p0, stb_bitset * p1, int len)
{
    int i, changed = 0;
    for (i = 0; i < len; ++i) {
        stb_bitset d = p0[i] | p1[i];
        if (d != p0[i]) {
            p0[i] = d;
            changed = 1;
        }
    }
    return changed;
}

stb_bitset* stb_bitset_new(int value, int len)
{
    int i;
    stb_bitset* d = (stb_bitset*)malloc(sizeof(*d) * len);
    if (value) value = 0xffffffff;
    for (i = 0; i < len; ++i) d[i] = value;
    return d;
}

int* stb_bitset_getlist(stb_bitset * out, int start, int end)
{
    int* list = NULL;
    int i;
    for (i = start; i < end; ++i)
        if (stb_bitset_testbit(out, i))
            stb_arr_push(list, i);
    return list;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//      stb_wordwrap    quality word-wrapping for fixed-width fonts
//

STB_EXTERN int stb_wordwrap(int* pairs, int pair_max, int count, char* str);
STB_EXTERN int* stb_wordwrapalloc(int count, char* str);

#ifdef STB_DEFINE

int stb_wordwrap(int* pairs, int pair_max, int count, char* str)
{
    int n = 0, i = 0, start = 0, nonwhite = 0;
    if (pairs == NULL) pair_max = 0x7ffffff0;
    else pair_max *= 2;
    // parse
    for (;;) {
        int s = i; // first whitespace char; last nonwhite+1
        int w;   // word start
        // accept whitespace
        while (isspace(str[i])) {
            if (str[i] == '\n' || str[i] == '\r') {
                if (str[i] + str[i + 1] == '\n' + '\r') ++i;
                if (n >= pair_max) return -1;
                if (pairs) pairs[n] = start, pairs[n + 1] = s - start;
                n += 2;
                nonwhite = 0;
                start = i + 1;
                s = start;
            }
            ++i;
        }
        if (i >= start + count) {
            // we've gone off the end using whitespace
            if (nonwhite) {
                if (n >= pair_max) return -1;
                if (pairs) pairs[n] = start, pairs[n + 1] = s - start;
                n += 2;
                start = s = i;
                nonwhite = 0;
            }
            else {
                // output all the whitespace
                while (i >= start + count) {
                    if (n >= pair_max) return -1;
                    if (pairs) pairs[n] = start, pairs[n + 1] = count;
                    n += 2;
                    start += count;
                }
                s = start;
            }
        }

        if (str[i] == 0) break;
        // now scan out a word and see if it fits
        w = i;
        while (str[i] && !isspace(str[i])) {
            ++i;
        }
        // wrapped?
        if (i > start + count) {
            // huge?
            if (i - s <= count) {
                if (n >= pair_max) return -1;
                if (pairs) pairs[n] = start, pairs[n + 1] = s - start;
                n += 2;
                start = w;
            }
            else {
                // This word is longer than one line. If we wrap it onto N lines
                // there are leftover chars. do those chars fit on the cur line?
                // But if we have leading whitespace, we force it to start here.
                if ((w - start) + ((i - w) % count) <= count || !nonwhite) {
                    // output a full line
                    if (n >= pair_max) return -1;
                    if (pairs) pairs[n] = start, pairs[n + 1] = count;
                    n += 2;
                    start += count;
                    w = start;
                }
                else {
                    // output a partial line, trimming trailing whitespace
                    if (s != start) {
                        if (n >= pair_max) return -1;
                        if (pairs) pairs[n] = start, pairs[n + 1] = s - start;
                        n += 2;
                        start = w;
                    }
                }
                // now output full lines as needed
                while (start + count <= i) {
                    if (n >= pair_max) return -1;
                    if (pairs) pairs[n] = start, pairs[n + 1] = count;
                    n += 2;
                    start += count;
                }
            }
        }
        nonwhite = 1;
    }
    if (start < i) {
        if (n >= pair_max) return -1;
        if (pairs) pairs[n] = start, pairs[n + 1] = i - start;
        n += 2;
    }
    return n >> 1;
}

int* stb_wordwrapalloc(int count, char* str)
{
    int n = stb_wordwrap(NULL, 0, count, str);
    int* z = NULL;
    stb_arr_setlen(z, n * 2);
    stb_wordwrap(z, n, count, str);
    return z;
}
#endif


//////////////////////////////////////////////////////////////////////////////
//
//         stb_match:    wildcards and regexping
//

STB_EXTERN int stb_wildmatch(char* expr, char* candidate);
STB_EXTERN int stb_wildmatchi(char* expr, char* candidate);
STB_EXTERN int stb_wildfind(char* expr, char* candidate);
STB_EXTERN int stb_wildfindi(char* expr, char* candidate);

STB_EXTERN int stb_regex(char* regex, char* candidate);

typedef struct stb_matcher stb_matcher;

STB_EXTERN stb_matcher* stb_regex_matcher(char* regex);
STB_EXTERN int stb_matcher_match(stb_matcher * m, char* str);
STB_EXTERN int stb_matcher_find(stb_matcher * m, char* str);
STB_EXTERN void stb_matcher_free(stb_matcher * f);

STB_EXTERN stb_matcher* stb_lex_matcher(void);
STB_EXTERN int stb_lex_item(stb_matcher * m, const char* str, int result);
STB_EXTERN int stb_lex_item_wild(stb_matcher * matcher, const char* regex, int result);
STB_EXTERN int stb_lex(stb_matcher * m, char* str, int* len);



#ifdef STB_DEFINE

static int stb__match_qstring(char* candidate, char* qstring, int qlen, int insensitive)
{
    int i;
    if (insensitive) {
        for (i = 0; i < qlen; ++i)
            if (qstring[i] == '?') {
                if (!candidate[i]) return 0;
            }
            else
                if (tolower(qstring[i]) != tolower(candidate[i]))
                    return 0;
    }
    else {
        for (i = 0; i < qlen; ++i)
            if (qstring[i] == '?') {
                if (!candidate[i]) return 0;
            }
            else
                if (qstring[i] != candidate[i])
                    return 0;
    }
    return 1;
}

static int stb__find_qstring(char* candidate, char* qstring, int qlen, int insensitive)
{
    char c;

    int offset = 0;
    while (*qstring == '?') {
        ++qstring;
        --qlen;
        ++candidate;
        if (qlen == 0) return 0;
        if (*candidate == 0) return -1;
    }

    c = *qstring++;
    --qlen;
    if (insensitive) c = tolower(c);

    while (candidate[offset]) {
        if (c == (insensitive ? tolower(candidate[offset]) : candidate[offset]))
            if (stb__match_qstring(candidate + offset + 1, qstring, qlen, insensitive))
                return offset;
        ++offset;
    }

    return -1;
}

int stb__wildmatch_raw2(char* expr, char* candidate, int search, int insensitive)
{
    int where = 0;
    int start = -1;

    if (!search) {
        // parse to first '*'
        if (*expr != '*')
            start = 0;
        while (*expr != '*') {
            if (!*expr)
                return *candidate == 0 ? 0 : -1;
            if (*expr == '?') {
                if (!*candidate) return -1;
            }
            else {
                if (insensitive) {
                    if (tolower(*candidate) != tolower(*expr))
                        return -1;
                }
                else
                    if (*candidate != *expr)
                        return -1;
            }
            ++candidate, ++expr, ++where;
        }
    }
    else {
        // 0-length search string
        if (!*expr)
            return 0;
    }

    assert(search || *expr == '*');
    if (!search)
        ++expr;

    // implicit '*' at this point

    while (*expr) {
        int o = 0;
        // combine redundant * characters
        while (expr[0] == '*') ++expr;

        // ok, at this point, expr[-1] == '*',
        // and expr[0] != '*'

        if (!expr[0]) return start >= 0 ? start : 0;

        // now find next '*'
        o = 0;
        while (expr[o] != '*') {
            if (expr[o] == 0)
                break;
            ++o;
        }
        // if no '*', scan to end, then match at end
        if (expr[o] == 0 && !search) {
            int z;
            for (z = 0; z < o; ++z)
                if (candidate[z] == 0)
                    return -1;
            while (candidate[z])
                ++z;
            // ok, now check if they match
            if (stb__match_qstring(candidate + z - o, expr, o, insensitive))
                return start >= 0 ? start : 0;
            return -1;
        }
        else {
            // if yes '*', then do stb__find_qmatch on the intervening chars
            int n = stb__find_qstring(candidate, expr, o, insensitive);
            if (n < 0)
                return -1;
            if (start < 0)
                start = where + n;
            expr += o;
            candidate += n + o;
        }

        if (*expr == 0) {
            assert(search);
            return start;
        }

        assert(*expr == '*');
        ++expr;
    }

    return start >= 0 ? start : 0;
}

int stb__wildmatch_raw(char* expr, char* candidate, int search, int insensitive)
{
    char buffer[256];
    // handle multiple search strings
    char* s = strchr(expr, ';');
    char* last = expr;
    while (s) {
        int z;
        // need to allow for non-writeable strings... assume they're small
        if (s - last < 256) {
            stb_strncpy(buffer, last, (int)(s - last + 1));
            buffer[s - last] = 0;
            z = stb__wildmatch_raw2(buffer, candidate, search, insensitive);
        }
        else {
            *s = 0;
            z = stb__wildmatch_raw2(last, candidate, search, insensitive);
            *s = ';';
        }
        if (z >= 0) return z;
        last = s + 1;
        s = strchr(last, ';');
    }
    return stb__wildmatch_raw2(last, candidate, search, insensitive);
}

int stb_wildmatch(char* expr, char* candidate)
{
    return stb__wildmatch_raw(expr, candidate, 0, 0) >= 0;
}

int stb_wildmatchi(char* expr, char* candidate)
{
    return stb__wildmatch_raw(expr, candidate, 0, 1) >= 0;
}

int stb_wildfind(char* expr, char* candidate)
{
    return stb__wildmatch_raw(expr, candidate, 1, 0);
}

int stb_wildfindi(char* expr, char* candidate)
{
    return stb__wildmatch_raw(expr, candidate, 1, 1);
}

typedef struct
{
    stb_int16 transition[256];
} stb_dfa;

// an NFA node represents a state you're in; it then has
// an arbitrary number of edges dangling off of it
// note this isn't utf8-y
typedef struct
{
    stb_int16  match; // character/set to match
    stb_uint16 node;  // output node to go to
} stb_nfa_edge;

typedef struct
{
    stb_int16 goal;   // does reaching this win the prize?
    stb_uint8 active; // is this in the active list
    stb_nfa_edge* out;
    stb_uint16* eps;  // list of epsilon closures
} stb_nfa_node;

#define STB__DFA_UNDEF  -1
#define STB__DFA_GOAL   -2
#define STB__DFA_END    -3
#define STB__DFA_MGOAL  -4
#define STB__DFA_VALID  0

#define STB__NFA_STOP_GOAL -1

// compiled regexp
struct stb_matcher
{
    stb_uint16 start_node;
    stb_int16 dfa_start;
    stb_uint32* charset;
    int num_charset;
    int match_start;
    stb_nfa_node* nodes;
    int does_lex;

    // dfa matcher
    stb_dfa* dfa;
    stb_uint32* dfa_mapping;
    stb_int16* dfa_result;
    int num_words_per_dfa;
};

static int stb__add_node(stb_matcher * matcher)
{
    stb_nfa_node z;
    z.active = 0;
    z.eps = 0;
    z.goal = 0;
    z.out = 0;
    stb_arr_push(matcher->nodes, z);
    return stb_arr_len(matcher->nodes) - 1;
}

static void stb__add_epsilon(stb_matcher * matcher, int from, int to)
{
    assert(from != to);
    if (matcher->nodes[from].eps == NULL)
        stb_arr_malloc((void**)&matcher->nodes[from].eps, matcher);
    stb_arr_push(matcher->nodes[from].eps, to);
}

static void stb__add_edge(stb_matcher * matcher, int from, int to, int type)
{
    stb_nfa_edge z = { (stb_int16)type, (stb_uint16)to };
    if (matcher->nodes[from].out == NULL)
        stb_arr_malloc((void**)&matcher->nodes[from].out, matcher);
    stb_arr_push(matcher->nodes[from].out, z);
}

static char* stb__reg_parse_alt(stb_matcher * m, int s, char* r, stb_uint16 * e);
static char* stb__reg_parse(stb_matcher * matcher, int start, char* regex, stb_uint16 * end)
{
    int n;
    int last_start = -1;
    stb_uint16 last_end = start;

    while (*regex) {
        switch (*regex) {
        case '(':
            last_start = last_end;
            regex = stb__reg_parse_alt(matcher, last_end, regex + 1, &last_end);
            if (regex == NULL || *regex != ')')
                return NULL;
            ++regex;
            break;

        case '|':
        case ')':
            *end = last_end;
            return regex;

        case '?':
            if (last_start < 0) return NULL;
            stb__add_epsilon(matcher, last_start, last_end);
            ++regex;
            break;

        case '*':
            if (last_start < 0) return NULL;
            stb__add_epsilon(matcher, last_start, last_end);

            // fall through

        case '+':
            if (last_start < 0) return NULL;
            stb__add_epsilon(matcher, last_end, last_start);
            // prevent links back to last_end from chaining to last_start
            n = stb__add_node(matcher);
            stb__add_epsilon(matcher, last_end, n);
            last_end = n;
            ++regex;
            break;

        case '{':   // not supported!
           // @TODO: given {n,m}, clone last_start to last_end m times,
           // and include epsilons from start to first m-n blocks
            return NULL;

        case '\\':
            ++regex;
            if (!*regex) return NULL;

            // fallthrough
        default: // match exactly this character
            n = stb__add_node(matcher);
            stb__add_edge(matcher, last_end, n, *regex);
            last_start = last_end;
            last_end = n;
            ++regex;
            break;

        case '$':
            n = stb__add_node(matcher);
            stb__add_edge(matcher, last_end, n, '\n');
            last_start = last_end;
            last_end = n;
            ++regex;
            break;

        case '.':
            n = stb__add_node(matcher);
            stb__add_edge(matcher, last_end, n, -1);
            last_start = last_end;
            last_end = n;
            ++regex;
            break;

        case '[': {
            stb_uint8 flags[256];
            int invert = 0, z;
            ++regex;
            if (matcher->num_charset == 0) {
                matcher->charset = (stb_uint*)stb_malloc(matcher, sizeof(*matcher->charset) * 256);
                memset(matcher->charset, 0, sizeof(*matcher->charset) * 256);
            }

            memset(flags, 0, sizeof(flags));

            // leading ^ is special
            if (*regex == '^')
                ++regex, invert = 1;

            // leading ] is special
            if (*regex == ']') {
                flags[(int)']'] = 1;
                ++regex;
            }
            while (*regex != ']') {
                stb_uint a;
                if (!*regex) return NULL;
                a = *regex++;
                if (regex[0] == '-' && regex[1] != ']') {
                    stb_uint i, b = regex[1];
                    regex += 2;
                    if (b == 0) return NULL;
                    if (a > b) return NULL;
                    for (i = a; i <= b; ++i)
                        flags[i] = 1;
                }
                else
                    flags[a] = 1;
            }
            ++regex;
            if (invert) {
                int i;
                for (i = 0; i < 256; ++i)
                    flags[i] = 1 - flags[i];
            }

            // now check if any existing charset matches
            for (z = 0; z < matcher->num_charset; ++z) {
                int i, k[2] = { 0, 1 << z };
                for (i = 0; i < 256; ++i) {
                    unsigned int f = k[flags[i]];
                    if ((matcher->charset[i] & k[1]) != f)
                        break;
                }
                if (i == 256) break;
            }

            if (z == matcher->num_charset) {
                int i;
                ++matcher->num_charset;
                if (matcher->num_charset > 32) {
                    assert(0); /* NOTREACHED */
                    return NULL; // too many charsets, oops
                }
                for (i = 0; i < 256; ++i)
                    if (flags[i])
                        matcher->charset[i] |= (1 << z);
            }

            n = stb__add_node(matcher);
            stb__add_edge(matcher, last_end, n, -2 - z);
            last_start = last_end;
            last_end = n;
            break;
        }
        }
    }
    *end = last_end;
    return regex;
}

static char* stb__reg_parse_alt(stb_matcher * matcher, int start, char* regex, stb_uint16 * end)
{
    stb_uint16 last_end = start;
    stb_uint16 main_end;

    int head, tail;

    head = stb__add_node(matcher);
    stb__add_epsilon(matcher, start, head);

    regex = stb__reg_parse(matcher, head, regex, &last_end);
    if (regex == NULL) return NULL;
    if (*regex == 0 || *regex == ')') {
        *end = last_end;
        return regex;
    }

    main_end = last_end;
    tail = stb__add_node(matcher);

    stb__add_epsilon(matcher, last_end, tail);

    // start alternatives from the same starting node; use epsilon
    // transitions to combine their endings
    while (*regex && *regex != ')') {
        assert(*regex == '|');
        head = stb__add_node(matcher);
        stb__add_epsilon(matcher, start, head);
        regex = stb__reg_parse(matcher, head, regex + 1, &last_end);
        if (regex == NULL)
            return NULL;
        stb__add_epsilon(matcher, last_end, tail);
    }

    *end = tail;
    return regex;
}

static char* stb__wild_parse(stb_matcher * matcher, int start, char* str, stb_uint16 * end)
{
    int n;
    stb_uint16 last_end;

    last_end = stb__add_node(matcher);
    stb__add_epsilon(matcher, start, last_end);

    while (*str) {
        switch (*str) {
            // fallthrough
        default: // match exactly this character
            n = stb__add_node(matcher);
            if (toupper(*str) == tolower(*str)) {
                stb__add_edge(matcher, last_end, n, *str);
            }
            else {
                stb__add_edge(matcher, last_end, n, tolower(*str));
                stb__add_edge(matcher, last_end, n, toupper(*str));
            }
            last_end = n;
            ++str;
            break;

        case '?':
            n = stb__add_node(matcher);
            stb__add_edge(matcher, last_end, n, -1);
            last_end = n;
            ++str;
            break;

        case '*':
            n = stb__add_node(matcher);
            stb__add_edge(matcher, last_end, n, -1);
            stb__add_epsilon(matcher, last_end, n);
            stb__add_epsilon(matcher, n, last_end);
            last_end = n;
            ++str;
            break;
        }
    }

    // now require end of string to match
    n = stb__add_node(matcher);
    stb__add_edge(matcher, last_end, n, 0);
    last_end = n;

    *end = last_end;
    return str;
}

static int stb__opt(stb_matcher * m, int n)
{
    for (;;) {
        stb_nfa_node* p = &m->nodes[n];
        if (p->goal)                  return n;
        if (stb_arr_len(p->out))      return n;
        if (stb_arr_len(p->eps) != 1) return n;
        n = p->eps[0];
    }
}

static void stb__optimize(stb_matcher * m)
{
    // if the target of any edge is a node with exactly
    // one out-epsilon, shorten it
    int i, j;
    for (i = 0; i < stb_arr_len(m->nodes); ++i) {
        stb_nfa_node* p = &m->nodes[i];
        for (j = 0; j < stb_arr_len(p->out); ++j)
            p->out[j].node = stb__opt(m, p->out[j].node);
        for (j = 0; j < stb_arr_len(p->eps); ++j)
            p->eps[j] = stb__opt(m, p->eps[j]);
    }
    m->start_node = stb__opt(m, m->start_node);
}

void stb_matcher_free(stb_matcher * f)
{
    stb_free(f);
}

static stb_matcher* stb__alloc_matcher(void)
{
    stb_matcher* matcher = (stb_matcher*)stb_malloc(0, sizeof(*matcher));

    matcher->start_node = 0;
    stb_arr_malloc((void**)&matcher->nodes, matcher);
    matcher->num_charset = 0;
    matcher->match_start = 0;
    matcher->does_lex = 0;

    matcher->dfa_start = STB__DFA_UNDEF;
    stb_arr_malloc((void**)&matcher->dfa, matcher);
    stb_arr_malloc((void**)&matcher->dfa_mapping, matcher);
    stb_arr_malloc((void**)&matcher->dfa_result, matcher);

    stb__add_node(matcher);

    return matcher;
}

static void stb__lex_reset(stb_matcher * matcher)
{
    // flush cached dfa data
    stb_arr_setlen(matcher->dfa, 0);
    stb_arr_setlen(matcher->dfa_mapping, 0);
    stb_arr_setlen(matcher->dfa_result, 0);
    matcher->dfa_start = STB__DFA_UNDEF;
}

stb_matcher* stb_regex_matcher(char* regex)
{
    char* z;
    stb_uint16 end;
    stb_matcher* matcher = stb__alloc_matcher();
    if (*regex == '^') {
        matcher->match_start = 1;
        ++regex;
    }

    z = stb__reg_parse_alt(matcher, matcher->start_node, regex, &end);

    if (!z || *z) {
        stb_free(matcher);
        return NULL;
    }

    ((matcher->nodes)[(int)end]).goal = STB__NFA_STOP_GOAL;

    return matcher;
}

stb_matcher* stb_lex_matcher(void)
{
    stb_matcher* matcher = stb__alloc_matcher();

    matcher->match_start = 1;
    matcher->does_lex = 1;

    return matcher;
}

int stb_lex_item(stb_matcher * matcher, const char* regex, int result)
{
    char* z;
    stb_uint16 end;

    z = stb__reg_parse_alt(matcher, matcher->start_node, (char*)regex, &end);

    if (z == NULL)
        return 0;

    stb__lex_reset(matcher);

    matcher->nodes[(int)end].goal = result;
    return 1;
}

int stb_lex_item_wild(stb_matcher * matcher, const char* regex, int result)
{
    char* z;
    stb_uint16 end;

    z = stb__wild_parse(matcher, matcher->start_node, (char*)regex, &end);

    if (z == NULL)
        return 0;

    stb__lex_reset(matcher);

    matcher->nodes[(int)end].goal = result;
    return 1;
}

static void stb__clear(stb_matcher * m, stb_uint16 * list)
{
    int i;
    for (i = 0; i < stb_arr_len(list); ++i)
        m->nodes[(int)list[i]].active = 0;
}

static int stb__clear_goalcheck(stb_matcher * m, stb_uint16 * list)
{
    int i, t = 0;
    for (i = 0; i < stb_arr_len(list); ++i) {
        t += m->nodes[(int)list[i]].goal;
        m->nodes[(int)list[i]].active = 0;
    }
    return t;
}

static stb_uint16* stb__add_if_inactive(stb_matcher * m, stb_uint16 * list, int n)
{
    if (!m->nodes[n].active) {
        stb_arr_push(list, n);
        m->nodes[n].active = 1;
    }
    return list;
}

static stb_uint16* stb__eps_closure(stb_matcher * m, stb_uint16 * list)
{
    int i, n = stb_arr_len(list);

    for (i = 0; i < n; ++i) {
        stb_uint16* e = m->nodes[(int)list[i]].eps;
        if (e) {
            int j, k = stb_arr_len(e);
            for (j = 0; j < k; ++j)
                list = stb__add_if_inactive(m, list, e[j]);
            n = stb_arr_len(list);
        }
    }

    return list;
}

int stb_matcher_match(stb_matcher * m, char* str)
{
    int result = 0;
    int i, j, y, z;
    stb_uint16* previous = NULL;
    stb_uint16* current = NULL;
    stb_uint16* temp;

    stb_arr_setsize(previous, 4);
    stb_arr_setsize(current, 4);

    previous = stb__add_if_inactive(m, previous, m->start_node);
    previous = stb__eps_closure(m, previous);
    stb__clear(m, previous);

    while (*str && stb_arr_len(previous)) {
        y = stb_arr_len(previous);
        for (i = 0; i < y; ++i) {
            stb_nfa_node* n = &m->nodes[(int)previous[i]];
            z = stb_arr_len(n->out);
            for (j = 0; j < z; ++j) {
                if (n->out[j].match >= 0) {
                    if (n->out[j].match == *str)
                        current = stb__add_if_inactive(m, current, n->out[j].node);
                }
                else if (n->out[j].match == -1) {
                    if (*str != '\n')
                        current = stb__add_if_inactive(m, current, n->out[j].node);
                }
                else if (n->out[j].match < -1) {
                    int z = -n->out[j].match - 2;
                    if (m->charset[(stb_uint8)*str] & (1 << z))
                        current = stb__add_if_inactive(m, current, n->out[j].node);
                }
            }
        }
        stb_arr_setlen(previous, 0);

        temp = previous;
        previous = current;
        current = temp;

        previous = stb__eps_closure(m, previous);
        stb__clear(m, previous);

        ++str;
    }

    // transition to pick up a '$' at the end
    y = stb_arr_len(previous);
    for (i = 0; i < y; ++i)
        m->nodes[(int)previous[i]].active = 1;

    for (i = 0; i < y; ++i) {
        stb_nfa_node* n = &m->nodes[(int)previous[i]];
        z = stb_arr_len(n->out);
        for (j = 0; j < z; ++j) {
            if (n->out[j].match == '\n')
                current = stb__add_if_inactive(m, current, n->out[j].node);
        }
    }

    previous = stb__eps_closure(m, previous);
    stb__clear(m, previous);

    y = stb_arr_len(previous);
    for (i = 0; i < y; ++i)
        if (m->nodes[(int)previous[i]].goal)
            result = 1;

    stb_arr_free(previous);
    stb_arr_free(current);

    return result && *str == 0;
}

stb_int16 stb__get_dfa_node(stb_matcher * m, stb_uint16 * list)
{
    stb_uint16 node;
    stb_uint32 data[8], * state, * newstate;
    int i, j, n;

    state = (stb_uint32*)stb_temp(data, m->num_words_per_dfa * 4);
    memset(state, 0, m->num_words_per_dfa * 4);

    n = stb_arr_len(list);
    for (i = 0; i < n; ++i) {
        int x = list[i];
        state[x >> 5] |= 1 << (x & 31);
    }

    // @TODO use a hash table
    n = stb_arr_len(m->dfa_mapping);
    i = j = 0;
    for (; j < n; ++i, j += m->num_words_per_dfa) {
        // @TODO special case for <= 32
        if (!memcmp(state, m->dfa_mapping + j, m->num_words_per_dfa * 4)) {
            node = i;
            goto done;
        }
    }

    assert(stb_arr_len(m->dfa) == i);
    node = i;

    newstate = stb_arr_addn(m->dfa_mapping, m->num_words_per_dfa);
    memcpy(newstate, state, m->num_words_per_dfa * 4);

    // set all transitions to 'unknown'
    stb_arr_add(m->dfa);
    memset(m->dfa[i].transition, -1, sizeof(m->dfa[i].transition));

    if (m->does_lex) {
        int result = -1;
        n = stb_arr_len(list);
        for (i = 0; i < n; ++i) {
            if (m->nodes[(int)list[i]].goal > result)
                result = m->nodes[(int)list[i]].goal;
        }

        stb_arr_push(m->dfa_result, result);
    }

done:
    stb_tempfree(data, state);
    return node;
}

static int stb__matcher_dfa(stb_matcher * m, char* str_c, int* len)
{
    stb_uint8* str = (stb_uint8*)str_c;
    stb_int16 node, prevnode;
    stb_dfa* trans;
    int match_length = 0;
    stb_int16 match_result = 0;

    if (m->dfa_start == STB__DFA_UNDEF) {
        stb_uint16* list;

        m->num_words_per_dfa = (stb_arr_len(m->nodes) + 31) >> 5;
        stb__optimize(m);

        list = stb__add_if_inactive(m, NULL, m->start_node);
        list = stb__eps_closure(m, list);
        if (m->does_lex) {
            m->dfa_start = stb__get_dfa_node(m, list);
            stb__clear(m, list);
            // DON'T allow start state to be a goal state!
            // this allows people to specify regexes that can match 0
            // characters without them actually matching (also we don't
            // check _before_ advancing anyway
            if (m->dfa_start <= STB__DFA_MGOAL)
                m->dfa_start = -(m->dfa_start - STB__DFA_MGOAL);
        }
        else {
            if (stb__clear_goalcheck(m, list))
                m->dfa_start = STB__DFA_GOAL;
            else
                m->dfa_start = stb__get_dfa_node(m, list);
        }
        stb_arr_free(list);
    }

    prevnode = STB__DFA_UNDEF;
    node = m->dfa_start;
    trans = m->dfa;

    if (m->dfa_start == STB__DFA_GOAL)
        return 1;

    for (;;) {
        assert(node >= STB__DFA_VALID);

        // fast inner DFA loop; especially if STB__DFA_VALID is 0

        do {
            prevnode = node;
            node = trans[node].transition[*str++];
        } while (node >= STB__DFA_VALID);

        assert(node >= STB__DFA_MGOAL - stb_arr_len(m->dfa));
        assert(node < stb_arr_len(m->dfa));

        // special case for lex: need _longest_ match, so notice goal
        // state without stopping
        if (node <= STB__DFA_MGOAL) {
            match_length = (int)(str - (stb_uint8*)str_c);
            node = -(node - STB__DFA_MGOAL);
            match_result = node;
            continue;
        }

        // slow NFA->DFA conversion

        // or we hit the goal or the end of the string, but those
        // can only happen once per search...

        if (node == STB__DFA_UNDEF) {
            // build a list  -- @TODO special case <= 32 states
            // heck, use a more compact data structure for <= 16 and <= 8 ?!

            // @TODO keep states/newstates around instead of reallocating them
            stb_uint16* states = NULL;
            stb_uint16* newstates = NULL;
            int i, j, y, z;
            stb_uint32* flags = &m->dfa_mapping[prevnode * m->num_words_per_dfa];
            assert(prevnode != STB__DFA_UNDEF);
            stb_arr_setsize(states, 4);
            stb_arr_setsize(newstates, 4);
            for (j = 0; j < m->num_words_per_dfa; ++j) {
                for (i = 0; i < 32; ++i) {
                    if (*flags & (1 << i))
                        stb_arr_push(states, j * 32 + i);
                }
                ++flags;
            }
            // states is now the states we were in in the previous node;
            // so now we can compute what node it transitions to on str[-1]

            y = stb_arr_len(states);
            for (i = 0; i < y; ++i) {
                stb_nfa_node* n = &m->nodes[(int)states[i]];
                z = stb_arr_len(n->out);
                for (j = 0; j < z; ++j) {
                    if (n->out[j].match >= 0) {
                        if (n->out[j].match == str[-1] || (str[-1] == 0 && n->out[j].match == '\n'))
                            newstates = stb__add_if_inactive(m, newstates, n->out[j].node);
                    }
                    else if (n->out[j].match == -1) {
                        if (str[-1] != '\n' && str[-1])
                            newstates = stb__add_if_inactive(m, newstates, n->out[j].node);
                    }
                    else if (n->out[j].match < -1) {
                        int z = -n->out[j].match - 2;
                        if (m->charset[str[-1]] & (1 << z))
                            newstates = stb__add_if_inactive(m, newstates, n->out[j].node);
                    }
                }
            }
            // AND add in the start state!
            if (!m->match_start || (str[-1] == '\n' && !m->does_lex))
                newstates = stb__add_if_inactive(m, newstates, m->start_node);
            // AND epsilon close it
            newstates = stb__eps_closure(m, newstates);
            // if it's a goal state, then that's all there is to it
            if (stb__clear_goalcheck(m, newstates)) {
                if (m->does_lex) {
                    match_length = (int)(str - (stb_uint8*)str_c);
                    node = stb__get_dfa_node(m, newstates);
                    match_result = node;
                    node = -node + STB__DFA_MGOAL;
                    trans = m->dfa; // could have gotten realloc()ed
                }
                else
                    node = STB__DFA_GOAL;
            }
            else if (str[-1] == 0 || stb_arr_len(newstates) == 0) {
                node = STB__DFA_END;
            }
            else {
                node = stb__get_dfa_node(m, newstates);
                trans = m->dfa; // could have gotten realloc()ed
            }
            trans[prevnode].transition[str[-1]] = node;
            if (node <= STB__DFA_MGOAL)
                node = -(node - STB__DFA_MGOAL);
            stb_arr_free(newstates);
            stb_arr_free(states);
        }

        if (node == STB__DFA_GOAL) {
            return 1;
        }
        if (node == STB__DFA_END) {
            if (m->does_lex) {
                if (match_result) {
                    if (len) *len = match_length;
                    return m->dfa_result[(int)match_result];
                }
            }
            return 0;
        }

        assert(node != STB__DFA_UNDEF);
    }
}

int stb_matcher_find(stb_matcher * m, char* str)
{
    assert(m->does_lex == 0);
    return stb__matcher_dfa(m, str, NULL);
}

int stb_lex(stb_matcher * m, char* str, int* len)
{
    assert(m->does_lex);
    return stb__matcher_dfa(m, str, len);
}

#ifdef STB_PERFECT_HASH
int stb_regex(char* regex, char* str)
{
    static stb_perfect p;
    static stb_matcher** matchers;
    static char** regexps;
    static char** regexp_cache;
    static unsigned short* mapping;
    int z = stb_perfect_hash(&p, (int)(size_t)regex);
    if (z >= 0) {
        if (strcmp(regex, regexp_cache[(int)mapping[z]])) {
            int i = mapping[z];
            stb_matcher_free(matchers[i]);
            free(regexp_cache[i]);
            regexps[i] = regex;
            regexp_cache[i] = stb_p_strdup(regex);
            matchers[i] = stb_regex_matcher(regex);
        }
    }
    else {
        int i, n;
        if (regex == NULL) {
            for (i = 0; i < stb_arr_len(matchers); ++i) {
                stb_matcher_free(matchers[i]);
                free(regexp_cache[i]);
            }
            stb_arr_free(matchers);
            stb_arr_free(regexps);
            stb_arr_free(regexp_cache);
            stb_perfect_destroy(&p);
            free(mapping); mapping = NULL;
            return -1;
        }
        stb_arr_push(regexps, regex);
        stb_arr_push(regexp_cache, stb_p_strdup(regex));
        stb_arr_push(matchers, stb_regex_matcher(regex));
        stb_perfect_destroy(&p);
        n = stb_perfect_create(&p, (unsigned int*)(char**)regexps, stb_arr_len(regexps));
        mapping = (unsigned short*)realloc(mapping, n * sizeof(*mapping));
        for (i = 0; i < stb_arr_len(regexps); ++i)
            mapping[stb_perfect_hash(&p, (int)(size_t)regexps[i])] = i;
        z = stb_perfect_hash(&p, (int)(size_t)regex);
    }
    return stb_matcher_find(matchers[(int)mapping[z]], str);
}
#endif
#endif // STB_DEFINE


#if 0
//////////////////////////////////////////////////////////////////////////////
//
//                      C source-code introspection
//

// runtime structure
typedef struct
{
    char* name;
    char* type;     // base type
    char* comment;  // content of comment field
    int   size;     // size of base type
    int   offset;   // field offset
    int   arrcount[8]; // array sizes; -1 = pointer indirection; 0 = end of list
} stb_info_field;

typedef struct
{
    char* structname;
    int size;
    int num_fields;
    stb_info_field* fields;
} stb_info_struct;

extern stb_info_struct stb_introspect_output[];

//

STB_EXTERN void stb_introspect_precompiled(stb_info_struct * compiled);
STB_EXTERN void stb__introspect(char* path, char* file);

#define stb_introspect_ship()            stb__introspect(NULL, NULL, stb__introspect_output)

#ifdef STB_SHIP
#define stb_introspect()                 stb_introspect_ship()
#define stb_introspect_path(p)           stb_introspect_ship()
#else
// bootstrapping: define stb_introspect() (or 'path') the first time
#define stb_introspect()                 stb__introspect(NULL, __FILE__, NULL)
#define stb_introspect_auto()            stb__introspect(NULL, __FILE__, stb__introspect_output)

#define stb_introspect_path(p)           stb__introspect(p, __FILE__, NULL)
#define stb_introspect_path(p)           stb__introspect(p, __FILE__, NULL)
#endif

#ifdef STB_DEFINE

#ifndef STB_INTROSPECT_CPP
#ifdef __cplusplus
#define STB_INTROSPECT_CPP 1
#else
#define STB_INTROSPECT_CPP 0
#endif
#endif

void stb_introspect_precompiled(stb_info_struct * compiled)
{

}


static void stb__introspect_filename(char* buffer, char* path)
{
#if STB_INTROSPECT_CPP
    stb_p_sprintf(buffer stb_p_size(9999), "%s/stb_introspect.cpp", path);
#else
    stb_p_sprintf(buffer stb_p_size(9999), "%s/stb_introspect.c", path);
#endif
}

static void stb__introspect_compute(char* path, char* file)
{
    int i;
    char** include_list = NULL;
    char** introspect_list = NULL;
    FILE* f;
    f = stb_p_fopen(file, "w");
    if (!f) return;

    fputs("// if you get compiler errors, change the following 0 to a 1:\n", f);
    fputs("#define STB_INTROSPECT_INVALID 0\n\n", f);
    fputs("// this will force the code to compile, and force the introspector\n", f);
    fputs("// to run and then exit, allowing you to recompile\n\n\n", f);
    fputs("#include \"stb.h\"\n\n", f);
    fputs("#if STB_INTROSPECT_INVALID\n", f);
    fputs("   stb_info_struct stb__introspect_output[] = { (void *) 1 }\n", f);
    fputs("#else\n\n", f);
    for (i = 0; i < stb_arr_len(include_list); ++i)
        fprintf(f, " #include \"%s\"\n", include_list[i]);

    fputs(" stb_info_struct stb__introspect_output[] =\n{\n", f);
    for (i = 0; i < stb_arr_len(introspect_list); ++i)
        fprintf(f, "  stb_introspect_%s,\n", introspect_list[i]);
    fputs(" };\n", f);
    fputs("#endif\n", f);
    fclose(f);
}

static stb_info_struct* stb__introspect_info;

#ifndef STB_SHIP

#endif

void stb__introspect(char* path, char* file, stb_info_struct * compiled)
{
    static int first = 1;
    if (!first) return;
    first = 0;

    stb__introspect_info = compiled;

#ifndef STB_SHIP
    if (path || file) {
        int bail_flag = compiled && compiled[0].structname == (void*)1;
        int needs_building = bail_flag;
        struct stb__stat st;
        char buffer[1024], buffer2[1024];
        if (!path) {
            stb_splitpath(buffer, file, STB_PATH);
            path = buffer;
        }
        // bail if the source path doesn't exist
        if (!stb_fexists(path)) return;

        stb__introspect_filename(buffer2, path);

        // get source/include files timestamps, compare to output-file timestamp;
        // if mismatched, regenerate

        if (stb__stat(buffer2, &st))
            needs_building = STB_TRUE;

        {
            // find any file that contains an introspection command and is newer
            // if needs_building is already true, we don't need to do this test,
            // but we still need these arrays, so go ahead and get them
            char** all[3];
            all[0] = stb_readdir_files_mask(path, "*.h");
            all[1] = stb_readdir_files_mask(path, "*.c");
            all[2] = stb_readdir_files_mask(path, "*.cpp");
            int i, j;
            if (needs_building) {
                for (j = 0; j < 3; ++j) {
                    for (i = 0; i < stb_arr_len(all[j]); ++i) {
                        struct stb__stat st2;
                        if (!stb__stat(all[j][i], &st2)) {
                            if (st.st_mtime < st2.st_mtime) {
                                char* z = stb_filec(all[j][i], NULL);
                                int found = STB_FALSE;
                                while (y) {
                                    y = strstr(y, "//si");
                                    if (y && isspace(y[4])) {
                                        found = STB_TRUE;
                                        break;
                                    }
                                }
                                needs_building = STB_TRUE;
                                goto done;
                            }
                        }
                    }
                }
            done:;
            }
            char* z = stb_filec(all[i], NULL), * y = z;
            int found = STB_FALSE;
            while (y) {
                y = strstr(y, "//si");
                if (y && isspace(y[4])) {
                    found = STB_TRUE;
                    break;
                }
            }
            if (found)
                stb_arr_push(introspect_h, stb_p_strdup(all[i]));
            free(z);
        }
    }
    stb_readdir_free(all);
    if (!needs_building) {
        for (i = 0; i < stb_arr_len(introspect_h); ++i) {
            struct stb__stat st2;
            if (!stb__stat(introspect_h[i], &st2))
                if (st.st_mtime < st2.st_mtime)
                    needs_building = STB_TRUE;
        }
    }

    if (needs_building) {
        stb__introspect_compute(path, buffer2);
    }
}
   }
#endif
}
#endif
#endif

#ifdef STB_INTROSPECT
// compile-time code-generator
#define INTROSPECT(x)   int main(int argc, char **argv) { stb__introspect(__FILE__); return 0; }
#define FILE(x)

void stb__introspect(char* filename)
{
    char* file = stb_file(filename, NULL);
    char* s = file, * t, ** p;
    char* out_name = "stb_introspect.c";
    char* out_path;
    STB_ARR(char) filelist = NULL;
    int i, n;
    if (!file) stb_fatal("Couldn't open %s", filename);

    out_path = stb_splitpathdup(filename, STB_PATH);

    // search for the macros
    while (*s) {
        char buffer[256];
        while (*s && !isupper(*s)) ++s;
        s = stb_strtok_invert(buffer, s, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        s = stb_skipwhite(s);
        if (*s == '(') {
            ++s;
            t = strchr(s, ')');
            if (t == NULL) stb_fatal("Error parsing %s", filename);

        }
    }
}



#endif


//////////////////////////////////////////////////////////////////////////////
//
//             STB-C sliding-window dictionary compression
//
//  This uses a DEFLATE-style sliding window, but no bitwise entropy.
//  Everything is on byte boundaries, so you could then apply a byte-wise
//  entropy code, though that's nowhere near as effective.
//
//  An STB-C stream begins with a 16-byte header:
//      4 bytes: 0x57 0xBC 0x00 0x00
//      8 bytes: big-endian size of decompressed data, 64-bits
//      4 bytes: big-endian size of window (how far back decompressor may need)
//
//  The following symbols appear in the stream (these were determined ad hoc,
//  not by analysis):
//
//  [dict]      00000100 yyyyyyyy yyyyyyyy yyyyyyyy xxxxxxxx xxxxxxxx
//  [END]       00000101 11111010 cccccccc cccccccc cccccccc cccccccc
//  [dict]      00000110 yyyyyyyy yyyyyyyy yyyyyyyy xxxxxxxx
//  [literals]  00000111 zzzzzzzz zzzzzzzz
//  [literals]  00001zzz zzzzzzzz
//  [dict]      00010yyy yyyyyyyy yyyyyyyy xxxxxxxx xxxxxxxx
//  [dict]      00011yyy yyyyyyyy yyyyyyyy xxxxxxxx
//  [literals]  001zzzzz
//  [dict]      01yyyyyy yyyyyyyy xxxxxxxx
//  [dict]      1xxxxxxx yyyyyyyy
//
//      xxxxxxxx: match length - 1
//      yyyyyyyy: backwards distance - 1
//      zzzzzzzz: num literals - 1
//      cccccccc: adler32 checksum of decompressed data
//   (all big-endian)


STB_EXTERN stb_uint stb_decompress_length(stb_uchar* input);
STB_EXTERN stb_uint stb_decompress(stb_uchar* out, stb_uchar* in, stb_uint len);
STB_EXTERN stb_uint stb_compress(stb_uchar* out, stb_uchar* in, stb_uint len);
STB_EXTERN void stb_compress_window(int z);
STB_EXTERN void stb_compress_hashsize(unsigned int z);

STB_EXTERN int stb_compress_tofile(char* filename, char* in, stb_uint  len);
STB_EXTERN int stb_compress_intofile(FILE* f, char* input, stb_uint  len);
STB_EXTERN char* stb_decompress_fromfile(char* filename, stb_uint* len);

STB_EXTERN int stb_compress_stream_start(FILE* f);
STB_EXTERN void stb_compress_stream_end(int close);
STB_EXTERN void stb_write(char* data, int data_len);

#ifdef STB_DEFINE

stb_uint stb_decompress_length(stb_uchar* input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

////////////////////           decompressor         ///////////////////////

// simple implementation that just writes whole thing into big block

static unsigned char* stb__barrier;
static unsigned char* stb__barrier2;
static unsigned char* stb__barrier3;
static unsigned char* stb__barrier4;

static stb_uchar* stb__dout;
static void stb__match(stb_uchar* data, stb_uint length)
{
    // INVERSE of memmove... write each byte before copying the next...
    assert(stb__dout + length <= stb__barrier);
    if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
    if (data < stb__barrier4) { stb__dout = stb__barrier + 1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(stb_uchar* data, stb_uint length)
{
    assert(stb__dout + length <= stb__barrier);
    if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
    if (data < stb__barrier2) { stb__dout = stb__barrier + 1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static stb_uchar* stb_decompress_token(stb_uchar* i)
{
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    }
    else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
        else if (*i >= 0x08)  stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
    }
    return i;
}

stb_uint stb_decompress(stb_uchar* output, stb_uchar* i, stb_uint length)
{
    stb_uint olen;
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    olen = stb_decompress_length(i);
    stb__barrier2 = i;
    stb__barrier3 = i + length;
    stb__barrier = output + olen;
    stb__barrier4 = output;
    i += 16;

    stb__dout = output;
    while (1) {
        stb_uchar* old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                assert(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != (stb_uint)stb__in4(2))
                    return 0;
                return olen;
            }
            else {
                assert(0); /* NOTREACHED */
                return 0;
            }
        }
        assert(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

char* stb_decompress_fromfile(char* filename, unsigned int* len)
{
    unsigned int n;
    char* q;
    unsigned char* p;
    FILE* f = stb_p_fopen(filename, "rb");   if (f == NULL) return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = (unsigned char*)malloc(n); if (p == NULL) return NULL;
    fread(p, 1, n, f);
    fclose(f);
    if (p == NULL) return NULL;
    if (p[0] != 0x57 || p[1] != 0xBc || p[2] || p[3]) { free(p); return NULL; }
    q = (char*)malloc(stb_decompress_length(p) + 1);
    if (!q) { free(p); return NULL; }
    *len = stb_decompress((unsigned char*)q, p, n);
    if (*len) q[*len] = 0;
    free(p);
    return q;
}

#if 0
//  streaming decompressor

static struct
{
    stb__uchar* in_buffer;
    stb__uchar* match;

    stb__uint pending_literals;
    stb__uint pending_match;
} xx;



static void stb__match(stb_uchar* data, stb_uint length)
{
    // INVERSE of memmove... write each byte before copying the next...
    assert(stb__dout + length <= stb__barrier);
    if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
    if (data < stb__barrier2) { stb__dout = stb__barrier + 1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(stb_uchar* data, stb_uint length)
{
    assert(stb__dout + length <= stb__barrier);
    if (stb__dout + length > stb__barrier) { stb__dout += length; return; }
    if (data < stb__barrier2) { stb__dout = stb__barrier + 1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

static void sx_match(stb_uchar* data, stb_uint length)
{
    xx.match = data;
    xx.pending_match = length;
}

static void sx_lit(stb_uchar* data, stb_uint length)
{
    xx.pending_lit = length;
}

static int stb_decompress_token_state(void)
{
    stb__uchar* i = xx.in_buffer;

    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       sx_match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  sx_match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
        else /* *i >= 0x20 */ sx_lit(i + 1, i[0] - 0x20 + 1), i += 1;
    }
    else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       sx_match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
        else if (*i >= 0x10)  sx_match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
        else if (*i >= 0x08)  sx_lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2;
        else if (*i == 0x07)  sx_lit(i + 3, stb__in2(1) + 1), i += 3;
        else if (*i == 0x06)  sx_match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
        else if (*i == 0x04)  sx_match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
        else return 0;
    }
    xx.in_buffer = i;
    return 1;
}
#endif



////////////////////           compressor         ///////////////////////

static unsigned int stb_matchlen(stb_uchar* m1, stb_uchar* m2, stb_uint maxlen)
{
    stb_uint i;
    for (i = 0; i < maxlen; ++i)
        if (m1[i] != m2[i]) return i;
    return i;
}

// simple implementation that just takes the source data in a big block

static stb_uchar* stb__out;
static FILE* stb__outfile;
static stb_uint   stb__outbytes;

static void stb__write(unsigned char v)
{
    fputc(v, stb__outfile);
    ++stb__outbytes;
}

#define stb_out(v)    (stb__out ? (void)(*stb__out++ = (stb_uchar) (v)) : stb__write((stb_uchar) (v)))

static void stb_out2(stb_uint v)
{
    stb_out(v >> 8);
    stb_out(v);
}

static void stb_out3(stb_uint v) { stb_out(v >> 16); stb_out(v >> 8); stb_out(v); }
static void stb_out4(stb_uint v) {
    stb_out(v >> 24); stb_out(v >> 16);
    stb_out(v >> 8); stb_out(v);
}

static void outliterals(stb_uchar* in, ptrdiff_t numlit)
{
    while (numlit > 65536) {
        outliterals(in, 65536);
        in += 65536;
        numlit -= 65536;
    }

    if (numlit == 0);
    else if (numlit <= 32)    stb_out(0x000020 + (stb_uint)numlit - 1);
    else if (numlit <= 2048)    stb_out2(0x000800 + (stb_uint)numlit - 1);
    else /*  numlit <= 65536) */ stb_out3(0x070000 + (stb_uint)numlit - 1);

    if (stb__out) {
        memcpy(stb__out, in, numlit);
        stb__out += numlit;
    }
    else
        fwrite(in, 1, numlit, stb__outfile);
}

static int stb__window = 0x40000; // 256K
void stb_compress_window(int z)
{
    if (z >= 0x1000000) z = 0x1000000; // limit of implementation
    if (z < 0x100) z = 0x100;   // insanely small
    stb__window = z;
}

static int stb_not_crap(int best, int dist)
{
    return   ((best > 2 && dist <= 0x00100)
        || (best > 5 && dist <= 0x04000)
        || (best > 7 && dist <= 0x80000));
}

static  stb_uint stb__hashsize = 32768;
void stb_compress_hashsize(unsigned int y)
{
    unsigned int z = 1024;
    while (z < y) z <<= 1;
    stb__hashsize = z >> 2;   // pass in bytes, store #pointers
}

// note that you can play with the hashing functions all you
// want without needing to change the decompressor
#define stb__hc(q,h,c)      (((h) << 7) + ((h) >> 25) + q[c])
#define stb__hc2(q,h,c,d)   (((h) << 14) + ((h) >> 18) + (q[c] << 7) + q[d])
#define stb__hc3(q,c,d,e)   ((q[c] << 14) + (q[d] << 7) + q[e])

static stb_uint32 stb__running_adler;

static int stb_compress_chunk(stb_uchar* history,
    stb_uchar* start,
    stb_uchar* end,
    int length,
    int* pending_literals,
    stb_uchar** chash,
    stb_uint mask)
{
    int window = stb__window;
    stb_uint match_max;
    stb_uchar* lit_start = start - *pending_literals;
    stb_uchar* q = start;

#define STB__SCRAMBLE(h)   (((h) + ((h) >> 16)) & mask)

    // stop short of the end so we don't scan off the end doing
    // the hashing; this means we won't compress the last few bytes
    // unless they were part of something longer
    while (q < start + length && q + 12 < end) {
        int m;
        stb_uint h1, h2, h3, h4, h;
        stb_uchar* t;
        int best = 2, dist = 0;

        if (q + 65536 > end)
            match_max = (stb_uint)(end - q);
        else
            match_max = 65536u;

#define stb__nc(b,d)  ((d) <= window && ((b) > 9 || stb_not_crap(b,d)))

#define STB__TRY(t,p)  /* avoid retrying a match we already tried */ \
                      if (p ? dist != (int) (q-t) : 1)                     \
                      if ((m = (int) stb_matchlen(t, q, match_max)) > best)\
                      if (stb__nc(m,(int) (q-(t))))                        \
                          best = m, dist = (int) (q - (t))

        // rather than search for all matches, only try 4 candidate locations,
        // chosen based on 4 different hash functions of different lengths.
        // this strategy is inspired by LZO; hashing is unrolled here using the
        // 'hc' macro
        h = stb__hc3(q, 0, 1, 2); h1 = STB__SCRAMBLE(h);
        t = chash[h1]; if (t) STB__TRY(t, 0);
        h = stb__hc2(q, h, 3, 4); h2 = STB__SCRAMBLE(h);
        h = stb__hc2(q, h, 5, 6);        t = chash[h2]; if (t) STB__TRY(t, 1);
        h = stb__hc2(q, h, 7, 8); h3 = STB__SCRAMBLE(h);
        h = stb__hc2(q, h, 9, 10);        t = chash[h3]; if (t) STB__TRY(t, 1);
        h = stb__hc2(q, h, 11, 12); h4 = STB__SCRAMBLE(h);
        t = chash[h4]; if (t) STB__TRY(t, 1);

        // because we use a shared hash table, can only update it
        // _after_ we've probed all of them
        chash[h1] = chash[h2] = chash[h3] = chash[h4] = q;

        if (best > 2)
            assert(dist > 0);

        // see if our best match qualifies
        if (best < 3) { // fast path literals
            ++q;
        }
        else if (best > 2 && best <= 0x80 && dist <= 0x100) {
            outliterals(lit_start, q - lit_start); lit_start = (q += best);
            stb_out(0x80 + best - 1);
            stb_out(dist - 1);
        }
        else if (best > 5 && best <= 0x100 && dist <= 0x4000) {
            outliterals(lit_start, q - lit_start); lit_start = (q += best);
            stb_out2(0x4000 + dist - 1);
            stb_out(best - 1);
        }
        else if (best > 7 && best <= 0x100 && dist <= 0x80000) {
            outliterals(lit_start, q - lit_start); lit_start = (q += best);
            stb_out3(0x180000 + dist - 1);
            stb_out(best - 1);
        }
        else if (best > 8 && best <= 0x10000 && dist <= 0x80000) {
            outliterals(lit_start, q - lit_start); lit_start = (q += best);
            stb_out3(0x100000 + dist - 1);
            stb_out2(best - 1);
        }
        else if (best > 9 && dist <= 0x1000000) {
            if (best > 65536) best = 65536;
            outliterals(lit_start, q - lit_start); lit_start = (q += best);
            if (best <= 0x100) {
                stb_out(0x06);
                stb_out3(dist - 1);
                stb_out(best - 1);
            }
            else {
                stb_out(0x04);
                stb_out3(dist - 1);
                stb_out2(best - 1);
            }
        }
        else {  // fallback literals if no match was a balanced tradeoff
            ++q;
        }
    }

    // if we didn't get all the way, add the rest to literals
    if (q - start < length)
        q = start + length;

    // the literals are everything from lit_start to q
    *pending_literals = (int)(q - lit_start);

    stb__running_adler = stb_adler32(stb__running_adler, start, (int)(q - start));
    return (int)(q - start);
}

static int stb_compress_inner(stb_uchar* input, stb_uint length)
{
    int literals = 0;
    stb_uint len, i;

    stb_uchar** chash;
    chash = (stb_uchar**)malloc(stb__hashsize * sizeof(stb_uchar*));
    if (chash == NULL) return 0; // failure
    for (i = 0; i < stb__hashsize; ++i)
        chash[i] = NULL;

    // stream signature
    stb_out(0x57); stb_out(0xbc);
    stb_out2(0);

    stb_out4(0);       // 64-bit length requires 32-bit leading 0
    stb_out4(length);
    stb_out4(stb__window);

    stb__running_adler = 1;

    len = stb_compress_chunk(input, input, input + length, length, &literals, chash, stb__hashsize - 1);
    assert(len == length);

    outliterals(input + length - literals, literals);

    free(chash);

    stb_out2(0x05fa); // end opcode

    stb_out4(stb__running_adler);

    return 1; // success
}

stb_uint stb_compress(stb_uchar* out, stb_uchar* input, stb_uint length)
{
    stb__out = out;
    stb__outfile = NULL;

    stb_compress_inner(input, length);

    return (stb_uint)(stb__out - out);
}

int stb_compress_tofile(char* filename, char* input, unsigned int length)
{
    //int maxlen = length + 512 + (length >> 2); // total guess
    //char *buffer = (char *) malloc(maxlen);
    //int blen = stb_compress((stb_uchar*)buffer, (stb_uchar*)input, length);

    stb__out = NULL;
    stb__outfile = stb_p_fopen(filename, "wb");
    if (!stb__outfile) return 0;

    stb__outbytes = 0;

    if (!stb_compress_inner((stb_uchar*)input, length))
        return 0;

    fclose(stb__outfile);

    return stb__outbytes;
}

int stb_compress_intofile(FILE* f, char* input, unsigned int length)
{
    //int maxlen = length + 512 + (length >> 2); // total guess
    //char *buffer = (char*)malloc(maxlen);
    //int blen = stb_compress((stb_uchar*)buffer, (stb_uchar*)input, length);

    stb__out = NULL;
    stb__outfile = f;
    if (!stb__outfile) return 0;

    stb__outbytes = 0;

    if (!stb_compress_inner((stb_uchar*)input, length))
        return 0;

    return stb__outbytes;
}

//////////////////////    streaming I/O version    /////////////////////


static size_t stb_out_backpatch_id(void)
{
    if (stb__out)
        return (size_t)stb__out;
    else
        return ftell(stb__outfile);
}

static void stb_out_backpatch(size_t id, stb_uint value)
{
    stb_uchar data[4] = { (stb_uchar)(value >> 24), (stb_uchar)(value >> 16), (stb_uchar)(value >> 8), (stb_uchar)(value) };
    if (stb__out) {
        memcpy((void*)id, data, 4);
    }
    else {
        stb_uint where = ftell(stb__outfile);
        fseek(stb__outfile, (long)id, SEEK_SET);
        fwrite(data, 4, 1, stb__outfile);
        fseek(stb__outfile, where, SEEK_SET);
    }
}

// ok, the wraparound buffer was a total failure. let's instead
// use a copying-in-place buffer, which lets us share the code.
// This is way less efficient but it'll do for now.

static struct
{
    stb_uchar* buffer;
    int size;           // physical size of buffer in bytes

    int valid;          // amount of valid data in bytes
    int start;          // bytes of data already output

    int window;
    int fsize;

    int pending_literals; // bytes not-quite output but counted in start
    int length_id;

    stb_uint total_bytes;

    stb_uchar** chash;
    stb_uint    hashmask;
} xtb;

static int stb_compress_streaming_start(void)
{
    stb_uint i;
    xtb.size = stb__window * 3;
    xtb.buffer = (stb_uchar*)malloc(xtb.size);
    if (!xtb.buffer) return 0;

    xtb.chash = (stb_uchar**)malloc(sizeof(*xtb.chash) * stb__hashsize);
    if (!xtb.chash) {
        free(xtb.buffer);
        return 0;
    }

    for (i = 0; i < stb__hashsize; ++i)
        xtb.chash[i] = NULL;

    xtb.hashmask = stb__hashsize - 1;

    xtb.valid = 0;
    xtb.start = 0;
    xtb.window = stb__window;
    xtb.fsize = stb__window;
    xtb.pending_literals = 0;
    xtb.total_bytes = 0;

    // stream signature
    stb_out(0x57); stb_out(0xbc); stb_out2(0);

    stb_out4(0);       // 64-bit length requires 32-bit leading 0

    xtb.length_id = (int)stb_out_backpatch_id();
    stb_out4(0);       // we don't know the output length yet

    stb_out4(stb__window);

    stb__running_adler = 1;

    return 1;
}

static int stb_compress_streaming_end(void)
{
    // flush out any remaining data
    stb_compress_chunk(xtb.buffer, xtb.buffer + xtb.start, xtb.buffer + xtb.valid,
        xtb.valid - xtb.start, &xtb.pending_literals, xtb.chash, xtb.hashmask);

    // write out pending literals
    outliterals(xtb.buffer + xtb.valid - xtb.pending_literals, xtb.pending_literals);

    stb_out2(0x05fa); // end opcode
    stb_out4(stb__running_adler);

    stb_out_backpatch(xtb.length_id, xtb.total_bytes);

    free(xtb.buffer);
    free(xtb.chash);
    return 1;
}

void stb_write(char* data, int data_len)
{
    stb_uint i;

    // @TODO: fast path for filling the buffer and doing nothing else
    //   if (xtb.valid + data_len < xtb.size)

    xtb.total_bytes += data_len;

    while (data_len) {
        // fill buffer
        if (xtb.valid < xtb.size) {
            int amt = xtb.size - xtb.valid;
            if (data_len < amt) amt = data_len;
            memcpy(xtb.buffer + xtb.valid, data, amt);
            data_len -= amt;
            data += amt;
            xtb.valid += amt;
        }
        if (xtb.valid < xtb.size)
            return;

        // at this point, the buffer is full

        // if we can process some data, go for it; make sure
        // we leave an 'fsize's worth of data, though
        if (xtb.start + xtb.fsize < xtb.valid) {
            int amount = (xtb.valid - xtb.fsize) - xtb.start;
            int n;
            assert(amount > 0);
            n = stb_compress_chunk(xtb.buffer, xtb.buffer + xtb.start, xtb.buffer + xtb.valid,
                amount, &xtb.pending_literals, xtb.chash, xtb.hashmask);
            xtb.start += n;
        }

        assert(xtb.start + xtb.fsize >= xtb.valid);
        // at this point, our future size is too small, so we
        // need to flush some history. we, in fact, flush exactly
        // one window's worth of history

        {
            int flush = xtb.window;
            assert(xtb.start >= flush);
            assert(xtb.valid >= flush);

            // if 'pending literals' extends back into the shift region,
            // write them out
            if (xtb.start - xtb.pending_literals < flush) {
                outliterals(xtb.buffer + xtb.start - xtb.pending_literals, xtb.pending_literals);
                xtb.pending_literals = 0;
            }

            // now shift the window
            memmove(xtb.buffer, xtb.buffer + flush, xtb.valid - flush);
            xtb.start -= flush;
            xtb.valid -= flush;

            for (i = 0; i <= xtb.hashmask; ++i)
                if (xtb.chash[i] < xtb.buffer + flush)
                    xtb.chash[i] = NULL;
                else
                    xtb.chash[i] -= flush;
        }
        // and now that we've made room for more data, go back to the top
    }
}

int stb_compress_stream_start(FILE* f)
{
    stb__out = NULL;
    stb__outfile = f;

    if (f == NULL)
        return 0;

    if (!stb_compress_streaming_start())
        return 0;

    return 1;
}

void stb_compress_stream_end(int close)
{
    stb_compress_streaming_end();
    if (close && stb__outfile) {
        fclose(stb__outfile);
    }
}

#endif // STB_DEFINE

//////////////////////////////////////////////////////////////////////////////
//
//  File abstraction... tired of not having this... we can write
//  compressors to be layers over these that auto-close their children.


typedef struct stbfile
{
    int (*getbyte)(struct stbfile*);  // -1 on EOF
    unsigned int (*getdata)(struct stbfile*, void* block, unsigned int len);

    int (*putbyte)(struct stbfile*, int byte);
    unsigned int (*putdata)(struct stbfile*, void* block, unsigned int len);

    unsigned int (*size)(struct stbfile*);

    unsigned int (*tell)(struct stbfile*);
    void (*backpatch)(struct stbfile*, unsigned int tell, void* block, unsigned int len);

    void (*close)(struct stbfile*);

    FILE* f;  // file to fread/fwrite
    unsigned char* buffer; // input/output buffer
    unsigned char* indata, * inend; // input buffer
    union {
        int various;
        void* ptr;
    };
} stbfile;

STB_EXTERN unsigned int stb_getc(stbfile* f); // read
STB_EXTERN int stb_putc(stbfile* f, int ch); // write
STB_EXTERN unsigned int stb_getdata(stbfile* f, void* buffer, unsigned int len); // read
STB_EXTERN unsigned int stb_putdata(stbfile* f, void* buffer, unsigned int len); // write
STB_EXTERN unsigned int stb_tell(stbfile* f); // read
STB_EXTERN unsigned int stb_size(stbfile* f); // read/write
STB_EXTERN void stb_backpatch(stbfile* f, unsigned int tell, void* buffer, unsigned int len); // write

#ifdef STB_DEFINE

unsigned int stb_getc(stbfile* f) { return f->getbyte(f); }
int stb_putc(stbfile* f, int ch) { return f->putbyte(f, ch); }

unsigned int stb_getdata(stbfile* f, void* buffer, unsigned int len)
{
    return f->getdata(f, buffer, len);
}
unsigned int stb_putdata(stbfile* f, void* buffer, unsigned int len)
{
    return f->putdata(f, buffer, len);
}
void stb_close(stbfile* f)
{
    f->close(f);
    free(f);
}
unsigned int stb_tell(stbfile* f) { return f->tell(f); }
unsigned int stb_size(stbfile* f) { return f->size(f); }
void stb_backpatch(stbfile* f, unsigned int tell, void* buffer, unsigned int len)
{
    f->backpatch(f, tell, buffer, len);
}

// FILE * implementation
static int stb__fgetbyte(stbfile* f) { return fgetc(f->f); }
static int stb__fputbyte(stbfile* f, int ch) { return fputc(ch, f->f) == 0; }
static unsigned int stb__fgetdata(stbfile* f, void* buffer, unsigned int len) { return (unsigned int)fread(buffer, 1, len, f->f); }
static unsigned int stb__fputdata(stbfile* f, void* buffer, unsigned int len) { return (unsigned int)fwrite(buffer, 1, len, f->f); }
static unsigned int stb__fsize(stbfile* f) { return (unsigned int)stb_filelen(f->f); }
static unsigned int stb__ftell(stbfile* f) { return (unsigned int)ftell(f->f); }
static void stb__fbackpatch(stbfile* f, unsigned int where, void* buffer, unsigned int len)
{
    fseek(f->f, where, SEEK_SET);
    fwrite(buffer, 1, len, f->f);
    fseek(f->f, 0, SEEK_END);
}
static void         stb__fclose(stbfile* f) { fclose(f->f); }

stbfile* stb_openf(FILE* f)
{
    stbfile m = { stb__fgetbyte, stb__fgetdata,
                  stb__fputbyte, stb__fputdata,
                  stb__fsize, stb__ftell, stb__fbackpatch, stb__fclose,
                  0,0,0, };
    stbfile* z = (stbfile*)malloc(sizeof(*z));
    if (z) {
        *z = m;
        z->f = f;
    }
    return z;
}

static int stb__nogetbyte(stbfile* f) { assert(0); return -1; }
static unsigned int stb__nogetdata(stbfile* f, void* buffer, unsigned int len) { assert(0); return 0; }
static int stb__noputbyte(stbfile* f, int ch) { assert(0); return 0; }
static unsigned int stb__noputdata(stbfile* f, void* buffer, unsigned int len) { assert(0); return 0; }
static void stb__nobackpatch(stbfile* f, unsigned int where, void* buffer, unsigned int len) { assert(0); }

static int stb__bgetbyte(stbfile* s)
{
    if (s->indata < s->inend)
        return *s->indata++;
    else
        return -1;
}

static unsigned int stb__bgetdata(stbfile* s, void* buffer, unsigned int len)
{
    if (s->indata + len > s->inend)
        len = (unsigned int)(s->inend - s->indata);
    memcpy(buffer, s->indata, len);
    s->indata += len;
    return len;
}
static unsigned int stb__bsize(stbfile* s) { return (unsigned int)(s->inend - s->buffer); }
static unsigned int stb__btell(stbfile* s) { return (unsigned int)(s->indata - s->buffer); }

static void stb__bclose(stbfile* s)
{
    if (s->various)
        free(s->buffer);
}

stbfile* stb_open_inbuffer(void* buffer, unsigned int len)
{
    stbfile m = { stb__bgetbyte, stb__bgetdata,
                  stb__noputbyte, stb__noputdata,
                  stb__bsize, stb__btell, stb__nobackpatch, stb__bclose };
    stbfile* z = (stbfile*)malloc(sizeof(*z));
    if (z) {
        *z = m;
        z->buffer = (unsigned char*)buffer;
        z->indata = z->buffer;
        z->inend = z->indata + len;
    }
    return z;
}

stbfile* stb_open_inbuffer_free(void* buffer, unsigned int len)
{
    stbfile* z = stb_open_inbuffer(buffer, len);
    if (z)
        z->various = 1; // free
    return z;
}

#ifndef STB_VERSION
// if we've been cut-and-pasted elsewhere, you get a limited
// version of stb_open, without the 'k' flag and utf8 support
static void stb__fclose2(stbfile* f)
{
    fclose(f->f);
}

stbfile* stb_open(char* filename, char* mode)
{
    FILE* f = stb_p_fopen(filename, mode);
    stbfile* s;
    if (f == NULL) return NULL;
    s = stb_openf(f);
    if (s)
        s->close = stb__fclose2;
    return s;
}
#else
// the full version depends on some code in stb.h; this
// also includes the memory buffer output format implemented with stb_arr
static void stb__fclose2(stbfile* f)
{
    stb_fclose(f->f, f->various);
}

stbfile* stb_open(char* filename, char* mode)
{
    FILE* f = stb_fopen(filename, mode[0] == 'k' ? mode + 1 : mode);
    stbfile* s;
    if (f == NULL) return NULL;
    s = stb_openf(f);
    if (s) {
        s->close = stb__fclose2;
        s->various = mode[0] == 'k' ? stb_keep_if_different : stb_keep_yes;
    }
    return s;
}

static int stb__aputbyte(stbfile* f, int ch)
{
    stb_arr_push(f->buffer, ch);
    return 1;
}
static unsigned int stb__aputdata(stbfile* f, void* data, unsigned int len)
{
    memcpy(stb_arr_addn(f->buffer, (int)len), data, len);
    return len;
}
static unsigned int stb__asize(stbfile* f) { return stb_arr_len(f->buffer); }
static void stb__abackpatch(stbfile* f, unsigned int where, void* data, unsigned int len)
{
    memcpy(f->buffer + where, data, len);
}
static void stb__aclose(stbfile* f)
{
    *(unsigned char**)f->ptr = f->buffer;
}

stbfile* stb_open_outbuffer(unsigned char** update_on_close)
{
    stbfile m = { stb__nogetbyte, stb__nogetdata,
                  stb__aputbyte, stb__aputdata,
                  stb__asize, stb__asize, stb__abackpatch, stb__aclose };
    stbfile* z = (stbfile*)malloc(sizeof(*z));
    if (z) {
        z->ptr = update_on_close;
        *z = m;
    }
    return z;
}
#endif
#endif


//////////////////////////////////////////////////////////////////////////////
//
//  Arithmetic coder... based on cbloom's notes on the subject, should be
//  less code than a huffman code.

typedef struct
{
    unsigned int range_low;
    unsigned int range_high;
    unsigned int code, range; // decode
    int buffered_u8;
    int pending_ffs;
    stbfile* output;
} stb_arith;

STB_EXTERN void stb_arith_init_encode(stb_arith* a, stbfile* out);
STB_EXTERN void stb_arith_init_decode(stb_arith* a, stbfile* in);
STB_EXTERN stbfile* stb_arith_encode_close(stb_arith* a);
STB_EXTERN stbfile* stb_arith_decode_close(stb_arith* a);

STB_EXTERN void stb_arith_encode(stb_arith* a, unsigned int totalfreq, unsigned int freq, unsigned int cumfreq);
STB_EXTERN void stb_arith_encode_log2(stb_arith* a, unsigned int totalfreq2, unsigned int freq, unsigned int cumfreq);
STB_EXTERN unsigned int stb_arith_decode_value(stb_arith* a, unsigned int totalfreq);
STB_EXTERN void stb_arith_decode_advance(stb_arith* a, unsigned int totalfreq, unsigned int freq, unsigned int cumfreq);
STB_EXTERN unsigned int stb_arith_decode_value_log2(stb_arith* a, unsigned int totalfreq2);
STB_EXTERN void stb_arith_decode_advance_log2(stb_arith* a, unsigned int totalfreq2, unsigned int freq, unsigned int cumfreq);

STB_EXTERN void stb_arith_encode_byte(stb_arith* a, int byte);
STB_EXTERN int  stb_arith_decode_byte(stb_arith* a);

// this is a memory-inefficient way of doing things, but it's
// fast(?) and simple
typedef struct
{
    unsigned short cumfreq;
    unsigned short samples;
} stb_arith_symstate_item;

typedef struct
{
    int num_sym;
    unsigned int pow2;
    int countdown;
    stb_arith_symstate_item data[1];
} stb_arith_symstate;

#ifdef STB_DEFINE
void stb_arith_init_encode(stb_arith* a, stbfile* out)
{
    a->range_low = 0;
    a->range_high = 0xffffffff;
    a->pending_ffs = -1; // means no buffered character currently, to speed up normal case
    a->output = out;
}

static void stb__arith_carry(stb_arith* a)
{
    int i;
    assert(a->pending_ffs != -1); // can't carry with no data
    stb_putc(a->output, a->buffered_u8);
    for (i = 0; i < a->pending_ffs; ++i)
        stb_putc(a->output, 0);
}

static void stb__arith_putbyte(stb_arith* a, int byte)
{
    if (a->pending_ffs) {
        if (a->pending_ffs == -1) { // means no buffered data; encoded for fast path efficiency
            if (byte == 0xff)
                stb_putc(a->output, byte); // just write it immediately
            else {
                a->buffered_u8 = byte;
                a->pending_ffs = 0;
            }
        }
        else if (byte == 0xff) {
            ++a->pending_ffs;
        }
        else {
            int i;
            stb_putc(a->output, a->buffered_u8);
            for (i = 0; i < a->pending_ffs; ++i)
                stb_putc(a->output, 0xff);
        }
    }
    else if (byte == 0xff) {
        ++a->pending_ffs;
    }
    else {
        // fast path
        stb_putc(a->output, a->buffered_u8);
        a->buffered_u8 = byte;
    }
}

static void stb__arith_flush(stb_arith* a)
{
    if (a->pending_ffs >= 0) {
        int i;
        stb_putc(a->output, a->buffered_u8);
        for (i = 0; i < a->pending_ffs; ++i)
            stb_putc(a->output, 0xff);
    }
}

static void stb__renorm_encoder(stb_arith* a)
{
    stb__arith_putbyte(a, a->range_low >> 24);
    a->range_low <<= 8;
    a->range_high = (a->range_high << 8) | 0xff;
}

static void stb__renorm_decoder(stb_arith* a)
{
    int c = stb_getc(a->output);
    a->code = (a->code << 8) + (c >= 0 ? c : 0); // if EOF, insert 0
}

void stb_arith_encode(stb_arith* a, unsigned int totalfreq, unsigned int freq, unsigned int cumfreq)
{
    unsigned int range = a->range_high - a->range_low;
    unsigned int old = a->range_low;
    range /= totalfreq;
    a->range_low += range * cumfreq;
    a->range_high = a->range_low + range * freq;
    if (a->range_low < old)
        stb__arith_carry(a);
    while (a->range_high - a->range_low < 0x1000000)
        stb__renorm_encoder(a);
}

void stb_arith_encode_log2(stb_arith* a, unsigned int totalfreq2, unsigned int freq, unsigned int cumfreq)
{
    unsigned int range = a->range_high - a->range_low;
    unsigned int old = a->range_low;
    range >>= totalfreq2;
    a->range_low += range * cumfreq;
    a->range_high = a->range_low + range * freq;
    if (a->range_low < old)
        stb__arith_carry(a);
    while (a->range_high - a->range_low < 0x1000000)
        stb__renorm_encoder(a);
}

unsigned int stb_arith_decode_value(stb_arith* a, unsigned int totalfreq)
{
    unsigned int freqsize = a->range / totalfreq;
    unsigned int z = a->code / freqsize;
    return z >= totalfreq ? totalfreq - 1 : z;
}

void stb_arith_decode_advance(stb_arith* a, unsigned int totalfreq, unsigned int freq, unsigned int cumfreq)
{
    unsigned int freqsize = a->range / totalfreq; // @OPTIMIZE, share with above divide somehow?
    a->code -= freqsize * cumfreq;
    a->range = freqsize * freq;
    while (a->range < 0x1000000)
        stb__renorm_decoder(a);
}

unsigned int stb_arith_decode_value_log2(stb_arith* a, unsigned int totalfreq2)
{
    unsigned int freqsize = a->range >> totalfreq2;
    unsigned int z = a->code / freqsize;
    return z >= (1U << totalfreq2) ? (1U << totalfreq2) - 1 : z;
}

void stb_arith_decode_advance_log2(stb_arith* a, unsigned int totalfreq2, unsigned int freq, unsigned int cumfreq)
{
    unsigned int freqsize = a->range >> totalfreq2;
    a->code -= freqsize * cumfreq;
    a->range = freqsize * freq;
    while (a->range < 0x1000000)
        stb__renorm_decoder(a);
}

stbfile* stb_arith_encode_close(stb_arith* a)
{
    // put exactly as many bytes as we'll read, so we can turn on/off arithmetic coding in a stream
    stb__arith_putbyte(a, a->range_low >> 24);
    stb__arith_putbyte(a, a->range_low >> 16);
    stb__arith_putbyte(a, a->range_low >> 8);
    stb__arith_putbyte(a, a->range_low >> 0);
    stb__arith_flush(a);
    return a->output;
}

stbfile* stb_arith_decode_close(stb_arith* a)
{
    return a->output;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
//                         Threads
//

#ifndef _WIN32
#ifdef STB_THREADS
#error "threads not implemented except for Windows"
#endif
#endif

// call this function to free any global variables for memory testing
STB_EXTERN void stb_thread_cleanup(void);

typedef void* (*stb_thread_func)(void*);

// do not rely on these types, this is an implementation detail.
// compare against STB_THREAD_NULL and ST_SEMAPHORE_NULL
typedef void* stb_thread;
typedef void* stb_semaphore;
typedef void* stb_mutex;
typedef struct stb__sync* stb_sync;

#define STB_SEMAPHORE_NULL    NULL
#define STB_THREAD_NULL       NULL
#define STB_MUTEX_NULL        NULL
#define STB_SYNC_NULL         NULL

// get the number of processors (limited to those in the affinity mask for this process).
STB_EXTERN int stb_processor_count(void);
// force to run on a single core -- needed for RDTSC to work, e.g. for iprof
STB_EXTERN void stb_force_uniprocessor(void);

// stb_work functions: queue up work to be done by some worker threads

// set number of threads to serve the queue; you can change this on the fly,
// but if you decrease it, it won't decrease until things currently on the
// queue are finished
STB_EXTERN void          stb_work_numthreads(int n);
// set maximum number of units in the queue; you can only set this BEFORE running any work functions
STB_EXTERN int           stb_work_maxunits(int n);
// enqueue some work to be done (can do this from any thread, or even from a piece of work);
// return value of f is stored in *return_code if non-NULL
STB_EXTERN int           stb_work(stb_thread_func f, void* d, volatile void** return_code);
// as above, but stb_sync_reach is called on 'rel' after work is complete
STB_EXTERN int           stb_work_reach(stb_thread_func f, void* d, volatile void** return_code, stb_sync rel);


// necessary to call this when using volatile to order writes/reads
STB_EXTERN void          stb_barrier(void);

// support for independent queues with their own threads

typedef struct stb__workqueue stb_workqueue;

STB_EXTERN stb_workqueue* stb_workq_new(int numthreads, int max_units);
STB_EXTERN stb_workqueue* stb_workq_new_flags(int numthreads, int max_units, int no_add_mutex, int no_remove_mutex);
STB_EXTERN void          stb_workq_delete(stb_workqueue* q);
STB_EXTERN void          stb_workq_numthreads(stb_workqueue* q, int n);
STB_EXTERN int           stb_workq(stb_workqueue* q, stb_thread_func f, void* d, volatile void** return_code);
STB_EXTERN int           stb_workq_reach(stb_workqueue* q, stb_thread_func f, void* d, volatile void** return_code, stb_sync rel);
STB_EXTERN int           stb_workq_length(stb_workqueue* q);

STB_EXTERN stb_thread    stb_create_thread(stb_thread_func f, void* d);
STB_EXTERN stb_thread    stb_create_thread2(stb_thread_func f, void* d, volatile void** return_code, stb_semaphore rel);
STB_EXTERN void          stb_destroy_thread(stb_thread t);

STB_EXTERN stb_semaphore stb_sem_new(int max_val);
STB_EXTERN stb_semaphore stb_sem_new_extra(int max_val, int start_val);
STB_EXTERN void          stb_sem_delete(stb_semaphore s);
STB_EXTERN void          stb_sem_waitfor(stb_semaphore s);
STB_EXTERN void          stb_sem_release(stb_semaphore s);

STB_EXTERN stb_mutex     stb_mutex_new(void);
STB_EXTERN void          stb_mutex_delete(stb_mutex m);
STB_EXTERN void          stb_mutex_begin(stb_mutex m);
STB_EXTERN void          stb_mutex_end(stb_mutex m);

STB_EXTERN stb_sync      stb_sync_new(void);
STB_EXTERN void          stb_sync_delete(stb_sync s);
STB_EXTERN int           stb_sync_set_target(stb_sync s, int count);
STB_EXTERN void          stb_sync_reach_and_wait(stb_sync s);    // wait for 'target' reachers
STB_EXTERN int           stb_sync_reach(stb_sync s);

typedef struct stb__threadqueue stb_threadqueue;
#define STB_THREADQ_DYNAMIC   0
STB_EXTERN stb_threadqueue* stb_threadq_new(int item_size, int num_items, int many_add, int many_remove);
STB_EXTERN void             stb_threadq_delete(stb_threadqueue* tq);
STB_EXTERN int              stb_threadq_get(stb_threadqueue* tq, void* output);
STB_EXTERN void             stb_threadq_get_block(stb_threadqueue* tq, void* output);
STB_EXTERN int              stb_threadq_add(stb_threadqueue* tq, void* input);
// can return FALSE if STB_THREADQ_DYNAMIC and attempt to grow fails
STB_EXTERN int              stb_threadq_add_block(stb_threadqueue* tq, void* input);

#ifdef STB_THREADS
#ifdef STB_DEFINE

typedef struct
{
    stb_thread_func f;
    void* d;
    volatile void** return_val;
    stb_semaphore sem;
} stb__thread;

// this is initialized along all possible paths to create threads, therefore
// it's always initialized before any other threads are create, therefore
// it's free of races AS LONG AS you only create threads through stb_*
static stb_mutex stb__threadmutex, stb__workmutex;

static void stb__threadmutex_init(void)
{
    if (stb__threadmutex == STB_SEMAPHORE_NULL) {
        stb__threadmutex = stb_mutex_new();
        stb__workmutex = stb_mutex_new();
    }
}

#ifdef STB_THREAD_TEST
volatile float stb__t1 = 1, stb__t2;

static void stb__wait(int n)
{
    float z = 0;
    int i;
    for (i = 0; i < n; ++i)
        z += 1 / (stb__t1 + i);
    stb__t2 = z;
}
#else
#define stb__wait(x)
#endif

#ifdef _WIN32

// avoid including windows.h -- note that our definitions aren't
// exactly the same (we don't define the security descriptor struct)
// so if you want to include windows.h, make sure you do it first.
#include <process.h>

#ifndef _WINDOWS_  // check windows.h guard
#define STB__IMPORT   STB_EXTERN __declspec(dllimport)
#define STB__DW       unsigned long

STB__IMPORT int     __stdcall TerminateThread(void*, STB__DW);
STB__IMPORT void* __stdcall CreateSemaphoreA(void* sec, long, long, char*);
STB__IMPORT int     __stdcall CloseHandle(void*);
STB__IMPORT STB__DW __stdcall WaitForSingleObject(void*, STB__DW);
STB__IMPORT int     __stdcall ReleaseSemaphore(void*, long, long*);
STB__IMPORT void    __stdcall Sleep(STB__DW);
#endif

// necessary to call this when using volatile to order writes/reads
void stb_barrier(void)
{
#ifdef MemoryBarrier
    MemoryBarrier();
#else
    long temp;
    __asm xchg temp, eax;
#endif
}

static void stb__thread_run(void* t)
{
    void* res;
    stb__thread info = *(stb__thread*)t;
    free(t);
    res = info.f(info.d);
    if (info.return_val)
        *info.return_val = res;
    if (info.sem != STB_SEMAPHORE_NULL)
        stb_sem_release(info.sem);
}

static stb_thread stb_create_thread_raw(stb_thread_func f, void* d, volatile void** return_code, stb_semaphore rel)
{
#ifdef _MT
#if defined(STB_FASTMALLOC) && !defined(STB_FASTMALLOC_ITS_OKAY_I_ONLY_MALLOC_IN_ONE_THREAD)
    stb_fatal("Error! Cannot use STB_FASTMALLOC with threads.\n");
    return STB_THREAD_NULL;
#else
    unsigned long id;
    stb__thread* data = (stb__thread*)malloc(sizeof(*data));
    if (!data) return NULL;
    stb__threadmutex_init();
    data->f = f;
    data->d = d;
    data->return_val = return_code;
    data->sem = rel;
    id = _beginthread(stb__thread_run, 0, data);
    if (id == -1) return NULL;
    return (void*)id;
#endif
#else
#ifdef STB_NO_STB_STRINGS
    stb_fatal("Invalid compilation");
#else
    stb_fatal("Must compile mult-threaded to use stb_thread/stb_work.");
#endif
    return NULL;
#endif
}

// trivial win32 wrappers
void          stb_destroy_thread(stb_thread t) { TerminateThread(t, 0); }
stb_semaphore stb_sem_new(int maxv) { return CreateSemaphoreA(NULL, 0, maxv, NULL); }
stb_semaphore stb_sem_new_extra(int maxv, int start) { return CreateSemaphoreA(NULL, start, maxv, NULL); }
void          stb_sem_delete(stb_semaphore s) { if (s != NULL) CloseHandle(s); }
void          stb_sem_waitfor(stb_semaphore s) { WaitForSingleObject(s, 0xffffffff); } // INFINITE
void          stb_sem_release(stb_semaphore s) { ReleaseSemaphore(s, 1, NULL); }
static void   stb__thread_sleep(int ms) { Sleep(ms); }

#ifndef _WINDOWS_
STB__IMPORT int __stdcall GetProcessAffinityMask(void*, STB__DW*, STB__DW*);
STB__IMPORT void* __stdcall GetCurrentProcess(void);
STB__IMPORT int __stdcall SetProcessAffinityMask(void*, STB__DW);
#endif

int stb_processor_count(void)
{
    unsigned long proc, sys;
    GetProcessAffinityMask(GetCurrentProcess(), &proc, &sys);
    return stb_bitcount(proc);
}

void stb_force_uniprocessor(void)
{
    unsigned long proc, sys;
    GetProcessAffinityMask(GetCurrentProcess(), &proc, &sys);
    if (stb_bitcount(proc) > 1) {
        int z;
        for (z = 0; z < 32; ++z)
            if (proc & (1 << z))
                break;
        if (z < 32) {
            proc = 1 << z;
            SetProcessAffinityMask(GetCurrentProcess(), proc);
        }
    }
}

#ifdef _WINDOWS_
#define STB_MUTEX_NATIVE
void* stb_mutex_new(void)
{
    CRITICAL_SECTION* p = (CRITICAL_SECTION*)malloc(sizeof(*p));
    if (p)
#if _WIN32_WINNT >= 0x0500
        InitializeCriticalSectionAndSpinCount(p, 500);
#else
        InitializeCriticalSection(p);
#endif
    return p;
}

void stb_mutex_delete(void* p)
{
    if (p) {
        DeleteCriticalSection((CRITICAL_SECTION*)p);
        free(p);
    }
}

void stb_mutex_begin(void* p)
{
    stb__wait(500);
    if (p)
        EnterCriticalSection((CRITICAL_SECTION*)p);
}

void stb_mutex_end(void* p)
{
    if (p)
        LeaveCriticalSection((CRITICAL_SECTION*)p);
    stb__wait(500);
}
#endif // _WINDOWS_

#if 0
// for future reference,
// InterlockedCompareExchange for x86:
int cas64_mp(void* dest, void* xcmp, void* xxchg) {
    __asm
    {
        mov             esi, [xxchg]; exchange
        mov             ebx, [esi + 0]
        mov             ecx, [esi + 4]

        mov             esi, [xcmp]; comparand
        mov             eax, [esi + 0]
        mov             edx, [esi + 4]

        mov             edi, [dest]; destination
        lock cmpxchg8b[edi]
        jz              yyyy;

        mov[esi + 0], eax;
        mov[esi + 4], edx;

    yyyy:
        xor eax, eax;
        setz    al;
    };

    inline unsigned __int64 _InterlockedCompareExchange64(volatile unsigned __int64* dest
        , unsigned __int64 exchange
        , unsigned __int64 comperand)
    {
        //value returned in eax::edx
        __asm {
            lea esi, comperand;
            lea edi, exchange;

            mov eax, [esi];
            mov edx, 4[esi];
            mov ebx, [edi];
            mov ecx, 4[edi];
            mov esi, dest;
            lock CMPXCHG8B[esi];
        }
#endif // #if 0

#endif // _WIN32

        stb_thread stb_create_thread2(stb_thread_func f, void* d, volatile void** return_code, stb_semaphore rel)
        {
            return stb_create_thread_raw(f, d, return_code, rel);
        }

        stb_thread stb_create_thread(stb_thread_func f, void* d)
        {
            return stb_create_thread2(f, d, NULL, STB_SEMAPHORE_NULL);
        }

        // mutex implemented by wrapping semaphore
#ifndef STB_MUTEX_NATIVE
        stb_mutex stb_mutex_new(void) { return stb_sem_new_extra(1, 1); }
        void      stb_mutex_delete(stb_mutex m) { stb_sem_delete(m); }
        void      stb_mutex_begin(stb_mutex m) { stb__wait(500); if (m) stb_sem_waitfor(m); }
        void      stb_mutex_end(stb_mutex m) { if (m) stb_sem_release(m); stb__wait(500); }
#endif

        // thread merge operation
        struct stb__sync
        {
            int target;  // target number of threads to hit it
            int sofar;   // total threads that hit it
            int waiting; // total threads waiting

            stb_mutex start;   // mutex to prevent starting again before finishing previous
            stb_mutex mutex;   // mutex while tweaking state
            stb_semaphore release; // semaphore wake up waiting threads
               // we have to wake them up one at a time, rather than using a single release
               // call, because win32 semaphores don't let you dynamically change the max count!
        };

        stb_sync stb_sync_new(void)
        {
            stb_sync s = (stb_sync)malloc(sizeof(*s));
            if (!s) return s;

            s->target = s->sofar = s->waiting = 0;
            s->mutex = stb_mutex_new();
            s->start = stb_mutex_new();
            s->release = stb_sem_new(1);
            if (s->mutex == STB_MUTEX_NULL || s->release == STB_SEMAPHORE_NULL || s->start == STB_MUTEX_NULL) {
                stb_mutex_delete(s->mutex);
                stb_mutex_delete(s->mutex);
                stb_sem_delete(s->release);
                free(s);
                return NULL;
            }
            return s;
        }

        void stb_sync_delete(stb_sync s)
        {
            if (s->waiting) {
                // it's bad to delete while there are threads waiting!
                // shall we wait for them to reach, or just bail? just bail
                assert(0);
            }
            stb_mutex_delete(s->mutex);
            stb_mutex_delete(s->release);
            free(s);
        }

        int stb_sync_set_target(stb_sync s, int count)
        {
            // don't allow setting a target until the last one is fully released;
            // note that this can lead to inefficient pipelining, and maybe we'd
            // be better off ping-ponging between two internal syncs?
            // I tried seeing how often this happened using TryEnterCriticalSection
            // and could _never_ get it to happen in imv(stb), even with more threads
            // than processors. So who knows!
            stb_mutex_begin(s->start);

            // this mutex is pointless, since it's not valid for threads
            // to call reach() before anyone calls set_target() anyway
            stb_mutex_begin(s->mutex);

            assert(s->target == 0); // enforced by start mutex
            s->target = count;
            s->sofar = 0;
            s->waiting = 0;
            stb_mutex_end(s->mutex);
            return STB_TRUE;
        }

        void stb__sync_release(stb_sync s)
        {
            if (s->waiting)
                stb_sem_release(s->release);
            else {
                s->target = 0;
                stb_mutex_end(s->start);
            }
        }

        int stb_sync_reach(stb_sync s)
        {
            int n;
            stb_mutex_begin(s->mutex);
            assert(s->sofar < s->target);
            n = ++s->sofar; // record this value to avoid possible race if we did 'return s->sofar';
            if (s->sofar == s->target)
                stb__sync_release(s);
            stb_mutex_end(s->mutex);
            return n;
        }

        void stb_sync_reach_and_wait(stb_sync s)
        {
            stb_mutex_begin(s->mutex);
            assert(s->sofar < s->target);
            ++s->sofar;
            if (s->sofar == s->target) {
                stb__sync_release(s);
                stb_mutex_end(s->mutex);
            }
            else {
                ++s->waiting; // we're waiting, so one more waiter
                stb_mutex_end(s->mutex); // release the mutex to other threads

                stb_sem_waitfor(s->release); // wait for merge completion

                stb_mutex_begin(s->mutex); // on merge completion, grab the mutex
                --s->waiting; // we're done waiting
                stb__sync_release(s);    // restart the next waiter
                stb_mutex_end(s->mutex); // and now we're done
                // this ends the same as the first case, but it's a lot
                // clearer to understand without sharing the code
            }
        }

        struct stb__threadqueue
        {
            stb_mutex add, remove;
            stb_semaphore nonempty, nonfull;
            int head_blockers;  // number of threads blocking--used to know whether to release(avail)
            int tail_blockers;
            int head, tail, array_size, growable;
            int item_size;
            char* data;
        };

        static int stb__tq_wrap(volatile stb_threadqueue * z, int p)
        {
            if (p == z->array_size)
                return p - z->array_size;
            else
                return p;
        }

        int stb__threadq_get_raw(stb_threadqueue * tq2, void* output, int block)
        {
            volatile stb_threadqueue* tq = (volatile stb_threadqueue*)tq2;
            if (tq->head == tq->tail && !block) return 0;

            stb_mutex_begin(tq->remove);

            while (tq->head == tq->tail) {
                if (!block) {
                    stb_mutex_end(tq->remove);
                    return 0;
                }
                ++tq->head_blockers;
                stb_mutex_end(tq->remove);

                stb_sem_waitfor(tq->nonempty);

                stb_mutex_begin(tq->remove);
                --tq->head_blockers;
            }

            memcpy(output, tq->data + tq->head * tq->item_size, tq->item_size);
            stb_barrier();
            tq->head = stb__tq_wrap(tq, tq->head + 1);

            stb_sem_release(tq->nonfull);
            if (tq->head_blockers) // can't check if actually non-empty due to race?
                stb_sem_release(tq->nonempty); // if there are other blockers, wake one

            stb_mutex_end(tq->remove);
            return STB_TRUE;
        }

        int stb__threadq_grow(volatile stb_threadqueue * tq)
        {
            int n;
            char* p;
            assert(tq->remove != STB_MUTEX_NULL); // must have this to allow growth!
            stb_mutex_begin(tq->remove);

            n = tq->array_size * 2;
            p = (char*)realloc(tq->data, n * tq->item_size);
            if (p == NULL) {
                stb_mutex_end(tq->remove);
                stb_mutex_end(tq->add);
                return STB_FALSE;
            }
            if (tq->tail < tq->head) {
                memcpy(p + tq->array_size * tq->item_size, p, tq->tail * tq->item_size);
                tq->tail += tq->array_size;
            }
            tq->data = p;
            tq->array_size = n;

            stb_mutex_end(tq->remove);
            return STB_TRUE;
        }

        int stb__threadq_add_raw(stb_threadqueue * tq2, void* input, int block)
        {
            int tail, pos;
            volatile stb_threadqueue* tq = (volatile stb_threadqueue*)tq2;
            stb_mutex_begin(tq->add);
            for (;;) {
                pos = tq->tail;
                tail = stb__tq_wrap(tq, pos + 1);
                if (tail != tq->head) break;

                // full
                if (tq->growable) {
                    if (!stb__threadq_grow(tq)) {
                        stb_mutex_end(tq->add);
                        return STB_FALSE; // out of memory
                    }
                }
                else if (!block) {
                    stb_mutex_end(tq->add);
                    return STB_FALSE;
                }
                else {
                    ++tq->tail_blockers;
                    stb_mutex_end(tq->add);

                    stb_sem_waitfor(tq->nonfull);

                    stb_mutex_begin(tq->add);
                    --tq->tail_blockers;
                }
            }
            memcpy(tq->data + tq->item_size * pos, input, tq->item_size);
            stb_barrier();
            tq->tail = tail;
            stb_sem_release(tq->nonempty);
            if (tq->tail_blockers) // can't check if actually non-full due to race?
                stb_sem_release(tq->nonfull);
            stb_mutex_end(tq->add);
            return STB_TRUE;
        }

        int stb_threadq_length(stb_threadqueue * tq2)
        {
            int a, b, n;
            volatile stb_threadqueue* tq = (volatile stb_threadqueue*)tq2;
            stb_mutex_begin(tq->add);
            a = tq->head;
            b = tq->tail;
            n = tq->array_size;
            stb_mutex_end(tq->add);
            if (a > b) b += n;
            return b - a;
        }

        int stb_threadq_get(stb_threadqueue * tq, void* output)
        {
            return stb__threadq_get_raw(tq, output, STB_FALSE);
        }

        void stb_threadq_get_block(stb_threadqueue * tq, void* output)
        {
            stb__threadq_get_raw(tq, output, STB_TRUE);
        }

        int stb_threadq_add(stb_threadqueue * tq, void* input)
        {
            return stb__threadq_add_raw(tq, input, STB_FALSE);
        }

        int stb_threadq_add_block(stb_threadqueue * tq, void* input)
        {
            return stb__threadq_add_raw(tq, input, STB_TRUE);
        }

        void stb_threadq_delete(stb_threadqueue * tq)
        {
            if (tq) {
                free(tq->data);
                stb_mutex_delete(tq->add);
                stb_mutex_delete(tq->remove);
                stb_sem_delete(tq->nonempty);
                stb_sem_delete(tq->nonfull);
                free(tq);
            }
        }

#define STB_THREADQUEUE_DYNAMIC   0
        stb_threadqueue* stb_threadq_new(int item_size, int num_items, int many_add, int many_remove)
        {
            int error = 0;
            stb_threadqueue* tq = (stb_threadqueue*)malloc(sizeof(*tq));
            if (tq == NULL) return NULL;

            if (num_items == STB_THREADQUEUE_DYNAMIC) {
                tq->growable = STB_TRUE;
                num_items = 32;
            }
            else
                tq->growable = STB_FALSE;

            tq->item_size = item_size;
            tq->array_size = num_items + 1;

            tq->add = tq->remove = STB_MUTEX_NULL;
            tq->nonempty = tq->nonfull = STB_SEMAPHORE_NULL;
            tq->data = NULL;
            if (many_add)
            {
                tq->add = stb_mutex_new(); if (tq->add == STB_MUTEX_NULL) goto error;
            }
            if (many_remove || tq->growable)
            {
                tq->remove = stb_mutex_new(); if (tq->remove == STB_MUTEX_NULL) goto error;
            }
            tq->nonempty = stb_sem_new(1); if (tq->nonempty == STB_SEMAPHORE_NULL) goto error;
            tq->nonfull = stb_sem_new(1); if (tq->nonfull == STB_SEMAPHORE_NULL) goto error;
            tq->data = (char*)malloc(tq->item_size * tq->array_size);
            if (tq->data == NULL) goto error;

            tq->head = tq->tail = 0;
            tq->head_blockers = tq->tail_blockers = 0;

            return tq;

        error:
            stb_threadq_delete(tq);
            return NULL;
        }

        typedef struct
        {
            stb_thread_func f;
            void* d;
            volatile void** retval;
            stb_sync sync;
        } stb__workinfo;

        //static volatile stb__workinfo *stb__work;

        struct stb__workqueue
        {
            int numthreads;
            stb_threadqueue* tq;
        };

        static stb_workqueue* stb__work_global;

        static void* stb__thread_workloop(void* p)
        {
            volatile stb_workqueue* q = (volatile stb_workqueue*)p;
            for (;;) {
                void* z;
                stb__workinfo w;
                stb_threadq_get_block(q->tq, &w);
                if (w.f == NULL) // null work is a signal to end the thread
                    return NULL;
                z = w.f(w.d);
                if (w.retval) { stb_barrier(); *w.retval = z; }
                if (w.sync != STB_SYNC_NULL) stb_sync_reach(w.sync);
            }
        }

        stb_workqueue* stb_workq_new(int num_threads, int max_units)
        {
            return stb_workq_new_flags(num_threads, max_units, 0, 0);
        }

        stb_workqueue* stb_workq_new_flags(int numthreads, int max_units, int no_add_mutex, int no_remove_mutex)
        {
            stb_workqueue* q = (stb_workqueue*)malloc(sizeof(*q));
            if (q == NULL) return NULL;
            q->tq = stb_threadq_new(sizeof(stb__workinfo), max_units, !no_add_mutex, !no_remove_mutex);
            if (q->tq == NULL) { free(q); return NULL; }
            q->numthreads = 0;
            stb_workq_numthreads(q, numthreads);
            return q;
        }

        void stb_workq_delete(stb_workqueue * q)
        {
            while (stb_workq_length(q) != 0)
                stb__thread_sleep(1);
            stb_threadq_delete(q->tq);
            free(q);
        }

        static int stb__work_maxitems = STB_THREADQUEUE_DYNAMIC;

        static void stb_work_init(int num_threads)
        {
            if (stb__work_global == NULL) {
                stb__threadmutex_init();
                stb_mutex_begin(stb__workmutex);
                stb_barrier();
                if (*(stb_workqueue* volatile*)&stb__work_global == NULL)
                    stb__work_global = stb_workq_new(num_threads, stb__work_maxitems);
                stb_mutex_end(stb__workmutex);
            }
        }

        static int stb__work_raw(stb_workqueue * q, stb_thread_func f, void* d, volatile void** return_code, stb_sync rel)
        {
            stb__workinfo w;
            if (q == NULL) {
                stb_work_init(1);
                q = stb__work_global;
            }
            w.f = f;
            w.d = d;
            w.retval = return_code;
            w.sync = rel;
            return stb_threadq_add(q->tq, &w);
        }

        int stb_workq_length(stb_workqueue * q)
        {
            return stb_threadq_length(q->tq);
        }

        int stb_workq(stb_workqueue * q, stb_thread_func f, void* d, volatile void** return_code)
        {
            if (f == NULL) return 0;
            return stb_workq_reach(q, f, d, return_code, NULL);
        }

        int stb_workq_reach(stb_workqueue * q, stb_thread_func f, void* d, volatile void** return_code, stb_sync rel)
        {
            if (f == NULL) return 0;
            return stb__work_raw(q, f, d, return_code, rel);
        }

        static void stb__workq_numthreads(stb_workqueue * q, int n)
        {
            while (q->numthreads < n) {
                stb_create_thread(stb__thread_workloop, q);
                ++q->numthreads;
            }
            while (q->numthreads > n) {
                stb__work_raw(q, NULL, NULL, NULL, NULL);
                --q->numthreads;
            }
        }

        void stb_workq_numthreads(stb_workqueue * q, int n)
        {
            stb_mutex_begin(stb__threadmutex);
            stb__workq_numthreads(q, n);
            stb_mutex_end(stb__threadmutex);
        }

        int stb_work_maxunits(int n)
        {
            if (stb__work_global == NULL) {
                stb__work_maxitems = n;
                stb_work_init(1);
            }
            return stb__work_maxitems;
        }

        int stb_work(stb_thread_func f, void* d, volatile void** return_code)
        {
            return stb_workq(stb__work_global, f, d, return_code);
        }

        int stb_work_reach(stb_thread_func f, void* d, volatile void** return_code, stb_sync rel)
        {
            return stb_workq_reach(stb__work_global, f, d, return_code, rel);
        }

        void stb_work_numthreads(int n)
        {
            if (stb__work_global == NULL)
                stb_work_init(n);
            else
                stb_workq_numthreads(stb__work_global, n);
        }
#endif // STB_DEFINE


        //////////////////////////////////////////////////////////////////////////////
        //
        // Background disk I/O
        //
        //

#define STB_BGIO_READ_ALL   (-1)
        STB_EXTERN int stb_bgio_read(char* filename, int offset, int len, stb_uchar * *result, int* olen);
        STB_EXTERN int stb_bgio_readf(FILE * f, int offset, int len, stb_uchar * *result, int* olen);
        STB_EXTERN int stb_bgio_read_to(char* filename, int offset, int len, stb_uchar * buffer, int* olen);
        STB_EXTERN int stb_bgio_readf_to(FILE * f, int offset, int len, stb_uchar * buffer, int* olen);

        typedef struct
        {
            int have_data;
            int is_valid;
            int is_dir;
            time_t filetime;
            stb_int64 filesize;
        } stb_bgstat;

        STB_EXTERN int stb_bgio_stat(char* filename, stb_bgstat * result);

#ifdef STB_DEFINE

        static stb_workqueue* stb__diskio;
        static stb_mutex stb__diskio_mutex;

        void stb_thread_cleanup(void)
        {
            if (stb__work_global) stb_workq_delete(stb__work_global); stb__work_global = NULL;
            if (stb__threadmutex) stb_mutex_delete(stb__threadmutex); stb__threadmutex = NULL;
            if (stb__workmutex)   stb_mutex_delete(stb__workmutex);   stb__workmutex = NULL;
            if (stb__diskio)      stb_workq_delete(stb__diskio);      stb__diskio = NULL;
            if (stb__diskio_mutex)stb_mutex_delete(stb__diskio_mutex); stb__diskio_mutex = NULL;
        }


        typedef struct
        {
            char* filename;
            FILE* f;
            int offset;
            int len;

            stb_bgstat* stat_out;
            stb_uchar* output;
            stb_uchar** result;
            int* len_output;
            int* flag;
        } stb__disk_command;

#define STB__MAX_DISK_COMMAND 100
        static stb__disk_command stb__dc_queue[STB__MAX_DISK_COMMAND];
        static int stb__dc_offset;

        void stb__io_init(void)
        {
            if (!stb__diskio) {
                stb__threadmutex_init();
                stb_mutex_begin(stb__threadmutex);
                stb_barrier();
                if (*(stb_thread* volatile*)&stb__diskio == NULL) {
                    stb__diskio_mutex = stb_mutex_new();
                    // use many threads so OS can try to schedule seeks
                    stb__diskio = stb_workq_new_flags(16, STB__MAX_DISK_COMMAND, STB_FALSE, STB_FALSE);
                }
                stb_mutex_end(stb__threadmutex);
            }
        }

        static void* stb__io_error(stb__disk_command * dc)
        {
            if (dc->len_output) *dc->len_output = 0;
            if (dc->result) *dc->result = NULL;
            if (dc->flag) *dc->flag = -1;
            return NULL;
        }

        static void* stb__io_task(void* p)
        {
            stb__disk_command* dc = (stb__disk_command*)p;
            int len;
            FILE* f;
            stb_uchar* buf;

            if (dc->stat_out) {
                struct _stati64 s;
                if (!_stati64(dc->filename, &s)) {
                    dc->stat_out->filesize = s.st_size;
                    dc->stat_out->filetime = s.st_mtime;
                    dc->stat_out->is_dir = s.st_mode & _S_IFDIR;
                    dc->stat_out->is_valid = (s.st_mode & _S_IFREG) || dc->stat_out->is_dir;
                }
                else
                    dc->stat_out->is_valid = 0;
                stb_barrier();
                dc->stat_out->have_data = 1;
                free(dc->filename);
                return 0;
            }
            if (dc->f) {
#ifdef WIN32
                f = _fdopen(_dup(_fileno(dc->f)), "rb");
#else
                f = fdopen(dup(fileno(dc->f)), "rb");
#endif
                if (!f)
                    return stb__io_error(dc);
            }
            else {
                f = fopen(dc->filename, "rb");
                free(dc->filename);
                if (!f)
                    return stb__io_error(dc);
            }

            len = dc->len;
            if (len < 0) {
                fseek(f, 0, SEEK_END);
                len = ftell(f) - dc->offset;
            }

            if (fseek(f, dc->offset, SEEK_SET)) {
                fclose(f);
                return stb__io_error(dc);
            }

            if (dc->output)
                buf = dc->output;
            else {
                buf = (stb_uchar*)malloc(len);
                if (buf == NULL) {
                    fclose(f);
                    return stb__io_error(dc);
                }
            }

            len = fread(buf, 1, len, f);
            fclose(f);
            if (dc->len_output) *dc->len_output = len;
            if (dc->result) *dc->result = buf;
            if (dc->flag) *dc->flag = 1;

            return NULL;
        }

        int stb__io_add(char* fname, FILE * f, int off, int len, stb_uchar * out, stb_uchar * *result, int* olen, int* flag, stb_bgstat * stat)
        {
            int res;
            stb__io_init();
            // do memory allocation outside of mutex
            if (fname) fname = stb_p_strdup(fname);
            stb_mutex_begin(stb__diskio_mutex);
            {
                stb__disk_command* dc = &stb__dc_queue[stb__dc_offset];
                dc->filename = fname;
                dc->f = f;
                dc->offset = off;
                dc->len = len;
                dc->output = out;
                dc->result = result;
                dc->len_output = olen;
                dc->flag = flag;
                dc->stat_out = stat;
                res = stb_workq(stb__diskio, stb__io_task, dc, NULL);
                if (res)
                    stb__dc_offset = (stb__dc_offset + 1 == STB__MAX_DISK_COMMAND ? 0 : stb__dc_offset + 1);
            }
            stb_mutex_end(stb__diskio_mutex);
            return res;
        }

        int stb_bgio_read(char* filename, int offset, int len, stb_uchar * *result, int* olen)
        {
            return stb__io_add(filename, NULL, offset, len, NULL, result, olen, NULL, NULL);
        }

        int stb_bgio_readf(FILE * f, int offset, int len, stb_uchar * *result, int* olen)
        {
            return stb__io_add(NULL, f, offset, len, NULL, result, olen, NULL, NULL);
        }

        int stb_bgio_read_to(char* filename, int offset, int len, stb_uchar * buffer, int* olen)
        {
            return stb__io_add(filename, NULL, offset, len, buffer, NULL, olen, NULL, NULL);
        }

        int stb_bgio_readf_to(FILE * f, int offset, int len, stb_uchar * buffer, int* olen)
        {
            return stb__io_add(NULL, f, offset, len, buffer, NULL, olen, NULL, NULL);
        }

        STB_EXTERN int stb_bgio_stat(char* filename, stb_bgstat * result)
        {
            result->have_data = 0;
            return stb__io_add(filename, NULL, 0, 0, 0, NULL, 0, NULL, result);
        }
#endif
#endif



        //////////////////////////////////////////////////////////////////////////////
        //
        //                         Fast malloc implementation
        //
        //   This is a clone of TCMalloc, but without the thread support.
        //      1. large objects are allocated directly, page-aligned
        //      2. small objects are allocated in homogeonous heaps, 0 overhead
        //
        //   We keep an allocation table for pages a la TCMalloc. This would
        //   require 4MB for the entire address space, but we only allocate
        //   the parts that are in use. The overhead from using homogenous heaps
        //   everywhere is 3MB. (That is, if you allocate 1 object of each size,
        //   you'll use 3MB.)

#if defined(STB_DEFINE) && ((defined(_WIN32) && !defined(_M_AMD64)) || defined(STB_FASTMALLOC))

#ifdef _WIN32
#ifndef _WINDOWS_
#ifndef STB__IMPORT
#define STB__IMPORT   STB_EXTERN __declspec(dllimport)
#define STB__DW       unsigned long
#endif
        STB__IMPORT void* __stdcall VirtualAlloc(void* p, unsigned long size, unsigned long type, unsigned long protect);
        STB__IMPORT int   __stdcall VirtualFree(void* p, unsigned long size, unsigned long freetype);
#endif
#define stb__alloc_pages_raw(x)     (stb_uint32) VirtualAlloc(NULL, (x), 0x3000, 0x04)
#define stb__dealloc_pages_raw(p)   VirtualFree((void *) p, 0, 0x8000)
#else
#error "Platform not currently supported"
#endif

        typedef struct stb__span
        {
            int                start, len;
            struct stb__span* next, * prev;
            void* first_free;
            unsigned short     list; // 1..256 free; 257..511 sizeclass; 0=large block
            short              allocations; // # outstanding allocations for sizeclass
        } stb__span;  // 24

        static stb__span** stb__span_for_page;
        static int stb__firstpage, stb__lastpage;
        static void stb__update_page_range(int first, int last)
        {
            stb__span** sfp;
            int i, f, l;
            if (first >= stb__firstpage && last <= stb__lastpage) return;
            if (stb__span_for_page == NULL) {
                f = first;
                l = f + stb_max(last - f, 16384);
                l = stb_min(l, 1 << 20);
            }
            else if (last > stb__lastpage) {
                f = stb__firstpage;
                l = f + (stb__lastpage - f) * 2;
                l = stb_clamp(last, l, 1 << 20);
            }
            else {
                l = stb__lastpage;
                f = l - (l - stb__firstpage) * 2;
                f = stb_clamp(f, 0, first);
            }
            sfp = (stb__span**)stb__alloc_pages_raw(sizeof(void*) * (l - f));
            for (i = f; i < stb__firstpage; ++i) sfp[i - f] = NULL;
            for (; i < stb__lastpage; ++i) sfp[i - f] = stb__span_for_page[i - stb__firstpage];
            for (; i < l; ++i) sfp[i - f] = NULL;
            if (stb__span_for_page) stb__dealloc_pages_raw(stb__span_for_page);
            stb__firstpage = f;
            stb__lastpage = l;
            stb__span_for_page = sfp;
        }

        static stb__span* stb__span_free = NULL;
        static stb__span* stb__span_first, * stb__span_end;
        static stb__span* stb__span_alloc(void)
        {
            stb__span* s = stb__span_free;
            if (s)
                stb__span_free = s->next;
            else {
                if (!stb__span_first) {
                    stb__span_first = (stb__span*)stb__alloc_pages_raw(65536);
                    if (stb__span_first == NULL) return NULL;
                    stb__span_end = stb__span_first + (65536 / sizeof(stb__span));
                }
                s = stb__span_first++;
                if (stb__span_first == stb__span_end) stb__span_first = NULL;
            }
            return s;
        }

        static stb__span* stb__spanlist[512];

        static void stb__spanlist_unlink(stb__span * s)
        {
            if (s->prev)
                s->prev->next = s->next;
            else {
                int n = s->list;
                assert(stb__spanlist[n] == s);
                stb__spanlist[n] = s->next;
            }
            if (s->next)
                s->next->prev = s->prev;
            s->next = s->prev = NULL;
            s->list = 0;
        }

        static void stb__spanlist_add(int n, stb__span * s)
        {
            s->list = n;
            s->next = stb__spanlist[n];
            s->prev = NULL;
            stb__spanlist[n] = s;
            if (s->next) s->next->prev = s;
        }

#define stb__page_shift       12
#define stb__page_size        (1 << stb__page_shift)
#define stb__page_number(x)   ((x) >> stb__page_shift)
#define stb__page_address(x)  ((x) << stb__page_shift)

        static void stb__set_span_for_page(stb__span * s)
        {
            int i;
            for (i = 0; i < s->len; ++i)
                stb__span_for_page[s->start + i - stb__firstpage] = s;
        }

        static stb__span* stb__coalesce(stb__span * a, stb__span * b)
        {
            assert(a->start + a->len == b->start);
            if (a->list) stb__spanlist_unlink(a);
            if (b->list) stb__spanlist_unlink(b);
            a->len += b->len;
            b->len = 0;
            b->next = stb__span_free;
            stb__span_free = b;
            stb__set_span_for_page(a);
            return a;
        }

        static void stb__free_span(stb__span * s)
        {
            stb__span* n = NULL;
            if (s->start > stb__firstpage) {
                n = stb__span_for_page[s->start - 1 - stb__firstpage];
                if (n && n->allocations == -2 && n->start + n->len == s->start) s = stb__coalesce(n, s);
            }
            if (s->start + s->len < stb__lastpage) {
                n = stb__span_for_page[s->start + s->len - stb__firstpage];
                if (n && n->allocations == -2 && s->start + s->len == n->start) s = stb__coalesce(s, n);
            }
            s->allocations = -2;
            stb__spanlist_add(s->len > 256 ? 256 : s->len, s);
        }

        static stb__span* stb__alloc_pages(int num)
        {
            stb__span* s = stb__span_alloc();
            int p;
            if (!s) return NULL;
            p = stb__alloc_pages_raw(num << stb__page_shift);
            if (p == 0) { s->next = stb__span_free; stb__span_free = s; return 0; }
            assert(stb__page_address(stb__page_number(p)) == p);
            p = stb__page_number(p);
            stb__update_page_range(p, p + num);
            s->start = p;
            s->len = num;
            s->next = NULL;
            s->prev = NULL;
            stb__set_span_for_page(s);
            return s;
        }

        static stb__span* stb__alloc_span(int pagecount)
        {
            int i;
            stb__span* p = NULL;
            for (i = pagecount; i < 256; ++i)
                if (stb__spanlist[i]) {
                    p = stb__spanlist[i];
                    break;
                }
            if (!p) {
                p = stb__spanlist[256];
                while (p && p->len < pagecount)
                    p = p->next;
            }
            if (!p) {
                p = stb__alloc_pages(pagecount < 16 ? 16 : pagecount);
                if (p == NULL) return 0;
            }
            else
                stb__spanlist_unlink(p);

            if (p->len > pagecount) {
                stb__span* q = stb__span_alloc();
                if (q) {
                    q->start = p->start + pagecount;
                    q->len = p->len - pagecount;
                    p->len = pagecount;
                    for (i = 0; i < q->len; ++i)
                        stb__span_for_page[q->start + i - stb__firstpage] = q;
                    stb__spanlist_add(q->len > 256 ? 256 : q->len, q);
                }
            }
            return p;
        }

#define STB__MAX_SMALL_SIZE     32768
#define STB__MAX_SIZE_CLASSES   256

        static unsigned char stb__class_base[32];
        static unsigned char stb__class_shift[32];
        static unsigned char stb__pages_for_class[STB__MAX_SIZE_CLASSES];
        static           int stb__size_for_class[STB__MAX_SIZE_CLASSES];

        stb__span* stb__get_nonempty_sizeclass(int c)
        {
            int s = c + 256, i, size, tsize; // remap to span-list index
            char* z;
            void* q;
            stb__span* p = stb__spanlist[s];
            if (p) {
                if (p->first_free) return p; // fast path: it's in the first one in list
                for (p = p->next; p; p = p->next)
                    if (p->first_free) {
                        // move to front for future queries
                        stb__spanlist_unlink(p);
                        stb__spanlist_add(s, p);
                        return p;
                    }
            }
            // no non-empty ones, so allocate a new one
            p = stb__alloc_span(stb__pages_for_class[c]);
            if (!p) return NULL;
            // create the free list up front
            size = stb__size_for_class[c];
            tsize = stb__pages_for_class[c] << stb__page_shift;
            i = 0;
            z = (char*)stb__page_address(p->start);
            q = NULL;
            while (i + size <= tsize) {
                *(void**)z = q; q = z;
                z += size;
                i += size;
            }
            p->first_free = q;
            p->allocations = 0;
            stb__spanlist_add(s, p);
            return p;
        }

        static int stb__sizeclass(size_t sz)
        {
            int z = stb_log2_floor(sz); // -1 below to group e.g. 13,14,15,16 correctly
            return stb__class_base[z] + ((sz - 1) >> stb__class_shift[z]);
        }

        static void stb__init_sizeclass(void)
        {
            int i, size, overhead;
            int align_shift = 2;  // allow 4-byte and 12-byte blocks as well, vs. TCMalloc
            int next_class = 1;
            int last_log = 0;

            for (i = 0; i < align_shift; i++) {
                stb__class_base[i] = next_class;
                stb__class_shift[i] = align_shift;
            }

            for (size = 1 << align_shift; size <= STB__MAX_SMALL_SIZE; size += 1 << align_shift) {
                i = stb_log2_floor(size);
                if (i > last_log) {
                    if (size == 16) ++align_shift; // switch from 4-byte to 8-byte alignment
                    else if (size >= 128 && align_shift < 8) ++align_shift;
                    stb__class_base[i] = next_class - ((size - 1) >> align_shift);
                    stb__class_shift[i] = align_shift;
                    last_log = i;
                }
                stb__size_for_class[next_class++] = size;
            }

            for (i = 1; i <= STB__MAX_SMALL_SIZE; ++i)
                assert(i <= stb__size_for_class[stb__sizeclass(i)]);

            overhead = 0;
            for (i = 1; i < next_class; i++) {
                int s = stb__size_for_class[i];
                size = stb__page_size;
                while (size % s > size >> 3)
                    size += stb__page_size;
                stb__pages_for_class[i] = (unsigned char)(size >> stb__page_shift);
                overhead += size;
            }
            assert(overhead < (4 << 20)); // make sure it's under 4MB of overhead
        }

#ifdef STB_DEBUG
#define stb__smemset(a,b,c)  memset((void *) a, b, c)
#elif defined(STB_FASTMALLOC_INIT)
#define stb__smemset(a,b,c)  memset((void *) a, b, c)
#else
#define stb__smemset(a,b,c)
#endif
        void* stb_smalloc(size_t sz)
        {
            stb__span* s;
            if (sz == 0) return NULL;
            if (stb__size_for_class[1] == 0) stb__init_sizeclass();
            if (sz > STB__MAX_SMALL_SIZE) {
                s = stb__alloc_span((sz + stb__page_size - 1) >> stb__page_shift);
                if (s == NULL) return NULL;
                s->list = 0;
                s->next = s->prev = NULL;
                s->allocations = -32767;
                stb__smemset(stb__page_address(s->start), 0xcd, (sz + 3) & ~3);
                return (void*)stb__page_address(s->start);
            }
            else {
                void* p;
                int c = stb__sizeclass(sz);
                s = stb__spanlist[256 + c];
                if (!s || !s->first_free)
                    s = stb__get_nonempty_sizeclass(c);
                if (s == NULL) return NULL;
                p = s->first_free;
                s->first_free = *(void**)p;
                ++s->allocations;
                stb__smemset(p, 0xcd, sz);
                return p;
            }
        }

        int stb_ssize(void* p)
        {
            stb__span* s;
            if (p == NULL) return 0;
            s = stb__span_for_page[stb__page_number((stb_uint)p) - stb__firstpage];
            if (s->list >= 256) {
                return stb__size_for_class[s->list - 256];
            }
            else {
                assert(s->list == 0);
                return s->len << stb__page_shift;
            }
        }

        void stb_sfree(void* p)
        {
            stb__span* s;
            if (p == NULL) return;
            s = stb__span_for_page[stb__page_number((stb_uint)p) - stb__firstpage];
            if (s->list >= 256) {
                stb__smemset(p, 0xfe, stb__size_for_class[s->list - 256]);
                *(void**)p = s->first_free;
                s->first_free = p;
                if (--s->allocations == 0) {
                    stb__spanlist_unlink(s);
                    stb__free_span(s);
                }
            }
            else {
                assert(s->list == 0);
                stb__smemset(p, 0xfe, stb_ssize(p));
                stb__free_span(s);
            }
        }

        void* stb_srealloc(void* p, size_t sz)
        {
            size_t cur_size;
            if (p == NULL) return stb_smalloc(sz);
            if (sz == 0) { stb_sfree(p); return NULL; }
            cur_size = stb_ssize(p);
            if (sz > cur_size || sz <= (cur_size >> 1)) {
                void* q;
                if (sz > cur_size && sz < (cur_size << 1)) sz = cur_size << 1;
                q = stb_smalloc(sz); if (q == NULL) return NULL;
                memcpy(q, p, sz < cur_size ? sz : cur_size);
                stb_sfree(p);
                return q;
            }
            return p;
        }

        void* stb_scalloc(size_t n, size_t sz)
        {
            void* p;
            if (n == 0 || sz == 0) return NULL;
            if (stb_log2_ceil(n) + stb_log2_ceil(n) >= 32) return NULL;
            p = stb_smalloc(n * sz);
            if (p) memset(p, 0, n * sz);
            return p;
        }

        char* stb_sstrdup(char* s)
        {
            int n = strlen(s);
            char* p = (char*)stb_smalloc(n + 1);
            if (p) stb_p_strcpy_s(p, n + 1, s);
            return p;
        }
#endif // STB_DEFINE



        //////////////////////////////////////////////////////////////////////////////
        //
        //                         Source code constants
        //
        // This is a trivial system to let you specify constants in source code,
        // then while running you can change the constants.
        //
        // Note that you can't wrap the #defines, because we need to know their
        // names. So we provide a pre-wrapped version without 'STB_' for convenience;
        // to request it, #define STB_CONVENIENT_H, yielding:
        //       KI -- integer
        //       KU -- unsigned integer
        //       KF -- float
        //       KD -- double
        //       KS -- string constant
        //
        // Defaults to functioning in debug build, not in release builds.
        // To force on, define STB_ALWAYS_H

#ifdef STB_CONVENIENT_H
#define KI(x) STB_I(x)
#define KU(x) STB_UI(x)
#define KF(x) STB_F(x)
#define KD(x) STB_D(x)
#define KS(x) STB_S(x)
#endif

        STB_EXTERN void stb_source_path(char* str);
#ifdef STB_DEFINE
        char* stb__source_path;
        void stb_source_path(char* path)
        {
            stb__source_path = path;
        }

        char* stb__get_sourcefile_path(char* file)
        {
            static char filebuf[512];
            if (stb__source_path) {
                stb_p_sprintf(filebuf stb_p_size(sizeof(filebuf)), "%s/%s", stb__source_path, file);
                if (stb_fexists(filebuf)) return filebuf;
            }

            if (stb_fexists(file)) return file;

            stb_p_sprintf(filebuf stb_p_size(sizeof(filebuf)), "../%s", file);
            if (!stb_fexists(filebuf)) return filebuf;

            return file;
        }
#endif

#define STB_F(x)   ((float) STB_H(x))
#define STB_UI(x)  ((unsigned int) STB_I(x))

#if !defined(STB_DEBUG) && !defined(STB_ALWAYS_H)
#define STB_D(x)   ((double) (x))
#define STB_I(x)   ((int) (x))
#define STB_S(x)   ((char *) (x))
#else
#define STB_D(x)   stb__double_constant(__FILE__, __LINE__-1, (x))
#define STB_I(x)   stb__int_constant(__FILE__, __LINE__-1, (x))
#define STB_S(x)   stb__string_constant(__FILE__, __LINE__-1, (x))

        STB_EXTERN double stb__double_constant(char* file, int line, double x);
        STB_EXTERN int    stb__int_constant(char* file, int line, int x);
        STB_EXTERN char* stb__string_constant(char* file, int line, char* str);

#ifdef STB_DEFINE

        enum
        {
            STB__CTYPE_int,
            STB__CTYPE_uint,
            STB__CTYPE_float,
            STB__CTYPE_double,
            STB__CTYPE_string,
        };

        typedef struct
        {
            int line;
            int type;
            union {
                int ival;
                double dval;
                char* sval;
            };
        } stb__Entry;

        typedef struct
        {
            stb__Entry* entries;
            char* filename;
            time_t timestamp;
            char** file_data;
            int file_len;
            unsigned short* line_index;
        } stb__FileEntry;

        static void stb__constant_parse(stb__FileEntry * f, int i)
        {
            char* s;
            int n;
            if (!stb_arr_valid(f->entries, i)) return;
            n = f->entries[i].line;
            if (n >= f->file_len) return;
            s = f->file_data[n];
            switch (f->entries[i].type) {
            case STB__CTYPE_float:
                while (*s) {
                    if (!strncmp(s, "STB_D(", 6)) { s += 6; goto matched_float; }
                    if (!strncmp(s, "STB_F(", 6)) { s += 6; goto matched_float; }
                    if (!strncmp(s, "KD(", 3)) { s += 3; goto matched_float; }
                    if (!strncmp(s, "KF(", 3)) { s += 3; goto matched_float; }
                    ++s;
                }
                break;
            matched_float:
                f->entries[i].dval = strtod(s, NULL);
                break;
            case STB__CTYPE_int:
                while (*s) {
                    if (!strncmp(s, "STB_I(", 6)) { s += 6; goto matched_int; }
                    if (!strncmp(s, "STB_UI(", 7)) { s += 7; goto matched_int; }
                    if (!strncmp(s, "KI(", 3)) { s += 3; goto matched_int; }
                    if (!strncmp(s, "KU(", 3)) { s += 3; goto matched_int; }
                    ++s;
                }
                break;
            matched_int: {
                int neg = 0;
                s = stb_skipwhite(s);
                while (*s == '-') { neg = !neg; s = stb_skipwhite(s + 1); } // handle '- - 5', pointlessly
                if (s[0] == '0' && tolower(s[1]) == 'x')
                    f->entries[i].ival = strtol(s, NULL, 16);
                else if (s[0] == '0')
                    f->entries[i].ival = strtol(s, NULL, 8);
                else
                    f->entries[i].ival = strtol(s, NULL, 10);
                if (neg) f->entries[i].ival = -f->entries[i].ival;
                break;
                }
            case STB__CTYPE_string:
                // @TODO
                break;
            }
        }

        static stb_sdict* stb__constant_file_hash;

        stb__Entry* stb__constant_get_entry(char* filename, int line, int type)
        {
            int i;
            stb__FileEntry* f;
            if (stb__constant_file_hash == NULL)
                stb__constant_file_hash = stb_sdict_new(STB_TRUE);
            f = (stb__FileEntry*)stb_sdict_get(stb__constant_file_hash, filename);
            if (f == NULL) {
                char* s = stb__get_sourcefile_path(filename);
                if (s == NULL || !stb_fexists(s)) return 0;
                f = (stb__FileEntry*)malloc(sizeof(*f));
                f->timestamp = stb_ftimestamp(s);
                f->file_data = stb_stringfile(s, &f->file_len);
                f->filename = stb_p_strdup(s); // cache the full path
                f->entries = NULL;
                f->line_index = 0;
                stb_arr_setlen(f->line_index, f->file_len);
                memset(f->line_index, 0xff, stb_arr_storage(f->line_index));
            }
            else {
                time_t t = stb_ftimestamp(f->filename);
                if (f->timestamp != t) {
                    f->timestamp = t;
                    free(f->file_data);
                    f->file_data = stb_stringfile(f->filename, &f->file_len);
                    stb_arr_setlen(f->line_index, f->file_len);
                    for (i = 0; i < stb_arr_len(f->entries); ++i)
                        stb__constant_parse(f, i);
                }
            }

            if (line >= f->file_len) return 0;

            if (f->line_index[line] >= stb_arr_len(f->entries)) {
                // need a new entry
                int n = stb_arr_len(f->entries);
                stb__Entry e;
                e.line = line;
                if (line < f->file_len)
                    f->line_index[line] = n;
                e.type = type;
                stb_arr_push(f->entries, e);
                stb__constant_parse(f, n);
            }
            return f->entries + f->line_index[line];
        }

        double stb__double_constant(char* file, int line, double x)
        {
            stb__Entry* e = stb__constant_get_entry(file, line, STB__CTYPE_float);
            if (!e) return x;
            return e->dval;
        }

        int    stb__int_constant(char* file, int line, int x)
        {
            stb__Entry* e = stb__constant_get_entry(file, line, STB__CTYPE_int);
            if (!e) return x;
            return e->ival;
        }

        char* stb__string_constant(char* file, int line, char* x)
        {
            stb__Entry* e = stb__constant_get_entry(file, line, STB__CTYPE_string);
            if (!e) return x;
            return e->sval;
        }

#endif // STB_DEFINE
#endif // !STB_DEBUG && !STB_ALWAYS_H

#undef STB_EXTERN
#endif // STB_INCLUDE_STB_H

        /*
        ------------------------------------------------------------------------------
        This software is available under 2 licenses -- choose whichever you prefer.
        ------------------------------------------------------------------------------
        ALTERNATIVE A - MIT License
        Copyright (c) 2017 Sean Barrett
        Permission is hereby granted, free of charge, to any person obtaining a copy of
        this software and associated documentation files (the "Software"), to deal in
        the Software without restriction, including without limitation the rights to
        use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
        of the Software, and to permit persons to whom the Software is furnished to do
        so, subject to the following conditions:
        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.
        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
        SOFTWARE.
        ------------------------------------------------------------------------------
        ALTERNATIVE B - Public Domain (www.unlicense.org)
        This is free and unencumbered software released into the public domain.
        Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
        software, either in source code form or as a compiled binary, for any purpose,
        commercial or non-commercial, and by any means.
        In jurisdictions that recognize copyright laws, the author or authors of this
        software dedicate any and all copyright interest in the software to the public
        domain. We make this dedication for the benefit of the public at large and to
        the detriment of our heirs and successors. We intend this dedication to be an
        overt act of relinquishment in perpetuity of all present and future rights to
        this software under copyright law.
        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
        ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
        WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
        ------------------------------------------------------------------------------
        */

#define STB_TRUETYPE_IMPLEMENTATION
		// stb_truetype.h - v1.26 - public domain
		// authored from 2009-2021 by Sean Barrett / RAD Game Tools
		//
		// =======================================================================
		//
		//    NO SECURITY GUARANTEE -- DO NOT USE THIS ON UNTRUSTED FONT FILES
		//
		// This library does no range checking of the offsets found in the file,
		// meaning an attacker can use it to read arbitrary memory.
		//
		// =======================================================================
		//
		//   This library processes TrueType files:
		//        parse files
		//        extract glyph metrics
		//        extract glyph shapes
		//        render glyphs to one-channel bitmaps with antialiasing (box filter)
		//        render glyphs to one-channel SDF bitmaps (signed-distance field/function)
		//
		//   Todo:
		//        non-MS cmaps
		//        crashproof on bad data
		//        hinting? (no longer patented)
		//        cleartype-style AA?
		//        optimize: use simple memory allocator for intermediates
		//        optimize: build edge-list directly from curves
		//        optimize: rasterize directly from curves?
		//
		// ADDITIONAL CONTRIBUTORS
		//
		//   Mikko Mononen: compound shape support, more cmap formats
		//   Tor Andersson: kerning, subpixel rendering
		//   Dougall Johnson: OpenType / Type 2 font handling
		//   Daniel Ribeiro Maciel: basic GPOS-based kerning
		//
		//   Misc other:
		//       Ryan Gordon
		//       Simon Glass
		//       github:IntellectualKitty
		//       Imanol Celaya
		//       Daniel Ribeiro Maciel
		//
		//   Bug/warning reports/fixes:
		//       "Zer" on mollyrocket       Fabian "ryg" Giesen   github:NiLuJe
		//       Cass Everitt               Martins Mozeiko       github:aloucks
		//       stoiko (Haemimont Games)   Cap Petschulat        github:oyvindjam
		//       Brian Hook                 Omar Cornut           github:vassvik
		//       Walter van Niftrik         Ryan Griege
		//       David Gow                  Peter LaValle
		//       David Given                Sergey Popov
		//       Ivan-Assen Ivanov          Giumo X. Clanjor
		//       Anthony Pesch              Higor Euripedes
		//       Johan Duparc               Thomas Fields
		//       Hou Qiming                 Derek Vinyard
		//       Rob Loach                  Cort Stratton
		//       Kenney Phillis Jr.         Brian Costabile
		//       Ken Voskuil (kaesve)
		//
		// VERSION HISTORY
		//
		//   1.26 (2021-08-28) fix broken rasterizer
		//   1.25 (2021-07-11) many fixes
		//   1.24 (2020-02-05) fix warning
		//   1.23 (2020-02-02) query SVG data for glyphs; query whole kerning table (but only kern not GPOS)
		//   1.22 (2019-08-11) minimize missing-glyph duplication; fix kerning if both 'GPOS' and 'kern' are defined
		//   1.21 (2019-02-25) fix warning
		//   1.20 (2019-02-07) PackFontRange skips missing codepoints; GetScaleFontVMetrics()
		//   1.19 (2018-02-11) GPOS kerning, STBTT_fmod
		//   1.18 (2018-01-29) add missing function
		//   1.17 (2017-07-23) make more arguments const; doc fix
		//   1.16 (2017-07-12) SDF support
		//   1.15 (2017-03-03) make more arguments const
		//   1.14 (2017-01-16) num-fonts-in-TTC function
		//   1.13 (2017-01-02) support OpenType fonts, certain Apple fonts
		//   1.12 (2016-10-25) suppress warnings about casting away const with -Wcast-qual
		//   1.11 (2016-04-02) fix unused-variable warning
		//   1.10 (2016-04-02) user-defined fabs(); rare memory leak; remove duplicate typedef
		//   1.09 (2016-01-16) warning fix; avoid crash on outofmem; use allocation userdata properly
		//   1.08 (2015-09-13) document stbtt_Rasterize(); fixes for vertical & horizontal edges
		//   1.07 (2015-08-01) allow PackFontRanges to accept arrays of sparse codepoints;
		//                     variant PackFontRanges to pack and render in separate phases;
		//                     fix stbtt_GetFontOFfsetForIndex (never worked for non-0 input?);
		//                     fixed an assert() bug in the new rasterizer
		//                     replace assert() with STBTT_assert() in new rasterizer
		//
		//   Full history can be found at the end of this file.
		//
		// LICENSE
		//
		//   See end of file for license information.
		//
		// USAGE
		//
		//   Include this file in whatever places need to refer to it. In ONE C/C++
		//   file, write:
		//      #define STB_TRUETYPE_IMPLEMENTATION
		//   before the #include of this file. This expands out the actual
		//   implementation into that C/C++ file.
		//
		//   To make the implementation private to the file that generates the implementation,
		//      #define STBTT_STATIC
		//
		//   Simple 3D API (don't ship this, but it's fine for tools and quick start)
		//           stbtt_BakeFontBitmap()               -- bake a font to a bitmap for use as texture
		//           stbtt_GetBakedQuad()                 -- compute quad to draw for a given char
		//
		//   Improved 3D API (more shippable):
		//           #include "stb_rect_pack.h"           -- optional, but you really want it
		//           stbtt_PackBegin()
		//           stbtt_PackSetOversampling()          -- for improved quality on small fonts
		//           stbtt_PackFontRanges()               -- pack and renders
		//           stbtt_PackEnd()
		//           stbtt_GetPackedQuad()
		//
		//   "Load" a font file from a memory buffer (you have to keep the buffer loaded)
		//           stbtt_InitFont()
		//           stbtt_GetFontOffsetForIndex()        -- indexing for TTC font collections
		//           stbtt_GetNumberOfFonts()             -- number of fonts for TTC font collections
		//
		//   Render a unicode codepoint to a bitmap
		//           stbtt_GetCodepointBitmap()           -- allocates and returns a bitmap
		//           stbtt_MakeCodepointBitmap()          -- renders into bitmap you provide
		//           stbtt_GetCodepointBitmapBox()        -- how big the bitmap must be
		//
		//   Character advance/positioning
		//           stbtt_GetCodepointHMetrics()
		//           stbtt_GetFontVMetrics()
		//           stbtt_GetFontVMetricsOS2()
		//           stbtt_GetCodepointKernAdvance()
		//
		//   Starting with version 1.06, the rasterizer was replaced with a new,
		//   faster and generally-more-precise rasterizer. The new rasterizer more
		//   accurately measures pixel coverage for anti-aliasing, except in the case
		//   where multiple shapes overlap, in which case it overestimates the AA pixel
		//   coverage. Thus, anti-aliasing of intersecting shapes may look wrong. If
		//   this turns out to be a problem, you can re-enable the old rasterizer with
		//        #define STBTT_RASTERIZER_VERSION 1
		//   which will incur about a 15% speed hit.
		//
		// ADDITIONAL DOCUMENTATION
		//
		//   Immediately after this block comment are a series of sample programs.
		//
		//   After the sample programs is the "header file" section. This section
		//   includes documentation for each API function.
		//
		//   Some important concepts to understand to use this library:
		//
		//      Codepoint
		//         Characters are defined by unicode codepoints, e.g. 65 is
		//         uppercase A, 231 is lowercase c with a cedilla, 0x7e30 is
		//         the hiragana for "ma".
		//
		//      Glyph
		//         A visual character shape (every codepoint is rendered as
		//         some glyph)
		//
		//      Glyph index
		//         A font-specific integer ID representing a glyph
		//
		//      Baseline
		//         Glyph shapes are defined relative to a baseline, which is the
		//         bottom of uppercase characters. Characters extend both above
		//         and below the baseline.
		//
		//      Current Point
		//         As you draw text to the screen, you keep track of a "current point"
		//         which is the origin of each character. The current point's vertical
		//         position is the baseline. Even "baked fonts" use this model.
		//
		//      Vertical Font Metrics
		//         The vertical qualities of the font, used to vertically position
		//         and space the characters. See docs for stbtt_GetFontVMetrics.
		//
		//      Font Size in Pixels or Points
		//         The preferred interface for specifying font sizes in stb_truetype
		//         is to specify how tall the font's vertical extent should be in pixels.
		//         If that sounds good enough, skip the next paragraph.
		//
		//         Most font APIs instead use "points", which are a common typographic
		//         measurement for describing font size, defined as 72 points per inch.
		//         stb_truetype provides a point API for compatibility. However, true
		//         "per inch" conventions don't make much sense on computer displays
		//         since different monitors have different number of pixels per
		//         inch. For example, Windows traditionally uses a convention that
		//         there are 96 pixels per inch, thus making 'inch' measurements have
		//         nothing to do with inches, and thus effectively defining a point to
		//         be 1.333 pixels. Additionally, the TrueType font data provides
		//         an explicit scale factor to scale a given font's glyphs to points,
		//         but the author has observed that this scale factor is often wrong
		//         for non-commercial fonts, thus making fonts scaled in points
		//         according to the TrueType spec incoherently sized in practice.
		//
		// DETAILED USAGE:
		//
		//  Scale:
		//    Select how high you want the font to be, in points or pixels.
		//    Call ScaleForPixelHeight or ScaleForMappingEmToPixels to compute
		//    a scale factor SF that will be used by all other functions.
		//
		//  Baseline:
		//    You need to select a y-coordinate that is the baseline of where
		//    your text will appear. Call GetFontBoundingBox to get the baseline-relative
		//    bounding box for all characters. SF*-y0 will be the distance in pixels
		//    that the worst-case character could extend above the baseline, so if
		//    you want the top edge of characters to appear at the top of the
		//    screen where y=0, then you would set the baseline to SF*-y0.
		//
		//  Current point:
		//    Set the current point where the first character will appear. The
		//    first character could extend left of the current point; this is font
		//    dependent. You can either choose a current point that is the leftmost
		//    point and hope, or add some padding, or check the bounding box or
		//    left-side-bearing of the first character to be displayed and set
		//    the current point based on that.
		//
		//  Displaying a character:
		//    Compute the bounding box of the character. It will contain signed values
		//    relative to <current_point, baseline>. I.e. if it returns x0,y0,x1,y1,
		//    then the character should be displayed in the rectangle from
		//    <current_point+SF*x0, baseline+SF*y0> to <current_point+SF*x1,baseline+SF*y1).
		//
		//  Advancing for the next character:
		//    Call GlyphHMetrics, and compute 'current_point += SF * advance'.
		//
		//
		// ADVANCED USAGE
		//
		//   Quality:
		//
		//    - Use the functions with Subpixel at the end to allow your characters
		//      to have subpixel positioning. Since the font is anti-aliased, not
		//      hinted, this is very import for quality. (This is not possible with
		//      baked fonts.)
		//
		//    - Kerning is now supported, and if you're supporting subpixel rendering
		//      then kerning is worth using to give your text a polished look.
		//
		//   Performance:
		//
		//    - Convert Unicode codepoints to glyph indexes and operate on the glyphs;
		//      if you don't do this, stb_truetype is forced to do the conversion on
		//      every call.
		//
		//    - There are a lot of memory allocations. We should modify it to take
		//      a temp buffer and allocate from the temp buffer (without freeing),
		//      should help performance a lot.
		//
		// NOTES
		//
		//   The system uses the raw data found in the .ttf file without changing it
		//   and without building auxiliary data structures. This is a bit inefficient
		//   on little-endian systems (the data is big-endian), but assuming you're
		//   caching the bitmaps or glyph shapes this shouldn't be a big deal.
		//
		//   It appears to be very hard to programmatically determine what font a
		//   given file is in a general way. I provide an API for this, but I don't
		//   recommend it.
		//
		//
		// PERFORMANCE MEASUREMENTS FOR 1.06:
		//
		//                      32-bit     64-bit
		//   Previous release:  8.83 s     7.68 s
		//   Pool allocations:  7.72 s     6.34 s
		//   Inline sort     :  6.54 s     5.65 s
		//   New rasterizer  :  5.63 s     5.00 s

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		////
		////  SAMPLE PROGRAMS
		////
		//
		//  Incomplete text-in-3d-api example, which draws quads properly aligned to be lossless.
		//  See "tests/truetype_demo_win32.c" for a complete version.
#if 0
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

		unsigned char ttf_buffer[1 << 20];
		unsigned char temp_bitmap[512 * 512];

		stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
		GLuint ftex;

		void my_stbtt_initfont(void)
		{
			fread(ttf_buffer, 1, 1 << 20, fopen("c:/windows/fonts/times.ttf", "rb"));
			stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
			// can free ttf_buffer at this point
			glGenTextures(1, &ftex);
			glBindTexture(GL_TEXTURE_2D, ftex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
			// can free temp_bitmap at this point
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		void my_stbtt_print(float x, float y, char* text)
		{
			// assume orthographic projection with units = screen pixels, origin at top left
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ftex);
			glBegin(GL_QUADS);
			while (*text) {
				if (*text >= 32 && *text < 128) {
					stbtt_aligned_quad q;
					stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9
					glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
					glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
					glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
					glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
				}
				++text;
			}
			glEnd();
		}
#endif
		//
		//
		//////////////////////////////////////////////////////////////////////////////
		//
		// Complete program (this compiles): get a single bitmap, print as ASCII art
		//
#if 0
#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

		char ttf_buffer[1 << 25];

		int main(int argc, char** argv)
		{
			stbtt_fontinfo font;
			unsigned char* bitmap;
			int w, h, i, j, c = (argc > 1 ? atoi(argv[1]) : 'a'), s = (argc > 2 ? atoi(argv[2]) : 20);

			fread(ttf_buffer, 1, 1 << 25, fopen(argc > 3 ? argv[3] : "c:/windows/fonts/arialbd.ttf", "rb"));

			stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
			bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0, 0);

			for (j = 0; j < h; ++j) {
				for (i = 0; i < w; ++i)
					putchar(" .:ioVM@"[bitmap[j * w + i] >> 5]);
				putchar('\n');
			}
			return 0;
		}
#endif
		//
		// Output:
		//
		//     .ii.
		//    @@@@@@.
		//   V@Mio@@o
		//   :i.  V@V
		//     :oM@@M
		//   :@@@MM@M
		//   @@o  o@M
		//  :@@.  M@M
		//   @@@o@@@@
		//   :M@@V:@@.
		//
		//////////////////////////////////////////////////////////////////////////////
		//
		// Complete program: print "Hello World!" banner, with bugs
		//
#if 0
		char buffer[24 << 20];
		unsigned char screen[20][79];

		int main(int arg, char** argv)
		{
			stbtt_fontinfo font;
			int i, j, ascent, baseline, ch = 0;
			float scale, xpos = 2; // leave a little padding in case the character extends left
			char* text = "Heljo World!"; // intentionally misspelled to show 'lj' brokenness

			fread(buffer, 1, 1000000, fopen("c:/windows/fonts/arialbd.ttf", "rb"));
			stbtt_InitFont(&font, buffer, 0);

			scale = stbtt_ScaleForPixelHeight(&font, 15);
			stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
			baseline = (int)(ascent * scale);

			while (text[ch]) {
				int advance, lsb, x0, y0, x1, y1;
				float x_shift = xpos - (float)floor(xpos);
				stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);
				stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
				stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, 79, scale, scale, x_shift, 0, text[ch]);
				// note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
				// because this API is really for baking character bitmaps into textures. if you want to render
				// a sequence of characters, you really need to render each bitmap to a temp buffer, then
				// "alpha blend" that into the working buffer
				xpos += (advance * scale);
				if (text[ch + 1])
					xpos += scale * stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch + 1]);
				++ch;
			}

			for (j = 0; j < 20; ++j) {
				for (i = 0; i < 78; ++i)
					putchar(" .:ioVM@"[screen[j][i] >> 5]);
				putchar('\n');
			}

			return 0;
		}
#endif


		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		////
		////   INTEGRATION WITH YOUR CODEBASE
		////
		////   The following sections allow you to supply alternate definitions
		////   of C library functions used by stb_truetype, e.g. if you don't
		////   link with the C runtime library.

#ifdef STB_TRUETYPE_IMPLEMENTATION
   // #define your own (u)stbtt_int8/16/32 before including to override this
#ifndef stbtt_uint8
		typedef unsigned char   stbtt_uint8;
		typedef signed   char   stbtt_int8;
		typedef unsigned short  stbtt_uint16;
		typedef signed   short  stbtt_int16;
		typedef unsigned int    stbtt_uint32;
		typedef signed   int    stbtt_int32;
#endif

		typedef char stbtt__check_size32[sizeof(stbtt_int32) == 4 ? 1 : -1];
		typedef char stbtt__check_size16[sizeof(stbtt_int16) == 2 ? 1 : -1];

		// e.g. #define your own STBTT_ifloor/STBTT_iceil() to avoid math.h
#ifndef STBTT_ifloor
#include <math.h>
#define STBTT_ifloor(x)   ((int) floor(x))
#define STBTT_iceil(x)    ((int) ceil(x))
#endif

#ifndef STBTT_sqrt
#include <math.h>
#define STBTT_sqrt(x)      sqrt(x)
#define STBTT_pow(x,y)     pow(x,y)
#endif

#ifndef STBTT_fmod
#include <math.h>
#define STBTT_fmod(x,y)    fmod(x,y)
#endif

#ifndef STBTT_cos
#include <math.h>
#define STBTT_cos(x)       cos(x)
#define STBTT_acos(x)      acos(x)
#endif

#ifndef STBTT_fabs
#include <math.h>
#define STBTT_fabs(x)      fabs(x)
#endif

// #define your own functions "STBTT_malloc" / "STBTT_free" to avoid malloc.h
#ifndef STBTT_malloc
#include <stdlib.h>
#define STBTT_malloc(x,u)  ((void)(u),malloc(x))
#define STBTT_free(x,u)    ((void)(u),free(x))
#endif

#ifndef STBTT_assert
#include <assert.h>
#define STBTT_assert(x)    assert(x)
#endif

#ifndef STBTT_strlen
#include <string.h>
#define STBTT_strlen(x)    strlen(x)
#endif

#ifndef STBTT_memcpy
#include <string.h>
#define STBTT_memcpy       memcpy
#define STBTT_memset       memset
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////
////   INTERFACE
////
////

#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#define __STB_INCLUDE_STB_TRUETYPE_H__

#ifdef STBTT_STATIC
#define STBTT_DEF static
#else
#define STBTT_DEF extern
#endif

#ifdef __cplusplus
		extern "C" {
#endif

			// private structure
			typedef struct
			{
				unsigned char* data;
				int cursor;
				int size;
			} stbtt__buf;

			//////////////////////////////////////////////////////////////////////////////
			//
			// TEXTURE BAKING API
			//
			// If you use this API, you only have to call two functions ever.
			//

			typedef struct
			{
				unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
				float xoff, yoff, xadvance;
			} stbtt_bakedchar;

			STBTT_DEF int stbtt_BakeFontBitmap(const unsigned char* data, int offset,  // font location (use offset=0 for plain .ttf)
				float pixel_height,                     // height of font in pixels
				unsigned char* pixels, int pw, int ph,  // bitmap to be filled in
				int first_char, int num_chars,          // characters to bake
				stbtt_bakedchar* chardata);             // you allocate this, it's num_chars long
// if return is positive, the first unused row of the bitmap
// if return is negative, returns the negative of the number of characters that fit
// if return is 0, no characters fit and no rows were used
// This uses a very crappy packing.

			typedef struct
			{
				float x0, y0, s0, t0; // top-left
				float x1, y1, s1, t1; // bottom-right
			} stbtt_aligned_quad;

			STBTT_DEF void stbtt_GetBakedQuad(const stbtt_bakedchar* chardata, int pw, int ph,  // same data as above
				int char_index,             // character to display
				float* xpos, float* ypos,   // pointers to current position in screen pixel space
				stbtt_aligned_quad* q,      // output: quad to draw
				int opengl_fillrule);       // true if opengl fill rule; false if DX9 or earlier
// Call GetBakedQuad with char_index = 'character - first_char', and it
// creates the quad you need to draw and advances the current position.
//
// The coordinate system used assumes y increases downwards.
//
// Characters will extend both above and below the current position;
// see discussion of "BASELINE" above.
//
// It's inefficient; you might want to c&p it and optimize it.

			STBTT_DEF void stbtt_GetScaledFontVMetrics(const unsigned char* fontdata, int index, float size, float* ascent, float* descent, float* lineGap);
			// Query the font vertical metrics without having to create a font first.


			//////////////////////////////////////////////////////////////////////////////
			//
			// NEW TEXTURE BAKING API
			//
			// This provides options for packing multiple fonts into one atlas, not
			// perfectly but better than nothing.

			typedef struct
			{
				unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
				float xoff, yoff, xadvance;
				float xoff2, yoff2;
			} stbtt_packedchar;

			typedef struct stbtt_pack_context stbtt_pack_context;
			typedef struct stbtt_fontinfo stbtt_fontinfo;
#ifndef STB_RECT_PACK_VERSION
			typedef struct stbrp_rect stbrp_rect;
#endif

			STBTT_DEF int  stbtt_PackBegin(stbtt_pack_context* spc, unsigned char* pixels, int width, int height, int stride_in_bytes, int padding, void* alloc_context);
			// Initializes a packing context stored in the passed-in stbtt_pack_context.
			// Future calls using this context will pack characters into the bitmap passed
			// in here: a 1-channel bitmap that is width * height. stride_in_bytes is
			// the distance from one row to the next (or 0 to mean they are packed tightly
			// together). "padding" is the amount of padding to leave between each
			// character (normally you want '1' for bitmaps you'll use as textures with
			// bilinear filtering).
			//
			// Returns 0 on failure, 1 on success.

			STBTT_DEF void stbtt_PackEnd(stbtt_pack_context* spc);
			// Cleans up the packing context and frees all memory.

#define STBTT_POINT_SIZE(x)   (-(x))

			STBTT_DEF int  stbtt_PackFontRange(stbtt_pack_context* spc, const unsigned char* fontdata, int font_index, float font_size,
				int first_unicode_char_in_range, int num_chars_in_range, stbtt_packedchar* chardata_for_range);
			// Creates character bitmaps from the font_index'th font found in fontdata (use
			// font_index=0 if you don't know what that is). It creates num_chars_in_range
			// bitmaps for characters with unicode values starting at first_unicode_char_in_range
			// and increasing. Data for how to render them is stored in chardata_for_range;
			// pass these to stbtt_GetPackedQuad to get back renderable quads.
			//
			// font_size is the full height of the character from ascender to descender,
			// as computed by stbtt_ScaleForPixelHeight. To use a point size as computed
			// by stbtt_ScaleForMappingEmToPixels, wrap the point size in STBTT_POINT_SIZE()
			// and pass that result as 'font_size':
			//       ...,                  20 , ... // font max minus min y is 20 pixels tall
			//       ..., STBTT_POINT_SIZE(20), ... // 'M' is 20 pixels tall

			typedef struct
			{
				float font_size;
				int first_unicode_codepoint_in_range;  // if non-zero, then the chars are continuous, and this is the first codepoint
				int* array_of_unicode_codepoints;       // if non-zero, then this is an array of unicode codepoints
				int num_chars;
				stbtt_packedchar* chardata_for_range; // output
				unsigned char h_oversample, v_oversample; // don't set these, they're used internally
			} stbtt_pack_range;

			STBTT_DEF int  stbtt_PackFontRanges(stbtt_pack_context* spc, const unsigned char* fontdata, int font_index, stbtt_pack_range* ranges, int num_ranges);
			// Creates character bitmaps from multiple ranges of characters stored in
			// ranges. This will usually create a better-packed bitmap than multiple
			// calls to stbtt_PackFontRange. Note that you can call this multiple
			// times within a single PackBegin/PackEnd.

			STBTT_DEF void stbtt_PackSetOversampling(stbtt_pack_context* spc, unsigned int h_oversample, unsigned int v_oversample);
			// Oversampling a font increases the quality by allowing higher-quality subpixel
			// positioning, and is especially valuable at smaller text sizes.
			//
			// This function sets the amount of oversampling for all following calls to
			// stbtt_PackFontRange(s) or stbtt_PackFontRangesGatherRects for a given
			// pack context. The default (no oversampling) is achieved by h_oversample=1
			// and v_oversample=1. The total number of pixels required is
			// h_oversample*v_oversample larger than the default; for example, 2x2
			// oversampling requires 4x the storage of 1x1. For best results, render
			// oversampled textures with bilinear filtering. Look at the readme in
			// stb/tests/oversample for information about oversampled fonts
			//
			// To use with PackFontRangesGather etc., you must set it before calls
			// call to PackFontRangesGatherRects.

			STBTT_DEF void stbtt_PackSetSkipMissingCodepoints(stbtt_pack_context* spc, int skip);
			// If skip != 0, this tells stb_truetype to skip any codepoints for which
			// there is no corresponding glyph. If skip=0, which is the default, then
			// codepoints without a glyph recived the font's "missing character" glyph,
			// typically an empty box by convention.

			STBTT_DEF void stbtt_GetPackedQuad(const stbtt_packedchar* chardata, int pw, int ph,  // same data as above
				int char_index,             // character to display
				float* xpos, float* ypos,   // pointers to current position in screen pixel space
				stbtt_aligned_quad* q,      // output: quad to draw
				int align_to_integer);

			STBTT_DEF int  stbtt_PackFontRangesGatherRects(stbtt_pack_context* spc, const stbtt_fontinfo* info, stbtt_pack_range* ranges, int num_ranges, stbrp_rect* rects);
			STBTT_DEF void stbtt_PackFontRangesPackRects(stbtt_pack_context* spc, stbrp_rect* rects, int num_rects);
			STBTT_DEF int  stbtt_PackFontRangesRenderIntoRects(stbtt_pack_context* spc, const stbtt_fontinfo* info, stbtt_pack_range* ranges, int num_ranges, stbrp_rect* rects);
			// Calling these functions in sequence is roughly equivalent to calling
			// stbtt_PackFontRanges(). If you more control over the packing of multiple
			// fonts, or if you want to pack custom data into a font texture, take a look
			// at the source to of stbtt_PackFontRanges() and create a custom version
			// using these functions, e.g. call GatherRects multiple times,
			// building up a single array of rects, then call PackRects once,
			// then call RenderIntoRects repeatedly. This may result in a
			// better packing than calling PackFontRanges multiple times
			// (or it may not).

			// this is an opaque structure that you shouldn't mess with which holds
			// all the context needed from PackBegin to PackEnd.
			struct stbtt_pack_context {
				void* user_allocator_context;
				void* pack_info;
				int   width;
				int   height;
				int   stride_in_bytes;
				int   padding;
				int   skip_missing;
				unsigned int   h_oversample, v_oversample;
				unsigned char* pixels;
				void* nodes;
			};

			//////////////////////////////////////////////////////////////////////////////
			//
			// FONT LOADING
			//
			//

			STBTT_DEF int stbtt_GetNumberOfFonts(const unsigned char* data);
			// This function will determine the number of fonts in a font file.  TrueType
			// collection (.ttc) files may contain multiple fonts, while TrueType font
			// (.ttf) files only contain one font. The number of fonts can be used for
			// indexing with the previous function where the index is between zero and one
			// less than the total fonts. If an error occurs, -1 is returned.

			STBTT_DEF int stbtt_GetFontOffsetForIndex(const unsigned char* data, int index);
			// Each .ttf/.ttc file may have more than one font. Each font has a sequential
			// index number starting from 0. Call this function to get the font offset for
			// a given index; it returns -1 if the index is out of range. A regular .ttf
			// file will only define one font and it always be at offset 0, so it will
			// return '0' for index 0, and -1 for all other indices.

			// The following structure is defined publicly so you can declare one on
			// the stack or as a global or etc, but you should treat it as opaque.
			struct stbtt_fontinfo
			{
				void* userdata;
				unsigned char* data;              // pointer to .ttf file
				int              fontstart;         // offset of start of font

				int numGlyphs;                     // number of glyphs, needed for range checking

				int loca, head, glyf, hhea, hmtx, kern, gpos, svg; // table locations as offset from start of .ttf
				int index_map;                     // a cmap mapping for our chosen character encoding
				int indexToLocFormat;              // format needed to map from glyph index to glyph

				stbtt__buf cff;                    // cff font data
				stbtt__buf charstrings;            // the charstring index
				stbtt__buf gsubrs;                 // global charstring subroutines index
				stbtt__buf subrs;                  // private charstring subroutines index
				stbtt__buf fontdicts;              // array of font dicts
				stbtt__buf fdselect;               // map from glyph to fontdict
			};

			STBTT_DEF int stbtt_InitFont(stbtt_fontinfo* info, const unsigned char* data, int offset);
			// Given an offset into the file that defines a font, this function builds
			// the necessary cached info for the rest of the system. You must allocate
			// the stbtt_fontinfo yourself, and stbtt_InitFont will fill it out. You don't
			// need to do anything special to free it, because the contents are pure
			// value data with no additional data structures. Returns 0 on failure.


			//////////////////////////////////////////////////////////////////////////////
			//
			// CHARACTER TO GLYPH-INDEX CONVERSIOn

			STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo* info, int unicode_codepoint);
			// If you're going to perform multiple operations on the same character
			// and you want a speed-up, call this function with the character you're
			// going to process, then use glyph-based functions instead of the
			// codepoint-based functions.
			// Returns 0 if the character codepoint is not defined in the font.


			//////////////////////////////////////////////////////////////////////////////
			//
			// CHARACTER PROPERTIES
			//

			STBTT_DEF float stbtt_ScaleForPixelHeight(const stbtt_fontinfo* info, float pixels);
			// computes a scale factor to produce a font whose "height" is 'pixels' tall.
			// Height is measured as the distance from the highest ascender to the lowest
			// descender; in other words, it's equivalent to calling stbtt_GetFontVMetrics
			// and computing:
			//       scale = pixels / (ascent - descent)
			// so if you prefer to measure height by the ascent only, use a similar calculation.

			STBTT_DEF float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo* info, float pixels);
			// computes a scale factor to produce a font whose EM size is mapped to
			// 'pixels' tall. This is probably what traditional APIs compute, but
			// I'm not positive.

			STBTT_DEF void stbtt_GetFontVMetrics(const stbtt_fontinfo* info, int* ascent, int* descent, int* lineGap);
			// ascent is the coordinate above the baseline the font extends; descent
			// is the coordinate below the baseline the font extends (i.e. it is typically negative)
			// lineGap is the spacing between one row's descent and the next row's ascent...
			// so you should advance the vertical position by "*ascent - *descent + *lineGap"
			//   these are expressed in unscaled coordinates, so you must multiply by
			//   the scale factor for a given size

			STBTT_DEF int  stbtt_GetFontVMetricsOS2(const stbtt_fontinfo* info, int* typoAscent, int* typoDescent, int* typoLineGap);
			// analogous to GetFontVMetrics, but returns the "typographic" values from the OS/2
			// table (specific to MS/Windows TTF files).
			//
			// Returns 1 on success (table present), 0 on failure.

			STBTT_DEF void stbtt_GetFontBoundingBox(const stbtt_fontinfo* info, int* x0, int* y0, int* x1, int* y1);
			// the bounding box around all possible characters

			STBTT_DEF void stbtt_GetCodepointHMetrics(const stbtt_fontinfo* info, int codepoint, int* advanceWidth, int* leftSideBearing);
			// leftSideBearing is the offset from the current horizontal position to the left edge of the character
			// advanceWidth is the offset from the current horizontal position to the next horizontal position
			//   these are expressed in unscaled coordinates

			STBTT_DEF int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo* info, int ch1, int ch2);
			// an additional amount to add to the 'advance' value between ch1 and ch2

			STBTT_DEF int stbtt_GetCodepointBox(const stbtt_fontinfo* info, int codepoint, int* x0, int* y0, int* x1, int* y1);
			// Gets the bounding box of the visible part of the glyph, in unscaled coordinates

			STBTT_DEF void stbtt_GetGlyphHMetrics(const stbtt_fontinfo* info, int glyph_index, int* advanceWidth, int* leftSideBearing);
			STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo* info, int glyph1, int glyph2);
			STBTT_DEF int  stbtt_GetGlyphBox(const stbtt_fontinfo* info, int glyph_index, int* x0, int* y0, int* x1, int* y1);
			// as above, but takes one or more glyph indices for greater efficiency

			typedef struct stbtt_kerningentry
			{
				int glyph1; // use stbtt_FindGlyphIndex
				int glyph2;
				int advance;
			} stbtt_kerningentry;

			STBTT_DEF int  stbtt_GetKerningTableLength(const stbtt_fontinfo* info);
			STBTT_DEF int  stbtt_GetKerningTable(const stbtt_fontinfo* info, stbtt_kerningentry* table, int table_length);
			// Retrieves a complete list of all of the kerning pairs provided by the font
			// stbtt_GetKerningTable never writes more than table_length entries and returns how many entries it did write.
			// The table will be sorted by (a.glyph1 == b.glyph1)?(a.glyph2 < b.glyph2):(a.glyph1 < b.glyph1)

			//////////////////////////////////////////////////////////////////////////////
			//
			// GLYPH SHAPES (you probably don't need these, but they have to go before
			// the bitmaps for C declaration-order reasons)
			//

#ifndef STBTT_vmove // you can predefine these to use different values (but why?)
			enum {
				STBTT_vmove = 1,
				STBTT_vline,
				STBTT_vcurve,
				STBTT_vcubic
			};
#endif

#ifndef stbtt_vertex // you can predefine this to use different values
			// (we share this with other code at RAD)
#define stbtt_vertex_type short // can't use stbtt_int16 because that's not visible in the header file
			typedef struct
			{
				stbtt_vertex_type x, y, cx, cy, cx1, cy1;
				unsigned char type, padding;
			} stbtt_vertex;
#endif

			STBTT_DEF int stbtt_IsGlyphEmpty(const stbtt_fontinfo* info, int glyph_index);
			// returns non-zero if nothing is drawn for this glyph

			STBTT_DEF int stbtt_GetCodepointShape(const stbtt_fontinfo* info, int unicode_codepoint, stbtt_vertex** vertices);
			STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo* info, int glyph_index, stbtt_vertex** vertices);
			// returns # of vertices and fills *vertices with the pointer to them
			//   these are expressed in "unscaled" coordinates
			//
			// The shape is a series of contours. Each one starts with
			// a STBTT_moveto, then consists of a series of mixed
			// STBTT_lineto and STBTT_curveto segments. A lineto
			// draws a line from previous endpoint to its x,y; a curveto
			// draws a quadratic bezier from previous endpoint to
			// its x,y, using cx,cy as the bezier control point.

			STBTT_DEF void stbtt_FreeShape(const stbtt_fontinfo* info, stbtt_vertex* vertices);
			// frees the data allocated above

			STBTT_DEF unsigned char* stbtt_FindSVGDoc(const stbtt_fontinfo* info, int gl);
			STBTT_DEF int stbtt_GetCodepointSVG(const stbtt_fontinfo* info, int unicode_codepoint, const char** svg);
			STBTT_DEF int stbtt_GetGlyphSVG(const stbtt_fontinfo* info, int gl, const char** svg);
			// fills svg with the character's SVG data.
			// returns data size or 0 if SVG not found.

			//////////////////////////////////////////////////////////////////////////////
			//
			// BITMAP RENDERING
			//

			STBTT_DEF void stbtt_FreeBitmap(unsigned char* bitmap, void* userdata);
			// frees the bitmap allocated below

			STBTT_DEF unsigned char* stbtt_GetCodepointBitmap(const stbtt_fontinfo* info, float scale_x, float scale_y, int codepoint, int* width, int* height, int* xoff, int* yoff);
			// allocates a large-enough single-channel 8bpp bitmap and renders the
			// specified character/glyph at the specified scale into it, with
			// antialiasing. 0 is no coverage (transparent), 255 is fully covered (opaque).
			// *width & *height are filled out with the width & height of the bitmap,
			// which is stored left-to-right, top-to-bottom.
			//
			// xoff/yoff are the offset it pixel space from the glyph origin to the top-left of the bitmap

			STBTT_DEF unsigned char* stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo* info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int* width, int* height, int* xoff, int* yoff);
			// the same as stbtt_GetCodepoitnBitmap, but you can specify a subpixel
			// shift for the character

			STBTT_DEF void stbtt_MakeCodepointBitmap(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint);
			// the same as stbtt_GetCodepointBitmap, but you pass in storage for the bitmap
			// in the form of 'output', with row spacing of 'out_stride' bytes. the bitmap
			// is clipped to out_w/out_h bytes. Call stbtt_GetCodepointBitmapBox to get the
			// width and height and positioning info for it first.

			STBTT_DEF void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint);
			// same as stbtt_MakeCodepointBitmap, but you can specify a subpixel
			// shift for the character

			STBTT_DEF void stbtt_MakeCodepointBitmapSubpixelPrefilter(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int oversample_x, int oversample_y, float* sub_x, float* sub_y, int codepoint);
			// same as stbtt_MakeCodepointBitmapSubpixel, but prefiltering
			// is performed (see stbtt_PackSetOversampling)

			STBTT_DEF void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo* font, int codepoint, float scale_x, float scale_y, int* ix0, int* iy0, int* ix1, int* iy1);
			// get the bbox of the bitmap centered around the glyph origin; so the
			// bitmap width is ix1-ix0, height is iy1-iy0, and location to place
			// the bitmap top left is (leftSideBearing*scale,iy0).
			// (Note that the bitmap uses y-increases-down, but the shape uses
			// y-increases-up, so CodepointBitmapBox and CodepointBox are inverted.)

			STBTT_DEF void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo* font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int* ix0, int* iy0, int* ix1, int* iy1);
			// same as stbtt_GetCodepointBitmapBox, but you can specify a subpixel
			// shift for the character

			// the following functions are equivalent to the above functions, but operate
			// on glyph indices instead of Unicode codepoints (for efficiency)
			STBTT_DEF unsigned char* stbtt_GetGlyphBitmap(const stbtt_fontinfo* info, float scale_x, float scale_y, int glyph, int* width, int* height, int* xoff, int* yoff);
			STBTT_DEF unsigned char* stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo* info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int* width, int* height, int* xoff, int* yoff);
			STBTT_DEF void stbtt_MakeGlyphBitmap(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph);
			STBTT_DEF void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph);
			STBTT_DEF void stbtt_MakeGlyphBitmapSubpixelPrefilter(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int oversample_x, int oversample_y, float* sub_x, float* sub_y, int glyph);
			STBTT_DEF void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo* font, int glyph, float scale_x, float scale_y, int* ix0, int* iy0, int* ix1, int* iy1);
			STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo* font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int* ix0, int* iy0, int* ix1, int* iy1);


			// @TODO: don't expose this structure
			typedef struct
			{
				int w, h, stride;
				unsigned char* pixels;
			} stbtt__bitmap;

			// rasterize a shape with quadratic beziers into a bitmap
			STBTT_DEF void stbtt_Rasterize(stbtt__bitmap* result,        // 1-channel bitmap to draw into
				float flatness_in_pixels,     // allowable error of curve in pixels
				stbtt_vertex* vertices,       // array of vertices defining shape
				int num_verts,                // number of vertices in above array
				float scale_x, float scale_y, // scale applied to input vertices
				float shift_x, float shift_y, // translation applied to input vertices
				int x_off, int y_off,         // another translation applied to input
				int invert,                   // if non-zero, vertically flip shape
				void* userdata);              // context for to STBTT_MALLOC

//////////////////////////////////////////////////////////////////////////////
//
// Signed Distance Function (or Field) rendering

			STBTT_DEF void stbtt_FreeSDF(unsigned char* bitmap, void* userdata);
			// frees the SDF bitmap allocated below

			STBTT_DEF unsigned char* stbtt_GetGlyphSDF(const stbtt_fontinfo* info, float scale, int glyph, int padding, unsigned char onedge_value, float pixel_dist_scale, int* width, int* height, int* xoff, int* yoff);
			STBTT_DEF unsigned char* stbtt_GetCodepointSDF(const stbtt_fontinfo* info, float scale, int codepoint, int padding, unsigned char onedge_value, float pixel_dist_scale, int* width, int* height, int* xoff, int* yoff);
			// These functions compute a discretized SDF field for a single character, suitable for storing
			// in a single-channel texture, sampling with bilinear filtering, and testing against
			// larger than some threshold to produce scalable fonts.
			//        info              --  the font
			//        scale             --  controls the size of the resulting SDF bitmap, same as it would be creating a regular bitmap
			//        glyph/codepoint   --  the character to generate the SDF for
			//        padding           --  extra "pixels" around the character which are filled with the distance to the character (not 0),
			//                                 which allows effects like bit outlines
			//        onedge_value      --  value 0-255 to test the SDF against to reconstruct the character (i.e. the isocontour of the character)
			//        pixel_dist_scale  --  what value the SDF should increase by when moving one SDF "pixel" away from the edge (on the 0..255 scale)
			//                                 if positive, > onedge_value is inside; if negative, < onedge_value is inside
			//        width,height      --  output height & width of the SDF bitmap (including padding)
			//        xoff,yoff         --  output origin of the character
			//        return value      --  a 2D array of bytes 0..255, width*height in size
			//
			// pixel_dist_scale & onedge_value are a scale & bias that allows you to make
			// optimal use of the limited 0..255 for your application, trading off precision
			// and special effects. SDF values outside the range 0..255 are clamped to 0..255.
			//
			// Example:
			//      scale = stbtt_ScaleForPixelHeight(22)
			//      padding = 5
			//      onedge_value = 180
			//      pixel_dist_scale = 180/5.0 = 36.0
			//
			//      This will create an SDF bitmap in which the character is about 22 pixels
			//      high but the whole bitmap is about 22+5+5=32 pixels high. To produce a filled
			//      shape, sample the SDF at each pixel and fill the pixel if the SDF value
			//      is greater than or equal to 180/255. (You'll actually want to antialias,
			//      which is beyond the scope of this example.) Additionally, you can compute
			//      offset outlines (e.g. to stroke the character border inside & outside,
			//      or only outside). For example, to fill outside the character up to 3 SDF
			//      pixels, you would compare against (180-36.0*3)/255 = 72/255. The above
			//      choice of variables maps a range from 5 pixels outside the shape to
			//      2 pixels inside the shape to 0..255; this is intended primarily for apply
			//      outside effects only (the interior range is needed to allow proper
			//      antialiasing of the font at *smaller* sizes)
			//
			// The function computes the SDF analytically at each SDF pixel, not by e.g.
			// building a higher-res bitmap and approximating it. In theory the quality
			// should be as high as possible for an SDF of this size & representation, but
			// unclear if this is true in practice (perhaps building a higher-res bitmap
			// and computing from that can allow drop-out prevention).
			//
			// The algorithm has not been optimized at all, so expect it to be slow
			// if computing lots of characters or very large sizes.



			//////////////////////////////////////////////////////////////////////////////
			//
			// Finding the right font...
			//
			// You should really just solve this offline, keep your own tables
			// of what font is what, and don't try to get it out of the .ttf file.
			// That's because getting it out of the .ttf file is really hard, because
			// the names in the file can appear in many possible encodings, in many
			// possible languages, and e.g. if you need a case-insensitive comparison,
			// the details of that depend on the encoding & language in a complex way
			// (actually underspecified in truetype, but also gigantic).
			//
			// But you can use the provided functions in two possible ways:
			//     stbtt_FindMatchingFont() will use *case-sensitive* comparisons on
			//             unicode-encoded names to try to find the font you want;
			//             you can run this before calling stbtt_InitFont()
			//
			//     stbtt_GetFontNameString() lets you get any of the various strings
			//             from the file yourself and do your own comparisons on them.
			//             You have to have called stbtt_InitFont() first.


			STBTT_DEF int stbtt_FindMatchingFont(const unsigned char* fontdata, const char* name, int flags);
			// returns the offset (not index) of the font that matches, or -1 if none
			//   if you use STBTT_MACSTYLE_DONTCARE, use a font name like "Arial Bold".
			//   if you use any other flag, use a font name like "Arial"; this checks
			//     the 'macStyle' header field; i don't know if fonts set this consistently
#define STBTT_MACSTYLE_DONTCARE     0
#define STBTT_MACSTYLE_BOLD         1
#define STBTT_MACSTYLE_ITALIC       2
#define STBTT_MACSTYLE_UNDERSCORE   4
#define STBTT_MACSTYLE_NONE         8   // <= not same as 0, this makes us check the bitfield is 0

			STBTT_DEF int stbtt_CompareUTF8toUTF16_bigendian(const char* s1, int len1, const char* s2, int len2);
			// returns 1/0 whether the first string interpreted as utf8 is identical to
			// the second string interpreted as big-endian utf16... useful for strings from next func

			STBTT_DEF const char* stbtt_GetFontNameString(const stbtt_fontinfo* font, int* length, int platformID, int encodingID, int languageID, int nameID);
			// returns the string (which may be big-endian double byte, e.g. for unicode)
			// and puts the length in bytes in *length.
			//
			// some of the values for the IDs are below; for more see the truetype spec:
			//     http://developer.apple.com/textfonts/TTRefMan/RM06/Chap6name.html
			//     http://www.microsoft.com/typography/otspec/name.htm

			enum { // platformID
				STBTT_PLATFORM_ID_UNICODE = 0,
				STBTT_PLATFORM_ID_MAC = 1,
				STBTT_PLATFORM_ID_ISO = 2,
				STBTT_PLATFORM_ID_MICROSOFT = 3
			};

			enum { // encodingID for STBTT_PLATFORM_ID_UNICODE
				STBTT_UNICODE_EID_UNICODE_1_0 = 0,
				STBTT_UNICODE_EID_UNICODE_1_1 = 1,
				STBTT_UNICODE_EID_ISO_10646 = 2,
				STBTT_UNICODE_EID_UNICODE_2_0_BMP = 3,
				STBTT_UNICODE_EID_UNICODE_2_0_FULL = 4
			};

			enum { // encodingID for STBTT_PLATFORM_ID_MICROSOFT
				STBTT_MS_EID_SYMBOL = 0,
				STBTT_MS_EID_UNICODE_BMP = 1,
				STBTT_MS_EID_SHIFTJIS = 2,
				STBTT_MS_EID_UNICODE_FULL = 10
			};

			enum { // encodingID for STBTT_PLATFORM_ID_MAC; same as Script Manager codes
				STBTT_MAC_EID_ROMAN = 0, STBTT_MAC_EID_ARABIC = 4,
				STBTT_MAC_EID_JAPANESE = 1, STBTT_MAC_EID_HEBREW = 5,
				STBTT_MAC_EID_CHINESE_TRAD = 2, STBTT_MAC_EID_GREEK = 6,
				STBTT_MAC_EID_KOREAN = 3, STBTT_MAC_EID_RUSSIAN = 7
			};

			enum { // languageID for STBTT_PLATFORM_ID_MICROSOFT; same as LCID...
				   // problematic because there are e.g. 16 english LCIDs and 16 arabic LCIDs
				STBTT_MS_LANG_ENGLISH = 0x0409, STBTT_MS_LANG_ITALIAN = 0x0410,
				STBTT_MS_LANG_CHINESE = 0x0804, STBTT_MS_LANG_JAPANESE = 0x0411,
				STBTT_MS_LANG_DUTCH = 0x0413, STBTT_MS_LANG_KOREAN = 0x0412,
				STBTT_MS_LANG_FRENCH = 0x040c, STBTT_MS_LANG_RUSSIAN = 0x0419,
				STBTT_MS_LANG_GERMAN = 0x0407, STBTT_MS_LANG_SPANISH = 0x0409,
				STBTT_MS_LANG_HEBREW = 0x040d, STBTT_MS_LANG_SWEDISH = 0x041D
			};

			enum { // languageID for STBTT_PLATFORM_ID_MAC
				STBTT_MAC_LANG_ENGLISH = 0, STBTT_MAC_LANG_JAPANESE = 11,
				STBTT_MAC_LANG_ARABIC = 12, STBTT_MAC_LANG_KOREAN = 23,
				STBTT_MAC_LANG_DUTCH = 4, STBTT_MAC_LANG_RUSSIAN = 32,
				STBTT_MAC_LANG_FRENCH = 1, STBTT_MAC_LANG_SPANISH = 6,
				STBTT_MAC_LANG_GERMAN = 2, STBTT_MAC_LANG_SWEDISH = 5,
				STBTT_MAC_LANG_HEBREW = 10, STBTT_MAC_LANG_CHINESE_SIMPLIFIED = 33,
				STBTT_MAC_LANG_ITALIAN = 3, STBTT_MAC_LANG_CHINESE_TRAD = 19
			};

#ifdef __cplusplus
		}
#endif

#endif // __STB_INCLUDE_STB_TRUETYPE_H__

		///////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////
		////
		////   IMPLEMENTATION
		////
		////

#ifdef STB_TRUETYPE_IMPLEMENTATION

#ifndef STBTT_MAX_OVERSAMPLE
#define STBTT_MAX_OVERSAMPLE   8
#endif

#if STBTT_MAX_OVERSAMPLE > 255
#error "STBTT_MAX_OVERSAMPLE cannot be > 255"
#endif

		typedef int stbtt__test_oversample_pow2[(STBTT_MAX_OVERSAMPLE & (STBTT_MAX_OVERSAMPLE - 1)) == 0 ? 1 : -1];

#ifndef STBTT_RASTERIZER_VERSION
#define STBTT_RASTERIZER_VERSION 2
#endif

#ifdef _MSC_VER
#define STBTT__NOTUSED(v)  (void)(v)
#else
#define STBTT__NOTUSED(v)  (void)sizeof(v)
#endif

		//////////////////////////////////////////////////////////////////////////
		//
		// stbtt__buf helpers to parse data from file
		//

		static stbtt_uint8 stbtt__buf_get8(stbtt__buf* b)
		{
			if (b->cursor >= b->size)
				return 0;
			return b->data[b->cursor++];
		}

		static stbtt_uint8 stbtt__buf_peek8(stbtt__buf* b)
		{
			if (b->cursor >= b->size)
				return 0;
			return b->data[b->cursor];
		}

		static void stbtt__buf_seek(stbtt__buf* b, int o)
		{
			STBTT_assert(!(o > b->size || o < 0));
			b->cursor = (o > b->size || o < 0) ? b->size : o;
		}

		static void stbtt__buf_skip(stbtt__buf* b, int o)
		{
			stbtt__buf_seek(b, b->cursor + o);
		}

		static stbtt_uint32 stbtt__buf_get(stbtt__buf* b, int n)
		{
			stbtt_uint32 v = 0;
			int i;
			STBTT_assert(n >= 1 && n <= 4);
			for (i = 0; i < n; i++)
				v = (v << 8) | stbtt__buf_get8(b);
			return v;
		}

		static stbtt__buf stbtt__new_buf(const void* p, size_t size)
		{
			stbtt__buf r;
			STBTT_assert(size < 0x40000000);
			r.data = (stbtt_uint8*)p;
			r.size = (int)size;
			r.cursor = 0;
			return r;
		}

#define stbtt__buf_get16(b)  stbtt__buf_get((b), 2)
#define stbtt__buf_get32(b)  stbtt__buf_get((b), 4)

		static stbtt__buf stbtt__buf_range(const stbtt__buf* b, int o, int s)
		{
			stbtt__buf r = stbtt__new_buf(NULL, 0);
			if (o < 0 || s < 0 || o > b->size || s > b->size - o) return r;
			r.data = b->data + o;
			r.size = s;
			return r;
		}

		static stbtt__buf stbtt__cff_get_index(stbtt__buf* b)
		{
			int count, start, offsize;
			start = b->cursor;
			count = stbtt__buf_get16(b);
			if (count) {
				offsize = stbtt__buf_get8(b);
				STBTT_assert(offsize >= 1 && offsize <= 4);
				stbtt__buf_skip(b, offsize * count);
				stbtt__buf_skip(b, stbtt__buf_get(b, offsize) - 1);
			}
			return stbtt__buf_range(b, start, b->cursor - start);
		}

		static stbtt_uint32 stbtt__cff_int(stbtt__buf* b)
		{
			int b0 = stbtt__buf_get8(b);
			if (b0 >= 32 && b0 <= 246)       return b0 - 139;
			else if (b0 >= 247 && b0 <= 250) return (b0 - 247) * 256 + stbtt__buf_get8(b) + 108;
			else if (b0 >= 251 && b0 <= 254) return -(b0 - 251) * 256 - stbtt__buf_get8(b) - 108;
			else if (b0 == 28)               return stbtt__buf_get16(b);
			else if (b0 == 29)               return stbtt__buf_get32(b);
			STBTT_assert(0);
			return 0;
		}

		static void stbtt__cff_skip_operand(stbtt__buf* b) {
			int v, b0 = stbtt__buf_peek8(b);
			STBTT_assert(b0 >= 28);
			if (b0 == 30) {
				stbtt__buf_skip(b, 1);
				while (b->cursor < b->size) {
					v = stbtt__buf_get8(b);
					if ((v & 0xF) == 0xF || (v >> 4) == 0xF)
						break;
				}
			}
			else {
				stbtt__cff_int(b);
			}
		}

		static stbtt__buf stbtt__dict_get(stbtt__buf* b, int key)
		{
			stbtt__buf_seek(b, 0);
			while (b->cursor < b->size) {
				int start = b->cursor, end, op;
				while (stbtt__buf_peek8(b) >= 28)
					stbtt__cff_skip_operand(b);
				end = b->cursor;
				op = stbtt__buf_get8(b);
				if (op == 12)  op = stbtt__buf_get8(b) | 0x100;
				if (op == key) return stbtt__buf_range(b, start, end - start);
			}
			return stbtt__buf_range(b, 0, 0);
		}

		static void stbtt__dict_get_ints(stbtt__buf* b, int key, int outcount, stbtt_uint32* out)
		{
			int i;
			stbtt__buf operands = stbtt__dict_get(b, key);
			for (i = 0; i < outcount && operands.cursor < operands.size; i++)
				out[i] = stbtt__cff_int(&operands);
		}

		static int stbtt__cff_index_count(stbtt__buf* b)
		{
			stbtt__buf_seek(b, 0);
			return stbtt__buf_get16(b);
		}

		static stbtt__buf stbtt__cff_index_get(stbtt__buf b, int i)
		{
			int count, offsize, start, end;
			stbtt__buf_seek(&b, 0);
			count = stbtt__buf_get16(&b);
			offsize = stbtt__buf_get8(&b);
			STBTT_assert(i >= 0 && i < count);
			STBTT_assert(offsize >= 1 && offsize <= 4);
			stbtt__buf_skip(&b, i * offsize);
			start = stbtt__buf_get(&b, offsize);
			end = stbtt__buf_get(&b, offsize);
			return stbtt__buf_range(&b, 2 + (count + 1) * offsize + start, end - start);
		}

		//////////////////////////////////////////////////////////////////////////
		//
		// accessors to parse data from file
		//

		// on platforms that don't allow misaligned reads, if we want to allow
		// truetype fonts that aren't padded to alignment, define ALLOW_UNALIGNED_TRUETYPE

#define ttBYTE(p)     (* (stbtt_uint8 *) (p))
#define ttCHAR(p)     (* (stbtt_int8 *) (p))
#define ttFixed(p)    ttLONG(p)

		static stbtt_uint16 ttUSHORT(stbtt_uint8* p) { return p[0] * 256 + p[1]; }
		static stbtt_int16 ttSHORT(stbtt_uint8* p) { return p[0] * 256 + p[1]; }
		static stbtt_uint32 ttULONG(stbtt_uint8* p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }
		static stbtt_int32 ttLONG(stbtt_uint8* p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }

#define stbtt_tag4(p,c0,c1,c2,c3) ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define stbtt_tag(p,str)           stbtt_tag4(p,str[0],str[1],str[2],str[3])

		static int stbtt__isfont(stbtt_uint8* font)
		{
			// check the version number
			if (stbtt_tag4(font, '1', 0, 0, 0))  return 1; // TrueType 1
			if (stbtt_tag(font, "typ1"))   return 1; // TrueType with type 1 font -- we don't support this!
			if (stbtt_tag(font, "OTTO"))   return 1; // OpenType with CFF
			if (stbtt_tag4(font, 0, 1, 0, 0)) return 1; // OpenType 1.0
			if (stbtt_tag(font, "true"))   return 1; // Apple specification for TrueType fonts
			return 0;
		}

		// @OPTIMIZE: binary search
		static stbtt_uint32 stbtt__find_table(stbtt_uint8* data, stbtt_uint32 fontstart, const char* tag)
		{
			stbtt_int32 num_tables = ttUSHORT(data + fontstart + 4);
			stbtt_uint32 tabledir = fontstart + 12;
			stbtt_int32 i;
			for (i = 0; i < num_tables; ++i) {
				stbtt_uint32 loc = tabledir + 16 * i;
				if (stbtt_tag(data + loc + 0, tag))
					return ttULONG(data + loc + 8);
			}
			return 0;
		}

		static int stbtt_GetFontOffsetForIndex_internal(unsigned char* font_collection, int index)
		{
			// if it's just a font, there's only one valid index
			if (stbtt__isfont(font_collection))
				return index == 0 ? 0 : -1;

			// check if it's a TTC
			if (stbtt_tag(font_collection, "ttcf")) {
				// version 1?
				if (ttULONG(font_collection + 4) == 0x00010000 || ttULONG(font_collection + 4) == 0x00020000) {
					stbtt_int32 n = ttLONG(font_collection + 8);
					if (index >= n)
						return -1;
					return ttULONG(font_collection + 12 + index * 4);
				}
			}
			return -1;
		}

		static int stbtt_GetNumberOfFonts_internal(unsigned char* font_collection)
		{
			// if it's just a font, there's only one valid font
			if (stbtt__isfont(font_collection))
				return 1;

			// check if it's a TTC
			if (stbtt_tag(font_collection, "ttcf")) {
				// version 1?
				if (ttULONG(font_collection + 4) == 0x00010000 || ttULONG(font_collection + 4) == 0x00020000) {
					return ttLONG(font_collection + 8);
				}
			}
			return 0;
		}

		static stbtt__buf stbtt__get_subrs(stbtt__buf cff, stbtt__buf fontdict)
		{
			stbtt_uint32 subrsoff = 0, private_loc[2] = { 0, 0 };
			stbtt__buf pdict;
			stbtt__dict_get_ints(&fontdict, 18, 2, private_loc);
			if (!private_loc[1] || !private_loc[0]) return stbtt__new_buf(NULL, 0);
			pdict = stbtt__buf_range(&cff, private_loc[1], private_loc[0]);
			stbtt__dict_get_ints(&pdict, 19, 1, &subrsoff);
			if (!subrsoff) return stbtt__new_buf(NULL, 0);
			stbtt__buf_seek(&cff, private_loc[1] + subrsoff);
			return stbtt__cff_get_index(&cff);
		}

		// since most people won't use this, find this table the first time it's needed
		static int stbtt__get_svg(stbtt_fontinfo* info)
		{
			stbtt_uint32 t;
			if (info->svg < 0) {
				t = stbtt__find_table(info->data, info->fontstart, "SVG ");
				if (t) {
					stbtt_uint32 offset = ttULONG(info->data + t + 2);
					info->svg = t + offset;
				}
				else {
					info->svg = 0;
				}
			}
			return info->svg;
		}

		static int stbtt_InitFont_internal(stbtt_fontinfo* info, unsigned char* data, int fontstart)
		{
			stbtt_uint32 cmap, t;
			stbtt_int32 i, numTables;

			info->data = data;
			info->fontstart = fontstart;
			info->cff = stbtt__new_buf(NULL, 0);

			cmap = stbtt__find_table(data, fontstart, "cmap");       // required
			info->loca = stbtt__find_table(data, fontstart, "loca"); // required
			info->head = stbtt__find_table(data, fontstart, "head"); // required
			info->glyf = stbtt__find_table(data, fontstart, "glyf"); // required
			info->hhea = stbtt__find_table(data, fontstart, "hhea"); // required
			info->hmtx = stbtt__find_table(data, fontstart, "hmtx"); // required
			info->kern = stbtt__find_table(data, fontstart, "kern"); // not required
			info->gpos = stbtt__find_table(data, fontstart, "GPOS"); // not required

			if (!cmap || !info->head || !info->hhea || !info->hmtx)
				return 0;
			if (info->glyf) {
				// required for truetype
				if (!info->loca) return 0;
			}
			else {
				// initialization for CFF / Type2 fonts (OTF)
				stbtt__buf b, topdict, topdictidx;
				stbtt_uint32 cstype = 2, charstrings = 0, fdarrayoff = 0, fdselectoff = 0;
				stbtt_uint32 cff;

				cff = stbtt__find_table(data, fontstart, "CFF ");
				if (!cff) return 0;

				info->fontdicts = stbtt__new_buf(NULL, 0);
				info->fdselect = stbtt__new_buf(NULL, 0);

				// @TODO this should use size from table (not 512MB)
				info->cff = stbtt__new_buf(data + cff, 512 * 1024 * 1024);
				b = info->cff;

				// read the header
				stbtt__buf_skip(&b, 2);
				stbtt__buf_seek(&b, stbtt__buf_get8(&b)); // hdrsize

				// @TODO the name INDEX could list multiple fonts,
				// but we just use the first one.
				stbtt__cff_get_index(&b);  // name INDEX
				topdictidx = stbtt__cff_get_index(&b);
				topdict = stbtt__cff_index_get(topdictidx, 0);
				stbtt__cff_get_index(&b);  // string INDEX
				info->gsubrs = stbtt__cff_get_index(&b);

				stbtt__dict_get_ints(&topdict, 17, 1, &charstrings);
				stbtt__dict_get_ints(&topdict, 0x100 | 6, 1, &cstype);
				stbtt__dict_get_ints(&topdict, 0x100 | 36, 1, &fdarrayoff);
				stbtt__dict_get_ints(&topdict, 0x100 | 37, 1, &fdselectoff);
				info->subrs = stbtt__get_subrs(b, topdict);

				// we only support Type 2 charstrings
				if (cstype != 2) return 0;
				if (charstrings == 0) return 0;

				if (fdarrayoff) {
					// looks like a CID font
					if (!fdselectoff) return 0;
					stbtt__buf_seek(&b, fdarrayoff);
					info->fontdicts = stbtt__cff_get_index(&b);
					info->fdselect = stbtt__buf_range(&b, fdselectoff, b.size - fdselectoff);
				}

				stbtt__buf_seek(&b, charstrings);
				info->charstrings = stbtt__cff_get_index(&b);
			}

			t = stbtt__find_table(data, fontstart, "maxp");
			if (t)
				info->numGlyphs = ttUSHORT(data + t + 4);
			else
				info->numGlyphs = 0xffff;

			info->svg = -1;

			// find a cmap encoding table we understand *now* to avoid searching
			// later. (todo: could make this installable)
			// the same regardless of glyph.
			numTables = ttUSHORT(data + cmap + 2);
			info->index_map = 0;
			for (i = 0; i < numTables; ++i) {
				stbtt_uint32 encoding_record = cmap + 4 + 8 * i;
				// find an encoding we understand:
				switch (ttUSHORT(data + encoding_record)) {
				case STBTT_PLATFORM_ID_MICROSOFT:
					switch (ttUSHORT(data + encoding_record + 2)) {
					case STBTT_MS_EID_UNICODE_BMP:
					case STBTT_MS_EID_UNICODE_FULL:
						// MS/Unicode
						info->index_map = cmap + ttULONG(data + encoding_record + 4);
						break;
					}
					break;
				case STBTT_PLATFORM_ID_UNICODE:
					// Mac/iOS has these
					// all the encodingIDs are unicode, so we don't bother to check it
					info->index_map = cmap + ttULONG(data + encoding_record + 4);
					break;
				}
			}
			if (info->index_map == 0)
				return 0;

			info->indexToLocFormat = ttUSHORT(data + info->head + 50);
			return 1;
		}

		STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo* info, int unicode_codepoint)
		{
			stbtt_uint8* data = info->data;
			stbtt_uint32 index_map = info->index_map;

			stbtt_uint16 format = ttUSHORT(data + index_map + 0);
			if (format == 0) { // apple byte encoding
				stbtt_int32 bytes = ttUSHORT(data + index_map + 2);
				if (unicode_codepoint < bytes - 6)
					return ttBYTE(data + index_map + 6 + unicode_codepoint);
				return 0;
			}
			else if (format == 6) {
				stbtt_uint32 first = ttUSHORT(data + index_map + 6);
				stbtt_uint32 count = ttUSHORT(data + index_map + 8);
				if ((stbtt_uint32)unicode_codepoint >= first && (stbtt_uint32)unicode_codepoint < first + count)
					return ttUSHORT(data + index_map + 10 + (unicode_codepoint - first) * 2);
				return 0;
			}
			else if (format == 2) {
				STBTT_assert(0); // @TODO: high-byte mapping for japanese/chinese/korean
				return 0;
			}
			else if (format == 4) { // standard mapping for windows fonts: binary search collection of ranges
				stbtt_uint16 segcount = ttUSHORT(data + index_map + 6) >> 1;
				stbtt_uint16 searchRange = ttUSHORT(data + index_map + 8) >> 1;
				stbtt_uint16 entrySelector = ttUSHORT(data + index_map + 10);
				stbtt_uint16 rangeShift = ttUSHORT(data + index_map + 12) >> 1;

				// do a binary search of the segments
				stbtt_uint32 endCount = index_map + 14;
				stbtt_uint32 search = endCount;

				if (unicode_codepoint > 0xffff)
					return 0;

				// they lie from endCount .. endCount + segCount
				// but searchRange is the nearest power of two, so...
				if (unicode_codepoint >= ttUSHORT(data + search + rangeShift * 2))
					search += rangeShift * 2;

				// now decrement to bias correctly to find smallest
				search -= 2;
				while (entrySelector) {
					stbtt_uint16 end;
					searchRange >>= 1;
					end = ttUSHORT(data + search + searchRange * 2);
					if (unicode_codepoint > end)
						search += searchRange * 2;
					--entrySelector;
				}
				search += 2;

				{
					stbtt_uint16 offset, start, last;
					stbtt_uint16 item = (stbtt_uint16)((search - endCount) >> 1);

					start = ttUSHORT(data + index_map + 14 + segcount * 2 + 2 + 2 * item);
					last = ttUSHORT(data + endCount + 2 * item);
					if (unicode_codepoint < start || unicode_codepoint > last)
						return 0;

					offset = ttUSHORT(data + index_map + 14 + segcount * 6 + 2 + 2 * item);
					if (offset == 0)
						return (stbtt_uint16)(unicode_codepoint + ttSHORT(data + index_map + 14 + segcount * 4 + 2 + 2 * item));

					return ttUSHORT(data + offset + (unicode_codepoint - start) * 2 + index_map + 14 + segcount * 6 + 2 + 2 * item);
				}
			}
			else if (format == 12 || format == 13) {
				stbtt_uint32 ngroups = ttULONG(data + index_map + 12);
				stbtt_int32 low, high;
				low = 0; high = (stbtt_int32)ngroups;
				// Binary search the right group.
				while (low < high) {
					stbtt_int32 mid = low + ((high - low) >> 1); // rounds down, so low <= mid < high
					stbtt_uint32 start_char = ttULONG(data + index_map + 16 + mid * 12);
					stbtt_uint32 end_char = ttULONG(data + index_map + 16 + mid * 12 + 4);
					if ((stbtt_uint32)unicode_codepoint < start_char)
						high = mid;
					else if ((stbtt_uint32)unicode_codepoint > end_char)
						low = mid + 1;
					else {
						stbtt_uint32 start_glyph = ttULONG(data + index_map + 16 + mid * 12 + 8);
						if (format == 12)
							return start_glyph + unicode_codepoint - start_char;
						else // format == 13
							return start_glyph;
					}
				}
				return 0; // not found
			}
			// @TODO
			STBTT_assert(0);
			return 0;
		}

		STBTT_DEF int stbtt_GetCodepointShape(const stbtt_fontinfo* info, int unicode_codepoint, stbtt_vertex** vertices)
		{
			return stbtt_GetGlyphShape(info, stbtt_FindGlyphIndex(info, unicode_codepoint), vertices);
		}

		static void stbtt_setvertex(stbtt_vertex* v, stbtt_uint8 type, stbtt_int32 x, stbtt_int32 y, stbtt_int32 cx, stbtt_int32 cy)
		{
			v->type = type;
			v->x = (stbtt_int16)x;
			v->y = (stbtt_int16)y;
			v->cx = (stbtt_int16)cx;
			v->cy = (stbtt_int16)cy;
		}

		static int stbtt__GetGlyfOffset(const stbtt_fontinfo* info, int glyph_index)
		{
			int g1, g2;

			STBTT_assert(!info->cff.size);

			if (glyph_index >= info->numGlyphs) return -1; // glyph index out of range
			if (info->indexToLocFormat >= 2)    return -1; // unknown index->glyph map format

			if (info->indexToLocFormat == 0) {
				g1 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2) * 2;
				g2 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2 + 2) * 2;
			}
			else {
				g1 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4);
				g2 = info->glyf + ttULONG(info->data + info->loca + glyph_index * 4 + 4);
			}

			return g1 == g2 ? -1 : g1; // if length is 0, return -1
		}

		static int stbtt__GetGlyphInfoT2(const stbtt_fontinfo* info, int glyph_index, int* x0, int* y0, int* x1, int* y1);

		STBTT_DEF int stbtt_GetGlyphBox(const stbtt_fontinfo* info, int glyph_index, int* x0, int* y0, int* x1, int* y1)
		{
			if (info->cff.size) {
				stbtt__GetGlyphInfoT2(info, glyph_index, x0, y0, x1, y1);
			}
			else {
				int g = stbtt__GetGlyfOffset(info, glyph_index);
				if (g < 0) return 0;

				if (x0) *x0 = ttSHORT(info->data + g + 2);
				if (y0) *y0 = ttSHORT(info->data + g + 4);
				if (x1) *x1 = ttSHORT(info->data + g + 6);
				if (y1) *y1 = ttSHORT(info->data + g + 8);
			}
			return 1;
		}

		STBTT_DEF int stbtt_GetCodepointBox(const stbtt_fontinfo* info, int codepoint, int* x0, int* y0, int* x1, int* y1)
		{
			return stbtt_GetGlyphBox(info, stbtt_FindGlyphIndex(info, codepoint), x0, y0, x1, y1);
		}

		STBTT_DEF int stbtt_IsGlyphEmpty(const stbtt_fontinfo* info, int glyph_index)
		{
			stbtt_int16 numberOfContours;
			int g;
			if (info->cff.size)
				return stbtt__GetGlyphInfoT2(info, glyph_index, NULL, NULL, NULL, NULL) == 0;
			g = stbtt__GetGlyfOffset(info, glyph_index);
			if (g < 0) return 1;
			numberOfContours = ttSHORT(info->data + g);
			return numberOfContours == 0;
		}

		static int stbtt__close_shape(stbtt_vertex* vertices, int num_vertices, int was_off, int start_off,
			stbtt_int32 sx, stbtt_int32 sy, stbtt_int32 scx, stbtt_int32 scy, stbtt_int32 cx, stbtt_int32 cy)
		{
			if (start_off) {
				if (was_off)
					stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + scx) >> 1, (cy + scy) >> 1, cx, cy);
				stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, scx, scy);
			}
			else {
				if (was_off)
					stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx, sy, cx, cy);
				else
					stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, sx, sy, 0, 0);
			}
			return num_vertices;
		}

		static int stbtt__GetGlyphShapeTT(const stbtt_fontinfo* info, int glyph_index, stbtt_vertex** pvertices)
		{
			stbtt_int16 numberOfContours;
			stbtt_uint8* endPtsOfContours;
			stbtt_uint8* data = info->data;
			stbtt_vertex* vertices = 0;
			int num_vertices = 0;
			int g = stbtt__GetGlyfOffset(info, glyph_index);

			*pvertices = NULL;

			if (g < 0) return 0;

			numberOfContours = ttSHORT(data + g);

			if (numberOfContours > 0) {
				stbtt_uint8 flags = 0, flagcount;
				stbtt_int32 ins, i, j = 0, m, n, next_move, was_off = 0, off, start_off = 0;
				stbtt_int32 x, y, cx, cy, sx, sy, scx, scy;
				stbtt_uint8* points;
				endPtsOfContours = (data + g + 10);
				ins = ttUSHORT(data + g + 10 + numberOfContours * 2);
				points = data + g + 10 + numberOfContours * 2 + 2 + ins;

				n = 1 + ttUSHORT(endPtsOfContours + numberOfContours * 2 - 2);

				m = n + 2 * numberOfContours;  // a loose bound on how many vertices we might need
				vertices = (stbtt_vertex*)STBTT_malloc(m * sizeof(vertices[0]), info->userdata);
				if (vertices == 0)
					return 0;

				next_move = 0;
				flagcount = 0;

				// in first pass, we load uninterpreted data into the allocated array
				// above, shifted to the end of the array so we won't overwrite it when
				// we create our final data starting from the front

				off = m - n; // starting offset for uninterpreted data, regardless of how m ends up being calculated

				// first load flags

				for (i = 0; i < n; ++i) {
					if (flagcount == 0) {
						flags = *points++;
						if (flags & 8)
							flagcount = *points++;
					}
					else
						--flagcount;
					vertices[off + i].type = flags;
				}

				// now load x coordinates
				x = 0;
				for (i = 0; i < n; ++i) {
					flags = vertices[off + i].type;
					if (flags & 2) {
						stbtt_int16 dx = *points++;
						x += (flags & 16) ? dx : -dx; // ???
					}
					else {
						if (!(flags & 16)) {
							x = x + (stbtt_int16)(points[0] * 256 + points[1]);
							points += 2;
						}
					}
					vertices[off + i].x = (stbtt_int16)x;
				}

				// now load y coordinates
				y = 0;
				for (i = 0; i < n; ++i) {
					flags = vertices[off + i].type;
					if (flags & 4) {
						stbtt_int16 dy = *points++;
						y += (flags & 32) ? dy : -dy; // ???
					}
					else {
						if (!(flags & 32)) {
							y = y + (stbtt_int16)(points[0] * 256 + points[1]);
							points += 2;
						}
					}
					vertices[off + i].y = (stbtt_int16)y;
				}

				// now convert them to our format
				num_vertices = 0;
				sx = sy = cx = cy = scx = scy = 0;
				for (i = 0; i < n; ++i) {
					flags = vertices[off + i].type;
					x = (stbtt_int16)vertices[off + i].x;
					y = (stbtt_int16)vertices[off + i].y;

					if (next_move == i) {
						if (i != 0)
							num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);

						// now start the new one
						start_off = !(flags & 1);
						if (start_off) {
							// if we start off with an off-curve point, then when we need to find a point on the curve
							// where we can start, and we need to save some state for when we wraparound.
							scx = x;
							scy = y;
							if (!(vertices[off + i + 1].type & 1)) {
								// next point is also a curve point, so interpolate an on-point curve
								sx = (x + (stbtt_int32)vertices[off + i + 1].x) >> 1;
								sy = (y + (stbtt_int32)vertices[off + i + 1].y) >> 1;
							}
							else {
								// otherwise just use the next point as our start point
								sx = (stbtt_int32)vertices[off + i + 1].x;
								sy = (stbtt_int32)vertices[off + i + 1].y;
								++i; // we're using point i+1 as the starting point, so skip it
							}
						}
						else {
							sx = x;
							sy = y;
						}
						stbtt_setvertex(&vertices[num_vertices++], STBTT_vmove, sx, sy, 0, 0);
						was_off = 0;
						next_move = 1 + ttUSHORT(endPtsOfContours + j * 2);
						++j;
					}
					else {
						if (!(flags & 1)) { // if it's a curve
							if (was_off) // two off-curve control points in a row means interpolate an on-curve midpoint
								stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx + x) >> 1, (cy + y) >> 1, cx, cy);
							cx = x;
							cy = y;
							was_off = 1;
						}
						else {
							if (was_off)
								stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, x, y, cx, cy);
							else
								stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, x, y, 0, 0);
							was_off = 0;
						}
					}
				}
				num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);
			}
			else if (numberOfContours < 0) {
				// Compound shapes.
				int more = 1;
				stbtt_uint8* comp = data + g + 10;
				num_vertices = 0;
				vertices = 0;
				while (more) {
					stbtt_uint16 flags, gidx;
					int comp_num_verts = 0, i;
					stbtt_vertex* comp_verts = 0, * tmp = 0;
					float mtx[6] = { 1,0,0,1,0,0 }, m, n;

					flags = ttSHORT(comp); comp += 2;
					gidx = ttSHORT(comp); comp += 2;

					if (flags & 2) { // XY values
						if (flags & 1) { // shorts
							mtx[4] = ttSHORT(comp); comp += 2;
							mtx[5] = ttSHORT(comp); comp += 2;
						}
						else {
							mtx[4] = ttCHAR(comp); comp += 1;
							mtx[5] = ttCHAR(comp); comp += 1;
						}
					}
					else {
						// @TODO handle matching point
						STBTT_assert(0);
					}
					if (flags & (1 << 3)) { // WE_HAVE_A_SCALE
						mtx[0] = mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
						mtx[1] = mtx[2] = 0;
					}
					else if (flags & (1 << 6)) { // WE_HAVE_AN_X_AND_YSCALE
						mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
						mtx[1] = mtx[2] = 0;
						mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
					}
					else if (flags & (1 << 7)) { // WE_HAVE_A_TWO_BY_TWO
						mtx[0] = ttSHORT(comp) / 16384.0f; comp += 2;
						mtx[1] = ttSHORT(comp) / 16384.0f; comp += 2;
						mtx[2] = ttSHORT(comp) / 16384.0f; comp += 2;
						mtx[3] = ttSHORT(comp) / 16384.0f; comp += 2;
					}

					// Find transformation scales.
					m = (float)STBTT_sqrt(mtx[0] * mtx[0] + mtx[1] * mtx[1]);
					n = (float)STBTT_sqrt(mtx[2] * mtx[2] + mtx[3] * mtx[3]);

					// Get indexed glyph.
					comp_num_verts = stbtt_GetGlyphShape(info, gidx, &comp_verts);
					if (comp_num_verts > 0) {
						// Transform vertices.
						for (i = 0; i < comp_num_verts; ++i) {
							stbtt_vertex* v = &comp_verts[i];
							stbtt_vertex_type x, y;
							x = v->x; y = v->y;
							v->x = (stbtt_vertex_type)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
							v->y = (stbtt_vertex_type)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
							x = v->cx; y = v->cy;
							v->cx = (stbtt_vertex_type)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
							v->cy = (stbtt_vertex_type)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
						}
						// Append vertices.
						tmp = (stbtt_vertex*)STBTT_malloc((num_vertices + comp_num_verts) * sizeof(stbtt_vertex), info->userdata);
						if (!tmp) {
							if (vertices) STBTT_free(vertices, info->userdata);
							if (comp_verts) STBTT_free(comp_verts, info->userdata);
							return 0;
						}
						if (num_vertices > 0 && vertices) STBTT_memcpy(tmp, vertices, num_vertices * sizeof(stbtt_vertex));
						STBTT_memcpy(tmp + num_vertices, comp_verts, comp_num_verts * sizeof(stbtt_vertex));
						if (vertices) STBTT_free(vertices, info->userdata);
						vertices = tmp;
						STBTT_free(comp_verts, info->userdata);
						num_vertices += comp_num_verts;
					}
					// More components ?
					more = flags & (1 << 5);
				}
			}
			else {
				// numberOfCounters == 0, do nothing
			}

			*pvertices = vertices;
			return num_vertices;
		}

		typedef struct
		{
			int bounds;
			int started;
			float first_x, first_y;
			float x, y;
			stbtt_int32 min_x, max_x, min_y, max_y;

			stbtt_vertex* pvertices;
			int num_vertices;
		} stbtt__csctx;

#define STBTT__CSCTX_INIT(bounds) {bounds,0, 0,0, 0,0, 0,0,0,0, NULL, 0}

		static void stbtt__track_vertex(stbtt__csctx* c, stbtt_int32 x, stbtt_int32 y)
		{
			if (x > c->max_x || !c->started) c->max_x = x;
			if (y > c->max_y || !c->started) c->max_y = y;
			if (x < c->min_x || !c->started) c->min_x = x;
			if (y < c->min_y || !c->started) c->min_y = y;
			c->started = 1;
		}

		static void stbtt__csctx_v(stbtt__csctx* c, stbtt_uint8 type, stbtt_int32 x, stbtt_int32 y, stbtt_int32 cx, stbtt_int32 cy, stbtt_int32 cx1, stbtt_int32 cy1)
		{
			if (c->bounds) {
				stbtt__track_vertex(c, x, y);
				if (type == STBTT_vcubic) {
					stbtt__track_vertex(c, cx, cy);
					stbtt__track_vertex(c, cx1, cy1);
				}
			}
			else {
				stbtt_setvertex(&c->pvertices[c->num_vertices], type, x, y, cx, cy);
				c->pvertices[c->num_vertices].cx1 = (stbtt_int16)cx1;
				c->pvertices[c->num_vertices].cy1 = (stbtt_int16)cy1;
			}
			c->num_vertices++;
		}

		static void stbtt__csctx_close_shape(stbtt__csctx* ctx)
		{
			if (ctx->first_x != ctx->x || ctx->first_y != ctx->y)
				stbtt__csctx_v(ctx, STBTT_vline, (int)ctx->first_x, (int)ctx->first_y, 0, 0, 0, 0);
		}

		static void stbtt__csctx_rmove_to(stbtt__csctx* ctx, float dx, float dy)
		{
			stbtt__csctx_close_shape(ctx);
			ctx->first_x = ctx->x = ctx->x + dx;
			ctx->first_y = ctx->y = ctx->y + dy;
			stbtt__csctx_v(ctx, STBTT_vmove, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
		}

		static void stbtt__csctx_rline_to(stbtt__csctx* ctx, float dx, float dy)
		{
			ctx->x += dx;
			ctx->y += dy;
			stbtt__csctx_v(ctx, STBTT_vline, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
		}

		static void stbtt__csctx_rccurve_to(stbtt__csctx* ctx, float dx1, float dy1, float dx2, float dy2, float dx3, float dy3)
		{
			float cx1 = ctx->x + dx1;
			float cy1 = ctx->y + dy1;
			float cx2 = cx1 + dx2;
			float cy2 = cy1 + dy2;
			ctx->x = cx2 + dx3;
			ctx->y = cy2 + dy3;
			stbtt__csctx_v(ctx, STBTT_vcubic, (int)ctx->x, (int)ctx->y, (int)cx1, (int)cy1, (int)cx2, (int)cy2);
		}

		static stbtt__buf stbtt__get_subr(stbtt__buf idx, int n)
		{
			int count = stbtt__cff_index_count(&idx);
			int bias = 107;
			if (count >= 33900)
				bias = 32768;
			else if (count >= 1240)
				bias = 1131;
			n += bias;
			if (n < 0 || n >= count)
				return stbtt__new_buf(NULL, 0);
			return stbtt__cff_index_get(idx, n);
		}

		static stbtt__buf stbtt__cid_get_glyph_subrs(const stbtt_fontinfo* info, int glyph_index)
		{
			stbtt__buf fdselect = info->fdselect;
			int nranges, start, end, v, fmt, fdselector = -1, i;

			stbtt__buf_seek(&fdselect, 0);
			fmt = stbtt__buf_get8(&fdselect);
			if (fmt == 0) {
				// untested
				stbtt__buf_skip(&fdselect, glyph_index);
				fdselector = stbtt__buf_get8(&fdselect);
			}
			else if (fmt == 3) {
				nranges = stbtt__buf_get16(&fdselect);
				start = stbtt__buf_get16(&fdselect);
				for (i = 0; i < nranges; i++) {
					v = stbtt__buf_get8(&fdselect);
					end = stbtt__buf_get16(&fdselect);
					if (glyph_index >= start && glyph_index < end) {
						fdselector = v;
						break;
					}
					start = end;
				}
			}
			if (fdselector == -1) stbtt__new_buf(NULL, 0);
			return stbtt__get_subrs(info->cff, stbtt__cff_index_get(info->fontdicts, fdselector));
		}

		static int stbtt__run_charstring(const stbtt_fontinfo* info, int glyph_index, stbtt__csctx* c)
		{
			int in_header = 1, maskbits = 0, subr_stack_height = 0, sp = 0, v, i, b0;
			int has_subrs = 0, clear_stack;
			float s[48];
			stbtt__buf subr_stack[10], subrs = info->subrs, b;
			float f;

#define STBTT__CSERR(s) (0)

			// this currently ignores the initial width value, which isn't needed if we have hmtx
			b = stbtt__cff_index_get(info->charstrings, glyph_index);
			while (b.cursor < b.size) {
				i = 0;
				clear_stack = 1;
				b0 = stbtt__buf_get8(&b);
				switch (b0) {
					// @TODO implement hinting
				case 0x13: // hintmask
				case 0x14: // cntrmask
					if (in_header)
						maskbits += (sp / 2); // implicit "vstem"
					in_header = 0;
					stbtt__buf_skip(&b, (maskbits + 7) / 8);
					break;

				case 0x01: // hstem
				case 0x03: // vstem
				case 0x12: // hstemhm
				case 0x17: // vstemhm
					maskbits += (sp / 2);
					break;

				case 0x15: // rmoveto
					in_header = 0;
					if (sp < 2) return STBTT__CSERR("rmoveto stack");
					stbtt__csctx_rmove_to(c, s[sp - 2], s[sp - 1]);
					break;
				case 0x04: // vmoveto
					in_header = 0;
					if (sp < 1) return STBTT__CSERR("vmoveto stack");
					stbtt__csctx_rmove_to(c, 0, s[sp - 1]);
					break;
				case 0x16: // hmoveto
					in_header = 0;
					if (sp < 1) return STBTT__CSERR("hmoveto stack");
					stbtt__csctx_rmove_to(c, s[sp - 1], 0);
					break;

				case 0x05: // rlineto
					if (sp < 2) return STBTT__CSERR("rlineto stack");
					for (; i + 1 < sp; i += 2)
						stbtt__csctx_rline_to(c, s[i], s[i + 1]);
					break;

					// hlineto/vlineto and vhcurveto/hvcurveto alternate horizontal and vertical
					// starting from a different place.

				case 0x07: // vlineto
					if (sp < 1) return STBTT__CSERR("vlineto stack");
					goto vlineto;
				case 0x06: // hlineto
					if (sp < 1) return STBTT__CSERR("hlineto stack");
					for (;;) {
						if (i >= sp) break;
						stbtt__csctx_rline_to(c, s[i], 0);
						i++;
					vlineto:
						if (i >= sp) break;
						stbtt__csctx_rline_to(c, 0, s[i]);
						i++;
					}
					break;

				case 0x1F: // hvcurveto
					if (sp < 4) return STBTT__CSERR("hvcurveto stack");
					goto hvcurveto;
				case 0x1E: // vhcurveto
					if (sp < 4) return STBTT__CSERR("vhcurveto stack");
					for (;;) {
						if (i + 3 >= sp) break;
						stbtt__csctx_rccurve_to(c, 0, s[i], s[i + 1], s[i + 2], s[i + 3], (sp - i == 5) ? s[i + 4] : 0.0f);
						i += 4;
					hvcurveto:
						if (i + 3 >= sp) break;
						stbtt__csctx_rccurve_to(c, s[i], 0, s[i + 1], s[i + 2], (sp - i == 5) ? s[i + 4] : 0.0f, s[i + 3]);
						i += 4;
					}
					break;

				case 0x08: // rrcurveto
					if (sp < 6) return STBTT__CSERR("rcurveline stack");
					for (; i + 5 < sp; i += 6)
						stbtt__csctx_rccurve_to(c, s[i], s[i + 1], s[i + 2], s[i + 3], s[i + 4], s[i + 5]);
					break;

				case 0x18: // rcurveline
					if (sp < 8) return STBTT__CSERR("rcurveline stack");
					for (; i + 5 < sp - 2; i += 6)
						stbtt__csctx_rccurve_to(c, s[i], s[i + 1], s[i + 2], s[i + 3], s[i + 4], s[i + 5]);
					if (i + 1 >= sp) return STBTT__CSERR("rcurveline stack");
					stbtt__csctx_rline_to(c, s[i], s[i + 1]);
					break;

				case 0x19: // rlinecurve
					if (sp < 8) return STBTT__CSERR("rlinecurve stack");
					for (; i + 1 < sp - 6; i += 2)
						stbtt__csctx_rline_to(c, s[i], s[i + 1]);
					if (i + 5 >= sp) return STBTT__CSERR("rlinecurve stack");
					stbtt__csctx_rccurve_to(c, s[i], s[i + 1], s[i + 2], s[i + 3], s[i + 4], s[i + 5]);
					break;

				case 0x1A: // vvcurveto
				case 0x1B: // hhcurveto
					if (sp < 4) return STBTT__CSERR("(vv|hh)curveto stack");
					f = 0.0;
					if (sp & 1) { f = s[i]; i++; }
					for (; i + 3 < sp; i += 4) {
						if (b0 == 0x1B)
							stbtt__csctx_rccurve_to(c, s[i], f, s[i + 1], s[i + 2], s[i + 3], 0.0);
						else
							stbtt__csctx_rccurve_to(c, f, s[i], s[i + 1], s[i + 2], 0.0, s[i + 3]);
						f = 0.0;
					}
					break;

				case 0x0A: // callsubr
					if (!has_subrs) {
						if (info->fdselect.size)
							subrs = stbtt__cid_get_glyph_subrs(info, glyph_index);
						has_subrs = 1;
					}
					// FALLTHROUGH
				case 0x1D: // callgsubr
					if (sp < 1) return STBTT__CSERR("call(g|)subr stack");
					v = (int)s[--sp];
					if (subr_stack_height >= 10) return STBTT__CSERR("recursion limit");
					subr_stack[subr_stack_height++] = b;
					b = stbtt__get_subr(b0 == 0x0A ? subrs : info->gsubrs, v);
					if (b.size == 0) return STBTT__CSERR("subr not found");
					b.cursor = 0;
					clear_stack = 0;
					break;

				case 0x0B: // return
					if (subr_stack_height <= 0) return STBTT__CSERR("return outside subr");
					b = subr_stack[--subr_stack_height];
					clear_stack = 0;
					break;

				case 0x0E: // endchar
					stbtt__csctx_close_shape(c);
					return 1;

				case 0x0C: { // two-byte escape
					float dx1, dx2, dx3, dx4, dx5, dx6, dy1, dy2, dy3, dy4, dy5, dy6;
					float dx, dy;
					int b1 = stbtt__buf_get8(&b);
					switch (b1) {
						// @TODO These "flex" implementations ignore the flex-depth and resolution,
						// and always draw beziers.
					case 0x22: // hflex
						if (sp < 7) return STBTT__CSERR("hflex stack");
						dx1 = s[0];
						dx2 = s[1];
						dy2 = s[2];
						dx3 = s[3];
						dx4 = s[4];
						dx5 = s[5];
						dx6 = s[6];
						stbtt__csctx_rccurve_to(c, dx1, 0, dx2, dy2, dx3, 0);
						stbtt__csctx_rccurve_to(c, dx4, 0, dx5, -dy2, dx6, 0);
						break;

					case 0x23: // flex
						if (sp < 13) return STBTT__CSERR("flex stack");
						dx1 = s[0];
						dy1 = s[1];
						dx2 = s[2];
						dy2 = s[3];
						dx3 = s[4];
						dy3 = s[5];
						dx4 = s[6];
						dy4 = s[7];
						dx5 = s[8];
						dy5 = s[9];
						dx6 = s[10];
						dy6 = s[11];
						//fd is s[12]
						stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
						stbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
						break;

					case 0x24: // hflex1
						if (sp < 9) return STBTT__CSERR("hflex1 stack");
						dx1 = s[0];
						dy1 = s[1];
						dx2 = s[2];
						dy2 = s[3];
						dx3 = s[4];
						dx4 = s[5];
						dx5 = s[6];
						dy5 = s[7];
						dx6 = s[8];
						stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, 0);
						stbtt__csctx_rccurve_to(c, dx4, 0, dx5, dy5, dx6, -(dy1 + dy2 + dy5));
						break;

					case 0x25: // flex1
						if (sp < 11) return STBTT__CSERR("flex1 stack");
						dx1 = s[0];
						dy1 = s[1];
						dx2 = s[2];
						dy2 = s[3];
						dx3 = s[4];
						dy3 = s[5];
						dx4 = s[6];
						dy4 = s[7];
						dx5 = s[8];
						dy5 = s[9];
						dx6 = dy6 = s[10];
						dx = dx1 + dx2 + dx3 + dx4 + dx5;
						dy = dy1 + dy2 + dy3 + dy4 + dy5;
						if (STBTT_fabs(dx) > STBTT_fabs(dy))
							dy6 = -dy;
						else
							dx6 = -dx;
						stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
						stbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
						break;

					default:
						return STBTT__CSERR("unimplemented");
					}
				} break;

				default:
					if (b0 != 255 && b0 != 28 && b0 < 32)
						return STBTT__CSERR("reserved operator");

					// push immediate
					if (b0 == 255) {
						f = (float)(stbtt_int32)stbtt__buf_get32(&b) / 0x10000;
					}
					else {
						stbtt__buf_skip(&b, -1);
						f = (float)(stbtt_int16)stbtt__cff_int(&b);
					}
					if (sp >= 48) return STBTT__CSERR("push stack overflow");
					s[sp++] = f;
					clear_stack = 0;
					break;
				}
				if (clear_stack) sp = 0;
			}
			return STBTT__CSERR("no endchar");

#undef STBTT__CSERR
		}

		static int stbtt__GetGlyphShapeT2(const stbtt_fontinfo* info, int glyph_index, stbtt_vertex** pvertices)
		{
			// runs the charstring twice, once to count and once to output (to avoid realloc)
			stbtt__csctx count_ctx = STBTT__CSCTX_INIT(1);
			stbtt__csctx output_ctx = STBTT__CSCTX_INIT(0);
			if (stbtt__run_charstring(info, glyph_index, &count_ctx)) {
				*pvertices = (stbtt_vertex*)STBTT_malloc(count_ctx.num_vertices * sizeof(stbtt_vertex), info->userdata);
				output_ctx.pvertices = *pvertices;
				if (stbtt__run_charstring(info, glyph_index, &output_ctx)) {
					STBTT_assert(output_ctx.num_vertices == count_ctx.num_vertices);
					return output_ctx.num_vertices;
				}
			}
			*pvertices = NULL;
			return 0;
		}

		static int stbtt__GetGlyphInfoT2(const stbtt_fontinfo* info, int glyph_index, int* x0, int* y0, int* x1, int* y1)
		{
			stbtt__csctx c = STBTT__CSCTX_INIT(1);
			int r = stbtt__run_charstring(info, glyph_index, &c);
			if (x0)  *x0 = r ? c.min_x : 0;
			if (y0)  *y0 = r ? c.min_y : 0;
			if (x1)  *x1 = r ? c.max_x : 0;
			if (y1)  *y1 = r ? c.max_y : 0;
			return r ? c.num_vertices : 0;
		}

		STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo* info, int glyph_index, stbtt_vertex** pvertices)
		{
			if (!info->cff.size)
				return stbtt__GetGlyphShapeTT(info, glyph_index, pvertices);
			else
				return stbtt__GetGlyphShapeT2(info, glyph_index, pvertices);
		}

		STBTT_DEF void stbtt_GetGlyphHMetrics(const stbtt_fontinfo* info, int glyph_index, int* advanceWidth, int* leftSideBearing)
		{
			stbtt_uint16 numOfLongHorMetrics = ttUSHORT(info->data + info->hhea + 34);
			if (glyph_index < numOfLongHorMetrics) {
				if (advanceWidth)     *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * glyph_index);
				if (leftSideBearing)  *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * glyph_index + 2);
			}
			else {
				if (advanceWidth)     *advanceWidth = ttSHORT(info->data + info->hmtx + 4 * (numOfLongHorMetrics - 1));
				if (leftSideBearing)  *leftSideBearing = ttSHORT(info->data + info->hmtx + 4 * numOfLongHorMetrics + 2 * (glyph_index - numOfLongHorMetrics));
			}
		}

		STBTT_DEF int  stbtt_GetKerningTableLength(const stbtt_fontinfo* info)
		{
			stbtt_uint8* data = info->data + info->kern;

			// we only look at the first table. it must be 'horizontal' and format 0.
			if (!info->kern)
				return 0;
			if (ttUSHORT(data + 2) < 1) // number of tables, need at least 1
				return 0;
			if (ttUSHORT(data + 8) != 1) // horizontal flag must be set in format
				return 0;

			return ttUSHORT(data + 10);
		}

		STBTT_DEF int stbtt_GetKerningTable(const stbtt_fontinfo* info, stbtt_kerningentry* table, int table_length)
		{
			stbtt_uint8* data = info->data + info->kern;
			int k, length;

			// we only look at the first table. it must be 'horizontal' and format 0.
			if (!info->kern)
				return 0;
			if (ttUSHORT(data + 2) < 1) // number of tables, need at least 1
				return 0;
			if (ttUSHORT(data + 8) != 1) // horizontal flag must be set in format
				return 0;

			length = ttUSHORT(data + 10);
			if (table_length < length)
				length = table_length;

			for (k = 0; k < length; k++)
			{
				table[k].glyph1 = ttUSHORT(data + 18 + (k * 6));
				table[k].glyph2 = ttUSHORT(data + 20 + (k * 6));
				table[k].advance = ttSHORT(data + 22 + (k * 6));
			}

			return length;
		}

		static int stbtt__GetGlyphKernInfoAdvance(const stbtt_fontinfo* info, int glyph1, int glyph2)
		{
			stbtt_uint8* data = info->data + info->kern;
			stbtt_uint32 needle, straw;
			int l, r, m;

			// we only look at the first table. it must be 'horizontal' and format 0.
			if (!info->kern)
				return 0;
			if (ttUSHORT(data + 2) < 1) // number of tables, need at least 1
				return 0;
			if (ttUSHORT(data + 8) != 1) // horizontal flag must be set in format
				return 0;

			l = 0;
			r = ttUSHORT(data + 10) - 1;
			needle = glyph1 << 16 | glyph2;
			while (l <= r) {
				m = (l + r) >> 1;
				straw = ttULONG(data + 18 + (m * 6)); // note: unaligned read
				if (needle < straw)
					r = m - 1;
				else if (needle > straw)
					l = m + 1;
				else
					return ttSHORT(data + 22 + (m * 6));
			}
			return 0;
		}

		static stbtt_int32 stbtt__GetCoverageIndex(stbtt_uint8* coverageTable, int glyph)
		{
			stbtt_uint16 coverageFormat = ttUSHORT(coverageTable);
			switch (coverageFormat) {
			case 1: {
				stbtt_uint16 glyphCount = ttUSHORT(coverageTable + 2);

				// Binary search.
				stbtt_int32 l = 0, r = glyphCount - 1, m;
				int straw, needle = glyph;
				while (l <= r) {
					stbtt_uint8* glyphArray = coverageTable + 4;
					stbtt_uint16 glyphID;
					m = (l + r) >> 1;
					glyphID = ttUSHORT(glyphArray + 2 * m);
					straw = glyphID;
					if (needle < straw)
						r = m - 1;
					else if (needle > straw)
						l = m + 1;
					else {
						return m;
					}
				}
				break;
			}

			case 2: {
				stbtt_uint16 rangeCount = ttUSHORT(coverageTable + 2);
				stbtt_uint8* rangeArray = coverageTable + 4;

				// Binary search.
				stbtt_int32 l = 0, r = rangeCount - 1, m;
				int strawStart, strawEnd, needle = glyph;
				while (l <= r) {
					stbtt_uint8* rangeRecord;
					m = (l + r) >> 1;
					rangeRecord = rangeArray + 6 * m;
					strawStart = ttUSHORT(rangeRecord);
					strawEnd = ttUSHORT(rangeRecord + 2);
					if (needle < strawStart)
						r = m - 1;
					else if (needle > strawEnd)
						l = m + 1;
					else {
						stbtt_uint16 startCoverageIndex = ttUSHORT(rangeRecord + 4);
						return startCoverageIndex + glyph - strawStart;
					}
				}
				break;
			}

			default: return -1; // unsupported
			}

			return -1;
		}

		static stbtt_int32  stbtt__GetGlyphClass(stbtt_uint8* classDefTable, int glyph)
		{
			stbtt_uint16 classDefFormat = ttUSHORT(classDefTable);
			switch (classDefFormat)
			{
			case 1: {
				stbtt_uint16 startGlyphID = ttUSHORT(classDefTable + 2);
				stbtt_uint16 glyphCount = ttUSHORT(classDefTable + 4);
				stbtt_uint8* classDef1ValueArray = classDefTable + 6;

				if (glyph >= startGlyphID && glyph < startGlyphID + glyphCount)
					return (stbtt_int32)ttUSHORT(classDef1ValueArray + 2 * (glyph - startGlyphID));
				break;
			}

			case 2: {
				stbtt_uint16 classRangeCount = ttUSHORT(classDefTable + 2);
				stbtt_uint8* classRangeRecords = classDefTable + 4;

				// Binary search.
				stbtt_int32 l = 0, r = classRangeCount - 1, m;
				int strawStart, strawEnd, needle = glyph;
				while (l <= r) {
					stbtt_uint8* classRangeRecord;
					m = (l + r) >> 1;
					classRangeRecord = classRangeRecords + 6 * m;
					strawStart = ttUSHORT(classRangeRecord);
					strawEnd = ttUSHORT(classRangeRecord + 2);
					if (needle < strawStart)
						r = m - 1;
					else if (needle > strawEnd)
						l = m + 1;
					else
						return (stbtt_int32)ttUSHORT(classRangeRecord + 4);
				}
				break;
			}

			default:
				return -1; // Unsupported definition type, return an error.
			}

			// "All glyphs not assigned to a class fall into class 0". (OpenType spec)
			return 0;
		}

		// Define to STBTT_assert(x) if you want to break on unimplemented formats.
#define STBTT_GPOS_TODO_assert(x)

		static stbtt_int32 stbtt__GetGlyphGPOSInfoAdvance(const stbtt_fontinfo* info, int glyph1, int glyph2)
		{
			stbtt_uint16 lookupListOffset;
			stbtt_uint8* lookupList;
			stbtt_uint16 lookupCount;
			stbtt_uint8* data;
			stbtt_int32 i, sti;

			if (!info->gpos) return 0;

			data = info->data + info->gpos;

			if (ttUSHORT(data + 0) != 1) return 0; // Major version 1
			if (ttUSHORT(data + 2) != 0) return 0; // Minor version 0

			lookupListOffset = ttUSHORT(data + 8);
			lookupList = data + lookupListOffset;
			lookupCount = ttUSHORT(lookupList);

			for (i = 0; i < lookupCount; ++i) {
				stbtt_uint16 lookupOffset = ttUSHORT(lookupList + 2 + 2 * i);
				stbtt_uint8* lookupTable = lookupList + lookupOffset;

				stbtt_uint16 lookupType = ttUSHORT(lookupTable);
				stbtt_uint16 subTableCount = ttUSHORT(lookupTable + 4);
				stbtt_uint8* subTableOffsets = lookupTable + 6;
				if (lookupType != 2) // Pair Adjustment Positioning Subtable
					continue;

				for (sti = 0; sti < subTableCount; sti++) {
					stbtt_uint16 subtableOffset = ttUSHORT(subTableOffsets + 2 * sti);
					stbtt_uint8* table = lookupTable + subtableOffset;
					stbtt_uint16 posFormat = ttUSHORT(table);
					stbtt_uint16 coverageOffset = ttUSHORT(table + 2);
					stbtt_int32 coverageIndex = stbtt__GetCoverageIndex(table + coverageOffset, glyph1);
					if (coverageIndex == -1) continue;

					switch (posFormat) {
					case 1: {
						stbtt_int32 l, r, m;
						int straw, needle;
						stbtt_uint16 valueFormat1 = ttUSHORT(table + 4);
						stbtt_uint16 valueFormat2 = ttUSHORT(table + 6);
						if (valueFormat1 == 4 && valueFormat2 == 0) { // Support more formats?
							stbtt_int32 valueRecordPairSizeInBytes = 2;
							stbtt_uint16 pairSetCount = ttUSHORT(table + 8);
							stbtt_uint16 pairPosOffset = ttUSHORT(table + 10 + 2 * coverageIndex);
							stbtt_uint8* pairValueTable = table + pairPosOffset;
							stbtt_uint16 pairValueCount = ttUSHORT(pairValueTable);
							stbtt_uint8* pairValueArray = pairValueTable + 2;

							if (coverageIndex >= pairSetCount) return 0;

							needle = glyph2;
							r = pairValueCount - 1;
							l = 0;

							// Binary search.
							while (l <= r) {
								stbtt_uint16 secondGlyph;
								stbtt_uint8* pairValue;
								m = (l + r) >> 1;
								pairValue = pairValueArray + (2 + valueRecordPairSizeInBytes) * m;
								secondGlyph = ttUSHORT(pairValue);
								straw = secondGlyph;
								if (needle < straw)
									r = m - 1;
								else if (needle > straw)
									l = m + 1;
								else {
									stbtt_int16 xAdvance = ttSHORT(pairValue + 2);
									return xAdvance;
								}
							}
						}
						else
							return 0;
						break;
					}

					case 2: {
						stbtt_uint16 valueFormat1 = ttUSHORT(table + 4);
						stbtt_uint16 valueFormat2 = ttUSHORT(table + 6);
						if (valueFormat1 == 4 && valueFormat2 == 0) { // Support more formats?
							stbtt_uint16 classDef1Offset = ttUSHORT(table + 8);
							stbtt_uint16 classDef2Offset = ttUSHORT(table + 10);
							int glyph1class = stbtt__GetGlyphClass(table + classDef1Offset, glyph1);
							int glyph2class = stbtt__GetGlyphClass(table + classDef2Offset, glyph2);

							stbtt_uint16 class1Count = ttUSHORT(table + 12);
							stbtt_uint16 class2Count = ttUSHORT(table + 14);
							stbtt_uint8* class1Records, * class2Records;
							stbtt_int16 xAdvance;

							if (glyph1class < 0 || glyph1class >= class1Count) return 0; // malformed
							if (glyph2class < 0 || glyph2class >= class2Count) return 0; // malformed

							class1Records = table + 16;
							class2Records = class1Records + 2 * (glyph1class * class2Count);
							xAdvance = ttSHORT(class2Records + 2 * glyph2class);
							return xAdvance;
						}
						else
							return 0;
						break;
					}

					default:
						return 0; // Unsupported position format
					}
				}
			}

			return 0;
		}

		STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo* info, int g1, int g2)
		{
			int xAdvance = 0;

			if (info->gpos)
				xAdvance += stbtt__GetGlyphGPOSInfoAdvance(info, g1, g2);
			else if (info->kern)
				xAdvance += stbtt__GetGlyphKernInfoAdvance(info, g1, g2);

			return xAdvance;
		}

		STBTT_DEF int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo* info, int ch1, int ch2)
		{
			if (!info->kern && !info->gpos) // if no kerning table, don't waste time looking up both codepoint->glyphs
				return 0;
			return stbtt_GetGlyphKernAdvance(info, stbtt_FindGlyphIndex(info, ch1), stbtt_FindGlyphIndex(info, ch2));
		}

		STBTT_DEF void stbtt_GetCodepointHMetrics(const stbtt_fontinfo* info, int codepoint, int* advanceWidth, int* leftSideBearing)
		{
			stbtt_GetGlyphHMetrics(info, stbtt_FindGlyphIndex(info, codepoint), advanceWidth, leftSideBearing);
		}

		STBTT_DEF void stbtt_GetFontVMetrics(const stbtt_fontinfo* info, int* ascent, int* descent, int* lineGap)
		{
			if (ascent) *ascent = ttSHORT(info->data + info->hhea + 4);
			if (descent) *descent = ttSHORT(info->data + info->hhea + 6);
			if (lineGap) *lineGap = ttSHORT(info->data + info->hhea + 8);
		}

		STBTT_DEF int  stbtt_GetFontVMetricsOS2(const stbtt_fontinfo* info, int* typoAscent, int* typoDescent, int* typoLineGap)
		{
			int tab = stbtt__find_table(info->data, info->fontstart, "OS/2");
			if (!tab)
				return 0;
			if (typoAscent) *typoAscent = ttSHORT(info->data + tab + 68);
			if (typoDescent) *typoDescent = ttSHORT(info->data + tab + 70);
			if (typoLineGap) *typoLineGap = ttSHORT(info->data + tab + 72);
			return 1;
		}

		STBTT_DEF void stbtt_GetFontBoundingBox(const stbtt_fontinfo* info, int* x0, int* y0, int* x1, int* y1)
		{
			*x0 = ttSHORT(info->data + info->head + 36);
			*y0 = ttSHORT(info->data + info->head + 38);
			*x1 = ttSHORT(info->data + info->head + 40);
			*y1 = ttSHORT(info->data + info->head + 42);
		}

		STBTT_DEF float stbtt_ScaleForPixelHeight(const stbtt_fontinfo* info, float height)
		{
			int fheight = ttSHORT(info->data + info->hhea + 4) - ttSHORT(info->data + info->hhea + 6);
			return (float)height / fheight;
		}

		STBTT_DEF float stbtt_ScaleForMappingEmToPixels(const stbtt_fontinfo* info, float pixels)
		{
			int unitsPerEm = ttUSHORT(info->data + info->head + 18);
			return pixels / unitsPerEm;
		}

		STBTT_DEF void stbtt_FreeShape(const stbtt_fontinfo* info, stbtt_vertex* v)
		{
			STBTT_free(v, info->userdata);
		}

		STBTT_DEF stbtt_uint8* stbtt_FindSVGDoc(const stbtt_fontinfo* info, int gl)
		{
			int i;
			stbtt_uint8* data = info->data;
			stbtt_uint8* svg_doc_list = data + stbtt__get_svg((stbtt_fontinfo*)info);

			int numEntries = ttUSHORT(svg_doc_list);
			stbtt_uint8* svg_docs = svg_doc_list + 2;

			for (i = 0; i < numEntries; i++) {
				stbtt_uint8* svg_doc = svg_docs + (12 * i);
				if ((gl >= ttUSHORT(svg_doc)) && (gl <= ttUSHORT(svg_doc + 2)))
					return svg_doc;
			}
			return 0;
		}

		STBTT_DEF int stbtt_GetGlyphSVG(const stbtt_fontinfo* info, int gl, const char** svg)
		{
			stbtt_uint8* data = info->data;
			stbtt_uint8* svg_doc;

			if (info->svg == 0)
				return 0;

			svg_doc = stbtt_FindSVGDoc(info, gl);
			if (svg_doc != NULL) {
				*svg = (char*)data + info->svg + ttULONG(svg_doc + 4);
				return ttULONG(svg_doc + 8);
			}
			else {
				return 0;
			}
		}

		STBTT_DEF int stbtt_GetCodepointSVG(const stbtt_fontinfo* info, int unicode_codepoint, const char** svg)
		{
			return stbtt_GetGlyphSVG(info, stbtt_FindGlyphIndex(info, unicode_codepoint), svg);
		}

		//////////////////////////////////////////////////////////////////////////////
		//
		// antialiasing software rasterizer
		//

		STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo* font, int glyph, float scale_x, float scale_y, float shift_x, float shift_y, int* ix0, int* iy0, int* ix1, int* iy1)
		{
			int x0 = 0, y0 = 0, x1, y1; // =0 suppresses compiler warning
			if (!stbtt_GetGlyphBox(font, glyph, &x0, &y0, &x1, &y1)) {
				// e.g. space character
				if (ix0) *ix0 = 0;
				if (iy0) *iy0 = 0;
				if (ix1) *ix1 = 0;
				if (iy1) *iy1 = 0;
			}
			else {
				// move to integral bboxes (treating pixels as little squares, what pixels get touched)?
				if (ix0) *ix0 = STBTT_ifloor(x0 * scale_x + shift_x);
				if (iy0) *iy0 = STBTT_ifloor(-y1 * scale_y + shift_y);
				if (ix1) *ix1 = STBTT_iceil(x1 * scale_x + shift_x);
				if (iy1) *iy1 = STBTT_iceil(-y0 * scale_y + shift_y);
			}
		}

		STBTT_DEF void stbtt_GetGlyphBitmapBox(const stbtt_fontinfo* font, int glyph, float scale_x, float scale_y, int* ix0, int* iy0, int* ix1, int* iy1)
		{
			stbtt_GetGlyphBitmapBoxSubpixel(font, glyph, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
		}

		STBTT_DEF void stbtt_GetCodepointBitmapBoxSubpixel(const stbtt_fontinfo* font, int codepoint, float scale_x, float scale_y, float shift_x, float shift_y, int* ix0, int* iy0, int* ix1, int* iy1)
		{
			stbtt_GetGlyphBitmapBoxSubpixel(font, stbtt_FindGlyphIndex(font, codepoint), scale_x, scale_y, shift_x, shift_y, ix0, iy0, ix1, iy1);
		}

		STBTT_DEF void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo* font, int codepoint, float scale_x, float scale_y, int* ix0, int* iy0, int* ix1, int* iy1)
		{
			stbtt_GetCodepointBitmapBoxSubpixel(font, codepoint, scale_x, scale_y, 0.0f, 0.0f, ix0, iy0, ix1, iy1);
		}

		//////////////////////////////////////////////////////////////////////////////
		//
		//  Rasterizer

		typedef struct stbtt__hheap_chunk
		{
			struct stbtt__hheap_chunk* next;
		} stbtt__hheap_chunk;

		typedef struct stbtt__hheap
		{
			struct stbtt__hheap_chunk* head;
			void* first_free;
			int    num_remaining_in_head_chunk;
		} stbtt__hheap;

		static void* stbtt__hheap_alloc(stbtt__hheap* hh, size_t size, void* userdata)
		{
			if (hh->first_free) {
				void* p = hh->first_free;
				hh->first_free = *(void**)p;
				return p;
			}
			else {
				if (hh->num_remaining_in_head_chunk == 0) {
					int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
					stbtt__hheap_chunk* c = (stbtt__hheap_chunk*)STBTT_malloc(sizeof(stbtt__hheap_chunk) + size * count, userdata);
					if (c == NULL)
						return NULL;
					c->next = hh->head;
					hh->head = c;
					hh->num_remaining_in_head_chunk = count;
				}
				--hh->num_remaining_in_head_chunk;
				return (char*)(hh->head) + sizeof(stbtt__hheap_chunk) + size * hh->num_remaining_in_head_chunk;
			}
		}

		static void stbtt__hheap_free(stbtt__hheap* hh, void* p)
		{
			*(void**)p = hh->first_free;
			hh->first_free = p;
		}

		static void stbtt__hheap_cleanup(stbtt__hheap* hh, void* userdata)
		{
			stbtt__hheap_chunk* c = hh->head;
			while (c) {
				stbtt__hheap_chunk* n = c->next;
				STBTT_free(c, userdata);
				c = n;
			}
		}

		typedef struct stbtt__edge {
			float x0, y0, x1, y1;
			int invert;
		} stbtt__edge;


		typedef struct stbtt__active_edge
		{
			struct stbtt__active_edge* next;
#if STBTT_RASTERIZER_VERSION==1
			int x, dx;
			float ey;
			int direction;
#elif STBTT_RASTERIZER_VERSION==2
			float fx, fdx, fdy;
			float direction;
			float sy;
			float ey;
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif
		} stbtt__active_edge;

#if STBTT_RASTERIZER_VERSION == 1
#define STBTT_FIXSHIFT   10
#define STBTT_FIX        (1 << STBTT_FIXSHIFT)
#define STBTT_FIXMASK    (STBTT_FIX-1)

		static stbtt__active_edge* stbtt__new_active(stbtt__hheap* hh, stbtt__edge* e, int off_x, float start_point, void* userdata)
		{
			stbtt__active_edge* z = (stbtt__active_edge*)stbtt__hheap_alloc(hh, sizeof(*z), userdata);
			float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
			STBTT_assert(z != NULL);
			if (!z) return z;

			// round dx down to avoid overshooting
			if (dxdy < 0)
				z->dx = -STBTT_ifloor(STBTT_FIX * -dxdy);
			else
				z->dx = STBTT_ifloor(STBTT_FIX * dxdy);

			z->x = STBTT_ifloor(STBTT_FIX * e->x0 + z->dx * (start_point - e->y0)); // use z->dx so when we offset later it's by the same amount
			z->x -= off_x * STBTT_FIX;

			z->ey = e->y1;
			z->next = 0;
			z->direction = e->invert ? 1 : -1;
			return z;
		}
#elif STBTT_RASTERIZER_VERSION == 2
		static stbtt__active_edge* stbtt__new_active(stbtt__hheap* hh, stbtt__edge* e, int off_x, float start_point, void* userdata)
		{
			stbtt__active_edge* z = (stbtt__active_edge*)stbtt__hheap_alloc(hh, sizeof(*z), userdata);
			float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
			STBTT_assert(z != NULL);
			//STBTT_assert(e->y0 <= start_point);
			if (!z) return z;
			z->fdx = dxdy;
			z->fdy = dxdy != 0.0f ? (1.0f / dxdy) : 0.0f;
			z->fx = e->x0 + dxdy * (start_point - e->y0);
			z->fx -= off_x;
			z->direction = e->invert ? 1.0f : -1.0f;
			z->sy = e->y0;
			z->ey = e->y1;
			z->next = 0;
			return z;
		}
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

#if STBTT_RASTERIZER_VERSION == 1
		// note: this routine clips fills that extend off the edges... ideally this
		// wouldn't happen, but it could happen if the truetype glyph bounding boxes
		// are wrong, or if the user supplies a too-small bitmap
		static void stbtt__fill_active_edges(unsigned char* scanline, int len, stbtt__active_edge* e, int max_weight)
		{
			// non-zero winding fill
			int x0 = 0, w = 0;

			while (e) {
				if (w == 0) {
					// if we're currently at zero, we need to record the edge start point
					x0 = e->x; w += e->direction;
				}
				else {
					int x1 = e->x; w += e->direction;
					// if we went to zero, we need to draw
					if (w == 0) {
						int i = x0 >> STBTT_FIXSHIFT;
						int j = x1 >> STBTT_FIXSHIFT;

						if (i < len && j >= 0) {
							if (i == j) {
								// x0,x1 are the same pixel, so compute combined coverage
								scanline[i] = scanline[i] + (stbtt_uint8)((x1 - x0) * max_weight >> STBTT_FIXSHIFT);
							}
							else {
								if (i >= 0) // add antialiasing for x0
									scanline[i] = scanline[i] + (stbtt_uint8)(((STBTT_FIX - (x0 & STBTT_FIXMASK)) * max_weight) >> STBTT_FIXSHIFT);
								else
									i = -1; // clip

								if (j < len) // add antialiasing for x1
									scanline[j] = scanline[j] + (stbtt_uint8)(((x1 & STBTT_FIXMASK) * max_weight) >> STBTT_FIXSHIFT);
								else
									j = len; // clip

								for (++i; i < j; ++i) // fill pixels between x0 and x1
									scanline[i] = scanline[i] + (stbtt_uint8)max_weight;
							}
						}
					}
				}

				e = e->next;
			}
		}

		static void stbtt__rasterize_sorted_edges(stbtt__bitmap* result, stbtt__edge* e, int n, int vsubsample, int off_x, int off_y, void* userdata)
		{
			stbtt__hheap hh = { 0, 0, 0 };
			stbtt__active_edge* active = NULL;
			int y, j = 0;
			int max_weight = (255 / vsubsample);  // weight per vertical scanline
			int s; // vertical subsample index
			unsigned char scanline_data[512], * scanline;

			if (result->w > 512)
				scanline = (unsigned char*)STBTT_malloc(result->w, userdata);
			else
				scanline = scanline_data;

			y = off_y * vsubsample;
			e[n].y0 = (off_y + result->h) * (float)vsubsample + 1;

			while (j < result->h) {
				STBTT_memset(scanline, 0, result->w);
				for (s = 0; s < vsubsample; ++s) {
					// find center of pixel for this scanline
					float scan_y = y + 0.5f;
					stbtt__active_edge** step = &active;

					// update all active edges;
					// remove all active edges that terminate before the center of this scanline
					while (*step) {
						stbtt__active_edge* z = *step;
						if (z->ey <= scan_y) {
							*step = z->next; // delete from list
							STBTT_assert(z->direction);
							z->direction = 0;
							stbtt__hheap_free(&hh, z);
						}
						else {
							z->x += z->dx; // advance to position for current scanline
							step = &((*step)->next); // advance through list
						}
					}

					// resort the list if needed
					for (;;) {
						int changed = 0;
						step = &active;
						while (*step && (*step)->next) {
							if ((*step)->x > (*step)->next->x) {
								stbtt__active_edge* t = *step;
								stbtt__active_edge* q = t->next;

								t->next = q->next;
								q->next = t;
								*step = q;
								changed = 1;
							}
							step = &(*step)->next;
						}
						if (!changed) break;
					}

					// insert all edges that start before the center of this scanline -- omit ones that also end on this scanline
					while (e->y0 <= scan_y) {
						if (e->y1 > scan_y) {
							stbtt__active_edge* z = stbtt__new_active(&hh, e, off_x, scan_y, userdata);
							if (z != NULL) {
								// find insertion point
								if (active == NULL)
									active = z;
								else if (z->x < active->x) {
									// insert at front
									z->next = active;
									active = z;
								}
								else {
									// find thing to insert AFTER
									stbtt__active_edge* p = active;
									while (p->next && p->next->x < z->x)
										p = p->next;
									// at this point, p->next->x is NOT < z->x
									z->next = p->next;
									p->next = z;
								}
							}
						}
						++e;
					}

					// now process all active edges in XOR fashion
					if (active)
						stbtt__fill_active_edges(scanline, result->w, active, max_weight);

					++y;
				}
				STBTT_memcpy(result->pixels + j * result->stride, scanline, result->w);
				++j;
			}

			stbtt__hheap_cleanup(&hh, userdata);

			if (scanline != scanline_data)
				STBTT_free(scanline, userdata);
		}

#elif STBTT_RASTERIZER_VERSION == 2

		// the edge passed in here does not cross the vertical line at x or the vertical line at x+1
		// (i.e. it has already been clipped to those)
		static void stbtt__handle_clipped_edge(float* scanline, int x, stbtt__active_edge* e, float x0, float y0, float x1, float y1)
		{
			if (y0 == y1) return;
			STBTT_assert(y0 < y1);
			STBTT_assert(e->sy <= e->ey);
			if (y0 > e->ey) return;
			if (y1 < e->sy) return;
			if (y0 < e->sy) {
				x0 += (x1 - x0) * (e->sy - y0) / (y1 - y0);
				y0 = e->sy;
			}
			if (y1 > e->ey) {
				x1 += (x1 - x0) * (e->ey - y1) / (y1 - y0);
				y1 = e->ey;
			}

			if (x0 == x)
				STBTT_assert(x1 <= x + 1);
			else if (x0 == x + 1)
				STBTT_assert(x1 >= x);
			else if (x0 <= x)
				STBTT_assert(x1 <= x);
			else if (x0 >= x + 1)
				STBTT_assert(x1 >= x + 1);
			else
				STBTT_assert(x1 >= x && x1 <= x + 1);

			if (x0 <= x && x1 <= x)
				scanline[x] += e->direction * (y1 - y0);
			else if (x0 >= x + 1 && x1 >= x + 1)
				;
			else {
				STBTT_assert(x0 >= x && x0 <= x + 1 && x1 >= x && x1 <= x + 1);
				scanline[x] += e->direction * (y1 - y0) * (1 - ((x0 - x) + (x1 - x)) / 2); // coverage = 1 - average x position
			}
		}

		static float stbtt__sized_trapezoid_area(float height, float top_width, float bottom_width)
		{
			STBTT_assert(top_width >= 0);
			STBTT_assert(bottom_width >= 0);
			return (top_width + bottom_width) / 2.0f * height;
		}

		static float stbtt__position_trapezoid_area(float height, float tx0, float tx1, float bx0, float bx1)
		{
			return stbtt__sized_trapezoid_area(height, tx1 - tx0, bx1 - bx0);
		}

		static float stbtt__sized_triangle_area(float height, float width)
		{
			return height * width / 2;
		}

		static void stbtt__fill_active_edges_new(float* scanline, float* scanline_fill, int len, stbtt__active_edge* e, float y_top)
		{
			float y_bottom = y_top + 1;

			while (e) {
				// brute force every pixel

				// compute intersection points with top & bottom
				STBTT_assert(e->ey >= y_top);

				if (e->fdx == 0) {
					float x0 = e->fx;
					if (x0 < len) {
						if (x0 >= 0) {
							stbtt__handle_clipped_edge(scanline, (int)x0, e, x0, y_top, x0, y_bottom);
							stbtt__handle_clipped_edge(scanline_fill - 1, (int)x0 + 1, e, x0, y_top, x0, y_bottom);
						}
						else {
							stbtt__handle_clipped_edge(scanline_fill - 1, 0, e, x0, y_top, x0, y_bottom);
						}
					}
				}
				else {
					float x0 = e->fx;
					float dx = e->fdx;
					float xb = x0 + dx;
					float x_top, x_bottom;
					float sy0, sy1;
					float dy = e->fdy;
					STBTT_assert(e->sy <= y_bottom && e->ey >= y_top);

					// compute endpoints of line segment clipped to this scanline (if the
					// line segment starts on this scanline. x0 is the intersection of the
					// line with y_top, but that may be off the line segment.
					if (e->sy > y_top) {
						x_top = x0 + dx * (e->sy - y_top);
						sy0 = e->sy;
					}
					else {
						x_top = x0;
						sy0 = y_top;
					}
					if (e->ey < y_bottom) {
						x_bottom = x0 + dx * (e->ey - y_top);
						sy1 = e->ey;
					}
					else {
						x_bottom = xb;
						sy1 = y_bottom;
					}

					if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {
						// from here on, we don't have to range check x values

						if ((int)x_top == (int)x_bottom) {
							float height;
							// simple case, only spans one pixel
							int x = (int)x_top;
							height = (sy1 - sy0) * e->direction;
							STBTT_assert(x >= 0 && x < len);
							scanline[x] += stbtt__position_trapezoid_area(height, x_top, x + 1.0f, x_bottom, x + 1.0f);
							scanline_fill[x] += height; // everything right of this pixel is filled
						}
						else {
							int x, x1, x2;
							float y_crossing, y_final, step, sign, area;
							// covers 2+ pixels
							if (x_top > x_bottom) {
								// flip scanline vertically; signed area is the same
								float t;
								sy0 = y_bottom - (sy0 - y_top);
								sy1 = y_bottom - (sy1 - y_top);
								t = sy0, sy0 = sy1, sy1 = t;
								t = x_bottom, x_bottom = x_top, x_top = t;
								dx = -dx;
								dy = -dy;
								t = x0, x0 = xb, xb = t;
							}
							STBTT_assert(dy >= 0);
							STBTT_assert(dx >= 0);

							x1 = (int)x_top;
							x2 = (int)x_bottom;
							// compute intersection with y axis at x1+1
							y_crossing = y_top + dy * (x1 + 1 - x0);

							// compute intersection with y axis at x2
							y_final = y_top + dy * (x2 - x0);

							//           x1    x_top                            x2    x_bottom
							//     y_top  +------|-----+------------+------------+--------|---+------------+
							//            |            |            |            |            |            |
							//            |            |            |            |            |            |
							//       sy0  |      Txxxxx|............|............|............|............|
							// y_crossing |            *xxxxx.......|............|............|............|
							//            |            |     xxxxx..|............|............|............|
							//            |            |     /-   xx*xxxx........|............|............|
							//            |            | dy <       |    xxxxxx..|............|............|
							//   y_final  |            |     \-     |          xx*xxx.........|............|
							//       sy1  |            |            |            |   xxxxxB...|............|
							//            |            |            |            |            |            |
							//            |            |            |            |            |            |
							//  y_bottom  +------------+------------+------------+------------+------------+
							//
							// goal is to measure the area covered by '.' in each pixel

							// if x2 is right at the right edge of x1, y_crossing can blow up, github #1057
							// @TODO: maybe test against sy1 rather than y_bottom?
							if (y_crossing > y_bottom)
								y_crossing = y_bottom;

							sign = e->direction;

							// area of the rectangle covered from sy0..y_crossing
							area = sign * (y_crossing - sy0);

							// area of the triangle (x_top,sy0), (x1+1,sy0), (x1+1,y_crossing)
							scanline[x1] += stbtt__sized_triangle_area(area, x1 + 1 - x_top);

							// check if final y_crossing is blown up; no test case for this
							if (y_final > y_bottom) {
								y_final = y_bottom;
								dy = (y_final - y_crossing) / (x2 - (x1 + 1)); // if denom=0, y_final = y_crossing, so y_final <= y_bottom
							}

							// in second pixel, area covered by line segment found in first pixel
							// is always a rectangle 1 wide * the height of that line segment; this
							// is exactly what the variable 'area' stores. it also gets a contribution
							// from the line segment within it. the THIRD pixel will get the first
							// pixel's rectangle contribution, the second pixel's rectangle contribution,
							// and its own contribution. the 'own contribution' is the same in every pixel except
							// the leftmost and rightmost, a trapezoid that slides down in each pixel.
							// the second pixel's contribution to the third pixel will be the
							// rectangle 1 wide times the height change in the second pixel, which is dy.

							step = sign * dy * 1; // dy is dy/dx, change in y for every 1 change in x,
							// which multiplied by 1-pixel-width is how much pixel area changes for each step in x
							// so the area advances by 'step' every time

							for (x = x1 + 1; x < x2; ++x) {
								scanline[x] += area + step / 2; // area of trapezoid is 1*step/2
								area += step;
							}
							STBTT_assert(STBTT_fabs(area) <= 1.01f); // accumulated error from area += step unless we round step down
							STBTT_assert(sy1 > y_final - 0.01f);

							// area covered in the last pixel is the rectangle from all the pixels to the left,
							// plus the trapezoid filled by the line segment in this pixel all the way to the right edge
							scanline[x2] += area + sign * stbtt__position_trapezoid_area(sy1 - y_final, (float)x2, x2 + 1.0f, x_bottom, x2 + 1.0f);

							// the rest of the line is filled based on the total height of the line segment in this pixel
							scanline_fill[x2] += sign * (sy1 - sy0);
						}
					}
					else {
						// if edge goes outside of box we're drawing, we require
						// clipping logic. since this does not match the intended use
						// of this library, we use a different, very slow brute
						// force implementation
						// note though that this does happen some of the time because
						// x_top and x_bottom can be extrapolated at the top & bottom of
						// the shape and actually lie outside the bounding box
						int x;
						for (x = 0; x < len; ++x) {
							// cases:
							//
							// there can be up to two intersections with the pixel. any intersection
							// with left or right edges can be handled by splitting into two (or three)
							// regions. intersections with top & bottom do not necessitate case-wise logic.
							//
							// the old way of doing this found the intersections with the left & right edges,
							// then used some simple logic to produce up to three segments in sorted order
							// from top-to-bottom. however, this had a problem: if an x edge was epsilon
							// across the x border, then the corresponding y position might not be distinct
							// from the other y segment, and it might ignored as an empty segment. to avoid
							// that, we need to explicitly produce segments based on x positions.

							// rename variables to clearly-defined pairs
							float y0 = y_top;
							float x1 = (float)(x);
							float x2 = (float)(x + 1);
							float x3 = xb;
							float y3 = y_bottom;

							// x = e->x + e->dx * (y-y_top)
							// (y-y_top) = (x - e->x) / e->dx
							// y = (x - e->x) / e->dx + y_top
							float y1 = (x - x0) / dx + y_top;
							float y2 = (x + 1 - x0) / dx + y_top;

							if (x0 < x1 && x3 > x2) {         // three segments descending down-right
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
								stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x2, y2);
								stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
							}
							else if (x3 < x1 && x0 > x2) {  // three segments descending down-left
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
								stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x1, y1);
								stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
							}
							else if (x0 < x1 && x3 > x1) {  // two segments across x, down-right
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
								stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
							}
							else if (x3 < x1 && x0 > x1) {  // two segments across x, down-left
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x1, y1);
								stbtt__handle_clipped_edge(scanline, x, e, x1, y1, x3, y3);
							}
							else if (x0 < x2 && x3 > x2) {  // two segments across x+1, down-right
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
								stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
							}
							else if (x3 < x2 && x0 > x2) {  // two segments across x+1, down-left
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x2, y2);
								stbtt__handle_clipped_edge(scanline, x, e, x2, y2, x3, y3);
							}
							else {  // one segment
								stbtt__handle_clipped_edge(scanline, x, e, x0, y0, x3, y3);
							}
						}
					}
				}
				e = e->next;
			}
		}

		// directly AA rasterize edges w/o supersampling
		static void stbtt__rasterize_sorted_edges(stbtt__bitmap* result, stbtt__edge* e, int n, int vsubsample, int off_x, int off_y, void* userdata)
		{
			stbtt__hheap hh = { 0, 0, 0 };
			stbtt__active_edge* active = NULL;
			int y, j = 0, i;
			float scanline_data[129], * scanline, * scanline2;

			STBTT__NOTUSED(vsubsample);

			if (result->w > 64)
				scanline = (float*)STBTT_malloc((result->w * 2 + 1) * sizeof(float), userdata);
			else
				scanline = scanline_data;

			scanline2 = scanline + result->w;

			y = off_y;
			e[n].y0 = (float)(off_y + result->h) + 1;

			while (j < result->h) {
				// find center of pixel for this scanline
				float scan_y_top = y + 0.0f;
				float scan_y_bottom = y + 1.0f;
				stbtt__active_edge** step = &active;

				STBTT_memset(scanline, 0, result->w * sizeof(scanline[0]));
				STBTT_memset(scanline2, 0, (result->w + 1) * sizeof(scanline[0]));

				// update all active edges;
				// remove all active edges that terminate before the top of this scanline
				while (*step) {
					stbtt__active_edge* z = *step;
					if (z->ey <= scan_y_top) {
						*step = z->next; // delete from list
						STBTT_assert(z->direction);
						z->direction = 0;
						stbtt__hheap_free(&hh, z);
					}
					else {
						step = &((*step)->next); // advance through list
					}
				}

				// insert all edges that start before the bottom of this scanline
				while (e->y0 <= scan_y_bottom) {
					if (e->y0 != e->y1) {
						stbtt__active_edge* z = stbtt__new_active(&hh, e, off_x, scan_y_top, userdata);
						if (z != NULL) {
							if (j == 0 && off_y != 0) {
								if (z->ey < scan_y_top) {
									// this can happen due to subpixel positioning and some kind of fp rounding error i think
									z->ey = scan_y_top;
								}
							}
							STBTT_assert(z->ey >= scan_y_top); // if we get really unlucky a tiny bit of an edge can be out of bounds
							// insert at front
							z->next = active;
							active = z;
						}
					}
					++e;
				}

				// now process all active edges
				if (active)
					stbtt__fill_active_edges_new(scanline, scanline2 + 1, result->w, active, scan_y_top);

				{
					float sum = 0;
					for (i = 0; i < result->w; ++i) {
						float k;
						int m;
						sum += scanline2[i];
						k = scanline[i] + sum;
						k = (float)STBTT_fabs(k) * 255 + 0.5f;
						m = (int)k;
						if (m > 255) m = 255;
						result->pixels[j * result->stride + i] = (unsigned char)m;
					}
				}
				// advance all the edges
				step = &active;
				while (*step) {
					stbtt__active_edge* z = *step;
					z->fx += z->fdx; // advance to position for current scanline
					step = &((*step)->next); // advance through list
				}

				++y;
				++j;
			}

			stbtt__hheap_cleanup(&hh, userdata);

			if (scanline != scanline_data)
				STBTT_free(scanline, userdata);
		}
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

#define STBTT__COMPARE(a,b)  ((a)->y0 < (b)->y0)

		static void stbtt__sort_edges_ins_sort(stbtt__edge* p, int n)
		{
			int i, j;
			for (i = 1; i < n; ++i) {
				stbtt__edge t = p[i], * a = &t;
				j = i;
				while (j > 0) {
					stbtt__edge* b = &p[j - 1];
					int c = STBTT__COMPARE(a, b);
					if (!c) break;
					p[j] = p[j - 1];
					--j;
				}
				if (i != j)
					p[j] = t;
			}
		}

		static void stbtt__sort_edges_quicksort(stbtt__edge* p, int n)
		{
			/* threshold for transitioning to insertion sort */
			while (n > 12) {
				stbtt__edge t;
				int c01, c12, c, m, i, j;

				/* compute median of three */
				m = n >> 1;
				c01 = STBTT__COMPARE(&p[0], &p[m]);
				c12 = STBTT__COMPARE(&p[m], &p[n - 1]);
				/* if 0 >= mid >= end, or 0 < mid < end, then use mid */
				if (c01 != c12) {
					/* otherwise, we'll need to swap something else to middle */
					int z;
					c = STBTT__COMPARE(&p[0], &p[n - 1]);
					/* 0>mid && mid<n:  0>n => n; 0<n => 0 */
					/* 0<mid && mid>n:  0>n => 0; 0<n => n */
					z = (c == c12) ? 0 : n - 1;
					t = p[z];
					p[z] = p[m];
					p[m] = t;
				}
				/* now p[m] is the median-of-three */
				/* swap it to the beginning so it won't move around */
				t = p[0];
				p[0] = p[m];
				p[m] = t;

				/* partition loop */
				i = 1;
				j = n - 1;
				for (;;) {
					/* handling of equality is crucial here */
					/* for sentinels & efficiency with duplicates */
					for (;; ++i) {
						if (!STBTT__COMPARE(&p[i], &p[0])) break;
					}
					for (;; --j) {
						if (!STBTT__COMPARE(&p[0], &p[j])) break;
					}
					/* make sure we haven't crossed */
					if (i >= j) break;
					t = p[i];
					p[i] = p[j];
					p[j] = t;

					++i;
					--j;
				}
				/* recurse on smaller side, iterate on larger */
				if (j < (n - i)) {
					stbtt__sort_edges_quicksort(p, j);
					p = p + i;
					n = n - i;
				}
				else {
					stbtt__sort_edges_quicksort(p + i, n - i);
					n = j;
				}
			}
		}

		static void stbtt__sort_edges(stbtt__edge* p, int n)
		{
			stbtt__sort_edges_quicksort(p, n);
			stbtt__sort_edges_ins_sort(p, n);
		}

		typedef struct
		{
			float x, y;
		} stbtt__point;

		static void stbtt__rasterize(stbtt__bitmap* result, stbtt__point* pts, int* wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert, void* userdata)
		{
			float y_scale_inv = invert ? -scale_y : scale_y;
			stbtt__edge* e;
			int n, i, j, k, m;
#if STBTT_RASTERIZER_VERSION == 1
			int vsubsample = result->h < 8 ? 15 : 5;
#elif STBTT_RASTERIZER_VERSION == 2
			int vsubsample = 1;
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif
			// vsubsample should divide 255 evenly; otherwise we won't reach full opacity

			// now we have to blow out the windings into explicit edge lists
			n = 0;
			for (i = 0; i < windings; ++i)
				n += wcount[i];

			e = (stbtt__edge*)STBTT_malloc(sizeof(*e) * (n + 1), userdata); // add an extra one as a sentinel
			if (e == 0) return;
			n = 0;

			m = 0;
			for (i = 0; i < windings; ++i) {
				stbtt__point* p = pts + m;
				m += wcount[i];
				j = wcount[i] - 1;
				for (k = 0; k < wcount[i]; j = k++) {
					int a = k, b = j;
					// skip the edge if horizontal
					if (p[j].y == p[k].y)
						continue;
					// add edge from j to k to the list
					e[n].invert = 0;
					if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
						e[n].invert = 1;
						a = j, b = k;
					}
					e[n].x0 = p[a].x * scale_x + shift_x;
					e[n].y0 = (p[a].y * y_scale_inv + shift_y) * vsubsample;
					e[n].x1 = p[b].x * scale_x + shift_x;
					e[n].y1 = (p[b].y * y_scale_inv + shift_y) * vsubsample;
					++n;
				}
			}

			// now sort the edges by their highest point (should snap to integer, and then by x)
			//STBTT_sort(e, n, sizeof(e[0]), stbtt__edge_compare);
			stbtt__sort_edges(e, n);

			// now, traverse the scanlines and find the intersections on each scanline, use xor winding rule
			stbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, userdata);

			STBTT_free(e, userdata);
		}

		static void stbtt__add_point(stbtt__point* points, int n, float x, float y)
		{
			if (!points) return; // during first pass, it's unallocated
			points[n].x = x;
			points[n].y = y;
		}

		// tessellate until threshold p is happy... @TODO warped to compensate for non-linear stretching
		static int stbtt__tesselate_curve(stbtt__point* points, int* num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n)
		{
			// midpoint
			float mx = (x0 + 2 * x1 + x2) / 4;
			float my = (y0 + 2 * y1 + y2) / 4;
			// versus directly drawn line
			float dx = (x0 + x2) / 2 - mx;
			float dy = (y0 + y2) / 2 - my;
			if (n > 16) // 65536 segments on one curve better be enough!
				return 1;
			if (dx * dx + dy * dy > objspace_flatness_squared) { // half-pixel error allowed... need to be smaller if AA
				stbtt__tesselate_curve(points, num_points, x0, y0, (x0 + x1) / 2.0f, (y0 + y1) / 2.0f, mx, my, objspace_flatness_squared, n + 1);
				stbtt__tesselate_curve(points, num_points, mx, my, (x1 + x2) / 2.0f, (y1 + y2) / 2.0f, x2, y2, objspace_flatness_squared, n + 1);
			}
			else {
				stbtt__add_point(points, *num_points, x2, y2);
				*num_points = *num_points + 1;
			}
			return 1;
		}

		static void stbtt__tesselate_cubic(stbtt__point* points, int* num_points, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float objspace_flatness_squared, int n)
		{
			// @TODO this "flatness" calculation is just made-up nonsense that seems to work well enough
			float dx0 = x1 - x0;
			float dy0 = y1 - y0;
			float dx1 = x2 - x1;
			float dy1 = y2 - y1;
			float dx2 = x3 - x2;
			float dy2 = y3 - y2;
			float dx = x3 - x0;
			float dy = y3 - y0;
			float longlen = (float)(STBTT_sqrt(dx0 * dx0 + dy0 * dy0) + STBTT_sqrt(dx1 * dx1 + dy1 * dy1) + STBTT_sqrt(dx2 * dx2 + dy2 * dy2));
			float shortlen = (float)STBTT_sqrt(dx * dx + dy * dy);
			float flatness_squared = longlen * longlen - shortlen * shortlen;

			if (n > 16) // 65536 segments on one curve better be enough!
				return;

			if (flatness_squared > objspace_flatness_squared) {
				float x01 = (x0 + x1) / 2;
				float y01 = (y0 + y1) / 2;
				float x12 = (x1 + x2) / 2;
				float y12 = (y1 + y2) / 2;
				float x23 = (x2 + x3) / 2;
				float y23 = (y2 + y3) / 2;

				float xa = (x01 + x12) / 2;
				float ya = (y01 + y12) / 2;
				float xb = (x12 + x23) / 2;
				float yb = (y12 + y23) / 2;

				float mx = (xa + xb) / 2;
				float my = (ya + yb) / 2;

				stbtt__tesselate_cubic(points, num_points, x0, y0, x01, y01, xa, ya, mx, my, objspace_flatness_squared, n + 1);
				stbtt__tesselate_cubic(points, num_points, mx, my, xb, yb, x23, y23, x3, y3, objspace_flatness_squared, n + 1);
			}
			else {
				stbtt__add_point(points, *num_points, x3, y3);
				*num_points = *num_points + 1;
			}
		}

		// returns number of contours
		static stbtt__point* stbtt_FlattenCurves(stbtt_vertex* vertices, int num_verts, float objspace_flatness, int** contour_lengths, int* num_contours, void* userdata)
		{
			stbtt__point* points = 0;
			int num_points = 0;

			float objspace_flatness_squared = objspace_flatness * objspace_flatness;
			int i, n = 0, start = 0, pass;

			// count how many "moves" there are to get the contour count
			for (i = 0; i < num_verts; ++i)
				if (vertices[i].type == STBTT_vmove)
					++n;

			*num_contours = n;
			if (n == 0) return 0;

			*contour_lengths = (int*)STBTT_malloc(sizeof(**contour_lengths) * n, userdata);

			if (*contour_lengths == 0) {
				*num_contours = 0;
				return 0;
			}

			// make two passes through the points so we don't need to realloc
			for (pass = 0; pass < 2; ++pass) {
				float x = 0, y = 0;
				if (pass == 1) {
					points = (stbtt__point*)STBTT_malloc(num_points * sizeof(points[0]), userdata);
					if (points == NULL) goto error;
				}
				num_points = 0;
				n = -1;
				for (i = 0; i < num_verts; ++i) {
					switch (vertices[i].type) {
					case STBTT_vmove:
						// start the next contour
						if (n >= 0)
							(*contour_lengths)[n] = num_points - start;
						++n;
						start = num_points;

						x = vertices[i].x, y = vertices[i].y;
						stbtt__add_point(points, num_points++, x, y);
						break;
					case STBTT_vline:
						x = vertices[i].x, y = vertices[i].y;
						stbtt__add_point(points, num_points++, x, y);
						break;
					case STBTT_vcurve:
						stbtt__tesselate_curve(points, &num_points, x, y,
							vertices[i].cx, vertices[i].cy,
							vertices[i].x, vertices[i].y,
							objspace_flatness_squared, 0);
						x = vertices[i].x, y = vertices[i].y;
						break;
					case STBTT_vcubic:
						stbtt__tesselate_cubic(points, &num_points, x, y,
							vertices[i].cx, vertices[i].cy,
							vertices[i].cx1, vertices[i].cy1,
							vertices[i].x, vertices[i].y,
							objspace_flatness_squared, 0);
						x = vertices[i].x, y = vertices[i].y;
						break;
					}
				}
				(*contour_lengths)[n] = num_points - start;
			}

			return points;
		error:
			STBTT_free(points, userdata);
			STBTT_free(*contour_lengths, userdata);
			*contour_lengths = 0;
			*num_contours = 0;
			return NULL;
		}

		STBTT_DEF void stbtt_Rasterize(stbtt__bitmap* result, float flatness_in_pixels, stbtt_vertex* vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void* userdata)
		{
			float scale = scale_x > scale_y ? scale_y : scale_x;
			int winding_count = 0;
			int* winding_lengths = NULL;
			stbtt__point* windings = stbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, userdata);
			if (windings) {
				stbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, userdata);
				STBTT_free(winding_lengths, userdata);
				STBTT_free(windings, userdata);
			}
		}

		STBTT_DEF void stbtt_FreeBitmap(unsigned char* bitmap, void* userdata)
		{
			STBTT_free(bitmap, userdata);
		}

		STBTT_DEF unsigned char* stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo* info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int* width, int* height, int* xoff, int* yoff)
		{
			int ix0, iy0, ix1, iy1;
			stbtt__bitmap gbm;
			stbtt_vertex* vertices;
			int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);

			if (scale_x == 0) scale_x = scale_y;
			if (scale_y == 0) {
				if (scale_x == 0) {
					STBTT_free(vertices, info->userdata);
					return NULL;
				}
				scale_y = scale_x;
			}

			stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, &ix1, &iy1);

			// now we get the size
			gbm.w = (ix1 - ix0);
			gbm.h = (iy1 - iy0);
			gbm.pixels = NULL; // in case we error

			if (width) *width = gbm.w;
			if (height) *height = gbm.h;
			if (xoff) *xoff = ix0;
			if (yoff) *yoff = iy0;

			if (gbm.w && gbm.h) {
				gbm.pixels = (unsigned char*)STBTT_malloc(gbm.w * gbm.h, info->userdata);
				if (gbm.pixels) {
					gbm.stride = gbm.w;

					stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);
				}
			}
			STBTT_free(vertices, info->userdata);
			return gbm.pixels;
		}

		STBTT_DEF unsigned char* stbtt_GetGlyphBitmap(const stbtt_fontinfo* info, float scale_x, float scale_y, int glyph, int* width, int* height, int* xoff, int* yoff)
		{
			return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, glyph, width, height, xoff, yoff);
		}

		STBTT_DEF void stbtt_MakeGlyphBitmapSubpixel(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int glyph)
		{
			int ix0, iy0;
			stbtt_vertex* vertices;
			int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);
			stbtt__bitmap gbm;

			stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0, &iy0, 0, 0);
			gbm.pixels = output;
			gbm.w = out_w;
			gbm.h = out_h;
			gbm.stride = out_stride;

			if (gbm.w && gbm.h)
				stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);

			STBTT_free(vertices, info->userdata);
		}

		STBTT_DEF void stbtt_MakeGlyphBitmap(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int glyph)
		{
			stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, glyph);
		}

		STBTT_DEF unsigned char* stbtt_GetCodepointBitmapSubpixel(const stbtt_fontinfo* info, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint, int* width, int* height, int* xoff, int* yoff)
		{
			return stbtt_GetGlyphBitmapSubpixel(info, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint), width, height, xoff, yoff);
		}

		STBTT_DEF void stbtt_MakeCodepointBitmapSubpixelPrefilter(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int oversample_x, int oversample_y, float* sub_x, float* sub_y, int codepoint)
		{
			stbtt_MakeGlyphBitmapSubpixelPrefilter(info, output, out_w, out_h, out_stride, scale_x, scale_y, shift_x, shift_y, oversample_x, oversample_y, sub_x, sub_y, stbtt_FindGlyphIndex(info, codepoint));
		}

		STBTT_DEF void stbtt_MakeCodepointBitmapSubpixel(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int codepoint)
		{
			stbtt_MakeGlyphBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, shift_x, shift_y, stbtt_FindGlyphIndex(info, codepoint));
		}

		STBTT_DEF unsigned char* stbtt_GetCodepointBitmap(const stbtt_fontinfo* info, float scale_x, float scale_y, int codepoint, int* width, int* height, int* xoff, int* yoff)
		{
			return stbtt_GetCodepointBitmapSubpixel(info, scale_x, scale_y, 0.0f, 0.0f, codepoint, width, height, xoff, yoff);
		}

		STBTT_DEF void stbtt_MakeCodepointBitmap(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint)
		{
			stbtt_MakeCodepointBitmapSubpixel(info, output, out_w, out_h, out_stride, scale_x, scale_y, 0.0f, 0.0f, codepoint);
		}

		//////////////////////////////////////////////////////////////////////////////
		//
		// bitmap baking
		//
		// This is SUPER-CRAPPY packing to keep source code small

		static int stbtt_BakeFontBitmap_internal(unsigned char* data, int offset,  // font location (use offset=0 for plain .ttf)
			float pixel_height,                     // height of font in pixels
			unsigned char* pixels, int pw, int ph,  // bitmap to be filled in
			int first_char, int num_chars,          // characters to bake
			stbtt_bakedchar* chardata)
		{
			float scale;
			int x, y, bottom_y, i;
			stbtt_fontinfo f;
			f.userdata = NULL;
			if (!stbtt_InitFont(&f, data, offset))
				return -1;
			STBTT_memset(pixels, 0, pw * ph); // background of 0 around pixels
			x = y = 1;
			bottom_y = 1;

			scale = stbtt_ScaleForPixelHeight(&f, pixel_height);

			for (i = 0; i < num_chars; ++i) {
				int advance, lsb, x0, y0, x1, y1, gw, gh;
				int g = stbtt_FindGlyphIndex(&f, first_char + i);
				stbtt_GetGlyphHMetrics(&f, g, &advance, &lsb);
				stbtt_GetGlyphBitmapBox(&f, g, scale, scale, &x0, &y0, &x1, &y1);
				gw = x1 - x0;
				gh = y1 - y0;
				if (x + gw + 1 >= pw)
					y = bottom_y, x = 1; // advance to next row
				if (y + gh + 1 >= ph) // check if it fits vertically AFTER potentially moving to next row
					return -i;
				STBTT_assert(x + gw < pw);
				STBTT_assert(y + gh < ph);
				stbtt_MakeGlyphBitmap(&f, pixels + x + y * pw, gw, gh, pw, scale, scale, g);
				chardata[i].x0 = (stbtt_int16)x;
				chardata[i].y0 = (stbtt_int16)y;
				chardata[i].x1 = (stbtt_int16)(x + gw);
				chardata[i].y1 = (stbtt_int16)(y + gh);
				chardata[i].xadvance = scale * advance;
				chardata[i].xoff = (float)x0;
				chardata[i].yoff = (float)y0;
				x = x + gw + 1;
				if (y + gh + 1 > bottom_y)
					bottom_y = y + gh + 1;
			}
			return bottom_y;
		}

		STBTT_DEF void stbtt_GetBakedQuad(const stbtt_bakedchar* chardata, int pw, int ph, int char_index, float* xpos, float* ypos, stbtt_aligned_quad* q, int opengl_fillrule)
		{
			float d3d_bias = opengl_fillrule ? 0 : -0.5f;
			float ipw = 1.0f / pw, iph = 1.0f / ph;
			const stbtt_bakedchar* b = chardata + char_index;
			int round_x = STBTT_ifloor((*xpos + b->xoff) + 0.5f);
			int round_y = STBTT_ifloor((*ypos + b->yoff) + 0.5f);

			q->x0 = round_x + d3d_bias;
			q->y0 = round_y + d3d_bias;
			q->x1 = round_x + b->x1 - b->x0 + d3d_bias;
			q->y1 = round_y + b->y1 - b->y0 + d3d_bias;

			q->s0 = b->x0 * ipw;
			q->t0 = b->y0 * iph;
			q->s1 = b->x1 * ipw;
			q->t1 = b->y1 * iph;

			*xpos += b->xadvance;
		}

		//////////////////////////////////////////////////////////////////////////////
		//
		// rectangle packing replacement routines if you don't have stb_rect_pack.h
		//

#ifndef STB_RECT_PACK_VERSION

		typedef int stbrp_coord;

		////////////////////////////////////////////////////////////////////////////////////
		//                                                                                //
		//                                                                                //
		// COMPILER WARNING ?!?!?                                                         //
		//                                                                                //
		//                                                                                //
		// if you get a compile warning due to these symbols being defined more than      //
		// once, move #include "stb_rect_pack.h" before #include "stb_truetype.h"         //
		//                                                                                //
		////////////////////////////////////////////////////////////////////////////////////

		typedef struct
		{
			int width, height;
			int x, y, bottom_y;
		} stbrp_context;

		typedef struct
		{
			unsigned char x;
		} stbrp_node;

		struct stbrp_rect
		{
			stbrp_coord x, y;
			int id, w, h, was_packed;
		};

		static void stbrp_init_target(stbrp_context* con, int pw, int ph, stbrp_node* nodes, int num_nodes)
		{
			con->width = pw;
			con->height = ph;
			con->x = 0;
			con->y = 0;
			con->bottom_y = 0;
			STBTT__NOTUSED(nodes);
			STBTT__NOTUSED(num_nodes);
		}

		static void stbrp_pack_rects(stbrp_context* con, stbrp_rect* rects, int num_rects)
		{
			int i;
			for (i = 0; i < num_rects; ++i) {
				if (con->x + rects[i].w > con->width) {
					con->x = 0;
					con->y = con->bottom_y;
				}
				if (con->y + rects[i].h > con->height)
					break;
				rects[i].x = con->x;
				rects[i].y = con->y;
				rects[i].was_packed = 1;
				con->x += rects[i].w;
				if (con->y + rects[i].h > con->bottom_y)
					con->bottom_y = con->y + rects[i].h;
			}
			for (; i < num_rects; ++i)
				rects[i].was_packed = 0;
		}
#endif

		//////////////////////////////////////////////////////////////////////////////
		//
		// bitmap baking
		//
		// This is SUPER-AWESOME (tm Ryan Gordon) packing using stb_rect_pack.h. If
		// stb_rect_pack.h isn't available, it uses the BakeFontBitmap strategy.

		STBTT_DEF int stbtt_PackBegin(stbtt_pack_context* spc, unsigned char* pixels, int pw, int ph, int stride_in_bytes, int padding, void* alloc_context)
		{
			stbrp_context* context = (stbrp_context*)STBTT_malloc(sizeof(*context), alloc_context);
			int            num_nodes = pw - padding;
			stbrp_node* nodes = (stbrp_node*)STBTT_malloc(sizeof(*nodes) * num_nodes, alloc_context);

			if (context == NULL || nodes == NULL) {
				if (context != NULL) STBTT_free(context, alloc_context);
				if (nodes != NULL) STBTT_free(nodes, alloc_context);
				return 0;
			}

			spc->user_allocator_context = alloc_context;
			spc->width = pw;
			spc->height = ph;
			spc->pixels = pixels;
			spc->pack_info = context;
			spc->nodes = nodes;
			spc->padding = padding;
			spc->stride_in_bytes = stride_in_bytes != 0 ? stride_in_bytes : pw;
			spc->h_oversample = 1;
			spc->v_oversample = 1;
			spc->skip_missing = 0;

			stbrp_init_target(context, pw - padding, ph - padding, nodes, num_nodes);

			if (pixels)
				STBTT_memset(pixels, 0, pw * ph); // background of 0 around pixels

			return 1;
		}

		STBTT_DEF void stbtt_PackEnd(stbtt_pack_context* spc)
		{
			STBTT_free(spc->nodes, spc->user_allocator_context);
			STBTT_free(spc->pack_info, spc->user_allocator_context);
		}

		STBTT_DEF void stbtt_PackSetOversampling(stbtt_pack_context* spc, unsigned int h_oversample, unsigned int v_oversample)
		{
			STBTT_assert(h_oversample <= STBTT_MAX_OVERSAMPLE);
			STBTT_assert(v_oversample <= STBTT_MAX_OVERSAMPLE);
			if (h_oversample <= STBTT_MAX_OVERSAMPLE)
				spc->h_oversample = h_oversample;
			if (v_oversample <= STBTT_MAX_OVERSAMPLE)
				spc->v_oversample = v_oversample;
		}

		STBTT_DEF void stbtt_PackSetSkipMissingCodepoints(stbtt_pack_context* spc, int skip)
		{
			spc->skip_missing = skip;
		}

#define STBTT__OVER_MASK  (STBTT_MAX_OVERSAMPLE-1)

		static void stbtt__h_prefilter(unsigned char* pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
		{
			unsigned char buffer[STBTT_MAX_OVERSAMPLE];
			int safe_w = w - kernel_width;
			int j;
			STBTT_memset(buffer, 0, STBTT_MAX_OVERSAMPLE); // suppress bogus warning from VS2013 -analyze
			for (j = 0; j < h; ++j) {
				int i;
				unsigned int total;
				STBTT_memset(buffer, 0, kernel_width);

				total = 0;

				// make kernel_width a constant in common cases so compiler can optimize out the divide
				switch (kernel_width) {
				case 2:
					for (i = 0; i <= safe_w; ++i) {
						total += pixels[i] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
						pixels[i] = (unsigned char)(total / 2);
					}
					break;
				case 3:
					for (i = 0; i <= safe_w; ++i) {
						total += pixels[i] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
						pixels[i] = (unsigned char)(total / 3);
					}
					break;
				case 4:
					for (i = 0; i <= safe_w; ++i) {
						total += pixels[i] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
						pixels[i] = (unsigned char)(total / 4);
					}
					break;
				case 5:
					for (i = 0; i <= safe_w; ++i) {
						total += pixels[i] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
						pixels[i] = (unsigned char)(total / 5);
					}
					break;
				default:
					for (i = 0; i <= safe_w; ++i) {
						total += pixels[i] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i];
						pixels[i] = (unsigned char)(total / kernel_width);
					}
					break;
				}

				for (; i < w; ++i) {
					STBTT_assert(pixels[i] == 0);
					total -= buffer[i & STBTT__OVER_MASK];
					pixels[i] = (unsigned char)(total / kernel_width);
				}

				pixels += stride_in_bytes;
			}
		}

		static void stbtt__v_prefilter(unsigned char* pixels, int w, int h, int stride_in_bytes, unsigned int kernel_width)
		{
			unsigned char buffer[STBTT_MAX_OVERSAMPLE];
			int safe_h = h - kernel_width;
			int j;
			STBTT_memset(buffer, 0, STBTT_MAX_OVERSAMPLE); // suppress bogus warning from VS2013 -analyze
			for (j = 0; j < w; ++j) {
				int i;
				unsigned int total;
				STBTT_memset(buffer, 0, kernel_width);

				total = 0;

				// make kernel_width a constant in common cases so compiler can optimize out the divide
				switch (kernel_width) {
				case 2:
					for (i = 0; i <= safe_h; ++i) {
						total += pixels[i * stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i * stride_in_bytes];
						pixels[i * stride_in_bytes] = (unsigned char)(total / 2);
					}
					break;
				case 3:
					for (i = 0; i <= safe_h; ++i) {
						total += pixels[i * stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i * stride_in_bytes];
						pixels[i * stride_in_bytes] = (unsigned char)(total / 3);
					}
					break;
				case 4:
					for (i = 0; i <= safe_h; ++i) {
						total += pixels[i * stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i * stride_in_bytes];
						pixels[i * stride_in_bytes] = (unsigned char)(total / 4);
					}
					break;
				case 5:
					for (i = 0; i <= safe_h; ++i) {
						total += pixels[i * stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i * stride_in_bytes];
						pixels[i * stride_in_bytes] = (unsigned char)(total / 5);
					}
					break;
				default:
					for (i = 0; i <= safe_h; ++i) {
						total += pixels[i * stride_in_bytes] - buffer[i & STBTT__OVER_MASK];
						buffer[(i + kernel_width) & STBTT__OVER_MASK] = pixels[i * stride_in_bytes];
						pixels[i * stride_in_bytes] = (unsigned char)(total / kernel_width);
					}
					break;
				}

				for (; i < h; ++i) {
					STBTT_assert(pixels[i * stride_in_bytes] == 0);
					total -= buffer[i & STBTT__OVER_MASK];
					pixels[i * stride_in_bytes] = (unsigned char)(total / kernel_width);
				}

				pixels += 1;
			}
		}

		static float stbtt__oversample_shift(int oversample)
		{
			if (!oversample)
				return 0.0f;

			// The prefilter is a box filter of width "oversample",
			// which shifts phase by (oversample - 1)/2 pixels in
			// oversampled space. We want to shift in the opposite
			// direction to counter this.
			return (float)-(oversample - 1) / (2.0f * (float)oversample);
		}

		// rects array must be big enough to accommodate all characters in the given ranges
		STBTT_DEF int stbtt_PackFontRangesGatherRects(stbtt_pack_context* spc, const stbtt_fontinfo* info, stbtt_pack_range* ranges, int num_ranges, stbrp_rect* rects)
		{
			int i, j, k;
			int missing_glyph_added = 0;

			k = 0;
			for (i = 0; i < num_ranges; ++i) {
				float fh = ranges[i].font_size;
				float scale = fh > 0 ? stbtt_ScaleForPixelHeight(info, fh) : stbtt_ScaleForMappingEmToPixels(info, -fh);
				ranges[i].h_oversample = (unsigned char)spc->h_oversample;
				ranges[i].v_oversample = (unsigned char)spc->v_oversample;
				for (j = 0; j < ranges[i].num_chars; ++j) {
					int x0, y0, x1, y1;
					int codepoint = ranges[i].array_of_unicode_codepoints == NULL ? ranges[i].first_unicode_codepoint_in_range + j : ranges[i].array_of_unicode_codepoints[j];
					int glyph = stbtt_FindGlyphIndex(info, codepoint);
					if (glyph == 0 && (spc->skip_missing || missing_glyph_added)) {
						rects[k].w = rects[k].h = 0;
					}
					else {
						stbtt_GetGlyphBitmapBoxSubpixel(info, glyph,
							scale * spc->h_oversample,
							scale * spc->v_oversample,
							0, 0,
							&x0, &y0, &x1, &y1);
						rects[k].w = (stbrp_coord)(x1 - x0 + spc->padding + spc->h_oversample - 1);
						rects[k].h = (stbrp_coord)(y1 - y0 + spc->padding + spc->v_oversample - 1);
						if (glyph == 0)
							missing_glyph_added = 1;
					}
					++k;
				}
			}

			return k;
		}

		STBTT_DEF void stbtt_MakeGlyphBitmapSubpixelPrefilter(const stbtt_fontinfo* info, unsigned char* output, int out_w, int out_h, int out_stride, float scale_x, float scale_y, float shift_x, float shift_y, int prefilter_x, int prefilter_y, float* sub_x, float* sub_y, int glyph)
		{
			stbtt_MakeGlyphBitmapSubpixel(info,
				output,
				out_w - (prefilter_x - 1),
				out_h - (prefilter_y - 1),
				out_stride,
				scale_x,
				scale_y,
				shift_x,
				shift_y,
				glyph);

			if (prefilter_x > 1)
				stbtt__h_prefilter(output, out_w, out_h, out_stride, prefilter_x);

			if (prefilter_y > 1)
				stbtt__v_prefilter(output, out_w, out_h, out_stride, prefilter_y);

			*sub_x = stbtt__oversample_shift(prefilter_x);
			*sub_y = stbtt__oversample_shift(prefilter_y);
		}

		// rects array must be big enough to accommodate all characters in the given ranges
		STBTT_DEF int stbtt_PackFontRangesRenderIntoRects(stbtt_pack_context* spc, const stbtt_fontinfo* info, stbtt_pack_range* ranges, int num_ranges, stbrp_rect* rects)
		{
			int i, j, k, missing_glyph = -1, return_value = 1;

			// save current values
			int old_h_over = spc->h_oversample;
			int old_v_over = spc->v_oversample;

			k = 0;
			for (i = 0; i < num_ranges; ++i) {
				float fh = ranges[i].font_size;
				float scale = fh > 0 ? stbtt_ScaleForPixelHeight(info, fh) : stbtt_ScaleForMappingEmToPixels(info, -fh);
				float recip_h, recip_v, sub_x, sub_y;
				spc->h_oversample = ranges[i].h_oversample;
				spc->v_oversample = ranges[i].v_oversample;
				recip_h = 1.0f / spc->h_oversample;
				recip_v = 1.0f / spc->v_oversample;
				sub_x = stbtt__oversample_shift(spc->h_oversample);
				sub_y = stbtt__oversample_shift(spc->v_oversample);
				for (j = 0; j < ranges[i].num_chars; ++j) {
					stbrp_rect* r = &rects[k];
					if (r->was_packed && r->w != 0 && r->h != 0) {
						stbtt_packedchar* bc = &ranges[i].chardata_for_range[j];
						int advance, lsb, x0, y0, x1, y1;
						int codepoint = ranges[i].array_of_unicode_codepoints == NULL ? ranges[i].first_unicode_codepoint_in_range + j : ranges[i].array_of_unicode_codepoints[j];
						int glyph = stbtt_FindGlyphIndex(info, codepoint);
						stbrp_coord pad = (stbrp_coord)spc->padding;

						// pad on left and top
						r->x += pad;
						r->y += pad;
						r->w -= pad;
						r->h -= pad;
						stbtt_GetGlyphHMetrics(info, glyph, &advance, &lsb);
						stbtt_GetGlyphBitmapBox(info, glyph,
							scale * spc->h_oversample,
							scale * spc->v_oversample,
							&x0, &y0, &x1, &y1);
						stbtt_MakeGlyphBitmapSubpixel(info,
							spc->pixels + r->x + r->y * spc->stride_in_bytes,
							r->w - spc->h_oversample + 1,
							r->h - spc->v_oversample + 1,
							spc->stride_in_bytes,
							scale * spc->h_oversample,
							scale * spc->v_oversample,
							0, 0,
							glyph);

						if (spc->h_oversample > 1)
							stbtt__h_prefilter(spc->pixels + r->x + r->y * spc->stride_in_bytes,
								r->w, r->h, spc->stride_in_bytes,
								spc->h_oversample);

						if (spc->v_oversample > 1)
							stbtt__v_prefilter(spc->pixels + r->x + r->y * spc->stride_in_bytes,
								r->w, r->h, spc->stride_in_bytes,
								spc->v_oversample);

						bc->x0 = (stbtt_int16)r->x;
						bc->y0 = (stbtt_int16)r->y;
						bc->x1 = (stbtt_int16)(r->x + r->w);
						bc->y1 = (stbtt_int16)(r->y + r->h);
						bc->xadvance = scale * advance;
						bc->xoff = (float)x0 * recip_h + sub_x;
						bc->yoff = (float)y0 * recip_v + sub_y;
						bc->xoff2 = (x0 + r->w) * recip_h + sub_x;
						bc->yoff2 = (y0 + r->h) * recip_v + sub_y;

						if (glyph == 0)
							missing_glyph = j;
					}
					else if (spc->skip_missing) {
						return_value = 0;
					}
					else if (r->was_packed && r->w == 0 && r->h == 0 && missing_glyph >= 0) {
						ranges[i].chardata_for_range[j] = ranges[i].chardata_for_range[missing_glyph];
					}
					else {
						return_value = 0; // if any fail, report failure
					}

					++k;
				}
			}

			// restore original values
			spc->h_oversample = old_h_over;
			spc->v_oversample = old_v_over;

			return return_value;
		}

		STBTT_DEF void stbtt_PackFontRangesPackRects(stbtt_pack_context* spc, stbrp_rect* rects, int num_rects)
		{
			stbrp_pack_rects((stbrp_context*)spc->pack_info, rects, num_rects);
		}

		STBTT_DEF int stbtt_PackFontRanges(stbtt_pack_context* spc, const unsigned char* fontdata, int font_index, stbtt_pack_range* ranges, int num_ranges)
		{
			stbtt_fontinfo info;
			int i, j, n, return_value = 1;
			//stbrp_context *context = (stbrp_context *) spc->pack_info;
			stbrp_rect* rects;

			// flag all characters as NOT packed
			for (i = 0; i < num_ranges; ++i)
				for (j = 0; j < ranges[i].num_chars; ++j)
					ranges[i].chardata_for_range[j].x0 =
					ranges[i].chardata_for_range[j].y0 =
					ranges[i].chardata_for_range[j].x1 =
					ranges[i].chardata_for_range[j].y1 = 0;

			n = 0;
			for (i = 0; i < num_ranges; ++i)
				n += ranges[i].num_chars;

			rects = (stbrp_rect*)STBTT_malloc(sizeof(*rects) * n, spc->user_allocator_context);
			if (rects == NULL)
				return 0;

			info.userdata = spc->user_allocator_context;
			stbtt_InitFont(&info, fontdata, stbtt_GetFontOffsetForIndex(fontdata, font_index));

			n = stbtt_PackFontRangesGatherRects(spc, &info, ranges, num_ranges, rects);

			stbtt_PackFontRangesPackRects(spc, rects, n);

			return_value = stbtt_PackFontRangesRenderIntoRects(spc, &info, ranges, num_ranges, rects);

			STBTT_free(rects, spc->user_allocator_context);
			return return_value;
		}

		STBTT_DEF int stbtt_PackFontRange(stbtt_pack_context* spc, const unsigned char* fontdata, int font_index, float font_size,
			int first_unicode_codepoint_in_range, int num_chars_in_range, stbtt_packedchar* chardata_for_range)
		{
			stbtt_pack_range range;
			range.first_unicode_codepoint_in_range = first_unicode_codepoint_in_range;
			range.array_of_unicode_codepoints = NULL;
			range.num_chars = num_chars_in_range;
			range.chardata_for_range = chardata_for_range;
			range.font_size = font_size;
			return stbtt_PackFontRanges(spc, fontdata, font_index, &range, 1);
		}

		STBTT_DEF void stbtt_GetScaledFontVMetrics(const unsigned char* fontdata, int index, float size, float* ascent, float* descent, float* lineGap)
		{
			int i_ascent, i_descent, i_lineGap;
			float scale;
			stbtt_fontinfo info;
			stbtt_InitFont(&info, fontdata, stbtt_GetFontOffsetForIndex(fontdata, index));
			scale = size > 0 ? stbtt_ScaleForPixelHeight(&info, size) : stbtt_ScaleForMappingEmToPixels(&info, -size);
			stbtt_GetFontVMetrics(&info, &i_ascent, &i_descent, &i_lineGap);
			*ascent = (float)i_ascent * scale;
			*descent = (float)i_descent * scale;
			*lineGap = (float)i_lineGap * scale;
		}

		STBTT_DEF void stbtt_GetPackedQuad(const stbtt_packedchar* chardata, int pw, int ph, int char_index, float* xpos, float* ypos, stbtt_aligned_quad* q, int align_to_integer)
		{
			float ipw = 1.0f / pw, iph = 1.0f / ph;
			const stbtt_packedchar* b = chardata + char_index;

			if (align_to_integer) {
				float x = (float)STBTT_ifloor((*xpos + b->xoff) + 0.5f);
				float y = (float)STBTT_ifloor((*ypos + b->yoff) + 0.5f);
				q->x0 = x;
				q->y0 = y;
				q->x1 = x + b->xoff2 - b->xoff;
				q->y1 = y + b->yoff2 - b->yoff;
			}
			else {
				q->x0 = *xpos + b->xoff;
				q->y0 = *ypos + b->yoff;
				q->x1 = *xpos + b->xoff2;
				q->y1 = *ypos + b->yoff2;
			}

			q->s0 = b->x0 * ipw;
			q->t0 = b->y0 * iph;
			q->s1 = b->x1 * ipw;
			q->t1 = b->y1 * iph;

			*xpos += b->xadvance;
		}

		//////////////////////////////////////////////////////////////////////////////
		//
		// sdf computation
		//

#define STBTT_min(a,b)  ((a) < (b) ? (a) : (b))
#define STBTT_max(a,b)  ((a) < (b) ? (b) : (a))

		static int stbtt__ray_intersect_bezier(float orig[2], float ray[2], float q0[2], float q1[2], float q2[2], float hits[2][2])
		{
			float q0perp = q0[1] * ray[0] - q0[0] * ray[1];
			float q1perp = q1[1] * ray[0] - q1[0] * ray[1];
			float q2perp = q2[1] * ray[0] - q2[0] * ray[1];
			float roperp = orig[1] * ray[0] - orig[0] * ray[1];

			float a = q0perp - 2 * q1perp + q2perp;
			float b = q1perp - q0perp;
			float c = q0perp - roperp;

			float s0 = 0., s1 = 0.;
			int num_s = 0;

			if (a != 0.0) {
				float discr = b * b - a * c;
				if (discr > 0.0) {
					float rcpna = -1 / a;
					float d = (float)STBTT_sqrt(discr);
					s0 = (b + d) * rcpna;
					s1 = (b - d) * rcpna;
					if (s0 >= 0.0 && s0 <= 1.0)
						num_s = 1;
					if (d > 0.0 && s1 >= 0.0 && s1 <= 1.0) {
						if (num_s == 0) s0 = s1;
						++num_s;
					}
				}
			}
			else {
				// 2*b*s + c = 0
				// s = -c / (2*b)
				s0 = c / (-2 * b);
				if (s0 >= 0.0 && s0 <= 1.0)
					num_s = 1;
			}

			if (num_s == 0)
				return 0;
			else {
				float rcp_len2 = 1 / (ray[0] * ray[0] + ray[1] * ray[1]);
				float rayn_x = ray[0] * rcp_len2, rayn_y = ray[1] * rcp_len2;

				float q0d = q0[0] * rayn_x + q0[1] * rayn_y;
				float q1d = q1[0] * rayn_x + q1[1] * rayn_y;
				float q2d = q2[0] * rayn_x + q2[1] * rayn_y;
				float rod = orig[0] * rayn_x + orig[1] * rayn_y;

				float q10d = q1d - q0d;
				float q20d = q2d - q0d;
				float q0rd = q0d - rod;

				hits[0][0] = q0rd + s0 * (2.0f - 2.0f * s0) * q10d + s0 * s0 * q20d;
				hits[0][1] = a * s0 + b;

				if (num_s > 1) {
					hits[1][0] = q0rd + s1 * (2.0f - 2.0f * s1) * q10d + s1 * s1 * q20d;
					hits[1][1] = a * s1 + b;
					return 2;
				}
				else {
					return 1;
				}
			}
		}

		static int equal(float* a, float* b)
		{
			return (a[0] == b[0] && a[1] == b[1]);
		}

		static int stbtt__compute_crossings_x(float x, float y, int nverts, stbtt_vertex* verts)
		{
			int i;
			float orig[2], ray[2] = { 1, 0 };
			float y_frac;
			int winding = 0;

			// make sure y never passes through a vertex of the shape
			y_frac = (float)STBTT_fmod(y, 1.0f);
			if (y_frac < 0.01f)
				y += 0.01f;
			else if (y_frac > 0.99f)
				y -= 0.01f;

			orig[0] = x;
			orig[1] = y;

			// test a ray from (-infinity,y) to (x,y)
			for (i = 0; i < nverts; ++i) {
				if (verts[i].type == STBTT_vline) {
					int x0 = (int)verts[i - 1].x, y0 = (int)verts[i - 1].y;
					int x1 = (int)verts[i].x, y1 = (int)verts[i].y;
					if (y > STBTT_min(y0, y1) && y < STBTT_max(y0, y1) && x > STBTT_min(x0, x1)) {
						float x_inter = (y - y0) / (y1 - y0) * (x1 - x0) + x0;
						if (x_inter < x)
							winding += (y0 < y1) ? 1 : -1;
					}
				}
				if (verts[i].type == STBTT_vcurve) {
					int x0 = (int)verts[i - 1].x, y0 = (int)verts[i - 1].y;
					int x1 = (int)verts[i].cx, y1 = (int)verts[i].cy;
					int x2 = (int)verts[i].x, y2 = (int)verts[i].y;
					int ax = STBTT_min(x0, STBTT_min(x1, x2)), ay = STBTT_min(y0, STBTT_min(y1, y2));
					int by = STBTT_max(y0, STBTT_max(y1, y2));
					if (y > ay && y < by && x > ax) {
						float q0[2], q1[2], q2[2];
						float hits[2][2];
						q0[0] = (float)x0;
						q0[1] = (float)y0;
						q1[0] = (float)x1;
						q1[1] = (float)y1;
						q2[0] = (float)x2;
						q2[1] = (float)y2;
						if (equal(q0, q1) || equal(q1, q2)) {
							x0 = (int)verts[i - 1].x;
							y0 = (int)verts[i - 1].y;
							x1 = (int)verts[i].x;
							y1 = (int)verts[i].y;
							if (y > STBTT_min(y0, y1) && y < STBTT_max(y0, y1) && x > STBTT_min(x0, x1)) {
								float x_inter = (y - y0) / (y1 - y0) * (x1 - x0) + x0;
								if (x_inter < x)
									winding += (y0 < y1) ? 1 : -1;
							}
						}
						else {
							int num_hits = stbtt__ray_intersect_bezier(orig, ray, q0, q1, q2, hits);
							if (num_hits >= 1)
								if (hits[0][0] < 0)
									winding += (hits[0][1] < 0 ? -1 : 1);
							if (num_hits >= 2)
								if (hits[1][0] < 0)
									winding += (hits[1][1] < 0 ? -1 : 1);
						}
					}
				}
			}
			return winding;
		}

		static float stbtt__cuberoot(float x)
		{
			if (x < 0)
				return -(float)STBTT_pow(-x, 1.0f / 3.0f);
			else
				return  (float)STBTT_pow(x, 1.0f / 3.0f);
		}

		// x^3 + a*x^2 + b*x + c = 0
		static int stbtt__solve_cubic(float a, float b, float c, float* r)
		{
			float s = -a / 3;
			float p = b - a * a / 3;
			float q = a * (2 * a * a - 9 * b) / 27 + c;
			float p3 = p * p * p;
			float d = q * q + 4 * p3 / 27;
			if (d >= 0) {
				float z = (float)STBTT_sqrt(d);
				float u = (-q + z) / 2;
				float v = (-q - z) / 2;
				u = stbtt__cuberoot(u);
				v = stbtt__cuberoot(v);
				r[0] = s + u + v;
				return 1;
			}
			else {
				float u = (float)STBTT_sqrt(-p / 3);
				float v = (float)STBTT_acos(-STBTT_sqrt(-27 / p3) * q / 2) / 3; // p3 must be negative, since d is negative
				float m = (float)STBTT_cos(v);
				float n = (float)STBTT_cos(v - 3.141592 / 2) * 1.732050808f;
				r[0] = s + u * 2 * m;
				r[1] = s - u * (m + n);
				r[2] = s - u * (m - n);

				//STBTT_assert( STBTT_fabs(((r[0]+a)*r[0]+b)*r[0]+c) < 0.05f);  // these asserts may not be safe at all scales, though they're in bezier t parameter units so maybe?
				//STBTT_assert( STBTT_fabs(((r[1]+a)*r[1]+b)*r[1]+c) < 0.05f);
				//STBTT_assert( STBTT_fabs(((r[2]+a)*r[2]+b)*r[2]+c) < 0.05f);
				return 3;
			}
		}

		STBTT_DEF unsigned char* stbtt_GetGlyphSDF(const stbtt_fontinfo* info, float scale, int glyph, int padding, unsigned char onedge_value, float pixel_dist_scale, int* width, int* height, int* xoff, int* yoff)
		{
			float scale_x = scale, scale_y = scale;
			int ix0, iy0, ix1, iy1;
			int w, h;
			unsigned char* data;

			if (scale == 0) return NULL;

			stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale, scale, 0.0f, 0.0f, &ix0, &iy0, &ix1, &iy1);

			// if empty, return NULL
			if (ix0 == ix1 || iy0 == iy1)
				return NULL;

			ix0 -= padding;
			iy0 -= padding;
			ix1 += padding;
			iy1 += padding;

			w = (ix1 - ix0);
			h = (iy1 - iy0);

			if (width) *width = w;
			if (height) *height = h;
			if (xoff) *xoff = ix0;
			if (yoff) *yoff = iy0;

			// invert for y-downwards bitmaps
			scale_y = -scale_y;

			{
				int x, y, i, j;
				float* precompute;
				stbtt_vertex* verts;
				int num_verts = stbtt_GetGlyphShape(info, glyph, &verts);
				data = (unsigned char*)STBTT_malloc(w * h, info->userdata);
				precompute = (float*)STBTT_malloc(num_verts * sizeof(float), info->userdata);

				for (i = 0, j = num_verts - 1; i < num_verts; j = i++) {
					if (verts[i].type == STBTT_vline) {
						float x0 = verts[i].x * scale_x, y0 = verts[i].y * scale_y;
						float x1 = verts[j].x * scale_x, y1 = verts[j].y * scale_y;
						float dist = (float)STBTT_sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
						precompute[i] = (dist == 0) ? 0.0f : 1.0f / dist;
					}
					else if (verts[i].type == STBTT_vcurve) {
						float x2 = verts[j].x * scale_x, y2 = verts[j].y * scale_y;
						float x1 = verts[i].cx * scale_x, y1 = verts[i].cy * scale_y;
						float x0 = verts[i].x * scale_x, y0 = verts[i].y * scale_y;
						float bx = x0 - 2 * x1 + x2, by = y0 - 2 * y1 + y2;
						float len2 = bx * bx + by * by;
						if (len2 != 0.0f)
							precompute[i] = 1.0f / (bx * bx + by * by);
						else
							precompute[i] = 0.0f;
					}
					else
						precompute[i] = 0.0f;
				}

				for (y = iy0; y < iy1; ++y) {
					for (x = ix0; x < ix1; ++x) {
						float val;
						float min_dist = 999999.0f;
						float sx = (float)x + 0.5f;
						float sy = (float)y + 0.5f;
						float x_gspace = (sx / scale_x);
						float y_gspace = (sy / scale_y);

						int winding = stbtt__compute_crossings_x(x_gspace, y_gspace, num_verts, verts); // @OPTIMIZE: this could just be a rasterization, but needs to be line vs. non-tesselated curves so a new path

						for (i = 0; i < num_verts; ++i) {
							float x0 = verts[i].x * scale_x, y0 = verts[i].y * scale_y;

							if (verts[i].type == STBTT_vline && precompute[i] != 0.0f) {
								float x1 = verts[i - 1].x * scale_x, y1 = verts[i - 1].y * scale_y;

								float dist, dist2 = (x0 - sx) * (x0 - sx) + (y0 - sy) * (y0 - sy);
								if (dist2 < min_dist * min_dist)
									min_dist = (float)STBTT_sqrt(dist2);

								// coarse culling against bbox
								//if (sx > STBTT_min(x0,x1)-min_dist && sx < STBTT_max(x0,x1)+min_dist &&
								//    sy > STBTT_min(y0,y1)-min_dist && sy < STBTT_max(y0,y1)+min_dist)
								dist = (float)STBTT_fabs((x1 - x0) * (y0 - sy) - (y1 - y0) * (x0 - sx)) * precompute[i];
								STBTT_assert(i != 0);
								if (dist < min_dist) {
									// check position along line
									// x' = x0 + t*(x1-x0), y' = y0 + t*(y1-y0)
									// minimize (x'-sx)*(x'-sx)+(y'-sy)*(y'-sy)
									float dx = x1 - x0, dy = y1 - y0;
									float px = x0 - sx, py = y0 - sy;
									// minimize (px+t*dx)^2 + (py+t*dy)^2 = px*px + 2*px*dx*t + t^2*dx*dx + py*py + 2*py*dy*t + t^2*dy*dy
									// derivative: 2*px*dx + 2*py*dy + (2*dx*dx+2*dy*dy)*t, set to 0 and solve
									float t = -(px * dx + py * dy) / (dx * dx + dy * dy);
									if (t >= 0.0f && t <= 1.0f)
										min_dist = dist;
								}
							}
							else if (verts[i].type == STBTT_vcurve) {
								float x2 = verts[i - 1].x * scale_x, y2 = verts[i - 1].y * scale_y;
								float x1 = verts[i].cx * scale_x, y1 = verts[i].cy * scale_y;
								float box_x0 = STBTT_min(STBTT_min(x0, x1), x2);
								float box_y0 = STBTT_min(STBTT_min(y0, y1), y2);
								float box_x1 = STBTT_max(STBTT_max(x0, x1), x2);
								float box_y1 = STBTT_max(STBTT_max(y0, y1), y2);
								// coarse culling against bbox to avoid computing cubic unnecessarily
								if (sx > box_x0 - min_dist && sx < box_x1 + min_dist && sy > box_y0 - min_dist && sy < box_y1 + min_dist) {
									int num = 0;
									float ax = x1 - x0, ay = y1 - y0;
									float bx = x0 - 2 * x1 + x2, by = y0 - 2 * y1 + y2;
									float mx = x0 - sx, my = y0 - sy;
									float res[3] = { 0.f,0.f,0.f };
									float px, py, t, it, dist2;
									float a_inv = precompute[i];
									if (a_inv == 0.0) { // if a_inv is 0, it's 2nd degree so use quadratic formula
										float a = 3 * (ax * bx + ay * by);
										float b = 2 * (ax * ax + ay * ay) + (mx * bx + my * by);
										float c = mx * ax + my * ay;
										if (a == 0.0) { // if a is 0, it's linear
											if (b != 0.0) {
												res[num++] = -c / b;
											}
										}
										else {
											float discriminant = b * b - 4 * a * c;
											if (discriminant < 0)
												num = 0;
											else {
												float root = (float)STBTT_sqrt(discriminant);
												res[0] = (-b - root) / (2 * a);
												res[1] = (-b + root) / (2 * a);
												num = 2; // don't bother distinguishing 1-solution case, as code below will still work
											}
										}
									}
									else {
										float b = 3 * (ax * bx + ay * by) * a_inv; // could precompute this as it doesn't depend on sample point
										float c = (2 * (ax * ax + ay * ay) + (mx * bx + my * by)) * a_inv;
										float d = (mx * ax + my * ay) * a_inv;
										num = stbtt__solve_cubic(b, c, d, res);
									}
									dist2 = (x0 - sx) * (x0 - sx) + (y0 - sy) * (y0 - sy);
									if (dist2 < min_dist * min_dist)
										min_dist = (float)STBTT_sqrt(dist2);

									if (num >= 1 && res[0] >= 0.0f && res[0] <= 1.0f) {
										t = res[0], it = 1.0f - t;
										px = it * it * x0 + 2 * t * it * x1 + t * t * x2;
										py = it * it * y0 + 2 * t * it * y1 + t * t * y2;
										dist2 = (px - sx) * (px - sx) + (py - sy) * (py - sy);
										if (dist2 < min_dist * min_dist)
											min_dist = (float)STBTT_sqrt(dist2);
									}
									if (num >= 2 && res[1] >= 0.0f && res[1] <= 1.0f) {
										t = res[1], it = 1.0f - t;
										px = it * it * x0 + 2 * t * it * x1 + t * t * x2;
										py = it * it * y0 + 2 * t * it * y1 + t * t * y2;
										dist2 = (px - sx) * (px - sx) + (py - sy) * (py - sy);
										if (dist2 < min_dist * min_dist)
											min_dist = (float)STBTT_sqrt(dist2);
									}
									if (num >= 3 && res[2] >= 0.0f && res[2] <= 1.0f) {
										t = res[2], it = 1.0f - t;
										px = it * it * x0 + 2 * t * it * x1 + t * t * x2;
										py = it * it * y0 + 2 * t * it * y1 + t * t * y2;
										dist2 = (px - sx) * (px - sx) + (py - sy) * (py - sy);
										if (dist2 < min_dist * min_dist)
											min_dist = (float)STBTT_sqrt(dist2);
									}
								}
							}
						}
						if (winding == 0)
							min_dist = -min_dist;  // if outside the shape, value is negative
						val = onedge_value + pixel_dist_scale * min_dist;
						if (val < 0)
							val = 0;
						else if (val > 255)
							val = 255;
						data[(y - iy0) * w + (x - ix0)] = (unsigned char)val;
					}
				}
				STBTT_free(precompute, info->userdata);
				STBTT_free(verts, info->userdata);
			}
			return data;
		}

		STBTT_DEF unsigned char* stbtt_GetCodepointSDF(const stbtt_fontinfo* info, float scale, int codepoint, int padding, unsigned char onedge_value, float pixel_dist_scale, int* width, int* height, int* xoff, int* yoff)
		{
			return stbtt_GetGlyphSDF(info, scale, stbtt_FindGlyphIndex(info, codepoint), padding, onedge_value, pixel_dist_scale, width, height, xoff, yoff);
		}

		STBTT_DEF void stbtt_FreeSDF(unsigned char* bitmap, void* userdata)
		{
			STBTT_free(bitmap, userdata);
		}

		//////////////////////////////////////////////////////////////////////////////
		//
		// font name matching -- recommended not to use this
		//

		// check if a utf8 string contains a prefix which is the utf16 string; if so return length of matching utf8 string
		static stbtt_int32 stbtt__CompareUTF8toUTF16_bigendian_prefix(stbtt_uint8* s1, stbtt_int32 len1, stbtt_uint8* s2, stbtt_int32 len2)
		{
			stbtt_int32 i = 0;

			// convert utf16 to utf8 and compare the results while converting
			while (len2) {
				stbtt_uint16 ch = s2[0] * 256 + s2[1];
				if (ch < 0x80) {
					if (i >= len1) return -1;
					if (s1[i++] != ch) return -1;
				}
				else if (ch < 0x800) {
					if (i + 1 >= len1) return -1;
					if (s1[i++] != 0xc0 + (ch >> 6)) return -1;
					if (s1[i++] != 0x80 + (ch & 0x3f)) return -1;
				}
				else if (ch >= 0xd800 && ch < 0xdc00) {
					stbtt_uint32 c;
					stbtt_uint16 ch2 = s2[2] * 256 + s2[3];
					if (i + 3 >= len1) return -1;
					c = ((ch - 0xd800) << 10) + (ch2 - 0xdc00) + 0x10000;
					if (s1[i++] != 0xf0 + (c >> 18)) return -1;
					if (s1[i++] != 0x80 + ((c >> 12) & 0x3f)) return -1;
					if (s1[i++] != 0x80 + ((c >> 6) & 0x3f)) return -1;
					if (s1[i++] != 0x80 + ((c) & 0x3f)) return -1;
					s2 += 2; // plus another 2 below
					len2 -= 2;
				}
				else if (ch >= 0xdc00 && ch < 0xe000) {
					return -1;
				}
				else {
					if (i + 2 >= len1) return -1;
					if (s1[i++] != 0xe0 + (ch >> 12)) return -1;
					if (s1[i++] != 0x80 + ((ch >> 6) & 0x3f)) return -1;
					if (s1[i++] != 0x80 + ((ch) & 0x3f)) return -1;
				}
				s2 += 2;
				len2 -= 2;
			}
			return i;
		}

		static int stbtt_CompareUTF8toUTF16_bigendian_internal(char* s1, int len1, char* s2, int len2)
		{
			return len1 == stbtt__CompareUTF8toUTF16_bigendian_prefix((stbtt_uint8*)s1, len1, (stbtt_uint8*)s2, len2);
		}

		// returns results in whatever encoding you request... but note that 2-byte encodings
		// will be BIG-ENDIAN... use stbtt_CompareUTF8toUTF16_bigendian() to compare
		STBTT_DEF const char* stbtt_GetFontNameString(const stbtt_fontinfo* font, int* length, int platformID, int encodingID, int languageID, int nameID)
		{
			stbtt_int32 i, count, stringOffset;
			stbtt_uint8* fc = font->data;
			stbtt_uint32 offset = font->fontstart;
			stbtt_uint32 nm = stbtt__find_table(fc, offset, "name");
			if (!nm) return NULL;

			count = ttUSHORT(fc + nm + 2);
			stringOffset = nm + ttUSHORT(fc + nm + 4);
			for (i = 0; i < count; ++i) {
				stbtt_uint32 loc = nm + 6 + 12 * i;
				if (platformID == ttUSHORT(fc + loc + 0) && encodingID == ttUSHORT(fc + loc + 2)
					&& languageID == ttUSHORT(fc + loc + 4) && nameID == ttUSHORT(fc + loc + 6)) {
					*length = ttUSHORT(fc + loc + 8);
					return (const char*)(fc + stringOffset + ttUSHORT(fc + loc + 10));
				}
			}
			return NULL;
		}

		static int stbtt__matchpair(stbtt_uint8* fc, stbtt_uint32 nm, stbtt_uint8* name, stbtt_int32 nlen, stbtt_int32 target_id, stbtt_int32 next_id)
		{
			stbtt_int32 i;
			stbtt_int32 count = ttUSHORT(fc + nm + 2);
			stbtt_int32 stringOffset = nm + ttUSHORT(fc + nm + 4);

			for (i = 0; i < count; ++i) {
				stbtt_uint32 loc = nm + 6 + 12 * i;
				stbtt_int32 id = ttUSHORT(fc + loc + 6);
				if (id == target_id) {
					// find the encoding
					stbtt_int32 platform = ttUSHORT(fc + loc + 0), encoding = ttUSHORT(fc + loc + 2), language = ttUSHORT(fc + loc + 4);

					// is this a Unicode encoding?
					if (platform == 0 || (platform == 3 && encoding == 1) || (platform == 3 && encoding == 10)) {
						stbtt_int32 slen = ttUSHORT(fc + loc + 8);
						stbtt_int32 off = ttUSHORT(fc + loc + 10);

						// check if there's a prefix match
						stbtt_int32 matchlen = stbtt__CompareUTF8toUTF16_bigendian_prefix(name, nlen, fc + stringOffset + off, slen);
						if (matchlen >= 0) {
							// check for target_id+1 immediately following, with same encoding & language
							if (i + 1 < count && ttUSHORT(fc + loc + 12 + 6) == next_id && ttUSHORT(fc + loc + 12) == platform && ttUSHORT(fc + loc + 12 + 2) == encoding && ttUSHORT(fc + loc + 12 + 4) == language) {
								slen = ttUSHORT(fc + loc + 12 + 8);
								off = ttUSHORT(fc + loc + 12 + 10);
								if (slen == 0) {
									if (matchlen == nlen)
										return 1;
								}
								else if (matchlen < nlen && name[matchlen] == ' ') {
									++matchlen;
									if (stbtt_CompareUTF8toUTF16_bigendian_internal((char*)(name + matchlen), nlen - matchlen, (char*)(fc + stringOffset + off), slen))
										return 1;
								}
							}
							else {
								// if nothing immediately following
								if (matchlen == nlen)
									return 1;
							}
						}
					}

					// @TODO handle other encodings
				}
			}
			return 0;
		}

		static int stbtt__matches(stbtt_uint8* fc, stbtt_uint32 offset, stbtt_uint8* name, stbtt_int32 flags)
		{
			stbtt_int32 nlen = (stbtt_int32)STBTT_strlen((char*)name);
			stbtt_uint32 nm, hd;
			if (!stbtt__isfont(fc + offset)) return 0;

			// check italics/bold/underline flags in macStyle...
			if (flags) {
				hd = stbtt__find_table(fc, offset, "head");
				if ((ttUSHORT(fc + hd + 44) & 7) != (flags & 7)) return 0;
			}

			nm = stbtt__find_table(fc, offset, "name");
			if (!nm) return 0;

			if (flags) {
				// if we checked the macStyle flags, then just check the family and ignore the subfamily
				if (stbtt__matchpair(fc, nm, name, nlen, 16, -1))  return 1;
				if (stbtt__matchpair(fc, nm, name, nlen, 1, -1))  return 1;
				if (stbtt__matchpair(fc, nm, name, nlen, 3, -1))  return 1;
			}
			else {
				if (stbtt__matchpair(fc, nm, name, nlen, 16, 17))  return 1;
				if (stbtt__matchpair(fc, nm, name, nlen, 1, 2))  return 1;
				if (stbtt__matchpair(fc, nm, name, nlen, 3, -1))  return 1;
			}

			return 0;
		}

		static int stbtt_FindMatchingFont_internal(unsigned char* font_collection, char* name_utf8, stbtt_int32 flags)
		{
			stbtt_int32 i;
			for (i = 0;; ++i) {
				stbtt_int32 off = stbtt_GetFontOffsetForIndex(font_collection, i);
				if (off < 0) return off;
				if (stbtt__matches((stbtt_uint8*)font_collection, off, (stbtt_uint8*)name_utf8, flags))
					return off;
			}
		}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif

		STBTT_DEF int stbtt_BakeFontBitmap(const unsigned char* data, int offset,
			float pixel_height, unsigned char* pixels, int pw, int ph,
			int first_char, int num_chars, stbtt_bakedchar* chardata)
		{
			return stbtt_BakeFontBitmap_internal((unsigned char*)data, offset, pixel_height, pixels, pw, ph, first_char, num_chars, chardata);
		}

		STBTT_DEF int stbtt_GetFontOffsetForIndex(const unsigned char* data, int index)
		{
			return stbtt_GetFontOffsetForIndex_internal((unsigned char*)data, index);
		}

		STBTT_DEF int stbtt_GetNumberOfFonts(const unsigned char* data)
		{
			return stbtt_GetNumberOfFonts_internal((unsigned char*)data);
		}

		STBTT_DEF int stbtt_InitFont(stbtt_fontinfo* info, const unsigned char* data, int offset)
		{
			return stbtt_InitFont_internal(info, (unsigned char*)data, offset);
		}

		STBTT_DEF int stbtt_FindMatchingFont(const unsigned char* fontdata, const char* name, int flags)
		{
			return stbtt_FindMatchingFont_internal((unsigned char*)fontdata, (char*)name, flags);
		}

		STBTT_DEF int stbtt_CompareUTF8toUTF16_bigendian(const char* s1, int len1, const char* s2, int len2)
		{
			return stbtt_CompareUTF8toUTF16_bigendian_internal((char*)s1, len1, (char*)s2, len2);
		}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#endif // STB_TRUETYPE_IMPLEMENTATION


		// FULL VERSION HISTORY
		//
		//   1.25 (2021-07-11) many fixes
		//   1.24 (2020-02-05) fix warning
		//   1.23 (2020-02-02) query SVG data for glyphs; query whole kerning table (but only kern not GPOS)
		//   1.22 (2019-08-11) minimize missing-glyph duplication; fix kerning if both 'GPOS' and 'kern' are defined
		//   1.21 (2019-02-25) fix warning
		//   1.20 (2019-02-07) PackFontRange skips missing codepoints; GetScaleFontVMetrics()
		//   1.19 (2018-02-11) OpenType GPOS kerning (horizontal only), STBTT_fmod
		//   1.18 (2018-01-29) add missing function
		//   1.17 (2017-07-23) make more arguments const; doc fix
		//   1.16 (2017-07-12) SDF support
		//   1.15 (2017-03-03) make more arguments const
		//   1.14 (2017-01-16) num-fonts-in-TTC function
		//   1.13 (2017-01-02) support OpenType fonts, certain Apple fonts
		//   1.12 (2016-10-25) suppress warnings about casting away const with -Wcast-qual
		//   1.11 (2016-04-02) fix unused-variable warning
		//   1.10 (2016-04-02) allow user-defined fabs() replacement
		//                     fix memory leak if fontsize=0.0
		//                     fix warning from duplicate typedef
		//   1.09 (2016-01-16) warning fix; avoid crash on outofmem; use alloc userdata for PackFontRanges
		//   1.08 (2015-09-13) document stbtt_Rasterize(); fixes for vertical & horizontal edges
		//   1.07 (2015-08-01) allow PackFontRanges to accept arrays of sparse codepoints;
		//                     allow PackFontRanges to pack and render in separate phases;
		//                     fix stbtt_GetFontOFfsetForIndex (never worked for non-0 input?);
		//                     fixed an assert() bug in the new rasterizer
		//                     replace assert() with STBTT_assert() in new rasterizer
		//   1.06 (2015-07-14) performance improvements (~35% faster on x86 and x64 on test machine)
		//                     also more precise AA rasterizer, except if shapes overlap
		//                     remove need for STBTT_sort
		//   1.05 (2015-04-15) fix misplaced definitions for STBTT_STATIC
		//   1.04 (2015-04-15) typo in example
		//   1.03 (2015-04-12) STBTT_STATIC, fix memory leak in new packing, various fixes
		//   1.02 (2014-12-10) fix various warnings & compile issues w/ stb_rect_pack, C++
		//   1.01 (2014-12-08) fix subpixel position when oversampling to exactly match
		//                        non-oversampled; STBTT_POINT_SIZE for packed case only
		//   1.00 (2014-12-06) add new PackBegin etc. API, w/ support for oversampling
		//   0.99 (2014-09-18) fix multiple bugs with subpixel rendering (ryg)
		//   0.9  (2014-08-07) support certain mac/iOS fonts without an MS platformID
		//   0.8b (2014-07-07) fix a warning
		//   0.8  (2014-05-25) fix a few more warnings
		//   0.7  (2013-09-25) bugfix: subpixel glyph bug fixed in 0.5 had come back
		//   0.6c (2012-07-24) improve documentation
		//   0.6b (2012-07-20) fix a few more warnings
		//   0.6  (2012-07-17) fix warnings; added stbtt_ScaleForMappingEmToPixels,
		//                        stbtt_GetFontBoundingBox, stbtt_IsGlyphEmpty
		//   0.5  (2011-12-09) bugfixes:
		//                        subpixel glyph renderer computed wrong bounding box
		//                        first vertex of shape can be off-curve (FreeSans)
		//   0.4b (2011-12-03) fixed an error in the font baking example
		//   0.4  (2011-12-01) kerning, subpixel rendering (tor)
		//                    bugfixes for:
		//                        codepoint-to-glyph conversion using table fmt=12
		//                        codepoint-to-glyph conversion using table fmt=4
		//                        stbtt_GetBakedQuad with non-square texture (Zer)
		//                    updated Hello World! sample to use kerning and subpixel
		//                    fixed some warnings
		//   0.3  (2009-06-24) cmap fmt=12, compound shapes (MM)
		//                    userdata, malloc-from-userdata, non-zero fill (stb)
		//   0.2  (2009-03-11) Fix unsigned/signed char warnings
		//   0.1  (2009-03-09) First public release
		//

		/*
		------------------------------------------------------------------------------
		This software is available under 2 licenses -- choose whichever you prefer.
		------------------------------------------------------------------------------
		ALTERNATIVE A - MIT License
		Copyright (c) 2017 Sean Barrett
		Permission is hereby granted, free of charge, to any person obtaining a copy of
		this software and associated documentation files (the "Software"), to deal in
		the Software without restriction, including without limitation the rights to
		use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
		of the Software, and to permit persons to whom the Software is furnished to do
		so, subject to the following conditions:
		The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.
		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
		SOFTWARE.
		------------------------------------------------------------------------------
		ALTERNATIVE B - Public Domain (www.unlicense.org)
		This is free and unencumbered software released into the public domain.
		Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
		software, either in source code form or as a compiled binary, for any purpose,
		commercial or non-commercial, and by any means.
		In jurisdictions that recognize copyright laws, the author or authors of this
		software dedicate any and all copyright interest in the software to the public
		domain. We make this dedication for the benefit of the public at large and to
		the detriment of our heirs and successors. We intend this dedication to be an
		overt act of relinquishment in perpetuity of all present and future rights to
		this software under copyright law.
		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
		ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
		WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
		------------------------------------------------------------------------------
		*/

#define STB_IMAGE_WRITE_IMPLEMENTATION
        /* stb_image_write - v1.16 - public domain - http://nothings.org/stb
           writes out PNG/BMP/TGA/JPEG/HDR images to C stdio - Sean Barrett 2010-2015
                                             no warranty implied; use at your own risk

           Before #including,

               #define STB_IMAGE_WRITE_IMPLEMENTATION

           in the file that you want to have the implementation.

           Will probably not work correctly with strict-aliasing optimizations.

        ABOUT:

           This header file is a library for writing images to C stdio or a callback.

           The PNG output is not optimal; it is 20-50% larger than the file
           written by a decent optimizing implementation; though providing a custom
           zlib compress function (see STBIW_ZLIB_COMPRESS) can mitigate that.
           This library is designed for source code compactness and simplicity,
           not optimal image file size or run-time performance.

        BUILDING:

           You can #define STBIW_ASSERT(x) before the #include to avoid using assert.h.
           You can #define STBIW_MALLOC(), STBIW_REALLOC(), and STBIW_FREE() to replace
           malloc,realloc,free.
           You can #define STBIW_MEMMOVE() to replace memmove()
           You can #define STBIW_ZLIB_COMPRESS to use a custom zlib-style compress function
           for PNG compression (instead of the builtin one), it must have the following signature:
           unsigned char * my_compress(unsigned char *data, int data_len, int *out_len, int quality);
           The returned data will be freed with STBIW_FREE() (free() by default),
           so it must be heap allocated with STBIW_MALLOC() (malloc() by default),

        UNICODE:

           If compiling for Windows and you wish to use Unicode filenames, compile
           with
               #define STBIW_WINDOWS_UTF8
           and pass utf8-encoded filenames. Call stbiw_convert_wchar_to_utf8 to convert
           Windows wchar_t filenames to utf8.

        USAGE:

           There are five functions, one for each image file format:

             int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
             int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);
             int stbi_write_tga(char const *filename, int w, int h, int comp, const void *data);
             int stbi_write_jpg(char const *filename, int w, int h, int comp, const void *data, int quality);
             int stbi_write_hdr(char const *filename, int w, int h, int comp, const float *data);

             void stbi_flip_vertically_on_write(int flag); // flag is non-zero to flip data vertically

           There are also five equivalent functions that use an arbitrary write function. You are
           expected to open/close your file-equivalent before and after calling these:

             int stbi_write_png_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data, int stride_in_bytes);
             int stbi_write_bmp_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data);
             int stbi_write_tga_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data);
             int stbi_write_hdr_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const float *data);
             int stbi_write_jpg_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const void *data, int quality);

           where the callback is:
              void stbi_write_func(void *context, void *data, int size);

           You can configure it with these global variables:
              int stbi_write_tga_with_rle;             // defaults to true; set to 0 to disable RLE
              int stbi_write_png_compression_level;    // defaults to 8; set to higher for more compression
              int stbi_write_force_png_filter;         // defaults to -1; set to 0..5 to force a filter mode


           You can define STBI_WRITE_NO_STDIO to disable the file variant of these
           functions, so the library will not use stdio.h at all. However, this will
           also disable HDR writing, because it requires stdio for formatted output.

           Each function returns 0 on failure and non-0 on success.

           The functions create an image file defined by the parameters. The image
           is a rectangle of pixels stored from left-to-right, top-to-bottom.
           Each pixel contains 'comp' channels of data stored interleaved with 8-bits
           per channel, in the following order: 1=Y, 2=YA, 3=RGB, 4=RGBA. (Y is
           monochrome color.) The rectangle is 'w' pixels wide and 'h' pixels tall.
           The *data pointer points to the first byte of the top-left-most pixel.
           For PNG, "stride_in_bytes" is the distance in bytes from the first byte of
           a row of pixels to the first byte of the next row of pixels.

           PNG creates output files with the same number of components as the input.
           The BMP format expands Y to RGB in the file format and does not
           output alpha.

           PNG supports writing rectangles of data even when the bytes storing rows of
           data are not consecutive in memory (e.g. sub-rectangles of a larger image),
           by supplying the stride between the beginning of adjacent rows. The other
           formats do not. (Thus you cannot write a native-format BMP through the BMP
           writer, both because it is in BGR order and because it may have padding
           at the end of the line.)

           PNG allows you to set the deflate compression level by setting the global
           variable 'stbi_write_png_compression_level' (it defaults to 8).

           HDR expects linear float data. Since the format is always 32-bit rgb(e)
           data, alpha (if provided) is discarded, and for monochrome data it is
           replicated across all three channels.

           TGA supports RLE or non-RLE compressed data. To use non-RLE-compressed
           data, set the global variable 'stbi_write_tga_with_rle' to 0.

           JPEG does ignore alpha channels in input data; quality is between 1 and 100.
           Higher quality looks better but results in a bigger image.
           JPEG baseline (no JPEG progressive).

        CREDITS:


           Sean Barrett           -    PNG/BMP/TGA
           Baldur Karlsson        -    HDR
           Jean-Sebastien Guay    -    TGA monochrome
           Tim Kelsey             -    misc enhancements
           Alan Hickman           -    TGA RLE
           Emmanuel Julien        -    initial file IO callback implementation
           Jon Olick              -    original jo_jpeg.cpp code
           Daniel Gibson          -    integrate JPEG, allow external zlib
           Aarni Koskela          -    allow choosing PNG filter

           bugfixes:
              github:Chribba
              Guillaume Chereau
              github:jry2
              github:romigrou
              Sergio Gonzalez
              Jonas Karlsson
              Filip Wasil
              Thatcher Ulrich
              github:poppolopoppo
              Patrick Boettcher
              github:xeekworx
              Cap Petschulat
              Simon Rodriguez
              Ivan Tikhonov
              github:ignotion
              Adam Schackart
              Andrew Kensler

        LICENSE

          See end of file for license information.

        */

#ifndef INCLUDE_STB_IMAGE_WRITE_H
#define INCLUDE_STB_IMAGE_WRITE_H

#include <stdlib.h>

        // if STB_IMAGE_WRITE_STATIC causes problems, try defining STBIWDEF to 'inline' or 'static inline'
#ifndef STBIWDEF
#ifdef STB_IMAGE_WRITE_STATIC
#define STBIWDEF  static
#else
#ifdef __cplusplus
#define STBIWDEF  extern "C"
#else
#define STBIWDEF  extern
#endif
#endif
#endif

#ifndef STB_IMAGE_WRITE_STATIC  // C++ forbids static forward declarations
STBIWDEF int stbi_write_tga_with_rle;
STBIWDEF int stbi_write_png_compression_level;
STBIWDEF int stbi_write_force_png_filter;
#endif

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_png(char const* filename, int w, int h, int comp, const void* data, int stride_in_bytes);
STBIWDEF int stbi_write_bmp(char const* filename, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_tga(char const* filename, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_hdr(char const* filename, int w, int h, int comp, const float* data);
STBIWDEF int stbi_write_jpg(char const* filename, int x, int y, int comp, const void* data, int quality);

#ifdef STBIW_WINDOWS_UTF8
STBIWDEF int stbiw_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input);
#endif
#endif

typedef void stbi_write_func(void* context, void* data, int size);

STBIWDEF int stbi_write_png_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const void* data, int stride_in_bytes);
STBIWDEF int stbi_write_bmp_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_tga_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const void* data);
STBIWDEF int stbi_write_hdr_to_func(stbi_write_func* func, void* context, int w, int h, int comp, const float* data);
STBIWDEF int stbi_write_jpg_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int quality);

STBIWDEF void stbi_flip_vertically_on_write(int flip_boolean);

#endif//INCLUDE_STB_IMAGE_WRITE_H

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
#endif

#ifndef STBI_WRITE_NO_STDIO
#include <stdio.h>
#endif // STBI_WRITE_NO_STDIO

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(STBIW_MALLOC) && defined(STBIW_FREE) && (defined(STBIW_REALLOC) || defined(STBIW_REALLOC_SIZED))
// ok
#elif !defined(STBIW_MALLOC) && !defined(STBIW_FREE) && !defined(STBIW_REALLOC) && !defined(STBIW_REALLOC_SIZED)
// ok
#else
#error "Must define all or none of STBIW_MALLOC, STBIW_FREE, and STBIW_REALLOC (or STBIW_REALLOC_SIZED)."
#endif

#ifndef STBIW_MALLOC
#define STBIW_MALLOC(sz)        malloc(sz)
#define STBIW_REALLOC(p,newsz)  realloc(p,newsz)
#define STBIW_FREE(p)           free(p)
#endif

#ifndef STBIW_REALLOC_SIZED
#define STBIW_REALLOC_SIZED(p,oldsz,newsz) STBIW_REALLOC(p,newsz)
#endif


#ifndef STBIW_MEMMOVE
#define STBIW_MEMMOVE(a,b,sz) memmove(a,b,sz)
#endif


#ifndef STBIW_ASSERT
#include <assert.h>
#define STBIW_ASSERT(x) assert(x)
#endif

#define STBIW_UCHAR(x) (unsigned char) ((x) & 0xff)

#ifdef STB_IMAGE_WRITE_STATIC
static int stbi_write_png_compression_level = 8;
static int stbi_write_tga_with_rle = 1;
static int stbi_write_force_png_filter = -1;
#else
int stbi_write_png_compression_level = 8;
int stbi_write_tga_with_rle = 1;
int stbi_write_force_png_filter = -1;
#endif

static int stbi__flip_vertically_on_write = 0;

STBIWDEF void stbi_flip_vertically_on_write(int flag)
{
    stbi__flip_vertically_on_write = flag;
}

typedef struct
{
    stbi_write_func* func;
    void* context;
    unsigned char buffer[64];
    int buf_used;
} stbi__write_context;

// initialize a callback-based context
static void stbi__start_write_callbacks(stbi__write_context* s, stbi_write_func* c, void* context)
{
    s->func = c;
    s->context = context;
}

#ifndef STBI_WRITE_NO_STDIO

static void stbi__stdio_write(void* context, void* data, int size)
{
    fwrite(data, 1, size, (FILE*)context);
}

#if defined(_WIN32) && defined(STBIW_WINDOWS_UTF8)
#ifdef __cplusplus
#define STBIW_EXTERN extern "C"
#else
#define STBIW_EXTERN extern
#endif
STBIW_EXTERN __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags, const char* str, int cbmb, wchar_t* widestr, int cchwide);
STBIW_EXTERN __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t* widestr, int cchwide, char* str, int cbmb, const char* defchar, int* used_default);

STBIWDEF int stbiw_convert_wchar_to_utf8(char* buffer, size_t bufferlen, const wchar_t* input)
{
    return WideCharToMultiByte(65001 /* UTF8 */, 0, input, -1, buffer, (int)bufferlen, NULL, NULL);
}
#endif

static FILE* stbiw__fopen(char const* filename, char const* mode)
{
    FILE* f;
#if defined(_WIN32) && defined(STBIW_WINDOWS_UTF8)
    wchar_t wMode[64];
    wchar_t wFilename[1024];
    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, filename, -1, wFilename, sizeof(wFilename) / sizeof(*wFilename)))
        return 0;

    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, mode, -1, wMode, sizeof(wMode) / sizeof(*wMode)))
        return 0;

#if defined(_MSC_VER) && _MSC_VER >= 1400
    if (0 != _wfopen_s(&f, wFilename, wMode))
        f = 0;
#else
    f = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
    if (0 != fopen_s(&f, filename, mode))
        f = 0;
#else
    f = fopen(filename, mode);
#endif
    return f;
}

static int stbi__start_write_file(stbi__write_context* s, const char* filename)
{
    FILE* f = stbiw__fopen(filename, "wb");
    stbi__start_write_callbacks(s, stbi__stdio_write, (void*)f);
    return f != NULL;
}

static void stbi__end_write_file(stbi__write_context* s)
{
    fclose((FILE*)s->context);
}

#endif // !STBI_WRITE_NO_STDIO

typedef unsigned int stbiw_uint32;
typedef int stb_image_write_test[sizeof(stbiw_uint32) == 4 ? 1 : -1];

static void stbiw__writefv(stbi__write_context* s, const char* fmt, va_list v)
{
    while (*fmt) {
        switch (*fmt++) {
        case ' ': break;
        case '1': { unsigned char x = STBIW_UCHAR(va_arg(v, int));
            s->func(s->context, &x, 1);
            break; }
        case '2': { int x = va_arg(v, int);
            unsigned char b[2];
            b[0] = STBIW_UCHAR(x);
            b[1] = STBIW_UCHAR(x >> 8);
            s->func(s->context, b, 2);
            break; }
        case '4': { stbiw_uint32 x = va_arg(v, int);
            unsigned char b[4];
            b[0] = STBIW_UCHAR(x);
            b[1] = STBIW_UCHAR(x >> 8);
            b[2] = STBIW_UCHAR(x >> 16);
            b[3] = STBIW_UCHAR(x >> 24);
            s->func(s->context, b, 4);
            break; }
        default:
            STBIW_ASSERT(0);
            return;
        }
    }
}

static void stbiw__writef(stbi__write_context* s, const char* fmt, ...)
{
    va_list v;
    va_start(v, fmt);
    stbiw__writefv(s, fmt, v);
    va_end(v);
}

static void stbiw__write_flush(stbi__write_context* s)
{
    if (s->buf_used) {
        s->func(s->context, &s->buffer, s->buf_used);
        s->buf_used = 0;
    }
}

static void stbiw__putc(stbi__write_context* s, unsigned char c)
{
    s->func(s->context, &c, 1);
}

static void stbiw__write1(stbi__write_context* s, unsigned char a)
{
    if ((size_t)s->buf_used + 1 > sizeof(s->buffer))
        stbiw__write_flush(s);
    s->buffer[s->buf_used++] = a;
}

static void stbiw__write3(stbi__write_context* s, unsigned char a, unsigned char b, unsigned char c)
{
    int n;
    if ((size_t)s->buf_used + 3 > sizeof(s->buffer))
        stbiw__write_flush(s);
    n = s->buf_used;
    s->buf_used = n + 3;
    s->buffer[n + 0] = a;
    s->buffer[n + 1] = b;
    s->buffer[n + 2] = c;
}

static void stbiw__write_pixel(stbi__write_context* s, int rgb_dir, int comp, int write_alpha, int expand_mono, unsigned char* d)
{
    unsigned char bg[3] = { 255, 0, 255 }, px[3];
    int k;

    if (write_alpha < 0)
        stbiw__write1(s, d[comp - 1]);

    switch (comp) {
    case 2: // 2 pixels = mono + alpha, alpha is written separately, so same as 1-channel case
    case 1:
        if (expand_mono)
            stbiw__write3(s, d[0], d[0], d[0]); // monochrome bmp
        else
            stbiw__write1(s, d[0]);  // monochrome TGA
        break;
    case 4:
        if (!write_alpha) {
            // composite against pink background
            for (k = 0; k < 3; ++k)
                px[k] = bg[k] + ((d[k] - bg[k]) * d[3]) / 255;
            stbiw__write3(s, px[1 - rgb_dir], px[1], px[1 + rgb_dir]);
            break;
        }
        /* FALLTHROUGH */
    case 3:
        stbiw__write3(s, d[1 - rgb_dir], d[1], d[1 + rgb_dir]);
        break;
    }
    if (write_alpha > 0)
        stbiw__write1(s, d[comp - 1]);
}

static void stbiw__write_pixels(stbi__write_context* s, int rgb_dir, int vdir, int x, int y, int comp, void* data, int write_alpha, int scanline_pad, int expand_mono)
{
    stbiw_uint32 zero = 0;
    int i, j, j_end;

    if (y <= 0)
        return;

    if (stbi__flip_vertically_on_write)
        vdir *= -1;

    if (vdir < 0) {
        j_end = -1; j = y - 1;
    }
    else {
        j_end = y; j = 0;
    }

    for (; j != j_end; j += vdir) {
        for (i = 0; i < x; ++i) {
            unsigned char* d = (unsigned char*)data + (j * x + i) * comp;
            stbiw__write_pixel(s, rgb_dir, comp, write_alpha, expand_mono, d);
        }
        stbiw__write_flush(s);
        s->func(s->context, &zero, scanline_pad);
    }
}

static int stbiw__outfile(stbi__write_context* s, int rgb_dir, int vdir, int x, int y, int comp, int expand_mono, void* data, int alpha, int pad, const char* fmt, ...)
{
    if (y < 0 || x < 0) {
        return 0;
    }
    else {
        va_list v;
        va_start(v, fmt);
        stbiw__writefv(s, fmt, v);
        va_end(v);
        stbiw__write_pixels(s, rgb_dir, vdir, x, y, comp, data, alpha, pad, expand_mono);
        return 1;
    }
}

static int stbi_write_bmp_core(stbi__write_context* s, int x, int y, int comp, const void* data)
{
    if (comp != 4) {
        // write RGB bitmap
        int pad = (-x * 3) & 3;
        return stbiw__outfile(s, -1, -1, x, y, comp, 1, (void*)data, 0, pad,
            "11 4 22 4" "4 44 22 444444",
            'B', 'M', 14 + 40 + (x * 3 + pad) * y, 0, 0, 14 + 40,  // file header
            40, x, y, 1, 24, 0, 0, 0, 0, 0, 0);             // bitmap header
    }
    else {
        // RGBA bitmaps need a v4 header
        // use BI_BITFIELDS mode with 32bpp and alpha mask
        // (straight BI_RGB with alpha mask doesn't work in most readers)
        return stbiw__outfile(s, -1, -1, x, y, comp, 1, (void*)data, 1, 0,
            "11 4 22 4" "4 44 22 444444 4444 4 444 444 444 444",
            'B', 'M', 14 + 108 + x * y * 4, 0, 0, 14 + 108, // file header
            108, x, y, 1, 32, 3, 0, 0, 0, 0, 0, 0xff0000, 0xff00, 0xff, 0xff000000u, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // bitmap V4 header
    }
}

STBIWDEF int stbi_write_bmp_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_bmp_core(&s, x, y, comp, data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_bmp(char const* filename, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_bmp_core(&s, x, y, comp, data);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif //!STBI_WRITE_NO_STDIO

static int stbi_write_tga_core(stbi__write_context* s, int x, int y, int comp, void* data)
{
    int has_alpha = (comp == 2 || comp == 4);
    int colorbytes = has_alpha ? comp - 1 : comp;
    int format = colorbytes < 2 ? 3 : 2; // 3 color channels (RGB/RGBA) = 2, 1 color channel (Y/YA) = 3

    if (y < 0 || x < 0)
        return 0;

    if (!stbi_write_tga_with_rle) {
        return stbiw__outfile(s, -1, -1, x, y, comp, 0, (void*)data, has_alpha, 0,
            "111 221 2222 11", 0, 0, format, 0, 0, 0, 0, 0, x, y, (colorbytes + has_alpha) * 8, has_alpha * 8);
    }
    else {
        int i, j, k;
        int jend, jdir;

        stbiw__writef(s, "111 221 2222 11", 0, 0, format + 8, 0, 0, 0, 0, 0, x, y, (colorbytes + has_alpha) * 8, has_alpha * 8);

        if (stbi__flip_vertically_on_write) {
            j = 0;
            jend = y;
            jdir = 1;
        }
        else {
            j = y - 1;
            jend = -1;
            jdir = -1;
        }
        for (; j != jend; j += jdir) {
            unsigned char* row = (unsigned char*)data + j * x * comp;
            int len;

            for (i = 0; i < x; i += len) {
                unsigned char* begin = row + i * comp;
                int diff = 1;
                len = 1;

                if (i < x - 1) {
                    ++len;
                    diff = memcmp(begin, row + (i + 1) * comp, comp);
                    if (diff) {
                        const unsigned char* prev = begin;
                        for (k = i + 2; k < x && len < 128; ++k) {
                            if (memcmp(prev, row + k * comp, comp)) {
                                prev += comp;
                                ++len;
                            }
                            else {
                                --len;
                                break;
                            }
                        }
                    }
                    else {
                        for (k = i + 2; k < x && len < 128; ++k) {
                            if (!memcmp(begin, row + k * comp, comp)) {
                                ++len;
                            }
                            else {
                                break;
                            }
                        }
                    }
                }

                if (diff) {
                    unsigned char header = STBIW_UCHAR(len - 1);
                    stbiw__write1(s, header);
                    for (k = 0; k < len; ++k) {
                        stbiw__write_pixel(s, -1, comp, has_alpha, 0, begin + k * comp);
                    }
                }
                else {
                    unsigned char header = STBIW_UCHAR(len - 129);
                    stbiw__write1(s, header);
                    stbiw__write_pixel(s, -1, comp, has_alpha, 0, begin);
                }
            }
        }
        stbiw__write_flush(s);
    }
    return 1;
}

STBIWDEF int stbi_write_tga_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_tga_core(&s, x, y, comp, (void*)data);
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_tga(char const* filename, int x, int y, int comp, const void* data)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_tga_core(&s, x, y, comp, (void*)data);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif

// *************************************************************************************************
// Radiance RGBE HDR writer
// by Baldur Karlsson

#define stbiw__max(a, b)  ((a) > (b) ? (a) : (b))

#ifndef STBI_WRITE_NO_STDIO

static void stbiw__linear_to_rgbe(unsigned char* rgbe, float* linear)
{
    int exponent;
    float maxcomp = stbiw__max(linear[0], stbiw__max(linear[1], linear[2]));

    if (maxcomp < 1e-32f) {
        rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
    }
    else {
        float normalize = (float)frexp(maxcomp, &exponent) * 256.0f / maxcomp;

        rgbe[0] = (unsigned char)(linear[0] * normalize);
        rgbe[1] = (unsigned char)(linear[1] * normalize);
        rgbe[2] = (unsigned char)(linear[2] * normalize);
        rgbe[3] = (unsigned char)(exponent + 128);
    }
}

static void stbiw__write_run_data(stbi__write_context* s, int length, unsigned char databyte)
{
    unsigned char lengthbyte = STBIW_UCHAR(length + 128);
    STBIW_ASSERT(length + 128 <= 255);
    s->func(s->context, &lengthbyte, 1);
    s->func(s->context, &databyte, 1);
}

static void stbiw__write_dump_data(stbi__write_context* s, int length, unsigned char* data)
{
    unsigned char lengthbyte = STBIW_UCHAR(length);
    STBIW_ASSERT(length <= 128); // inconsistent with spec but consistent with official code
    s->func(s->context, &lengthbyte, 1);
    s->func(s->context, data, length);
}

static void stbiw__write_hdr_scanline(stbi__write_context* s, int width, int ncomp, unsigned char* scratch, float* scanline)
{
    unsigned char scanlineheader[4] = { 2, 2, 0, 0 };
    unsigned char rgbe[4];
    float linear[3];
    int x;

    scanlineheader[2] = (width & 0xff00) >> 8;
    scanlineheader[3] = (width & 0x00ff);

    /* skip RLE for images too small or large */
    if (width < 8 || width >= 32768) {
        for (x = 0; x < width; x++) {
            switch (ncomp) {
            case 4: /* fallthrough */
            case 3: linear[2] = scanline[x * ncomp + 2];
                linear[1] = scanline[x * ncomp + 1];
                linear[0] = scanline[x * ncomp + 0];
                break;
            default:
                linear[0] = linear[1] = linear[2] = scanline[x * ncomp + 0];
                break;
            }
            stbiw__linear_to_rgbe(rgbe, linear);
            s->func(s->context, rgbe, 4);
        }
    }
    else {
        int c, r;
        /* encode into scratch buffer */
        for (x = 0; x < width; x++) {
            switch (ncomp) {
            case 4: /* fallthrough */
            case 3: linear[2] = scanline[x * ncomp + 2];
                linear[1] = scanline[x * ncomp + 1];
                linear[0] = scanline[x * ncomp + 0];
                break;
            default:
                linear[0] = linear[1] = linear[2] = scanline[x * ncomp + 0];
                break;
            }
            stbiw__linear_to_rgbe(rgbe, linear);
            scratch[x + width * 0] = rgbe[0];
            scratch[x + width * 1] = rgbe[1];
            scratch[x + width * 2] = rgbe[2];
            scratch[x + width * 3] = rgbe[3];
        }

        s->func(s->context, scanlineheader, 4);

        /* RLE each component separately */
        for (c = 0; c < 4; c++) {
            unsigned char* comp = &scratch[width * c];

            x = 0;
            while (x < width) {
                // find first run
                r = x;
                while (r + 2 < width) {
                    if (comp[r] == comp[r + 1] && comp[r] == comp[r + 2])
                        break;
                    ++r;
                }
                if (r + 2 >= width)
                    r = width;
                // dump up to first run
                while (x < r) {
                    int len = r - x;
                    if (len > 128) len = 128;
                    stbiw__write_dump_data(s, len, &comp[x]);
                    x += len;
                }
                // if there's a run, output it
                if (r + 2 < width) { // same test as what we break out of in search loop, so only true if we break'd
                   // find next byte after run
                    while (r < width && comp[r] == comp[x])
                        ++r;
                    // output run up to r
                    while (x < r) {
                        int len = r - x;
                        if (len > 127) len = 127;
                        stbiw__write_run_data(s, len, comp[x]);
                        x += len;
                    }
                }
            }
        }
    }
}

static int stbi_write_hdr_core(stbi__write_context* s, int x, int y, int comp, float* data)
{
    if (y <= 0 || x <= 0 || data == NULL)
        return 0;
    else {
        // Each component is stored separately. Allocate scratch space for full output scanline.
        unsigned char* scratch = (unsigned char*)STBIW_MALLOC(x * 4);
        int i, len;
        char buffer[128];
        char header[] = "#?RADIANCE\n# Written by stb_image_write.h\nFORMAT=32-bit_rle_rgbe\n";
        s->func(s->context, header, sizeof(header) - 1);

#ifdef __STDC_LIB_EXT1__
        len = sprintf_s(buffer, sizeof(buffer), "EXPOSURE=          1.0000000000000\n\n-Y %d +X %d\n", y, x);
#else
        len = sprintf(buffer, "EXPOSURE=          1.0000000000000\n\n-Y %d +X %d\n", y, x);
#endif
        s->func(s->context, buffer, len);

        for (i = 0; i < y; i++)
            stbiw__write_hdr_scanline(s, x, comp, scratch, data + comp * x * (stbi__flip_vertically_on_write ? y - 1 - i : i));
        STBIW_FREE(scratch);
        return 1;
    }
}

STBIWDEF int stbi_write_hdr_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const float* data)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_hdr_core(&s, x, y, comp, (float*)data);
}

STBIWDEF int stbi_write_hdr(char const* filename, int x, int y, int comp, const float* data)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_hdr_core(&s, x, y, comp, (float*)data);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif // STBI_WRITE_NO_STDIO


//////////////////////////////////////////////////////////////////////////////
//
// PNG writer
//

#ifndef STBIW_ZLIB_COMPRESS
// stretchy buffer; stbiw__sbpush() == vector<>::push_back() -- stbiw__sbcount() == vector<>::size()
#define stbiw__sbraw(a) ((int *) (void *) (a) - 2)
#define stbiw__sbm(a)   stbiw__sbraw(a)[0]
#define stbiw__sbn(a)   stbiw__sbraw(a)[1]

#define stbiw__sbneedgrow(a,n)  ((a)==0 || stbiw__sbn(a)+n >= stbiw__sbm(a))
#define stbiw__sbmaybegrow(a,n) (stbiw__sbneedgrow(a,(n)) ? stbiw__sbgrow(a,n) : 0)
#define stbiw__sbgrow(a,n)  stbiw__sbgrowf((void **) &(a), (n), sizeof(*(a)))

#define stbiw__sbpush(a, v)      (stbiw__sbmaybegrow(a,1), (a)[stbiw__sbn(a)++] = (v))
#define stbiw__sbcount(a)        ((a) ? stbiw__sbn(a) : 0)
#define stbiw__sbfree(a)         ((a) ? STBIW_FREE(stbiw__sbraw(a)),0 : 0)

static void* stbiw__sbgrowf(void** arr, int increment, int itemsize)
{
    int m = *arr ? 2 * stbiw__sbm(*arr) + increment : increment + 1;
    void* p = STBIW_REALLOC_SIZED(*arr ? stbiw__sbraw(*arr) : 0, *arr ? (stbiw__sbm(*arr) * itemsize + sizeof(int) * 2) : 0, itemsize * m + sizeof(int) * 2);
    STBIW_ASSERT(p);
    if (p) {
        if (!*arr) ((int*)p)[1] = 0;
        *arr = (void*)((int*)p + 2);
        stbiw__sbm(*arr) = m;
    }
    return *arr;
}

static unsigned char* stbiw__zlib_flushf(unsigned char* data, unsigned int* bitbuffer, int* bitcount)
{
    while (*bitcount >= 8) {
        stbiw__sbpush(data, STBIW_UCHAR(*bitbuffer));
        *bitbuffer >>= 8;
        *bitcount -= 8;
    }
    return data;
}

static int stbiw__zlib_bitrev(int code, int codebits)
{
    int res = 0;
    while (codebits--) {
        res = (res << 1) | (code & 1);
        code >>= 1;
    }
    return res;
}

static unsigned int stbiw__zlib_countm(unsigned char* a, unsigned char* b, int limit)
{
    int i;
    for (i = 0; i < limit && i < 258; ++i)
        if (a[i] != b[i]) break;
    return i;
}

static unsigned int stbiw__zhash(unsigned char* data)
{
    stbiw_uint32 hash = data[0] + (data[1] << 8) + (data[2] << 16);
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

#define stbiw__zlib_flush() (out = stbiw__zlib_flushf(out, &bitbuf, &bitcount))
#define stbiw__zlib_add(code,codebits) \
      (bitbuf |= (code) << bitcount, bitcount += (codebits), stbiw__zlib_flush())
#define stbiw__zlib_huffa(b,c)  stbiw__zlib_add(stbiw__zlib_bitrev(b,c),c)
// default huffman tables
#define stbiw__zlib_huff1(n)  stbiw__zlib_huffa(0x30 + (n), 8)
#define stbiw__zlib_huff2(n)  stbiw__zlib_huffa(0x190 + (n)-144, 9)
#define stbiw__zlib_huff3(n)  stbiw__zlib_huffa(0 + (n)-256,7)
#define stbiw__zlib_huff4(n)  stbiw__zlib_huffa(0xc0 + (n)-280,8)
#define stbiw__zlib_huff(n)  ((n) <= 143 ? stbiw__zlib_huff1(n) : (n) <= 255 ? stbiw__zlib_huff2(n) : (n) <= 279 ? stbiw__zlib_huff3(n) : stbiw__zlib_huff4(n))
#define stbiw__zlib_huffb(n) ((n) <= 143 ? stbiw__zlib_huff1(n) : stbiw__zlib_huff2(n))

#define stbiw__ZHASH   16384

#endif // STBIW_ZLIB_COMPRESS

STBIWDEF unsigned char* stbi_zlib_compress(unsigned char* data, int data_len, int* out_len, int quality)
{
#ifdef STBIW_ZLIB_COMPRESS
    // user provided a zlib compress implementation, use that
    return STBIW_ZLIB_COMPRESS(data, data_len, out_len, quality);
#else // use builtin
    static unsigned short lengthc[] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258, 259 };
    static unsigned char  lengtheb[] = { 0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5,  0 };
    static unsigned short distc[] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577, 32768 };
    static unsigned char  disteb[] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
    unsigned int bitbuf = 0;
    int i, j, bitcount = 0;
    unsigned char* out = NULL;
    unsigned char*** hash_table = (unsigned char***)STBIW_MALLOC(stbiw__ZHASH * sizeof(unsigned char**));
    if (hash_table == NULL)
        return NULL;
    if (quality < 5) quality = 5;

    stbiw__sbpush(out, 0x78);   // DEFLATE 32K window
    stbiw__sbpush(out, 0x5e);   // FLEVEL = 1
    stbiw__zlib_add(1, 1);  // BFINAL = 1
    stbiw__zlib_add(1, 2);  // BTYPE = 1 -- fixed huffman

    for (i = 0; i < stbiw__ZHASH; ++i)
        hash_table[i] = NULL;

    i = 0;
    while (i < data_len - 3) {
        // hash next 3 bytes of data to be compressed
        int h = stbiw__zhash(data + i) & (stbiw__ZHASH - 1), best = 3;
        unsigned char* bestloc = 0;
        unsigned char** hlist = hash_table[h];
        int n = stbiw__sbcount(hlist);
        for (j = 0; j < n; ++j) {
            if (hlist[j] - data > i - 32768) { // if entry lies within window
                int d = stbiw__zlib_countm(hlist[j], data + i, data_len - i);
                if (d >= best) { best = d; bestloc = hlist[j]; }
            }
        }
        // when hash table entry is too long, delete half the entries
        if (hash_table[h] && stbiw__sbn(hash_table[h]) == 2 * quality) {
            STBIW_MEMMOVE(hash_table[h], hash_table[h] + quality, sizeof(hash_table[h][0]) * quality);
            stbiw__sbn(hash_table[h]) = quality;
        }
        stbiw__sbpush(hash_table[h], data + i);

        if (bestloc) {
            // "lazy matching" - check match at *next* byte, and if it's better, do cur byte as literal
            h = stbiw__zhash(data + i + 1) & (stbiw__ZHASH - 1);
            hlist = hash_table[h];
            n = stbiw__sbcount(hlist);
            for (j = 0; j < n; ++j) {
                if (hlist[j] - data > i - 32767) {
                    int e = stbiw__zlib_countm(hlist[j], data + i + 1, data_len - i - 1);
                    if (e > best) { // if next match is better, bail on current match
                        bestloc = NULL;
                        break;
                    }
                }
            }
        }

        if (bestloc) {
            int d = (int)(data + i - bestloc); // distance back
            STBIW_ASSERT(d <= 32767 && best <= 258);
            for (j = 0; best > lengthc[j + 1] - 1; ++j);
            stbiw__zlib_huff(j + 257);
            if (lengtheb[j]) stbiw__zlib_add(best - lengthc[j], lengtheb[j]);
            for (j = 0; d > distc[j + 1] - 1; ++j);
            stbiw__zlib_add(stbiw__zlib_bitrev(j, 5), 5);
            if (disteb[j]) stbiw__zlib_add(d - distc[j], disteb[j]);
            i += best;
        }
        else {
            stbiw__zlib_huffb(data[i]);
            ++i;
        }
    }
    // write out final bytes
    for (; i < data_len; ++i)
        stbiw__zlib_huffb(data[i]);
    stbiw__zlib_huff(256); // end of block
    // pad with 0 bits to byte boundary
    while (bitcount)
        stbiw__zlib_add(0, 1);

    for (i = 0; i < stbiw__ZHASH; ++i)
        (void)stbiw__sbfree(hash_table[i]);
    STBIW_FREE(hash_table);

    // store uncompressed instead if compression was worse
    if (stbiw__sbn(out) > data_len + 2 + ((data_len + 32766) / 32767) * 5) {
        stbiw__sbn(out) = 2;  // truncate to DEFLATE 32K window and FLEVEL = 1
        for (j = 0; j < data_len;) {
            int blocklen = data_len - j;
            if (blocklen > 32767) blocklen = 32767;
            stbiw__sbpush(out, data_len - j == blocklen); // BFINAL = ?, BTYPE = 0 -- no compression
            stbiw__sbpush(out, STBIW_UCHAR(blocklen)); // LEN
            stbiw__sbpush(out, STBIW_UCHAR(blocklen >> 8));
            stbiw__sbpush(out, STBIW_UCHAR(~blocklen)); // NLEN
            stbiw__sbpush(out, STBIW_UCHAR(~blocklen >> 8));
            memcpy(out + stbiw__sbn(out), data + j, blocklen);
            stbiw__sbn(out) += blocklen;
            j += blocklen;
        }
    }

    {
        // compute adler32 on input
        unsigned int s1 = 1, s2 = 0;
        int blocklen = (int)(data_len % 5552);
        j = 0;
        while (j < data_len) {
            for (i = 0; i < blocklen; ++i) { s1 += data[j + i]; s2 += s1; }
            s1 %= 65521; s2 %= 65521;
            j += blocklen;
            blocklen = 5552;
        }
        stbiw__sbpush(out, STBIW_UCHAR(s2 >> 8));
        stbiw__sbpush(out, STBIW_UCHAR(s2));
        stbiw__sbpush(out, STBIW_UCHAR(s1 >> 8));
        stbiw__sbpush(out, STBIW_UCHAR(s1));
    }
    *out_len = stbiw__sbn(out);
    // make returned pointer freeable
    STBIW_MEMMOVE(stbiw__sbraw(out), out, *out_len);
    return (unsigned char*)stbiw__sbraw(out);
#endif // STBIW_ZLIB_COMPRESS
}

static unsigned int stbiw__crc32(unsigned char* buffer, int len)
{
#ifdef STBIW_CRC32
    return STBIW_CRC32(buffer, len);
#else
    static unsigned int crc_table[256] =
    {
       0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
       0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
       0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
       0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
       0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
       0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
       0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
       0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
       0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
       0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
       0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
       0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
       0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
       0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
       0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
       0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
       0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
       0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
       0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
       0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
       0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
       0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
       0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
       0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
       0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
       0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
       0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
       0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
       0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
       0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
       0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
       0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };

    unsigned int crc = ~0u;
    int i;
    for (i = 0; i < len; ++i)
        crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
    return ~crc;
#endif
}

#define stbiw__wpng4(o,a,b,c,d) ((o)[0]=STBIW_UCHAR(a),(o)[1]=STBIW_UCHAR(b),(o)[2]=STBIW_UCHAR(c),(o)[3]=STBIW_UCHAR(d),(o)+=4)
#define stbiw__wp32(data,v) stbiw__wpng4(data, (v)>>24,(v)>>16,(v)>>8,(v));
#define stbiw__wptag(data,s) stbiw__wpng4(data, s[0],s[1],s[2],s[3])

static void stbiw__wpcrc(unsigned char** data, int len)
{
    unsigned int crc = stbiw__crc32(*data - len - 4, len + 4);
    stbiw__wp32(*data, crc);
}

static unsigned char stbiw__paeth(int a, int b, int c)
{
    int p = a + b - c, pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);
    if (pa <= pb && pa <= pc) return STBIW_UCHAR(a);
    if (pb <= pc) return STBIW_UCHAR(b);
    return STBIW_UCHAR(c);
}

// @OPTIMIZE: provide an option that always forces left-predict or paeth predict
static void stbiw__encode_png_line(unsigned char* pixels, int stride_bytes, int width, int height, int y, int n, int filter_type, signed char* line_buffer)
{
    static int mapping[] = { 0,1,2,3,4 };
    static int firstmap[] = { 0,1,0,5,6 };
    int* mymap = (y != 0) ? mapping : firstmap;
    int i;
    int type = mymap[filter_type];
    unsigned char* z = pixels + stride_bytes * (stbi__flip_vertically_on_write ? height - 1 - y : y);
    int signed_stride = stbi__flip_vertically_on_write ? -stride_bytes : stride_bytes;

    if (type == 0) {
        memcpy(line_buffer, z, width * n);
        return;
    }

    // first loop isn't optimized since it's just one pixel
    for (i = 0; i < n; ++i) {
        switch (type) {
        case 1: line_buffer[i] = z[i]; break;
        case 2: line_buffer[i] = z[i] - z[i - signed_stride]; break;
        case 3: line_buffer[i] = z[i] - (z[i - signed_stride] >> 1); break;
        case 4: line_buffer[i] = (signed char)(z[i] - stbiw__paeth(0, z[i - signed_stride], 0)); break;
        case 5: line_buffer[i] = z[i]; break;
        case 6: line_buffer[i] = z[i]; break;
        }
    }
    switch (type) {
    case 1: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - z[i - n]; break;
    case 2: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - z[i - signed_stride]; break;
    case 3: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - ((z[i - n] + z[i - signed_stride]) >> 1); break;
    case 4: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i - n], z[i - signed_stride], z[i - signed_stride - n]); break;
    case 5: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - (z[i - n] >> 1); break;
    case 6: for (i = n; i < width * n; ++i) line_buffer[i] = z[i] - stbiw__paeth(z[i - n], 0, 0); break;
    }
}

STBIWDEF unsigned char* stbi_write_png_to_mem(const unsigned char* pixels, int stride_bytes, int x, int y, int n, int* out_len)
{
    int force_filter = stbi_write_force_png_filter;
    int ctype[5] = { -1, 0, 4, 2, 6 };
    unsigned char sig[8] = { 137,80,78,71,13,10,26,10 };
    unsigned char* out, * o, * filt, * zlib;
    signed char* line_buffer;
    int j, zlen;

    if (stride_bytes == 0)
        stride_bytes = x * n;

    if (force_filter >= 5) {
        force_filter = -1;
    }

    filt = (unsigned char*)STBIW_MALLOC((x * n + 1) * y); if (!filt) return 0;
    line_buffer = (signed char*)STBIW_MALLOC(x * n); if (!line_buffer) { STBIW_FREE(filt); return 0; }
    for (j = 0; j < y; ++j) {
        int filter_type;
        if (force_filter > -1) {
            filter_type = force_filter;
            stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, force_filter, line_buffer);
        }
        else { // Estimate the best filter by running through all of them:
            int best_filter = 0, best_filter_val = 0x7fffffff, est, i;
            for (filter_type = 0; filter_type < 5; filter_type++) {
                stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, filter_type, line_buffer);

                // Estimate the entropy of the line using this filter; the less, the better.
                est = 0;
                for (i = 0; i < x * n; ++i) {
                    est += abs((signed char)line_buffer[i]);
                }
                if (est < best_filter_val) {
                    best_filter_val = est;
                    best_filter = filter_type;
                }
            }
            if (filter_type != best_filter) {  // If the last iteration already got us the best filter, don't redo it
                stbiw__encode_png_line((unsigned char*)(pixels), stride_bytes, x, y, j, n, best_filter, line_buffer);
                filter_type = best_filter;
            }
        }
        // when we get here, filter_type contains the filter type, and line_buffer contains the data
        filt[j * (x * n + 1)] = (unsigned char)filter_type;
        STBIW_MEMMOVE(filt + j * (x * n + 1) + 1, line_buffer, x * n);
    }
    STBIW_FREE(line_buffer);
    zlib = stbi_zlib_compress(filt, y * (x * n + 1), &zlen, stbi_write_png_compression_level);
    STBIW_FREE(filt);
    if (!zlib) return 0;

    // each tag requires 12 bytes of overhead
    out = (unsigned char*)STBIW_MALLOC(8 + 12 + 13 + 12 + zlen + 12);
    if (!out) return 0;
    *out_len = 8 + 12 + 13 + 12 + zlen + 12;

    o = out;
    STBIW_MEMMOVE(o, sig, 8); o += 8;
    stbiw__wp32(o, 13); // header length
    stbiw__wptag(o, "IHDR");
    stbiw__wp32(o, x);
    stbiw__wp32(o, y);
    *o++ = 8;
    *o++ = STBIW_UCHAR(ctype[n]);
    *o++ = 0;
    *o++ = 0;
    *o++ = 0;
    stbiw__wpcrc(&o, 13);

    stbiw__wp32(o, zlen);
    stbiw__wptag(o, "IDAT");
    STBIW_MEMMOVE(o, zlib, zlen);
    o += zlen;
    STBIW_FREE(zlib);
    stbiw__wpcrc(&o, zlen);

    stbiw__wp32(o, 0);
    stbiw__wptag(o, "IEND");
    stbiw__wpcrc(&o, 0);

    STBIW_ASSERT(o == out + *out_len);

    return out;
}

#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_png(char const* filename, int x, int y, int comp, const void* data, int stride_bytes)
{
    FILE* f;
    int len;
    unsigned char* png = stbi_write_png_to_mem((const unsigned char*)data, stride_bytes, x, y, comp, &len);
    if (png == NULL) return 0;

    f = stbiw__fopen(filename, "wb");
    if (!f) { STBIW_FREE(png); return 0; }
    fwrite(png, 1, len, f);
    fclose(f);
    STBIW_FREE(png);
    return 1;
}
#endif

STBIWDEF int stbi_write_png_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int stride_bytes)
{
    int len;
    unsigned char* png = stbi_write_png_to_mem((const unsigned char*)data, stride_bytes, x, y, comp, &len);
    if (png == NULL) return 0;
    func(context, png, len);
    STBIW_FREE(png);
    return 1;
}


/* ***************************************************************************
 *
 * JPEG writer
 *
 * This is based on Jon Olick's jo_jpeg.cpp:
 * public domain Simple, Minimalistic JPEG writer - http://www.jonolick.com/code.html
 */

static const unsigned char stbiw__jpg_ZigZag[] = { 0,1,5,6,14,15,27,28,2,4,7,13,16,26,29,42,3,8,12,17,25,30,41,43,9,11,18,
      24,31,40,44,53,10,19,23,32,39,45,52,54,20,22,33,38,46,51,55,60,21,34,37,47,50,56,59,61,35,36,48,49,57,58,62,63 };

static void stbiw__jpg_writeBits(stbi__write_context* s, int* bitBufP, int* bitCntP, const unsigned short* bs) {
    int bitBuf = *bitBufP, bitCnt = *bitCntP;
    bitCnt += bs[1];
    bitBuf |= bs[0] << (24 - bitCnt);
    while (bitCnt >= 8) {
        unsigned char c = (bitBuf >> 16) & 255;
        stbiw__putc(s, c);
        if (c == 255) {
            stbiw__putc(s, 0);
        }
        bitBuf <<= 8;
        bitCnt -= 8;
    }
    *bitBufP = bitBuf;
    *bitCntP = bitCnt;
}

static void stbiw__jpg_DCT(float* d0p, float* d1p, float* d2p, float* d3p, float* d4p, float* d5p, float* d6p, float* d7p) {
    float d0 = *d0p, d1 = *d1p, d2 = *d2p, d3 = *d3p, d4 = *d4p, d5 = *d5p, d6 = *d6p, d7 = *d7p;
    float z1, z2, z3, z4, z5, z11, z13;

    float tmp0 = d0 + d7;
    float tmp7 = d0 - d7;
    float tmp1 = d1 + d6;
    float tmp6 = d1 - d6;
    float tmp2 = d2 + d5;
    float tmp5 = d2 - d5;
    float tmp3 = d3 + d4;
    float tmp4 = d3 - d4;

    // Even part
    float tmp10 = tmp0 + tmp3;   // phase 2
    float tmp13 = tmp0 - tmp3;
    float tmp11 = tmp1 + tmp2;
    float tmp12 = tmp1 - tmp2;

    d0 = tmp10 + tmp11;       // phase 3
    d4 = tmp10 - tmp11;

    z1 = (tmp12 + tmp13) * 0.707106781f; // c4
    d2 = tmp13 + z1;       // phase 5
    d6 = tmp13 - z1;

    // Odd part
    tmp10 = tmp4 + tmp5;       // phase 2
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    // The rotator is modified from fig 4-8 to avoid extra negations.
    z5 = (tmp10 - tmp12) * 0.382683433f; // c6
    z2 = tmp10 * 0.541196100f + z5; // c2-c6
    z4 = tmp12 * 1.306562965f + z5; // c2+c6
    z3 = tmp11 * 0.707106781f; // c4

    z11 = tmp7 + z3;      // phase 5
    z13 = tmp7 - z3;

    *d5p = z13 + z2;         // phase 6
    *d3p = z13 - z2;
    *d1p = z11 + z4;
    *d7p = z11 - z4;

    *d0p = d0;  *d2p = d2;  *d4p = d4;  *d6p = d6;
}

static void stbiw__jpg_calcBits(int val, unsigned short bits[2]) {
    int tmp1 = val < 0 ? -val : val;
    val = val < 0 ? val - 1 : val;
    bits[1] = 1;
    while (tmp1 >>= 1) {
        ++bits[1];
    }
    bits[0] = val & ((1 << bits[1]) - 1);
}

static int stbiw__jpg_processDU(stbi__write_context* s, int* bitBuf, int* bitCnt, float* CDU, int du_stride, float* fdtbl, int DC, const unsigned short HTDC[256][2], const unsigned short HTAC[256][2]) {
    const unsigned short EOB[2] = { HTAC[0x00][0], HTAC[0x00][1] };
    const unsigned short M16zeroes[2] = { HTAC[0xF0][0], HTAC[0xF0][1] };
    int dataOff, i, j, n, diff, end0pos, x, y;
    int DU[64];

    // DCT rows
    for (dataOff = 0, n = du_stride * 8; dataOff < n; dataOff += du_stride) {
        stbiw__jpg_DCT(&CDU[dataOff], &CDU[dataOff + 1], &CDU[dataOff + 2], &CDU[dataOff + 3], &CDU[dataOff + 4], &CDU[dataOff + 5], &CDU[dataOff + 6], &CDU[dataOff + 7]);
    }
    // DCT columns
    for (dataOff = 0; dataOff < 8; ++dataOff) {
        stbiw__jpg_DCT(&CDU[dataOff], &CDU[dataOff + du_stride], &CDU[dataOff + du_stride * 2], &CDU[dataOff + du_stride * 3], &CDU[dataOff + du_stride * 4],
            &CDU[dataOff + du_stride * 5], &CDU[dataOff + du_stride * 6], &CDU[dataOff + du_stride * 7]);
    }
    // Quantize/descale/zigzag the coefficients
    for (y = 0, j = 0; y < 8; ++y) {
        for (x = 0; x < 8; ++x, ++j) {
            float v;
            i = y * du_stride + x;
            v = CDU[i] * fdtbl[j];
            // DU[stbiw__jpg_ZigZag[j]] = (int)(v < 0 ? ceilf(v - 0.5f) : floorf(v + 0.5f));
            // ceilf() and floorf() are C99, not C89, but I /think/ they're not needed here anyway?
            DU[stbiw__jpg_ZigZag[j]] = (int)(v < 0 ? v - 0.5f : v + 0.5f);
        }
    }

    // Encode DC
    diff = DU[0] - DC;
    if (diff == 0) {
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTDC[0]);
    }
    else {
        unsigned short bits[2];
        stbiw__jpg_calcBits(diff, bits);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTDC[bits[1]]);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, bits);
    }
    // Encode ACs
    end0pos = 63;
    for (; (end0pos > 0) && (DU[end0pos] == 0); --end0pos) {
    }
    // end0pos = first element in reverse order !=0
    if (end0pos == 0) {
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, EOB);
        return DU[0];
    }
    for (i = 1; i <= end0pos; ++i) {
        int startpos = i;
        int nrzeroes;
        unsigned short bits[2];
        for (; DU[i] == 0 && i <= end0pos; ++i) {
        }
        nrzeroes = i - startpos;
        if (nrzeroes >= 16) {
            int lng = nrzeroes >> 4;
            int nrmarker;
            for (nrmarker = 1; nrmarker <= lng; ++nrmarker)
                stbiw__jpg_writeBits(s, bitBuf, bitCnt, M16zeroes);
            nrzeroes &= 15;
        }
        stbiw__jpg_calcBits(DU[i], bits);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, HTAC[(nrzeroes << 4) + bits[1]]);
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, bits);
    }
    if (end0pos != 63) {
        stbiw__jpg_writeBits(s, bitBuf, bitCnt, EOB);
    }
    return DU[0];
}

static int stbi_write_jpg_core(stbi__write_context* s, int width, int height, int comp, const void* data, int quality) {
    // Constants that don't pollute global namespace
    static const unsigned char std_dc_luminance_nrcodes[] = { 0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };
    static const unsigned char std_dc_luminance_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
    static const unsigned char std_ac_luminance_nrcodes[] = { 0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d };
    static const unsigned char std_ac_luminance_values[] = {
       0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
       0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
       0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
       0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
       0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
       0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
       0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
    };
    static const unsigned char std_dc_chrominance_nrcodes[] = { 0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };
    static const unsigned char std_dc_chrominance_values[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
    static const unsigned char std_ac_chrominance_nrcodes[] = { 0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77 };
    static const unsigned char std_ac_chrominance_values[] = {
       0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
       0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
       0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
       0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
       0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
       0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
       0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa
    };
    // Huffman tables
    static const unsigned short YDC_HT[256][2] = { {0,2},{2,3},{3,3},{4,3},{5,3},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9} };
    static const unsigned short UVDC_HT[256][2] = { {0,2},{1,2},{2,2},{6,3},{14,4},{30,5},{62,6},{126,7},{254,8},{510,9},{1022,10},{2046,11} };
    static const unsigned short YAC_HT[256][2] = {
       {10,4},{0,2},{1,2},{4,3},{11,4},{26,5},{120,7},{248,8},{1014,10},{65410,16},{65411,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {12,4},{27,5},{121,7},{502,9},{2038,11},{65412,16},{65413,16},{65414,16},{65415,16},{65416,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {28,5},{249,8},{1015,10},{4084,12},{65417,16},{65418,16},{65419,16},{65420,16},{65421,16},{65422,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {58,6},{503,9},{4085,12},{65423,16},{65424,16},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {59,6},{1016,10},{65430,16},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {122,7},{2039,11},{65438,16},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {123,7},{4086,12},{65446,16},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {250,8},{4087,12},{65454,16},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {504,9},{32704,15},{65462,16},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {505,9},{65470,16},{65471,16},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {506,9},{65479,16},{65480,16},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {1017,10},{65488,16},{65489,16},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {1018,10},{65497,16},{65498,16},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {2040,11},{65506,16},{65507,16},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {65515,16},{65516,16},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{0,0},{0,0},{0,0},{0,0},{0,0},
       {2041,11},{65525,16},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
    };
    static const unsigned short UVAC_HT[256][2] = {
       {0,2},{1,2},{4,3},{10,4},{24,5},{25,5},{56,6},{120,7},{500,9},{1014,10},{4084,12},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {11,4},{57,6},{246,8},{501,9},{2038,11},{4085,12},{65416,16},{65417,16},{65418,16},{65419,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {26,5},{247,8},{1015,10},{4086,12},{32706,15},{65420,16},{65421,16},{65422,16},{65423,16},{65424,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {27,5},{248,8},{1016,10},{4087,12},{65425,16},{65426,16},{65427,16},{65428,16},{65429,16},{65430,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {58,6},{502,9},{65431,16},{65432,16},{65433,16},{65434,16},{65435,16},{65436,16},{65437,16},{65438,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {59,6},{1017,10},{65439,16},{65440,16},{65441,16},{65442,16},{65443,16},{65444,16},{65445,16},{65446,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {121,7},{2039,11},{65447,16},{65448,16},{65449,16},{65450,16},{65451,16},{65452,16},{65453,16},{65454,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {122,7},{2040,11},{65455,16},{65456,16},{65457,16},{65458,16},{65459,16},{65460,16},{65461,16},{65462,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {249,8},{65463,16},{65464,16},{65465,16},{65466,16},{65467,16},{65468,16},{65469,16},{65470,16},{65471,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {503,9},{65472,16},{65473,16},{65474,16},{65475,16},{65476,16},{65477,16},{65478,16},{65479,16},{65480,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {504,9},{65481,16},{65482,16},{65483,16},{65484,16},{65485,16},{65486,16},{65487,16},{65488,16},{65489,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {505,9},{65490,16},{65491,16},{65492,16},{65493,16},{65494,16},{65495,16},{65496,16},{65497,16},{65498,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {506,9},{65499,16},{65500,16},{65501,16},{65502,16},{65503,16},{65504,16},{65505,16},{65506,16},{65507,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {2041,11},{65508,16},{65509,16},{65510,16},{65511,16},{65512,16},{65513,16},{65514,16},{65515,16},{65516,16},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
       {16352,14},{65517,16},{65518,16},{65519,16},{65520,16},{65521,16},{65522,16},{65523,16},{65524,16},{65525,16},{0,0},{0,0},{0,0},{0,0},{0,0},
       {1018,10},{32707,15},{65526,16},{65527,16},{65528,16},{65529,16},{65530,16},{65531,16},{65532,16},{65533,16},{65534,16},{0,0},{0,0},{0,0},{0,0},{0,0}
    };
    static const int YQT[] = { 16,11,10,16,24,40,51,61,12,12,14,19,26,58,60,55,14,13,16,24,40,57,69,56,14,17,22,29,51,87,80,62,18,22,
                              37,56,68,109,103,77,24,35,55,64,81,104,113,92,49,64,78,87,103,121,120,101,72,92,95,98,112,100,103,99 };
    static const int UVQT[] = { 17,18,24,47,99,99,99,99,18,21,26,66,99,99,99,99,24,26,56,99,99,99,99,99,47,66,99,99,99,99,99,99,
                               99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
    static const float aasf[] = { 1.0f * 2.828427125f, 1.387039845f * 2.828427125f, 1.306562965f * 2.828427125f, 1.175875602f * 2.828427125f,
                                  1.0f * 2.828427125f, 0.785694958f * 2.828427125f, 0.541196100f * 2.828427125f, 0.275899379f * 2.828427125f };

    int row, col, i, k, subsample;
    float fdtbl_Y[64], fdtbl_UV[64];
    unsigned char YTable[64], UVTable[64];

    if (!data || !width || !height || comp > 4 || comp < 1) {
        return 0;
    }

    quality = quality ? quality : 90;
    subsample = quality <= 90 ? 1 : 0;
    quality = quality < 1 ? 1 : quality > 100 ? 100 : quality;
    quality = quality < 50 ? 5000 / quality : 200 - quality * 2;

    for (i = 0; i < 64; ++i) {
        int uvti, yti = (YQT[i] * quality + 50) / 100;
        YTable[stbiw__jpg_ZigZag[i]] = (unsigned char)(yti < 1 ? 1 : yti > 255 ? 255 : yti);
        uvti = (UVQT[i] * quality + 50) / 100;
        UVTable[stbiw__jpg_ZigZag[i]] = (unsigned char)(uvti < 1 ? 1 : uvti > 255 ? 255 : uvti);
    }

    for (row = 0, k = 0; row < 8; ++row) {
        for (col = 0; col < 8; ++col, ++k) {
            fdtbl_Y[k] = 1 / (YTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
            fdtbl_UV[k] = 1 / (UVTable[stbiw__jpg_ZigZag[k]] * aasf[row] * aasf[col]);
        }
    }

    // Write Headers
    {
        static const unsigned char head0[] = { 0xFF,0xD8,0xFF,0xE0,0,0x10,'J','F','I','F',0,1,1,0,0,1,0,1,0,0,0xFF,0xDB,0,0x84,0 };
        static const unsigned char head2[] = { 0xFF,0xDA,0,0xC,3,1,0,2,0x11,3,0x11,0,0x3F,0 };
        const unsigned char head1[] = { 0xFF,0xC0,0,0x11,8,(unsigned char)(height >> 8),STBIW_UCHAR(height),(unsigned char)(width >> 8),STBIW_UCHAR(width),
                                        3,1,(unsigned char)(subsample ? 0x22 : 0x11),0,2,0x11,1,3,0x11,1,0xFF,0xC4,0x01,0xA2,0 };
        s->func(s->context, (void*)head0, sizeof(head0));
        s->func(s->context, (void*)YTable, sizeof(YTable));
        stbiw__putc(s, 1);
        s->func(s->context, UVTable, sizeof(UVTable));
        s->func(s->context, (void*)head1, sizeof(head1));
        s->func(s->context, (void*)(std_dc_luminance_nrcodes + 1), sizeof(std_dc_luminance_nrcodes) - 1);
        s->func(s->context, (void*)std_dc_luminance_values, sizeof(std_dc_luminance_values));
        stbiw__putc(s, 0x10); // HTYACinfo
        s->func(s->context, (void*)(std_ac_luminance_nrcodes + 1), sizeof(std_ac_luminance_nrcodes) - 1);
        s->func(s->context, (void*)std_ac_luminance_values, sizeof(std_ac_luminance_values));
        stbiw__putc(s, 1); // HTUDCinfo
        s->func(s->context, (void*)(std_dc_chrominance_nrcodes + 1), sizeof(std_dc_chrominance_nrcodes) - 1);
        s->func(s->context, (void*)std_dc_chrominance_values, sizeof(std_dc_chrominance_values));
        stbiw__putc(s, 0x11); // HTUACinfo
        s->func(s->context, (void*)(std_ac_chrominance_nrcodes + 1), sizeof(std_ac_chrominance_nrcodes) - 1);
        s->func(s->context, (void*)std_ac_chrominance_values, sizeof(std_ac_chrominance_values));
        s->func(s->context, (void*)head2, sizeof(head2));
    }

    // Encode 8x8 macroblocks
    {
        static const unsigned short fillBits[] = { 0x7F, 7 };
        int DCY = 0, DCU = 0, DCV = 0;
        int bitBuf = 0, bitCnt = 0;
        // comp == 2 is grey+alpha (alpha is ignored)
        int ofsG = comp > 2 ? 1 : 0, ofsB = comp > 2 ? 2 : 0;
        const unsigned char* dataR = (const unsigned char*)data;
        const unsigned char* dataG = dataR + ofsG;
        const unsigned char* dataB = dataR + ofsB;
        int x, y, pos;
        if (subsample) {
            for (y = 0; y < height; y += 16) {
                for (x = 0; x < width; x += 16) {
                    float Y[256], U[256], V[256];
                    for (row = y, pos = 0; row < y + 16; ++row) {
                        // row >= height => use last input row
                        int clamped_row = (row < height) ? row : height - 1;
                        int base_p = (stbi__flip_vertically_on_write ? (height - 1 - clamped_row) : clamped_row) * width * comp;
                        for (col = x; col < x + 16; ++col, ++pos) {
                            // if col >= width => use pixel from last input column
                            int p = base_p + ((col < width) ? col : (width - 1)) * comp;
                            float r = dataR[p], g = dataG[p], b = dataB[p];
                            Y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
                            U[pos] = -0.16874f * r - 0.33126f * g + 0.50000f * b;
                            V[pos] = +0.50000f * r - 0.41869f * g - 0.08131f * b;
                        }
                    }
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 0, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 8, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 128, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y + 136, 16, fdtbl_Y, DCY, YDC_HT, YAC_HT);

                    // subsample U,V
                    {
                        float subU[64], subV[64];
                        int yy, xx;
                        for (yy = 0, pos = 0; yy < 8; ++yy) {
                            for (xx = 0; xx < 8; ++xx, ++pos) {
                                int j = yy * 32 + xx * 2;
                                subU[pos] = (U[j + 0] + U[j + 1] + U[j + 16] + U[j + 17]) * 0.25f;
                                subV[pos] = (V[j + 0] + V[j + 1] + V[j + 16] + V[j + 17]) * 0.25f;
                            }
                        }
                        DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, subU, 8, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
                        DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, subV, 8, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
                    }
                }
            }
        }
        else {
            for (y = 0; y < height; y += 8) {
                for (x = 0; x < width; x += 8) {
                    float Y[64], U[64], V[64];
                    for (row = y, pos = 0; row < y + 8; ++row) {
                        // row >= height => use last input row
                        int clamped_row = (row < height) ? row : height - 1;
                        int base_p = (stbi__flip_vertically_on_write ? (height - 1 - clamped_row) : clamped_row) * width * comp;
                        for (col = x; col < x + 8; ++col, ++pos) {
                            // if col >= width => use pixel from last input column
                            int p = base_p + ((col < width) ? col : (width - 1)) * comp;
                            float r = dataR[p], g = dataG[p], b = dataB[p];
                            Y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
                            U[pos] = -0.16874f * r - 0.33126f * g + 0.50000f * b;
                            V[pos] = +0.50000f * r - 0.41869f * g - 0.08131f * b;
                        }
                    }

                    DCY = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, Y, 8, fdtbl_Y, DCY, YDC_HT, YAC_HT);
                    DCU = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, U, 8, fdtbl_UV, DCU, UVDC_HT, UVAC_HT);
                    DCV = stbiw__jpg_processDU(s, &bitBuf, &bitCnt, V, 8, fdtbl_UV, DCV, UVDC_HT, UVAC_HT);
                }
            }
        }

        // Do the bit alignment of the EOI marker
        stbiw__jpg_writeBits(s, &bitBuf, &bitCnt, fillBits);
    }

    // EOI
    stbiw__putc(s, 0xFF);
    stbiw__putc(s, 0xD9);

    return 1;
}

STBIWDEF int stbi_write_jpg_to_func(stbi_write_func* func, void* context, int x, int y, int comp, const void* data, int quality)
{
    stbi__write_context s = { 0 };
    stbi__start_write_callbacks(&s, func, context);
    return stbi_write_jpg_core(&s, x, y, comp, (void*)data, quality);
}


#ifndef STBI_WRITE_NO_STDIO
STBIWDEF int stbi_write_jpg(char const* filename, int x, int y, int comp, const void* data, int quality)
{
    stbi__write_context s = { 0 };
    if (stbi__start_write_file(&s, filename)) {
        int r = stbi_write_jpg_core(&s, x, y, comp, data, quality);
        stbi__end_write_file(&s);
        return r;
    }
    else
        return 0;
}
#endif

#endif // STB_IMAGE_WRITE_IMPLEMENTATION

/* Revision history
      1.16  (2021-07-11)
             make Deflate code emit uncompressed blocks when it would otherwise expand
             support writing BMPs with alpha channel
      1.15  (2020-07-13) unknown
      1.14  (2020-02-02) updated JPEG writer to downsample chroma channels
      1.13
      1.12
      1.11  (2019-08-11)

      1.10  (2019-02-07)
             support utf8 filenames in Windows; fix warnings and platform ifdefs
      1.09  (2018-02-11)
             fix typo in zlib quality API, improve STB_I_W_STATIC in C++
      1.08  (2018-01-29)
             add stbi__flip_vertically_on_write, external zlib, zlib quality, choose PNG filter
      1.07  (2017-07-24)
             doc fix
      1.06 (2017-07-23)
             writing JPEG (using Jon Olick's code)
      1.05   ???
      1.04 (2017-03-03)
             monochrome BMP expansion
      1.03   ???
      1.02 (2016-04-02)
             avoid allocating large structures on the stack
      1.01 (2016-01-16)
             STBIW_REALLOC_SIZED: support allocators with no realloc support
             avoid race-condition in crc initialization
             minor compile issues
      1.00 (2015-09-14)
             installable file IO function
      0.99 (2015-09-13)
             warning fixes; TGA rle support
      0.98 (2015-04-08)
             added STBIW_MALLOC, STBIW_ASSERT etc
      0.97 (2015-01-18)
             fixed HDR asserts, rewrote HDR rle logic
      0.96 (2015-01-17)
             add HDR output
             fix monochrome BMP
      0.95 (2014-08-17)
             add monochrome TGA output
      0.94 (2014-05-31)
             rename private functions to avoid conflicts with stb_image.h
      0.93 (2014-05-27)
             warning fixes
      0.92 (2010-08-01)
             casts to unsigned char to fix warnings
      0.91 (2010-07-17)
             first public release
      0.90   first internal release
*/

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

// used both to compute SDF and in 'shader'
float sdf_size = 50.0;          // the larger this is, the better large font sizes look
float pixel_dist_scale = 64.0;  // trades off precision w/ ability to handle *smaller* sizes
int onedge_value = 128;
int padding = 3; // not used in shader

typedef struct
{
	float advance;
	signed char xoff;
	signed char yoff;
	unsigned char w, h;
	unsigned char* data;
} fontchar;

fontchar fdata[128];

#define BITMAP_W  1200
#define BITMAP_H  800
unsigned char bitmap[BITMAP_H][BITMAP_W][3];

char* sample = "This is goofy text, size %d!";
char* small_sample = "This is goofy text, size %d! Really needs in-shader supersampling to look good.";

void blend_pixel(int x, int y, int color, float alpha)
{
	int i;
	for (i = 0; i < 3; ++i)
		bitmap[y][x][i] = (unsigned char)(stb_lerp(alpha, bitmap[y][x][i], color) + 0.5); // round
}

void draw_char(float px, float py, char c, float relative_scale)
{
	int x, y;
	fontchar* fc = &fdata[c];
	float fx0 = px + fc->xoff * relative_scale;
	float fy0 = py + fc->yoff * relative_scale;
	float fx1 = fx0 + fc->w * relative_scale;
	float fy1 = fy0 + fc->h * relative_scale;
	int ix0 = (int)floor(fx0);
	int iy0 = (int)floor(fy0);
	int ix1 = (int)ceil(fx1);
	int iy1 = (int)ceil(fy1);
	// clamp to viewport
	if (ix0 < 0) ix0 = 0;
	if (iy0 < 0) iy0 = 0;
	if (ix1 > BITMAP_W) ix1 = BITMAP_W;
	if (iy1 > BITMAP_H) iy1 = BITMAP_H;

	for (y = iy0; y < iy1; ++y) {
		for (x = ix0; x < ix1; ++x) {
			float sdf_dist, pix_dist;
			float bmx = stb_linear_remap(x, fx0, fx1, 0, fc->w);
			float bmy = stb_linear_remap(y, fy0, fy1, 0, fc->h);
			int v00, v01, v10, v11;
			float v0, v1, v;
			int sx0 = (int)bmx;
			int sx1 = sx0 + 1;
			int sy0 = (int)bmy;
			int sy1 = sy0 + 1;
			// compute lerp weights
			bmx = bmx - sx0;
			bmy = bmy - sy0;
			// clamp to edge
			sx0 = stb_clamp(sx0, 0, fc->w - 1);
			sx1 = stb_clamp(sx1, 0, fc->w - 1);
			sy0 = stb_clamp(sy0, 0, fc->h - 1);
			sy1 = stb_clamp(sy1, 0, fc->h - 1);
			// bilinear texture sample
			v00 = fc->data[sy0 * fc->w + sx0];
			v01 = fc->data[sy0 * fc->w + sx1];
			v10 = fc->data[sy1 * fc->w + sx0];
			v11 = fc->data[sy1 * fc->w + sx1];
			v0 = stb_lerp(bmx, v00, v01);
			v1 = stb_lerp(bmx, v10, v11);
			v = stb_lerp(bmy, v0, v1);
#if 0
			// non-anti-aliased
			if (v > onedge_value)
				blend_pixel(x, y, 0, 1.0);
#else
			// Following math can be greatly simplified

			// convert distance in SDF value to distance in SDF bitmap
			sdf_dist = stb_linear_remap(v, onedge_value, onedge_value + pixel_dist_scale, 0, 1);
			// convert distance in SDF bitmap to distance in output bitmap
			pix_dist = sdf_dist * relative_scale;
			// anti-alias by mapping 1/2 pixel around contour from 0..1 alpha
			v = stb_linear_remap(pix_dist, -0.5f, 0.5f, 0, 1);
			if (v > 1) v = 1;
			if (v > 0)
				blend_pixel(x, y, 0, v);
#endif
		}
	}
}

void print_text(float x, float y, char* text, float scale)
{
	int i;
	for (i = 0; text[i]; ++i) {
		if (fdata[text[i]].data)
			draw_char(x, y, text[i], scale);
		x += fdata[text[i]].advance * scale;
	}
}

#include "graphics/font/Truetype.h"
#include "core/Assert.h"

int main(int argc, char** argv)
{
	int ch;
	float scale, ypos;
	stbtt_fontinfo font;
	
    std::string filename = "C:/Users/koma/Desktop/CascadiaMono.ttf";
    
    void* data = stb_file((char*)filename.c_str(), NULL);
    stbtt_InitFont(&font, (const unsigned char*) data, 0);

    monk::Truetype tt((uint8_t*)data);

    scale = stbtt_ScaleForPixelHeight(&font, sdf_size);
    
    int w, h, xoff, yoff;
    auto stbData = stbtt_GetCodepointSDF(&font, scale, 'K', padding, onedge_value, pixel_dist_scale, &w, &h, &xoff, &yoff);

    for (int c = 32; c < 127; c++)
    {
		stbtt_vertex* stbVerts;
		int stbGlyph = stbtt_FindGlyphIndex(&font, c);
		int stbLen = stbtt_GetGlyphShape(&font, stbGlyph, &stbVerts);

        monk::GlyphVertex* myVerts;
        int myGlyph = tt.FindGlyphIndex(c);
        int myLen = tt.GetGlyphShape(myGlyph, &myVerts);

        LOG_INFO("glyph: {0}, {1}", stbGlyph, myGlyph);
        LOG_INFO("len: {0}, {1}", stbLen, myLen);

        MONK_ASSERT(myGlyph == stbGlyph);
        MONK_ASSERT(myLen == stbLen);

        for (int i = 0; i < myLen; i++)
        {
            monk::GlyphVertex my = myVerts[i];
            stbtt_vertex stb = stbVerts[i];

            MONK_ASSERT(my.x == stb.x);
            MONK_ASSERT(my.y == stb.y);
            MONK_ASSERT(my.cx == stb.cx);
            MONK_ASSERT(my.cy == stb.cy);
			MONK_ASSERT(my.cx1 == stb.cx1);
			MONK_ASSERT(my.cy1 == stb.cy1);
			MONK_ASSERT(my.type == stb.type);
			MONK_ASSERT(my.padding == stb.padding);
        }

        LOG_INFO("========================");
    }

	for (ch = 32; ch < 127; ++ch) {
		fontchar fc;
		int xoff, yoff, w, h, advance;
		fc.data = stbtt_GetCodepointSDF(&font, scale, ch, padding, onedge_value, pixel_dist_scale, &w, &h, &xoff, &yoff);
		fc.xoff = xoff;
		fc.yoff = yoff;
		fc.w = w;
		fc.h = h;
		stbtt_GetCodepointHMetrics(&font, ch, &advance, NULL);
		fc.advance = advance * scale;
		fdata[ch] = fc;
	}

	ypos = 60;
	memset(bitmap, 255, sizeof(bitmap));
	print_text(400, ypos + 30, stb_sprintf("sdf bitmap height %d", (int)sdf_size), 30 / sdf_size);
	ypos += 80;
	for (scale = 8.0; scale < 120.0; scale *= 1.33f) {
		print_text(80, ypos + scale, stb_sprintf(scale == 8.0 ? small_sample : sample, (int)scale), scale / sdf_size);
		ypos += scale * 1.05f + 20;
	}

	stbi_write_png("sdf_test.png", BITMAP_W, BITMAP_H, 3, bitmap, 0);
	return 0;
}
#endif
