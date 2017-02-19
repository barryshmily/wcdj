#include "CSV_message_queue.h"

CSVMessageQueue::CSVMessageQueue()
:  _key(-1), _oflag(0), _msgid(-1), _is_init(false)
{
	memset(_error_text, 0x0, sizeof(_error_text));
}

CSVMessageQueue::CSVMessageQueue(key_t key, int oflag)
{
	init(key, oflag);
}

void
CSVMessageQueue::init(key_t key, int oflag)
{
	_key           =  key;
	_oflag         =  oflag;
	_msgid         =  -1;
	_is_init       =  true;

	memset(_error_text, 0x0, sizeof(_error_text));
}

CSVMessageQueue::~CSVMessageQueue()
{
	close();
}

int
CSVMessageQueue::create()
{
	if (!_is_init) 
	{
		snprintf(_error_text, sizeof(_error_text), "%s", "CSVMessageQueue not init");
		return -1;
	}

	_msgid = msgget(_key, _oflag | IPC_CREAT | IPC_EXCL);
	if (_msgid < 0) 
	{
		if (errno != EEXIST) 
		{
			snprintf(_error_text, sizeof(_error_text), "msgget: msgid[%d] err[%d:%s]", _msgid, errno, strerror(errno));
			return -1;
		}
		else 
		{
			// MQ has existed
			return 1;
		}
	}

	return 0;
}

int
CSVMessageQueue::open()
{
	if (!_is_init) 
	{
		snprintf(_error_text, sizeof(_error_text), "%s", "CSVMessageQueue not init");
		return -1;
	}

	_msgid = msgget(_key, _oflag);
	if (_msgid < 0) 
	{
		snprintf(_error_text, sizeof(_error_text), "msgget: msgid[%d] err[%d:%s]", _msgid, errno, strerror(errno));
		return -1;
	}

	return 0;
}

int
CSVMessageQueue::close()
{
	return 0;
}

int
CSVMessageQueue::send(ur_msgbuf& msg, size_t msgsize, int mflags)
{
	if (msgsnd(_msgid, (struct msgbuf *) &msg, msgsize, mflags) < 0) 
	{
		snprintf(_error_text, sizeof(_error_text), "msgsnd: msgid[%d] err[%d:%s]", _msgid, errno, strerror(errno));
		return -1;
	}

	return 0;
}

int
CSVMessageQueue::recv(ur_msgbuf& msg, size_t msgsize, long mtype, int mflags)
{
	if (msgrcv(_msgid, (struct msgbuf *) &msg, msgsize, mtype, mflags) < 0) 
	{
		snprintf(_error_text, sizeof(_error_text), "msgrcv: msgid[%d] err[%d:%s]", _msgid, errno, strerror(errno));
		return -1;
	}

	return 0;
}

int
CSVMessageQueue::remove()
{
	return control(IPC_RMID);
}

int
CSVMessageQueue::control(int cmd, struct msqid_ds *ds /*= 0*/)
{
	if (msgctl(_msgid, cmd, (struct msqid_ds *) ds) < 0) 
	{
		snprintf(_error_text, sizeof(_error_text), "msgctl: msgid[%d] err[%d:%s]", _msgid, errno, strerror(errno));
		return -1;
	}

	return 0;
}

