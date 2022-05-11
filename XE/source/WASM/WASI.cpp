#include "WASI.h"

BEG_META( XE::WASI )
END_META()

XE::WASI::__wasi_errno_t XE::WASI::__wasi_args_get( char ** argv, char * argv_buf )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_args_sizes_get( __wasi_size_t * argc, __wasi_size_t * argv_buf_size )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_clock_res_get( __wasi_clockid_t clock_id, __wasi_timestamp_t * resolution )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_clock_time_get( __wasi_clockid_t clock_id, __wasi_timestamp_t precision, __wasi_timestamp_t * time )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_environ_get( char ** environ, char * environ_buf )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_environ_sizes_get( __wasi_size_t * environ_count, __wasi_size_t * environ_buf_size )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_prestat_get( __wasi_fd_t fd, __wasi_prestat_t * buf )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_prestat_dir_name( __wasi_fd_t fd, char * path, __wasi_size_t path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_close( __wasi_fd_t fd )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_datasync( __wasi_fd_t fd )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_pread( __wasi_fd_t fd, const __wasi_iovec_t * iovs, __wasi_size_t iovs_len, __wasi_filesize_t offset, __wasi_size_t * nread )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_pwrite( __wasi_fd_t fd, const __wasi_ciovec_t * iovs, __wasi_size_t iovs_len, __wasi_filesize_t offset, __wasi_size_t * nwritten )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_read( __wasi_fd_t fd, const __wasi_iovec_t * iovs, __wasi_size_t iovs_len, __wasi_size_t * nread )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_renumber( __wasi_fd_t from, __wasi_fd_t to )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_seek( __wasi_fd_t fd, __wasi_filedelta_t offset, __wasi_whence_t whence, __wasi_filesize_t * newoffset )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_tell( __wasi_fd_t fd, __wasi_filesize_t * newoffset )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_fdstat_get( __wasi_fd_t fd, __wasi_fdstat_t * buf )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_fdstat_set_flags( __wasi_fd_t fd, __wasi_fdflags_t flags )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_fdstat_set_rights( __wasi_fd_t fd, __wasi_rights_t fs_rights_base, __wasi_rights_t fs_rights_inheriting )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_sync( __wasi_fd_t fd )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_write( __wasi_fd_t fd, const __wasi_ciovec_t * iovs, __wasi_size_t iovs_len, __wasi_size_t * nwritten )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_advise( __wasi_fd_t fd, __wasi_filesize_t offset, __wasi_filesize_t len, __wasi_advice_t advice )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_allocate( __wasi_fd_t fd, __wasi_filesize_t offset, __wasi_filesize_t len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_create_directory( __wasi_fd_t fd, const char * path, __wasi_size_t path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_link( __wasi_fd_t old_fd, __wasi_lookupflags_t old_flags, const char * old_path, __wasi_size_t old_path_len, __wasi_fd_t new_fd, const char * new_path, __wasi_size_t new_path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_open( __wasi_fd_t dirfd, __wasi_lookupflags_t dirflags, const char * path, __wasi_size_t path_len, __wasi_oflags_t oflags, __wasi_rights_t fs_rights_base, __wasi_rights_t fs_rights_inheriting, __wasi_fdflags_t fs_flags, __wasi_fd_t * fd )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_readdir( __wasi_fd_t fd, void * buf, __wasi_size_t buf_len, __wasi_dircookie_t cookie, __wasi_size_t * bufused )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_readlink( __wasi_fd_t fd, const char * path, __wasi_size_t path_len, char * buf, __wasi_size_t buf_len, __wasi_size_t * bufused )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_rename( __wasi_fd_t old_fd, const char * old_path, __wasi_size_t old_path_len, __wasi_fd_t new_fd, const char * new_path, __wasi_size_t new_path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_filestat_get( __wasi_fd_t fd, __wasi_filestat_t * buf )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_filestat_set_times( __wasi_fd_t fd, __wasi_timestamp_t st_atim, __wasi_timestamp_t st_mtim, __wasi_fstflags_t fstflags )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_fd_filestat_set_size( __wasi_fd_t fd, __wasi_filesize_t st_size )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_filestat_get( __wasi_fd_t fd, __wasi_lookupflags_t flags, const char * path, __wasi_size_t path_len, __wasi_filestat_t * buf )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_filestat_set_times( __wasi_fd_t fd, __wasi_lookupflags_t flags, const char * path, __wasi_size_t path_len, __wasi_timestamp_t st_atim, __wasi_timestamp_t st_mtim, __wasi_fstflags_t fstflags )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_symlink( const char * old_path, __wasi_size_t old_path_len, __wasi_fd_t fd, const char * new_path, __wasi_size_t new_path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_unlink_file( __wasi_fd_t fd, const char * path, __wasi_size_t path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_path_remove_directory( __wasi_fd_t fd, const char * path, __wasi_size_t path_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_poll_oneoff( const __wasi_subscription_t * in, __wasi_event_t * out, __wasi_size_t nsubscriptions, __wasi_size_t * nevents )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_proc_raise( __wasi_signal_t sig )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_random_get( void * buf, __wasi_size_t buf_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_accept( __wasi_fd_t fd, __wasi_fd_t * fd_new )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_addr_local( __wasi_fd_t fd, XE::uint8 * buf, __wasi_size_t buf_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_addr_remote( __wasi_fd_t fd, XE::uint8 * buf, __wasi_size_t buf_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_open( __wasi_fd_t poolfd, __wasi_address_family_t af, __wasi_sock_type_t socktype, __wasi_fd_t * sockfd )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_bind( __wasi_fd_t fd, __wasi_addr_t * addr )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_connect( __wasi_fd_t fd, __wasi_addr_t * addr )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_listen( __wasi_fd_t fd, __wasi_size_t backlog )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_recv( __wasi_fd_t sock, void * buf, __wasi_size_t buf_len, __wasi_size_t * recv_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_send( __wasi_fd_t sock, const void * buf, __wasi_size_t buf_len, __wasi_size_t * sent_len )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sock_shutdown( __wasi_fd_t sock )
{
	return 0;
}

XE::WASI::__wasi_errno_t XE::WASI::__wasi_sched_yield()
{
	return 0;
}
