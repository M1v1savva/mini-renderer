#include <model/model.h>
#include <model/rgb.h>
#include <model/image.h>

#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <openssl/md5.h>

using namespace std;

using mini_renderer::RGB;
using mini_renderer::Model;
using mini_renderer::Image;

std::string fileHash(const char* filename) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open file for hashing");

    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buffer[4096];
    while (f.read(buffer, sizeof(buffer)) || f.gcount()) {
        MD5_Update(&ctx, buffer, f.gcount());
    }

    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_Final(hash, &ctx);

    std::string result;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        result += buf;
    }
    return result;
}

TEST(RGBTest, ConstructorAndMultiply) {
    RGB c1;                  
    EXPECT_EQ(c1.r, 0);
    EXPECT_EQ(c1.g, 0);
    EXPECT_EQ(c1.b, 0);

    RGB c2(0.5f);            
    EXPECT_EQ(c2.r, 127);
    EXPECT_EQ(c2.g, 127);
    EXPECT_EQ(c2.b, 127);

    RGB c3(10, 20, 30);      
    auto c4 = c3 * 2.0f;     
    EXPECT_EQ(c4.r, 20);
    EXPECT_EQ(c4.g, 40);
    EXPECT_EQ(c4.b, 60);
}

TEST(ImageTest, BMPCopyIntegrity) {
    mini_renderer::Image<800,800> img;
    img.loadBMP("obj/name.bmp");
    img.saveBMP("obj/copy.bmp");

    std::string hash1 = fileHash("obj/african_head_diffuse.bmp");
    std::string hash2 = fileHash("obj/african_head_diffuse_copy.bmp");

    EXPECT_EQ(hash1, hash2);
}
