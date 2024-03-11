#include "mogl.h"

#include <windows.h>

#pragma region OpenGL 1.0 functions
PFNGLCULLFACEPROC _glCullFace = nullptr;
PFNGLFRONTFACEPROC _glFrontFace = nullptr;
PFNGLHINTPROC _glHint = nullptr;
PFNGLLINEWIDTHPROC _glLineWidth = nullptr;
PFNGLPOINTSIZEPROC _glPointSize = nullptr;
PFNGLPOLYGONMODEPROC _glPolygonMode = nullptr;
PFNGLSCISSORPROC _glScissor = nullptr;
PFNGLTEXPARAMETERFPROC _glTexParameterf = nullptr;
PFNGLTEXPARAMETERFVPROC _glTexParameterfv = nullptr;
PFNGLTEXPARAMETERIPROC _glTexParameteri = nullptr;
PFNGLTEXPARAMETERIVPROC _glTexParameteriv = nullptr;
PFNGLTEXIMAGE1DPROC _glTexImage1D = nullptr;
PFNGLTEXIMAGE2DPROC _glTexImage2D = nullptr;
PFNGLDRAWBUFFERPROC _glDrawBuffer = nullptr;
PFNGLCLEARPROC _glClear = nullptr;
PFNGLCLEARCOLORPROC _glClearColor = nullptr;
PFNGLCLEARSTENCILPROC _glClearStencil = nullptr;
PFNGLCLEARDEPTHPROC _glClearDepth = nullptr;
PFNGLSTENCILMASKPROC _glStencilMask = nullptr;
PFNGLCOLORMASKPROC _glColorMask = nullptr;
PFNGLDEPTHMASKPROC _glDepthMask = nullptr;
PFNGLDISABLEPROC _glDisable = nullptr;
PFNGLENABLEPROC _glEnable = nullptr;
PFNGLFINISHPROC _glFinish = nullptr;
PFNGLFLUSHPROC _glFlush = nullptr;
PFNGLBLENDFUNCPROC _glBlendFunc = nullptr;
PFNGLLOGICOPPROC _glLogicOp = nullptr;
PFNGLSTENCILFUNCPROC _glStencilFunc = nullptr;
PFNGLSTENCILOPPROC _glStencilOp = nullptr;
PFNGLDEPTHFUNCPROC _glDepthFunc = nullptr;
PFNGLPIXELSTOREFPROC _glPixelStoref = nullptr;
PFNGLPIXELSTOREIPROC _glPixelStorei = nullptr;
PFNGLREADBUFFERPROC _glReadBuffer = nullptr;
PFNGLREADPIXELSPROC _glReadPixels = nullptr;
PFNGLGETBOOLEANVPROC _glGetBooleanv = nullptr;
PFNGLGETDOUBLEVPROC _glGetDoublev = nullptr;
PFNGLGETERRORPROC _glGetError = nullptr;
PFNGLGETFLOATVPROC _glGetFloatv = nullptr;
PFNGLGETINTEGERVPROC _glGetIntegerv = nullptr;
PFNGLGETSTRINGPROC _glGetString = nullptr;
PFNGLGETTEXIMAGEPROC _glGetTexImage = nullptr;
PFNGLGETTEXPARAMETERFVPROC _glGetTexParameterfv = nullptr;
PFNGLGETTEXPARAMETERIVPROC _glGetTexParameteriv = nullptr;
PFNGLGETTEXLEVELPARAMETERFVPROC _glGetTexLevelParameterfv = nullptr;
PFNGLGETTEXLEVELPARAMETERIVPROC _glGetTexLevelParameteriv = nullptr;
PFNGLISENABLEDPROC _glIsEnabled = nullptr;
PFNGLDEPTHRANGEPROC _glDepthRange = nullptr;
PFNGLVIEWPORTPROC _glViewport = nullptr;
#pragma endregion

#pragma region OpenGL 1.1 functions
PFNGLDRAWARRAYSPROC _glDrawArrays = nullptr;
PFNGLDRAWELEMENTSPROC _glDrawElements = nullptr;
PFNGLPOLYGONOFFSETPROC _glPolygonOffset = nullptr;
PFNGLCOPYTEXIMAGE1DPROC _glCopyTexImage1D = nullptr;
PFNGLCOPYTEXIMAGE2DPROC _glCopyTexImage2D = nullptr;
PFNGLCOPYTEXSUBIMAGE1DPROC _glCopyTexSubImage1D = nullptr;
PFNGLCOPYTEXSUBIMAGE2DPROC _glCopyTexSubImage2D = nullptr;
PFNGLTEXSUBIMAGE1DPROC _glTexSubImage1D = nullptr;
PFNGLTEXSUBIMAGE2DPROC _glTexSubImage2D = nullptr;
PFNGLBINDTEXTUREPROC _glBindTexture = nullptr;
PFNGLDELETETEXTURESPROC _glDeleteTextures = nullptr;
PFNGLGENTEXTURESPROC _glGenTextures = nullptr;
PFNGLISTEXTUREPROC _glIsTexture = nullptr;
#pragma endregion

#pragma region OpenGL 1.2 functions
PFNGLDRAWRANGEELEMENTSPROC _glDrawRangeElements = nullptr;
PFNGLTEXIMAGE3DPROC _glTexImage3D = nullptr;
PFNGLTEXSUBIMAGE3DPROC _glTexSubImage3D = nullptr;
PFNGLCOPYTEXSUBIMAGE3DPROC _glCopyTexSubImage3D = nullptr;
#pragma endregion

#pragma region OpenGL 1.3 functions
PFNGLACTIVETEXTUREPROC _glActiveTexture = nullptr;
PFNGLSAMPLECOVERAGEPROC _glSampleCoverage = nullptr;
PFNGLCOMPRESSEDTEXIMAGE3DPROC _glCompressedTexImage3D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE2DPROC _glCompressedTexImage2D = nullptr;
PFNGLCOMPRESSEDTEXIMAGE1DPROC _glCompressedTexImage1D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC _glCompressedTexSubImage3D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC _glCompressedTexSubImage2D = nullptr;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC _glCompressedTexSubImage1D = nullptr;
PFNGLGETCOMPRESSEDTEXIMAGEPROC _glGetCompressedTexImage = nullptr;
#pragma endregion

#pragma region OpenGL 1.4 functions
PFNGLBLENDFUNCSEPARATEPROC _glBlendFuncSeparate = nullptr;
PFNGLMULTIDRAWARRAYSPROC _glMultiDrawArrays = nullptr;
PFNGLMULTIDRAWELEMENTSPROC _glMultiDrawElements = nullptr;
PFNGLPOINTPARAMETERFPROC _glPointParameterf = nullptr;
PFNGLPOINTPARAMETERFVPROC _glPointParameterfv = nullptr;
PFNGLPOINTPARAMETERIPROC _glPointParameteri = nullptr;
PFNGLPOINTPARAMETERIVPROC _glPointParameteriv = nullptr;
PFNGLBLENDCOLORPROC _glBlendColor = nullptr;
PFNGLBLENDEQUATIONPROC _glBlendEquation = nullptr;
#pragma endregion

#pragma region OpenGL 1.5 functions
PFNGLGENQUERIESPROC _glGenQueries = nullptr;
PFNGLDELETEQUERIESPROC _glDeleteQueries = nullptr;
PFNGLISQUERYPROC _glIsQuery = nullptr;
PFNGLBEGINQUERYPROC _glBeginQuery = nullptr;
PFNGLENDQUERYPROC _glEndQuery = nullptr;
PFNGLGETQUERYIVPROC _glGetQueryiv = nullptr;
PFNGLGETQUERYOBJECTIVPROC _glGetQueryObjectiv = nullptr;
PFNGLGETQUERYOBJECTUIVPROC _glGetQueryObjectuiv = nullptr;
PFNGLBINDBUFFERPROC _glBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC _glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC _glGenBuffers = nullptr;
PFNGLISBUFFERPROC _glIsBuffer = nullptr;
PFNGLBUFFERDATAPROC _glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC _glBufferSubData = nullptr;
PFNGLGETBUFFERSUBDATAPROC _glGetBufferSubData = nullptr;
PFNGLMAPBUFFERPROC _glMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC _glUnmapBuffer = nullptr;
PFNGLGETBUFFERPARAMETERIVPROC _glGetBufferParameteriv = nullptr;
PFNGLGETBUFFERPOINTERVPROC _glGetBufferPointerv = nullptr;
#pragma endregion

#pragma region OpenGL 2.0 functions
PFNGLBLENDEQUATIONSEPARATEPROC _glBlendEquationSeparate = nullptr;
PFNGLDRAWBUFFERSPROC _glDrawBuffers = nullptr;
PFNGLSTENCILOPSEPARATEPROC _glStencilOpSeparate = nullptr;
PFNGLSTENCILFUNCSEPARATEPROC _glStencilFuncSeparate = nullptr;
PFNGLSTENCILMASKSEPARATEPROC _glStencilMaskSeparate = nullptr;
PFNGLATTACHSHADERPROC _glAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC _glBindAttribLocation = nullptr;
PFNGLCOMPILESHADERPROC _glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC _glCreateProgram = nullptr;
PFNGLCREATESHADERPROC _glCreateShader = nullptr;
PFNGLDELETEPROGRAMPROC _glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC _glDeleteShader = nullptr;
PFNGLDETACHSHADERPROC _glDetachShader = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC _glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC _glEnableVertexAttribArray = nullptr;
PFNGLGETACTIVEATTRIBPROC _glGetActiveAttrib = nullptr;
PFNGLGETACTIVEUNIFORMPROC _glGetActiveUniform = nullptr;
PFNGLGETATTACHEDSHADERSPROC _glGetAttachedShaders = nullptr;
PFNGLGETATTRIBLOCATIONPROC _glGetAttribLocation = nullptr;
PFNGLGETPROGRAMIVPROC _glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC _glGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC _glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC _glGetShaderInfoLog = nullptr;
PFNGLGETSHADERSOURCEPROC _glGetShaderSource = nullptr;
PFNGLGETUNIFORMLOCATIONPROC _glGetUniformLocation = nullptr;
PFNGLGETUNIFORMFVPROC _glGetUniformfv = nullptr;
PFNGLGETUNIFORMIVPROC _glGetUniformiv = nullptr;
PFNGLGETVERTEXATTRIBDVPROC _glGetVertexAttribdv = nullptr;
PFNGLGETVERTEXATTRIBFVPROC _glGetVertexAttribfv = nullptr;
PFNGLGETVERTEXATTRIBIVPROC _glGetVertexAttribiv = nullptr;
PFNGLGETVERTEXATTRIBPOINTERVPROC _glGetVertexAttribPointerv = nullptr;
PFNGLISPROGRAMPROC _glIsProgram = nullptr;
PFNGLISSHADERPROC _glIsShader = nullptr;
PFNGLLINKPROGRAMPROC _glLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC _glShaderSource = nullptr;
PFNGLUSEPROGRAMPROC _glUseProgram = nullptr;
PFNGLUNIFORM1FPROC _glUniform1f = nullptr;
PFNGLUNIFORM2FPROC _glUniform2f = nullptr;
PFNGLUNIFORM3FPROC _glUniform3f = nullptr;
PFNGLUNIFORM4FPROC _glUniform4f = nullptr;
PFNGLUNIFORM1IPROC _glUniform1i = nullptr;
PFNGLUNIFORM2IPROC _glUniform2i = nullptr;
PFNGLUNIFORM3IPROC _glUniform3i = nullptr;
PFNGLUNIFORM4IPROC _glUniform4i = nullptr;
PFNGLUNIFORM1FVPROC _glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC _glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC _glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC _glUniform4fv = nullptr;
PFNGLUNIFORM1IVPROC _glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC _glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC _glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC _glUniform4iv = nullptr;
PFNGLUNIFORMMATRIX2FVPROC _glUniformMatrix2fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC _glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC _glUniformMatrix4fv = nullptr;
PFNGLVALIDATEPROGRAMPROC _glValidateProgram = nullptr;
PFNGLVERTEXATTRIB1DPROC _glVertexAttrib1d = nullptr;
PFNGLVERTEXATTRIB1DVPROC _glVertexAttrib1dv = nullptr;
PFNGLVERTEXATTRIB1FPROC _glVertexAttrib1f = nullptr;
PFNGLVERTEXATTRIB1FVPROC _glVertexAttrib1fv = nullptr;
PFNGLVERTEXATTRIB1SPROC _glVertexAttrib1s = nullptr;
PFNGLVERTEXATTRIB1SVPROC _glVertexAttrib1sv = nullptr;
PFNGLVERTEXATTRIB2DPROC _glVertexAttrib2d = nullptr;
PFNGLVERTEXATTRIB2DVPROC _glVertexAttrib2dv = nullptr;
PFNGLVERTEXATTRIB2FPROC _glVertexAttrib2f = nullptr;
PFNGLVERTEXATTRIB2FVPROC _glVertexAttrib2fv = nullptr;
PFNGLVERTEXATTRIB2SPROC _glVertexAttrib2s = nullptr;
PFNGLVERTEXATTRIB2SVPROC _glVertexAttrib2sv = nullptr;
PFNGLVERTEXATTRIB3DPROC _glVertexAttrib3d = nullptr;
PFNGLVERTEXATTRIB3DVPROC _glVertexAttrib3dv = nullptr;
PFNGLVERTEXATTRIB3FPROC _glVertexAttrib3f = nullptr;
PFNGLVERTEXATTRIB3FVPROC _glVertexAttrib3fv = nullptr;
PFNGLVERTEXATTRIB3SPROC _glVertexAttrib3s = nullptr;
PFNGLVERTEXATTRIB3SVPROC _glVertexAttrib3sv = nullptr;
PFNGLVERTEXATTRIB4NBVPROC _glVertexAttrib4Nbv = nullptr;
PFNGLVERTEXATTRIB4NIVPROC _glVertexAttrib4Niv = nullptr;
PFNGLVERTEXATTRIB4NSVPROC _glVertexAttrib4Nsv = nullptr;
PFNGLVERTEXATTRIB4NUBPROC _glVertexAttrib4Nub = nullptr;
PFNGLVERTEXATTRIB4NUBVPROC _glVertexAttrib4Nubv = nullptr;
PFNGLVERTEXATTRIB4NUIVPROC _glVertexAttrib4Nuiv = nullptr;
PFNGLVERTEXATTRIB4NUSVPROC _glVertexAttrib4Nusv = nullptr;
PFNGLVERTEXATTRIB4BVPROC _glVertexAttrib4bv = nullptr;
PFNGLVERTEXATTRIB4DPROC _glVertexAttrib4d = nullptr;
PFNGLVERTEXATTRIB4DVPROC _glVertexAttrib4dv = nullptr;
PFNGLVERTEXATTRIB4FPROC _glVertexAttrib4f = nullptr;
PFNGLVERTEXATTRIB4FVPROC _glVertexAttrib4fv = nullptr;
PFNGLVERTEXATTRIB4IVPROC _glVertexAttrib4iv = nullptr;
PFNGLVERTEXATTRIB4SPROC _glVertexAttrib4s = nullptr;
PFNGLVERTEXATTRIB4SVPROC _glVertexAttrib4sv = nullptr;
PFNGLVERTEXATTRIB4UBVPROC _glVertexAttrib4ubv = nullptr;
PFNGLVERTEXATTRIB4UIVPROC _glVertexAttrib4uiv = nullptr;
PFNGLVERTEXATTRIB4USVPROC _glVertexAttrib4usv = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC _glVertexAttribPointer = nullptr;
#pragma endregion

#pragma region OpenGL 2.1 functions
PFNGLUNIFORMMATRIX2X3FVPROC _glUniformMatrix2x3fv = nullptr;
PFNGLUNIFORMMATRIX3X2FVPROC _glUniformMatrix3x2fv = nullptr;
PFNGLUNIFORMMATRIX2X4FVPROC _glUniformMatrix2x4fv = nullptr;
PFNGLUNIFORMMATRIX4X2FVPROC _glUniformMatrix4x2fv = nullptr;
PFNGLUNIFORMMATRIX3X4FVPROC _glUniformMatrix3x4fv = nullptr;
PFNGLUNIFORMMATRIX4X3FVPROC _glUniformMatrix4x3fv = nullptr;
#pragma endregion

#pragma region OpenGL 3.0 functions
PFNGLCOLORMASKIPROC _glColorMaski = nullptr;
PFNGLGETBOOLEANI_VPROC _glGetBooleani_v = nullptr;
PFNGLGETINTEGERI_VPROC _glGetIntegeri_v = nullptr;
PFNGLENABLEIPROC _glEnablei = nullptr;
PFNGLDISABLEIPROC _glDisablei = nullptr;
PFNGLISENABLEDIPROC _glIsEnabledi = nullptr;
PFNGLBEGINTRANSFORMFEEDBACKPROC _glBeginTransformFeedback = nullptr;
PFNGLENDTRANSFORMFEEDBACKPROC _glEndTransformFeedback = nullptr;
PFNGLBINDBUFFERRANGEPROC _glBindBufferRange = nullptr;
PFNGLBINDBUFFERBASEPROC _glBindBufferBase = nullptr;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC _glTransformFeedbackVaryings = nullptr;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC _glGetTransformFeedbackVarying = nullptr;
PFNGLCLAMPCOLORPROC _glClampColor = nullptr;
PFNGLBEGINCONDITIONALRENDERPROC _glBeginConditionalRender = nullptr;
PFNGLENDCONDITIONALRENDERPROC _glEndConditionalRender = nullptr;
PFNGLVERTEXATTRIBIPOINTERPROC _glVertexAttribIPointer = nullptr;
PFNGLGETVERTEXATTRIBIIVPROC _glGetVertexAttribIiv = nullptr;
PFNGLGETVERTEXATTRIBIUIVPROC _glGetVertexAttribIuiv = nullptr;
PFNGLVERTEXATTRIBI1IPROC _glVertexAttribI1i = nullptr;
PFNGLVERTEXATTRIBI2IPROC _glVertexAttribI2i = nullptr;
PFNGLVERTEXATTRIBI3IPROC _glVertexAttribI3i = nullptr;
PFNGLVERTEXATTRIBI4IPROC _glVertexAttribI4i = nullptr;
PFNGLVERTEXATTRIBI1UIPROC _glVertexAttribI1ui = nullptr;
PFNGLVERTEXATTRIBI2UIPROC _glVertexAttribI2ui = nullptr;
PFNGLVERTEXATTRIBI3UIPROC _glVertexAttribI3ui = nullptr;
PFNGLVERTEXATTRIBI4UIPROC _glVertexAttribI4ui = nullptr;
PFNGLVERTEXATTRIBI1IVPROC _glVertexAttribI1iv = nullptr;
PFNGLVERTEXATTRIBI2IVPROC _glVertexAttribI2iv = nullptr;
PFNGLVERTEXATTRIBI3IVPROC _glVertexAttribI3iv = nullptr;
PFNGLVERTEXATTRIBI4IVPROC _glVertexAttribI4iv = nullptr;
PFNGLVERTEXATTRIBI1UIVPROC _glVertexAttribI1uiv = nullptr;
PFNGLVERTEXATTRIBI2UIVPROC _glVertexAttribI2uiv = nullptr;
PFNGLVERTEXATTRIBI3UIVPROC _glVertexAttribI3uiv = nullptr;
PFNGLVERTEXATTRIBI4UIVPROC _glVertexAttribI4uiv = nullptr;
PFNGLVERTEXATTRIBI4BVPROC _glVertexAttribI4bv = nullptr;
PFNGLVERTEXATTRIBI4SVPROC _glVertexAttribI4sv = nullptr;
PFNGLVERTEXATTRIBI4UBVPROC _glVertexAttribI4ubv = nullptr;
PFNGLVERTEXATTRIBI4USVPROC _glVertexAttribI4usv = nullptr;
PFNGLGETUNIFORMUIVPROC _glGetUniformuiv = nullptr;
PFNGLBINDFRAGDATALOCATIONPROC _glBindFragDataLocation = nullptr;
PFNGLGETFRAGDATALOCATIONPROC _glGetFragDataLocation = nullptr;
PFNGLUNIFORM1UIPROC _glUniform1ui = nullptr;
PFNGLUNIFORM2UIPROC _glUniform2ui = nullptr;
PFNGLUNIFORM3UIPROC _glUniform3ui = nullptr;
PFNGLUNIFORM4UIPROC _glUniform4ui = nullptr;
PFNGLUNIFORM1UIVPROC _glUniform1uiv = nullptr;
PFNGLUNIFORM2UIVPROC _glUniform2uiv = nullptr;
PFNGLUNIFORM3UIVPROC _glUniform3uiv = nullptr;
PFNGLUNIFORM4UIVPROC _glUniform4uiv = nullptr;
PFNGLTEXPARAMETERIIVPROC _glTexParameterIiv = nullptr;
PFNGLTEXPARAMETERIUIVPROC _glTexParameterIuiv = nullptr;
PFNGLGETTEXPARAMETERIIVPROC _glGetTexParameterIiv = nullptr;
PFNGLGETTEXPARAMETERIUIVPROC _glGetTexParameterIuiv = nullptr;
PFNGLCLEARBUFFERIVPROC _glClearBufferiv = nullptr;
PFNGLCLEARBUFFERUIVPROC _glClearBufferuiv = nullptr;
PFNGLCLEARBUFFERFVPROC _glClearBufferfv = nullptr;
PFNGLCLEARBUFFERFIPROC _glClearBufferfi = nullptr;
PFNGLGETSTRINGIPROC _glGetStringi = nullptr;
PFNGLISRENDERBUFFERPROC _glIsRenderbuffer = nullptr;
PFNGLBINDRENDERBUFFERPROC _glBindRenderbuffer = nullptr;
PFNGLDELETERENDERBUFFERSPROC _glDeleteRenderbuffers = nullptr;
PFNGLGENRENDERBUFFERSPROC _glGenRenderbuffers = nullptr;
PFNGLRENDERBUFFERSTORAGEPROC _glRenderbufferStorage = nullptr;
PFNGLGETRENDERBUFFERPARAMETERIVPROC _glGetRenderbufferParameteriv = nullptr;
PFNGLISFRAMEBUFFERPROC _glIsFramebuffer = nullptr;
PFNGLBINDFRAMEBUFFERPROC _glBindFramebuffer = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC _glDeleteFramebuffers = nullptr;
PFNGLGENFRAMEBUFFERSPROC _glGenFramebuffers = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC _glCheckFramebufferStatus = nullptr;
PFNGLFRAMEBUFFERTEXTURE1DPROC _glFramebufferTexture1D = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC _glFramebufferTexture2D = nullptr;
PFNGLFRAMEBUFFERTEXTURE3DPROC _glFramebufferTexture3D = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC _glFramebufferRenderbuffer = nullptr;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC _glGetFramebufferAttachmentParameteriv = nullptr;
PFNGLGENERATEMIPMAPPROC _glGenerateMipmap = nullptr;
PFNGLBLITFRAMEBUFFERPROC _glBlitFramebuffer = nullptr;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC _glRenderbufferStorageMultisample = nullptr;
PFNGLFRAMEBUFFERTEXTURELAYERPROC _glFramebufferTextureLayer = nullptr;
PFNGLMAPBUFFERRANGEPROC _glMapBufferRange = nullptr;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC _glFlushMappedBufferRange = nullptr;
PFNGLBINDVERTEXARRAYPROC _glBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSPROC _glDeleteVertexArrays = nullptr;
PFNGLGENVERTEXARRAYSPROC _glGenVertexArrays = nullptr;
PFNGLISVERTEXARRAYPROC _glIsVertexArray = nullptr;
#pragma endregion

#pragma region OpenGL 3.1 functions
PFNGLDRAWARRAYSINSTANCEDPROC _glDrawArraysInstanced = nullptr;
PFNGLDRAWELEMENTSINSTANCEDPROC _glDrawElementsInstanced = nullptr;
PFNGLTEXBUFFERPROC _glTexBuffer = nullptr;
PFNGLPRIMITIVERESTARTINDEXPROC _glPrimitiveRestartIndex = nullptr;
PFNGLCOPYBUFFERSUBDATAPROC _glCopyBufferSubData = nullptr;
PFNGLGETUNIFORMINDICESPROC _glGetUniformIndices = nullptr;
PFNGLGETACTIVEUNIFORMSIVPROC _glGetActiveUniformsiv = nullptr;
PFNGLGETACTIVEUNIFORMNAMEPROC _glGetActiveUniformName = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC _glGetUniformBlockIndex = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC _glGetActiveUniformBlockiv = nullptr;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC _glGetActiveUniformBlockName = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC _glUniformBlockBinding = nullptr;
#pragma endregion

#pragma region OpenGL 3.2 functions
PFNGLDRAWELEMENTSBASEVERTEXPROC _glDrawElementsBaseVertex = nullptr;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC _glDrawRangeElementsBaseVertex = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC _glDrawElementsInstancedBaseVertex = nullptr;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC _glMultiDrawElementsBaseVertex = nullptr;
PFNGLPROVOKINGVERTEXPROC _glProvokingVertex = nullptr;
PFNGLFENCESYNCPROC _glFenceSync = nullptr;
PFNGLISSYNCPROC _glIsSync = nullptr;
PFNGLDELETESYNCPROC _glDeleteSync = nullptr;
PFNGLCLIENTWAITSYNCPROC _glClientWaitSync = nullptr;
PFNGLWAITSYNCPROC _glWaitSync = nullptr;
PFNGLGETINTEGER64VPROC _glGetInteger64v = nullptr;
PFNGLGETSYNCIVPROC _glGetSynciv = nullptr;
PFNGLGETINTEGER64I_VPROC _glGetInteger64i_v = nullptr;
PFNGLGETBUFFERPARAMETERI64VPROC _glGetBufferParameteri64v = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC _glFramebufferTexture = nullptr;
PFNGLTEXIMAGE2DMULTISAMPLEPROC _glTexImage2DMultisample = nullptr;
PFNGLTEXIMAGE3DMULTISAMPLEPROC _glTexImage3DMultisample = nullptr;
PFNGLGETMULTISAMPLEFVPROC _glGetMultisamplefv = nullptr;
PFNGLSAMPLEMASKIPROC _glSampleMaski = nullptr;
#pragma endregion

#pragma region OpenGL 3.3 functions
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC _glBindFragDataLocationIndexed = nullptr;
PFNGLGETFRAGDATAINDEXPROC _glGetFragDataIndex = nullptr;
PFNGLGENSAMPLERSPROC _glGenSamplers = nullptr;
PFNGLDELETESAMPLERSPROC _glDeleteSamplers = nullptr;
PFNGLISSAMPLERPROC _glIsSampler = nullptr;
PFNGLBINDSAMPLERPROC _glBindSampler = nullptr;
PFNGLSAMPLERPARAMETERIPROC _glSamplerParameteri = nullptr;
PFNGLSAMPLERPARAMETERIVPROC _glSamplerParameteriv = nullptr;
PFNGLSAMPLERPARAMETERFPROC _glSamplerParameterf = nullptr;
PFNGLSAMPLERPARAMETERFVPROC _glSamplerParameterfv = nullptr;
PFNGLSAMPLERPARAMETERIIVPROC _glSamplerParameterIiv = nullptr;
PFNGLSAMPLERPARAMETERIUIVPROC _glSamplerParameterIuiv = nullptr;
PFNGLGETSAMPLERPARAMETERIVPROC _glGetSamplerParameteriv = nullptr;
PFNGLGETSAMPLERPARAMETERIIVPROC _glGetSamplerParameterIiv = nullptr;
PFNGLGETSAMPLERPARAMETERFVPROC _glGetSamplerParameterfv = nullptr;
PFNGLGETSAMPLERPARAMETERIUIVPROC _glGetSamplerParameterIuiv = nullptr;
PFNGLQUERYCOUNTERPROC _glQueryCounter = nullptr;
PFNGLGETQUERYOBJECTI64VPROC _glGetQueryObjecti64v = nullptr;
PFNGLGETQUERYOBJECTUI64VPROC _glGetQueryObjectui64v = nullptr;
PFNGLVERTEXATTRIBDIVISORPROC _glVertexAttribDivisor = nullptr;
PFNGLVERTEXATTRIBP1UIPROC _glVertexAttribP1ui = nullptr;
PFNGLVERTEXATTRIBP1UIVPROC _glVertexAttribP1uiv = nullptr;
PFNGLVERTEXATTRIBP2UIPROC _glVertexAttribP2ui = nullptr;
PFNGLVERTEXATTRIBP2UIVPROC _glVertexAttribP2uiv = nullptr;
PFNGLVERTEXATTRIBP3UIPROC _glVertexAttribP3ui = nullptr;
PFNGLVERTEXATTRIBP3UIVPROC _glVertexAttribP3uiv = nullptr;
PFNGLVERTEXATTRIBP4UIPROC _glVertexAttribP4ui = nullptr;
PFNGLVERTEXATTRIBP4UIVPROC _glVertexAttribP4uiv = nullptr;
PFNGLVERTEXP2UIPROC _glVertexP2ui = nullptr;
PFNGLVERTEXP2UIVPROC _glVertexP2uiv = nullptr;
PFNGLVERTEXP3UIPROC _glVertexP3ui = nullptr;
PFNGLVERTEXP3UIVPROC _glVertexP3uiv = nullptr;
PFNGLVERTEXP4UIPROC _glVertexP4ui = nullptr;
PFNGLVERTEXP4UIVPROC _glVertexP4uiv = nullptr;
PFNGLTEXCOORDP1UIPROC _glTexCoordP1ui = nullptr;
PFNGLTEXCOORDP1UIVPROC _glTexCoordP1uiv = nullptr;
PFNGLTEXCOORDP2UIPROC _glTexCoordP2ui = nullptr;
PFNGLTEXCOORDP2UIVPROC _glTexCoordP2uiv = nullptr;
PFNGLTEXCOORDP3UIPROC _glTexCoordP3ui = nullptr;
PFNGLTEXCOORDP3UIVPROC _glTexCoordP3uiv = nullptr;
PFNGLTEXCOORDP4UIPROC _glTexCoordP4ui = nullptr;
PFNGLTEXCOORDP4UIVPROC _glTexCoordP4uiv = nullptr;
PFNGLMULTITEXCOORDP1UIPROC _glMultiTexCoordP1ui = nullptr;
PFNGLMULTITEXCOORDP1UIVPROC _glMultiTexCoordP1uiv = nullptr;
PFNGLMULTITEXCOORDP2UIPROC _glMultiTexCoordP2ui = nullptr;
PFNGLMULTITEXCOORDP2UIVPROC _glMultiTexCoordP2uiv = nullptr;
PFNGLMULTITEXCOORDP3UIPROC _glMultiTexCoordP3ui = nullptr;
PFNGLMULTITEXCOORDP3UIVPROC _glMultiTexCoordP3uiv = nullptr;
PFNGLMULTITEXCOORDP4UIPROC _glMultiTexCoordP4ui = nullptr;
PFNGLMULTITEXCOORDP4UIVPROC _glMultiTexCoordP4uiv = nullptr;
PFNGLNORMALP3UIPROC _glNormalP3ui = nullptr;
PFNGLNORMALP3UIVPROC _glNormalP3uiv = nullptr;
PFNGLCOLORP3UIPROC _glColorP3ui = nullptr;
PFNGLCOLORP3UIVPROC _glColorP3uiv = nullptr;
PFNGLCOLORP4UIPROC _glColorP4ui = nullptr;
PFNGLCOLORP4UIVPROC _glColorP4uiv = nullptr;
PFNGLSECONDARYCOLORP3UIPROC _glSecondaryColorP3ui = nullptr;
PFNGLSECONDARYCOLORP3UIVPROC _glSecondaryColorP3uiv = nullptr;
#pragma endregion

#pragma region OpenGL 4.0 functions
PFNGLMINSAMPLESHADINGPROC _glMinSampleShading = nullptr;
PFNGLBLENDEQUATIONIPROC _glBlendEquationi = nullptr;
PFNGLBLENDEQUATIONSEPARATEIPROC _glBlendEquationSeparatei = nullptr;
PFNGLBLENDFUNCIPROC _glBlendFunci = nullptr;
PFNGLBLENDFUNCSEPARATEIPROC _glBlendFuncSeparatei = nullptr;
PFNGLDRAWARRAYSINDIRECTPROC _glDrawArraysIndirect = nullptr;
PFNGLDRAWELEMENTSINDIRECTPROC _glDrawElementsIndirect = nullptr;
PFNGLUNIFORM1DPROC _glUniform1d = nullptr;
PFNGLUNIFORM2DPROC _glUniform2d = nullptr;
PFNGLUNIFORM3DPROC _glUniform3d = nullptr;
PFNGLUNIFORM4DPROC _glUniform4d = nullptr;
PFNGLUNIFORM1DVPROC _glUniform1dv = nullptr;
PFNGLUNIFORM2DVPROC _glUniform2dv = nullptr;
PFNGLUNIFORM3DVPROC _glUniform3dv = nullptr;
PFNGLUNIFORM4DVPROC _glUniform4dv = nullptr;
PFNGLUNIFORMMATRIX2DVPROC _glUniformMatrix2dv = nullptr;
PFNGLUNIFORMMATRIX3DVPROC _glUniformMatrix3dv = nullptr;
PFNGLUNIFORMMATRIX4DVPROC _glUniformMatrix4dv = nullptr;
PFNGLUNIFORMMATRIX2X3DVPROC _glUniformMatrix2x3dv = nullptr;
PFNGLUNIFORMMATRIX2X4DVPROC _glUniformMatrix2x4dv = nullptr;
PFNGLUNIFORMMATRIX3X2DVPROC _glUniformMatrix3x2dv = nullptr;
PFNGLUNIFORMMATRIX3X4DVPROC _glUniformMatrix3x4dv = nullptr;
PFNGLUNIFORMMATRIX4X2DVPROC _glUniformMatrix4x2dv = nullptr;
PFNGLUNIFORMMATRIX4X3DVPROC _glUniformMatrix4x3dv = nullptr;
PFNGLGETUNIFORMDVPROC _glGetUniformdv = nullptr;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC _glGetSubroutineUniformLocation = nullptr;
PFNGLGETSUBROUTINEINDEXPROC _glGetSubroutineIndex = nullptr;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC _glGetActiveSubroutineUniformiv = nullptr;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC _glGetActiveSubroutineUniformName = nullptr;
PFNGLGETACTIVESUBROUTINENAMEPROC _glGetActiveSubroutineName = nullptr;
PFNGLUNIFORMSUBROUTINESUIVPROC _glUniformSubroutinesuiv = nullptr;
PFNGLGETUNIFORMSUBROUTINEUIVPROC _glGetUniformSubroutineuiv = nullptr;
PFNGLGETPROGRAMSTAGEIVPROC _glGetProgramStageiv = nullptr;
PFNGLPATCHPARAMETERIPROC _glPatchParameteri = nullptr;
PFNGLPATCHPARAMETERFVPROC _glPatchParameterfv = nullptr;
PFNGLBINDTRANSFORMFEEDBACKPROC _glBindTransformFeedback = nullptr;
PFNGLDELETETRANSFORMFEEDBACKSPROC _glDeleteTransformFeedbacks = nullptr;
PFNGLGENTRANSFORMFEEDBACKSPROC _glGenTransformFeedbacks = nullptr;
PFNGLISTRANSFORMFEEDBACKPROC _glIsTransformFeedback = nullptr;
PFNGLPAUSETRANSFORMFEEDBACKPROC _glPauseTransformFeedback = nullptr;
PFNGLRESUMETRANSFORMFEEDBACKPROC _glResumeTransformFeedback = nullptr;
PFNGLDRAWTRANSFORMFEEDBACKPROC _glDrawTransformFeedback = nullptr;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC _glDrawTransformFeedbackStream = nullptr;
PFNGLBEGINQUERYINDEXEDPROC _glBeginQueryIndexed = nullptr;
PFNGLENDQUERYINDEXEDPROC _glEndQueryIndexed = nullptr;
PFNGLGETQUERYINDEXEDIVPROC _glGetQueryIndexediv = nullptr;
#pragma endregion

#pragma region OpenGL 4.1 functions
PFNGLRELEASESHADERCOMPILERPROC _glReleaseShaderCompiler = nullptr;
PFNGLSHADERBINARYPROC _glShaderBinary = nullptr;
PFNGLGETSHADERPRECISIONFORMATPROC _glGetShaderPrecisionFormat = nullptr;
PFNGLDEPTHRANGEFPROC _glDepthRangef = nullptr;
PFNGLCLEARDEPTHFPROC _glClearDepthf = nullptr;
PFNGLGETPROGRAMBINARYPROC _glGetProgramBinary = nullptr;
PFNGLPROGRAMBINARYPROC _glProgramBinary = nullptr;
PFNGLPROGRAMPARAMETERIPROC _glProgramParameteri = nullptr;
PFNGLUSEPROGRAMSTAGESPROC _glUseProgramStages = nullptr;
PFNGLACTIVESHADERPROGRAMPROC _glActiveShaderProgram = nullptr;
PFNGLCREATESHADERPROGRAMVPROC _glCreateShaderProgramv = nullptr;
PFNGLBINDPROGRAMPIPELINEPROC _glBindProgramPipeline = nullptr;
PFNGLDELETEPROGRAMPIPELINESPROC _glDeleteProgramPipelines = nullptr;
PFNGLGENPROGRAMPIPELINESPROC _glGenProgramPipelines = nullptr;
PFNGLISPROGRAMPIPELINEPROC _glIsProgramPipeline = nullptr;
PFNGLGETPROGRAMPIPELINEIVPROC _glGetProgramPipelineiv = nullptr;
PFNGLPROGRAMUNIFORM1IPROC _glProgramUniform1i = nullptr;
PFNGLPROGRAMUNIFORM1IVPROC _glProgramUniform1iv = nullptr;
PFNGLPROGRAMUNIFORM1FPROC _glProgramUniform1f = nullptr;
PFNGLPROGRAMUNIFORM1FVPROC _glProgramUniform1fv = nullptr;
PFNGLPROGRAMUNIFORM1DPROC _glProgramUniform1d = nullptr;
PFNGLPROGRAMUNIFORM1DVPROC _glProgramUniform1dv = nullptr;
PFNGLPROGRAMUNIFORM1UIPROC _glProgramUniform1ui = nullptr;
PFNGLPROGRAMUNIFORM1UIVPROC _glProgramUniform1uiv = nullptr;
PFNGLPROGRAMUNIFORM2IPROC _glProgramUniform2i = nullptr;
PFNGLPROGRAMUNIFORM2IVPROC _glProgramUniform2iv = nullptr;
PFNGLPROGRAMUNIFORM2FPROC _glProgramUniform2f = nullptr;
PFNGLPROGRAMUNIFORM2FVPROC _glProgramUniform2fv = nullptr;
PFNGLPROGRAMUNIFORM2DPROC _glProgramUniform2d = nullptr;
PFNGLPROGRAMUNIFORM2DVPROC _glProgramUniform2dv = nullptr;
PFNGLPROGRAMUNIFORM2UIPROC _glProgramUniform2ui = nullptr;
PFNGLPROGRAMUNIFORM2UIVPROC _glProgramUniform2uiv = nullptr;
PFNGLPROGRAMUNIFORM3IPROC _glProgramUniform3i = nullptr;
PFNGLPROGRAMUNIFORM3IVPROC _glProgramUniform3iv = nullptr;
PFNGLPROGRAMUNIFORM3FPROC _glProgramUniform3f = nullptr;
PFNGLPROGRAMUNIFORM3FVPROC _glProgramUniform3fv = nullptr;
PFNGLPROGRAMUNIFORM3DPROC _glProgramUniform3d = nullptr;
PFNGLPROGRAMUNIFORM3DVPROC _glProgramUniform3dv = nullptr;
PFNGLPROGRAMUNIFORM3UIPROC _glProgramUniform3ui = nullptr;
PFNGLPROGRAMUNIFORM3UIVPROC _glProgramUniform3uiv = nullptr;
PFNGLPROGRAMUNIFORM4IPROC _glProgramUniform4i = nullptr;
PFNGLPROGRAMUNIFORM4IVPROC _glProgramUniform4iv = nullptr;
PFNGLPROGRAMUNIFORM4FPROC _glProgramUniform4f = nullptr;
PFNGLPROGRAMUNIFORM4FVPROC _glProgramUniform4fv = nullptr;
PFNGLPROGRAMUNIFORM4DPROC _glProgramUniform4d = nullptr;
PFNGLPROGRAMUNIFORM4DVPROC _glProgramUniform4dv = nullptr;
PFNGLPROGRAMUNIFORM4UIPROC _glProgramUniform4ui = nullptr;
PFNGLPROGRAMUNIFORM4UIVPROC _glProgramUniform4uiv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC _glProgramUniformMatrix2fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC _glProgramUniformMatrix3fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC _glProgramUniformMatrix4fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC _glProgramUniformMatrix2dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC _glProgramUniformMatrix3dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC _glProgramUniformMatrix4dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC _glProgramUniformMatrix2x3fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC _glProgramUniformMatrix3x2fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC _glProgramUniformMatrix2x4fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC _glProgramUniformMatrix4x2fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC _glProgramUniformMatrix3x4fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC _glProgramUniformMatrix4x3fv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC _glProgramUniformMatrix2x3dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC _glProgramUniformMatrix3x2dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC _glProgramUniformMatrix2x4dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC _glProgramUniformMatrix4x2dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC _glProgramUniformMatrix3x4dv = nullptr;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC _glProgramUniformMatrix4x3dv = nullptr;
PFNGLVALIDATEPROGRAMPIPELINEPROC _glValidateProgramPipeline = nullptr;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC _glGetProgramPipelineInfoLog = nullptr;
PFNGLVERTEXATTRIBL1DPROC _glVertexAttribL1d = nullptr;
PFNGLVERTEXATTRIBL2DPROC _glVertexAttribL2d = nullptr;
PFNGLVERTEXATTRIBL3DPROC _glVertexAttribL3d = nullptr;
PFNGLVERTEXATTRIBL4DPROC _glVertexAttribL4d = nullptr;
PFNGLVERTEXATTRIBL1DVPROC _glVertexAttribL1dv = nullptr;
PFNGLVERTEXATTRIBL2DVPROC _glVertexAttribL2dv = nullptr;
PFNGLVERTEXATTRIBL3DVPROC _glVertexAttribL3dv = nullptr;
PFNGLVERTEXATTRIBL4DVPROC _glVertexAttribL4dv = nullptr;
PFNGLVERTEXATTRIBLPOINTERPROC _glVertexAttribLPointer = nullptr;
PFNGLGETVERTEXATTRIBLDVPROC _glGetVertexAttribLdv = nullptr;
PFNGLVIEWPORTARRAYVPROC _glViewportArrayv = nullptr;
PFNGLVIEWPORTINDEXEDFPROC _glViewportIndexedf = nullptr;
PFNGLVIEWPORTINDEXEDFVPROC _glViewportIndexedfv = nullptr;
PFNGLSCISSORARRAYVPROC _glScissorArrayv = nullptr;
PFNGLSCISSORINDEXEDPROC _glScissorIndexed = nullptr;
PFNGLSCISSORINDEXEDVPROC _glScissorIndexedv = nullptr;
PFNGLDEPTHRANGEARRAYVPROC _glDepthRangeArrayv = nullptr;
PFNGLDEPTHRANGEINDEXEDPROC _glDepthRangeIndexed = nullptr;
PFNGLGETFLOATI_VPROC _glGetFloati_v = nullptr;
PFNGLGETDOUBLEI_VPROC _glGetDoublei_v = nullptr;
#pragma endregion

#pragma region OpenGL 4.2 functions
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC _glDrawArraysInstancedBaseInstance = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC _glDrawElementsInstancedBaseInstance = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC _glDrawElementsInstancedBaseVertexBaseInstance = nullptr;
PFNGLGETINTERNALFORMATIVPROC _glGetInternalformativ = nullptr;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC _glGetActiveAtomicCounterBufferiv = nullptr;
PFNGLBINDIMAGETEXTUREPROC _glBindImageTexture = nullptr;
PFNGLMEMORYBARRIERPROC _glMemoryBarrier = nullptr;
PFNGLTEXSTORAGE1DPROC _glTexStorage1D = nullptr;
PFNGLTEXSTORAGE2DPROC _glTexStorage2D = nullptr;
PFNGLTEXSTORAGE3DPROC _glTexStorage3D = nullptr;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC _glDrawTransformFeedbackInstanced = nullptr;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC _glDrawTransformFeedbackStreamInstanced = nullptr;
#pragma endregion

#pragma region OpenGL 4.3 functions
PFNGLCLEARBUFFERDATAPROC _glClearBufferData = nullptr;
PFNGLCLEARBUFFERSUBDATAPROC _glClearBufferSubData = nullptr;
PFNGLDISPATCHCOMPUTEPROC _glDispatchCompute = nullptr;
PFNGLDISPATCHCOMPUTEINDIRECTPROC _glDispatchComputeIndirect = nullptr;
PFNGLCOPYIMAGESUBDATAPROC _glCopyImageSubData = nullptr;
PFNGLFRAMEBUFFERPARAMETERIPROC _glFramebufferParameteri = nullptr;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC _glGetFramebufferParameteriv = nullptr;
PFNGLGETINTERNALFORMATI64VPROC _glGetInternalformati64v = nullptr;
PFNGLINVALIDATETEXSUBIMAGEPROC _glInvalidateTexSubImage = nullptr;
PFNGLINVALIDATETEXIMAGEPROC _glInvalidateTexImage = nullptr;
PFNGLINVALIDATEBUFFERSUBDATAPROC _glInvalidateBufferSubData = nullptr;
PFNGLINVALIDATEBUFFERDATAPROC _glInvalidateBufferData = nullptr;
PFNGLINVALIDATEFRAMEBUFFERPROC _glInvalidateFramebuffer = nullptr;
PFNGLINVALIDATESUBFRAMEBUFFERPROC _glInvalidateSubFramebuffer = nullptr;
PFNGLMULTIDRAWARRAYSINDIRECTPROC _glMultiDrawArraysIndirect = nullptr;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC _glMultiDrawElementsIndirect = nullptr;
PFNGLGETPROGRAMINTERFACEIVPROC _glGetProgramInterfaceiv = nullptr;
PFNGLGETPROGRAMRESOURCEINDEXPROC _glGetProgramResourceIndex = nullptr;
PFNGLGETPROGRAMRESOURCENAMEPROC _glGetProgramResourceName = nullptr;
PFNGLGETPROGRAMRESOURCEIVPROC _glGetProgramResourceiv = nullptr;
PFNGLGETPROGRAMRESOURCELOCATIONPROC _glGetProgramResourceLocation = nullptr;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC _glGetProgramResourceLocationIndex = nullptr;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC _glShaderStorageBlockBinding = nullptr;
PFNGLTEXBUFFERRANGEPROC _glTexBufferRange = nullptr;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC _glTexStorage2DMultisample = nullptr;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC _glTexStorage3DMultisample = nullptr;
PFNGLTEXTUREVIEWPROC _glTextureView = nullptr;
PFNGLBINDVERTEXBUFFERPROC _glBindVertexBuffer = nullptr;
PFNGLVERTEXATTRIBFORMATPROC _glVertexAttribFormat = nullptr;
PFNGLVERTEXATTRIBIFORMATPROC _glVertexAttribIFormat = nullptr;
PFNGLVERTEXATTRIBLFORMATPROC _glVertexAttribLFormat = nullptr;
PFNGLVERTEXATTRIBBINDINGPROC _glVertexAttribBinding = nullptr;
PFNGLVERTEXBINDINGDIVISORPROC _glVertexBindingDivisor = nullptr;
PFNGLDEBUGMESSAGECONTROLPROC _glDebugMessageControl = nullptr;
PFNGLDEBUGMESSAGEINSERTPROC _glDebugMessageInsert = nullptr;
PFNGLDEBUGMESSAGECALLBACKPROC _glDebugMessageCallback = nullptr;
PFNGLGETDEBUGMESSAGELOGPROC _glGetDebugMessageLog = nullptr;
PFNGLPUSHDEBUGGROUPPROC _glPushDebugGroup = nullptr;
PFNGLPOPDEBUGGROUPPROC _glPopDebugGroup = nullptr;
PFNGLOBJECTLABELPROC _glObjectLabel = nullptr;
PFNGLGETOBJECTLABELPROC _glGetObjectLabel = nullptr;
PFNGLOBJECTPTRLABELPROC _glObjectPtrLabel = nullptr;
PFNGLGETOBJECTPTRLABELPROC _glGetObjectPtrLabel = nullptr;
PFNGLGETPOINTERVPROC _glGetPointerv = nullptr;
#pragma endregion

#pragma region OpenGL 4.4 functions
PFNGLBUFFERSTORAGEPROC _glBufferStorage = nullptr;
PFNGLCLEARTEXIMAGEPROC _glClearTexImage = nullptr;
PFNGLCLEARTEXSUBIMAGEPROC _glClearTexSubImage = nullptr;
PFNGLBINDBUFFERSBASEPROC _glBindBuffersBase = nullptr;
PFNGLBINDBUFFERSRANGEPROC _glBindBuffersRange = nullptr;
PFNGLBINDTEXTURESPROC _glBindTextures = nullptr;
PFNGLBINDSAMPLERSPROC _glBindSamplers = nullptr;
PFNGLBINDIMAGETEXTURESPROC _glBindImageTextures = nullptr;
PFNGLBINDVERTEXBUFFERSPROC _glBindVertexBuffers = nullptr;
#pragma endregion

#pragma region OpenGL 4.5 functions
PFNGLCLIPCONTROLPROC _glClipControl = nullptr;
PFNGLCREATETRANSFORMFEEDBACKSPROC _glCreateTransformFeedbacks = nullptr;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC _glTransformFeedbackBufferBase = nullptr;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC _glTransformFeedbackBufferRange = nullptr;
PFNGLGETTRANSFORMFEEDBACKIVPROC _glGetTransformFeedbackiv = nullptr;
PFNGLGETTRANSFORMFEEDBACKI_VPROC _glGetTransformFeedbacki_v = nullptr;
PFNGLGETTRANSFORMFEEDBACKI64_VPROC _glGetTransformFeedbacki64_v = nullptr;
PFNGLCREATEBUFFERSPROC _glCreateBuffers = nullptr;
PFNGLNAMEDBUFFERSTORAGEPROC _glNamedBufferStorage = nullptr;
PFNGLNAMEDBUFFERDATAPROC _glNamedBufferData = nullptr;
PFNGLNAMEDBUFFERSUBDATAPROC _glNamedBufferSubData = nullptr;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC _glCopyNamedBufferSubData = nullptr;
PFNGLCLEARNAMEDBUFFERDATAPROC _glClearNamedBufferData = nullptr;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC _glClearNamedBufferSubData = nullptr;
PFNGLMAPNAMEDBUFFERPROC _glMapNamedBuffer = nullptr;
PFNGLMAPNAMEDBUFFERRANGEPROC _glMapNamedBufferRange = nullptr;
PFNGLUNMAPNAMEDBUFFERPROC _glUnmapNamedBuffer = nullptr;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC _glFlushMappedNamedBufferRange = nullptr;
PFNGLGETNAMEDBUFFERPARAMETERIVPROC _glGetNamedBufferParameteriv = nullptr;
PFNGLGETNAMEDBUFFERPARAMETERI64VPROC _glGetNamedBufferParameteri64v = nullptr;
PFNGLGETNAMEDBUFFERPOINTERVPROC _glGetNamedBufferPointerv = nullptr;
PFNGLGETNAMEDBUFFERSUBDATAPROC _glGetNamedBufferSubData = nullptr;
PFNGLCREATEFRAMEBUFFERSPROC _glCreateFramebuffers = nullptr;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC _glNamedFramebufferRenderbuffer = nullptr;
PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC _glNamedFramebufferParameteri = nullptr;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC _glNamedFramebufferTexture = nullptr;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC _glNamedFramebufferTextureLayer = nullptr;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC _glNamedFramebufferDrawBuffer = nullptr;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC _glNamedFramebufferDrawBuffers = nullptr;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC _glNamedFramebufferReadBuffer = nullptr;
PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC _glInvalidateNamedFramebufferData = nullptr;
PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC _glInvalidateNamedFramebufferSubData = nullptr;
PFNGLCLEARNAMEDFRAMEBUFFERIVPROC _glClearNamedFramebufferiv = nullptr;
PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC _glClearNamedFramebufferuiv = nullptr;
PFNGLCLEARNAMEDFRAMEBUFFERFVPROC _glClearNamedFramebufferfv = nullptr;
PFNGLCLEARNAMEDFRAMEBUFFERFIPROC _glClearNamedFramebufferfi = nullptr;
PFNGLBLITNAMEDFRAMEBUFFERPROC _glBlitNamedFramebuffer = nullptr;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC _glCheckNamedFramebufferStatus = nullptr;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC _glGetNamedFramebufferParameteriv = nullptr;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC _glGetNamedFramebufferAttachmentParameteriv = nullptr;
PFNGLCREATERENDERBUFFERSPROC _glCreateRenderbuffers = nullptr;
PFNGLNAMEDRENDERBUFFERSTORAGEPROC _glNamedRenderbufferStorage = nullptr;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC _glNamedRenderbufferStorageMultisample = nullptr;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC _glGetNamedRenderbufferParameteriv = nullptr;
PFNGLCREATETEXTURESPROC _glCreateTextures = nullptr;
PFNGLTEXTUREBUFFERPROC _glTextureBuffer = nullptr;
PFNGLTEXTUREBUFFERRANGEPROC _glTextureBufferRange = nullptr;
PFNGLTEXTURESTORAGE1DPROC _glTextureStorage1D = nullptr;
PFNGLTEXTURESTORAGE2DPROC _glTextureStorage2D = nullptr;
PFNGLTEXTURESTORAGE3DPROC _glTextureStorage3D = nullptr;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC _glTextureStorage2DMultisample = nullptr;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC _glTextureStorage3DMultisample = nullptr;
PFNGLTEXTURESUBIMAGE1DPROC _glTextureSubImage1D = nullptr;
PFNGLTEXTURESUBIMAGE2DPROC _glTextureSubImage2D = nullptr;
PFNGLTEXTURESUBIMAGE3DPROC _glTextureSubImage3D = nullptr;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC _glCompressedTextureSubImage1D = nullptr;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC _glCompressedTextureSubImage2D = nullptr;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC _glCompressedTextureSubImage3D = nullptr;
PFNGLCOPYTEXTURESUBIMAGE1DPROC _glCopyTextureSubImage1D = nullptr;
PFNGLCOPYTEXTURESUBIMAGE2DPROC _glCopyTextureSubImage2D = nullptr;
PFNGLCOPYTEXTURESUBIMAGE3DPROC _glCopyTextureSubImage3D = nullptr;
PFNGLTEXTUREPARAMETERFPROC _glTextureParameterf = nullptr;
PFNGLTEXTUREPARAMETERFVPROC _glTextureParameterfv = nullptr;
PFNGLTEXTUREPARAMETERIPROC _glTextureParameteri = nullptr;
PFNGLTEXTUREPARAMETERIIVPROC _glTextureParameterIiv = nullptr;
PFNGLTEXTUREPARAMETERIUIVPROC _glTextureParameterIuiv = nullptr;
PFNGLTEXTUREPARAMETERIVPROC _glTextureParameteriv = nullptr;
PFNGLGENERATETEXTUREMIPMAPPROC _glGenerateTextureMipmap = nullptr;
PFNGLBINDTEXTUREUNITPROC _glBindTextureUnit = nullptr;
PFNGLGETTEXTUREIMAGEPROC _glGetTextureImage = nullptr;
PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC _glGetCompressedTextureImage = nullptr;
PFNGLGETTEXTURELEVELPARAMETERFVPROC _glGetTextureLevelParameterfv = nullptr;
PFNGLGETTEXTURELEVELPARAMETERIVPROC _glGetTextureLevelParameteriv = nullptr;
PFNGLGETTEXTUREPARAMETERFVPROC _glGetTextureParameterfv = nullptr;
PFNGLGETTEXTUREPARAMETERIIVPROC _glGetTextureParameterIiv = nullptr;
PFNGLGETTEXTUREPARAMETERIUIVPROC _glGetTextureParameterIuiv = nullptr;
PFNGLGETTEXTUREPARAMETERIVPROC _glGetTextureParameteriv = nullptr;
PFNGLCREATEVERTEXARRAYSPROC _glCreateVertexArrays = nullptr;
PFNGLDISABLEVERTEXARRAYATTRIBPROC _glDisableVertexArrayAttrib = nullptr;
PFNGLENABLEVERTEXARRAYATTRIBPROC _glEnableVertexArrayAttrib = nullptr;
PFNGLVERTEXARRAYELEMENTBUFFERPROC _glVertexArrayElementBuffer = nullptr;
PFNGLVERTEXARRAYVERTEXBUFFERPROC _glVertexArrayVertexBuffer = nullptr;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC _glVertexArrayVertexBuffers = nullptr;
PFNGLVERTEXARRAYATTRIBBINDINGPROC _glVertexArrayAttribBinding = nullptr;
PFNGLVERTEXARRAYATTRIBFORMATPROC _glVertexArrayAttribFormat = nullptr;
PFNGLVERTEXARRAYATTRIBIFORMATPROC _glVertexArrayAttribIFormat = nullptr;
PFNGLVERTEXARRAYATTRIBLFORMATPROC _glVertexArrayAttribLFormat = nullptr;
PFNGLVERTEXARRAYBINDINGDIVISORPROC _glVertexArrayBindingDivisor = nullptr;
PFNGLGETVERTEXARRAYIVPROC _glGetVertexArrayiv = nullptr;
PFNGLGETVERTEXARRAYINDEXEDIVPROC _glGetVertexArrayIndexediv = nullptr;
PFNGLGETVERTEXARRAYINDEXED64IVPROC _glGetVertexArrayIndexed64iv = nullptr;
PFNGLCREATESAMPLERSPROC _glCreateSamplers = nullptr;
PFNGLCREATEPROGRAMPIPELINESPROC _glCreateProgramPipelines = nullptr;
PFNGLCREATEQUERIESPROC _glCreateQueries = nullptr;
PFNGLGETQUERYBUFFEROBJECTI64VPROC _glGetQueryBufferObjecti64v = nullptr;
PFNGLGETQUERYBUFFEROBJECTIVPROC _glGetQueryBufferObjectiv = nullptr;
PFNGLGETQUERYBUFFEROBJECTUI64VPROC _glGetQueryBufferObjectui64v = nullptr;
PFNGLGETQUERYBUFFEROBJECTUIVPROC _glGetQueryBufferObjectuiv = nullptr;
PFNGLMEMORYBARRIERBYREGIONPROC _glMemoryBarrierByRegion = nullptr;
PFNGLGETTEXTURESUBIMAGEPROC _glGetTextureSubImage = nullptr;
PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC _glGetCompressedTextureSubImage = nullptr;
PFNGLGETGRAPHICSRESETSTATUSPROC _glGetGraphicsResetStatus = nullptr;
PFNGLGETNCOMPRESSEDTEXIMAGEPROC _glGetnCompressedTexImage = nullptr;
PFNGLGETNTEXIMAGEPROC _glGetnTexImage = nullptr;
PFNGLGETNUNIFORMDVPROC _glGetnUniformdv = nullptr;
PFNGLGETNUNIFORMFVPROC _glGetnUniformfv = nullptr;
PFNGLGETNUNIFORMIVPROC _glGetnUniformiv = nullptr;
PFNGLGETNUNIFORMUIVPROC _glGetnUniformuiv = nullptr;
PFNGLREADNPIXELSPROC _glReadnPixels = nullptr;
PFNGLGETNMAPDVPROC _glGetnMapdv = nullptr;
PFNGLGETNMAPFVPROC _glGetnMapfv = nullptr;
PFNGLGETNMAPIVPROC _glGetnMapiv = nullptr;
PFNGLGETNPIXELMAPFVPROC _glGetnPixelMapfv = nullptr;
PFNGLGETNPIXELMAPUIVPROC _glGetnPixelMapuiv = nullptr;
PFNGLGETNPIXELMAPUSVPROC _glGetnPixelMapusv = nullptr;
PFNGLGETNPOLYGONSTIPPLEPROC _glGetnPolygonStipple = nullptr;
PFNGLGETNCOLORTABLEPROC _glGetnColorTable = nullptr;
PFNGLGETNCONVOLUTIONFILTERPROC _glGetnConvolutionFilter = nullptr;
PFNGLGETNSEPARABLEFILTERPROC _glGetnSeparableFilter = nullptr;
PFNGLGETNHISTOGRAMPROC _glGetnHistogram = nullptr;
PFNGLGETNMINMAXPROC _glGetnMinmax = nullptr;
PFNGLTEXTUREBARRIERPROC _glTextureBarrier = nullptr;
#pragma endregion

#pragma region OpenGL 4.6 functions
PFNGLSPECIALIZESHADERPROC _glSpecializeShader = nullptr;
PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC _glMultiDrawArraysIndirectCount = nullptr;
PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC _glMultiDrawElementsIndirectCount = nullptr;
PFNGLPOLYGONOFFSETCLAMPPROC _glPolygonOffsetClamp = nullptr;

#pragma endregion

namespace mogl
{
	HMODULE OpenGLLoader::s_OpenGLLib;

	OpenGLVersion OpenGLLoader::LoadOpenGL(OpenGLVersion version)
	{
		auto intVersion = (int)version;

		if (intVersion < 0 || intVersion >(int)OpenGLVersion::OPENGL_4_6)
			return OpenGLVersion::OPENGL_NONE;

		s_OpenGLLib = LoadLibrary(L"opengl32.dll");

		if (!s_OpenGLLib)
			return OpenGLVersion::OPENGL_NONE;

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_0)
		{
			if (!LoadOpenGL_1_0())
				return OpenGLVersion::OPENGL_NONE;
			if (intVersion == (int)OpenGLVersion::OPENGL_1_0)
				return OpenGLVersion::OPENGL_1_0;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_1)
		{
			if (!LoadOpenGL_1_1())
				return OpenGLVersion::OPENGL_1_0;
			if (intVersion == (int)OpenGLVersion::OPENGL_1_1)
				return OpenGLVersion::OPENGL_1_1;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_2)
		{
			if (!LoadOpenGL_1_2())
				return OpenGLVersion::OPENGL_1_1;
			if (intVersion == (int)OpenGLVersion::OPENGL_1_2)
				return OpenGLVersion::OPENGL_1_2;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_3)
		{
			if (!LoadOpenGL_1_3())
				return OpenGLVersion::OPENGL_1_2;
			if (intVersion == (int)OpenGLVersion::OPENGL_1_3)
				return OpenGLVersion::OPENGL_1_3;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_4)
		{
			if (!LoadOpenGL_1_4())
				return OpenGLVersion::OPENGL_1_3;
			if (intVersion == (int)OpenGLVersion::OPENGL_1_4)
				return OpenGLVersion::OPENGL_1_4;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_5)
		{
			if (!LoadOpenGL_1_5())
				return OpenGLVersion::OPENGL_1_4;
			if (intVersion == (int)OpenGLVersion::OPENGL_1_5)
				return OpenGLVersion::OPENGL_1_5;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_2_0)
		{
			if (!LoadOpenGL_2_0())
				return OpenGLVersion::OPENGL_1_5;
			if (intVersion == (int)OpenGLVersion::OPENGL_2_0)
				return OpenGLVersion::OPENGL_2_0;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_2_1)
		{
			if (!LoadOpenGL_2_1())
				return OpenGLVersion::OPENGL_2_0;
			if (intVersion == (int)OpenGLVersion::OPENGL_2_1)
				return OpenGLVersion::OPENGL_2_1;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_3_0)
		{
			if (!LoadOpenGL_3_0())
				return OpenGLVersion::OPENGL_2_1;
			if (intVersion == (int)OpenGLVersion::OPENGL_3_0)
				return OpenGLVersion::OPENGL_3_0;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_3_1)
		{
			if (!LoadOpenGL_3_1())
				return OpenGLVersion::OPENGL_3_0;
			if (intVersion == (int)OpenGLVersion::OPENGL_3_1)
				return OpenGLVersion::OPENGL_3_1;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_3_2)
		{
			if (!LoadOpenGL_3_2())
				return OpenGLVersion::OPENGL_3_1;
			if (intVersion == (int)OpenGLVersion::OPENGL_3_2)
				return OpenGLVersion::OPENGL_3_2;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_3_3)
		{
			if (!LoadOpenGL_3_3())
				return OpenGLVersion::OPENGL_3_2;
			if (intVersion == (int)OpenGLVersion::OPENGL_3_3)
				return OpenGLVersion::OPENGL_3_3;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_0)
		{
			if (!LoadOpenGL_4_0())
				return OpenGLVersion::OPENGL_3_3;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_0)
				return OpenGLVersion::OPENGL_4_0;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_1)
		{
			if (!LoadOpenGL_4_1())
				return OpenGLVersion::OPENGL_4_0;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_1)
				return OpenGLVersion::OPENGL_4_1;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_2)
		{
			if (!LoadOpenGL_4_2())
				return OpenGLVersion::OPENGL_4_1;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_2)
				return OpenGLVersion::OPENGL_4_2;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_3)
		{
			if (!LoadOpenGL_4_3())
				return OpenGLVersion::OPENGL_4_2;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_3)
				return OpenGLVersion::OPENGL_4_3;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_4)
		{
			if (!LoadOpenGL_4_4())
				return OpenGLVersion::OPENGL_4_3;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_4)
				return OpenGLVersion::OPENGL_4_4;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_5)
		{
			if (!LoadOpenGL_4_5())
				return OpenGLVersion::OPENGL_4_4;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_5)
				return OpenGLVersion::OPENGL_4_5;
		}

		if (intVersion >= (int)OpenGLVersion::OPENGL_4_6)
		{
			if (!LoadOpenGL_4_6())
				return OpenGLVersion::OPENGL_4_5;
			if (intVersion == (int)OpenGLVersion::OPENGL_4_6)
				return OpenGLVersion::OPENGL_4_6;
		}

		FreeLibrary(s_OpenGLLib);

		return OpenGLVersion::OPENGL_4_6;
	}

	void* OpenGLLoader::LoadFunction(const char* name, bool& success)
	{
		// Load newer functions via wglGetProcAddress
		auto* func = (void*)wglGetProcAddress(name);

		if ( func == nullptr ||
			(func == (void*)0x1) || 
			(func == (void*)0x2) || 
			(func == (void*)0x3) ||
			(func == (void*)-1))
		{
			func = (void*)GetProcAddress(s_OpenGLLib, name);
		}

		if (!func)
			success = false;

		return func;
	}

	bool OpenGLLoader::LoadOpenGL_1_0()
	{
		bool success = true;

		_glCullFace = (PFNGLCULLFACEPROC)LoadFunction("glCullFace", success);
		_glFrontFace = (PFNGLFRONTFACEPROC)LoadFunction("glFrontFace", success);
		_glHint = (PFNGLHINTPROC)LoadFunction("glHint", success);
		_glLineWidth = (PFNGLLINEWIDTHPROC)LoadFunction("glLineWidth", success);
		_glPointSize = (PFNGLPOINTSIZEPROC)LoadFunction("glPointSize", success);
		_glPolygonMode = (PFNGLPOLYGONMODEPROC)LoadFunction("glPolygonMode", success);
		_glScissor = (PFNGLSCISSORPROC)LoadFunction("glScissor", success);
		_glTexParameterf = (PFNGLTEXPARAMETERFPROC)LoadFunction("glTexParameterf", success);
		_glTexParameterfv = (PFNGLTEXPARAMETERFVPROC)LoadFunction("glTexParameterfv", success);
		_glTexParameteri = (PFNGLTEXPARAMETERIPROC)LoadFunction("glTexParameteri", success);
		_glTexParameteriv = (PFNGLTEXPARAMETERIVPROC)LoadFunction("glTexParameteriv", success);
		_glTexImage1D = (PFNGLTEXIMAGE1DPROC)LoadFunction("glTexImage1D", success);
		_glTexImage2D = (PFNGLTEXIMAGE2DPROC)LoadFunction("glTexImage2D", success);
		_glDrawBuffer = (PFNGLDRAWBUFFERPROC)LoadFunction("glDrawBuffer", success);
		_glClear = (PFNGLCLEARPROC)LoadFunction("glClear", success);
		_glClearColor = (PFNGLCLEARCOLORPROC)LoadFunction("glClearColor", success);
		_glClearStencil = (PFNGLCLEARSTENCILPROC)LoadFunction("glClearStencil", success);
		_glClearDepth = (PFNGLCLEARDEPTHPROC)LoadFunction("glClearDepth", success);
		_glStencilMask = (PFNGLSTENCILMASKPROC)LoadFunction("glStencilMask", success);
		_glColorMask = (PFNGLCOLORMASKPROC)LoadFunction("glColorMask", success);
		_glDepthMask = (PFNGLDEPTHMASKPROC)LoadFunction("glDepthMask", success);
		_glDisable = (PFNGLDISABLEPROC)LoadFunction("glDisable", success);
		_glEnable = (PFNGLENABLEPROC)LoadFunction("glEnable", success);
		_glFinish = (PFNGLFINISHPROC)LoadFunction("glFinish", success);
		_glFlush = (PFNGLFLUSHPROC)LoadFunction("glFlush", success);
		_glBlendFunc = (PFNGLBLENDFUNCPROC)LoadFunction("glBlendFunc", success);
		_glLogicOp = (PFNGLLOGICOPPROC)LoadFunction("glLogicOp", success);
		_glStencilFunc = (PFNGLSTENCILFUNCPROC)LoadFunction("glStencilFunc", success);
		_glStencilOp = (PFNGLSTENCILOPPROC)LoadFunction("glStencilOp", success);
		_glDepthFunc = (PFNGLDEPTHFUNCPROC)LoadFunction("glDepthFunc", success);
		_glPixelStoref = (PFNGLPIXELSTOREFPROC)LoadFunction("glPixelStoref", success);
		_glPixelStorei = (PFNGLPIXELSTOREIPROC)LoadFunction("glPixelStorei", success);
		_glReadBuffer = (PFNGLREADBUFFERPROC)LoadFunction("glReadBuffer", success);
		_glReadPixels = (PFNGLREADPIXELSPROC)LoadFunction("glReadPixels", success);
		_glGetBooleanv = (PFNGLGETBOOLEANVPROC)LoadFunction("glGetBooleanv", success);
		_glGetDoublev = (PFNGLGETDOUBLEVPROC)LoadFunction("glGetDoublev", success);
		_glGetError = (PFNGLGETERRORPROC)LoadFunction("glGetError", success);
		_glGetFloatv = (PFNGLGETFLOATVPROC)LoadFunction("glGetFloatv", success);
		_glGetIntegerv = (PFNGLGETINTEGERVPROC)LoadFunction("glGetIntegerv", success);
		_glGetString = (PFNGLGETSTRINGPROC)LoadFunction("glGetString", success);
		_glGetTexImage = (PFNGLGETTEXIMAGEPROC)LoadFunction("glGetTexImage", success);
		_glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)LoadFunction("glGetTexParameterfv", success);
		_glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)LoadFunction("glGetTexParameteriv", success);
		_glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC)LoadFunction("glGetTexLevelParameterfv", success);
		_glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC)LoadFunction("glGetTexLevelParameteriv", success);
		_glIsEnabled = (PFNGLISENABLEDPROC)LoadFunction("glIsEnabled", success);
		_glDepthRange = (PFNGLDEPTHRANGEPROC)LoadFunction("glDepthRange", success);
		_glViewport = (PFNGLVIEWPORTPROC)LoadFunction("glViewport", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_1_1()
	{
		bool success = true;

		_glDrawArrays = (PFNGLDRAWARRAYSPROC)LoadFunction("glDrawArrays", success);
		_glDrawElements = (PFNGLDRAWELEMENTSPROC)LoadFunction("glDrawElements", success);
		_glPolygonOffset = (PFNGLPOLYGONOFFSETPROC)LoadFunction("glPolygonOffset", success);
		_glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC)LoadFunction("glCopyTexImage1D", success);
		_glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)LoadFunction("glCopyTexImage2D", success);
		_glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC)LoadFunction("glCopyTexSubImage1D", success);
		_glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)LoadFunction("glCopyTexSubImage2D", success);
		_glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC)LoadFunction("glTexSubImage1D", success);
		_glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)LoadFunction("glTexSubImage2D", success);
		_glBindTexture = (PFNGLBINDTEXTUREPROC)LoadFunction("glBindTexture", success);
		_glDeleteTextures = (PFNGLDELETETEXTURESPROC)LoadFunction("glDeleteTextures", success);
		_glGenTextures = (PFNGLGENTEXTURESPROC)LoadFunction("glGenTextures", success);
		_glIsTexture = (PFNGLISTEXTUREPROC)LoadFunction("glIsTexture", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_1_2()
	{
		bool success = true;

		_glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)LoadFunction("glDrawRangeElements", success);
		_glTexImage3D = (PFNGLTEXIMAGE3DPROC)LoadFunction("glTexImage3D", success);
		_glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)LoadFunction("glTexSubImage3D", success);
		_glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)LoadFunction("glCopyTexSubImage3D", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_1_3()
	{
		bool success = true;

		_glActiveTexture = (PFNGLACTIVETEXTUREPROC)LoadFunction("glActiveTexture", success);
		_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)LoadFunction("glSampleCoverage", success);
		_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)LoadFunction("glCompressedTexImage3D", success);
		_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)LoadFunction("glCompressedTexImage2D", success);
		_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)LoadFunction("glCompressedTexImage1D", success);
		_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)LoadFunction("glCompressedTexSubImage3D", success);
		_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)LoadFunction("glCompressedTexSubImage2D", success);
		_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)LoadFunction("glCompressedTexSubImage1D", success);
		_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)LoadFunction("glGetCompressedTexImage", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_1_4()
	{
		bool success = true;

		_glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)LoadFunction("glBlendFuncSeparate", success);
		_glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)LoadFunction("glMultiDrawArrays", success);
		_glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)LoadFunction("glMultiDrawElements", success);
		_glPointParameterf = (PFNGLPOINTPARAMETERFPROC)LoadFunction("glPointParameterf", success);
		_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)LoadFunction("glPointParameterfv", success);
		_glPointParameteri = (PFNGLPOINTPARAMETERIPROC)LoadFunction("glPointParameteri", success);
		_glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)LoadFunction("glPointParameteriv", success);
		_glBlendColor = (PFNGLBLENDCOLORPROC)LoadFunction("glBlendColor", success);
		_glBlendEquation = (PFNGLBLENDEQUATIONPROC)LoadFunction("glBlendEquation", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_1_5()
	{
		bool success = true;

		_glGenQueries = (PFNGLGENQUERIESPROC)LoadFunction("glGenQueries", success);
		_glDeleteQueries = (PFNGLDELETEQUERIESPROC)LoadFunction("glDeleteQueries", success);
		_glIsQuery = (PFNGLISQUERYPROC)LoadFunction("glIsQuery", success);
		_glBeginQuery = (PFNGLBEGINQUERYPROC)LoadFunction("glBeginQuery", success);
		_glEndQuery = (PFNGLENDQUERYPROC)LoadFunction("glEndQuery", success);
		_glGetQueryiv = (PFNGLGETQUERYIVPROC)LoadFunction("glGetQueryiv", success);
		_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)LoadFunction("glGetQueryObjectiv", success);
		_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)LoadFunction("glGetQueryObjectuiv", success);
		_glBindBuffer = (PFNGLBINDBUFFERPROC)LoadFunction("glBindBuffer", success);
		_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)LoadFunction("glDeleteBuffers", success);
		_glGenBuffers = (PFNGLGENBUFFERSPROC)LoadFunction("glGenBuffers", success);
		_glIsBuffer = (PFNGLISBUFFERPROC)LoadFunction("glIsBuffer", success);
		_glBufferData = (PFNGLBUFFERDATAPROC)LoadFunction("glBufferData", success);
		_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)LoadFunction("glBufferSubData", success);
		_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)LoadFunction("glGetBufferSubData", success);
		_glMapBuffer = (PFNGLMAPBUFFERPROC)LoadFunction("glMapBuffer", success);
		_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)LoadFunction("glUnmapBuffer", success);
		_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)LoadFunction("glGetBufferParameteriv", success);
		_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)LoadFunction("glGetBufferPointerv", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_2_0()
	{
		bool success = true;

		_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)LoadFunction("glBlendEquationSeparate", success);
		_glDrawBuffers = (PFNGLDRAWBUFFERSPROC)LoadFunction("glDrawBuffers", success);
		_glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)LoadFunction("glStencilOpSeparate", success);
		_glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)LoadFunction("glStencilFuncSeparate", success);
		_glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)LoadFunction("glStencilMaskSeparate", success);
		_glAttachShader = (PFNGLATTACHSHADERPROC)LoadFunction("glAttachShader", success);
		_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)LoadFunction("glBindAttribLocation", success);
		_glCompileShader = (PFNGLCOMPILESHADERPROC)LoadFunction("glCompileShader", success);
		_glCreateProgram = (PFNGLCREATEPROGRAMPROC)LoadFunction("glCreateProgram", success);
		_glCreateShader = (PFNGLCREATESHADERPROC)LoadFunction("glCreateShader", success);
		_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)LoadFunction("glDeleteProgram", success);
		_glDeleteShader = (PFNGLDELETESHADERPROC)LoadFunction("glDeleteShader", success);
		_glDetachShader = (PFNGLDETACHSHADERPROC)LoadFunction("glDetachShader", success);
		_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)LoadFunction("glDisableVertexAttribArray", success);
		_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)LoadFunction("glEnableVertexAttribArray", success);
		_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)LoadFunction("glGetActiveAttrib", success);
		_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)LoadFunction("glGetActiveUniform", success);
		_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)LoadFunction("glGetAttachedShaders", success);
		_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)LoadFunction("glGetAttribLocation", success);
		_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)LoadFunction("glGetProgramiv", success);
		_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)LoadFunction("glGetProgramInfoLog", success);
		_glGetShaderiv = (PFNGLGETSHADERIVPROC)LoadFunction("glGetShaderiv", success);
		_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)LoadFunction("glGetShaderInfoLog", success);
		_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)LoadFunction("glGetShaderSource", success);
		_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)LoadFunction("glGetUniformLocation", success);
		_glGetUniformfv = (PFNGLGETUNIFORMFVPROC)LoadFunction("glGetUniformfv", success);
		_glGetUniformiv = (PFNGLGETUNIFORMIVPROC)LoadFunction("glGetUniformiv", success);
		_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)LoadFunction("glGetVertexAttribdv", success);
		_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)LoadFunction("glGetVertexAttribfv", success);
		_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)LoadFunction("glGetVertexAttribiv", success);
		_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)LoadFunction("glGetVertexAttribPointerv", success);
		_glIsProgram = (PFNGLISPROGRAMPROC)LoadFunction("glIsProgram", success);
		_glIsShader = (PFNGLISSHADERPROC)LoadFunction("glIsShader", success);
		_glLinkProgram = (PFNGLLINKPROGRAMPROC)LoadFunction("glLinkProgram", success);
		_glShaderSource = (PFNGLSHADERSOURCEPROC)LoadFunction("glShaderSource", success);
		_glUseProgram = (PFNGLUSEPROGRAMPROC)LoadFunction("glUseProgram", success);
		_glUniform1f = (PFNGLUNIFORM1FPROC)LoadFunction("glUniform1f", success);
		_glUniform2f = (PFNGLUNIFORM2FPROC)LoadFunction("glUniform2f", success);
		_glUniform3f = (PFNGLUNIFORM3FPROC)LoadFunction("glUniform3f", success);
		_glUniform4f = (PFNGLUNIFORM4FPROC)LoadFunction("glUniform4f", success);
		_glUniform1i = (PFNGLUNIFORM1IPROC)LoadFunction("glUniform1i", success);
		_glUniform2i = (PFNGLUNIFORM2IPROC)LoadFunction("glUniform2i", success);
		_glUniform3i = (PFNGLUNIFORM3IPROC)LoadFunction("glUniform3i", success);
		_glUniform4i = (PFNGLUNIFORM4IPROC)LoadFunction("glUniform4i", success);
		_glUniform1fv = (PFNGLUNIFORM1FVPROC)LoadFunction("glUniform1fv", success);
		_glUniform2fv = (PFNGLUNIFORM2FVPROC)LoadFunction("glUniform2fv", success);
		_glUniform3fv = (PFNGLUNIFORM3FVPROC)LoadFunction("glUniform3fv", success);
		_glUniform4fv = (PFNGLUNIFORM4FVPROC)LoadFunction("glUniform4fv", success);
		_glUniform1iv = (PFNGLUNIFORM1IVPROC)LoadFunction("glUniform1iv", success);
		_glUniform2iv = (PFNGLUNIFORM2IVPROC)LoadFunction("glUniform2iv", success);
		_glUniform3iv = (PFNGLUNIFORM3IVPROC)LoadFunction("glUniform3iv", success);
		_glUniform4iv = (PFNGLUNIFORM4IVPROC)LoadFunction("glUniform4iv", success);
		_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)LoadFunction("glUniformMatrix2fv", success);
		_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)LoadFunction("glUniformMatrix3fv", success);
		_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)LoadFunction("glUniformMatrix4fv", success);
		_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)LoadFunction("glValidateProgram", success);
		_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)LoadFunction("glVertexAttrib1d", success);
		_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)LoadFunction("glVertexAttrib1dv", success);
		_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)LoadFunction("glVertexAttrib1f", success);
		_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)LoadFunction("glVertexAttrib1fv", success);
		_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)LoadFunction("glVertexAttrib1s", success);
		_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)LoadFunction("glVertexAttrib1sv", success);
		_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)LoadFunction("glVertexAttrib2d", success);
		_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)LoadFunction("glVertexAttrib2dv", success);
		_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)LoadFunction("glVertexAttrib2f", success);
		_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)LoadFunction("glVertexAttrib2fv", success);
		_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)LoadFunction("glVertexAttrib2s", success);
		_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)LoadFunction("glVertexAttrib2sv", success);
		_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)LoadFunction("glVertexAttrib3d", success);
		_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)LoadFunction("glVertexAttrib3dv", success);
		_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)LoadFunction("glVertexAttrib3f", success);
		_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)LoadFunction("glVertexAttrib3fv", success);
		_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)LoadFunction("glVertexAttrib3s", success);
		_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)LoadFunction("glVertexAttrib3sv", success);
		_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)LoadFunction("glVertexAttrib4Nbv", success);
		_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)LoadFunction("glVertexAttrib4Niv", success);
		_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)LoadFunction("glVertexAttrib4Nsv", success);
		_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)LoadFunction("glVertexAttrib4Nub", success);
		_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)LoadFunction("glVertexAttrib4Nubv", success);
		_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)LoadFunction("glVertexAttrib4Nuiv", success);
		_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)LoadFunction("glVertexAttrib4Nusv", success);
		_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)LoadFunction("glVertexAttrib4bv", success);
		_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)LoadFunction("glVertexAttrib4d", success);
		_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)LoadFunction("glVertexAttrib4dv", success);
		_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)LoadFunction("glVertexAttrib4f", success);
		_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)LoadFunction("glVertexAttrib4fv", success);
		_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)LoadFunction("glVertexAttrib4iv", success);
		_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)LoadFunction("glVertexAttrib4s", success);
		_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)LoadFunction("glVertexAttrib4sv", success);
		_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)LoadFunction("glVertexAttrib4ubv", success);
		_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)LoadFunction("glVertexAttrib4uiv", success);
		_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)LoadFunction("glVertexAttrib4usv", success);
		_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)LoadFunction("glVertexAttribPointer", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_2_1()
	{
		bool success = true;

		_glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)LoadFunction("glUniformMatrix2x3fv", success);
		_glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)LoadFunction("glUniformMatrix3x2fv", success);
		_glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)LoadFunction("glUniformMatrix2x4fv", success);
		_glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)LoadFunction("glUniformMatrix4x2fv", success);
		_glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)LoadFunction("glUniformMatrix3x4fv", success);
		_glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)LoadFunction("glUniformMatrix4x3fv", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_3_0()
	{
		bool success = true;

		_glColorMaski = (PFNGLCOLORMASKIPROC)LoadFunction("glColorMaski", success);
		_glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)LoadFunction("glGetBooleani_v", success);
		_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)LoadFunction("glGetIntegeri_v", success);
		_glEnablei = (PFNGLENABLEIPROC)LoadFunction("glEnablei", success);
		_glDisablei = (PFNGLDISABLEIPROC)LoadFunction("glDisablei", success);
		_glIsEnabledi = (PFNGLISENABLEDIPROC)LoadFunction("glIsEnabledi", success);
		_glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)LoadFunction("glBeginTransformFeedback", success);
		_glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)LoadFunction("glEndTransformFeedback", success);
		_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)LoadFunction("glBindBufferRange", success);
		_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)LoadFunction("glBindBufferBase", success);
		_glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)LoadFunction("glTransformFeedbackVaryings", success);
		_glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)LoadFunction("glGetTransformFeedbackVarying", success);
		_glClampColor = (PFNGLCLAMPCOLORPROC)LoadFunction("glClampColor", success);
		_glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)LoadFunction("glBeginConditionalRender", success);
		_glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)LoadFunction("glEndConditionalRender", success);
		_glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)LoadFunction("glVertexAttribIPointer", success);
		_glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)LoadFunction("glGetVertexAttribIiv", success);
		_glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)LoadFunction("glGetVertexAttribIuiv", success);
		_glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)LoadFunction("glVertexAttribI1i", success);
		_glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)LoadFunction("glVertexAttribI2i", success);
		_glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)LoadFunction("glVertexAttribI3i", success);
		_glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)LoadFunction("glVertexAttribI4i", success);
		_glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)LoadFunction("glVertexAttribI1ui", success);
		_glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)LoadFunction("glVertexAttribI2ui", success);
		_glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)LoadFunction("glVertexAttribI3ui", success);
		_glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)LoadFunction("glVertexAttribI4ui", success);
		_glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)LoadFunction("glVertexAttribI1iv", success);
		_glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)LoadFunction("glVertexAttribI2iv", success);
		_glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)LoadFunction("glVertexAttribI3iv", success);
		_glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)LoadFunction("glVertexAttribI4iv", success);
		_glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)LoadFunction("glVertexAttribI1uiv", success);
		_glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)LoadFunction("glVertexAttribI2uiv", success);
		_glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)LoadFunction("glVertexAttribI3uiv", success);
		_glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)LoadFunction("glVertexAttribI4uiv", success);
		_glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)LoadFunction("glVertexAttribI4bv", success);
		_glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)LoadFunction("glVertexAttribI4sv", success);
		_glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)LoadFunction("glVertexAttribI4ubv", success);
		_glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)LoadFunction("glVertexAttribI4usv", success);
		_glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)LoadFunction("glGetUniformuiv", success);
		_glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)LoadFunction("glBindFragDataLocation", success);
		_glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)LoadFunction("glGetFragDataLocation", success);
		_glUniform1ui = (PFNGLUNIFORM1UIPROC)LoadFunction("glUniform1ui", success);
		_glUniform2ui = (PFNGLUNIFORM2UIPROC)LoadFunction("glUniform2ui", success);
		_glUniform3ui = (PFNGLUNIFORM3UIPROC)LoadFunction("glUniform3ui", success);
		_glUniform4ui = (PFNGLUNIFORM4UIPROC)LoadFunction("glUniform4ui", success);
		_glUniform1uiv = (PFNGLUNIFORM1UIVPROC)LoadFunction("glUniform1uiv", success);
		_glUniform2uiv = (PFNGLUNIFORM2UIVPROC)LoadFunction("glUniform2uiv", success);
		_glUniform3uiv = (PFNGLUNIFORM3UIVPROC)LoadFunction("glUniform3uiv", success);
		_glUniform4uiv = (PFNGLUNIFORM4UIVPROC)LoadFunction("glUniform4uiv", success);
		_glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)LoadFunction("glTexParameterIiv", success);
		_glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)LoadFunction("glTexParameterIuiv", success);
		_glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)LoadFunction("glGetTexParameterIiv", success);
		_glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)LoadFunction("glGetTexParameterIuiv", success);
		_glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)LoadFunction("glClearBufferiv", success);
		_glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)LoadFunction("glClearBufferuiv", success);
		_glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)LoadFunction("glClearBufferfv", success);
		_glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)LoadFunction("glClearBufferfi", success);
		_glGetStringi = (PFNGLGETSTRINGIPROC)LoadFunction("glGetStringi", success);
		_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)LoadFunction("glIsRenderbuffer", success);
		_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)LoadFunction("glBindRenderbuffer", success);
		_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)LoadFunction("glDeleteRenderbuffers", success);
		_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)LoadFunction("glGenRenderbuffers", success);
		_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)LoadFunction("glRenderbufferStorage", success);
		_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)LoadFunction("glGetRenderbufferParameteriv", success);
		_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)LoadFunction("glIsFramebuffer", success);
		_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)LoadFunction("glBindFramebuffer", success);
		_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)LoadFunction("glDeleteFramebuffers", success);
		_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)LoadFunction("glGenFramebuffers", success);
		_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)LoadFunction("glCheckFramebufferStatus", success);
		_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)LoadFunction("glFramebufferTexture1D", success);
		_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)LoadFunction("glFramebufferTexture2D", success);
		_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)LoadFunction("glFramebufferTexture3D", success);
		_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)LoadFunction("glFramebufferRenderbuffer", success);
		_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)LoadFunction("glGetFramebufferAttachmentParameteriv", success);
		_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)LoadFunction("glGenerateMipmap", success);
		_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)LoadFunction("glBlitFramebuffer", success);
		_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)LoadFunction("glRenderbufferStorageMultisample", success);
		_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)LoadFunction("glFramebufferTextureLayer", success);
		_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)LoadFunction("glMapBufferRange", success);
		_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)LoadFunction("glFlushMappedBufferRange", success);
		_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)LoadFunction("glBindVertexArray", success);
		_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)LoadFunction("glDeleteVertexArrays", success);
		_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)LoadFunction("glGenVertexArrays", success);
		_glIsVertexArray = (PFNGLISVERTEXARRAYPROC)LoadFunction("glIsVertexArray", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_3_1()
	{
		bool success = true;

		_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)LoadFunction("glDrawArraysInstanced", success);
		_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)LoadFunction("glDrawElementsInstanced", success);
		_glTexBuffer = (PFNGLTEXBUFFERPROC)LoadFunction("glTexBuffer", success);
		_glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)LoadFunction("glPrimitiveRestartIndex", success);
		_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)LoadFunction("glCopyBufferSubData", success);
		_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)LoadFunction("glGetUniformIndices", success);
		_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)LoadFunction("glGetActiveUniformsiv", success);
		_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)LoadFunction("glGetActiveUniformName", success);
		_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)LoadFunction("glGetUniformBlockIndex", success);
		_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)LoadFunction("glGetActiveUniformBlockiv", success);
		_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)LoadFunction("glGetActiveUniformBlockName", success);
		_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)LoadFunction("glUniformBlockBinding", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_3_2()
	{
		bool success = true;

		_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)LoadFunction("glDrawElementsBaseVertex", success);
		_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)LoadFunction("glDrawRangeElementsBaseVertex", success);
		_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)LoadFunction("glDrawElementsInstancedBaseVertex", success);
		_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)LoadFunction("glMultiDrawElementsBaseVertex", success);
		_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)LoadFunction("glProvokingVertex", success);
		_glFenceSync = (PFNGLFENCESYNCPROC)LoadFunction("glFenceSync", success);
		_glIsSync = (PFNGLISSYNCPROC)LoadFunction("glIsSync", success);
		_glDeleteSync = (PFNGLDELETESYNCPROC)LoadFunction("glDeleteSync", success);
		_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)LoadFunction("glClientWaitSync", success);
		_glWaitSync = (PFNGLWAITSYNCPROC)LoadFunction("glWaitSync", success);
		_glGetInteger64v = (PFNGLGETINTEGER64VPROC)LoadFunction("glGetInteger64v", success);
		_glGetSynciv = (PFNGLGETSYNCIVPROC)LoadFunction("glGetSynciv", success);
		_glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)LoadFunction("glGetInteger64i_v", success);
		_glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)LoadFunction("glGetBufferParameteri64v", success);
		_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)LoadFunction("glFramebufferTexture", success);
		_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)LoadFunction("glTexImage2DMultisample", success);
		_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)LoadFunction("glTexImage3DMultisample", success);
		_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)LoadFunction("glGetMultisamplefv", success);
		_glSampleMaski = (PFNGLSAMPLEMASKIPROC)LoadFunction("glSampleMaski", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_3_3()
	{
		bool success = true;

		_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)LoadFunction("glBindFragDataLocationIndexed", success);
		_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)LoadFunction("glGetFragDataIndex", success);
		_glGenSamplers = (PFNGLGENSAMPLERSPROC)LoadFunction("glGenSamplers", success);
		_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)LoadFunction("glDeleteSamplers", success);
		_glIsSampler = (PFNGLISSAMPLERPROC)LoadFunction("glIsSampler", success);
		_glBindSampler = (PFNGLBINDSAMPLERPROC)LoadFunction("glBindSampler", success);
		_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)LoadFunction("glSamplerParameteri", success);
		_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)LoadFunction("glSamplerParameteriv", success);
		_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)LoadFunction("glSamplerParameterf", success);
		_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)LoadFunction("glSamplerParameterfv", success);
		_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)LoadFunction("glSamplerParameterIiv", success);
		_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)LoadFunction("glSamplerParameterIuiv", success);
		_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)LoadFunction("glGetSamplerParameteriv", success);
		_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)LoadFunction("glGetSamplerParameterIiv", success);
		_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)LoadFunction("glGetSamplerParameterfv", success);
		_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)LoadFunction("glGetSamplerParameterIuiv", success);
		_glQueryCounter = (PFNGLQUERYCOUNTERPROC)LoadFunction("glQueryCounter", success);
		_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)LoadFunction("glGetQueryObjecti64v", success);
		_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)LoadFunction("glGetQueryObjectui64v", success);
		_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)LoadFunction("glVertexAttribDivisor", success);
		_glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)LoadFunction("glVertexAttribP1ui", success);
		_glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)LoadFunction("glVertexAttribP1uiv", success);
		_glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)LoadFunction("glVertexAttribP2ui", success);
		_glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)LoadFunction("glVertexAttribP2uiv", success);
		_glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)LoadFunction("glVertexAttribP3ui", success);
		_glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)LoadFunction("glVertexAttribP3uiv", success);
		_glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)LoadFunction("glVertexAttribP4ui", success);
		_glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)LoadFunction("glVertexAttribP4uiv", success);
		_glVertexP2ui = (PFNGLVERTEXP2UIPROC)LoadFunction("glVertexP2ui", success);
		_glVertexP2uiv = (PFNGLVERTEXP2UIVPROC)LoadFunction("glVertexP2uiv", success);
		_glVertexP3ui = (PFNGLVERTEXP3UIPROC)LoadFunction("glVertexP3ui", success);
		_glVertexP3uiv = (PFNGLVERTEXP3UIVPROC)LoadFunction("glVertexP3uiv", success);
		_glVertexP4ui = (PFNGLVERTEXP4UIPROC)LoadFunction("glVertexP4ui", success);
		_glVertexP4uiv = (PFNGLVERTEXP4UIVPROC)LoadFunction("glVertexP4uiv", success);
		_glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC)LoadFunction("glTexCoordP1ui", success);
		_glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC)LoadFunction("glTexCoordP1uiv", success);
		_glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC)LoadFunction("glTexCoordP2ui", success);
		_glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC)LoadFunction("glTexCoordP2uiv", success);
		_glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC)LoadFunction("glTexCoordP3ui", success);
		_glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC)LoadFunction("glTexCoordP3uiv", success);
		_glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC)LoadFunction("glTexCoordP4ui", success);
		_glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC)LoadFunction("glTexCoordP4uiv", success);
		_glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC)LoadFunction("glMultiTexCoordP1ui", success);
		_glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC)LoadFunction("glMultiTexCoordP1uiv", success);
		_glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC)LoadFunction("glMultiTexCoordP2ui", success);
		_glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC)LoadFunction("glMultiTexCoordP2uiv", success);
		_glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC)LoadFunction("glMultiTexCoordP3ui", success);
		_glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC)LoadFunction("glMultiTexCoordP3uiv", success);
		_glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC)LoadFunction("glMultiTexCoordP4ui", success);
		_glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC)LoadFunction("glMultiTexCoordP4uiv", success);
		_glNormalP3ui = (PFNGLNORMALP3UIPROC)LoadFunction("glNormalP3ui", success);
		_glNormalP3uiv = (PFNGLNORMALP3UIVPROC)LoadFunction("glNormalP3uiv", success);
		_glColorP3ui = (PFNGLCOLORP3UIPROC)LoadFunction("glColorP3ui", success);
		_glColorP3uiv = (PFNGLCOLORP3UIVPROC)LoadFunction("glColorP3uiv", success);
		_glColorP4ui = (PFNGLCOLORP4UIPROC)LoadFunction("glColorP4ui", success);
		_glColorP4uiv = (PFNGLCOLORP4UIVPROC)LoadFunction("glColorP4uiv", success);
		_glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC)LoadFunction("glSecondaryColorP3ui", success);
		_glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC)LoadFunction("glSecondaryColorP3uiv", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_0()
	{
		bool success = true;

		_glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)LoadFunction("glMinSampleShading", success);
		_glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)LoadFunction("glBlendEquationi", success);
		_glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)LoadFunction("glBlendEquationSeparatei", success);
		_glBlendFunci = (PFNGLBLENDFUNCIPROC)LoadFunction("glBlendFunci", success);
		_glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)LoadFunction("glBlendFuncSeparatei", success);
		_glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)LoadFunction("glDrawArraysIndirect", success);
		_glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)LoadFunction("glDrawElementsIndirect", success);
		_glUniform1d = (PFNGLUNIFORM1DPROC)LoadFunction("glUniform1d", success);
		_glUniform2d = (PFNGLUNIFORM2DPROC)LoadFunction("glUniform2d", success);
		_glUniform3d = (PFNGLUNIFORM3DPROC)LoadFunction("glUniform3d", success);
		_glUniform4d = (PFNGLUNIFORM4DPROC)LoadFunction("glUniform4d", success);
		_glUniform1dv = (PFNGLUNIFORM1DVPROC)LoadFunction("glUniform1dv", success);
		_glUniform2dv = (PFNGLUNIFORM2DVPROC)LoadFunction("glUniform2dv", success);
		_glUniform3dv = (PFNGLUNIFORM3DVPROC)LoadFunction("glUniform3dv", success);
		_glUniform4dv = (PFNGLUNIFORM4DVPROC)LoadFunction("glUniform4dv", success);
		_glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)LoadFunction("glUniformMatrix2dv", success);
		_glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)LoadFunction("glUniformMatrix3dv", success);
		_glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)LoadFunction("glUniformMatrix4dv", success);
		_glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)LoadFunction("glUniformMatrix2x3dv", success);
		_glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)LoadFunction("glUniformMatrix2x4dv", success);
		_glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)LoadFunction("glUniformMatrix3x2dv", success);
		_glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)LoadFunction("glUniformMatrix3x4dv", success);
		_glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)LoadFunction("glUniformMatrix4x2dv", success);
		_glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)LoadFunction("glUniformMatrix4x3dv", success);
		_glGetUniformdv = (PFNGLGETUNIFORMDVPROC)LoadFunction("glGetUniformdv", success);
		_glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)LoadFunction("glGetSubroutineUniformLocation", success);
		_glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)LoadFunction("glGetSubroutineIndex", success);
		_glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)LoadFunction("glGetActiveSubroutineUniformiv", success);
		_glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)LoadFunction("glGetActiveSubroutineUniformName", success);
		_glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)LoadFunction("glGetActiveSubroutineName", success);
		_glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)LoadFunction("glUniformSubroutinesuiv", success);
		_glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)LoadFunction("glGetUniformSubroutineuiv", success);
		_glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)LoadFunction("glGetProgramStageiv", success);
		_glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)LoadFunction("glPatchParameteri", success);
		_glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)LoadFunction("glPatchParameterfv", success);
		_glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)LoadFunction("glBindTransformFeedback", success);
		_glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)LoadFunction("glDeleteTransformFeedbacks", success);
		_glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)LoadFunction("glGenTransformFeedbacks", success);
		_glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)LoadFunction("glIsTransformFeedback", success);
		_glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)LoadFunction("glPauseTransformFeedback", success);
		_glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)LoadFunction("glResumeTransformFeedback", success);
		_glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)LoadFunction("glDrawTransformFeedback", success);
		_glDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC)LoadFunction("glDrawTransformFeedbackStream", success);
		_glBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)LoadFunction("glBeginQueryIndexed", success);
		_glEndQueryIndexed = (PFNGLENDQUERYINDEXEDPROC)LoadFunction("glEndQueryIndexed", success);
		_glGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIVPROC)LoadFunction("glGetQueryIndexediv", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_1()
	{
		bool success = true;

		_glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC)LoadFunction("glReleaseShaderCompiler", success);
		_glShaderBinary = (PFNGLSHADERBINARYPROC)LoadFunction("glShaderBinary", success);
		_glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMATPROC)LoadFunction("glGetShaderPrecisionFormat", success);
		_glDepthRangef = (PFNGLDEPTHRANGEFPROC)LoadFunction("glDepthRangef", success);
		_glClearDepthf = (PFNGLCLEARDEPTHFPROC)LoadFunction("glClearDepthf", success);
		_glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)LoadFunction("glGetProgramBinary", success);
		_glProgramBinary = (PFNGLPROGRAMBINARYPROC)LoadFunction("glProgramBinary", success);
		_glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)LoadFunction("glProgramParameteri", success);
		_glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)LoadFunction("glUseProgramStages", success);
		_glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)LoadFunction("glActiveShaderProgram", success);
		_glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)LoadFunction("glCreateShaderProgramv", success);
		_glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)LoadFunction("glBindProgramPipeline", success);
		_glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)LoadFunction("glDeleteProgramPipelines", success);
		_glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)LoadFunction("glGenProgramPipelines", success);
		_glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)LoadFunction("glIsProgramPipeline", success);
		_glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)LoadFunction("glGetProgramPipelineiv", success);
		_glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)LoadFunction("glProgramParameteri", success);
		_glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)LoadFunction("glProgramUniform1i", success);
		_glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)LoadFunction("glProgramUniform1iv", success);
		_glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)LoadFunction("glProgramUniform1f", success);
		_glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)LoadFunction("glProgramUniform1fv", success);
		_glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)LoadFunction("glProgramUniform1d", success);
		_glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)LoadFunction("glProgramUniform1dv", success);
		_glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)LoadFunction("glProgramUniform1ui", success);
		_glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)LoadFunction("glProgramUniform1uiv", success);
		_glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)LoadFunction("glProgramUniform2i", success);
		_glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)LoadFunction("glProgramUniform2iv", success);
		_glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)LoadFunction("glProgramUniform2f", success);
		_glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)LoadFunction("glProgramUniform2fv", success);
		_glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)LoadFunction("glProgramUniform2d", success);
		_glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)LoadFunction("glProgramUniform2dv", success);
		_glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)LoadFunction("glProgramUniform2ui", success);
		_glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)LoadFunction("glProgramUniform2uiv", success);
		_glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)LoadFunction("glProgramUniform3i", success);
		_glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)LoadFunction("glProgramUniform3iv", success);
		_glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)LoadFunction("glProgramUniform3f", success);
		_glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)LoadFunction("glProgramUniform3fv", success);
		_glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)LoadFunction("glProgramUniform3d", success);
		_glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)LoadFunction("glProgramUniform3dv", success);
		_glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)LoadFunction("glProgramUniform3ui", success);
		_glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)LoadFunction("glProgramUniform3uiv", success);
		_glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)LoadFunction("glProgramUniform4i", success);
		_glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)LoadFunction("glProgramUniform4iv", success);
		_glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)LoadFunction("glProgramUniform4f", success);
		_glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)LoadFunction("glProgramUniform4fv", success);
		_glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)LoadFunction("glProgramUniform4d", success);
		_glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)LoadFunction("glProgramUniform4dv", success);
		_glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)LoadFunction("glProgramUniform4ui", success);
		_glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)LoadFunction("glProgramUniform4uiv", success);
		_glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)LoadFunction("glProgramUniformMatrix2fv", success);
		_glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)LoadFunction("glProgramUniformMatrix3fv", success);
		_glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)LoadFunction("glProgramUniformMatrix4fv", success);
		_glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)LoadFunction("glProgramUniformMatrix2dv", success);
		_glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)LoadFunction("glProgramUniformMatrix3dv", success);
		_glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)LoadFunction("glProgramUniformMatrix4dv", success);
		_glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)LoadFunction("glProgramUniformMatrix2x3fv", success);
		_glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)LoadFunction("glProgramUniformMatrix3x2fv", success);
		_glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)LoadFunction("glProgramUniformMatrix2x4fv", success);
		_glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)LoadFunction("glProgramUniformMatrix4x2fv", success);
		_glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)LoadFunction("glProgramUniformMatrix3x4fv", success);
		_glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)LoadFunction("glProgramUniformMatrix4x3fv", success);
		_glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)LoadFunction("glProgramUniformMatrix2x3dv", success);
		_glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)LoadFunction("glProgramUniformMatrix3x2dv", success);
		_glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)LoadFunction("glProgramUniformMatrix2x4dv", success);
		_glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)LoadFunction("glProgramUniformMatrix4x2dv", success);
		_glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)LoadFunction("glProgramUniformMatrix3x4dv", success);
		_glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)LoadFunction("glProgramUniformMatrix4x3dv", success);
		_glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)LoadFunction("glValidateProgramPipeline", success);
		_glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)LoadFunction("glGetProgramPipelineInfoLog", success);
		_glVertexAttribL1d = (PFNGLVERTEXATTRIBL1DPROC)LoadFunction("glVertexAttribL1d", success);
		_glVertexAttribL2d = (PFNGLVERTEXATTRIBL2DPROC)LoadFunction("glVertexAttribL2d", success);
		_glVertexAttribL3d = (PFNGLVERTEXATTRIBL3DPROC)LoadFunction("glVertexAttribL3d", success);
		_glVertexAttribL4d = (PFNGLVERTEXATTRIBL4DPROC)LoadFunction("glVertexAttribL4d", success);
		_glVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DVPROC)LoadFunction("glVertexAttribL1dv", success);
		_glVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DVPROC)LoadFunction("glVertexAttribL2dv", success);
		_glVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DVPROC)LoadFunction("glVertexAttribL3dv", success);
		_glVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DVPROC)LoadFunction("glVertexAttribL4dv", success);
		_glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)LoadFunction("glVertexAttribLPointer", success);
		_glGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDVPROC)LoadFunction("glGetVertexAttribLdv", success);
		_glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)LoadFunction("glViewportArrayv", success);
		_glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)LoadFunction("glViewportIndexedf", success);
		_glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)LoadFunction("glViewportIndexedfv", success);
		_glScissorArrayv = (PFNGLSCISSORARRAYVPROC)LoadFunction("glScissorArrayv", success);
		_glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)LoadFunction("glScissorIndexed", success);
		_glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)LoadFunction("glScissorIndexedv", success);
		_glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)LoadFunction("glDepthRangeArrayv", success);
		_glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)LoadFunction("glDepthRangeIndexed", success);
		_glGetFloati_v = (PFNGLGETFLOATI_VPROC)LoadFunction("glGetFloati_v", success);
		_glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)LoadFunction("glGetDoublei_v", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_2()
	{
		bool success = true;

		_glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)LoadFunction("glDrawArraysInstancedBaseInstance", success);
		_glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)LoadFunction("glDrawElementsInstancedBaseInstance", success);
		_glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)LoadFunction("glDrawElementsInstancedBaseVertexBaseInstance", success);
		_glGetInternalformativ = (PFNGLGETINTERNALFORMATIVPROC)LoadFunction("glGetInternalformativ", success);
		_glGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC)LoadFunction("glGetActiveAtomicCounterBufferiv", success);
		_glBindImageTexture = (PFNGLBINDIMAGETEXTUREPROC)LoadFunction("glBindImageTexture", success);
		_glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)LoadFunction("glMemoryBarrier", success);
		_glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)LoadFunction("glTexStorage1D", success);
		_glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)LoadFunction("glTexStorage2D", success);
		_glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)LoadFunction("glTexStorage3D", success);
		_glDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC)LoadFunction("glDrawTransformFeedbackInstanced", success);
		_glDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC)LoadFunction("glDrawTransformFeedbackStreamInstanced", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_3()
	{
		bool success = true;

		_glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)LoadFunction("glClearBufferData", success);
		_glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)LoadFunction("glClearBufferSubData", success);
		_glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)LoadFunction("glDispatchCompute", success);
		_glDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECTPROC)LoadFunction("glDispatchComputeIndirect", success);
		_glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)LoadFunction("glCopyImageSubData", success);
		_glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)LoadFunction("glFramebufferParameteri", success);
		_glGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIVPROC)LoadFunction("glGetFramebufferParameteriv", success);
		_glGetInternalformati64v = (PFNGLGETINTERNALFORMATI64VPROC)LoadFunction("glGetInternalformati64v", success);
		_glInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGEPROC)LoadFunction("glInvalidateTexSubImage", success);
		_glInvalidateTexImage = (PFNGLINVALIDATETEXIMAGEPROC)LoadFunction("glInvalidateTexImage", success);
		_glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)LoadFunction("glInvalidateBufferSubData", success);
		_glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)LoadFunction("glInvalidateBufferData", success);
		_glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFERPROC)LoadFunction("glInvalidateFramebuffer", success);
		_glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFERPROC)LoadFunction("glInvalidateSubFramebuffer", success);
		_glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)LoadFunction("glMultiDrawArraysIndirect", success);
		_glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)LoadFunction("glMultiDrawElementsIndirect", success);
		_glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)LoadFunction("glGetProgramInterfaceiv", success);
		_glGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEXPROC)LoadFunction("glGetProgramResourceIndex", success);
		_glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)LoadFunction("glGetProgramResourceName", success);
		_glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)LoadFunction("glGetProgramResourceiv", success);
		_glGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATIONPROC)LoadFunction("glGetProgramResourceLocation", success);
		_glGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC)LoadFunction("glGetProgramResourceLocationIndex", success);
		_glShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDINGPROC)LoadFunction("glShaderStorageBlockBinding", success);
		_glTexBufferRange = (PFNGLTEXBUFFERRANGEPROC)LoadFunction("glTexBufferRange", success);
		_glTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLEPROC)LoadFunction("glTexStorage2DMultisample", success);
		_glTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLEPROC)LoadFunction("glTexStorage3DMultisample", success);
		_glTextureView = (PFNGLTEXTUREVIEWPROC)LoadFunction("glTextureView", success);
		_glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)LoadFunction("glBindVertexBuffer", success);
		_glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)LoadFunction("glVertexAttribFormat", success);
		_glVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMATPROC)LoadFunction("glVertexAttribIFormat", success);
		_glVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMATPROC)LoadFunction("glVertexAttribLFormat", success);
		_glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)LoadFunction("glVertexAttribBinding", success);
		_glVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISORPROC)LoadFunction("glVertexBindingDivisor", success);
		_glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)LoadFunction("glDebugMessageControl", success);
		_glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)LoadFunction("glDebugMessageInsert", success);
		_glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)LoadFunction("glDebugMessageCallback", success);
		_glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)LoadFunction("glGetDebugMessageLog", success);
		_glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)LoadFunction("glPushDebugGroup", success);
		_glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)LoadFunction("glPopDebugGroup", success);
		_glObjectLabel = (PFNGLOBJECTLABELPROC)LoadFunction("glObjectLabel", success);
		_glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)LoadFunction("glGetObjectLabel", success);
		_glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)LoadFunction("glObjectPtrLabel", success);
		_glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)LoadFunction("glGetObjectPtrLabel", success);
		_glGetPointerv = (PFNGLGETPOINTERVPROC)LoadFunction("glGetPointerv", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_4()
	{
		bool success = true;

		_glBufferStorage = (PFNGLBUFFERSTORAGEPROC)LoadFunction("glBufferStorage", success);
		_glClearTexImage = (PFNGLCLEARTEXIMAGEPROC)LoadFunction("glClearTexImage", success);
		_glClearTexSubImage = (PFNGLCLEARTEXSUBIMAGEPROC)LoadFunction("glClearTexSubImage", success);
		_glBindBuffersBase = (PFNGLBINDBUFFERSBASEPROC)LoadFunction("glBindBuffersBase", success);
		_glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)LoadFunction("glBindBuffersRange", success);
		_glBindTextures = (PFNGLBINDTEXTURESPROC)LoadFunction("glBindTextures", success);
		_glBindSamplers = (PFNGLBINDSAMPLERSPROC)LoadFunction("glBindSamplers", success);
		_glBindImageTextures = (PFNGLBINDIMAGETEXTURESPROC)LoadFunction("glBindImageTextures", success);
		_glBindVertexBuffers = (PFNGLBINDVERTEXBUFFERSPROC)LoadFunction("glBindVertexBuffers", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_5()
	{
		bool success = true;

		_glClipControl = (PFNGLCLIPCONTROLPROC)LoadFunction("glClipControl", success);
		_glCreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKSPROC)LoadFunction("glCreateTransformFeedbacks", success);
		_glTransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC)LoadFunction("glTransformFeedbackBufferBase", success);
		_glTransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC)LoadFunction("glTransformFeedbackBufferRange", success);
		_glGetTransformFeedbackiv = (PFNGLGETTRANSFORMFEEDBACKIVPROC)LoadFunction("glGetTransformFeedbackiv", success);
		_glGetTransformFeedbacki_v = (PFNGLGETTRANSFORMFEEDBACKI_VPROC)LoadFunction("glGetTransformFeedbacki_v", success);
		_glGetTransformFeedbacki64_v = (PFNGLGETTRANSFORMFEEDBACKI64_VPROC)LoadFunction("glGetTransformFeedbacki64_v", success);
		_glCreateBuffers = (PFNGLCREATEBUFFERSPROC)LoadFunction("glCreateBuffers", success);
		_glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)LoadFunction("glNamedBufferStorage", success);
		_glNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC)LoadFunction("glNamedBufferData", success);
		_glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)LoadFunction("glNamedBufferSubData", success);
		_glCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)LoadFunction("glCopyNamedBufferSubData", success);
		_glClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC)LoadFunction("glClearNamedBufferData", success);
		_glClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)LoadFunction("glClearNamedBufferSubData", success);
		_glMapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC)LoadFunction("glMapNamedBuffer", success);
		_glMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC)LoadFunction("glMapNamedBufferRange", success);
		_glUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC)LoadFunction("glUnmapNamedBuffer", success);
		_glFlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC)LoadFunction("glFlushMappedNamedBufferRange", success);
		_glGetNamedBufferParameteriv = (PFNGLGETNAMEDBUFFERPARAMETERIVPROC)LoadFunction("glGetNamedBufferParameteriv", success);
		_glGetNamedBufferParameteri64v = (PFNGLGETNAMEDBUFFERPARAMETERI64VPROC)LoadFunction("glGetNamedBufferParameteri64v", success);
		_glGetNamedBufferPointerv = (PFNGLGETNAMEDBUFFERPOINTERVPROC)LoadFunction("glGetNamedBufferPointerv", success);
		_glGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC)LoadFunction("glGetNamedBufferSubData", success);
		_glCreateFramebuffers = (PFNGLCREATEFRAMEBUFFERSPROC)LoadFunction("glCreateFramebuffers", success);
		_glNamedFramebufferRenderbuffer = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC)LoadFunction("glNamedFramebufferRenderbuffer", success);
		_glNamedFramebufferParameteri = (PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC)LoadFunction("glNamedFramebufferParameteri", success);
		_glNamedFramebufferTexture = (PFNGLNAMEDFRAMEBUFFERTEXTUREPROC)LoadFunction("glNamedFramebufferTexture", success);
		_glNamedFramebufferTextureLayer = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC)LoadFunction("glNamedFramebufferTextureLayer", success);
		_glNamedFramebufferDrawBuffer = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC)LoadFunction("glNamedFramebufferDrawBuffer", success);
		_glNamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC)LoadFunction("glNamedFramebufferDrawBuffers", success);
		_glNamedFramebufferReadBuffer = (PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC)LoadFunction("glNamedFramebufferReadBuffer", success);
		_glInvalidateNamedFramebufferData = (PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC)LoadFunction("glInvalidateNamedFramebufferData", success);
		_glInvalidateNamedFramebufferSubData = (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC)LoadFunction("glInvalidateNamedFramebufferSubData", success);
		_glClearNamedFramebufferiv = (PFNGLCLEARNAMEDFRAMEBUFFERIVPROC)LoadFunction("glClearNamedFramebufferiv", success);
		_glClearNamedFramebufferuiv = (PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC)LoadFunction("glClearNamedFramebufferuiv", success);
		_glClearNamedFramebufferfv = (PFNGLCLEARNAMEDFRAMEBUFFERFVPROC)LoadFunction("glClearNamedFramebufferfv", success);
		_glClearNamedFramebufferfi = (PFNGLCLEARNAMEDFRAMEBUFFERFIPROC)LoadFunction("glClearNamedFramebufferfi", success);
		_glBlitNamedFramebuffer = (PFNGLBLITNAMEDFRAMEBUFFERPROC)LoadFunction("glBlitNamedFramebuffer", success);
		_glCheckNamedFramebufferStatus = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC)LoadFunction("glCheckNamedFramebufferStatus", success);
		_glGetNamedFramebufferParameteriv = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC)LoadFunction("glGetNamedFramebufferParameteriv", success);
		_glGetNamedFramebufferAttachmentParameteriv = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC)LoadFunction("glGetNamedFramebufferAttachmentParameteriv", success);
		_glCreateRenderbuffers = (PFNGLCREATERENDERBUFFERSPROC)LoadFunction("glCreateRenderbuffers", success);
		_glNamedRenderbufferStorage = (PFNGLNAMEDRENDERBUFFERSTORAGEPROC)LoadFunction("glNamedRenderbufferStorage", success);
		_glNamedRenderbufferStorageMultisample = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC)LoadFunction("glNamedRenderbufferStorageMultisample", success);
		_glGetNamedRenderbufferParameteriv = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC)LoadFunction("glGetNamedRenderbufferParameteriv", success);
		_glCreateTextures = (PFNGLCREATETEXTURESPROC)LoadFunction("glCreateTextures", success);
		_glTextureBuffer = (PFNGLTEXTUREBUFFERPROC)LoadFunction("glTextureBuffer", success);
		_glTextureBufferRange = (PFNGLTEXTUREBUFFERRANGEPROC)LoadFunction("glTextureBufferRange", success);
		_glTextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC)LoadFunction("glTextureStorage1D", success);
		_glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)LoadFunction("glTextureStorage2D", success);
		_glTextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC)LoadFunction("glTextureStorage3D", success);
		_glTextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC)LoadFunction("glTextureStorage2DMultisample", success);
		_glTextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC)LoadFunction("glTextureStorage3DMultisample", success);
		_glTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC)LoadFunction("glTextureSubImage1D", success);
		_glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)LoadFunction("glTextureSubImage2D", success);
		_glTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC)LoadFunction("glTextureSubImage3D", success);
		_glCompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC)LoadFunction("glCompressedTextureSubImage1D", success);
		_glCompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC)LoadFunction("glCompressedTextureSubImage2D", success);
		_glCompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC)LoadFunction("glCompressedTextureSubImage3D", success);
		_glCopyTextureSubImage1D = (PFNGLCOPYTEXTURESUBIMAGE1DPROC)LoadFunction("glCopyTextureSubImage1D", success);
		_glCopyTextureSubImage2D = (PFNGLCOPYTEXTURESUBIMAGE2DPROC)LoadFunction("glCopyTextureSubImage2D", success);
		_glCopyTextureSubImage3D = (PFNGLCOPYTEXTURESUBIMAGE3DPROC)LoadFunction("glCopyTextureSubImage3D", success);
		_glTextureParameterf = (PFNGLTEXTUREPARAMETERFPROC)LoadFunction("glTextureParameterf", success);
		_glTextureParameterfv = (PFNGLTEXTUREPARAMETERFVPROC)LoadFunction("glTextureParameterfv", success);
		_glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)LoadFunction("glTextureParameteri", success);
		_glTextureParameterIiv = (PFNGLTEXTUREPARAMETERIIVPROC)LoadFunction("glTextureParameterIiv", success);
		_glTextureParameterIuiv = (PFNGLTEXTUREPARAMETERIUIVPROC)LoadFunction("glTextureParameterIuiv", success);
		_glTextureParameteriv = (PFNGLTEXTUREPARAMETERIVPROC)LoadFunction("glTextureParameteriv", success);
		_glGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC)LoadFunction("glGenerateTextureMipmap", success);
		_glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC)LoadFunction("glBindTextureUnit", success);
		_glGetTextureImage = (PFNGLGETTEXTUREIMAGEPROC)LoadFunction("glGetTextureImage", success);
		_glGetCompressedTextureImage = (PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC)LoadFunction("glGetCompressedTextureImage", success);
		_glGetTextureLevelParameterfv = (PFNGLGETTEXTURELEVELPARAMETERFVPROC)LoadFunction("glGetTextureLevelParameterfv", success);
		_glGetTextureLevelParameteriv = (PFNGLGETTEXTURELEVELPARAMETERIVPROC)LoadFunction("glGetTextureLevelParameteriv", success);
		_glGetTextureParameterfv = (PFNGLGETTEXTUREPARAMETERFVPROC)LoadFunction("glGetTextureParameterfv", success);
		_glGetTextureParameterIiv = (PFNGLGETTEXTUREPARAMETERIIVPROC)LoadFunction("glGetTextureParameterIiv", success);
		_glGetTextureParameterIuiv = (PFNGLGETTEXTUREPARAMETERIUIVPROC)LoadFunction("glGetTextureParameterIuiv", success);
		_glGetTextureParameteriv = (PFNGLGETTEXTUREPARAMETERIVPROC)LoadFunction("glGetTextureParameteriv", success);
		_glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)LoadFunction("glCreateVertexArrays", success);
		_glDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC)LoadFunction("glDisableVertexArrayAttrib", success);
		_glEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC)LoadFunction("glEnableVertexArrayAttrib", success);
		_glVertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFERPROC)LoadFunction("glVertexArrayElementBuffer", success);
		_glVertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFERPROC)LoadFunction("glVertexArrayVertexBuffer", success);
		_glVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC)LoadFunction("glVertexArrayVertexBuffers", success);
		_glVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDINGPROC)LoadFunction("glVertexArrayAttribBinding", success);
		_glVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC)LoadFunction("glVertexArrayAttribFormat", success);
		_glVertexArrayAttribIFormat = (PFNGLVERTEXARRAYATTRIBIFORMATPROC)LoadFunction("glVertexArrayAttribIFormat", success);
		_glVertexArrayAttribLFormat = (PFNGLVERTEXARRAYATTRIBLFORMATPROC)LoadFunction("glVertexArrayAttribLFormat", success);
		_glVertexArrayBindingDivisor = (PFNGLVERTEXARRAYBINDINGDIVISORPROC)LoadFunction("glVertexArrayBindingDivisor", success);
		_glGetVertexArrayiv = (PFNGLGETVERTEXARRAYIVPROC)LoadFunction("glGetVertexArrayiv", success);
		_glGetVertexArrayIndexediv = (PFNGLGETVERTEXARRAYINDEXEDIVPROC)LoadFunction("glGetVertexArrayIndexediv", success);
		_glGetVertexArrayIndexed64iv = (PFNGLGETVERTEXARRAYINDEXED64IVPROC)LoadFunction("glGetVertexArrayIndexed64iv", success);
		_glCreateSamplers = (PFNGLCREATESAMPLERSPROC)LoadFunction("glCreateSamplers", success);
		_glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)LoadFunction("glCreateProgramPipelines", success);
		_glCreateQueries = (PFNGLCREATEQUERIESPROC)LoadFunction("glCreateQueries", success);
		_glGetQueryBufferObjecti64v = (PFNGLGETQUERYBUFFEROBJECTI64VPROC)LoadFunction("glGetQueryBufferObjecti64v", success);
		_glGetQueryBufferObjectiv = (PFNGLGETQUERYBUFFEROBJECTIVPROC)LoadFunction("glGetQueryBufferObjectiv", success);
		_glGetQueryBufferObjectui64v = (PFNGLGETQUERYBUFFEROBJECTUI64VPROC)LoadFunction("glGetQueryBufferObjectui64v", success);
		_glGetQueryBufferObjectuiv = (PFNGLGETQUERYBUFFEROBJECTUIVPROC)LoadFunction("glGetQueryBufferObjectuiv", success);
		_glMemoryBarrierByRegion = (PFNGLMEMORYBARRIERBYREGIONPROC)LoadFunction("glMemoryBarrierByRegion", success);
		_glGetTextureSubImage = (PFNGLGETTEXTURESUBIMAGEPROC)LoadFunction("glGetTextureSubImage", success);
		_glGetCompressedTextureSubImage = (PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC)LoadFunction("glGetCompressedTextureSubImage", success);
		_glGetGraphicsResetStatus = (PFNGLGETGRAPHICSRESETSTATUSPROC)LoadFunction("glGetGraphicsResetStatus", success);
		_glGetnCompressedTexImage = (PFNGLGETNCOMPRESSEDTEXIMAGEPROC)LoadFunction("glGetnCompressedTexImage", success);
		_glGetnTexImage = (PFNGLGETNTEXIMAGEPROC)LoadFunction("glGetnTexImage", success);
		_glGetnUniformdv = (PFNGLGETNUNIFORMDVPROC)LoadFunction("glGetnUniformdv", success);
		_glGetnUniformfv = (PFNGLGETNUNIFORMFVPROC)LoadFunction("glGetnUniformfv", success);
		_glGetnUniformiv = (PFNGLGETNUNIFORMIVPROC)LoadFunction("glGetnUniformiv", success);
		_glGetnUniformuiv = (PFNGLGETNUNIFORMUIVPROC)LoadFunction("glGetnUniformuiv", success);
		_glReadnPixels = (PFNGLREADNPIXELSPROC)LoadFunction("glReadnPixels", success);
		_glGetnMapdv = (PFNGLGETNMAPDVPROC)LoadFunction("glGetnMapdv", success);
		_glGetnMapfv = (PFNGLGETNMAPFVPROC)LoadFunction("glGetnMapfv", success);
		_glGetnMapiv = (PFNGLGETNMAPIVPROC)LoadFunction("glGetnMapiv", success);
		_glGetnPixelMapfv = (PFNGLGETNPIXELMAPFVPROC)LoadFunction("glGetnPixelMapfv", success);
		_glGetnPixelMapuiv = (PFNGLGETNPIXELMAPUIVPROC)LoadFunction("glGetnPixelMapuiv", success);
		_glGetnPixelMapusv = (PFNGLGETNPIXELMAPUSVPROC)LoadFunction("glGetnPixelMapusv", success);
		_glGetnPolygonStipple = (PFNGLGETNPOLYGONSTIPPLEPROC)LoadFunction("glGetnPolygonStipple", success);
		_glGetnColorTable = (PFNGLGETNCOLORTABLEPROC)LoadFunction("glGetnColorTable", success);
		_glGetnConvolutionFilter = (PFNGLGETNCONVOLUTIONFILTERPROC)LoadFunction("glGetnConvolutionFilter", success);
		_glGetnSeparableFilter = (PFNGLGETNSEPARABLEFILTERPROC)LoadFunction("glGetnSeparableFilter", success);
		_glGetnHistogram = (PFNGLGETNHISTOGRAMPROC)LoadFunction("glGetnHistogram", success);
		_glGetnMinmax = (PFNGLGETNMINMAXPROC)LoadFunction("glGetnMinmax", success);
		_glTextureBarrier = (PFNGLTEXTUREBARRIERPROC)LoadFunction("glTextureBarrier", success);

		return success;
	}

	bool OpenGLLoader::LoadOpenGL_4_6()
	{
		bool success = true;

		_glSpecializeShader = (PFNGLSPECIALIZESHADERPROC)LoadFunction("glSpecializeShader", success);
		_glMultiDrawArraysIndirectCount = (PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC)LoadFunction("glMultiDrawArraysIndirectCount", success);
		_glMultiDrawElementsIndirectCount = (PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC)LoadFunction("glMultiDrawElementsIndirectCount", success);
		_glPolygonOffsetClamp = (PFNGLPOLYGONOFFSETCLAMPPROC)LoadFunction("glPolygonOffsetClamp", success);

		return success;
	}
}
