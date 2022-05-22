#include "WASI.h"

BEG_META( XE::WASI )
END_META()

XE::WASI::errno_t XE::WASI::args_get( char ** argv, char * argv_buf )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::args_sizes_get( XE::WASI::size_t * argc, XE::WASI::size_t * argv_buf_size )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::clock_res_get( XE::WASI::clockid_t clock_id, XE::WASI::timestamp_t * resolution )
{
	switch ( clock_id )
	{
	case XE::WASI::CLOCKID_REALTIME:
		break;
	case XE::WASI::CLOCKID_MONOTONIC:
		break;
	case XE::WASI::CLOCKID_PROCESS_CPUTIME_ID:
		break;
	case XE::WASI::CLOCKID_THREAD_CPUTIME_ID:
		break;
	default:
		return ERRNO_INVAL;
		break;
	}
}

XE::WASI::errno_t XE::WASI::clock_time_get( XE::WASI::clockid_t clock_id, XE::WASI::timestamp_t precision, XE::WASI::timestamp_t * time )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::environ_get( char ** environ, char * environ_buf )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::environ_sizes_get( XE::WASI::size_t * environ_count, XE::WASI::size_t * environ_buf_size )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_prestat_get( XE::WASI::fd_t fd, XE::WASI::prestat_t * buf )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_prestat_dir_name( XE::WASI::fd_t fd, char * path, XE::WASI::size_t path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_close( XE::WASI::fd_t fd )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_datasync( XE::WASI::fd_t fd )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_pread( XE::WASI::fd_t fd, const XE::WASI::iovec_t * iovs, XE::WASI::size_t iovs_len, XE::WASI::filesize_t offset, XE::WASI::size_t * nread )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_pwrite( XE::WASI::fd_t fd, const XE::WASI::ciovec_t * iovs, XE::WASI::size_t iovs_len, XE::WASI::filesize_t offset, XE::WASI::size_t * nwritten )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_read( XE::WASI::fd_t fd, const XE::WASI::iovec_t * iovs, XE::WASI::size_t iovs_len, XE::WASI::size_t * nread )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_renumber( XE::WASI::fd_t from, XE::WASI::fd_t to )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_seek( XE::WASI::fd_t fd, XE::WASI::filedelta_t offset, XE::WASI::whence_t whence, XE::WASI::filesize_t * newoffset )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_tell( XE::WASI::fd_t fd, XE::WASI::filesize_t * newoffset )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_fdstat_get( XE::WASI::fd_t fd, XE::WASI::fdstat_t * buf )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_fdstat_set_flags( XE::WASI::fd_t fd, XE::WASI::fdflags_t flags )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_fdstat_set_rights( XE::WASI::fd_t fd, XE::WASI::rights_t fs_rights_base, XE::WASI::rights_t fs_rights_inheriting )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_sync( XE::WASI::fd_t fd )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_write( XE::WASI::fd_t fd, const XE::WASI::ciovec_t * iovs, XE::WASI::size_t iovs_len, XE::WASI::size_t * nwritten )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_advise( XE::WASI::fd_t fd, XE::WASI::filesize_t offset, XE::WASI::filesize_t len, XE::WASI::advice_t advice )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_allocate( XE::WASI::fd_t fd, XE::WASI::filesize_t offset, XE::WASI::filesize_t len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_create_directory( XE::WASI::fd_t fd, const char * path, XE::WASI::size_t path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_link( XE::WASI::fd_t old_fd, XE::WASI::lookupflags_t old_flags, const char * old_path, XE::WASI::size_t old_path_len, XE::WASI::fd_t new_fd, const char * new_path, XE::WASI::size_t new_path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_open( XE::WASI::fd_t dirfd, XE::WASI::lookupflags_t dirflags, const char * path, XE::WASI::size_t path_len, XE::WASI::oflags_t oflags, XE::WASI::rights_t fs_rights_base, XE::WASI::rights_t fs_rights_inheriting, XE::WASI::fdflags_t fs_flags, XE::WASI::fd_t * fd )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_readdir( XE::WASI::fd_t fd, void * buf, XE::WASI::size_t buf_len, XE::WASI::dircookie_t cookie, XE::WASI::size_t * bufused )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_readlink( XE::WASI::fd_t fd, const char * path, XE::WASI::size_t path_len, char * buf, XE::WASI::size_t buf_len, XE::WASI::size_t * bufused )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_rename( XE::WASI::fd_t old_fd, const char * old_path, XE::WASI::size_t old_path_len, XE::WASI::fd_t new_fd, const char * new_path, XE::WASI::size_t new_path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_filestat_get( XE::WASI::fd_t fd, XE::WASI::filestat_t * buf )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_filestat_set_times( XE::WASI::fd_t fd, XE::WASI::timestamp_t st_atim, XE::WASI::timestamp_t st_mtim, XE::WASI::fstflags_t fstflags )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::fd_filestat_set_size( XE::WASI::fd_t fd, XE::WASI::filesize_t st_size )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_filestat_get( XE::WASI::fd_t fd, XE::WASI::lookupflags_t flags, const char * path, XE::WASI::size_t path_len, XE::WASI::filestat_t * buf )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_filestat_set_times( XE::WASI::fd_t fd, XE::WASI::lookupflags_t flags, const char * path, XE::WASI::size_t path_len, XE::WASI::timestamp_t st_atim, XE::WASI::timestamp_t st_mtim, XE::WASI::fstflags_t fstflags )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_symlink( const char * old_path, XE::WASI::size_t old_path_len, XE::WASI::fd_t fd, const char * new_path, XE::WASI::size_t new_path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_unlink_file( XE::WASI::fd_t fd, const char * path, XE::WASI::size_t path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::path_remove_directory( XE::WASI::fd_t fd, const char * path, XE::WASI::size_t path_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::poll_oneoff( const XE::WASI::subscription_t * in, XE::WASI::event_t * out, XE::WASI::size_t nsubscriptions, XE::WASI::size_t * nevents )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::proc_raise( XE::WASI::signal_t sig )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::random_get( void * buf, XE::WASI::size_t buf_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_accept( XE::WASI::fd_t fd, XE::WASI::fd_t * fd_new )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_addr_local( XE::WASI::fd_t fd, XE::uint8 * buf, XE::WASI::size_t buf_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_addr_remote( XE::WASI::fd_t fd, XE::uint8 * buf, XE::WASI::size_t buf_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_open( XE::WASI::fd_t poolfd, XE::WASI::address_family_t af, XE::WASI::sock_type_t socktype, XE::WASI::fd_t * sockfd )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_bind( XE::WASI::fd_t fd, XE::WASI::addr_t * addr )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_connect( XE::WASI::fd_t fd, XE::WASI::addr_t * addr )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_listen( XE::WASI::fd_t fd, XE::WASI::size_t backlog )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_recv( XE::WASI::fd_t sock, void * buf, XE::WASI::size_t buf_len, XE::WASI::size_t * recv_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_send( XE::WASI::fd_t sock, const void * buf, XE::WASI::size_t buf_len, XE::WASI::size_t * sent_len )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sock_shutdown( XE::WASI::fd_t sock )
{
	return ERRNO_SUCCESS;
}

XE::WASI::errno_t XE::WASI::sched_yield()
{
	return ERRNO_SUCCESS;
}
