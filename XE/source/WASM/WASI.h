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

class XE_API WASI : public XE::EnableSharedFromThis< WASI >
{
	OBJECT( WASI )

public:
	WASI() = default;

	virtual ~WASI() = default;

public:
	using device_t = XE::uint64;
	using dircookie_t = XE::uint64;
	using dirnamlen_t = XE::uint32;
	using exitcode_t = XE::uint32;
	using fd_t = XE::uint32;
	using filedelta_t = XE::int64;
	using filesize_t = XE::uint64;
	using inode_t = XE::uint64;
	using linkcount_t = XE::uint64;
	using siflags_t = XE::uint16;
	using size_t = XE::uint32;
	using sock_d_t = fd_t;
	using timestamp_t = XE::uint64;
	using userdata_t = XE::uint64;
	using ip_port_t = XE::uint16;

public:
	enum address_family_t : XE::uint8
	{
		ADDRESS_FAMILY_INET4 = 0,
		ADDRESS_FAMILY_INET6 = 1,
	};
	enum advice_t : XE::uint8
	{
		ADVICE_NORMAL = 0,
		ADVICE_SEQUENTIAL = 1,
		ADVICE_RANDOM = 2,
		ADVICE_WILLNEED = 3,
		ADVICE_DONTNEED = 4,
		ADVICE_NOREUSE = 5,
	};
	enum aiflags_t : XE::uint16
	{
		AIFLAGS_AI_PASSIVE = 1ULL << 0,
		AIFLAGS_AI_CANONNAME = 1ULL << 1,
		AIFLAGS_AI_NUMERICHOST = 1ULL << 2,
		AIFLAGS_AI_NUMERICSERV = 1ULL << 3,
		AIFLAGS_AI_V4MAPPED = 1ULL << 4,
		AIFLAGS_AI_ALL = 1ULL << 5,
		AIFLAGS_AI_ADDRCONFIG = 1ULL << 6,
	};
	enum clockid_t : XE::uint32
	{
		CLOCKID_REALTIME = 0,
		CLOCKID_MONOTONIC = 1,
		CLOCKID_PROCESS_CPUTIME_ID = 2,
		CLOCKID_THREAD_CPUTIME_ID = 3,
	};
	enum errno_t : XE::uint16
	{
		ERRNO_SUCCESS = 0,
		ERRNO_2BIG = 1,
		ERRNO_ACCES = 2,
		ERRNO_ADDRINUSE = 3,
		ERRNO_ADDRNOTAVAIL = 4,
		ERRNO_AFNOSUPPORT = 5,
		ERRNO_AGAIN = 6,
		ERRNO_ALREADY = 7,
		ERRNO_BADF = 8,
		ERRNO_BADMSG = 9,
		ERRNO_BUSY = 10,
		ERRNO_CANCELED = 11,
		ERRNO_CHILD = 12,
		ERRNO_CONNABORTED = 13,
		ERRNO_CONNREFUSED = 14,
		ERRNO_CONNRESET = 15,
		ERRNO_DEADLK = 16,
		ERRNO_DESTADDRREQ = 17,
		ERRNO_DOM = 18,
		ERRNO_DQUOT = 19,
		ERRNO_EXIST = 20,
		ERRNO_FAULT = 21,
		ERRNO_FBIG = 22,
		ERRNO_HOSTUNREACH = 23,
		ERRNO_IDRM = 24,
		ERRNO_ILSEQ = 25,
		ERRNO_INPROGRESS = 26,
		ERRNO_INTR = 27,
		ERRNO_INVAL = 28,
		ERRNO_IO = 29,
		ERRNO_ISCONN = 30,
		ERRNO_ISDIR = 31,
		ERRNO_LOOP = 32,
		ERRNO_MFILE = 33,
		ERRNO_MLINK = 34,
		ERRNO_MSGSIZE = 35,
		ERRNO_MULTIHOP = 36,
		ERRNO_NAMETOOLONG = 37,
		ERRNO_NETDOWN = 38,
		ERRNO_NETRESET = 39,
		ERRNO_NETUNREACH = 40,
		ERRNO_NFILE = 41,
		ERRNO_NOBUFS = 42,
		ERRNO_NODEV = 43,
		ERRNO_NOENT = 44,
		ERRNO_NOEXEC = 45,
		ERRNO_NOLCK = 46,
		ERRNO_NOLINK = 47,
		ERRNO_NOMEM = 48,
		ERRNO_NOMSG = 49,
		ERRNO_NOPROTOOPT = 50,
		ERRNO_NOSPC = 51,
		ERRNO_NOSYS = 52,
		ERRNO_NOTCONN = 53,
		ERRNO_NOTDIR = 54,
		ERRNO_NOTEMPTY = 55,
		ERRNO_NOTRECOVERABLE = 56,
		ERRNO_NOTSOCK = 57,
		ERRNO_NOTSUP = 58,
		ERRNO_NOTTY = 59,
		ERRNO_NXIO = 60,
		ERRNO_OVERFLOW = 61,
		ERRNO_OWNERDEAD = 62,
		ERRNO_PERM = 63,
		ERRNO_PIPE = 64,
		ERRNO_PROTO = 65,
		ERRNO_PROTONOSUPPORT = 66,
		ERRNO_PROTOTYPE = 67,
		ERRNO_RANGE = 68,
		ERRNO_ROFS = 69,
		ERRNO_SPIPE = 70,
		ERRNO_SRCH = 71,
		ERRNO_STALE = 72,
		ERRNO_TIMEDOUT = 73,
		ERRNO_TXTBSY = 74,
		ERRNO_XDEV = 75,
		ERRNO_NOTCAPABLE = 76,
		ERRNO_AIADDRFAMILY = 77,
		ERRNO_AIAGAIN = 78,
		ERRNO_AIBADFLAG = 79,
		ERRNO_AIFAIL = 80,
		ERRNO_AIFAMILY = 81,
		ERRNO_AIMEMORY = 82,
		ERRNO_AINODATA = 83,
		ERRNO_AINONAME = 84,
		ERRNO_AISERVICE = 85,
		ERRNO_AISOCKTYPE = 86,
		ERRNO_AISYSTEM = 87,
	};
	enum eventtype_t : XE::uint8
	{
		EVENTTYPE_CLOCK = 0,
		EVENTTYPE_FD_READ = 1,
		EVENTTYPE_FD_WRITE = 2,
	};
	enum eventrwflags_t : XE::uint16
	{
		EVENTRWFLAGS_FD_READWRITE_HANGUP = 1ULL << 0,
	};
	enum fdflags_t : XE::uint16
	{
		FDFLAGS_APPEND = 1ULL << 0,
		FDFLAGS_DSYNC = 1ULL << 1,
		FDFLAGS_NONBLOCK = 1ULL << 2,
		FDFLAGS_RSYNC = 1ULL << 3,
		FDFLAGS_SYNC = 1ULL << 4,
	};
	enum filetype_t : XE::uint8
	{
		FILETYPE_UNKNOWN = 0,
		FILETYPE_BLOCK_DEVICE = 1,
		FILETYPE_CHARACTER_DEVICE = 2,
		FILETYPE_DIRECTORY = 3,
		FILETYPE_REGULAR_FILE = 4,
		FILETYPE_SOCKET_DGRAM = 5,
		FILETYPE_SOCKET_STREAM = 6,
		FILETYPE_SYMBOLIC_LINK = 7,
	};
	enum fstflags_t : XE::uint16
	{
		FSTFLAGS_ATIM = 1ULL << 0,
		FSTFLAGS_ATIM_NOW = 1ULL << 1,
		FSTFLAGS_MTIM = 1ULL << 2,
		FSTFLAGS_MTIM_NOW = 1ULL << 3,
	};
	enum lookupflags_t : XE::uint32
	{
		LOOKUPFLAGS_SYMLINK_FOLLOW = 1ULL << 0,
	};
	enum oflags_t : XE::uint16
	{
		OFLAGS_CREAT = 1ULL << 0,
		OFLAGS_DIRECTORY = 1ULL << 1,
		OFLAGS_EXCL = 1ULL << 2,
		OFLAGS_TRUNC = 1ULL << 3,
	};
	enum preopentype_t : XE::uint8
	{
		PREOPENTYPE_DIR = 0,
	};
	enum protocol_t : XE::uint8
	{
		PROTOCOL_IPPROTO_TCP = 0,
		PROTOCOL_IPPROTO_UDP = 1,
	};
	enum riflags_t : XE::uint16
	{
		RIFLAGS_RECV_PEEK = 1ULL << 0,
		RIFLAGS_RECV_WAITALL = 1ULL << 1,
	};
	enum rights_t : XE::uint64
	{
		RIGHTS_FD_DATASYNC = 1ULL << 0,
		RIGHTS_FD_READ = 1ULL << 1,
		RIGHTS_FD_SEEK = 1ULL << 2,
		RIGHTS_FD_FDSTAT_SET_FLAGS = 1ULL << 3,
		RIGHTS_FD_SYNC = 1ULL << 4,
		RIGHTS_FD_TELL = 1ULL << 5,
		RIGHTS_FD_WRITE = 1ULL << 6,
		RIGHTS_FD_ADVISE = 1ULL << 7,
		RIGHTS_FD_ALLOCATE = 1ULL << 8,
		RIGHTS_PATH_CREATE_DIRECTORY = 1ULL << 9,
		RIGHTS_PATH_CREATE_FILE = 1ULL << 10,
		RIGHTS_PATH_LINK_SOURCE = 1ULL << 11,
		RIGHTS_PATH_LINK_TARGET = 1ULL << 12,
		RIGHTS_PATH_OPEN = 1ULL << 13,
		RIGHTS_FD_READDIR = 1ULL << 14,
		RIGHTS_PATH_READLINK = 1ULL << 15,
		RIGHTS_PATH_RENAME_SOURCE = 1ULL << 16,
		RIGHTS_PATH_RENAME_TARGET = 1ULL << 17,
		RIGHTS_PATH_FILESTAT_GET = 1ULL << 18,
		RIGHTS_PATH_FILESTAT_SET_SIZE = 1ULL << 19,
		RIGHTS_PATH_FILESTAT_SET_TIMES = 1ULL << 20,
		RIGHTS_FD_FILESTAT_GET = 1ULL << 21,
		RIGHTS_FD_FILESTAT_SET_SIZE = 1ULL << 22,
		RIGHTS_FD_FILESTAT_SET_TIMES = 1ULL << 23,
		RIGHTS_PATH_SYMLINK = 1ULL << 24,
		RIGHTS_PATH_REMOVE_DIRECTORY = 1ULL << 25,
		RIGHTS_PATH_UNLINK_FILE = 1ULL << 26,
		RIGHTS_POLL_FD_READWRITE = 1ULL << 27,
		RIGHTS_SOCK_SHUTDOWN = 1ULL << 28,
		RIGHTS_SOCK_OPEN = 1ULL << 29,
		RIGHTS_SOCK_CLOSE = 1ULL << 30,
		RIGHTS_SOCK_BIND = 1ULL << 31,
		RIGHTS_SOCK_RECV = 1ULL << 32,
		RIGHTS_SOCK_RECV_FROM = 1ULL << 33,
		RIGHTS_SOCK_SEND = 1ULL << 34,
		RIGHTS_SOCK_SEND_TO = 1ULL << 35,
	};
	enum roflags_t : XE::uint16
	{
		ROFLAGS_RECV_DATA_TRUNCATED = 1ULL << 0,
	};
	enum sdflags_t : XE::uint8
	{
		SDFLAGS_RD = 1ULL << 0,
		SDFLAGS_WR = 1ULL << 1,
	};
	enum signal_t : XE::uint8
	{
		SIGNAL_NONE = 0,
		SIGNAL_HUP = 1,
		SIGNAL_INT = 2,
		SIGNAL_QUIT = 3,
		SIGNAL_ILL = 4,
		SIGNAL_TRAP = 5,
		SIGNAL_ABRT = 6,
		SIGNAL_BUS = 7,
		SIGNAL_FPE = 8,
		SIGNAL_KILL = 9,
		SIGNAL_USR1 = 10,
		SIGNAL_SEGV = 11,
		SIGNAL_USR2 = 12,
		SIGNAL_PIPE = 13,
		SIGNAL_ALRM = 14,
		SIGNAL_TERM = 15,
		SIGNAL_CHLD = 16,
		SIGNAL_CONT = 17,
		SIGNAL_STOP = 18,
		SIGNAL_TSTP = 19,
		SIGNAL_TTIN = 20,
		SIGNAL_TTOU = 21,
		SIGNAL_URG = 22,
		SIGNAL_XCPU = 23,
		SIGNAL_XFSZ = 24,
		SIGNAL_VTALRM = 25,
		SIGNAL_PROF = 26,
		SIGNAL_WINCH = 27,
		SIGNAL_POLL = 28,
		SIGNAL_PWR = 29,
		SIGNAL_SYS = 30,
	};
	enum sock_opt_level_t : XE::uint32
	{
		SOCK_OPT_LEVEL_SOL_SOCKET = 0,
	};
	enum sock_opt_so_t : XE::uint32
	{
		SOCK_OPT_SO_REUSEADDR = 0,
		SOCK_OPT_SO_TYPE = 1,
		SOCK_OPT_SO_ERROR = 2,
		SOCK_OPT_SO_DONTROUTE = 3,
		SOCK_OPT_SO_BROADCAST = 4,
		SOCK_OPT_SO_SNDBUF = 5,
		SOCK_OPT_SO_RCVBUF = 6,
		SOCK_OPT_SO_KEEPALIVE = 7,
		SOCK_OPT_SO_OOBINLINE = 8,
		SOCK_OPT_SO_LINGER = 9,
		SOCK_OPT_SO_RCVLOWAT = 10,
		SOCK_OPT_SO_RCVTIMEO = 11,
		SOCK_OPT_SO_SNDTIMEO = 12,
		SOCK_OPT_SO_ACCEPTCONN = 13,
	};
	enum sock_type_t : XE::uint8
	{
		SOCK_TYPE_SOCK_DGRAM = 0,
		SOCK_TYPE_SOCK_STREAM = 1,
	};
	enum subclockflags_t : XE::uint16
	{
		SUBCLOCKFLAGS_SUBSCRIPTION_CLOCK_ABSTIME = 1ULL << 0,
	};
	enum whence_t : XE::uint8
	{
		WHENCE_SET = 0,
		WHENCE_CUR = 1,
		WHENCE_END = 2,
	};

public:
	struct dirent_t
	{
		dircookie_t d_next;
		inode_t d_ino;
		dirnamlen_t d_namlen;
		filetype_t d_type;
	};
	struct event_t
	{
		userdata_t userdata;
		errno_t error;
		eventtype_t type;
		XE::uint8 paddings[5];
		union event_u
		{
			struct event_u_fd_readwrite_t
			{
				filesize_t nbytes;
				eventrwflags_t flags;
				XE::uint8 paddings[6];
			} fd_readwrite;
		} u;
	};
	struct prestat_t
	{
		preopentype_t pr_type;
		union prestat_u
		{
			struct prestat_u_dir_t
			{
				size_t pr_name_len;
			} dir;
		} u;
	};
	struct fdstat_t
	{
		filetype_t fs_filetype;
		fdflags_t fs_flags;
		XE::uint8 paddings[4];
		rights_t fs_rights_base;
		rights_t fs_rights_inheriting;
	};
	struct filestat_t
	{
		device_t st_dev;
		inode_t st_ino;
		filetype_t st_filetype;
		linkcount_t st_nlink;
		filesize_t st_size;
		timestamp_t st_atim;
		timestamp_t st_mtim;
		timestamp_t st_ctim;
	};
	struct ciovec_t
	{
		const void * buf;
		size_t buf_len;
	};
	struct iovec_t
	{
		void * buf;
		size_t buf_len;
	};
	struct subscription_clock_t
	{
		clockid_t clock_id;
		XE::uint8 paddings1[4];
		timestamp_t timeout;
		timestamp_t precision;
		subclockflags_t flags;
		XE::uint8 paddings2[4];
	};
	struct subscription_fd_readwrite_t
	{
		fd_t fd;
	};
	union  subscription_u_u_t
	{
		subscription_clock_t clock;
		subscription_fd_readwrite_t fd_readwrite;
	};
	struct subscription_u_t
	{
		eventtype_t type;
		subscription_u_u_t u;
	};
	struct subscription_t
	{
		userdata_t userdata;
		subscription_u_t u;
	};
	struct addr_ip4
	{
		XE::uint8 n0;
		XE::uint8 n1;
		XE::uint8 n2;
		XE::uint8 n3;
	};
	struct addr_ip4_port_t
	{
		addr_ip4 addr;
		ip_port_t port;
	};
	struct addr_ip6
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
	struct addr_ip6_port_t
	{
		addr_ip6 addr;
		ip_port_t port;
	};
	struct addr_t
	{
		protocol_t kind;
		union
		{
			addr_ip4_port_t ip4;
			addr_ip6_port_t ip6;
		} addr;
	};

public:
	virtual WASI::errno_t args_get( char ** argv, char * argv_buf );
	virtual WASI::errno_t args_sizes_get( WASI::size_t * argc, WASI::size_t * argv_buf_size );
	virtual WASI::errno_t clock_res_get( WASI::clockid_t clock_id, WASI::timestamp_t * resolution );
	virtual WASI::errno_t clock_time_get( WASI::clockid_t clock_id, WASI::timestamp_t precision, WASI::timestamp_t * time );
	virtual WASI::errno_t environ_get( char ** environ, char * environ_buf );
	virtual WASI::errno_t environ_sizes_get( WASI::size_t * environ_count, WASI::size_t * environ_buf_size );
	virtual WASI::errno_t fd_prestat_get( WASI::fd_t fd, WASI::prestat_t * buf );
	virtual WASI::errno_t fd_prestat_dir_name( WASI::fd_t fd, char * path, WASI::size_t path_len );
	virtual WASI::errno_t fd_close( WASI::fd_t fd );
	virtual WASI::errno_t fd_datasync( WASI::fd_t fd );
	virtual WASI::errno_t fd_pread( WASI::fd_t fd, const WASI::iovec_t * iovs, WASI::size_t iovs_len, WASI::filesize_t offset, WASI::size_t * nread );
	virtual WASI::errno_t fd_pwrite( WASI::fd_t fd, const WASI::ciovec_t * iovs, WASI::size_t iovs_len, WASI::filesize_t offset, WASI::size_t * nwritten );
	virtual WASI::errno_t fd_read( WASI::fd_t fd, const WASI::iovec_t * iovs, WASI::size_t iovs_len, WASI::size_t * nread );
	virtual WASI::errno_t fd_renumber( WASI::fd_t from, WASI::fd_t to );
	virtual WASI::errno_t fd_seek( WASI::fd_t fd, WASI::filedelta_t offset, WASI::whence_t whence, WASI::filesize_t * newoffset );
	virtual WASI::errno_t fd_tell( WASI::fd_t fd, WASI::filesize_t * newoffset );
	virtual WASI::errno_t fd_fdstat_get( WASI::fd_t fd, WASI::fdstat_t * buf );
	virtual WASI::errno_t fd_fdstat_set_flags( WASI::fd_t fd, WASI::fdflags_t flags );
	virtual WASI::errno_t fd_fdstat_set_rights( WASI::fd_t fd, WASI::rights_t fs_rights_base, WASI::rights_t fs_rights_inheriting );
	virtual WASI::errno_t fd_sync( WASI::fd_t fd );
	virtual WASI::errno_t fd_write( WASI::fd_t fd, const WASI::ciovec_t * iovs, WASI::size_t iovs_len, WASI::size_t * nwritten );
	virtual WASI::errno_t fd_advise( WASI::fd_t fd, WASI::filesize_t offset, WASI::filesize_t len, WASI::advice_t advice );
	virtual WASI::errno_t fd_allocate( WASI::fd_t fd, WASI::filesize_t offset, WASI::filesize_t len );
	virtual WASI::errno_t path_create_directory( WASI::fd_t fd, const char * path, WASI::size_t path_len );
	virtual WASI::errno_t path_link( WASI::fd_t old_fd, WASI::lookupflags_t old_flags, const char * old_path, WASI::size_t old_path_len, WASI::fd_t new_fd, const char * new_path, WASI::size_t new_path_len );
	virtual WASI::errno_t path_open( WASI::fd_t dirfd, WASI::lookupflags_t dirflags, const char * path, WASI::size_t path_len, WASI::oflags_t oflags, WASI::rights_t fs_rights_base, WASI::rights_t fs_rights_inheriting, WASI::fdflags_t fs_flags, WASI::fd_t * fd );
	virtual WASI::errno_t fd_readdir( WASI::fd_t fd, void * buf, WASI::size_t buf_len, WASI::dircookie_t cookie, WASI::size_t * bufused );
	virtual WASI::errno_t path_readlink( WASI::fd_t fd, const char * path, WASI::size_t path_len, char * buf, WASI::size_t buf_len, WASI::size_t * bufused );
	virtual WASI::errno_t path_rename( WASI::fd_t old_fd, const char * old_path, WASI::size_t old_path_len, WASI::fd_t new_fd, const char * new_path, WASI::size_t new_path_len );
	virtual WASI::errno_t fd_filestat_get( WASI::fd_t fd, WASI::filestat_t * buf );
	virtual WASI::errno_t fd_filestat_set_times( WASI::fd_t fd, WASI::timestamp_t st_atim, WASI::timestamp_t st_mtim, WASI::fstflags_t fstflags );
	virtual WASI::errno_t fd_filestat_set_size( WASI::fd_t fd, WASI::filesize_t st_size );
	virtual WASI::errno_t path_filestat_get( WASI::fd_t fd, WASI::lookupflags_t flags, const char * path, WASI::size_t path_len, WASI::filestat_t * buf );
	virtual WASI::errno_t path_filestat_set_times( WASI::fd_t fd, WASI::lookupflags_t flags, const char * path, WASI::size_t path_len, WASI::timestamp_t st_atim, WASI::timestamp_t st_mtim, WASI::fstflags_t fstflags );
	virtual WASI::errno_t path_symlink( const char * old_path, WASI::size_t old_path_len, WASI::fd_t fd, const char * new_path, WASI::size_t new_path_len );
	virtual WASI::errno_t path_unlink_file( WASI::fd_t fd, const char * path, WASI::size_t path_len );
	virtual WASI::errno_t path_remove_directory( WASI::fd_t fd, const char * path, WASI::size_t path_len );
	virtual WASI::errno_t poll_oneoff( const WASI::subscription_t * in, WASI::event_t * out, WASI::size_t nsubscriptions, WASI::size_t * nevents );
	virtual WASI::errno_t proc_raise( WASI::signal_t sig );
	virtual WASI::errno_t random_get( void * buf, WASI::size_t buf_len );
	virtual WASI::errno_t sock_accept( WASI::fd_t fd, WASI::fd_t * fd_new );
	virtual WASI::errno_t sock_addr_local( WASI::fd_t fd, XE::uint8 * buf, WASI::size_t buf_len );
	virtual WASI::errno_t sock_addr_remote( WASI::fd_t fd, XE::uint8 * buf, WASI::size_t buf_len );
	virtual WASI::errno_t sock_open( WASI::fd_t poolfd, WASI::address_family_t af, WASI::sock_type_t socktype, WASI::fd_t * sockfd );
	virtual WASI::errno_t sock_bind( WASI::fd_t fd, WASI::addr_t * addr );
	virtual WASI::errno_t sock_connect( WASI::fd_t fd, WASI::addr_t * addr );
	virtual WASI::errno_t sock_listen( WASI::fd_t fd, WASI::size_t backlog );
	virtual WASI::errno_t sock_recv( WASI::fd_t sock, void * buf, WASI::size_t buf_len, WASI::size_t * recv_len );
	virtual WASI::errno_t sock_send( WASI::fd_t sock, const void * buf, WASI::size_t buf_len, WASI::size_t * sent_len );
	virtual WASI::errno_t sock_shutdown( WASI::fd_t sock );
	virtual WASI::errno_t sched_yield();
};

END_XE_NAMESPACE

#endif // WASI_H8A76CEB4_F41C_491B_A559_32437F29F17A
