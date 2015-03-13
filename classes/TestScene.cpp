#include "jmw.h"
#include "TestScene.h"



//#include <curl/curl.h>
//#include <iostream>
//#include <json/json.h>

TestScene::TestScene()
{

	//EXECUTE_CMSchedule(TestScene::ScenePopTest, nullptr, 1, 5);
}


TestScene::~TestScene()
{
}


void TestScene::Draw(sl32 delta)
{
	Scene::Draw(delta);
}


/*
struct MemoryStruct{
	char *memory;
	size_t size;
};


static size_t WriteMemoryCallBack(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct*)userp;

	mem->memory = (char*)realloc(mem->memory, mem->size +  realsize + 1);
	if(mem->memory == NULL) {

		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}


	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;

}









static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

*/



void TestScene::ScenePopTest(void* me, void* param, sl32 delta)
{
	LOG("%s", RunTime::GetInstance().GetStringTime(RunTime::GetInstance().GetCurTime()));
	//GetDirector().PopScene();

	//RedScene* rs = new RedScene();
	//GetDirector().PushScene(rs);

	//CURLcode res;
	//CURL *pstCUrl = curl_easy_init();

	//curl_version_info_data * vinfo = curl_version_info(CURLVERSION_NOW);
	//if(vinfo->features & CURL_VERSION_SSL){
	//	LOG("%s", "CURL: SSL enabled");
	//}else{
	//	LOG("%s", "CURL: SSL not enabled");
	//}

	/*
	struct MemoryStruct chunk;

	chunk.memory = (char*)malloc(1);
	chunk.size = 0;

	struct curl_slist *g_pstCUrlHeaders = NULL;
	char szPost[6000];
	char szReceipt[4000];

	sprintf(szReceipt, "%s", "{\"signature\" = \"Ap6wdSWPxdrg954yVKgArWvpcC078wTgzY3MT6XJVCT6NtjriD3qN4HZuSVHzY+I6btBGqzfpDHg/XjSuj3KxQiPmq92SptyCPcKutM3zxT9JxWq/DMLk3sqTlafr29Oesb4zCqcvCALMmWp9nyQhtA1ojNffadzURwe4BJMSSC4AAADVzCCA1MwggI7oAMCAQICCBup4+PAhm/LMA0GCSqGSIb3DQEBBQUAMH8xCzAJBgNVBAYTAlVTMRMwEQYDVQQKDApBcHBsZSBJbmMuMSYwJAYDVQQLDB1BcHBsZSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTEzMDEGA1UEAwwqQXBwbGUgaVR1bmVzIFN0b3JlIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTE0MDYwNzAwMDIyMVoXDTE2MDUxODE4MzEzMFowZDEjMCEGA1UEAwwaUHVyY2hhc2VSZWNlaXB0Q2VydGlmaWNhdGUxGzAZBgNVBAsMEkFwcGxlIGlUdW5lcyBTdG9yZTETMBEGA1UECgwKQXBwbGUgSW5jLjELMAkGA1UEBhMCVVMwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAMmTEuLgjimLwRJxy1oEf0esUNDVEIe6wDsnnal14hNBt1v195X6n93YO7gi3orPSux9D554SkMp+Sayg84lTc362UtmYLpWnb34nqyGx9KBVTy5OGV4ljE1OwC+oTnRM+QLRCmeNxMbPZhS47T+eZtDEhVB9usk3+JM2Cogfwo7AgMBAAGjcjBwMB0GA1UdDgQWBBSJaEeNuq9Df6ZfN68Fe+I2u22ssDAMBgNVHRMBAf8EAjAAMB8GA1UdIwQYMBaAFDYd6OKdgtIBGLUyaw7XQwuRWEM6MA4GA1UdDwEB/wQEAwIHgDAQBgoqhkiG92NkBgUBBAIFADANBgkqhkiG9w0BAQUFAAOCAQEAeaJV2U51rxfcqAAe5C2/fEW8KUl4iO4lMuta7N6XzP1pZIz1NkkCtIIweyNj5URYHK+HjRKSU9RLguNl0nkfxqObiMckwRudKSq69NInrZyCD66R4K77nb9lMTABSSYlsKt8oNtlhgR/1kjSSRQcHktsDcSiQGKMdkSlp4AyXf7vnHPBe4yCwYV2PpSN04kboiJ3pBlxsGwV/ZlL26M2ueYHKYCuXhdqFwxVgm52h3oeJOOt/vY4EcQq7eqHm6m03Z9b7PRzYM2KGXHDmOMk7vDpeMVlLDPSGYz1+U3sDxJzebSpbaJmT7imzUKfggEY7xxf4czfH0yj5wNzSGTOvQ==\";\"purchase-info\" = \"ewoJIm9yaWdpbmFsLXB1cmNoYXNlLWRhdGUtcHN0IiA9ICIyMDE0LTExLTEwIDAwOjAyOjMzIEFtZXJpY2EvTG9zX0FuZ2VsZXMiOwoJInB1cmNoYXNlLWRhdGUtbXMiID0gIjE0MTU2MDY1NTM1OTciOwoJInVuaXF1ZS1pZGVudGlmaWVyIiA9ICIzZGQ5MDQ0OGYyMTcxN2RlZjkwOGI3YmRkNDNmYTg3MmM3ZThmNjU3IjsKCSJvcmlnaW5hbC10cmFuc2FjdGlvbi1pZCIgPSAiNDQwMDAwMTIwNzg2MzUwIjsKCSJidnJzIiA9ICIxLjIuMCI7CgkiYXBwLWl0ZW0taWQiID0gIjg0NDY4NDkxMiI7CgkidHJhbnNhY3Rpb24taWQiID0gIjQ0MDAwMDEyMDc4NjM1MCI7CgkicXVhbnRpdHkiID0gIjEiOwoJIm9yaWdpbmFsLXB1cmNoYXNlLWRhdGUtbXMiID0gIjE0MTU2MDY1NTM1OTciOwoJInVuaXF1ZS12ZW5kb3ItaWRlbnRpZmllciIgPSAiMTM4RkVCMDMtMzUxRC00RjA0LUJCQkMtN0E5QzQ1MzlGMjRDIjsKCSJpdGVtLWlkIiA9ICI4NDQ2ODUwNDUiOwoJInZlcnNpb24tZXh0ZXJuYWwtaWRlbnRpZmllciIgPSAiNzU5MjAyODkxIjsKCSJwcm9kdWN0LWlkIiA9ICJjb20uZ2FtZXZpbC5mYW50YXN5d2FybG9yZC5pb3MuYXBwbGUud3cubm9ybWFsLjEwSmV3ZWxzIjsKCSJwdXJjaGFzZS1kYXRlIiA9ICIyMDE0LTExLTEwIDA4OjAyOjMzIEV0Yy9HTVQiOwoJIm9yaWdpbmFsLXB1cmNoYXNlLWRhdGUiID0gIjIwMTQtMTEtMTAgMDg6MDI6MzMgRXRjL0dNVCI7CgkiYmlkIiA9ICJjb20uZ2FtZXZpbC5mYW50YXN5d2FybG9yZC5pb3MuYXBwbGUud3cubm9ybWFsIjsKCSJwdXJjaGFzZS1kYXRlLXBzdCIgPSAiMjAxNC0xMS0xMCAwMDowMjozMyBBbWVyaWNhL0xvc19BbmdlbGVzIjsKfQ==\";\"pod\" = \"44\";\"signing-status\" = \"0\";}");
	sprintf(szPost, "{\"receipt-data\" : \"%s\"}", base64_encode((unsigned char const*)szReceipt, strlen(szReceipt)).c_str());


	curl_slist_append(g_pstCUrlHeaders, "Content-Type: application/json");
	curl_easy_setopt(pstCUrl, CURLOPT_URL, "https://buy.itunes.apple.com/verifyReceipt");


	curl_easy_setopt(pstCUrl, CURLOPT_POST, true);
	curl_easy_setopt(pstCUrl, CURLOPT_HTTPHEADER, g_pstCUrlHeaders);
	curl_easy_setopt(pstCUrl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(pstCUrl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(pstCUrl, CURLOPT_POSTFIELDSIZE, strlen(szPost));
	curl_easy_setopt(pstCUrl, CURLOPT_POSTFIELDS, szPost);
	curl_easy_setopt(pstCUrl, CURLOPT_WRITEDATA, (void*)&chunk);
	curl_easy_setopt(pstCUrl, CURLOPT_WRITEFUNCTION, WriteMemoryCallBack);


	res = curl_easy_perform(pstCUrl);
	if(res != CURLE_OK)
	{
		LOG("curl_easy_perform() failed: %s", curl_easy_strerror(res));
	}
	else
	{
		//LOG("%s", (char*)chunk.memory);

		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(chunk.memory, root);

		Json::Value receipt = root["receipt"];

		LOG("%s", receipt["product_id"].asCString());

	}


	curl_easy_cleanup(pstCUrl);

	if(chunk.memory)
		free(chunk.memory);

	*/

}
