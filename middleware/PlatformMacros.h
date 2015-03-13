#pragma once


// namespace jmw {}
#ifdef __cplusplus
#define NS_JMW_BEGIN                     namespace jmw {
#define NS_JMW_END                       }
#define USING_NS_JMW                     using namespace jmw
#else
#define NS_JML_BEGIN
#define NS_JML_END
#define USING_NS_JML
#endif




typedef			double					f64;
typedef			float					f32;
typedef			unsigned long long		u64;
typedef			signed long long		s64;
typedef         unsigned long           ul32;
typedef         signed long             sl32;
typedef			unsigned int			u32;
typedef			signed int				s32;
typedef			unsigned short			u16;
typedef			signed short			s16;
typedef			unsigned char			u8;
typedef			signed char				s8;
typedef			const char *			pcsz;
typedef			char					sz;
typedef			bool					bl;


#define SAFE_FREE_NULL(a) if(*a != NULL) free(*a); *a = NULL
#define SAFE_DELETE_NULL(a) if(*a) { delete *(a); *(a) = nullptr; }
#define SAFE_DELETE2_NULL(a) if(*a) { delete []*(a); *(a) = nullptr; }
#define SAFE_RELEASE(a) if(a) (a)->Release()


#define GETSET(type, var) \
	private: \
		 type _##var##0; \
	public: \
		 type Get##var() const \
		{\
			return _##var##0; \
		}\
		 void Set##var(type val) \
		{\
			_##var##0 = val; \
		}\



#define INIT_ZERO(name) \
	_##name##0(0)

#define INIT_VAR(name, var) \
	_##name##0(var)


#define CC_BREAK_IF(cond)            if(cond) break


#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))



#define ABGR(r,g,b,a) ((unsigned)( ((unsigned)(r)) | (((unsigned)(g))<<8) | (((unsigned)(b))<<16) | (((unsigned)(a))<<24) ))
#define GETR(c)			((unsigned)(c) & 0x000000ff)
#define GETG(c)			(((unsigned)(c)>>8) & 0x000000ff)
#define GETB(c)			(((unsigned)(c)>>16) & 0x000000ff)
#define GETALPAH(c)		((unsigned)(c)>>24)



#define To26Dot6(a) (a<<6)
#define From26Dot6(a) (a>>6)

#define MAX(a, b) (a>b) ? (a) : (b)
#define MIN(a, b) (a>b) ? (b) : (a)

#define MAX_PATH_LEN 1024

enum {
	TOUCH_UP,
	TOUCH_DOWN,
	TOUCH_MOVE
};

#define TOUCH_COMMAND 0
