#ifndef HW_STREAMING_EXCEPTION
#define HW_STREAMING_EXCEPTION

#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>

class StreamingException : public std::runtime_error
{
public:
  StreamingException(const std::string filename = "", 
                     unsigned int line = 0) :
   filename_(filename),
   line_(line),
   std::runtime_error(""),
   ss_(std::auto_ptr<std::stringstream>
       (new std::stringstream()))
  {
  }

  ~StreamingException() throw()
  {
  }

  template <typename T>
  StreamingException & operator << (const T & t)
  {
    (*ss_) << t;
    return *this;
  }

  virtual const char * what() const throw()
  {
    s_ = ss_->str();
    return s_.c_str();
  }

public: // fields
  std::string  filename_;
  unsigned int line_;

private:
  mutable std::auto_ptr<std::stringstream> ss_;
  mutable std::string s_;
};

#endif


