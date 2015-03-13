#include "jmw.h"
#include "TextureManager.h"
#include "png/png.h"
#include <fstream>

NS_JMW_BEGIN

#define PNGSIGSIZE  8





typedef struct
{
	u8*		data;
	size_t	size;
	s32		offset;
}PngSource;


static void PngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	PngSource* isource = (PngSource*)png_get_io_ptr(png_ptr);

	if ((size_t)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		LOG("%s", "PngReadCallback failed");
	}
}


TextureManager *TextureManager::pInstance = nullptr;

TextureManager& TextureManager::GetInstance(void)
{
	if (pInstance == nullptr)
		pInstance = new TextureManager();

	return *pInstance;
}


void TextureManager::ReleaseInstance(void)
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}



TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{

	//for (auto &t : m_TexMng)
	//	delete t.second;


	m_TexMng.clear();

}


shared_ptr<Image> TextureManager::GetImage(const string key)
{

	shared_ptr<Image> value0 = nullptr;

	auto img = m_TexMng.find(key);
	if (img != m_TexMng.cend())
		value0 = img->second;
	else
	{
		shared_ptr<Image> img = GetImageFromRes(key);

		if (img)
		{
			m_TexMng.insert(std::make_pair(key, img));
			value0 = img;
		}
	}

	return value0;
}


shared_ptr<Image> TextureManager::GetImageFromRes(const string fileName)
{
	shared_ptr<Image> img0 = nullptr;

	if (fileName.rfind(".png") != std::string::npos)
		img0 = LoadPng(fileName);


	return img0;
}



shared_ptr<Image> TextureManager::LoadPng(const string fileName)
{
	shared_ptr<Image>      img0 = nullptr;
	u8*         pdata = nullptr;
	png_structp png_ptr = 0;
	png_infop   info_ptr = 0;
	size_t      size0;



	do {

		pdata = GetData(fileName, &size0);
		CC_BREAK_IF(!pdata);

		char header[PNGSIGSIZE];
		memcpy(header, pdata, PNGSIGSIZE);

		CC_BREAK_IF(png_sig_cmp((png_bytep)header, 0, PNGSIGSIZE));

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		CC_BREAK_IF(!png_ptr);

		info_ptr = png_create_info_struct(png_ptr);
		CC_BREAK_IF(!info_ptr);

		CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));


		PngSource imageSource;
		imageSource.data = (unsigned char*)pdata;
		imageSource.size = size0;
		imageSource.offset = 0;
		png_set_read_fn(png_ptr, &imageSource, PngReadCallback);

		png_read_info(png_ptr, info_ptr);

		img0 = Image::Create();
		CC_BREAK_IF(!img0);

		img0->SettexW(png_get_image_width(png_ptr, info_ptr));
		img0->SettexH(png_get_image_height(png_ptr, info_ptr));
		img0->SettexSizeW(img0->GettexW());
		img0->SettexSizeH(img0->GettexH());



		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);


		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}

		if (color_type == PNG_COLOR_TYPE_GRAY && img0->GetDepth() < 8)
		{
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}

		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}

		if (img0->GetDepth() == 16)
		{
			png_set_strip_16(png_ptr);
		}

		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
		}


		png_uint_32 rowbytes;
		png_bytep* row_pointers = new png_bytep[sizeof(png_bytep) * img0->GettexH()];

		png_read_update_info(png_ptr, info_ptr);
		rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		for (unsigned short i = 0; i < img0->GettexH(); ++i)
		{
			row_pointers[i] = new unsigned char[rowbytes];
		}

		png_read_image(png_ptr, row_pointers);
		png_read_end(png_ptr, NULL);


		GLubyte * imageData = nullptr;
		imageData = new unsigned char[rowbytes * img0->GettexH()];


		for (int i = 0; i < img0->GettexH(); i++) {
			//memcpy(imageData + (rowbytes * (img0->GetH() - 1 - i)), row_pointers[i], rowbytes); // png는 상하로 바뀌어 있는 byte를 다시 옮긴다.

			memcpy(imageData + (rowbytes * i), row_pointers[i], rowbytes);
		}



		png_uint_32 channel = rowbytes / img0->GettexW();
		img0->SetDepth(channel);
		if (channel == 4)
		{

			unsigned int *tmp = (unsigned int *)imageData;
			for (unsigned short i = 0; i < img0->GettexH(); i++)
			{
				for (unsigned int j = 0; j < rowbytes; j += 4)
				{
					*tmp++ = CC_RGB_PREMULTIPLY_ALPHA(*(imageData + (rowbytes*i) + j), *(imageData + (rowbytes*i) + j + 1),
						*(imageData + (rowbytes*i) + j + 2), *(imageData + (rowbytes*i) + j + 3));
				}
			}
		}

		for (unsigned short i = 0; i < img0->GettexH(); ++i)
		{
			delete[] row_pointers[i];
		}
		delete[] row_pointers;



		glGenTextures(1, &img0->m_TextureID);
		glBindTexture(GL_TEXTURE_2D, img0->m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D,
			0,
			channel == 4 ? GL_RGBA : GL_RGB,
			img0->GettexW(),
			img0->GettexH(),
			0,
			channel == 4 ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		delete[] imageData;
	} while (0);

	if (png_ptr) png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	if (pdata) delete[] pdata;
	return img0;
}


NS_JMW_END
