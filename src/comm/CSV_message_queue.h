#ifndef __SV_MQ_H__
#define __SV_MQ_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define REQUSTMSG_MAX_LENGTH  1024 * 15
//#define REQUSTMSG_MAX_LENGTH  32
#define IPC_FLAGS             0664
#define IPC_MQ_TYPE           1

struct ur_msgbuf
{
	long mtype;
	long datalen;
	char mchar[REQUSTMSG_MAX_LENGTH];
};

class CSVMessageQueue
{
public:
	CSVMessageQueue();
	CSVMessageQueue(key_t key, int oflag);

	virtual ~CSVMessageQueue();

	void init(key_t key, int oflag);
	int create();
	int open();
	int close();

	const char * get_err_info() const {return _error_text;}

public:
	int send(ur_msgbuf& msg, size_t msgsize, int mflags = 0);
	int recv(ur_msgbuf& msg, size_t msgsize, long mtype = 0, int mflags = 0);

public:
	int remove();
	int control(int cmd, struct msqid_ds* ds = 0);
	int get_id() const {return this->_msgid;}
	void set_id(int msgid) {this->_msgid = msgid;}

private:
	key_t _key;
	int _oflag;
	int _msgid;
	bool _is_init;
	char _error_text[256];
};

#endif
