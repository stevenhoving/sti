#include <sti/atlas.hpp>
#include <sti/image_detail.hpp>
#include <sti/convert/convert_image.hpp>
#include "write_file_fixture.h"
#include <cstdint>
#include <string>

TEST_F(write_file_fixture, atlas_split_sprites)
{
    auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/KannettCSheet1.png");
    auto loaded_image = sti::codecs::png::decode(stream);

    auto result = sti::convert_image<float>::from_interleaved_image(loaded_image);

    auto new_images = sti::atlas<float>::split(result, 32, 32);

    int sprite_number = 0;
    for (auto &new_image : new_images)
    {
        auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(new_image);
        write_result("KannettCSheet1_" + std::to_string(sprite_number++) + ".png", new_interleaved_image);
    }

    // Blend blit over a photo
    auto stream2 = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
    auto dog = sti::codecs::png::decode(stream2);
    auto dog_image = sti::convert_image<float>::from_interleaved_image(dog);

    sti::detail<float>::blit(dog_image, 0, 0, new_images.at(0), sti::blit_operator<float>::blend_alpha);
    sti::detail<float>::blit(dog_image, 32, 0, new_images.at(1), sti::blit_operator<float>::blend_one_minus_alpha);
    sti::detail<float>::blit(dog_image, 64, 0, new_images.at(2), sti::blit_operator<float>::blend_alpha_ratio);

    auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(dog_image);
    write_result("DSC_7000_one_minus_alpha_blend_sprite.png", new_interleaved_image);
}
