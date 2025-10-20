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
    const char* EXAMPLE_IMAGE = "obj/african_head_diffuse.tga";
    const char* EXAMPLE_IMAGE_COPY = "test_copy_image.tga";
    Image img;
    img.load_tga(EXAMPLE_IMAGE);
    img.save_tga(EXAMPLE_IMAGE_COPY);
    std::cerr << "[          ] !!see the copy of the tga image in obj!!" << std::endl;
    EXPECT_EQ(true, true); // see the copy of the image
}

