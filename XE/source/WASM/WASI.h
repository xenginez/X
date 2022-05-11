/*!
 * \file   WASI.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef WASI_H8A76CEB4_F41C_491B_A559_32437F29F17A
#define WASI_H8A76CEB4_F41C_491B_A559_32437F29F17A

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API WASI : public std::enable_shared_from_this< WASI >
{
	OBJECT( WASI )

public:
	WASI() = default;

	virtual ~WASI() = default;

public:
	enum class __wasi_sock_type_t
	{
		SOCKET_DGRAM = 0,
		SOCKET_STREAM,
	};
	enum class __wasi_addr_type_t
	{
		IPv4 = 0,
		IPv6
	};
	enum class __wasi_address_family_t
	{
		INET4 = 0,
		INET6
	};

public:
	using __wasi_advice_t = XE::uint8;
	using __wasi_clockid_t = XE::uint32;
	using __wasi_device_t = XE::uint64;
	using __wasi_dircookie_t = XE::uint64;
	using __wasi_dirnamlen_t = XE::uint32;
	using __wasi_errno_t = XE::uint16;
	using __wasi_eventrwflags_t = XE::uint16;
	using __wasi_eventtype_t = XE::uint8;
	using __wasi_exitcode_t = XE::uint32;
	using __wasi_fd_t = XE::uint32;
	using __wasi_fdflags_t = XE::uint16;
	using __wasi_filedelta_t = XE::int64;
	using __wasi_filesize_t = XE::uint64;
	using __wasi_filetype_t = XE::uint8;
	using __wasi_fstflags_t = XE::uint16;
	using __wasi_inode_t = XE::uint64;
	using __wasi_linkcount_t = XE::uint64;
	using __wasi_lookupflags_t = XE::uint32;
	using __wasi_oflags_t = XE::uint16;
	using __wasi_riflags_t = XE::uint16;
	using __wasi_rights_t = XE::uint64;
	using __wasi_roflags_t = XE::uint16;
	using __wasi_sdflags_t = XE::uint8;
	using __wasi_siflags_t = XE::uint16;
	using __wasi_signal_t = XE::uint8;
	using __wasi_size_t = XE::uint32;
	using __wasi_subclockflags_t = XE::uint16;
	using __wasi_timestamp_t = XE::uint64;
	using __wasi_userdata_t = XE::uint64;
	using __wasi_whence_t = XE::uint8;
	using __wasi_preopentype_t = XE::uint8;
	using __wasi_ip_port_t = XE::uint16;

public:
	struct __wasi_dirent_t
	{
		__wasi_dircookie_t d_next;
		__wasi_inode_t d_ino;
		__wasi_dirnamlen_t d_namlen;
		__wasi_filetype_t d_type;
	};
	struct __wasi_event_t
	{
		__wasi_userdata_t userdata;
		__wasi_errno_t error;
		__wasi_eventtype_t type;
		XE::uint8 paddings[5];
		union event_u
		{
			struct event_u_fd_readwrite_t
			{
				__wasi_filesize_t nbytes;
				__wasi_eventrwflags_t flags;
				XE::uint8 paddings[6];
			} fd_readwrite;
		} u;
	};
	struct __wasi_prestat_t
	{
		__wasi_preopentype_t pr_type;
		union prestat_u
		{
			struct prestat_u_dir_t
			{
				__wasi_size_t pr_name_len;
			} dir;
		} u;
	};
	struct __wasi_fdstat_t
	{
		__wasi_filetype_t fs_filetype;
		__wasi_fdflags_t fs_flags;
		XE::uint8 paddings[4];
		__wasi_rights_t fs_rights_base;
		__wasi_rights_t fs_rights_inheriting;
	};
	struct __wasi_filestat_t
	{
		__wasi_device_t st_dev;
		__wasi_inode_t st_ino;
		__wasi_filetype_t st_filetype;
		__wasi_linkcount_t st_nlink;
		__wasi_filesize_t st_size;
		__wasi_timestamp_t st_atim;
		__wasi_timestamp_t st_mtim;
		__wasi_timestamp_t st_ctim;
	};
	struct __wasi_ciovec_t
	{
		const void * buf;
		__wasi_size_t buf_len;
	};
	struct __wasi_iovec_t
	{
		void * buf;
		__wasi_size_t buf_len;
	};
	struct __wasi_subscription_clock_t
	{
		__wasi_clockid_t clock_id;
		XE::uint8 paddings1[4];
		__wasi_timestamp_t timeout;
		__wasi_timestamp_t precision;
		__wasi_subclockflags_t flags;
		XE::uint8 paddings2[4];
	};
	struct __wasi_subscription_fd_readwrite_t
	{
		__wasi_fd_t fd;
	};
	union  __wasi_subscription_u_u_t
	{
		__wasi_subscription_clock_t clock;
		__wasi_subscription_fd_readwrite_t fd_readwrite;
	};
	struct __wasi_subscription_u_t
	{
		__wasi_eventtype_t type;
		__wasi_subscription_u_u_t u;
	};
	struct __wasi_subscription_t
	{
		__wasi_userdata_t userdata;
		__wasi_subscription_u_t u;
	};
	struct __wasi_addr_ip4
	{
		XE::uint8 n0;
		XE::uint8 n1;
		XE::uint8 n2;
		XE::uint8 n3;
	};
	struct __wasi_addr_ip4_port_t
	{
		__wasi_addr_ip4 addr;
		__wasi_ip_port_t port;
	};
	struct __wasi_addr_ip6
	{
		XE::uint16 n0;
		XE::uint16 n1;
		XE::uint16 n2;
		XE::uint16 n3;
		XE::uint16 h0;
		XE::uint16 h1;
		XE::uint16 h2;
		XE::uint16 h3;
	};
	struct __wasi_addr_ip6_port_t
	{
		__wasi_addr_ip6 addr;
		__wasi_ip_port_t port;
	};
	struct __wasi_addr_t
	{
		__wasi_addr_type_t kind;
		union
		{
			__wasi_addr_ip4_port_t ip4;
			__wasi_addr_ip6_port_t ip6;
		} addr;
	};

public:
	virtual __wasi_errno_t __wasi_args_get( char ** argv, char * argv_buf );
	virtual __wasi_errno_t __wasi_args_sizes_get( __wasi_size_t * argc, __wasi_size_t * argv_buf_size );
	virtual __wasi_errno_t __wasi_clock_res_get( __wasi_clockid_t clock_id, __wasi_timestamp_t * resolution );
	virtual __wasi_errno_t __wasi_clock_time_get( __wasi_clockid_t clock_id, __wasi_timestamp_t precision, __wasi_timestamp_t * time );
	virtual __wasi_errno_t __wasi_environ_get( char ** environ, char * environ_buf );
	virtual __wasi_errno_t __wasi_environ_sizes_get( __wasi_size_t * environ_count, __wasi_size_t * environ_buf_size );
	virtual __wasi_errno_t __wasi_fd_prestat_get( __wasi_fd_t fd, __wasi_prestat_t * buf );
	virtual __wasi_errno_t __wasi_fd_prestat_dir_name( __wasi_fd_t fd, char * path, __wasi_size_t path_len );
	virtual __wasi_errno_t __wasi_fd_close( __wasi_fd_t fd );
	virtual __wasi_errno_t __wasi_fd_datasync( __wasi_fd_t fd );
	virtual __wasi_errno_t __wasi_fd_pread( __wasi_fd_t fd, const __wasi_iovec_t * iovs, __wasi_size_t iovs_len, __wasi_filesize_t offset, __wasi_size_t * nread );
	virtual __wasi_errno_t __wasi_fd_pwrite( __wasi_fd_t fd, const __wasi_ciovec_t * iovs, __wasi_size_t iovs_len, __wasi_filesize_t offset, __wasi_size_t * nwritten );
	virtual __wasi_errno_t __wasi_fd_read( __wasi_fd_t fd, const __wasi_iovec_t * iovs, __wasi_size_t iovs_len, __wasi_size_t * nread );
	virtual __wasi_errno_t __wasi_fd_renumber( __wasi_fd_t from, __wasi_fd_t to );
	virtual __wasi_errno_t __wasi_fd_seek( __wasi_fd_t fd, __wasi_filedelta_t offset, __wasi_whence_t whence, __wasi_filesize_t * newoffset );
	virtual __wasi_errno_t __wasi_fd_tell( __wasi_fd_t fd, __wasi_filesize_t * newoffset );
	virtual __wasi_errno_t __wasi_fd_fdstat_get( __wasi_fd_t fd, __wasi_fdstat_t * buf );
	virtual __wasi_errno_t __wasi_fd_fdstat_set_flags( __wasi_fd_t fd, __wasi_fdflags_t flags );
	virtual __wasi_errno_t __wasi_fd_fdstat_set_rights( __wasi_fd_t fd, __wasi_rights_t fs_rights_base, __wasi_rights_t fs_rights_inheriting );
	virtual __wasi_errno_t __wasi_fd_sync( __wasi_fd_t fd );
	virtual __wasi_errno_t __wasi_fd_write( __wasi_fd_t fd, const __wasi_ciovec_t * iovs, __wasi_size_t iovs_len, __wasi_size_t * nwritten );
	virtual __wasi_errno_t __wasi_fd_advise( __wasi_fd_t fd, __wasi_filesize_t offset, __wasi_filesize_t len, __wasi_advice_t advice );
	virtual __wasi_errno_t __wasi_fd_allocate( __wasi_fd_t fd, __wasi_filesize_t offset, __wasi_filesize_t len );
	virtual __wasi_errno_t __wasi_path_create_directory( __wasi_fd_t fd, const char * path, __wasi_size_t path_len );
	virtual __wasi_errno_t __wasi_path_link( __wasi_fd_t old_fd, __wasi_lookupflags_t old_flags, const char * old_path, __wasi_size_t old_path_len, __wasi_fd_t new_fd, const char * new_path, __wasi_size_t new_path_len );
	virtual __wasi_errno_t __wasi_path_open( __wasi_fd_t dirfd, __wasi_lookupflags_t dirflags, const char * path, __wasi_size_t path_len, __wasi_oflags_t oflags, __wasi_rights_t fs_rights_base, __wasi_rights_t fs_rights_inheriting, __wasi_fdflags_t fs_flags, __wasi_fd_t * fd );
	virtual __wasi_errno_t __wasi_fd_readdir( __wasi_fd_t fd, void * buf, __wasi_size_t buf_len, __wasi_dircookie_t cookie, __wasi_size_t * bufused );
	virtual __wasi_errno_t __wasi_path_readlink( __wasi_fd_t fd, const char * path, __wasi_size_t path_len, char * buf, __wasi_size_t buf_len, __wasi_size_t * bufused );
	virtual __wasi_errno_t __wasi_path_rename( __wasi_fd_t old_fd, const char * old_path, __wasi_size_t old_path_len, __wasi_fd_t new_fd, const char * new_path, __wasi_size_t new_path_len );
	virtual __wasi_errno_t __wasi_fd_filestat_get( __wasi_fd_t fd, __wasi_filestat_t * buf );
	virtual __wasi_errno_t __wasi_fd_filestat_set_times( __wasi_fd_t fd, __wasi_timestamp_t st_atim, __wasi_timestamp_t st_mtim, __wasi_fstflags_t fstflags );
	virtual __wasi_errno_t __wasi_fd_filestat_set_size( __wasi_fd_t fd, __wasi_filesize_t st_size );
	virtual __wasi_errno_t __wasi_path_filestat_get( __wasi_fd_t fd, __wasi_lookupflags_t flags, const char * path, __wasi_size_t path_len, __wasi_filestat_t * buf );
	virtual __wasi_errno_t __wasi_path_filestat_set_times( __wasi_fd_t fd, __wasi_lookupflags_t flags, const char * path, __wasi_size_t path_len, __wasi_timestamp_t st_atim, __wasi_timestamp_t st_mtim, __wasi_fstflags_t fstflags );
	virtual __wasi_errno_t __wasi_path_symlink( const char * old_path, __wasi_size_t old_path_len, __wasi_fd_t fd, const char * new_path, __wasi_size_t new_path_len );
	virtual __wasi_errno_t __wasi_path_unlink_file( __wasi_fd_t fd, const char * path, __wasi_size_t path_len );
	virtual __wasi_errno_t __wasi_path_remove_directory( __wasi_fd_t fd, const char * path, __wasi_size_t path_len );
	virtual __wasi_errno_t __wasi_poll_oneoff( const __wasi_subscription_t * in, __wasi_event_t * out, __wasi_size_t nsubscriptions, __wasi_size_t * nevents );
	virtual __wasi_errno_t __wasi_proc_raise( __wasi_signal_t sig );
	virtual __wasi_errno_t __wasi_random_get( void * buf, __wasi_size_t buf_len );
	virtual __wasi_errno_t __wasi_sock_accept( __wasi_fd_t fd, __wasi_fd_t * fd_new );
	virtual __wasi_errno_t __wasi_sock_addr_local( __wasi_fd_t fd, XE::uint8 * buf, __wasi_size_t buf_len );
	virtual __wasi_errno_t __wasi_sock_addr_remote( __wasi_fd_t fd, XE::uint8 * buf, __wasi_size_t buf_len );
	virtual __wasi_errno_t __wasi_sock_open( __wasi_fd_t poolfd, __wasi_address_family_t af, __wasi_sock_type_t socktype, __wasi_fd_t * sockfd );
	virtual __wasi_errno_t __wasi_sock_bind( __wasi_fd_t fd, __wasi_addr_t * addr );
	virtual __wasi_errno_t __wasi_sock_connect( __wasi_fd_t fd, __wasi_addr_t * addr );
	virtual __wasi_errno_t __wasi_sock_listen( __wasi_fd_t fd, __wasi_size_t backlog );
	virtual __wasi_errno_t __wasi_sock_recv( __wasi_fd_t sock, void * buf, __wasi_size_t buf_len, __wasi_size_t * recv_len );
	virtual __wasi_errno_t __wasi_sock_send( __wasi_fd_t sock, const void * buf, __wasi_size_t buf_len, __wasi_size_t * sent_len );
	virtual __wasi_errno_t __wasi_sock_shutdown( __wasi_fd_t sock );
	virtual __wasi_errno_t __wasi_sched_yield();
};

END_XE_NAMESPACE

#endif // WASI_H8A76CEB4_F41C_491B_A559_32437F29F17A
