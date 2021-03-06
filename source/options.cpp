
#include "avkit/options.h"

using namespace cppkit;

namespace avkit
{

struct codec_options get_fast_h264_decoder_options( const ck_string& pict_type )
{
    struct codec_options options;

    options.video_codec = "h264";
    options.thread_count = 2;
    options.tune = "zerolatency";

    options.pict_type = pict_type;

    return options;
}

struct codec_options get_normal_h264_decoder_options( const ck_string& pict_type )
{
    struct codec_options options;

    options.video_codec = "h264";
    options.tune = "zerolatency";

    options.pict_type = pict_type;

    return options;
}

struct codec_options get_fast_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen )
{
    struct codec_options options;

    options.video_codec = "h264";
    options.gop_size = gopSize;
    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.time_base_num = timeBaseNum;
    options.time_base_den = timeBaseDen;
    options.me_method = 0;
    options.me_subpel_quality = 4;
    options.delay = 0;
    options.thread_count = 6;
    options.refs = 3;
    options.rc_buffer_size = 0;
    options.profile = "baseline";
    options.preset = "ultrafast";
    options.tune = "zerolatency";

    return options;
}

struct codec_options get_hls_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen )
{
    struct codec_options options;

    options.video_codec = "h264";
    options.gop_size = gopSize;
    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.time_base_num = timeBaseNum;
    options.time_base_den = timeBaseDen;
    options.delay = 0;
    options.thread_count = 6;
    options.preset = "superfast";
    options.tune = "zerolatency";
    options.x264opts = ck_string::format("no-scenecut:vbv-maxrate=%d:vbv-bufsize=%d:me=hex:subme=7", bitRate / 1000, bitRate / 1000);

    return options;
}

struct codec_options get_transcode_export_h264_encoder_options( int bitRate, int picWidth, int picHeight, int gopSize, int timeBaseNum, int timeBaseDen )
{
    struct codec_options options;

    options.video_codec = "h264";
    options.gop_size = gopSize;
    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.time_base_num = timeBaseNum;
    options.time_base_den = timeBaseDen;
    options.delay = 0;
    options.thread_count = 6;
    options.tune = "zerolatency";

    return options;
}

struct codec_options get_jpeg_options( int picWidth,
                                       int picHeight,
                                       int bitRate,
                                       int qmin,
                                       int qmax )
{
    struct codec_options options;

    options.bit_rate = bitRate;
    options.width = picWidth;
    options.height = picHeight;
    options.qmin = qmin;
    options.qmax = qmax;

    return options;
}

void add_audio_options( struct codec_options& options,
                        int sampleRate,
                        int channels,
                        const cppkit::ck_string& audioFormats,
                        int timeBaseNum,
                        int timeBaseDen )
{
    options.audio_sample_rate = sampleRate;
    options.audio_channels = channels;
    options.audio_format = audioFormats;
    options.audio_time_base_num = timeBaseNum;
    options.audio_time_base_den = timeBaseDen;        
}

}
