
#include "h264_decoder_test.h"
#include "avkit/h264_decoder.h"
#include "avkit/jpeg_encoder.h"
#include "avkit/options.h"
#include "avkit/locky.h"
#include "cppkit/ck_memory.h"

extern "C"
{
#include "libavformat/avformat.h"
};

#include "gop.c"

using namespace std;
using namespace cppkit;
using namespace avkit;

REGISTER_TEST_FIXTURE(h264_decoder_test);

void h264_decoder_test::setup()
{
//    av_register_all();
    locky::register_ffmpeg();
}

void h264_decoder_test::teardown()
{
    locky::unregister_ffmpeg();
}

void h264_decoder_test::test_constructor()
{
    UT_ASSERT_NO_THROW( shared_ptr<h264_decoder> d = make_shared<h264_decoder>( get_fast_h264_decoder_options() ) );
}

void h264_decoder_test::test_input_dimensions()
{
    shared_ptr<h264_decoder> decoder = make_shared<h264_decoder>( get_fast_h264_decoder_options() );

    decoder->decode( gop[0].frame, gop[0].frameSize );

    UT_ASSERT( decoder->get_input_width() == 1280 );
    UT_ASSERT( decoder->get_input_height() == 720 );
}

void h264_decoder_test::test_output_dimensions()
{
    shared_ptr<h264_decoder> decoder = make_shared<h264_decoder>( get_fast_h264_decoder_options() );

    decoder->decode( gop[0].frame, gop[0].frameSize );

    decoder->set_output_width( 640 );
    decoder->set_output_height( 360 );

    size_t picSize = decoder->get_yuv420p_size();

    UT_ASSERT( picSize == 345600 );

    shared_ptr<ck_memory> pic;
    UT_ASSERT_NO_THROW( pic = decoder->make_yuv420p() );

    UT_ASSERT( pic->size_data() == 345600 );
}

void h264_decoder_test::test_decode_gop()
{
    shared_ptr<h264_decoder> decoder = make_shared<h264_decoder>( get_fast_h264_decoder_options() );

    for( int i = 0; i < NUM_FRAMES_IN_GOP; i++ )
    {
        UT_ASSERT_NO_THROW( decoder->decode( gop[i].frame, gop[i].frameSize ) );

        shared_ptr<ck_memory> pic;
        UT_ASSERT_NO_THROW( pic = decoder->make_yuv420p() );

        UT_ASSERT( pic->size_data() == 1382400 );
    }
}
