

API

https://github.com/google/glog/blob/master/src/glog/logging.h.in


``` cpp
// logging.h


// We use the preprocessor's merging operator, "##", so that, e.g.,
// LOG(INFO) becomes the token GOOGLE_LOG_INFO.  There's some funny
// subtle difference between ostream member streaming functions (e.g.,
// ostream::operator<<(int) and ostream non-member streaming functions
// (e.g., ::operator<<(ostream&, string&): it turns out that it's
// impossible to stream something like a string directly to an unnamed
// ostream. We employ a neat hack by calling the stream() member
// function of LogMessage which seems to avoid the problem.
#define LOG(severity) COMPACT_GOOGLE_LOG_ ## severity.stream()         
#define SYSLOG(severity) SYSLOG_ ## severity(0).stream()


// Log messages below the GOOGLE_STRIP_LOG level will be compiled away for
// security reasons. See LOG(severtiy) below.

// A few definitions of macros that don't generate much code.  Since
// LOG(INFO) and its ilk are used all over our code, it's
// better to have compact code for these operations.

#if GOOGLE_STRIP_LOG == 0
#define COMPACT_GOOGLE_LOG_INFO google::LogMessage( \                      
__FILE__, __LINE__)
#define LOG_TO_STRING_INFO(message) google::LogMessage( \
__FILE__, __LINE__, google::GLOG_INFO, message)
#else
#define COMPACT_GOOGLE_LOG_INFO google::NullStream()
#define LOG_TO_STRING_INFO(message) google::NullStream()
#endif

// A class for which we define operator<<, which does nothing.
class GOOGLE_GLOG_DLL_DECL NullStream : public LogMessage::LogStream {
public:
  // Initialize the LogStream so the messages can be written somewhere
  // (they'll never be actually displayed). This will be needed if a
  // NullStream& is implicitly converted to LogStream&, in which case
  // the overloaded NullStream::operator<< will not be invoked.
  NullStream() : LogMessage::LogStream(message_buffer_, 1, 0) { }
  NullStream(const char* /*file*/, int /*line*/,
             const CheckOpString& /*result*/) :
  LogMessage::LogStream(message_buffer_, 1, 0) { }
  NullStream &stream() { return *this; }
private:
  // A very short buffer for messages (which we discard anyway). This
  // will be needed if NullStream& converted to LogStream& (e.g. as a
  // result of a conditional expression).
  char message_buffer_[2];
};

```

``` cpp
// This class more or less represents a particular log message.  You
// create an instance of LogMessage and then stream stuff to it.
// When you finish streaming to it, ~LogMessage is called and the
// full message gets streamed to the appropriate destination.
//
// You shouldn't actually use LogMessage's constructor to log things,
// though.  You should use the LOG() macro (and variants thereof)
// above.
class GOOGLE_GLOG_DLL_DECL LogMessage {
public:
  enum {
    // Passing kNoLogPrefix for the line number disables the
    // log-message prefix. Useful for using the LogMessage
    // infrastructure as a printing utility. See also the --log_prefix
    // flag for controlling the log-message prefix on an
    // application-wide basis.
    kNoLogPrefix = -1
  };

   // LogStream inherit from non-DLL-exported class (std::ostrstream)
   // and VC++ produces a warning for this situation.
   // However, MSDN says "C4275 can be ignored in Microsoft Visual C++
   // 2005 if you are deriving from a type in the Standard C++ Library"
   // http://msdn.microsoft.com/en-us/library/3tdb471s(VS.80).aspx
   // Let's just ignore the warning.
 #ifdef _MSC_VER
 # pragma warning(push)
 # pragma warning(disable: 4275)
 #endif
  class GOOGLE_GLOG_DLL_DECL LogStream : public std::ostream {
 #ifdef _MSC_VER
 # pragma warning(pop)
 #endif
  public:
   LogStream(char *buf, int len, int ctr)
   : std::ostream(NULL),
   streambuf_(buf, len),
   ctr_(ctr),
   self_(this) {
     rdbuf(&streambuf_);
   }
   
   int ctr() const { return ctr_; }
   void set_ctr(int ctr) { ctr_ = ctr; }
   LogStream* self() const { return self_; }
   
     // Legacy std::streambuf methods.
   size_t pcount() const { return streambuf_.pcount(); }
   char* pbase() const { return streambuf_.pbase(); }
   char* str() const { return pbase(); }
   
 private:
   LogStream(const LogStream&);
   LogStream& operator=(const LogStream&);
   base_logging::LogStreamBuf streambuf_;    

         int ctr_;  // Counter hack (for the LOG_EVERY_X() macro)
    LogStream *self_;  // Consistency check hack
  };
  
  // 写日志接口
  // SendToLog

};
```

主要实现接口

``` cpp
// logging.cc

// L >= log_mutex (callers must hold the log_mutex).
void LogMessage::SendToLog() EXCLUSIVE_LOCKS_REQUIRED(log_mutex) {
  static bool already_warned_before_initgoogle = false;

  log_mutex.AssertHeld();

  RAW_DCHECK(data_->num_chars_to_log_ > 0 &&
   data_->message_text_[data_->num_chars_to_log_-1] == '\n', "");

  // Messages of a given severity get logged to lower severity logs, too

  if (!already_warned_before_initgoogle && !IsGoogleLoggingInitialized()) {
    const char w[] = "WARNING: Logging before InitGoogleLogging() is "
    "written to STDERR\n";
    WriteToStderr(w, strlen(w));
    already_warned_before_initgoogle = true;
  }

// global flag: never log to file if set.  Also -- don't log to a
// file if we haven't parsed the command line flags to get the
// program name.
  if (FLAGS_logtostderr || !IsGoogleLoggingInitialized()) {
    ColoredWriteToStderr(data_->severity_, 
     data_->message_text_, data_->num_chars_to_log_);

  // this could be protected by a flag if necessary.
    LogDestination::LogToSinks(data_->severity_,
     data_->fullname_, data_->basename_,
     data_->line_, &data_->tm_time_,
     data_->message_text_ + data_->num_prefix_chars_,
     (data_->num_chars_to_log_ -
      data_->num_prefix_chars_ - 1));
  } else {
    // log this message to all log files of severity <= severity_
    LogDestination::LogToAllLogfiles(data_->severity_, data_->timestamp_,
     data_->message_text_,
     data_->num_chars_to_log_);

    LogDestination::MaybeLogToStderr(data_->severity_, data_->message_text_,
     data_->num_chars_to_log_);
    LogDestination::MaybeLogToEmail(data_->severity_, data_->message_text_,
      data_->num_chars_to_log_);
    LogDestination::LogToSinks(data_->severity_,
     data_->fullname_, data_->basename_,
     data_->line_, &data_->tm_time_,
     data_->message_text_ + data_->num_prefix_chars_,
     (data_->num_chars_to_log_
      - data_->num_prefix_chars_ - 1));
    // NOTE: -1 removes trailing \n
  }

// If we log a FATAL message, flush all the log destinations, then toss
// a signal for others to catch. We leave the logs in a state that
// someone else can use them (as long as they flush afterwards)
  if (data_->severity_ == GLOG_FATAL && exit_on_dfatal) {
    if (data_->first_fatal_) {
    // Store crash information so that it is accessible from within signal
    // handlers that may be invoked later.
      RecordCrashReason(&crash_reason);
      SetCrashReason(&crash_reason);

    // Store shortened fatal message for other logs and GWQ status
      const int copy = min<int>(data_->num_chars_to_log_,
        sizeof(fatal_message)-1);
      memcpy(fatal_message, data_->message_text_, copy);
      fatal_message[copy] = '\0';
      fatal_time = data_->timestamp_;
    }

    if (!FLAGS_logtostderr) {
      for (int i = 0; i < NUM_SEVERITIES; ++i) {
        if ( LogDestination::log_destinations_[i] )
          LogDestination::log_destinations_[i]->logger_->Write(true, 0, "", 0);
      }
    }

  // release the lock that our caller (directly or indirectly)
  // LogMessage::~LogMessage() grabbed so that signal handlers
  // can use the logging facility. Alternately, we could add
  // an entire unsafe logging interface to bypass locking
  // for signal handlers but this seems simpler.
    log_mutex.Unlock();
    LogDestination::WaitForSinks(data_);

    const char* message = "*** Check failure stack trace: ***\n";
    if (write(STDERR_FILENO, message, strlen(message)) < 0) {
    // Ignore errors.
    }
    Fail();
  }
}
```

``` cpp
inline void LogDestination::LogToAllLogfiles(LogSeverity severity,
                                              time_t timestamp,
                                              const char* message,
                                              size_t len) {
 
   if ( FLAGS_logtostderr ) {           // global flag: never log to file
     ColoredWriteToStderr(severity, message, len);
   } else {
     for (int i = severity; i >= 0; --i)
       LogDestination::MaybeLogToLogfile(i, timestamp, message, len);
   }
}
```

``` cpp
inline void LogDestination::MaybeLogToLogfile(LogSeverity severity,
                                              time_t timestamp,
                          const char* message,
                          size_t len) {
  const bool should_flush = severity > FLAGS_logbuflevel;
  LogDestination* destination = log_destination(severity);
  destination->logger_->Write(should_flush, timestamp, message, len);
}


namespace {

// Encapsulates all file-system related state
  class LogFileObject : public base::Logger {
  public:
    LogFileObject(LogSeverity severity, const char* base_filename);
    ~LogFileObject();

  virtual void Write(bool force_flush, // Should we force a flush here?
                     time_t timestamp,  // Timestamp for this entry
                     const char* message,
                     int message_len);

  // ...

};
```

写文件

``` cpp
void LogFileObject::Write(bool force_flush,
                          time_t timestamp,
                          const char* message,
                          int message_len) {
  MutexLock l(&lock_);

  // We don't log if the base_name_ is "" (which means "don't write")
  if (base_filename_selected_ && base_filename_.empty()) {
    return;
  }

  if (static_cast<int>(file_length_ >> 20) >= MaxLogSize() ||
      PidHasChanged()) {
    if (file_ != NULL) fclose(file_);
    file_ = NULL;
    file_length_ = bytes_since_flush_ = dropped_mem_length_ = 0;
    rollover_attempt_ = kRolloverAttemptFrequency - 1;
  }

  // If there's no destination file, make one before outputting
  if (file_ == NULL) {
    // Try to rollover the log file every 32 log messages.  The only time
    // this could matter would be when we have trouble creating the log
    // file.  If that happens, we'll lose lots of log messages, of course!
    if (++rollover_attempt_ != kRolloverAttemptFrequency) return;
    rollover_attempt_ = 0;

    struct ::tm tm_time;
    localtime_r(&timestamp, &tm_time);

    // The logfile's filename will have the date/time & pid in it
    ostringstream time_pid_stream;
    time_pid_stream.fill('0');
    time_pid_stream << 1900 + tm_time.tm_year
                    << setw(2) << 1 + tm_time.tm_mon
                    << setw(2) << tm_time.tm_mday
                    << '-'
                    << setw(2) << tm_time.tm_hour
                    << setw(2) << tm_time.tm_min
                    << setw(2) << tm_time.tm_sec
                    << '.'
                    << GetMainThreadPid();
    const string& time_pid_string = time_pid_stream.str();

    if (base_filename_selected_) {
      if (!CreateLogfile(time_pid_string)) {
        perror("Could not create log file");
        fprintf(stderr, "COULD NOT CREATE LOGFILE '%s'!\n",
                time_pid_string.c_str());
        return;
      }
    } else {

      // If no base filename for logs of this severity has been set, use a
      // default base filename of
      // "<program name>.<hostname>.<user name>.log.<severity level>.".  So
      // logfiles will have names like
      // webserver.examplehost.root.log.INFO.19990817-150000.4354, where
      // 19990817 is a date (1999 August 17), 150000 is a time (15:00:00),
      // and 4354 is the pid of the logging process.  The date & time reflect
      // when the file was created for output.
      //
      // Where does the file get put?  Successively try the directories
      // "/tmp", and "."
      string stripped_filename(
        glog_internal_namespace_::ProgramInvocationShortName());
      string hostname;
      GetHostName(&hostname);

      string uidname = MyUserName();
      // We should not call CHECK() here because this function can be
      // called after holding on to log_mutex. We don't want to
      // attempt to hold on to the same mutex, and get into a
      // deadlock. Simply use a name like invalid-user.
      if (uidname.empty()) uidname = "invalid-user";

      stripped_filename = stripped_filename + '.' + hostname + '.'
                          + uidname + ".log."
                          + LogSeverityNames[severity_] + '.';
      // We're going to (potentially) try to put logs in several different dirs
      const vector<string> & log_dirs = GetLoggingDirectories();

      // Go through the list of dirs, and try to create the log file in each
      // until we succeed or run out of options
      bool success = false;
      for (vector<string>::const_iterator dir = log_dirs.begin();
           dir != log_dirs.end();
           ++dir) {
        base_filename_ = *dir + "/" + stripped_filename;
        if ( CreateLogfile(time_pid_string) ) {
          success = true;
          break;
        }
      }
      // If we never succeeded, we have to give up
      if ( success == false ) {
        perror("Could not create logging file");
        fprintf(stderr, "COULD NOT CREATE A LOGGINGFILE %s!",
                time_pid_string.c_str());
        return;
      }
    }

    // Write a header message into the log file
    ostringstream file_header_stream;
    file_header_stream.fill('0');
    file_header_stream << "Log file created at: "
                       << 1900 + tm_time.tm_year << '/'
                       << setw(2) << 1 + tm_time.tm_mon << '/'
                       << setw(2) << tm_time.tm_mday
                       << ' '
                       << setw(2) << tm_time.tm_hour << ':'
                       << setw(2) << tm_time.tm_min << ':'
                       << setw(2) << tm_time.tm_sec << '\n'
                       << "Running on machine: "
                       << LogDestination::hostname() << '\n'
                       << "Log line format: [IWEF]mmdd hh:mm:ss.uuuuuu "
                       << "threadid file:line] msg" << '\n';
    const string& file_header_string = file_header_stream.str();

    const int header_len = file_header_string.size();
    fwrite(file_header_string.data(), 1, header_len, file_);
    file_length_ += header_len;
    bytes_since_flush_ += header_len;
  }

// Write to LOG file
  if ( !stop_writing ) {
    // fwrite() doesn't return an error when the disk is full, for
    // messages that are less than 4096 bytes. When the disk is full,
    // it returns the message length for messages that are less than
    // 4096 bytes. fwrite() returns 4096 for message lengths that are
    // greater than 4096, thereby indicating an error.
    errno = 0;
    fwrite(message, 1, message_len, file_);
    if ( FLAGS_stop_logging_if_full_disk &&
         errno == ENOSPC ) {  // disk full, stop writing to disk
      stop_writing = true;  // until the disk is
      return;
    } else {
      file_length_ += message_len;
      bytes_since_flush_ += message_len;
    }
  } else {
    if ( CycleClock_Now() >= next_flush_time_ )
      stop_writing = false;  // check to see if disk has free space.
    return;  // no need to flush
  }

  // See important msgs *now*.  Also, flush logs at least every 10^6 chars,
  // or every "FLAGS_logbufsecs" seconds.
  if ( force_flush ||
       (bytes_since_flush_ >= 1000000) ||
       (CycleClock_Now() >= next_flush_time_) ) {
    FlushUnlocked();
#ifdef OS_LINUX
    // Only consider files >= 3MiB
    if (FLAGS_drop_log_memory && file_length_ >= (3 << 20)) {
      // Don't evict the most recent 1-2MiB so as not to impact a tailer
      // of the log file and to avoid page rounding issue on linux < 4.7
      uint32 total_drop_length = (file_length_ & ~((1 << 20) - 1)) - (1 << 20);
      uint32 this_drop_length = total_drop_length - dropped_mem_length_;
      if (this_drop_length >= (2 << 20)) {
        // Only advise when >= 2MiB to drop
        posix_fadvise(fileno(file_), dropped_mem_length_, this_drop_length,
                      POSIX_FADV_DONTNEED);
        dropped_mem_length_ = total_drop_length;
      }
    }
#endif
  }
}

```