
#include "avkit/h264mp4_to_annexb.h"

#include "cppkit/ck_exception.h"

using namespace avkit;
using namespace cppkit;
using namespace std;

h264mp4_to_annexb::h264mp4_to_annexb( av_demuxer& deMuxer ) :
    _bsfc( av_bitstream_filter_init( "h264_mp4toannexb" ) ),
    _codec( deMuxer._context->streams[deMuxer._videoStreamIndex]->codec ),
    _filteredPacket()
{
    if( !_bsfc )
        CK_THROW(("Unable to initialize h264_mp4toannexb bitstream filter."));

    if( !_codec )
        CK_THROW(("h264mp4_to_annexb requires valid codec context (either from an encoder or a demuxer)."));
}

h264mp4_to_annexb::~h264mp4_to_annexb() throw()
{
    _free_filtered_packet();

    av_bitstream_filter_close( _bsfc );
}

void h264mp4_to_annexb::transform( uint8_t* src, size_t srcSize, bool keyFrame )
{
    AVPacket inputPacket;
    av_init_packet( &inputPacket );
    inputPacket.data = src;
    inputPacket.size = srcSize;

    _free_filtered_packet();

    // If the return value is 0, the output buffer is not allocated and should be considered identical to the
    // input buffer, or in case *poutbuf was set it points to the input buffer (not necessarily to its starting
    // address).

    av_bitstream_filter_filter( _bsfc, _codec, NULL,
                                &_filteredPacket.data, &_filteredPacket.size,
                                inputPacket.data, inputPacket.size,
                                (keyFrame) ? 1 : 0 );

    // Given the super tricky comment above (lifted right from the ffmpeg documentation!) our strategy is this:
    // If our output buffer overlaps our input, then allocate storage for it, copy the data into it and reset
    // our _filteredPacket.data and _filteredPacket.size to point to it. This way, we can always guarantee our
    // _filteredPacket is distinct from out inputPacket.

    if( _filteredPacket.data >= inputPacket.data && _filteredPacket.data < (inputPacket.data + inputPacket.size) )
    {
        size_t filteredSize = inputPacket.size - (_filteredPacket.data - inputPacket.data);
        uint8_t* data = (uint8_t*)av_malloc( filteredSize );
        if( !data )
            CK_THROW(("Unable to allocate filter packet."));
        memcpy( data, _filteredPacket.data, filteredSize );
        _filteredPacket.data = data;
        _filteredPacket.size = filteredSize;
    }
}

void h264mp4_to_annexb::transform( std::shared_ptr<ck_memory> src, bool keyFrame )
{
    transform( src->map().get_ptr(), src->size_data(), keyFrame );
}

size_t h264mp4_to_annexb::get_annexb_size() const
{
    return _filteredPacket.size;
}

void h264mp4_to_annexb::get_annexb( uint8_t* dest ) const
{
    memcpy( dest, _filteredPacket.data, _filteredPacket.size );
}

std::shared_ptr<ck_memory> h264mp4_to_annexb::get_annexb() const
{
    std::shared_ptr<ck_memory> buffer = make_shared<ck_memory>();
    uint8_t* dest = buffer->extend_data( _filteredPacket.size ).get_ptr();
    get_annexb( dest );
    return buffer;
}

void h264mp4_to_annexb::_free_filtered_packet()
{
    if( _filteredPacket.size > 0 )
    {
        av_free( _filteredPacket.data );
        _filteredPacket.data = NULL;
        _filteredPacket.size = 0;
    }
}
