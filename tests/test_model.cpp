#include <model/model.h>
#include <model/rgb.h>
#include <model/image.h>

#include <gtest/gtest.h>
#include <fstream>
#include <vector>

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
    const char* EXAMPLE_IMAGE = "obj/african_head_diffuse.bmp";
    const char* EXAMPLE_IMAGE_COPY = "obj/african_head_diffuse_copy.bmp";
    Image img;
    img.loadBMP(EXAMPLE_IMAGE);
    img.saveBMP(EXAMPLE_IMAGE_COPY);
    std::cerr << "[          ] !!see the copy of the bmp image in obj!!" << std::endl;
    EXPECT_EQ(true, true); // see the copy of the image
}

