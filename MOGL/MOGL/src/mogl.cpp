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

#pragma region WGL extensions
PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB = nullptr;
PFNWGLCHOOSEPIXELFOTMATARBPROC _wglChoosePixelFormatARB = nullptr;
#pragma endregion

namespace mogl
{
	HMODULE OpenGLLoader::s_OpenGLLib;

	bool OpenGLLoader::LoadOpenGL(OpenGLVersion version)
	{
		// TODO: Return error message if failed to load functions
		int intVersion = (int)version;

		if (intVersion < 0 || intVersion >= (int)OpenGLVersion::OPENGL_VERSION_COUNT)
			return false; // TODO: Log an error

		s_OpenGLLib = LoadLibrary(L"opengl32.dll");

		if (!s_OpenGLLib)
			return false; // TODO: Log an error

		if (intVersion >= (int)OpenGLVersion::OPENGL_1_0)
			LoadOpenGL_1_0();
		if (intVersion >= (int)OpenGLVersion::OPENGL_1_1)
			LoadOpenGL_1_1();
		if (intVersion >= (int)OpenGLVersion::OPENGL_1_2)
			LoadOpenGL_1_2();
		if (intVersion >= (int)OpenGLVersion::OPENGL_1_3)
			LoadOpenGL_1_3();
		if (intVersion >= (int)OpenGLVersion::OPENGL_1_4)
			LoadOpenGL_1_4();
		if (intVersion >= (int)OpenGLVersion::OPENGL_1_5)
			LoadOpenGL_1_5();
		if (intVersion >= (int)OpenGLVersion::OPENGL_2_0)
			LoadOpenGL_2_0();
		if (intVersion >= (int)OpenGLVersion::OPENGL_2_1)
			LoadOpenGL_2_1();
		if (intVersion >= (int)OpenGLVersion::OPENGL_3_0)
			LoadOpenGL_3_0();
		if (intVersion >= (int)OpenGLVersion::OPENGL_3_1)
			LoadOpenGL_3_1();
		if (intVersion >= (int)OpenGLVersion::OPENGL_3_2)
			LoadOpenGL_3_2();
		if (intVersion >= (int)OpenGLVersion::OPENGL_3_3)
			LoadOpenGL_3_3();

		FreeLibrary(s_OpenGLLib);

		return true;
	}

	void* OpenGLLoader::LoadFunction(const char* name)
	{
		void* result = GetProcAddress(s_OpenGLLib, name);
		if (!result)
			result = wglGetProcAddress(name);

		return result;
	}

	void OpenGLLoader::LoadOpenGL_1_0()
	{
		_glCullFace = (PFNGLCULLFACEPROC)LoadFunction("glCullFace");
		_glFrontFace = (PFNGLFRONTFACEPROC)LoadFunction("glFrontFace");
		_glHint = (PFNGLHINTPROC)LoadFunction("glHint");
		_glLineWidth = (PFNGLLINEWIDTHPROC)LoadFunction("glLineWidth");
		_glPointSize = (PFNGLPOINTSIZEPROC)LoadFunction("glPointSize");
		_glPolygonMode = (PFNGLPOLYGONMODEPROC)LoadFunction("glPolygonMode");
		_glScissor = (PFNGLSCISSORPROC)LoadFunction("glScissor");
		_glTexParameterf = (PFNGLTEXPARAMETERFPROC)LoadFunction("glTexParameterf");
		_glTexParameterfv = (PFNGLTEXPARAMETERFVPROC)LoadFunction("glTexParameterfv");
		_glTexParameteri = (PFNGLTEXPARAMETERIPROC)LoadFunction("glTexParameteri");
		_glTexParameteriv = (PFNGLTEXPARAMETERIVPROC)LoadFunction("glTexParameteriv");
		_glTexImage1D = (PFNGLTEXIMAGE1DPROC)LoadFunction("glTexImage1D");
		_glTexImage2D = (PFNGLTEXIMAGE2DPROC)LoadFunction("glTexImage2D");
		_glDrawBuffer = (PFNGLDRAWBUFFERPROC)LoadFunction("glDrawBuffer");
		_glClear = (PFNGLCLEARPROC)LoadFunction("glClear");
		_glClearColor = (PFNGLCLEARCOLORPROC)LoadFunction("glClearColor");
		_glClearStencil = (PFNGLCLEARSTENCILPROC)LoadFunction("glClearStencil");
		_glClearDepth = (PFNGLCLEARDEPTHPROC)LoadFunction("glClearDepth");
		_glStencilMask = (PFNGLSTENCILMASKPROC)LoadFunction("glStencilMask");
		_glColorMask = (PFNGLCOLORMASKPROC)LoadFunction("glColorMask");
		_glDepthMask = (PFNGLDEPTHMASKPROC)LoadFunction("glDepthMask");
		_glDisable = (PFNGLDISABLEPROC)LoadFunction("glDisable");
		_glEnable = (PFNGLENABLEPROC)LoadFunction("glEnable");
		_glFinish = (PFNGLFINISHPROC)LoadFunction("glFinish");
		_glFlush = (PFNGLFLUSHPROC)LoadFunction("glFlush");
		_glBlendFunc = (PFNGLBLENDFUNCPROC)LoadFunction("glBlendFunc");
		_glLogicOp = (PFNGLLOGICOPPROC)LoadFunction("glLogicOp");
		_glStencilFunc = (PFNGLSTENCILFUNCPROC)LoadFunction("glStencilFunc");
		_glStencilOp = (PFNGLSTENCILOPPROC)LoadFunction("glStencilOp");
		_glDepthFunc = (PFNGLDEPTHFUNCPROC)LoadFunction("glDepthFunc");
		_glPixelStoref = (PFNGLPIXELSTOREFPROC)LoadFunction("glPixelStoref");
		_glPixelStorei = (PFNGLPIXELSTOREIPROC)LoadFunction("glPixelStorei");
		_glReadBuffer = (PFNGLREADBUFFERPROC)LoadFunction("glReadBuffer");
		_glReadPixels = (PFNGLREADPIXELSPROC)LoadFunction("glReadPixels");
		_glGetBooleanv = (PFNGLGETBOOLEANVPROC)LoadFunction("glGetBooleanv");
		_glGetDoublev = (PFNGLGETDOUBLEVPROC)LoadFunction("glGetDoublev");
		_glGetError = (PFNGLGETERRORPROC)LoadFunction("glGetError");
		_glGetFloatv = (PFNGLGETFLOATVPROC)LoadFunction("glGetFloatv");
		_glGetIntegerv = (PFNGLGETINTEGERVPROC)LoadFunction("glGetIntegerv");
		_glGetString = (PFNGLGETSTRINGPROC)LoadFunction("glGetString");
		_glGetTexImage = (PFNGLGETTEXIMAGEPROC)LoadFunction("glGetTexImage");
		_glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)LoadFunction("glGetTexParameterfv");
		_glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)LoadFunction("glGetTexParameteriv");
		_glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC)LoadFunction("glGetTexLevelParameterfv");
		_glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC)LoadFunction("glGetTexLevelParameteriv");
		_glIsEnabled = (PFNGLISENABLEDPROC)LoadFunction("glIsEnabled");
		_glDepthRange = (PFNGLDEPTHRANGEPROC)LoadFunction("glDepthRange");
		_glViewport = (PFNGLVIEWPORTPROC)LoadFunction("glViewport");
	}

	void OpenGLLoader::LoadOpenGL_1_1()
	{
		_glDrawArrays = (PFNGLDRAWARRAYSPROC)LoadFunction("glDrawArrays");
		_glDrawElements = (PFNGLDRAWELEMENTSPROC)LoadFunction("glDrawElements");
		_glPolygonOffset = (PFNGLPOLYGONOFFSETPROC)LoadFunction("glPolygonOffset");
		_glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC)LoadFunction("glCopyTexImage1D");
		_glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)LoadFunction("glCopyTexImage2D");
		_glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC)LoadFunction("glCopyTexSubImage1D");
		_glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)LoadFunction("glCopyTexSubImage2D");
		_glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC)LoadFunction("glTexSubImage1D");
		_glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)LoadFunction("glTexSubImage2D");
		_glBindTexture = (PFNGLBINDTEXTUREPROC)LoadFunction("glBindTexture");
		_glDeleteTextures = (PFNGLDELETETEXTURESPROC)LoadFunction("glDeleteTextures");
		_glGenTextures = (PFNGLGENTEXTURESPROC)LoadFunction("glGenTextures");
		_glIsTexture = (PFNGLISTEXTUREPROC)LoadFunction("glIsTexture");
	}

	void OpenGLLoader::LoadOpenGL_1_2()
	{
		_glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)LoadFunction("glDrawRangeElements");
		_glTexImage3D = (PFNGLTEXIMAGE3DPROC)LoadFunction("glTexImage3D");
		_glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)LoadFunction("glTexSubImage3D");
		_glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)LoadFunction("glCopyTexSubImage3D");
	}

	void OpenGLLoader::LoadOpenGL_1_3()
	{
		_glActiveTexture = (PFNGLACTIVETEXTUREPROC)LoadFunction("glActiveTexture");
		_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)LoadFunction("glSampleCoverage");
		_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)LoadFunction("glCompressedTexImage3D");
		_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)LoadFunction("glCompressedTexImage2D");
		_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)LoadFunction("glCompressedTexImage1D");
		_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)LoadFunction("glCompressedTexSubImage3D");
		_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)LoadFunction("glCompressedTexSubImage2D");
		_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)LoadFunction("glCompressedTexSubImage1D");
		_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)LoadFunction("glGetCompressedTexImage");
	}

	void OpenGLLoader::LoadOpenGL_1_4()
	{
		_glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)LoadFunction("glBlendFuncSeparate");
		_glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)LoadFunction("glMultiDrawArrays");
		_glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)LoadFunction("glMultiDrawElements");
		_glPointParameterf = (PFNGLPOINTPARAMETERFPROC)LoadFunction("glPointParameterf");
		_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)LoadFunction("glPointParameterfv");
		_glPointParameteri = (PFNGLPOINTPARAMETERIPROC)LoadFunction("glPointParameteri");
		_glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)LoadFunction("glPointParameteriv");
		_glBlendColor = (PFNGLBLENDCOLORPROC)LoadFunction("glBlendColor");
		_glBlendEquation = (PFNGLBLENDEQUATIONPROC)LoadFunction("glBlendEquation");
	}

	void OpenGLLoader::LoadOpenGL_1_5()
	{
		_glGenQueries = (PFNGLGENQUERIESPROC)LoadFunction("glGenQueries");
		_glDeleteQueries = (PFNGLDELETEQUERIESPROC)LoadFunction("glDeleteQueries");
		_glIsQuery = (PFNGLISQUERYPROC)LoadFunction("glIsQuery");
		_glBeginQuery = (PFNGLBEGINQUERYPROC)LoadFunction("glBeginQuery");
		_glEndQuery = (PFNGLENDQUERYPROC)LoadFunction("glEndQuery");
		_glGetQueryiv = (PFNGLGETQUERYIVPROC)LoadFunction("glGetQueryiv");
		_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)LoadFunction("glGetQueryObjectiv");
		_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)LoadFunction("glGetQueryObjectuiv");
		_glBindBuffer = (PFNGLBINDBUFFERPROC)LoadFunction("glBindBuffer");
		_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)LoadFunction("glDeleteBuffers");
		_glGenBuffers = (PFNGLGENBUFFERSPROC)LoadFunction("glGenBuffers");
		_glIsBuffer = (PFNGLISBUFFERPROC)LoadFunction("glIsBuffer");
		_glBufferData = (PFNGLBUFFERDATAPROC)LoadFunction("glBufferData");
		_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)LoadFunction("glBufferSubData");
		_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)LoadFunction("glGetBufferSubData");
		_glMapBuffer = (PFNGLMAPBUFFERPROC)LoadFunction("glMapBuffer");
		_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)LoadFunction("glUnmapBuffer");
		_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)LoadFunction("glGetBufferParameteriv");
		_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)LoadFunction("glGetBufferPointerv");
	}

	void OpenGLLoader::LoadOpenGL_2_0()
	{
		_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)LoadFunction("glBlendEquationSeparate");
		_glDrawBuffers = (PFNGLDRAWBUFFERSPROC)LoadFunction("glDrawBuffers");
		_glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)LoadFunction("glStencilOpSeparate");
		_glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)LoadFunction("glStencilFuncSeparate");
		_glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)LoadFunction("glStencilMaskSeparate");
		_glAttachShader = (PFNGLATTACHSHADERPROC)LoadFunction("glAttachShader");
		_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)LoadFunction("glBindAttribLocation");
		_glCompileShader = (PFNGLCOMPILESHADERPROC)LoadFunction("glCompileShader");
		_glCreateProgram = (PFNGLCREATEPROGRAMPROC)LoadFunction("glCreateProgram");
		_glCreateShader = (PFNGLCREATESHADERPROC)LoadFunction("glCreateShader");
		_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)LoadFunction("glDeleteProgram");
		_glDeleteShader = (PFNGLDELETESHADERPROC)LoadFunction("glDeleteShader");
		_glDetachShader = (PFNGLDETACHSHADERPROC)LoadFunction("glDetachShader");
		_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)LoadFunction("glDisableVertexAttribArray");
		_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)LoadFunction("glEnableVertexAttribArray");
		_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)LoadFunction("glGetActiveAttrib");
		_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)LoadFunction("glGetActiveUniform");
		_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)LoadFunction("glGetAttachedShaders");
		_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)LoadFunction("glGetAttribLocation");
		_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)LoadFunction("glGetProgramiv");
		_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)LoadFunction("glGetProgramInfoLog");
		_glGetShaderiv = (PFNGLGETSHADERIVPROC)LoadFunction("glGetShaderiv");
		_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)LoadFunction("glGetShaderInfoLog");
		_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)LoadFunction("glGetShaderSource");
		_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)LoadFunction("glGetUniformLocation");
		_glGetUniformfv = (PFNGLGETUNIFORMFVPROC)LoadFunction("glGetUniformfv");
		_glGetUniformiv = (PFNGLGETUNIFORMIVPROC)LoadFunction("glGetUniformiv");
		_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)LoadFunction("glGetVertexAttribdv");
		_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)LoadFunction("glGetVertexAttribfv");
		_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)LoadFunction("glGetVertexAttribiv");
		_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)LoadFunction("glGetVertexAttribPointerv");
		_glIsProgram = (PFNGLISPROGRAMPROC)LoadFunction("glIsProgram");
		_glIsShader = (PFNGLISSHADERPROC)LoadFunction("glIsShader");
		_glLinkProgram = (PFNGLLINKPROGRAMPROC)LoadFunction("glLinkProgram");
		_glShaderSource = (PFNGLSHADERSOURCEPROC)LoadFunction("glShaderSource");
		_glUseProgram = (PFNGLUSEPROGRAMPROC)LoadFunction("glUseProgram");
		_glUniform1f = (PFNGLUNIFORM1FPROC)LoadFunction("glUniform1f");
		_glUniform2f = (PFNGLUNIFORM2FPROC)LoadFunction("glUniform2f");
		_glUniform3f = (PFNGLUNIFORM3FPROC)LoadFunction("glUniform3f");
		_glUniform4f = (PFNGLUNIFORM4FPROC)LoadFunction("glUniform4f");
		_glUniform1i = (PFNGLUNIFORM1IPROC)LoadFunction("glUniform1i");
		_glUniform2i = (PFNGLUNIFORM2IPROC)LoadFunction("glUniform2i");
		_glUniform3i = (PFNGLUNIFORM3IPROC)LoadFunction("glUniform3i");
		_glUniform4i = (PFNGLUNIFORM4IPROC)LoadFunction("glUniform4i");
		_glUniform1fv = (PFNGLUNIFORM1FVPROC)LoadFunction("glUniform1fv");
		_glUniform2fv = (PFNGLUNIFORM2FVPROC)LoadFunction("glUniform2fv");
		_glUniform3fv = (PFNGLUNIFORM3FVPROC)LoadFunction("glUniform3fv");
		_glUniform4fv = (PFNGLUNIFORM4FVPROC)LoadFunction("glUniform4fv");
		_glUniform1iv = (PFNGLUNIFORM1IVPROC)LoadFunction("glUniform1iv");
		_glUniform2iv = (PFNGLUNIFORM2IVPROC)LoadFunction("glUniform2iv");
		_glUniform3iv = (PFNGLUNIFORM3IVPROC)LoadFunction("glUniform3iv");
		_glUniform4iv = (PFNGLUNIFORM4IVPROC)LoadFunction("glUniform4iv");
		_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)LoadFunction("glUniformMatrix2fv");
		_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)LoadFunction("glUniformMatrix3fv");
		_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)LoadFunction("glUniformMatrix4fv");
		_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)LoadFunction("glValidateProgram");
		_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)LoadFunction("glVertexAttrib1d");
		_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)LoadFunction("glVertexAttrib1dv");
		_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)LoadFunction("glVertexAttrib1f");
		_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)LoadFunction("glVertexAttrib1fv");
		_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)LoadFunction("glVertexAttrib1s");
		_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)LoadFunction("glVertexAttrib1sv");
		_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)LoadFunction("glVertexAttrib2d");
		_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)LoadFunction("glVertexAttrib2dv");
		_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)LoadFunction("glVertexAttrib2f");
		_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)LoadFunction("glVertexAttrib2fv");
		_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)LoadFunction("glVertexAttrib2s");
		_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)LoadFunction("glVertexAttrib2sv");
		_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)LoadFunction("glVertexAttrib3d");
		_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)LoadFunction("glVertexAttrib3dv");
		_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)LoadFunction("glVertexAttrib3f");
		_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)LoadFunction("glVertexAttrib3fv");
		_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)LoadFunction("glVertexAttrib3s");
		_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)LoadFunction("glVertexAttrib3sv");
		_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)LoadFunction("glVertexAttrib4Nbv");
		_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)LoadFunction("glVertexAttrib4Niv");
		_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)LoadFunction("glVertexAttrib4Nsv");
		_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)LoadFunction("glVertexAttrib4Nub");
		_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)LoadFunction("glVertexAttrib4Nubv");
		_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)LoadFunction("glVertexAttrib4Nuiv");
		_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)LoadFunction("glVertexAttrib4Nusv");
		_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)LoadFunction("glVertexAttrib4bv");
		_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)LoadFunction("glVertexAttrib4d");
		_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)LoadFunction("glVertexAttrib4dv");
		_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)LoadFunction("glVertexAttrib4f");
		_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)LoadFunction("glVertexAttrib4fv");
		_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)LoadFunction("glVertexAttrib4iv");
		_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)LoadFunction("glVertexAttrib4s");
		_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)LoadFunction("glVertexAttrib4sv");
		_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)LoadFunction("glVertexAttrib4ubv");
		_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)LoadFunction("glVertexAttrib4uiv");
		_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)LoadFunction("glVertexAttrib4usv");
		_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)LoadFunction("glVertexAttribPointer");
	}

	void OpenGLLoader::LoadOpenGL_2_1()
	{
		_glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)LoadFunction("glUniformMatrix2x3fv");
		_glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)LoadFunction("glUniformMatrix3x2fv");
		_glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)LoadFunction("glUniformMatrix2x4fv");
		_glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)LoadFunction("glUniformMatrix4x2fv");
		_glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)LoadFunction("glUniformMatrix3x4fv");
		_glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)LoadFunction("glUniformMatrix4x3fv");
	}

	void OpenGLLoader::LoadOpenGL_3_0()
	{
		_glColorMaski = (PFNGLCOLORMASKIPROC)LoadFunction("glColorMaski");
		_glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)LoadFunction("glGetBooleani_v");
		_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)LoadFunction("glGetIntegeri_v");
		_glEnablei = (PFNGLENABLEIPROC)LoadFunction("glEnablei");
		_glDisablei = (PFNGLDISABLEIPROC)LoadFunction("glDisablei");
		_glIsEnabledi = (PFNGLISENABLEDIPROC)LoadFunction("glIsEnabledi");
		_glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)LoadFunction("glBeginTransformFeedback");
		_glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)LoadFunction("glEndTransformFeedback");
		_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)LoadFunction("glBindBufferRange");
		_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)LoadFunction("glBindBufferBase");
		_glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)LoadFunction("glTransformFeedbackVaryings");
		_glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)LoadFunction("glGetTransformFeedbackVarying");
		_glClampColor = (PFNGLCLAMPCOLORPROC)LoadFunction("glClampColor");
		_glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)LoadFunction("glBeginConditionalRender");
		_glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)LoadFunction("glEndConditionalRender");
		_glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)LoadFunction("glVertexAttribIPointer");
		_glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)LoadFunction("glGetVertexAttribIiv");
		_glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)LoadFunction("glGetVertexAttribIuiv");
		_glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)LoadFunction("glVertexAttribI1i");
		_glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)LoadFunction("glVertexAttribI2i");
		_glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)LoadFunction("glVertexAttribI3i");
		_glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)LoadFunction("glVertexAttribI4i");
		_glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)LoadFunction("glVertexAttribI1ui");
		_glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)LoadFunction("glVertexAttribI2ui");
		_glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)LoadFunction("glVertexAttribI3ui");
		_glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)LoadFunction("glVertexAttribI4ui");
		_glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)LoadFunction("glVertexAttribI1iv");
		_glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)LoadFunction("glVertexAttribI2iv");
		_glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)LoadFunction("glVertexAttribI3iv");
		_glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)LoadFunction("glVertexAttribI4iv");
		_glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)LoadFunction("glVertexAttribI1uiv");
		_glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)LoadFunction("glVertexAttribI2uiv");
		_glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)LoadFunction("glVertexAttribI3uiv");
		_glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)LoadFunction("glVertexAttribI4uiv");
		_glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)LoadFunction("glVertexAttribI4bv");
		_glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)LoadFunction("glVertexAttribI4sv");
		_glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)LoadFunction("glVertexAttribI4ubv");
		_glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)LoadFunction("glVertexAttribI4usv");
		_glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)LoadFunction("glGetUniformuiv");
		_glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)LoadFunction("glBindFragDataLocation");
		_glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)LoadFunction("glGetFragDataLocation");
		_glUniform1ui = (PFNGLUNIFORM1UIPROC)LoadFunction("glUniform1ui");
		_glUniform2ui = (PFNGLUNIFORM2UIPROC)LoadFunction("glUniform2ui");
		_glUniform3ui = (PFNGLUNIFORM3UIPROC)LoadFunction("glUniform3ui");
		_glUniform4ui = (PFNGLUNIFORM4UIPROC)LoadFunction("glUniform4ui");
		_glUniform1uiv = (PFNGLUNIFORM1UIVPROC)LoadFunction("glUniform1uiv");
		_glUniform2uiv = (PFNGLUNIFORM2UIVPROC)LoadFunction("glUniform2uiv");
		_glUniform3uiv = (PFNGLUNIFORM3UIVPROC)LoadFunction("glUniform3uiv");
		_glUniform4uiv = (PFNGLUNIFORM4UIVPROC)LoadFunction("glUniform4uiv");
		_glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)LoadFunction("glTexParameterIiv");
		_glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)LoadFunction("glTexParameterIuiv");
		_glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)LoadFunction("glGetTexParameterIiv");
		_glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)LoadFunction("glGetTexParameterIuiv");
		_glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)LoadFunction("glClearBufferiv");
		_glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)LoadFunction("glClearBufferuiv");
		_glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)LoadFunction("glClearBufferfv");
		_glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)LoadFunction("glClearBufferfi");
		_glGetStringi = (PFNGLGETSTRINGIPROC)LoadFunction("glGetStringi");
		_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)LoadFunction("glIsRenderbuffer");
		_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)LoadFunction("glBindRenderbuffer");
		_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)LoadFunction("glDeleteRenderbuffers");
		_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)LoadFunction("glGenRenderbuffers");
		_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)LoadFunction("glRenderbufferStorage");
		_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)LoadFunction("glGetRenderbufferParameteriv");
		_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)LoadFunction("glIsFramebuffer");
		_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)LoadFunction("glBindFramebuffer");
		_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)LoadFunction("glDeleteFramebuffers");
		_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)LoadFunction("glGenFramebuffers");
		_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)LoadFunction("glCheckFramebufferStatus");
		_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)LoadFunction("glFramebufferTexture1D");
		_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)LoadFunction("glFramebufferTexture2D");
		_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)LoadFunction("glFramebufferTexture3D");
		_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)LoadFunction("glFramebufferRenderbuffer");
		_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)LoadFunction("glGetFramebufferAttachmentParameteriv");
		_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)LoadFunction("glGenerateMipmap");
		_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)LoadFunction("glBlitFramebuffer");
		_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)LoadFunction("glRenderbufferStorageMultisample");
		_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)LoadFunction("glFramebufferTextureLayer");
		_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)LoadFunction("glMapBufferRange");
		_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)LoadFunction("glFlushMappedBufferRange");
		_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)LoadFunction("glBindVertexArray");
		_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)LoadFunction("glDeleteVertexArrays");
		_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)LoadFunction("glGenVertexArrays");
		_glIsVertexArray = (PFNGLISVERTEXARRAYPROC)LoadFunction("glIsVertexArray");
	}

	void OpenGLLoader::LoadOpenGL_3_1()
	{
		_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)LoadFunction("glDrawArraysInstanced");
		_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)LoadFunction("glDrawElementsInstanced");
		_glTexBuffer = (PFNGLTEXBUFFERPROC)LoadFunction("glTexBuffer");
		_glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)LoadFunction("glPrimitiveRestartIndex");
		_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)LoadFunction("glCopyBufferSubData");
		_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)LoadFunction("glGetUniformIndices");
		_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)LoadFunction("glGetActiveUniformsiv");
		_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)LoadFunction("glGetActiveUniformName");
		_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)LoadFunction("glGetUniformBlockIndex");
		_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)LoadFunction("glGetActiveUniformBlockiv");
		_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)LoadFunction("glGetActiveUniformBlockName");
		_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)LoadFunction("glUniformBlockBinding");
	}

	void OpenGLLoader::LoadOpenGL_3_2()
	{
		_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)LoadFunction("glDrawElementsBaseVertex");
		_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)LoadFunction("glDrawRangeElementsBaseVertex");
		_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)LoadFunction("glDrawElementsInstancedBaseVertex");
		_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)LoadFunction("glMultiDrawElementsBaseVertex");
		_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)LoadFunction("glProvokingVertex");
		_glFenceSync = (PFNGLFENCESYNCPROC)LoadFunction("glFenceSync");
		_glIsSync = (PFNGLISSYNCPROC)LoadFunction("glIsSync");
		_glDeleteSync = (PFNGLDELETESYNCPROC)LoadFunction("glDeleteSync");
		_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)LoadFunction("glClientWaitSync");
		_glWaitSync = (PFNGLWAITSYNCPROC)LoadFunction("glWaitSync");
		_glGetInteger64v = (PFNGLGETINTEGER64VPROC)LoadFunction("glGetInteger64v");
		_glGetSynciv = (PFNGLGETSYNCIVPROC)LoadFunction("glGetSynciv");
		_glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)LoadFunction("glGetInteger64i_v");
		_glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)LoadFunction("glGetBufferParameteri64v");
		_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)LoadFunction("glFramebufferTexture");
		_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)LoadFunction("glTexImage2DMultisample");
		_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)LoadFunction("glTexImage3DMultisample");
		_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)LoadFunction("glGetMultisamplefv");
		_glSampleMaski = (PFNGLSAMPLEMASKIPROC)LoadFunction("glSampleMaski");
	}

	void OpenGLLoader::LoadOpenGL_3_3()
	{
		_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)LoadFunction("glBindFragDataLocationIndexed");
		_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)LoadFunction("glGetFragDataIndex");
		_glGenSamplers = (PFNGLGENSAMPLERSPROC)LoadFunction("glGenSamplers");
		_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)LoadFunction("glDeleteSamplers");
		_glIsSampler = (PFNGLISSAMPLERPROC)LoadFunction("glIsSampler");
		_glBindSampler = (PFNGLBINDSAMPLERPROC)LoadFunction("glBindSampler");
		_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)LoadFunction("glSamplerParameteri");
		_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)LoadFunction("glSamplerParameteriv");
		_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)LoadFunction("glSamplerParameterf");
		_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)LoadFunction("glSamplerParameterfv");
		_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)LoadFunction("glSamplerParameterIiv");
		_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)LoadFunction("glSamplerParameterIuiv");
		_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)LoadFunction("glGetSamplerParameteriv");
		_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)LoadFunction("glGetSamplerParameterIiv");
		_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)LoadFunction("glGetSamplerParameterfv");
		_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)LoadFunction("glGetSamplerParameterIuiv");
		_glQueryCounter = (PFNGLQUERYCOUNTERPROC)LoadFunction("glQueryCounter");
		_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)LoadFunction("glGetQueryObjecti64v");
		_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)LoadFunction("glGetQueryObjectui64v");
		_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)LoadFunction("glVertexAttribDivisor");
		_glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)LoadFunction("glVertexAttribP1ui");
		_glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)LoadFunction("glVertexAttribP1uiv");
		_glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)LoadFunction("glVertexAttribP2ui");
		_glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)LoadFunction("glVertexAttribP2uiv");
		_glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)LoadFunction("glVertexAttribP3ui");
		_glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)LoadFunction("glVertexAttribP3uiv");
		_glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)LoadFunction("glVertexAttribP4ui");
		_glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)LoadFunction("glVertexAttribP4uiv");
		_glVertexP2ui = (PFNGLVERTEXP2UIPROC)LoadFunction("glVertexP2ui");
		_glVertexP2uiv = (PFNGLVERTEXP2UIVPROC)LoadFunction("glVertexP2uiv");
		_glVertexP3ui = (PFNGLVERTEXP3UIPROC)LoadFunction("glVertexP3ui");
		_glVertexP3uiv = (PFNGLVERTEXP3UIVPROC)LoadFunction("glVertexP3uiv");
		_glVertexP4ui = (PFNGLVERTEXP4UIPROC)LoadFunction("glVertexP4ui");
		_glVertexP4uiv = (PFNGLVERTEXP4UIVPROC)LoadFunction("glVertexP4uiv");
		_glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC)LoadFunction("glTexCoordP1ui");
		_glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC)LoadFunction("glTexCoordP1uiv");
		_glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC)LoadFunction("glTexCoordP2ui");
		_glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC)LoadFunction("glTexCoordP2uiv");
		_glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC)LoadFunction("glTexCoordP3ui");
		_glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC)LoadFunction("glTexCoordP3uiv");
		_glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC)LoadFunction("glTexCoordP4ui");
		_glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC)LoadFunction("glTexCoordP4uiv");
		_glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC)LoadFunction("glMultiTexCoordP1ui");
		_glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC)LoadFunction("glMultiTexCoordP1uiv");
		_glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC)LoadFunction("glMultiTexCoordP2ui");
		_glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC)LoadFunction("glMultiTexCoordP2uiv");
		_glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC)LoadFunction("glMultiTexCoordP3ui");
		_glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC)LoadFunction("glMultiTexCoordP3uiv");
		_glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC)LoadFunction("glMultiTexCoordP4ui");
		_glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC)LoadFunction("glMultiTexCoordP4uiv");
		_glNormalP3ui = (PFNGLNORMALP3UIPROC)LoadFunction("glNormalP3ui");
		_glNormalP3uiv = (PFNGLNORMALP3UIVPROC)LoadFunction("glNormalP3uiv");
		_glColorP3ui = (PFNGLCOLORP3UIPROC)LoadFunction("glColorP3ui");
		_glColorP3uiv = (PFNGLCOLORP3UIVPROC)LoadFunction("glColorP3uiv");
		_glColorP4ui = (PFNGLCOLORP4UIPROC)LoadFunction("glColorP4ui");
		_glColorP4uiv = (PFNGLCOLORP4UIVPROC)LoadFunction("glColorP4uiv");
		_glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC)LoadFunction("glSecondaryColorP3ui");
		_glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC)LoadFunction("glSecondaryColorP3uiv");
	}
}
